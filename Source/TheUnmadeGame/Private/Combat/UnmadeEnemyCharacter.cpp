#include "UI/UnmadePlayerFacingRules.h"
#include "Combat/UnmadeEncounterPerformanceRules.h"
#include "Combat/UnmadeEncounterInterruptionRules.h"
#include "Save/UnmadeSaveSession.h"
#include "UI/UnmadePlayerPreferences.h"
#include "UI/UnmadeRpgFeedback.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "World/UnmadeCharacterGrayboxRules.h"
#include "Combat/UnmadeTravelingProjectile.h"
#include "Combat/UnmadePresentation.h"
#include "World/UnmadeIdentityRules.h"
#include "Combat/UnmadeCreatureRoutineRules.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadePrototypeHub.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Components/PointLightComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "Sound/SoundBase.h"

AUnmadeEnemyCharacter::AUnmadeEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    Combat = CreateDefaultSubobject<UUnmadeCombatComponent>(TEXT("Combat"));
    TemporaryEnemyVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeEnemyVisual"));
    TemporaryEnemyVisual->SetupAttachment(GetCapsuleComponent());
    TemporaryEnemyVisual->SetRelativeLocation(FVector(0, 0, -2));
    TemporaryEnemyVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded()) TemporaryEnemyVisual->SetStaticMesh(Cube.Object);
    TemporaryEnemyVisual->SetRelativeScale3D(FVector(0.65, 0.65, 1.2));
    EncounterCrest=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CreatureCrest"));
    EncounterCrest->SetupAttachment(GetCapsuleComponent());
    EncounterCrest->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if(Sphere.Succeeded())EncounterCrest->SetStaticMesh(Sphere.Object);
    EncounterLabel=CreateDefaultSubobject<UTextRenderComponent>(TEXT("CreatureName"));
    EncounterLabel->SetupAttachment(GetCapsuleComponent());
    EncounterLabel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EncounterLabel->SetRelativeLocation(FVector(0,0,125));
    EncounterLabel->SetHorizontalAlignment(EHTA_Center);
    EncounterLabel->SetWorldSize(22.f);
    CombatSignal=CreateDefaultSubobject<UTextRenderComponent>(TEXT("CombatTelegraph"));
    CombatSignal->SetupAttachment(GetCapsuleComponent());
    CombatSignal->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CombatSignal->SetRelativeLocation(FVector(0,0,172));
    CombatSignal->SetHorizontalAlignment(EHTA_Center);
    CombatSignal->SetWorldSize(26.f);
    CombatSignal->SetText(FText::GetEmpty());
    TelegraphLight=CreateDefaultSubobject<UPointLightComponent>(TEXT("AttackWarningLight"));
    TelegraphLight->SetupAttachment(GetCapsuleComponent());
    TelegraphLight->SetRelativeLocation(FVector(0,0,80));
    TelegraphLight->SetIntensity(0.f);
    TelegraphLight->SetAttenuationRadius(350.f);
    TelegraphLight->SetLightColor(FLinearColor(1.f,.22f,.09f));
    ElderAimMarker=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SootboundLockedAim"));
    ElderAimMarker->SetupAttachment(GetCapsuleComponent());
    ElderAimMarker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if(Cylinder.Succeeded())ElderAimMarker->SetStaticMesh(Cylinder.Object);
    ElderAimMarker->SetVisibility(false);
}

void AUnmadeEnemyCharacter::ConfigureStyle(UnmadeCore::EnemyStyle NewStyle)
{
    Style = NewStyle;
    TryLoadEnemyRig();
    if (Style == UnmadeCore::EnemyStyle::Watcher)
    {
        Combat->Configure(65.0, 10.0, 2.25);
        TemporaryEnemyVisual->SetRelativeScale3D(FVector(1.0, 1.0, 0.8));
    }
    else
    {
        Combat->Configure(85.0, 14.0, 1.4);
        TemporaryEnemyVisual->SetRelativeScale3D(FVector(0.65, 0.65, 1.2));
    }
}

bool AUnmadeEnemyCharacter::CanBeTargeted()const
{
    return IsValid(Combat)&&!Combat->IsDefeated()&&!bCinderholdQuieted&&
        (ExpeditionArc<0||(CachedRealityHub.IsValid()&&!(bWingObserver?CachedRealityHub->WingObserverInactive():bWingGuardian?CachedRealityHub->WingGuardianInactive(ExpeditionArc):CachedRealityHub->ExpeditionEnemyInactive(ExpeditionArc,ExpeditionSlot))));
}
void AUnmadeEnemyCharacter::ConfigureCinderholdElder()
{
    bCinderholdElder=true;
    TerritoryHome=GetActorLocation();
}
void AUnmadeEnemyCharacter::ConfigureCreature(UnmadeCore::CreatureKind Kind,bool bChampion)
{
    bElderEncounter=bChampion;
    Species=Kind;
    Profile=UnmadeCore::CreatureStats(Kind,bChampion);
    ConfigureStyle(Profile.style);
    Combat->Configure(Profile.health,Profile.damage,Profile.cooldown);
    bSpeciesConfigured=true;
    // Species silhouettes are a fallback only; imported skeletal performances
    // continue to own actual animation and combat collision/capsule.
    const auto& Visual=UnmadeCore::CreatureGraybox(Kind);
    if(!bSkeletalRigLoaded&&IsValid(TemporaryEnemyVisual)&&IsValid(EncounterCrest)){
        const FString BodyPath=FString::Printf(TEXT("/Engine/BasicShapes/%s.%s"),
            UTF8_TO_TCHAR(Visual.bodyShape),UTF8_TO_TCHAR(Visual.bodyShape));
        const FString CrestPath=FString::Printf(TEXT("/Engine/BasicShapes/%s.%s"),
            UTF8_TO_TCHAR(Visual.crestShape),UTF8_TO_TCHAR(Visual.crestShape));
        if(UStaticMesh* Shape=LoadObject<UStaticMesh>(nullptr,*BodyPath))
            TemporaryEnemyVisual->SetStaticMesh(Shape);
        if(UStaticMesh* Shape=LoadObject<UStaticMesh>(nullptr,*CrestPath))
            EncounterCrest->SetStaticMesh(Shape);
        const float Scale=bChampion?1.13f:1.f;
        TemporaryEnemyVisual->SetRelativeScale3D(
            FVector(Visual.sx,Visual.sy,Visual.sz)*Scale);
        EncounterCrest->SetRelativeLocation(FVector(18,0,Visual.crestHeight));
        EncounterCrest->SetRelativeScale3D(FVector(
            Visual.crestScale,Visual.crestScale,Visual.crestScale)*Scale);
        EncounterCrest->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        EncounterCrest->SetGenerateOverlapEvents(false);
        EncounterCrest->SetCanEverAffectNavigation(false);
    }
}

void AUnmadeEnemyCharacter::ConfigureEncounter(FName Identity,const FString& Name,
    const FLinearColor& Accent)
{
    if(Identity.IsNone()||Name.IsEmpty())return;
    Tags.AddUnique(Identity);
    TerritoryHome=GetActorLocation();
    TryLoadEnemyRig();
    EncounterLabel->SetText(FText::FromString(Name));
    EncounterLabel->SetTextRenderColor(Accent.ToFColor(true));
    const bool bWatch=Style==UnmadeCore::EnemyStyle::Watcher;
    if(bSpeciesConfigured&&!bSkeletalRigLoaded){
        const auto& Visual=UnmadeCore::CreatureGraybox(SpeciesKind);
        const float Scale=bElderEncounter?1.13f:1.f;
        EncounterCrest->SetRelativeLocation(FVector(18,0,Visual.crestHeight));
        EncounterCrest->SetRelativeScale3D(
            FVector(Visual.crestScale,Visual.crestScale,Visual.crestScale)*Scale);
    }else if(!bSkeletalRigLoaded){
        EncounterCrest->SetRelativeLocation(bWatch?FVector(0,0,95):FVector(38,0,45));
        EncounterCrest->SetRelativeScale3D(bWatch?FVector(.18,.5,.55):FVector(.48,.43,.3));
    }
    if(UMaterialInstanceDynamic* Body=TemporaryEnemyVisual->CreateAndSetMaterialInstanceDynamic(0))
        Body->SetVectorParameterValue(TEXT("Color"),Accent);
}

void AUnmadeEnemyCharacter::SetVisualScale(FVector Scale)
{
    if(IsValid(TemporaryEnemyVisual))
        TemporaryEnemyVisual->SetRelativeScale3D(Scale);
}

void AUnmadeEnemyCharacter::ExposeToFold(double Now, double Seconds)
{
    if (FMath::IsFinite(Now) && Now >= 0.0 && Seconds > 0.0)
    {
        FoldExposedUntil = FMath::Max(FoldExposedUntil, Now + Seconds);
        if(bCinderholdElder)ElderPattern.Interrupt(Now);
        if(WindupEndsAt>=0.0)
        {
            WindupEndsAt=-1.0;
            RecoveryEndsAt=Now+1.0;
            NextWindupAllowedAt=FMath::Max(NextWindupAllowedAt,RecoveryEndsAt);
        }
    }
}

bool AUnmadeEnemyCharacter::IsFractureExposed(double Now) const
{
    return FMath::IsFinite(Now) && Now >= 0.0 && Now < FoldExposedUntil;
}

void AUnmadeEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    TerritoryHome=GetActorLocation();
    TryLoadEnemyRig();
}

void AUnmadeEnemyCharacter::TryLoadEnemyRig()
{
    // Actual keyed skeletal locomotion/attack clips are in the portable
    // performance kit. Imported assets are opt-in; no mannequin is assumed.
    if(!IsValid(GetMesh())) return;
    const bool bFloating=Style==UnmadeCore::EnemyStyle::Watcher;
    const TCHAR* Path=bFloating
        ?TEXT("/Game/UnmadeProduction/Characters/Wraith/SK_UnmadeWraith.SK_UnmadeWraith")
        :TEXT("/Game/UnmadeProduction/Characters/Beast/SK_UnmadeBeast.SK_UnmadeBeast");
    if(USkeletalMesh* Rig=LoadObject<USkeletalMesh>(nullptr,Path))
    {
        GetMesh()->SetSkeletalMesh(Rig);
        GetMesh()->SetRelativeLocation(FVector(0,0,-88));
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        TemporaryEnemyVisual->SetVisibility(false);
        EncounterCrest->SetVisibility(false);
        bSkeletalRigLoaded=true;
        CurrentMotion=NAME_None;
        SetMotionCue(FName(TEXT("Idle")));
    }
    else if(bSkeletalRigLoaded)
    {
        // Style changed and this rig has not been imported: show honest fallback.
        GetMesh()->SetSkeletalMesh(nullptr);
        TemporaryEnemyVisual->SetVisibility(true);
        EncounterCrest->SetVisibility(true);
        bSkeletalRigLoaded=false;
        CurrentMotion=NAME_None;
    }
}

void AUnmadeEnemyCharacter::SetMotionCue(FName Motion)
{
    const double now=GetWorld()?GetWorld()->GetTimeSeconds():0;
    const bool staggerHeld=(CurrentMotion==FName(TEXT("Stagger"))&&now<MotionHeldUntil)||now<HitFlashEndsAt;
    if(!UnmadeCore::MotionMayReplace(Combat&&Combat->IsDefeated(),CurrentMotion==FName(TEXT("Death")),staggerHeld,now<MotionHeldUntil,Motion==FName(TEXT("Death")),Motion==FName(TEXT("Stagger")),Motion==FName(TEXT("Idle"))||Motion==FName(TEXT("Walk"))))return;
    if(!bSkeletalRigLoaded || Motion==CurrentMotion || !IsValid(GetMesh()))return;
    const FString Root=Style==UnmadeCore::EnemyStyle::Watcher
        ?TEXT("/Game/UnmadeProduction/Characters/Wraith/")
        :TEXT("/Game/UnmadeProduction/Characters/Beast/");
    const FString Rig=Style==UnmadeCore::EnemyStyle::Watcher
        ?TEXT("Wraith"):TEXT("Beast");
    const FString Name=FString::Printf(TEXT("AN_Unmade%s_%s"),*Rig,*Motion.ToString());
    const FString Path=Root+Name+TEXT(".")+Name;
    if(UAnimSequence* Sequence=LoadObject<UAnimSequence>(nullptr,*Path))
    {
        GetMesh()->PlayAnimation(Sequence,Motion==FName(TEXT("Idle"))||Motion==FName(TEXT("Walk"))||Motion==FName(TEXT("Guard")));
        CurrentMotion=Motion;
        if(GetWorld())MotionHeldUntil=GetWorld()->GetTimeSeconds()+UnmadeCore::MotionHold(Motion==FName(TEXT("Stagger"))?3:Motion==FName(TEXT("Crush"))||Motion==FName(TEXT("Ranged"))?2:Motion==FName(TEXT("Attack"))||Motion==FName(TEXT("Seal"))?1:0);
    }
    else if(Motion!=FName(TEXT("Attack")))
    {
        // Preserve a playable idle pose when only the skeletal mesh has imported.
        CurrentMotion=NAME_None;
    }
}

void AUnmadeEnemyCharacter::NotifyPlayerHit(float Damage)
{
    if(Damage<=0.f || !GetWorld())return;
    UnmadePresentation::Sound(this,TEXT("injury"));
    if(!Combat->IsDefeated()&&CurrentMotion==FName(TEXT("Stagger")))CurrentMotion=NAME_None; // A new injury restarts its held response.
    SetMotionCue(FName(Combat->IsDefeated()?TEXT("Death"):TEXT("Stagger")));
    HitFlashEndsAt=GetWorld()->GetTimeSeconds()+.40;
    if(IsValid(CombatSignal))
    {
        CombatSignal->SetText(FText::FromString(
            FString::Printf(TEXT("-%.0f  HIT"),Damage)));
        CombatSignal->SetTextRenderColor(FColor(255,237,134));
    }
    if(IsValid(TelegraphLight))TelegraphLight->SetLightColor(FLinearColor(1.f,.78f,.12f));
}

void AUnmadeEnemyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if(!IsValid(Combat))return;
    UWorld* World=GetWorld();
    if(!World)return;
    const double Now=World->GetTimeSeconds();
    if(!CachedRealityHub.IsValid())CachedRealityHub=Cast<AUnmadePrototypeHub>(
        UGameplayStatics::GetActorOfClass(World,AUnmadePrototypeHub::StaticClass()));
    if(bWingObserver){TickWingObserver(DeltaSeconds);return;}
    if(ExpeditionArc>=0){TickExpedition(DeltaSeconds);return;}
    if(bCinderholdElder&&CachedRealityHub.IsValid())
    {
        const int32 Resolution=RoadElderCity>=0?CachedRealityHub->GetRoadElderResolution(RoadElderCity):CachedRealityHub->GetCinderholdElderResolution();
        if(Resolution!=0)
        {
            bCinderholdQuieted=Resolution==2;
            SetActorHiddenInGame(Resolution==1);
            SetActorEnableCollision(false);
            GetCharacterMovement()->StopMovementImmediately();
            ElderAimMarker->SetVisibility(false);
            TelegraphLight->SetIntensity(0);
            CombatSignal->SetText(FText::FromString(Resolution==2?TEXT("ELDER QUIETED - ALIVE"):TEXT("ELDER DEFEATED")));
            SetMotionCue(FName(TEXT("Idle")));
            SetActorTickEnabled(false);
            return;
        }
    }
    if(Combat->IsDefeated())
    {
        if(bCinderholdElder)
        {
            SetActorEnableCollision(false);SetActorHiddenInGame(true);
            if(CachedRealityHub.IsValid()&&Now>=NextElderSaveAttemptAt)
            {
                NextElderSaveAttemptAt=Now+3;
                if(RoadElderCity>=0?CachedRealityHub->RecordRoadElderDefeat(this,RoadElderCity):CachedRealityHub->RecordCinderholdElderDefeat(this))SetActorTickEnabled(false);
            }
            return; // Keep retrying a failed durable receipt, never announce a false victory.
        }
        if(!bDeathHandled)
        {
            bDeathHandled=true;
            WindupEndsAt=-1.0;
            SetActorEnableCollision(false);
            SetActorHiddenInGame(true);
            SetActorTickEnabled(false); // No invented loot or save-backed kill flag.
        }
        return;
    }
    const bool bElderSecondPhase=bElderEncounter&&
        Combat->GetHealth()<=Combat->GetMaxHealth()*.5f;
    AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(
        UGameplayStatics::GetPlayerCharacter(World,0));
    if(!UnmadeCore::RequireEncounterPlayer(IsValid(Player)&&IsValid(Player->GetCombat())&&
       !Player->GetCombat()->IsDefeated()&&Player->GetWorld()==World&&UnmadeSave::IsSessionWritable(Player),[&]{
        UnmadeCore::InterruptAttackWindup(WindupEndsAt,NextWindupAllowedAt,RecoveryEndsAt);
        UnmadeCore::InterruptEncounter(ElderPattern);ElderLastWarning=UnmadeCore::SlagAttack::None;
        Alertness.Reset();
        NormalLockedAim=ElderLockedAim=FVector::ZeroVector;bHasNavWaypoint=false;
        GetCharacterMovement()->StopMovementImmediately();
        ElderAimMarker->SetVisibility(false);TelegraphLight->SetIntensity(0);
        CombatSignal->SetText(FText::GetEmpty());
    }))return;

    if(!CachedRealityHub.IsValid())
        CachedRealityHub=Cast<AUnmadePrototypeHub>(
            UGameplayStatics::GetActorOfClass(World,AUnmadePrototypeHub::StaticClass()));
    const auto Consequence=CachedRealityHub.IsValid()
        ?CachedRealityHub->RealityConsequenceAt(GetActorLocation())
        :UnmadeCore::RealityConsequenceView{};
    FVector Delta=Player->GetActorLocation()-GetActorLocation();
    Delta.Z=0;
    const double DistanceCm=Delta.Size();
    const double TerritoryRadius=bCinderholdElder?950.0:1100.0;
    const bool bOutsideTerritory=FVector::DistSquared2D(Player->GetActorLocation(),TerritoryHome)>FMath::Square(TerritoryRadius)||
        FVector::DistSquared2D(GetActorLocation(),TerritoryHome)>FMath::Square(TerritoryRadius);
    const bool bReturnHome=bOutsideTerritory&&FVector::DistSquared2D(GetActorLocation(),TerritoryHome)>FMath::Square(85.f);
    const bool bNearby=!bOutsideTerritory&&DistanceCm<=1800.0 && DistanceCm>=1.0;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeEnemySight),false);
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(Player);
    const bool bClearSight=bNearby && !World->LineTraceTestByChannel(
        GetActorLocation()+FVector(0,0,55),
        Player->GetActorLocation()+FVector(0,0,55),ECC_Visibility,Params);
    // Wildlife may ignore a crouching traveler outside its forward sight cone,
    // but it remains wary briefly after a confirmed sighting.
    const bool bOrdinaryStealth=!bCinderholdElder&&!bElderEncounter;
    const bool bCrouched=IsValid(Player->GetCharacterMovement())&&
        Player->GetCharacterMovement()->IsCrouching();
    const double Facing=FVector::DotProduct(GetActorForwardVector(),Delta.GetSafeNormal2D());
    const bool bSpotted=UnmadeCore::CreatureCanSpot(
        bClearSight,bCrouched,DistanceCm/100.0,Facing);
    const bool bEngaged=bOrdinaryStealth
        ?bNearby&&Alertness.Update(Now,bSpotted):bNearby;
    // A committed, already-warned strike is not canceled just because
    // its species' feint cycle advances during that visible windup.
    const bool bExistingWindup=WindupEndsAt>=0.0;
    auto Intent=bEngaged
        ?(bSpeciesConfigured
            ?((bElderEncounter||bExistingWindup)?UnmadeCore::CreatureIntent(Profile,DistanceCm/100.0,bClearSight,Now<RecoveryEndsAt)
            :UnmadeCore::TacticalCreatureIntent(Species,Profile,DistanceCm/100.0,
                 bClearSight,Now<RecoveryEndsAt,Now,
                 Combat->GetHealth()/FMath::Max(1.f,Combat->GetMaxHealth())))
            :UnmadeCore::ChooseEnemyIntent(Style,DistanceCm/100.0,bClearSight))
        :UnmadeCore::EnemyIntent::Hold;
    UnmadeCore::SlagElderStep ElderStep;
    if(bCinderholdElder)
    {
        ElderStep=ElderPattern.Step(Now,bElderSecondPhase,bNearby&&DistanceCm<=420,
            bClearSight,(CachedRealityHub.IsValid()&&(RoadElderCity>=0?CachedRealityHub->IsRoadCooling(RoadElderCity):CachedRealityHub->IsCinderholdVentActive()))||IsFractureExposed(Now));
        if(ElderStep.warning!=UnmadeCore::SlagAttack::None&&!ElderStep.strike&&ElderLastWarning==UnmadeCore::SlagAttack::None)
            ElderLockedAim=Player->GetActorLocation();
        Intent=(!bNearby||ElderStep.warning!=UnmadeCore::SlagAttack::None||ElderStep.recovering||
            (CachedRealityHub.IsValid()&&(RoadElderCity>=0?CachedRealityHub->IsRoadCooling(RoadElderCity):CachedRealityHub->IsCinderholdVentActive()))||IsFractureExposed(Now))
            ?UnmadeCore::EnemyIntent::Hold:UnmadeCore::EnemyIntent::Approach;
    }
    const auto Routine=UnmadeCore::CreatureRoutine(Style,Now,
        CachedRealityHub.IsValid()&&CachedRealityHub->GetCurrentPhase()==UnmadeCore::DayPhase::Night,bEngaged);
    const FVector RoutineTarget=TerritoryHome+FVector(Routine.offsetX,Routine.offsetY,0);
    const bool bLocalRoutine=!bEngaged&&!bReturnHome&&FVector::DistSquared2D(GetActorLocation(),RoutineTarget)>FMath::Square(80.f);
    if(bReturnHome||bLocalRoutine)Intent=UnmadeCore::EnemyIntent::Approach;
    // Only ordinary creatures participate in the local pressure budget.
    // Bosses, guardians and authored elder timings keep their own rules.
    int ActiveOrdinaryWarnings=0;
    if(!bCinderholdElder&&!bElderEncounter&&bNearby&&bClearSight&&WindupEndsAt<0.0 &&
       Intent==UnmadeCore::EnemyIntent::Attack)
    {
        for(TActorIterator<AUnmadeEnemyCharacter> other(World);other;++other)
        {
            if(*other==this||other->ExpeditionArc>=0||other->bCinderholdElder||
               other->bElderEncounter||
               other->bWingObserver||!other->CanBeTargeted()||
               other->WindupEndsAt<Now)continue;
            if(FVector::DistSquared2D(other->GetActorLocation(),
               Player->GetActorLocation())>FMath::Square(1050.f))continue;
            ++ActiveOrdinaryWarnings;
            break; // One nearby active warning is enough to yield.
        }
    }
    const auto Tactic=UnmadeCore::CreatureTactic(Species);
    const bool bTacticalFlank=bSpeciesConfigured&&!bElderEncounter&&
        Tactic.flankSide!=0&&Intent==UnmadeCore::EnemyIntent::Approach&&
        bEngaged&&bClearSight&&
        UnmadeCore::CreatureIntent(Profile,DistanceCm/100.0,
            bClearSight,Now<RecoveryEndsAt)==UnmadeCore::EnemyIntent::Attack;
    const bool bCircleForOpening=UnmadeCore::ShouldCircleForOpening(
        Intent==UnmadeCore::EnemyIntent::Attack&&bClearSight,
        WindupEndsAt>=0.0,ActiveOrdinaryWarnings);
    if(bCircleForOpening)Intent=UnmadeCore::EnemyIntent::Approach;
    if(Combat->IsStaggered(Now)){Intent=UnmadeCore::EnemyIntent::Hold;WindupEndsAt=-1;}
    if(WindupEndsAt>=0.0 && (Intent!=UnmadeCore::EnemyIntent::Attack||!bClearSight))
        WindupEndsAt=-1.0; // The player can step behind real cover to cancel.

    // Navigation must be grounded in a real Unreal navigation path.
    // Never treat a direct root sweep through a pillar as pathfinding.
    if(UnmadeCore::WantsCreatureNavigation(bNearby,bReturnHome,bLocalRoutine) &&
       (Intent==UnmadeCore::EnemyIntent::Approach||
       Intent==UnmadeCore::EnemyIntent::Retreat))
    {
        const float BaseSpeed=bSpeciesConfigured?static_cast<float>(Profile.speed):(Style==UnmadeCore::EnemyStyle::Stalker?130.f:95.f);
        const float Speed=BaseSpeed*(bElderSecondPhase?1.25f:1.f)*static_cast<float>(Consequence.enemySpeedPercent)/100.f;
        GetCharacterMovement()->MaxWalkSpeed=FMath::Max(25.f,Speed)*Combat->MoveMultiplier(Now);
        // While another nearby creature telegraphs, move sideways to make
        // the formation tactical instead of launching overlapping strikes.
        const FVector Side(-Delta.Y,Delta.X,0);
        const FVector Flank=Player->GetActorLocation()+Side.GetSafeNormal2D()*
            (bTacticalFlank?Tactic.flankSide*260.f:((GetUniqueID()&1u)?260.f:-260.f));
        const FVector Destination=bReturnHome?TerritoryHome:bLocalRoutine?RoutineTarget:bCircleForOpening?Flank:bTacticalFlank?Flank:Intent==UnmadeCore::EnemyIntent::Retreat
            ?GetActorLocation()-Delta.GetSafeNormal()*480.f
            :Player->GetActorLocation();
        if(Now>=NextPathRefreshAt)
        {
            NextPathRefreshAt=Now+.6;
            bHasNavWaypoint=false;
            if(UNavigationPath* Path=UNavigationSystemV1::FindPathToLocationSynchronously(
                World,GetActorLocation(),Destination,this))
            {
                if(Path->IsValid()&&!Path->IsPartial()&&Path->PathPoints.Num()>=2)
                {
                    // Path[0] may be current position; seek the first reachable segment.
                    NextNavWaypoint=Path->PathPoints[1];
                    if(FVector::DistSquared2D(GetActorLocation(),NextNavWaypoint)<FMath::Square(85.f)
                       && Path->PathPoints.Num()>2)
                        NextNavWaypoint=Path->PathPoints[2];
                    bHasNavWaypoint=true;
                }
            }
        }
        const bool bLocalPathClear=(bReturnHome||bLocalRoutine||bCircleForOpening||bTacticalFlank)?!World->LineTraceTestByChannel(
            GetActorLocation()+FVector(0,0,55),Destination+FVector(0,0,55),ECC_Visibility,Params):bClearSight;
        FVector Walk=bHasNavWaypoint ? NextNavWaypoint-GetActorLocation()
            :((bLocalPathClear)?Destination-GetActorLocation():FVector::ZeroVector);
        Walk.Z=0;
        if(Walk.SizeSquared2D()>FMath::Square(70.f))
        {
            // CharacterMovement supplies collision and gravity; a missing navmesh
            // permits only unblocked local movement and never wall penetration.
            AddMovementInput(Walk.GetSafeNormal2D());
            SetActorRotation(Walk.Rotation());
        }
        SetMotionCue(FName(TEXT("Walk")));
    }
    else
    {
        SetMotionCue(FName(TEXT("Idle")));
        if(bEngaged)SetActorRotation(Delta.GetSafeNormal2D().Rotation());
    }

    if(bCinderholdElder)
    {
        const bool Warning=ElderStep.warning!=UnmadeCore::SlagAttack::None&&!ElderStep.strike;
        ElderAimMarker->SetVisibility(Warning);
        if(Warning)
        {
            const float Radius=ElderStep.warning==UnmadeCore::SlagAttack::GroundSlam?190.f:260.f;
            ElderAimMarker->SetWorldLocation(ElderLockedAim-FVector(0,0,85));
            ElderAimMarker->SetWorldScale3D(FVector(Radius/50.f,Radius/50.f,.025f));
            CombatSignal->SetText(FText::FromString(ElderStep.warning==UnmadeCore::SlagAttack::GroundSlam
                ?TEXT("ELDER SLAM - STEP OUT OF THE MARK"):
                 TEXT("ELDER SWEEP - WIDE MARK / TAKE COVER")));
            TelegraphLight->SetIntensity(4200.f);
            SetMotionCue(FName(TEXT("Attack")));
        }
        else
        {
            CombatSignal->SetText(FText::FromString(ElderStep.recovering?TEXT("ELDER RECOVERY - OPENING"):!bNearby?Routine.activity==UnmadeCore::WildActivity::Forage?TEXT("FORAGING IN ITS TERRITORY"):Routine.activity==UnmadeCore::WildActivity::Patrol?TEXT("PATROLLING ITS TERRITORY"):TEXT("RESTING AT ITS NEST"):TEXT("DEFENDS THE NEST")));
            TelegraphLight->SetIntensity(ElderStep.recovering?850.f:0.f);
        }
        if(ElderStep.strike)
        {
            if(USoundBase* Sound=LoadObject<USoundBase>(nullptr,TEXT("/Game/UnmadeProduction/Audio/Cinderhold/SFX_CH_elder_ground.SFX_CH_elder_ground"),nullptr,LOAD_NoWarn))
                UGameplayStatics::PlaySoundAtLocation(World,Sound,ElderLockedAim,.7f*GetDefault<UUnmadePlayerPreferences>()->AudioVolume);
        }
        if(ElderStep.strike&&bClearSight&&DistanceCm<=420)
        {
            const float Radius=ElderStep.warning==UnmadeCore::SlagAttack::GroundSlam?190.f:260.f;
            if(FVector::DistSquared2D(Player->GetActorLocation(),ElderLockedAim)<=FMath::Square(Radius))
                Combat->TryStrikeTarget(Player->GetCombat(),Now,true,false);
        }
        ElderLastWarning=Warning?ElderStep.warning:UnmadeCore::SlagAttack::None;
        return;
    }
    if(WindupEndsAt>=0.0 && Now>=WindupEndsAt)
    {
        WindupEndsAt=-1.0;
        NextWindupAllowedAt=Now+
            (bSpeciesConfigured?Profile.cooldown:(Style==UnmadeCore::EnemyStyle::Stalker?1.4:2.25));
        RecoveryEndsAt=Now+.65;
        SetMotionCue(FName(TEXT("Attack")));
        if(Intent==UnmadeCore::EnemyIntent::Attack && bClearSight)
        {
            const float Before=Player->GetCombat()->GetHealth();
            if(Style==UnmadeCore::EnemyStyle::Watcher){uint64 id=0;double damage=0;if(Combat->ReserveProjectile(Now,id,damage)){FVector muzzle=GetActorLocation()+FVector(0,0,55)+GetActorForwardVector()*80;if(auto* shot=World->SpawnActor<AUnmadeTravelingProjectile>(muzzle,FRotator::ZeroRotator))shot->Fire(this,NormalLockedAim+FVector(0,0,55),damage,id);} }
            else if(Combat->TryStrikeTarget(Player->GetCombat(),Now,true,false))
            {
                if(GEngine)UnmadeRpgFeedback(-1,1.4f,FColor::Red,
                    FString::Printf(TEXT("%s: -%.0f HP"),
                        Style==UnmadeCore::EnemyStyle::Stalker
                            ?TEXT("BEAST STRIKE"):TEXT("WATCHER PULSE"),
                        Before-Player->GetCombat()->GetHealth()));
                UnmadePresentation::Sound(this,TEXT("injury"));
    SetMotionCue(FName(Combat->IsDefeated()?TEXT("Death"):TEXT("Stagger")));
    HitFlashEndsAt=Now+.35;
            }
        }
    }
    if(Intent==UnmadeCore::EnemyIntent::Attack &&
       UnmadeCore::MayBeginOrdinaryWindup(bClearSight,ActiveOrdinaryWarnings) &&
       WindupEndsAt<0.0 && Now>=NextWindupAllowedAt && Now>=RecoveryEndsAt)
    {
        NormalLockedAim=Player->GetActorLocation();
        WindupEndsAt=Now+
            (bSpeciesConfigured?Profile.windup:(Style==UnmadeCore::EnemyStyle::Stalker?.65:.95));
        SetMotionCue(FName(TEXT("Attack")));
    }
    if(IsValid(CombatSignal)&&IsValid(TelegraphLight))
    {
        if(WindupEndsAt>=0.0)
        {
            CombatSignal->SetText(FText::FromString(bSpeciesConfigured
                ?FString(UTF8_TO_TCHAR(Profile.warning)):FString(TEXT("!  INCOMING  !"))));
            if(bElderSecondPhase)CombatSignal->SetText(FText::FromString(
                FString(TEXT("ELDER RALLY - "))+UTF8_TO_TCHAR(Profile.warning)));
            CombatSignal->SetTextRenderColor(FColor(255,109,78));
            TelegraphLight->SetLightColor(FLinearColor(1.f,.16f,.04f));
            TelegraphLight->SetIntensity(3600.f);
            if(!bSkeletalRigLoaded)
                EncounterCrest->SetRelativeScale3D(FVector(.5,.5,.35)*
                    (GetDefault<UUnmadePlayerPreferences>()->ReducedFlashes?1.f:1.f+.18f*FMath::Sin(static_cast<float>(Now)*18.f)));
        }
        else if(Now<HitFlashEndsAt&&!GetDefault<UUnmadePlayerPreferences>()->ReducedFlashes)
        {
            TelegraphLight->SetIntensity(2650.f);
        }
        else
        {
            CombatSignal->SetText(FText::FromString(bTacticalFlank
                ?FString(UTF8_TO_TCHAR(Tactic.cue)):FString()));
            TelegraphLight->SetIntensity(0.f);
        }
    }
}

void AUnmadeEnemyCharacter::ConfigureExpedition(int32 Arc,int32 Slot,UnmadeCore::ExpeditionRole Role){
 ExpeditionArc=Arc;ExpeditionSlot=Slot;ExpeditionRole=Role;TerritoryHome=GetActorLocation();
 const bool Boss=Role==UnmadeCore::ExpeditionRole::Boss;
 if(Boss&&SignatureTiles.IsEmpty())for(int i=0;i<81;++i){auto* tile=NewObject<UStaticMeshComponent>(this);tile->SetupAttachment(GetCapsuleComponent());tile->RegisterComponent();tile->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Engine/BasicShapes/Cube.Cube")));tile->SetCollisionEnabled(ECollisionEnabled::NoCollision);tile->SetVisibility(false);SignatureTiles.Add(tile);}
 Style=Role==UnmadeCore::ExpeditionRole::Marksman||Role==UnmadeCore::ExpeditionRole::Disruptor?UnmadeCore::EnemyStyle::Watcher:UnmadeCore::EnemyStyle::Stalker;
 Combat->SetHeavyResistance(Boss);
 Combat->Configure(Boss?240+Arc*25:Role==UnmadeCore::ExpeditionRole::Sentinel?95+Arc*8:65+Arc*6,UnmadeCore::ExpeditionMove(Role,Arc,0,false).damage,.5);
 if(Boss&&Arc>=0&&Arc<9){const auto& shape=UnmadeCore::EncounterPerformances[Arc];auto* prop=NewObject<UStaticMeshComponent>(this);prop->SetupAttachment(GetCapsuleComponent());prop->RegisterComponent();prop->SetCollisionEnabled(ECollisionEnabled::NoCollision);prop->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,shape.shape==1?TEXT("/Engine/BasicShapes/Sphere.Sphere"):shape.shape==2?TEXT("/Engine/BasicShapes/Cone.Cone"):TEXT("/Engine/BasicShapes/Cube.Cube")));prop->SetRelativeLocation(FVector(45,0,95));prop->SetRelativeScale3D(FVector(shape.propX,shape.propY,shape.propZ));}
 TryLoadEnemyRig();
}
void AUnmadeEnemyCharacter::ConfigureWingGuardian(int Arc){bWingGuardian=true;ConfigureExpedition(Arc,9,UnmadeCore::ExpeditionRole::Boss);Combat->Configure(150+Arc*12,18+Arc*2,.6);}
void AUnmadeEnemyCharacter::TickExpedition(float DeltaSeconds){
 UWorld* World=GetWorld();
 const auto Interrupt=[&]{
  UnmadeCore::InterruptEncounter(ExpeditionPattern);bExpeditionWarning=false;ExpeditionAim=FVector::ZeroVector;
  Combat->SetGuarding(false);GetCharacterMovement()->StopMovementImmediately();bHasNavWaypoint=false;
  ElderAimMarker->SetVisibility(false);TelegraphLight->SetIntensity(0);CombatSignal->SetText(FText::GetEmpty());
  for(const auto& tile:SignatureTiles)if(IsValid(tile))tile->SetVisibility(false);
 };
 if(!World||!CachedRealityHub.IsValid()){Interrupt();return;}double Now=World->GetTimeSeconds();
 for(const auto& tile:SignatureTiles)if(IsValid(tile))tile->SetVisibility(false);
 bool Inactive=bWingGuardian?CachedRealityHub->WingGuardianInactive(ExpeditionArc):CachedRealityHub->ExpeditionEnemyInactive(ExpeditionArc,ExpeditionSlot);
 bool Yielded=bWingGuardian?CachedRealityHub->WingGuardianYielded(ExpeditionArc):ExpeditionSlot==8&&CachedRealityHub->ExpeditionBossYielded(ExpeditionArc);
 SetActorHiddenInGame((Inactive&&!Yielded)||Combat->IsDefeated());SetActorEnableCollision(!Inactive&&!Combat->IsDefeated());
 if(Inactive){Interrupt();if(Yielded){SetMotionCue(FName(TEXT("Yield")));CombatSignal->SetText(FText::FromString((bWingGuardian?TEXT("YIELDED ALIVE - OATH HEARD"):TEXT("YIELDED ALIVE - EVIDENCE REMEMBERED"))));}GetCharacterMovement()->StopMovementImmediately();ElderAimMarker->SetVisibility(false);TelegraphLight->SetIntensity(0);return;}
 if(Combat->IsDefeated()){
  if(IsTemporaryArenaEscort()){
   // These temporary guards exert actual combat pressure but cannot claim
   // persistent boss/captive receipts or respawn as reward farms.
   SetActorHiddenInGame(true);SetActorEnableCollision(false);
   GetCharacterMovement()->StopMovementImmediately();
   SetActorTickEnabled(false);
   return;
  }
  if(Now>=NextElderSaveAttemptAt){NextElderSaveAttemptAt=Now+2;if(bWingGuardian)CachedRealityHub->RecordWingGuardian(this,ExpeditionArc);else CachedRealityHub->RecordExpeditionDefeat(this,ExpeditionArc,ExpeditionSlot);}return;
 }
 auto* P=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(World,0));
 if(!UnmadeCore::RequireEncounterPlayer(IsValid(P)&&IsValid(P->GetCombat())&&!P->GetCombat()->IsDefeated()&&P->GetWorld()==World&&UnmadeSave::IsSessionWritable(P),Interrupt))return;
 double Distance=FVector::Dist2D(P->GetActorLocation(),GetActorLocation());
 bool Territory=FVector::DistSquared2D(P->GetActorLocation(),TerritoryHome)<=FMath::Square(1450.f);
 FCollisionQueryParams Params(SCENE_QUERY_STAT(ExpeditionAttackSight),false);Params.AddIgnoredActor(this);Params.AddIgnoredActor(P);
 bool Sight=Territory&&!World->LineTraceTestByChannel(GetActorLocation()+FVector(0,0,55),P->GetActorLocation()+FVector(0,0,55),ECC_Visibility,Params);
 bool Phase=Combat->GetHealth()<=Combat->GetMaxHealth()*.5;
 if((P->bIsCrouched||P->GetCombat()->HasQuietLens(Now))&&Distance>350)Sight=false;
 int Warnings=0;bool ShieldSupport=false;for(auto weak:CachedRealityHub->CachedEnemies()){auto* Ally=weak.Get();if(!Ally||Ally==this||!Ally->IsExpeditionArc(ExpeditionArc)||!Ally->CanBeTargeted()||
   FVector::DistSquared2D(Ally->GetActorLocation(),GetActorLocation())>FMath::Square(1750.f))continue;Warnings+=Ally->bExpeditionWarning?1:0;if(Ally->ExpeditionRole==UnmadeCore::ExpeditionRole::Sentinel&&Ally->GetCombat()->IsGuarding()&&FVector::DistSquared2D(Ally->GetActorLocation(),GetActorLocation())<FMath::Square(500.f))ShieldSupport=true;}
 if(!bExpeditionWarning&&Warnings>=2)Sight=false;Combat->SetGearBonuses(0,ShieldSupport?4:0);
 auto Step=ExpeditionPattern.Step(Now,ExpeditionRole,ExpeditionArc,Phase,Sight,Distance,IsFractureExposed(Now)||Combat->IsStaggered(Now));
 if(Step.started){UnmadePresentation::Sound(this,TEXT("warning"));ExpeditionAim=P->GetActorLocation();bExpeditionWarning=true;}
 if(!Sight||IsFractureExposed(Now)||Step.recovering)bExpeditionWarning=false;
 if(Step.strike){FString strikeCue=Step.move.shape?FString::Printf(TEXT("boss_signature_%d"),ExpeditionArc):ExpeditionRole==UnmadeCore::ExpeditionRole::Boss?TEXT("boss_crush"):TEXT("swing");UnmadePresentation::Sound(this,*strikeCue);SetMotionCue(FName(Step.move.shape?TEXT("Seal"):ExpeditionRole==UnmadeCore::ExpeditionRole::Marksman?TEXT("Ranged"):TEXT("Crush")));bExpeditionWarning=false;FVector Offset=P->GetActorLocation()-ExpeditionAim;if(Sight&&(ExpeditionRole==UnmadeCore::ExpeditionRole::Marksman||UnmadeCore::SignatureHit(Step.move.shape,Offset.X,Offset.Y,Step.move.radius))){
  Combat->SetAttackDamage(Step.move.damage);
  if(ExpeditionRole==UnmadeCore::ExpeditionRole::Marksman){uint64 id=0;double damage=0;if(Combat->ReserveProjectile(Now,id,damage)){FVector muzzle=GetActorLocation()+FVector(0,0,55)+GetActorForwardVector()*80;if(auto* shot=World->SpawnActor<AUnmadeTravelingProjectile>(muzzle,FRotator::ZeroRotator))shot->Fire(this,ExpeditionAim+FVector(0,0,55),damage,id);}}
  else Combat->TryStrikeTarget(P->GetCombat(),Now,true,false);
 }}
 if(bExpeditionWarning&&Step.move.shape){for(int i=0;i<SignatureTiles.Num();++i){double x=((i%9)-4)*Step.move.radius/4.,y=((i/9)-4)*Step.move.radius/4.;auto* tile=SignatureTiles[i].Get();tile->SetVisibility(UnmadeCore::SignatureHit(Step.move.shape,x,y,Step.move.radius));tile->SetWorldLocation(ExpeditionAim+FVector(x,y,-82));tile->SetWorldRotation(FRotator::ZeroRotator);tile->SetWorldScale3D(FVector(Step.move.radius/400.,Step.move.radius/400.,.025));}}
 Combat->SetGuarding(ExpeditionRole==UnmadeCore::ExpeditionRole::Sentinel&&Sight&&!bExpeditionWarning&&!Step.recovering);
 ElderAimMarker->SetVisibility(bExpeditionWarning);if(bExpeditionWarning){ElderAimMarker->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,Step.move.shape==8?TEXT("/Engine/BasicShapes/Cube.Cube"):TEXT("/Engine/BasicShapes/Cylinder.Cylinder")));ElderAimMarker->SetWorldLocation(ExpeditionAim-FVector(0,0,85));ElderAimMarker->SetWorldScale3D(FVector(Step.move.radius/50,Step.move.radius/50,.025));CombatSignal->SetText(FText::FromString((ExpeditionRole==UnmadeCore::ExpeditionRole::Boss?FString(UTF8_TO_TCHAR(UnmadeCore::EncounterPerformances[ExpeditionArc].warning))+TEXT("\n"):FString())+UTF8_TO_TCHAR(Step.move.warning)));TelegraphLight->SetIntensity(4000);GetCharacterMovement()->StopMovementImmediately();SetMotionCue(FName(Step.move.shape?TEXT("Seal"):ExpeditionRole==UnmadeCore::ExpeditionRole::Marksman?TEXT("Ranged"):TEXT("Attack")));}
 else{TelegraphLight->SetIntensity(Step.recovering?700:0);CombatSignal->SetText(FText::FromString(Step.recovering?TEXT("RECOVERY - YOUR OPENING"):TEXT("WATCH THE NEXT ATTACK")));FVector Destination=!Territory?TerritoryHome:P->GetActorLocation();
  if(Territory&&ExpeditionRole==UnmadeCore::ExpeditionRole::Skirmisher)Destination+=FVector(-(P->GetActorLocation().Y-GetActorLocation().Y),P->GetActorLocation().X-GetActorLocation().X,0).GetSafeNormal()*180;
  bool Move=!Territory||Distance>Step.move.reach*.85;
  if(Territory&&(ExpeditionRole==UnmadeCore::ExpeditionRole::Marksman||ExpeditionRole==UnmadeCore::ExpeditionRole::Disruptor)&&Distance<350){Destination=GetActorLocation()+(GetActorLocation()-P->GetActorLocation()).GetSafeNormal2D()*350;Move=true;}
  if(Move&&!Step.recovering){GetCharacterMovement()->MaxWalkSpeed=(ExpeditionRole==UnmadeCore::ExpeditionRole::Skirmisher?210:120)*Combat->MoveMultiplier(Now);if(Now>=NextPathRefreshAt){NextPathRefreshAt=Now+.6;bHasNavWaypoint=false;if(auto* Path=UNavigationSystemV1::FindPathToLocationSynchronously(World,GetActorLocation(),Destination,this))if(Path->IsValid()&&!Path->IsPartial()&&Path->PathPoints.Num()>1){NextNavWaypoint=Path->PathPoints[1];bHasNavWaypoint=true;}}
   bool Clear=!World->LineTraceTestByChannel(GetActorLocation()+FVector(0,0,55),Destination+FVector(0,0,55),ECC_Visibility,Params);FVector Walk=bHasNavWaypoint?NextNavWaypoint-GetActorLocation():Clear?Destination-GetActorLocation():FVector::ZeroVector;Walk.Z=0;if(Walk.SizeSquared()>4900){AddMovementInput(Walk.GetSafeNormal());SetActorRotation(Walk.Rotation());}SetMotionCue(FName(TEXT("Walk")));}
  else{GetCharacterMovement()->StopMovementImmediately();if(Now>HitFlashEndsAt)SetMotionCue(FName(Combat->IsGuarding()?TEXT("Guard"):TEXT("Idle")));}
 }
}

FString AUnmadeEnemyCharacter::GetCombatName()const{return EncounterLabel?EncounterLabel->Text.ToString():TEXT("Enemy");}
FString AUnmadeEnemyCharacter::GetCombatWarning()const{return CombatSignal?CombatSignal->Text.ToString():FString();}

void AUnmadeEnemyCharacter::ConfigureWingObserver(){ExpeditionArc=6;ExpeditionSlot=10;bWingObserver=true;TerritoryHome=GetActorLocation();ConfigureStyle(UnmadeCore::EnemyStyle::Watcher);Combat->Configure(80,12,.6);}
void AUnmadeEnemyCharacter::TickWingObserver(float Delta){
 const auto Interrupt=[&]{
  UnmadeCore::InterruptAttackWindup(WindupEndsAt,NextWindupAllowedAt,RecoveryEndsAt);NormalLockedAim=FVector::ZeroVector;
  GetCharacterMovement()->StopMovementImmediately();bHasNavWaypoint=false;
  ElderAimMarker->SetVisibility(false);TelegraphLight->SetIntensity(0);CombatSignal->SetText(FText::GetEmpty());
 };
 if(!GetWorld()||!CachedRealityHub.IsValid()){Interrupt();return;}double now=GetWorld()->GetTimeSeconds();bool inactive=CachedRealityHub->WingObserverInactive();SetActorHiddenInGame(inactive||Combat->IsDefeated());SetActorEnableCollision(!inactive&&!Combat->IsDefeated());if(inactive){Interrupt();return;}if(Combat->IsDefeated()){if(now>=NextElderSaveAttemptAt){NextElderSaveAttemptAt=now+2;CachedRealityHub->RecordWingObserver(this);}return;}auto* p=Cast<AUnmadeCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
 if(!UnmadeCore::RequireEncounterPlayer(IsValid(p)&&IsValid(p->GetCombat())&&!p->GetCombat()->IsDefeated()&&p->GetWorld()==GetWorld()&&UnmadeSave::IsSessionWritable(p),Interrupt))return;
 FVector toward=p->GetActorLocation()-GetActorLocation();FCollisionQueryParams q(SCENE_QUERY_STAT(WingObserverSight),false);q.AddIgnoredActor(this);q.AddIgnoredActor(p);double range=p->bIsCrouched||p->GetCombat()->HasQuietLens(now)?350:700;bool seen=toward.Size2D()<range&&FVector::DotProduct(GetActorForwardVector(),toward.GetSafeNormal2D())>.35&&!GetWorld()->LineTraceTestByChannel(GetActorLocation()+FVector(0,0,55),p->GetActorLocation()+FVector(0,0,55),ECC_Visibility,q);
 if(Combat->IsStaggered(now)||IsFractureExposed(now)){WindupEndsAt=-1;return;}if(seen&&WindupEndsAt<0&&now>=NextWindupAllowedAt){NormalLockedAim=p->GetActorLocation();WindupEndsAt=now+1.5;UnmadePresentation::Sound(this,TEXT("warning"));}if(WindupEndsAt>=0){CombatSignal->SetText(FText::FromString(TEXT("INDEX SENTRY AIM - BREAK SIGHT OR MOVE")));SetMotionCue(FName(TEXT("Ranged")));if(now>=WindupEndsAt){uint64 id=0;double damage=0;if(Combat->ReserveProjectile(now,id,damage)){FVector muzzle=GetActorLocation()+FVector(0,0,55)+GetActorForwardVector()*80;if(auto* shot=GetWorld()->SpawnActor<AUnmadeTravelingProjectile>(muzzle,FRotator::ZeroRotator))shot->Fire(this,NormalLockedAim+FVector(0,0,55),damage,id);}WindupEndsAt=-1;NextWindupAllowedAt=now+2;}return;}
 CombatSignal->SetText(FText::FromString(TEXT("INDEX SENTRY - PATROL CONE")));FVector goal=TerritoryHome+FVector(FMath::Cos(now*.35),FMath::Sin(now*.35),0)*280;FVector move=goal-GetActorLocation();if(move.SizeSquared2D()>2500){AddMovementInput(move.GetSafeNormal2D());SetActorRotation(move.Rotation());GetCharacterMovement()->MaxWalkSpeed=90*Combat->MoveMultiplier(now);SetMotionCue(FName(TEXT("Walk")));}}

bool AUnmadeEnemyCharacter::HasPendingDefeatSave(const AUnmadePrototypeHub& Hub)const
{
    if(!IsValid(Combat)||!Combat->IsDefeated())return false;
    if(bWingObserver)return !Hub.WingObserverInactive();
    if(bWingGuardian)return !Hub.WingGuardianInactive(ExpeditionArc);
    if(ExpeditionArc>=0)return !Hub.ExpeditionEnemyInactive(ExpeditionArc,ExpeditionSlot);
    if(bCinderholdElder)return (RoadElderCity>=0?
        Hub.GetRoadElderResolution(RoadElderCity):Hub.GetCinderholdElderResolution())==0;
    return false; // Ordinary foes have no durable defeat receipt.
}
