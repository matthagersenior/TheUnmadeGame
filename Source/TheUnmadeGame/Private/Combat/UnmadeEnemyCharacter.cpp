#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Player/UnmadeCharacter.h"
#include "World/UnmadePrototypeHub.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
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

void AUnmadeEnemyCharacter::ConfigureEncounter(FName Identity,const FString& Name,
    const FLinearColor& Accent)
{
    if(Identity.IsNone()||Name.IsEmpty())return;
    Tags.AddUnique(Identity);
    TryLoadEnemyRig();
    EncounterLabel->SetText(FText::FromString(Name));
    EncounterLabel->SetTextRenderColor(Accent.ToFColor(true));
    const bool bWatch=Style==UnmadeCore::EnemyStyle::Watcher;
    EncounterCrest->SetRelativeLocation(bWatch?FVector(0,0,95):FVector(38,0,45));
    EncounterCrest->SetRelativeScale3D(bWatch?FVector(.18,.5,.55):FVector(.48,.43,.3));
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
        FoldExposedUntil = FMath::Max(FoldExposedUntil, Now + Seconds);
}

bool AUnmadeEnemyCharacter::IsFractureExposed(double Now) const
{
    return FMath::IsFinite(Now) && Now >= 0.0 && Now < FoldExposedUntil;
}

void AUnmadeEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
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
        GetMesh()->PlayAnimation(Sequence,Motion!=FName(TEXT("Attack")));
        CurrentMotion=Motion;
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
    if(Combat->IsDefeated())
    {
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
    UWorld* World=GetWorld();
    if(!World)return;
    const double Now=World->GetTimeSeconds();
    AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(
        UGameplayStatics::GetPlayerCharacter(World,0));
    if(!IsValid(Player)||!IsValid(Player->GetCombat())||
       Player->GetCombat()->IsDefeated())return;

    if(!CachedRealityHub.IsValid())
        CachedRealityHub=Cast<AUnmadePrototypeHub>(
            UGameplayStatics::GetActorOfClass(World,AUnmadePrototypeHub::StaticClass()));
    const auto Consequence=CachedRealityHub.IsValid()
        ?CachedRealityHub->RealityConsequenceAt(GetActorLocation())
        :UnmadeCore::RealityConsequenceView{};
    FVector Delta=Player->GetActorLocation()-GetActorLocation();
    Delta.Z=0;
    const double DistanceCm=Delta.Size();
    const bool bNearby=DistanceCm<=1800.0 && DistanceCm>=1.0;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeEnemySight),false);
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(Player);
    const bool bClearSight=bNearby && !World->LineTraceTestByChannel(
        GetActorLocation()+FVector(0,0,55),
        Player->GetActorLocation()+FVector(0,0,55),ECC_Visibility,Params);
    const auto Intent=bNearby
        ?UnmadeCore::ChooseEnemyIntent(Style,DistanceCm/100.0,bClearSight)
        :UnmadeCore::EnemyIntent::Approach;
    if(WindupEndsAt>=0.0 && (Intent!=UnmadeCore::EnemyIntent::Attack||!bClearSight))
        WindupEndsAt=-1.0; // The player can step behind real cover to cancel.

    // Navigation must be grounded in a real Unreal navigation path.
    // Never treat a direct root sweep through a pillar as pathfinding.
    if(bNearby && (Intent==UnmadeCore::EnemyIntent::Approach||
       Intent==UnmadeCore::EnemyIntent::Retreat))
    {
        const float BaseSpeed=Style==UnmadeCore::EnemyStyle::Stalker?130.f:95.f;
        const float Speed=BaseSpeed*static_cast<float>(Consequence.enemySpeedPercent)/100.f;
        GetCharacterMovement()->MaxWalkSpeed=FMath::Max(25.f,Speed);
        const FVector Destination=Intent==UnmadeCore::EnemyIntent::Retreat
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
        FVector Walk=bHasNavWaypoint ? NextNavWaypoint-GetActorLocation()
            :((bClearSight)?Destination-GetActorLocation():FVector::ZeroVector);
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
        if(bNearby)SetActorRotation(Delta.GetSafeNormal2D().Rotation());
    }

    if(WindupEndsAt>=0.0 && Now>=WindupEndsAt)
    {
        WindupEndsAt=-1.0;
        NextWindupAllowedAt=Now+
            (Style==UnmadeCore::EnemyStyle::Stalker?1.4:2.25);
        SetMotionCue(FName(TEXT("Attack")));
        if(Intent==UnmadeCore::EnemyIntent::Attack && bClearSight)
        {
            const float Before=Player->GetCombat()->GetHealth();
            if(Combat->TryStrikeTarget(Player->GetCombat(),Now,true,false))
            {
                if(GEngine)GEngine->AddOnScreenDebugMessage(-1,1.4f,FColor::Red,
                    FString::Printf(TEXT("%s: -%.0f HP"),
                        Style==UnmadeCore::EnemyStyle::Stalker
                            ?TEXT("BEAST STRIKE"):TEXT("WATCHER PULSE"),
                        Before-Player->GetCombat()->GetHealth()));
                HitFlashEndsAt=Now+.35;
            }
        }
    }
    if(Intent==UnmadeCore::EnemyIntent::Attack && bClearSight &&
       WindupEndsAt<0.0 && Now>=NextWindupAllowedAt)
    {
        WindupEndsAt=Now+
            (Style==UnmadeCore::EnemyStyle::Stalker?.65:.95);
        SetMotionCue(FName(TEXT("Attack")));
    }
    if(IsValid(CombatSignal)&&IsValid(TelegraphLight))
    {
        if(WindupEndsAt>=0.0)
        {
            CombatSignal->SetText(FText::FromString(TEXT("!  INCOMING  !")));
            CombatSignal->SetTextRenderColor(FColor(255,109,78));
            TelegraphLight->SetLightColor(FLinearColor(1.f,.16f,.04f));
            TelegraphLight->SetIntensity(3600.f);
            if(!bSkeletalRigLoaded)
                EncounterCrest->SetRelativeScale3D(FVector(.5,.5,.35)*
                    (1.f+.18f*FMath::Sin(static_cast<float>(Now)*18.f)));
        }
        else if(Now<HitFlashEndsAt)
        {
            TelegraphLight->SetIntensity(2650.f);
        }
        else
        {
            CombatSignal->SetText(FText::GetEmpty());
            TelegraphLight->SetIntensity(0.f);
        }
    }
}
