#include "UI/UnmadePlayerFacingRules.h"
#include "Save/UnmadeSaveSession.h"
#include "Save/UnmadeNpcMemorySaveRules.h"
#include "Save/UnmadeActionCommitRules.h"
#include "Save/UnmadeRealityTravelRules.h"
#include "Save/UnmadeRecoveryRules.h"
#include "UI/UnmadeRpgFeedback.h"
#include "Player/UnmadeCharacter.h"
#include "UI/UnmadePlayerPreferences.h"
#include "UI/UnmadeRpgWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Combat/UnmadePresentation.h"
#include "Fracture/UnmadeFractureAnchor.h"
#include "Story/UnmadeConflictGate.h"
#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeLoreSite.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Combat/UnmadeTravelingProjectile.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeBossCharacter.h"
#include "Combat/UnmadeRealmGuardian.h"
#include "Lexicon/UnmadeLexiconComponent.h"
#include "Items/UnmadeEquipmentComponent.h"
#include "Items/UnmadeItemRules.h"
#include "World/UnmadeTenfoldComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "NPC/UnmadeNpcCharacter.h"
#include "Dialogue/UnmadeLocalDialogueSubsystem.h"
#include "Engine/GameInstance.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "NPC/UnmadeMemorySaveRules.h"
#include "Save/UnmadePrototypeSave.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AUnmadeCharacter::AUnmadeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    Combat = CreateDefaultSubobject<UUnmadeCombatComponent>(TEXT("Combat"));
    Lexicon = CreateDefaultSubobject<UUnmadeLexiconComponent>(TEXT("Lexicon"));
    Equipment = CreateDefaultSubobject<UUnmadeEquipmentComponent>(TEXT("Equipment"));
    Tenfold = CreateDefaultSubobject<UUnmadeTenfoldComponent>(TEXT("TenfoldRites"));
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch=true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 340.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    PlaceholderBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypePlayerBody"));
    PlaceholderBody->SetupAttachment(GetCapsuleComponent());
    PlaceholderBody->SetRelativeLocation(FVector(0.f, 0.f, -3.f));
    PlaceholderBody->SetRelativeScale3D(FVector(.54f, .54f, 1.58f));
    PlaceholderBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (BodyMesh.Succeeded()) PlaceholderBody->SetStaticMesh(BodyMesh.Object);
    PlaceholderCrest=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrototypeHairSilhouette"));
    PlaceholderCrest->SetupAttachment(GetCapsuleComponent());
    PlaceholderCrest->SetRelativeLocation(FVector(0,0,82));
    PlaceholderCrest->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CrestMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if(CrestMesh.Succeeded())PlaceholderCrest->SetStaticMesh(CrestMesh.Object);
}

void AUnmadeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Early bootstrap mappings. Replace with data-driven Enhanced Input contexts in an engine editor.
    PlayerInputComponent->BindAction("StoryShelter", IE_Pressed, this, &AUnmadeCharacter::ChooseShelter);
    PlayerInputComponent->BindAction("StoryResearch", IE_Pressed, this, &AUnmadeCharacter::ChooseResearch);
    PlayerInputComponent->BindAction("SupplyActivity", IE_Pressed, this, &AUnmadeCharacter::ProgressSupplyActivity);
    PlayerInputComponent->BindAction("StoryJournal", IE_Pressed, this, &AUnmadeCharacter::ShowStoryJournal);
    PlayerInputComponent->BindAction("EvidenceNotebook", IE_Pressed, this, &AUnmadeCharacter::ShowEvidenceNotebook);
    PlayerInputComponent->BindAction("StudyObserve", IE_Pressed, this, &AUnmadeCharacter::StudyObserve);
    PlayerInputComponent->BindAction("StudyProgress", IE_Pressed, this, &AUnmadeCharacter::StudyProgress);
    PlayerInputComponent->BindAction("StudyUse", IE_Pressed, this, &AUnmadeCharacter::StudyUse);
    PlayerInputComponent->BindAction("StudyNext", IE_Pressed, this, &AUnmadeCharacter::StudyNext);
    PlayerInputComponent->BindAction("CharacterProfile",IE_Pressed,this,&AUnmadeCharacter::ShowCharacterProfile);
    PlayerInputComponent->BindAction("ItemInventory", IE_Pressed, this, &AUnmadeCharacter::ShowInventory);
    PlayerInputComponent->BindAction("ItemWeapon", IE_Pressed, this, &AUnmadeCharacter::EquipNextWeapon);
    PlayerInputComponent->BindAction("ItemArmor", IE_Pressed, this, &AUnmadeCharacter::EquipNextArmor);
    PlayerInputComponent->BindAction("ItemCharm", IE_Pressed, this, &AUnmadeCharacter::EquipNextCharm);
    PlayerInputComponent->BindAction("ItemHeal", IE_Pressed, this, &AUnmadeCharacter::UseHealthPotion);
    PlayerInputComponent->BindAction("ItemStrain", IE_Pressed, this, &AUnmadeCharacter::UseStrainPotion);
    PlayerInputComponent->BindAction("ItemForge", IE_Pressed, this, &AUnmadeCharacter::ForgeMythicGear);
    PlayerInputComponent->BindAction("MarketBuy", IE_Pressed, this, &AUnmadeCharacter::BuyMarketSupplies);
    PlayerInputComponent->BindAction("MarketSell", IE_Pressed, this, &AUnmadeCharacter::SellMarketSupplies);
    PlayerInputComponent->BindAction("ProfessionCraft", IE_Pressed, this, &AUnmadeCharacter::CraftLocalRecipe);
    PlayerInputComponent->BindAction("FactionSolidarity", IE_Pressed, this, &AUnmadeCharacter::CommitSolidarity);
    PlayerInputComponent->BindAction("FactionTruth", IE_Pressed, this, &AUnmadeCharacter::CommitTruth);
    PlayerInputComponent->BindAction("FrontierTravel", IE_Pressed, this, &AUnmadeCharacter::CrossFrontierGateway);
    if (IsValid(Tenfold))
    {
        PlayerInputComponent->BindAction("RiteNext",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::NextRite);
        PlayerInputComponent->BindAction("RitePrevious",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::PreviousRite);
        PlayerInputComponent->BindAction("RiteJournal",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::ShowRite);
        PlayerInputComponent->BindAction("RiteStudy",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::StudyRite);
        PlayerInputComponent->BindAction("RiteUse",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::InvokeRite);
        PlayerInputComponent->BindAction("RiteChoice1",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::DecideSolidarity);
        PlayerInputComponent->BindAction("RiteChoice2",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::DecideTruth);
        PlayerInputComponent->BindAction("RiteBreak",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::BreakChosenOath);
        PlayerInputComponent->BindAction("RiteLaw",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::CycleWorldLaw);
        PlayerInputComponent->BindAction("ConfluenceCycle",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::CycleConfluence);
        PlayerInputComponent->BindAction("ConfluenceStudy",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::StudyConfluence);
        PlayerInputComponent->BindAction("ConfluenceOption1",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::ConfluenceChoice1);
        PlayerInputComponent->BindAction("ConfluenceOption2",IE_Pressed,Tenfold.Get(),&UUnmadeTenfoldComponent::ConfluenceChoice2);
    }
    PlayerInputComponent->BindAction("DodgeStep",IE_Pressed,this,&AUnmadeCharacter::DodgeStep);
    PlayerInputComponent->BindAction("Sneak",IE_Pressed,this,&AUnmadeCharacter::StartSneak);
    PlayerInputComponent->BindAction("Sneak",IE_Released,this,&AUnmadeCharacter::StopSneak);
    PlayerInputComponent->BindAction("FieldKit",IE_Pressed,this,&AUnmadeCharacter::UseFieldKit);
    PlayerInputComponent->BindAction("FieldTechnique",IE_Pressed,this,&AUnmadeCharacter::UseFieldTechnique);
    PlayerInputComponent->BindAction("PersonalChoiceA",IE_Pressed,this,&AUnmadeCharacter::PersonalChoiceA);
    PlayerInputComponent->BindAction("PersonalChoiceB",IE_Pressed,this,&AUnmadeCharacter::PersonalChoiceB);
    PlayerInputComponent->BindAction("WorldMap",IE_Pressed,this,&AUnmadeCharacter::OpenWorldMap);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AUnmadeCharacter::AttemptMeleeAttack);
    PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &AUnmadeCharacter::StartGuard);
    PlayerInputComponent->BindAction("Guard", IE_Released, this, &AUnmadeCharacter::StopGuard);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUnmadeCharacter::Interact);
    PlayerInputComponent->BindAction("SkipDirectedScene",IE_Pressed,this,&AUnmadeCharacter::SkipDirectedScene);
    PlayerInputComponent->BindAction("OfferAid", IE_Pressed, this, &AUnmadeCharacter::OfferAid);
    PlayerInputComponent->BindAction("AnomalyPulse", IE_Pressed, this, &AUnmadeCharacter::DemonstrateAnomaly);
    PlayerInputComponent->BindAction("FoldReality", IE_Pressed, this, &AUnmadeCharacter::FoldReality);
    PlayerInputComponent->BindAction("RewriteOpen", IE_Pressed, this, &AUnmadeCharacter::RewriteOpen);
    PlayerInputComponent->BindAction("RewriteSealed", IE_Pressed, this, &AUnmadeCharacter::RewriteSealed);
    PlayerInputComponent->BindAxis("MoveForward", this, &AUnmadeCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AUnmadeCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AUnmadeCharacter::TurnView);
    PlayerInputComponent->BindAxis("LookUp", this, &AUnmadeCharacter::LookView);
}

void AUnmadeCharacter::ReconcileEarnedRewards()
{
    bStartupRewardsReady=false;
    bStartupNoticeShown=false;
    InitializeStartupRewards();
    if(GetWorld())NextStartupRewardsAttempt=GetWorld()->GetTimeSeconds()+5;
}

void AUnmadeCharacter::ShowInventory()
{
    if(RpgWidget){OpenRpgPage(1);return;}
    ReconcileEarnedRewards();
    if (IsValid(Equipment) && GEngine)
        UnmadeRpgFeedback(-1, 15.f, FColor::Cyan, Equipment->DescribeInventory());
}

void AUnmadeCharacter::EquipNextWeapon()
{
    if (IsValid(Equipment) && Equipment->EquipNext(UnmadeCore::GearSlot::Weapon))
        if(!IsRpgMenuOpen())ShowInventory();
}

void AUnmadeCharacter::EquipNextArmor()
{
    if (IsValid(Equipment) && Equipment->EquipNext(UnmadeCore::GearSlot::Armor))
        if(!IsRpgMenuOpen())ShowInventory();
}

void AUnmadeCharacter::EquipNextCharm()
{
    if (IsValid(Equipment) && Equipment->EquipNext(UnmadeCore::GearSlot::Charm))
        if(!IsRpgMenuOpen())ShowInventory();
}

void AUnmadeCharacter::UseHealthPotion()
{
    if (!IsValid(Equipment)) return;
    int32 Restored = 0;
    const UnmadeCore::ItemId Potions[] = {
        UnmadeCore::ItemId::HearthSalve, UnmadeCore::ItemId::NightwatchTonic,
        UnmadeCore::ItemId::RootboundPoultice, UnmadeCore::ItemId::AshOfPossibleLives
    };
    const auto result=UnmadeCore::UseFirstConsumable(Potions,[&](auto Id){
        return Equipment->UseConsumable(Id,FractureModel.CurrentStrain(),Restored);
    });
    if(result==UnmadeCore::ConsumableCommit::SaveFailed){
        UnmadeRpgFeedback(-1,8.f,FColor::Red,
            TEXT("Potion could not be saved. Nothing consumed or restored; retry after checking Save Profiles."));
        return;
    }
    if(result==UnmadeCore::ConsumableCommit::Used)
        {
            if (Restored > 0) FractureModel.Recover(Restored / UnmadeCore::FractureModel::RecoveryPerSecond);
            if (GEngine) UnmadeRpgFeedback(-1, 5.f, FColor::Green,
                TEXT("Healing consumed. Health and any Strain recovery applied."));
            return;
        }
    if (GEngine) UnmadeRpgFeedback(-1, 5.f, FColor::Silver,
        TEXT("No useful healing consumable in your bag."));
}

void AUnmadeCharacter::UseStrainPotion()
{
    if (!IsValid(Equipment)) return;
    int32 Restored = 0;
    const UnmadeCore::ItemId Potions[] = {
        UnmadeCore::ItemId::StrainVial, UnmadeCore::ItemId::AshOfPossibleLives
    };
    const auto result=UnmadeCore::UseFirstConsumable(Potions,[&](auto Id){
        return Equipment->UseConsumable(Id,FractureModel.CurrentStrain(),Restored);
    });
    if(result==UnmadeCore::ConsumableCommit::SaveFailed){
        UnmadeRpgFeedback(-1,8.f,FColor::Red,
            TEXT("Potion could not be saved. Nothing consumed or restored; retry after checking Save Profiles."));
        return;
    }
    if(result==UnmadeCore::ConsumableCommit::Used)
        {
            if (Restored > 0) FractureModel.Recover(Restored / UnmadeCore::FractureModel::RecoveryPerSecond);
            if (GEngine) UnmadeRpgFeedback(-1, 5.f, FColor::Cyan,
                TEXT("Stillness restored: Reality Strain diminished."));
            return;
        }
    if (GEngine) UnmadeRpgFeedback(-1, 5.f, FColor::Silver,
        TEXT("No useful Strain elixir remains."));
}

void AUnmadeCharacter::ForgeMythicGear()
{
    if (!GetWorld() || !IsValid(Equipment)) return;
    bool NearForge = false;
    for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
    {
        if (It->ActorHasTag(FName("Bellwold.Workshop")) &&
            FVector::DistSquared(GetActorLocation(), It->GetActorLocation()) < FMath::Square(550.f))
        {
            NearForge = true;
            break;
        }
    }
    if (!NearForge)
    {
        if (GEngine) UnmadeRpgFeedback(-1, 6.f, FColor::Yellow,
            TEXT("Waybreaker can only be forged at Bellwold's old workshop."));
        return;
    }
    if (!Equipment->ForgeWaybreaker() && GEngine)
        UnmadeRpgFeedback(-1, 7.f, FColor::Yellow,
            TEXT("The forge requires both village stories, all three villages, two Echo Glass, Bellmetal and Archive Ink."));
}

bool AUnmadeCharacter::IsBorrowedLifeActive() const
{
    return IsValid(Tenfold) && Tenfold->IsBorrowedIdentityActive();
}
int32 AUnmadeCharacter::GetBorrowedLifeRole() const
{
    return IsValid(Tenfold)?Tenfold->BorrowedLifeRole():-1;
}
int32 AUnmadeCharacter::GetBorrowedCraftLevel() const
{
    return GetBorrowedLifeRole()==1?2:0;
}

bool AUnmadeCharacter::HasNearbyHollowKeeper() const
{
    UWorld* World=GetWorld();
    if(!World)return false;
    for(TActorIterator<AUnmadeBossCharacter> It(World);It;++It)
    {
        if(It->GetBossId()!=UnmadeCore::BossId::HollowBell ||
           !It->CanBeTargeted() ||
           FVector::DistSquared(GetActorLocation(),It->GetActorLocation())>FMath::Square(1400.f))
            continue;
        FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeRiteKeeperSight),false);
        Sight.AddIgnoredActor(this);
        Sight.AddIgnoredActor(*It);
        if(!World->LineTraceTestByChannel(GetActorLocation()+FVector(0,0,70),
            It->GetActorLocation()+FVector(0,0,70),ECC_Visibility,Sight))
            return true;
    }
    return false;
}

void AUnmadeCharacter::ApplyRiteAbility(UnmadeCore::RiteId Rite,
    const UnmadeCore::RiteEffect& Effect,int32 SelectedLaw)
{
    UWorld* World=GetWorld();
    if(!World || Effect.result!=UnmadeCore::RiteResult::Applied)return;
    const double Now=World->GetTimeSeconds();
    switch(Rite)
    {
    case UnmadeCore::RiteId::UnwriteLaw:
        if(SelectedLaw==0)LaunchCharacter(FVector(0,0,520),false,true);
        else
        {
            for(TActorIterator<AUnmadeEnemyCharacter> It(World);It;++It)
            {
                if(!It->CanBeTargeted() ||
                   FVector::DistSquared(GetActorLocation(),It->GetActorLocation())>FMath::Square(850.f))
                    continue;
                if(SelectedLaw==1)It->ExposeToFold(Now,Effect.duration);
                else
                {
                    const FVector Away=(It->GetActorLocation()-GetActorLocation()).GetSafeNormal2D();
                    It->AddActorWorldOffset(Away*310.f,true);
                }
            }
        }
        break;
    case UnmadeCore::RiteId::UnderstandingBosses:
        for(TActorIterator<AUnmadeBossCharacter> It(World);It;++It)
        {
            if(It->GetBossId()!=UnmadeCore::BossId::HollowBell || !It->CanBeTargeted() ||
               FVector::DistSquared(GetActorLocation(),It->GetActorLocation())>FMath::Square(1400.f))
                continue;
            FCollisionQueryParams Sight(SCENE_QUERY_STAT(UnmadeMercySight),false);
            Sight.AddIgnoredActor(this);Sight.AddIgnoredActor(*It);
            if(World->LineTraceTestByChannel(GetActorLocation()+FVector(0,0,70),
                It->GetActorLocation()+FVector(0,0,70),ECC_Visibility,Sight))continue;
            It->ResolveByMercy();
            break;
        }
        for(TActorIterator<AUnmadeRealmGuardian> Guardian(World);Guardian;++Guardian)
            if(FVector::DistSquared(GetActorLocation(),Guardian->GetActorLocation())
                <FMath::Square(1000.f))
                Guardian->ExposeToFold(Now,5.0);
        break;
    case UnmadeCore::RiteId::BorrowedLives:
        if(GEngine)UnmadeRpgFeedback(-1,7.f,FColor::Cyan,
            TEXT("ANOTHER LIFE: an unlived fighter's instincts change your attack temporarily."));
        break;
    case UnmadeCore::RiteId::LegacyForging:
        if(GEngine)UnmadeRpgFeedback(-1,7.f,FColor::Cyan,
            TEXT("LEGACY STEEL: distinct deeds remembered by your blade increase attack."));
        break;
    case UnmadeCore::RiteId::TomorrowDebt:
        if(GEngine)UnmadeRpgFeedback(-1,8.f,FColor::Orange,
            TEXT("FUTURE BORROWED: powerful for now; the debt must be repaid tomorrow."));
        break;
    case UnmadeCore::RiteId::Oathbinding:
        if(GEngine)UnmadeRpgFeedback(-1,8.f,FColor::Cyan,
            TEXT("OATH ARMOR: a promise carries weight. Breaking it has permanent consequences."));
        break;
    default: break;
    }
}
void AUnmadeCharacter::ReportRealityConsequenceEvent(UnmadeCore::RiteId Id)
{
    const int Index=static_cast<int>(Id);
    if(Index<0 || Index>=static_cast<int>(UnmadeCore::RealityProfiles.size()))return;
    const bool bCare=UnmadeCore::RealityProfiles[Index].care;
    const auto* Policy=UnmadeCore::RealmReactionAt(
        GetActorLocation().X,GetActorLocation().Y);
    if(!Policy)return;
    const FName Source(*FString::Printf(TEXT("Reality.Realm.%d"),
        static_cast<int32>(Policy->realm)));
    ReportLocalEvent(FName(bCare?TEXT("Reality.CareWitnessed"):
        TEXT("Reality.InquiryWitnessed")),Source);
}

void AUnmadeCharacter::ReportRiteWitnessEvent()
{
    ReportLocalEvent(FName("Reality.Rite"),FName("TheUnmade.Tenfold"));
}
void AUnmadeCharacter::ReportRiteChoiceEvent(int32 Choice)
{
    if(Choice==1)ReportLocalEvent(FName("World.RiteProtect"),FName("TheUnmade.Tenfold"));
    else if(Choice==2)ReportLocalEvent(FName("World.RiteReveal"),FName("TheUnmade.Tenfold"));
}
void AUnmadeCharacter::ReportBrokenOathEvent()
{
    ReportLocalEvent(FName("World.BrokenOath"),FName("Bellwold.Refuge"));
}
void AUnmadeCharacter::ReportRedeemedOathEvent()
{
    ReportLocalEvent(FName("World.RedeemedOath"),FName("Bellwold.Refuge"));
}

void AUnmadeCharacter::CrossFrontierGateway()
{
    if(!GetWorld() || Combat->IsDefeated())return;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        if(!Hub->TryTravelFrontier(this))
        {
            if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Yellow,
                TEXT("No reachable realm crossing. Look for the marked stone gateways."));
        }
        else
        {
            ReconcileEarnedRewards();
        }
        break;
    }
}

void AUnmadeCharacter::BuyMarketSupplies()
{
    if(!IsValid(Equipment) || !GetWorld() || !Combat || Combat->IsDefeated())return;
    const FVector P=GetActorLocation();
    const auto Village=UnmadeCore::SettlementAt(P.X,P.Y);
    if(Village==UnmadeCore::SettlementId::None)return;
    bool bMerchantNearby=false;
    for(TActorIterator<AUnmadeNpcCharacter> It(GetWorld());It;++It)
    {
        if(FVector::DistSquared(P,It->GetActorLocation())>FMath::Square(340.f))continue;
        const FString Identity=It->GetStableId().ToString();
        const FTCHARToUTF8 Utf8(*Identity);
        const auto* Resident=UnmadeCore::FindResident(Utf8.Get());
        if(Resident && Resident->home==Village &&
           Resident->role==UnmadeCore::NpcRole::Merchant &&
           It->CanTradeWithPlayer())
        {
            FCollisionQueryParams sight(SCENE_QUERY_STAT(UnmadeTradeSight),false);sight.AddIgnoredActor(this);sight.AddIgnoredActor(*It);
            bool clear=!GetWorld()->LineTraceTestByChannel(P+FVector(0,0,55),It->GetActorLocation()+FVector(0,0,55),ECC_Visibility,sight);
            if(!UnmadeCore::PhysicalTradeAllowed(!Combat->IsDefeated(),true,FVector::DistSquared(P,It->GetActorLocation()),clear))continue;
            bMerchantNearby=true;
            break;
        }
    }
    if(!bMerchantNearby)
    {
        if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Yellow,
            TEXT("Trade: find a merchant in this village first."));
        return;
    }
    const auto Id=SelectedTrade!=UnmadeCore::ItemId::Count?SelectedTrade:Village==UnmadeCore::SettlementId::Bellwold
        ? UnmadeCore::ItemId::WildHerbs
        : Village==UnmadeCore::SettlementId::Paperhaven
          ? UnmadeCore::ItemId::BlankParchment : UnmadeCore::ItemId::IronScrap;
    int Trust=0;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        const auto Home=static_cast<UnmadeCore::Community>(static_cast<int32>(Village));
        Trust=Hub->GetCommunityOutcome(Home).tradeTrust;
        // The existing real market price calculation receives a bounded,
        // time-limited supply/trust credit only in the local Crossings.
        Trust+=Hub->GetRealmTradeTrustBonus(P);
        break;
    }
    if(Equipment->Buy(Id,Village,Trust))
    {
        if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Green,
            TEXT("Local materials purchased. Press I to view remaining marks."));
    }
    else if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Yellow,
        TEXT("Trade failed: insufficient marks, no bag space, or unavailable stock."));
}

void AUnmadeCharacter::SellMarketSupplies()
{
    if(!IsValid(Equipment) || !GetWorld() || !Combat || Combat->IsDefeated())return;
    const FVector P=GetActorLocation();
    const auto Village=UnmadeCore::SettlementAt(P.X,P.Y);
    if(Village==UnmadeCore::SettlementId::None)return;
    bool bMerchantNearby=false;
    for(TActorIterator<AUnmadeNpcCharacter> It(GetWorld());It;++It)
    {
        if(FVector::DistSquared(P,It->GetActorLocation())>FMath::Square(340.f))continue;
        const FString Identity=It->GetStableId().ToString();
        const FTCHARToUTF8 Utf8(*Identity);
        const auto* Resident=UnmadeCore::FindResident(Utf8.Get());
        if(Resident && Resident->home==Village &&
           Resident->role==UnmadeCore::NpcRole::Merchant &&
           It->CanTradeWithPlayer())
        {
            FCollisionQueryParams sight(SCENE_QUERY_STAT(UnmadeTradeSight),false);sight.AddIgnoredActor(this);sight.AddIgnoredActor(*It);
            bool clear=!GetWorld()->LineTraceTestByChannel(P+FVector(0,0,55),It->GetActorLocation()+FVector(0,0,55),ECC_Visibility,sight);
            if(!UnmadeCore::PhysicalTradeAllowed(!Combat->IsDefeated(),true,FVector::DistSquared(P,It->GetActorLocation()),clear))continue;
            bMerchantNearby=true;
            break;
        }
    }
    if(!bMerchantNearby)return;
    const auto Id=SelectedTrade!=UnmadeCore::ItemId::Count?SelectedTrade:Village==UnmadeCore::SettlementId::Bellwold
        ? UnmadeCore::ItemId::WildHerbs
        : Village==UnmadeCore::SettlementId::Paperhaven
          ? UnmadeCore::ItemId::BlankParchment : UnmadeCore::ItemId::IronScrap;
    if(!Equipment->Sell(Id,Village) && GEngine)
        UnmadeRpgFeedback(-1,6.f,FColor::Yellow,
            TEXT("No tradable surplus of this village's commodity."));
}

void AUnmadeCharacter::CraftLocalRecipe(){OpenRpgPage(5);}
void AUnmadeCharacter::CraftRecipeSelection(int Recipe)
{
    if(!GetWorld()||!Combat||Combat->IsDefeated())return;
    if(Recipe>=6&&Recipe<15){for(TActorIterator<AUnmadePrototypeHub> h(GetWorld());h;++h){h->TryFieldCraft(this,Recipe-6);return;}}
    if(!IsValid(Equipment) || !GetWorld())return;
    const FVector P=GetActorLocation();
    const auto Village=UnmadeCore::SettlementAt(P.X,P.Y);
    const FName Station=Village==UnmadeCore::SettlementId::Bellwold
        ? FName("Bellwold.Workshop") :
        Village==UnmadeCore::SettlementId::Paperhaven
        ? FName("Paperhaven.Scriptorium") : NAME_None;
    if(Station.IsNone())return;
    bool bNearStation=false;
    for(TActorIterator<AStaticMeshActor> It(GetWorld());It;++It)
    {
        if(It->ActorHasTag(Station) &&
           FVector::DistSquared(P,It->GetActorLocation())<FMath::Square(650.f))
        {
            bNearStation=true;
            break;
        }
    }
    if(!bNearStation)
    {
        if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Yellow,
            TEXT("Find the village's workshop or scriptorium to practice a profession."));
        return;
    }
    if(!Equipment->CraftAt(Village,Recipe) && GEngine)
        UnmadeRpgFeedback(-1,6.f,FColor::Yellow,
            TEXT("No known recipe can be made here with your current skill and materials."));
}

void AUnmadeCharacter::CommitSolidarity()
{
    CommitFaction(UnmadeCore::FactionEnding::Solidarity);
}
void AUnmadeCharacter::CommitTruth()
{
    CommitFaction(UnmadeCore::FactionEnding::Truth);
}
void AUnmadeCharacter::CommitFaction(UnmadeCore::FactionEnding Ending)
{
    if(!GetWorld())return;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        FName Scope=NAME_None;FString Consequence;
        if(!Hub->GetNearbyCommitPreview(static_cast<int32>(Ending),Scope,Consequence))
        {
            CommitmentGate.Cancel();
            if(GEngine)UnmadeRpgFeedback(-1,7.f,FColor::Yellow,
                TEXT("No matching final witness and evidence nearby for this outcome."));
            break;
        }
        const FString ScopeValue=Scope.ToString();
        const auto Attempt=CommitmentGate.Attempt(
            std::string(TCHAR_TO_UTF8(*ScopeValue)),static_cast<int32>(Ending),
            GetWorld()->GetTimeSeconds());
        if(Attempt!=UnmadeCore::CommitmentAttempt::Confirmed)
        {
            if(GEngine)UnmadeRpgFeedback(-1,8.f,FColor::Yellow,
                FString::Printf(TEXT("PERMANENT DECISION: %s | Press SAME choice by SAME witness within six seconds."),
                    *Consequence));
            break;
        }
        if(Hub->ResolvePreviewedCommit(Scope,static_cast<int32>(Ending)))
            ReconcileEarnedRewards();
        else if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Silver,
            TEXT("The confirmed decision was not applied. Check its witness and saving status, then preview and confirm again."));
        break;
    }
}

void AUnmadeCharacter::ChooseShelter()
{
    ChooseLocalConflict(UnmadeCore::ConflictChoice::Shelter);
}

void AUnmadeCharacter::ChooseResearch()
{
    ChooseLocalConflict(UnmadeCore::ConflictChoice::Research);
}

bool AUnmadeCharacter::SaveLocalConflict()
{
    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return false;

    const UnmadeCore::ConflictSnapshot Snapshot = LocalConflict.Snapshot();
    Save->bHasConflictSnapshot = true;
    Save->LocalConflictChoice = Snapshot.choice;
    Save->SupplyActivityStage = Snapshot.supplies;
    return UnmadeSave::SaveGameToSlot(Save,this);
}

void AUnmadeCharacter::ApplyConflictGates()
{
    if (!GetWorld()) return;
    for (TActorIterator<AUnmadeConflictGate> It(GetWorld()); It; ++It)
    {
        if (It->GetGateId() == FName("gate.prototype.shelter"))
            It->SetAccess(LocalConflict.ShelterOpen());
        else if (It->GetGateId() == FName("gate.prototype.archive"))
            It->SetAccess(LocalConflict.ArchiveOpen());
    }
}

void AUnmadeCharacter::ChooseLocalConflict(UnmadeCore::ConflictChoice Choice)
{
    if (!GetWorld() || !IsValid(FindNearbyFractureAnchor()))
    {
        PendingStoryChoice = UnmadeCore::ConflictChoice::None;
        if (GEngine) UnmadeRpgFeedback(-1, 6.f, FColor::Yellow,
            TEXT("Approach the central fracture before deciding the settlement dispute."));
        return;
    }
    const double Now = GetWorld()->GetTimeSeconds();
    const bool bConfirmed = PendingStoryChoice == Choice && Now <= PendingStoryExpiresAt;
    if (!bConfirmed)
    {
        PendingStoryChoice = UnmadeCore::ConflictChoice::None;
        if (LocalConflict.Preview(Choice) != UnmadeCore::ConflictResult::NeedsConfirmation)
        {
            if (GEngine) UnmadeRpgFeedback(-1, 7.f, FColor::Yellow,
                TEXT("This dispute is already resolved. Your decision cannot be overridden."));
            return;
        }
        PendingStoryChoice = Choice;
        PendingStoryExpiresAt = Now + 6.0;
        if (GEngine)
        {
            const FString Warning = Choice == UnmadeCore::ConflictChoice::Shelter
                ? TEXT("SUPPORT SHELTER: open the community passage, close the research route. Press Z / D-pad Up again within six seconds.")
                : TEXT("SUPPORT RESEARCH: open the archive passage, close the shelter route. Press X / D-pad Down again within six seconds.");
            UnmadeRpgFeedback(-1, 7.f, FColor::Orange, Warning);
        }
        return;
    }

    PendingStoryChoice = UnmadeCore::ConflictChoice::None;
    const UnmadeCore::ConflictSnapshot Previous = LocalConflict.Snapshot();
    if (LocalConflict.Commit(Choice, true) != UnmadeCore::ConflictResult::Committed)
        return;
    if (!CommitLocalEvent(Choice==UnmadeCore::ConflictChoice::Shelter?FName("World.ConflictShelter"):FName("World.ConflictResearch"),
        FName("region.prototype.hub"),[&]{return SaveLocalConflict();}))
    {
        LocalConflict.Restore(Previous);
        if (GEngine) UnmadeRpgFeedback(-1, 7.f, FColor::Red,
            TEXT("Dispute decision could not be saved; no world change was applied."));
        return;
    }

    ApplyConflictGates();
    ReconcileEarnedRewards();
    for (TActorIterator<AUnmadePrototypeHub> Hub(GetWorld()); Hub; ++Hub)
    {
        Hub->RefreshDistrictMood();
        Hub->RefreshCommunityConsequences();
    }
    const bool bShelter = Choice == UnmadeCore::ConflictChoice::Shelter;
    if (GEngine) UnmadeRpgFeedback(-1, 8.f, FColor::Yellow,
        bShelter ? TEXT("COMMUNITY ROUTE OPEN; archive closed. This decision persists.")
                 : TEXT("ARCHIVE ROUTE OPEN; community shelter closed. This decision persists."));
}

void AUnmadeCharacter::ProgressSupplyActivity()
{
    UWorld* World = GetWorld();
    if (!World || Combat->IsDefeated()) return;
    bool bNearMarket = false;
    bool bNearShelter = false;
    for (TActorIterator<AStaticMeshActor> It(World); It; ++It)
    {
        if (FVector::DistSquared(GetActorLocation(), It->GetActorLocation()) > FMath::Square(530.f))
            continue;
        bNearMarket = bNearMarket || It->ActorHasTag(FName("Hub.Market"));
        bNearShelter = bNearShelter || It->ActorHasTag(FName("Hub.Shelter"));
    }

    const auto Previous = LocalConflict.Snapshot();
    const bool bCompletedDelivery = LocalConflict.Supplies() == UnmadeCore::SupplyStage::Carrying;
    const bool bProgressed = bCompletedDelivery
        ? LocalConflict.DeliverSupplies(bNearShelter)
        : LocalConflict.CollectSupplies(bNearMarket);
    if (!bProgressed)
    {
        if (GEngine) UnmadeRpgFeedback(-1, 6.f, FColor::Yellow,
            TEXT("Supply errand: collect once at the market, then deliver at the shelter. Completed deliveries cannot be repeated."));
        return;
    }

    if (!(bCompletedDelivery?CommitLocalEvent(FName("Player.DeliveredSupplies"),FName("Hub.Shelter"),
        [&]{return SaveLocalConflict();}):SaveLocalConflict()))
    {
        LocalConflict.Restore(Previous);
        if (GEngine) UnmadeRpgFeedback(-1, 6.f, FColor::Red,
            TEXT("Supply activity could not be saved; progress restored."));
        return;
    }
    if (GEngine) UnmadeRpgFeedback(-1, 7.f, FColor::Cyan,
        bCompletedDelivery ? TEXT("SUPPLIES DELIVERED: the shelter remembers your help.")
                           : TEXT("SUPPLIES COLLECTED: bring these provisions to the shelter."));
    if (bCompletedDelivery)
    {
        ReconcileEarnedRewards();
    }
}

void AUnmadeCharacter::ShowEvidenceNotebook()
{
    if(!GetWorld())return;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        FString Evidence=Hub->GetEvidenceNotebook();
        Evidence+=TEXT("\n\n")+GetStudyJournal();
        // Text-mode input remains useful in the graybox; this same function
        // is BlueprintPure on the hub for a proper accessible UMG notebook.
        UE_LOG(LogTemp,Display,TEXT("%s"),*Evidence);
        if(GEngine)UnmadeRpgFeedback(-1,16.f,FColor::Cyan,Evidence);
        return;
    }
}

void AUnmadeCharacter::ShowStoryJournal()
{
    if(RpgWidget){OpenRpgPage(0);return;}
    if(GetWorld()&&GEngine)for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
        UnmadeRpgFeedback(-1,12.f,FColor::Cyan,Hub->GetCinderholdJournal());

    if(GetWorld()&&GEngine)for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
        UnmadeRpgFeedback(-1,15.f,FColor::Cyan,Hub->GetExpeditionJournal()+TEXT("\n")+Hub->GetRoadJournal());
    ReconcileEarnedRewards();
    FString Decision = TEXT("unresolved");
    if (LocalConflict.Choice() == UnmadeCore::ConflictChoice::Shelter)
        Decision = TEXT("community shelter supported");
    else if (LocalConflict.Choice() == UnmadeCore::ConflictChoice::Research)
        Decision = TEXT("research archive supported");

    FString Supply = TEXT("available: visit market");
    if (LocalConflict.Supplies() == UnmadeCore::SupplyStage::Carrying)
        Supply = TEXT("carrying: deliver at shelter");
    else if (LocalConflict.Supplies() == UnmadeCore::SupplyStage::Delivered)
        Supply = TEXT("delivered");

    const int32 Clues = Lexicon ? Lexicon->GetClueCount() : 0;
    FString WorldTime = TEXT("unknown");
    int32 SitesSeen = 0;
    int32 VillagesSeen = 0;
    FString VillageName = TEXT("unknown");
    int32 BellwoldQuest = 0;
    int32 PaperhavenQuest = 0;
    FString FactionSummary;
    FString RealmSummary;
    FString MainRoadSummary;
    FString WitnessEchoSummary;
    FString RealityConsequenceSummary;
    FString MaterialSummary;
    FString LivelihoodSummary;
    FString BesHouseholdSummary;
    if (GetWorld())
    {
        for (TActorIterator<AUnmadePrototypeHub> Hub(GetWorld()); Hub; ++Hub)
        {
            const int32 Minute = Hub->GetMinuteOfDay();
            WorldTime = FString::Printf(TEXT("day %d at %02d:%02d"),
                Hub->GetGameDay(), Minute / 60, Minute % 60);
            SitesSeen = Hub->GetDiscoveredCount();
            VillageName = Hub->GetCurrentVillageName();
            VillagesSeen = Hub->GetVisitedVillageCount();
            BellwoldQuest = static_cast<int32>(Hub->GetRegionalTask(UnmadeCore::SettlementId::Bellwold));
            PaperhavenQuest = static_cast<int32>(Hub->GetRegionalTask(UnmadeCore::SettlementId::Paperhaven));
            FactionSummary=FString::Printf(TEXT("Faction chapters: Refuge %d/3, Archive %d/3, Roadbound %d/3"),
                Hub->FactionStage(UnmadeCore::Faction::Refuge),
                Hub->FactionStage(UnmadeCore::Faction::Archive),
                Hub->FactionStage(UnmadeCore::Faction::Roadbound));
            RealmSummary=Hub->GetCurrentRealmName(GetActorLocation());
            MainRoadSummary=Hub->DescribeUnansweredRoad(this);
            WitnessEchoSummary=Hub->GetWitnessEchoJournal();
            RealityConsequenceSummary=Hub->GetRealityConsequenceJournal();
            MaterialSummary=Hub->GetLocalMaterialJournal();
            LivelihoodSummary=Hub->GetLivingEconomyJournal();
            BesHouseholdSummary=Hub->GetBesHouseholdJournal();
            if(Hub->GetAfterlightStage()>0)
                RealmSummary+=FString::Printf(
                    TEXT(" | Bellwold Afterlight: %d/4"),Hub->GetAfterlightStage());
            for(const auto At:{UnmadeCore::Realm::WidowedRain,UnmadeCore::Realm::HearthBeneath})
            {
                const int32 Stage=Hub->GetRealmAftermathStage(At);
                if(Stage>0)
                    RealmSummary+=FString::Printf(TEXT(" | %s return: %d/4, outcome %d"),
                        At==UnmadeCore::Realm::WidowedRain?TEXT("Saltwake"):TEXT("Cinderhold"),
                        Stage,Hub->GetRealmAftermathEnding(At));
            }
            for(const auto& Region:UnmadeCore::LaterRealms)
            {
                if(Hub->GetLaterRealmStage(Region.realm)>0)
                    RealmSummary+=FString::Printf(TEXT(" | %s first %d/3, return %d/4"),
                        UTF8_TO_TCHAR(Region.name),
                        Hub->GetLaterRealmStage(Region.realm),
                        Hub->GetRealmAftermathStage(Region.realm));
                if(Hub->GetEchoQuestStage(Region.realm)>0)
                    RealmSummary+=FString::Printf(TEXT(" | Echo %d/3, clues %d/3, ending %d"),
                        Hub->GetEchoQuestStage(Region.realm),
                        Hub->GetEchoQuestEvidence(Region.realm),
                        Hub->GetEchoQuestEnding(Region.realm));
            }
            RealmSummary+=TEXT(" | ");
            RealmSummary+=Hub->DescribeCommunityAt(GetActorLocation());
            RealmSummary+=FString::Printf(TEXT(" | Beyond the Reach: %d/2 discovered"),
                ((Hub->GetFrontierVisitMask()&1)?1:0)+((Hub->GetFrontierVisitMask()&2)?1:0));
            break;
        }
    }
    if (GEngine) UnmadeRpgFeedback(-1, 10.f, FColor::Cyan,
        FString::Printf(TEXT("JOURNAL | %s | %s | Villages %d/3, landmarks %d/6 | Bellwold lanterns %d/2 | Paperhaven testimony %d/2 | Dispute: %s | Supplies: %s | VEYL clues: %d/2 | Strain: %.0f/100"),
            *WorldTime, *VillageName, VillagesSeen, SitesSeen,
            BellwoldQuest, PaperhavenQuest, *Decision, *Supply, Clues, FractureModel.CurrentStrain()));
    if(GEngine && !FactionSummary.IsEmpty())
        UnmadeRpgFeedback(-1,8.f,FColor::Cyan,FactionSummary);
    if(GEngine && !RealmSummary.IsEmpty())
        UnmadeRpgFeedback(-1,8.f,FColor::Cyan,RealmSummary);
    if(GEngine && !MainRoadSummary.IsEmpty())
        UnmadeRpgFeedback(-1,18.f,FColor::Yellow,MainRoadSummary);
    if(GEngine && !WitnessEchoSummary.IsEmpty())
        UnmadeRpgFeedback(-1,15.f,FColor::Cyan,WitnessEchoSummary);
    if(GEngine && !RealityConsequenceSummary.IsEmpty())
        UnmadeRpgFeedback(-1,13.f,FColor::Yellow,RealityConsequenceSummary);
    if(GEngine && !MaterialSummary.IsEmpty())
        UnmadeRpgFeedback(-1,16.f,FColor::Cyan,MaterialSummary);
    if(GEngine && !LivelihoodSummary.IsEmpty())
        UnmadeRpgFeedback(-1,16.f,FColor::Yellow,LivelihoodSummary);
    if(GEngine && !BesHouseholdSummary.IsEmpty())
        UnmadeRpgFeedback(-1,17.f,FColor::Cyan,BesHouseholdSummary);
}

void AUnmadeCharacter::AttemptMeleeAttack(){
 if(!GetWorld()||!Combat||Combat->IsDefeated()||IsRpgMenuOpen())return;UnmadeCore::WeaponStyle Style;for(TActorIterator<AUnmadePrototypeHub> h(GetWorld());h;++h){Style=h->PlayerWeaponStyle(Equipment?Equipment->EquippedWeapon():UnmadeCore::ItemId::Count);Combat->SetWeaponStyle(Style,h->GetActiveMastery());break;}
 double now=GetWorld()->GetTimeSeconds();if(!Combat->BeginWeaponStrike(now))return;for(TActorIterator<AUnmadePrototypeHub> h(GetWorld());h;++h){h->LearnExperienceTutorial(2,this);break;}WeaponStrikeStarted=now;WeaponStrikeRhythm=Style.cooldown;WeaponReach=Style.reach;bFocusProjectile=Equipment&&Equipment->RealmWeapon()<0&&Equipment->EquippedWeapon()==UnmadeCore::ItemId::RedactedScepter;PreviousWeaponTip=GetActorLocation()+FVector(0,0,55)+GetActorForwardVector().RotateAngleAxis(-45,FVector::UpVector)*WeaponReach;UnmadePresentation::Sound(this,TEXT("swing"));AnimatePlayer(FName(TEXT("Attack")));if(GetMesh())GetMesh()->SetPlayRate(1.f/Style.cooldown);PlayerAttackMotionUntil=now+Style.cooldown;
}
void AUnmadeCharacter::TickWeaponContact(){
 if(!GetWorld()||!Combat)return;double now=GetWorld()->GetTimeSeconds();if(Combat->IsDefeated()||Combat->IsStaggered(now)||Combat->IsGuarding()||IsRpgMenuOpen()){Combat->CancelWeaponStrike();return;}if(!Combat->ContactActive(now))return;
 if(bFocusProjectile){uint64 id=0;double damage=0;UnmadeCore::CombatStatus status=UnmadeCore::CombatStatus::None;if(Combat->ReservedPlayerShot(now,id,damage,status)){FVector muzzle=GetActorLocation()+FVector(0,0,55)+GetActorForwardVector()*80;if(auto* shot=GetWorld()->SpawnActor<AUnmadeTravelingProjectile>(muzzle,FRotator::ZeroRotator))shot->Fire(this,muzzle+GetControlRotation().Vector()*WeaponReach,damage,id,1200,WeaponReach,status);}return;}
 FVector start=GetActorLocation()+FVector(0,0,55);double contact=FMath::Clamp(WeaponStrikeRhythm*.22,.10,.25),width=FMath::Clamp(WeaponStrikeRhythm*.22,.10,.22);double f=FMath::Clamp((now-WeaponStrikeStarted-contact)/width,0.,1.);FVector tip=start+GetActorForwardVector().RotateAngleAxis(-45+90*f,FVector::UpVector)*WeaponReach;
 if(GetMesh()&&GetMesh()->DoesSocketExist(TEXT("WeaponTip")))tip=GetMesh()->GetSocketLocation(TEXT("WeaponTip"));
 FCollisionObjectQueryParams objects;objects.AddObjectTypesToQuery(ECC_Pawn);FCollisionQueryParams q(SCENE_QUERY_STAT(WeaponContactSweep),false);q.AddIgnoredActor(this);TArray<FHitResult> hits;
 // Sweep the blade span and its previous tip; LOS excludes targets behind cover.
 GetWorld()->SweepMultiByObjectType(hits,start,tip,FQuat::Identity,objects,FCollisionShape::MakeSphere(35),q);TArray<FHitResult> arc;GetWorld()->SweepMultiByObjectType(arc,PreviousWeaponTip,tip,FQuat::Identity,objects,FCollisionShape::MakeSphere(35),q);hits.Append(arc);PreviousWeaponTip=tip;
 for(const auto& hit:hits){auto* enemy=Cast<AUnmadeEnemyCharacter>(hit.GetActor());if(!enemy||!enemy->CanBeTargeted())continue;FVector toward=enemy->GetActorLocation()-start;if(toward.Size2D()>WeaponReach+65||FVector::DotProduct(GetActorForwardVector(),toward.GetSafeNormal2D())<.2)continue;FCollisionQueryParams sight(q);sight.AddIgnoredActor(enemy);if(GetWorld()->LineTraceTestByChannel(start,enemy->GetActorLocation()+FVector(0,0,55),ECC_Visibility,sight))continue;float before=enemy->GetCombat()->GetHealth();if(Combat->ResolveWeaponContact(enemy->GetCombat(),now,enemy->IsFractureExposed(now)))HandleMeleeImpact(enemy,before);break;}
}
void AUnmadeCharacter::HandleMeleeImpact(AUnmadeEnemyCharacter* Target,float Before){if(!Target)return;Target->NotifyPlayerHit(Before-Target->GetCombat()->GetHealth());
        if(Target->GetCombat() && Before>Target->GetCombat()->GetHealth())
        {
            UnmadeCore::CreatureKind Species;
            if(Target->StudyCreature(Species)){
                UnmadeCore::StudyId StudyId=UnmadeCore::StudyId::Count;
                if(Species==UnmadeCore::CreatureKind::Cinderjaw)StudyId=UnmadeCore::StudyId::Cinderjaw;
                else if(Species==UnmadeCore::CreatureKind::PaleStrider)StudyId=UnmadeCore::StudyId::PaleStrider;
                else if(Species==UnmadeCore::CreatureKind::RedactionEye)StudyId=UnmadeCore::StudyId::RedactionEye;
                if(StudyId!=UnmadeCore::StudyId::Count)RecordStudyEvidence(StudyId,2);
            }
        }
        if (Target->GetCombat()->IsDefeated() && IsValid(Equipment))
        {
            AUnmadeBossCharacter* Boss=Cast<AUnmadeBossCharacter>(Target);
            if(Boss)Boss->CommitDefeatReceipt();
            else {
                if(!Target->IsTemporaryArenaEscort()){
                    CombatRewards.Queue(Target->GetEnemyStyle());
                    RetryCombatRewards();
                }
            }
        }
    ReportLocalEvent(FName("Player.Fought"),FName("combat.prototype.enemy"));
}

void AUnmadeCharacter::NotifyStatusDefeat(AUnmadeEnemyCharacter* Target)
{
    if(!IsValid(Target)||Target->GetWorld()!=GetWorld()||!IsValid(Target->GetCombat())||!Target->GetCombat()->IsDefeated())return;
    if(auto* Boss=Cast<AUnmadeBossCharacter>(Target))Boss->CommitDefeatReceipt();
    else if(!Target->IsTemporaryArenaEscort()){CombatRewards.Queue(Target->GetEnemyStyle());RetryCombatRewards();}
    // Delayed remote damage does not invent a fresh nearby fight witness.
}

void AUnmadeCharacter::StartGuard()
{
    if(IsRpgMenuOpen())return;
    AnimatePlayer(FName(TEXT("Guard")));
    if (IsValid(Combat)) Combat->SetGuarding(true);
}

void AUnmadeCharacter::StopGuard()
{
    if (IsValid(Combat)) Combat->SetGuarding(false);
}

void AUnmadeCharacter::TurnView(float Value){AddControllerYawInput(Value*GetDefault<UUnmadePlayerPreferences>()->CameraSensitivity);}
void AUnmadeCharacter::LookView(float Value){AddControllerPitchInput(Value*GetDefault<UUnmadePlayerPreferences>()->CameraSensitivity);}
void AUnmadeCharacter::MoveForward(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator YawOnly(0.f, Controller->GetControlRotation().Yaw, 0.f);
    AddMovementInput(FRotationMatrix(YawOnly).GetUnitAxis(EAxis::X), Value);
}

void AUnmadeCharacter::MoveRight(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value))
    {
        return;
    }

    const FRotator YawOnly(0.f, Controller->GetControlRotation().Yaw, 0.f);
    AddMovementInput(FRotationMatrix(YawOnly).GetUnitAxis(EAxis::Y), Value);
}

namespace
{
AUnmadeNpcCharacter* FindNearbyCitizen(UWorld* World, FVector Origin, float MaxDistance)
{
    AUnmadeNpcCharacter* Nearest = nullptr;
    float BestDistSq = FMath::Square(MaxDistance);
    for (TActorIterator<AUnmadeNpcCharacter> It(World); It; ++It)
    {
        const float DistSq = FVector::DistSquared(Origin, It->GetActorLocation());
        if (DistSq < BestDistSq)
        {
            FCollisionQueryParams sight(SCENE_QUERY_STAT(ResidentConversationSight),false);sight.AddIgnoredActor(*It);if(auto* player=UGameplayStatics::GetPlayerCharacter(World,0))sight.AddIgnoredActor(player);if(World->LineTraceTestByChannel(Origin+FVector(0,0,50),It->GetActorLocation()+FVector(0,0,50),ECC_Visibility,sight))continue;
            Nearest = *It;
            BestDistSq = DistSq;
        }
    }
    return Nearest;
}
}

void AUnmadeCharacter::SkipDirectedScene()
{
    if(IsRpgMenuOpen()){RpgWidget->Close();return;}
    if(!GetWorld())return;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
        if(Hub->IsBesDirectedScenePlaying()){Hub->SkipBesDirectedScene();return;}
}

void AUnmadeCharacter::Interact()
{
    if(IsRpgMenuOpen()||!Combat||Combat->IsDefeated())return;
    AUnmadeNpcCharacter* Target = FindNearbyCitizen(GetWorld(), GetActorLocation(), 260.f);
    AUnmadeLoreSite* NearestSite = nullptr;
    double BestSiteDistSq = FMath::Square(280.f);
    if (GetWorld())
    {
        for (TActorIterator<AUnmadeLoreSite> It(GetWorld()); It; ++It)
        {
            const double DistSq = FVector::DistSquared(GetActorLocation(), It->GetActorLocation());
            if (DistSq >= BestSiteDistSq) continue;
            FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeLoreSight), false);
            Params.AddIgnoredActor(this);
            Params.AddIgnoredActor(*It);
            if (GetWorld()->LineTraceTestByChannel(
                GetActorLocation() + FVector(0, 0, 50),
                It->GetActorLocation() + FVector(0, 0, 50), ECC_Visibility, Params))
                continue;
            NearestSite = *It;
            BestSiteDistSq = DistSq;
        }

        // Prioritize whichever interactable is actually closer, avoiding NPCs
        // unintentionally hiding a landmark's inspection action.
        const bool bLoreIsCloser = IsValid(NearestSite) &&
            (!IsValid(Target) || BestSiteDistSq <
                FVector::DistSquared(GetActorLocation(), Target->GetActorLocation()));
        if (bLoreIsCloser)
        {
            for (TActorIterator<AUnmadePrototypeHub> Hub(GetWorld()); Hub; ++Hub)
            {
                if (Hub->InspectSite(NearestSite))
            {
                ReconcileEarnedRewards();
                return;
            }
            }
        }
    }
    // A world mechanism or clue can outrank a farther NPC; otherwise a closer
    // resident keeps the conversation action, as in Bellwold's lore inspection.
    if(GetWorld())
    {
        const double ResidentDistSq=IsValid(Target)
            ?FVector::DistSquared(GetActorLocation(),Target->GetActorLocation())
            :FMath::Square(310.f)+1.f;
        for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
        {
            if(Hub->TryExpeditionInteraction(this,ResidentDistSq))return;
            if(Hub->TryRoadInteraction(this,ResidentDistSq))return;
            if(Hub->TryCinderholdInteraction(this,ResidentDistSq))return;
            if(Hub->TryFieldSurveyInteraction(this,ResidentDistSq))return;
            if(Hub->TryHabitatInteraction(this,ResidentDistSq))return;
            if(Hub->TryPetitionRelayInteraction(this,ResidentDistSq))return;
            if(Hub->TryHearthPetitionInteraction(this,ResidentDistSq))return;
            if(Hub->TryBesHouseholdInteraction(this,ResidentDistSq))return;
            if(Hub->TryLivingTradeInteraction(this,ResidentDistSq))return;
            if(Hub->TryMaterialInteraction(this,ResidentDistSq))return;
            if(Hub->TryWitnessReturn(this,ResidentDistSq))return;
            if(Hub->TryWitnessDispatch(this,ResidentDistSq))return;
            if(Hub->TryFinalInteraction(this,ResidentDistSq))return;
            if(Hub->InspectUnansweredRoad(this,ResidentDistSq))return;
            if(Hub->TryCalmNearbyGuardian(this,ResidentDistSq))return;
            if(Hub->InspectResonanceArchive(this,ResidentDistSq))return;
            if(Hub->InspectEchoQuestSite(this,ResidentDistSq))return;
            if(Hub->InspectLaterRealmSite(this,ResidentDistSq))return;
            if(Hub->InspectRealmAftermathSite(this,ResidentDistSq))return;
        }
    }
    if (!Target)
    {
        if(GetWorld())
        {
            for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
                if(Hub->InspectAfterlightClue(this))return;
        }
        if(IsValid(Tenfold) && Tenfold->TryInspectRiteStone())return;
        if(GetWorld())
        {
            for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
                if(Hub->InspectFrontierClue(this))return;
        }
        if (IsValid(FindNearbyFractureAnchor()) && GEngine)
        {
            const FString Inscription = Lexicon && Lexicon->UnderstandsVeyl()
                ? TEXT("The inscription: VEYL - the way that remains. It marks a surviving passage.")
                : TEXT("The inscription is unfamiliar. Investigate it with Glimpse and ask the archivist.");
            UnmadeRpgFeedback(-1, 7.f, FColor::Cyan, Inscription);
        }
        else if (GEngine)
        {
            UnmadeRpgFeedback(-1, 6.f, FColor::Cyan,
                TEXT("Nothing nearby to inspect or speak to."));
        }
        return;
    }

    // Hand-authored village missions are triggered by conversations, never LLM text.
    if (GetWorld())
    {
        for (TActorIterator<AUnmadePrototypeHub> Hub(GetWorld()); Hub; ++Hub)
        {
            bool bCompletedVillageTask = false;
            if (Hub->TryResidentVillageTask(Target->GetStableId(), bCompletedVillageTask) &&
                bCompletedVillageTask)
            {
                ReconcileEarnedRewards();
            }
            Hub->TryFactionConversation(Target->GetStableId());
            Hub->TryFrontierConversation(Target->GetStableId());
            Hub->TryAfterlightConversation(Target->GetStableId());
            Hub->TryRealmAftermathConversation(Target->GetStableId());
            Hub->TryLaterRealmConversation(Target->GetStableId());
            Hub->TryEchoQuestConversation(Target->GetStableId());
            Hub->RecordResidentConversation(Target->GetStableId());
            Hub->RecordRoadConversation(Target->GetStableId());
            if(!Hub->RecordExperienceConversation(Target->GetStableId(),this))
                Hub->LearnExperienceTutorial(0,this);
            const bool bRelayChanged=Hub->RecordPetitionRelayConversation(Target->GetStableId());
            const bool bHouseChanged=Hub->RecordBesHouseholdConversation(Target->GetStableId());
            const bool bAdventureChanged=Hub->RecordCinderholdConversation(Target->GetStableId());
            if(!bRelayChanged&&!bHouseChanged&&!bAdventureChanged)
                Hub->MaybePlayBesReturnScene(Target->GetStableId());
            if(IsValid(Tenfold)) Tenfold->TryWitnessConversation(Target->GetStableId());
            break;
        }
    }

    if (Lexicon && Target->GetStableId() == FName("npc.archivist.001"))
    {
        Lexicon->RecordEvidence(FName("evidence.archivist"));
        ReconcileEarnedRewards();
    }
    UUnmadeLocalDialogueSubsystem* Dialogue = GetGameInstance()
        ? GetGameInstance()->GetSubsystem<UUnmadeLocalDialogueSubsystem>()
        : nullptr;
    if(RpgWidget)
    {
        RpgWidget->BeginNpcConversation(Target);
    }
    else if (Dialogue)
    {
        Dialogue->RequestDialogue(Target, TEXT("Hello."));
    }
    else if (GEngine)
    {
        UnmadeRpgFeedback(-1, 6.f, FColor::Cyan, Target->GetReactionText());
    }
}

void AUnmadeCharacter::OfferAid()
{
    AUnmadeNpcCharacter* Target = FindNearbyCitizen(GetWorld(), GetActorLocation(), 260.f);
    if (!Target||Target->GetStableId().IsNone())
    {
        if (GEngine) UnmadeRpgFeedback(-1, 5.f, FColor::Yellow,
            TEXT("Approach a resident before offering aid."));
        return;
    }
    bool AlreadyWitnessed=false;
    for(const auto& Observation:Target->GetMemory()->GetObservations())
        if(Observation.Evidence==EUnmadeEvidenceKind::Witnessed&&Observation.EventKind==FName("Player.Helped")&&
           Observation.SubjectId==Target->GetStableId()){AlreadyWitnessed=true;break;}
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub){
        Hub->RecordResidentAid(Target->GetStableId(),AlreadyWitnessed);
        return;
    }
}

void AUnmadeCharacter::DemonstrateAnomaly()
{
    if(!UnmadeSave::IsSessionWritable(this))return;
    if(GetWorld())for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        if(Hub->TryExpeditionAbility(this,0))return;
        if(Hub->TryRoadAbility(this,0))return;
        if(Hub->TryCinderholdAbility(this,0))return;
    }
    AUnmadeFractureAnchor* Anchor = FindNearbyFractureAnchor();
    if (!GetWorld()) return;
    const int32 Discount = IsValid(Equipment) ? Equipment->GlimpseDiscount() : 0;
    const auto Result=UnmadeCore::CommitFractureAction(FractureModel,
        [&]{return FractureModel.Glimpse(IsValid(Anchor),GetWorld()->GetTimeSeconds(),Discount);},[&]{
            auto Save=[&]{return CommitLocalEvent(FName("Reality.Anomaly"),FName("region.prototype.hub"),[&]{return SaveFractureState();});};
            return IsValid(Lexicon)?Lexicon->CommitEvidence(FName("evidence.glimpse"),Save):Save();
        });
    if(Result!=UnmadeCore::FractureCommit::Applied){
        UnmadeRpgFeedback(-1,7.f,FColor::Yellow,Result==UnmadeCore::FractureCommit::SaveFailed?
            TEXT("GLIMPSE NOT SAVED: Strain, clue and witness memories were restored. Try again when saving is available."):
            TEXT("Glimpse unavailable. Get closer to the fracture or recover Strain."));
        return;
    }
    ApplyFractureVisuals();
    ReconcileEarnedRewards();
    if (GEngine) UnmadeRpgFeedback(-1, 5.f, FColor::Cyan,
        FString::Printf(TEXT("GLIMPSE: another possible version flickers into view (+%d Strain)."),
            8 - Discount));
    UnmadePresentation::Sound(this,TEXT("cast"));
    PlayIdentityMotion(FName(TEXT("Cast")));
}

void AUnmadeCharacter::FoldReality()
{
    if(!UnmadeSave::IsSessionWritable(this))return;
    if(GetWorld())for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        if(Hub->TryExpeditionAbility(this,1))return;
        if(Hub->TryRoadAbility(this,1))return;
        if(Hub->TryCinderholdAbility(this,1))return;
    }
    AUnmadeFractureAnchor* Anchor = FindNearbyFractureAnchor();
    if (!GetWorld()) return;
    const int32 Extension = IsValid(Equipment) ? Equipment->FoldBonusSeconds() : 0;
    const auto Result=UnmadeCore::CommitFractureAction(FractureModel,
        [&]{return FractureModel.Fold(IsValid(Anchor),GetWorld()->GetTimeSeconds(),Extension);},
        [&]{return CommitLocalEvent(FName("Reality.Anomaly"),FName("region.prototype.hub"),[&]{return SaveFractureState();});});
    if(Result!=UnmadeCore::FractureCommit::Applied){
        UnmadeRpgFeedback(-1,7.f,FColor::Yellow,Result==UnmadeCore::FractureCommit::SaveFailed?
            TEXT("FOLD NOT SAVED: Strain and witness memories were restored; no barrier or enemy effect applied."):
            TEXT("Fold unavailable: already active, out of range, or Strain too high."));
        return;
    }
    ApplyFractureVisuals();
    for (TActorIterator<AUnmadeEnemyCharacter> It(GetWorld()); It; ++It)
    {
        if (!!It->CanBeTargeted() &&
            FVector::DistSquared(It->GetActorLocation(), Anchor->GetActorLocation()) < FMath::Square(800.f))
            It->ExposeToFold(GetWorld()->GetTimeSeconds(), 4.0);
    }
    if (GEngine) UnmadeRpgFeedback(-1, 5.f, FColor::Cyan,
        FString::Printf(TEXT("FOLD: barrier vanishes for %d seconds (+24 Strain)."),
            6 + Extension));
    UnmadePresentation::Sound(this,TEXT("cast"));
    PlayIdentityMotion(FName(TEXT("Cast")));
}

void AUnmadeCharacter::RewriteOpen()
{
    RewriteChoice(FName("variant.open"));
}

void AUnmadeCharacter::RewriteSealed()
{
    RewriteChoice(FName("variant.sealed"));
}

void AUnmadeCharacter::RewriteChoice(FName ChoiceId)
{
    if(!UnmadeSave::IsSessionWritable(this))return;
    if(GetWorld())for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        if(Hub->TryRoadAbility(this,2,ChoiceId==FName(TEXT("variant.open"))?2:3))return;
        if(Hub->TryCinderholdAbility(this,2,ChoiceId==FName(TEXT("variant.open"))?2:3))return;
    }
    if (!GetWorld() || !IsValid(FindNearbyFractureAnchor()))
    {
        PendingRewriteChoice = NAME_None;
        if (GEngine) UnmadeRpgFeedback(-1, 5.f, FColor::Yellow,
            TEXT("Approach the anomaly before attempting Rewrite."));
        return;
    }
    const double Now = GetWorld()->GetTimeSeconds();
    const FString ChoiceString = ChoiceId.ToString();
    const std::string Choice(TCHAR_TO_UTF8(*ChoiceString));
    const bool bConfirmed = PendingRewriteChoice == ChoiceId && Now <= PendingRewriteExpiresAt;
    if (!bConfirmed)
    {
        const auto Preview = FractureModel.Rewrite("region.prototype.hub", Choice, false);
        PendingRewriteChoice = NAME_None;
        if (Preview != UnmadeCore::Result::NeedsConfirmation)
        {
            if (GEngine) UnmadeRpgFeedback(-1, 5.f, FColor::Yellow,
                TEXT("This world has already been committed or the choice is unavailable."));
            return;
        }
        PendingRewriteChoice = ChoiceId;
        PendingRewriteExpiresAt = Now + 6.0;
        if (GEngine) UnmadeRpgFeedback(-1, 6.f, FColor::Orange,
            FString::Printf(TEXT("PERMANENT REWRITE %s (+60 Strain). Press the same key again within 6 seconds to confirm."), *ChoiceString));
        return;
    }
    PendingRewriteChoice = NAME_None;
    const auto Result=UnmadeCore::CommitFractureAction(FractureModel,
        [&]{return FractureModel.Rewrite("region.prototype.hub",Choice,true);},
        [&]{return CommitLocalEvent(FName("Reality.Anomaly"),FName("region.prototype.hub"),[&]{return SaveFractureState();});});
    if(Result!=UnmadeCore::FractureCommit::Applied){
        UnmadeRpgFeedback(-1,7.f,FColor::Red,Result==UnmadeCore::FractureCommit::SaveFailed?
            TEXT("REWRITE NOT SAVED: world, Strain and witnesses restored. Preview and confirm again."):
            TEXT("Rewrite refused. Strain may be too high."));
        return;
    }
    ApplyFractureVisuals();
    if (GEngine) UnmadeRpgFeedback(-1, 7.f, FColor::Yellow,
        FString::Printf(TEXT("REWRITE COMMITTED: %s. This outcome persists across sessions."), *ChoiceString));
    UnmadePresentation::Sound(this,TEXT("cast"));
    PlayIdentityMotion(FName(TEXT("Cast")));
}

AUnmadeFractureAnchor* AUnmadeCharacter::FindNearbyFractureAnchor() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;
    for (TActorIterator<AUnmadeFractureAnchor> It(World); It; ++It)
    {
        if (FVector::DistSquared(GetActorLocation(), It->GetActorLocation()) <= FMath::Square(490.f))
            return *It;
    }
    return nullptr;
}

void AUnmadeCharacter::ApplyFractureVisuals()
{
    if (!GetWorld()) return;
    const double Now = GetWorld()->GetTimeSeconds();
    const FString VariantString(UTF8_TO_TCHAR(FractureModel.WorldVariant().c_str()));
    for (TActorIterator<AUnmadeFractureAnchor> It(GetWorld()); It; ++It)
    {
        It->ApplyFractureState(FractureModel.IsGlimpsing(Now),
            FractureModel.IsFolded(Now), FName(*VariantString));
    }
}

bool AUnmadeCharacter::SaveFractureState()
{
    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return false;
    const auto Snapshot = FractureModel.TakeSnapshot();
    Save->WorldVariantId = Snapshot.variant.empty()
        ? NAME_None : FName(UTF8_TO_TCHAR(Snapshot.variant.c_str()));
    Save->PlayerStrain = Snapshot.strain;
    Save->bHasFractureSnapshot = true;
    return UnmadeSave::SaveGameToSlot(Save,this);
}

bool AUnmadeCharacter::CommitIntegrationRecovery(double Amount, TFunctionRef<bool()> Write)
{
    return UnmadeCore::CommitStrainRecovery(FractureModel, Amount, Write);
}


FString AUnmadeCharacter::GetCharacterChosenName() const
{
    return FString(UTF8_TO_TCHAR(Identity.Snapshot().chosenName.c_str()));
}

void AUnmadeCharacter::ApplyIdentitySilhouette()
{
    // Primitive-art placeholder: data retains far more detail than this mesh.
    const auto& Options=Identity.Snapshot().options;
    static const float Widths[4]={.45f,.54f,.62f,.71f};
    static const float Heights[4]={1.48f,1.58f,1.65f,1.74f};
    if(IsValid(PlaceholderBody))
        PlaceholderBody->SetRelativeScale3D(FVector(
            Widths[Options[0]],Widths[Options[0]],Heights[Options[0]]));
    if(IsValid(PlaceholderCrest))
    {
        PlaceholderCrest->SetVisibility(Options[2]!=0);
        PlaceholderCrest->SetRelativeScale3D(FVector(
            .20f+Options[2]*.035f,.26f+Options[2]*.025f,
            .09f+Options[2]*.015f));
    }
}

bool AUnmadeCharacter::SaveCharacterIdentity()
{
    if(bCharacterIdentitySaveRejected)return false;
    UUnmadePrototypeSave* Save=UUnmadePrototypeSave::LoadOrCreate();
    if(!Save)return false;
    const auto& State=Identity.Snapshot();
    Save->bHasCharacterIdentity=true;
    Save->CharacterIdentityChoices.Reset();
    for(const int Choice:State.options)Save->CharacterIdentityChoices.Add(Choice);
    Save->CharacterChosenName=GetCharacterChosenName();
    return UnmadeSave::SaveGameToSlot(Save,this);
}

bool AUnmadeCharacter::SetCharacterFeature(int32 Aspect,int32 Choice)
{
    if(bCharacterIdentitySaveRejected)return false;
    const auto Before=Identity.Snapshot();
    if(!Identity.Select(static_cast<UnmadeCore::IdentityAspect>(Aspect),Choice))
        return false;
    if(!SaveCharacterIdentity())
    {
        Identity.Restore(Before);
        return false;
    }
    ApplyIdentitySilhouette();
    return true;
}

bool AUnmadeCharacter::SetCharacterChosenName(const FString& Name)
{
    if(bCharacterIdentitySaveRejected)return false;
    const auto Before=Identity.Snapshot();
    const std::string Utf8(TCHAR_TO_UTF8(*Name));
    if(!Identity.Rename(Utf8))return false;
    if(!SaveCharacterIdentity())
    {
        Identity.Restore(Before);
        return false;
    }
    return true;
}

void AUnmadeCharacter::ShowCharacterProfile()
{
    const auto& V=Identity.Snapshot().options;
    if(GEngine)UnmadeRpgFeedback(-1,12.f,FColor::Cyan,
        FString::Printf(TEXT("IDENTITY | %s | Frame %d, Face %d, Hair %d, Voice %d, Palette %d, Mark %d, Gait %d, Calling %d | Origin: THE IMPOSSIBLE"),
            *GetCharacterChosenName(),V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7]));
    OpenRpgPage(7);
}

void AUnmadeCharacter::BeginPlay()
{
    Super::BeginPlay();
    UnmadeSave::RegisterPlayer(this);
    GetMutableDefault<UUnmadePlayerPreferences>()->ApplyMappings();
    Combat->EnablePlayerTempo();
    if(auto* PC=Cast<APlayerController>(GetController())){RpgWidget=CreateWidget<UUnmadeRpgWidget>(PC,UUnmadeRpgWidget::StaticClass());if(RpgWidget)RpgWidget->AddToViewport(30);}

    if(USkeletalMesh* Rig=LoadObject<USkeletalMesh>(nullptr,
        TEXT("/Game/UnmadeProduction/Characters/Resident/SK_UnmadeResident.SK_UnmadeResident")))
    {
        GetMesh()->SetSkeletalMesh(Rig);
        GetMesh()->SetRelativeLocation(FVector(0,0,-88));
        GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        PlaceholderBody->SetVisibility(false);
        PlaceholderCrest->SetVisibility(false);
        bSkeletalRigLoaded=true;
        AnimatePlayer(FName(TEXT("Idle")));
    }
    const UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(
        UnmadeSave::LoadGameFromSlot());
    if(Save && Save->SchemaVersion==1 && Save->bHasAbilityStudy)
    {
        UnmadeCore::StudySnapshot Snapshot;
        bool Valid=Save->AbilityStudyStages.Num()==UnmadeCore::StudyCount &&
            Save->AbilityStudyEvidence.Num()==UnmadeCore::StudyCount &&
            Save->AbilityStudyTrials.Num()==UnmadeCore::StudyCount &&
            Save->AbilityStudySelection>=0 && Save->AbilityStudySelection<UnmadeCore::StudyCount;
        if(Valid){for(int i=0;i<UnmadeCore::StudyCount;++i){
            Snapshot.stages[i]=Save->AbilityStudyStages[i];
            Snapshot.evidence[i]=Save->AbilityStudyEvidence[i];
            Snapshot.trials[i]=Save->AbilityStudyTrials[i];
        }Valid=AbilityStudy.Restore(Snapshot);}
        if(Valid)SelectedStudy=Save->AbilityStudySelection;
        else {bStudySaveRejected=true;UE_LOG(LogTemp,Error,TEXT("Malformed Ability Study save. Existing profile protected from overwrite."));}
    }
    if (Save && Save->SchemaVersion == 1 && Save->bHasFractureSnapshot)
    {
        const FString Variant = Save->WorldVariantId.IsNone()
            ? TEXT("") : Save->WorldVariantId.ToString();
        if (!FractureModel.Restore({std::string(TCHAR_TO_UTF8(*Variant)), Save->PlayerStrain}))
        {
            bFractureSaveRejected=true;UE_LOG(LogTemp, Warning, TEXT("Invalid fracture save protected; writes disabled"));
        }
    }
    if(Save && Save->SchemaVersion==1 && Save->bHasCharacterIdentity)
    {
        if(Save->CharacterIdentityChoices.Num()!=8)
        {
            bCharacterIdentitySaveRejected=true;
            UE_LOG(LogTemp,Error,TEXT("Invalid identity field count: preserving saved profile"));
        }
        else
        {
            UnmadeCore::CharacterIdentitySnapshot Restored;
            Restored.chosenName=std::string(TCHAR_TO_UTF8(*Save->CharacterChosenName));
            for(int32 i=0;i<8;++i)Restored.options[i]=Save->CharacterIdentityChoices[i];
            if(!Identity.Restore(Restored))
            {
                bCharacterIdentitySaveRejected=true;
                UE_LOG(LogTemp,Error,TEXT("Invalid identity snapshot: edits disabled"));
            }
        }
    }
    ApplyIdentitySilhouette();
    // Older prototype saves have no conflict fields and safely remain unresolved.
    if (Save && Save->SchemaVersion == 1 && Save->bHasConflictSnapshot &&
        !LocalConflict.Restore({Save->LocalConflictChoice, Save->SupplyActivityStage}))
    {
        bConflictSaveRejected=true;UE_LOG(LogTemp, Warning, TEXT("Invalid local conflict snapshot protected; writes disabled"));
    }
    ApplyFractureVisuals();
    ApplyConflictGates();
}


FVector AUnmadeCharacter::ResolveSafeCheckpoint(const FVector& Current)const
{
    const auto Entry=UnmadeCore::SafeRealmEntry(Current.X,Current.Y);
    FVector Safe(Entry.X,Entry.Y,Entry.Z),Candidate=Safe;
    if(GetWorld())for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub){
        if(Hub->ExperienceCheckpoint(Current,Candidate)&&UnmadeCore::SameCheckpointRealm(Current.X,Current.Y,Candidate.X,Candidate.Y))Safe=Candidate;
        break;
    }
    return Safe;
}

bool AUnmadeCharacter::CanCommitCheckpoint()
{
    if(!UnmadeSave::IsSessionWritable(this)||!InitializeStartupRewards())return false;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
        if(Hub->HasPendingDefeatReceipts()){
            UnmadeRpgFeedback(-1,8.f,FColor::Orange,TEXT("CHECKPOINT WAITING: victory or history receipts still need to save. Continue the game for automatic retries."));return false;
        }
    return true;
}
bool AUnmadeCharacter::CommitCheckpointMove(const FVector& Destination,int32 FrontierRoute,TFunctionRef<bool()> Write,bool Recovery)
{
    if(!UnmadeSave::IsSessionWritable(this))return false;
    if(!Recovery&&!CanCommitCheckpoint())return false;
    const FVector Origin=GetActorLocation();
    const uint8 Mode=GetCharacterMovement()->MovementMode;
    const auto Result=UnmadeCore::CommitCheckpointRelocation(Origin,Destination,
        [&](const FVector& At){
            for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub){Hub->PrepareRealmGeometry(At,100000);break;}
            return SetActorLocation(At,false,nullptr,ETeleportType::TeleportPhysics);
        },[&]{
            bCaptureCheckpointPosition=true;
            const bool Saved=IsValid(Tenfold)?Tenfold->CommitExploredFrontier(FrontierRoute,Write):Write();
            bCaptureCheckpointPosition=false;return Saved;
        });
    if(Result==UnmadeCore::RelocationCommit::Applied)return true;
    if(Result==UnmadeCore::RelocationCommit::RollbackFailed){
        bCheckpointRollbackPending=true;CheckpointRollbackPosition=Origin;CheckpointRollbackMode=Mode;
        NextCheckpointRollback=GetWorld()->GetTimeSeconds()+5;GetCharacterMovement()->DisableMovement();
        UnmadeRpgFeedback(-1,10.f,FColor::Red,TEXT("RETURN MOVEMENT FAILED: saving is protected while automatic movement recovery retries. Reload remains available."));
    }else UnmadeRpgFeedback(-1,8.f,FColor::Red,TEXT("CHECKPOINT NOT SAVED: movement and progress were restored. Try again when saving is available."));
    return false;
}
void AUnmadeCharacter::RetryCheckpointRollback()
{
    if(!bCheckpointRollbackPending||!GetWorld()||GetWorld()->GetTimeSeconds()<NextCheckpointRollback)return;
    NextCheckpointRollback=GetWorld()->GetTimeSeconds()+5;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub){Hub->PrepareRealmGeometry(CheckpointRollbackPosition,100000);break;}
    if(!SetActorLocation(CheckpointRollbackPosition,false,nullptr,ETeleportType::TeleportPhysics))return;
    bCheckpointRollbackPending=false;
    GetCharacterMovement()->SetMovementMode(static_cast<EMovementMode>(CheckpointRollbackMode));
    UnmadeRpgFeedback(-1,7.f,FColor::Green,TEXT("Movement restored. Saving is available once this profile is ready."));
}
void AUnmadeCharacter::RecoverAtSafeCheckpoint()
{
    if(!GetWorld()||!Combat->IsDefeated()||GetWorld()->GetTimeSeconds()<DefeatRecordedAt+4.0)return;
    const FVector Safe=ResolveSafeCheckpoint(GetActorLocation());
    const auto Result=DeathRecoveryRetry.Attempt(0,GetWorld()->GetTimeSeconds(),[&]{
        return UnmadeCore::CommitCombatRecovery(*Combat,[]{return true;},[&]{
            return CommitCheckpointMove(Safe,-1,[&]{
                if(!Combat->ReviveAtCheckpoint())return false;
                for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)return Hub->SaveRecoveryState(this);
                return false;
            },true);
        });
    });
    if(Result==UnmadeCore::DefeatSaveResult::FirstFailure)
        UnmadeRpgFeedback(-1,9.f,FColor::Orange,TEXT("RECOVERY NOT SAVED: revival was restored to its prior state. Retrying every five active seconds."));
    if(Result!=UnmadeCore::DefeatSaveResult::Saved)return;
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);bPlayerDefeatHandled=false;DefeatRecordedAt=-1;
    UnmadeRpgFeedback(-1,9.f,FColor::Green,TEXT("RETURNED TO SAFETY: wounds and position saved; choices and witnesses remain. Any pending rewards still need to settle before a complete checkpoint."));
}

void AUnmadeCharacter::PlayIdentityMotion(FName Cue){PlayerMotion=NAME_None;AnimatePlayer(Cue);if(GetWorld())PlayerAttackMotionUntil=GetWorld()->GetTimeSeconds()+1;}

void AUnmadeCharacter::AnimatePlayer(FName Cue)
{
    if(!bSkeletalRigLoaded || Cue==PlayerMotion || !IsValid(GetMesh()))return;
    const FString Name=TEXT("AN_UnmadeResident_")+Cue.ToString();
    const FString Path=TEXT("/Game/UnmadeProduction/Characters/Resident/")+Name+TEXT(".")+Name;
    if(UAnimSequence* Clip=LoadObject<UAnimSequence>(nullptr,*Path))
    {
        GetMesh()->SetPlayRate(1);
        GetMesh()->PlayAnimation(Clip,Cue==FName(TEXT("Idle"))||Cue==FName(TEXT("Walk"))||Cue==FName(TEXT("Guard")));
        PlayerMotion=Cue;
    }
}
void AUnmadeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    RetryCheckpointRollback();
    RestoreSessionWhenReady();
    RetryCombatRewards();
    if(!bStartupRewardsReady&&GetWorld()&&GetWorld()->GetTimeSeconds()>=NextStartupRewardsAttempt){NextStartupRewardsAttempt=GetWorld()->GetTimeSeconds()+5;InitializeStartupRewards();}
    if(bSessionRestored&&GetWorld()&&GetWorld()->GetTimeSeconds()>=NextSessionSave){NextSessionSave=GetWorld()->GetTimeSeconds()+20;SaveSessionNow();}
    TickWeaponContact();
    if(StudyWardUntil>=0 && GetWorld() && GetWorld()->GetTimeSeconds()>=StudyWardUntil)
    {StudyWardUntil=-1;if(IsValid(Equipment))Equipment->SetStudyBonuses(0,0);}
    if(!RpgWidget)if(auto* PC=Cast<APlayerController>(GetController())){RpgWidget=CreateWidget<UUnmadeRpgWidget>(PC,UUnmadeRpgWidget::StaticClass());if(RpgWidget)RpgWidget->AddToViewport(30);}
    if(GetWorld() && GetWorld()->GetTimeSeconds()>=PlayerAttackMotionUntil)
        AnimatePlayer(Combat&&Combat->IsDefeated()?FName(TEXT("Death")):Combat&&Combat->IsGuarding()?FName(TEXT("Guard")):GetVelocity().SizeSquared2D()>FMath::Square(12.f)
            ?FName(TEXT("Walk")):FName(TEXT("Idle")));
    if(GetWorld()&&Combat){if(!Combat->IsDefeated())for(TActorIterator<AUnmadePrototypeHub> H(GetWorld());H;++H){Combat->SetWeaponStyle(H->PlayerWeaponStyle(Equipment?Equipment->EquippedWeapon():UnmadeCore::ItemId::Count),H->GetActiveMastery());{auto b=H->ActiveBuild();PlayerBuildSpeed=b.speed;if(Equipment)Equipment->SetBuildBonuses(b.attack,b.armor);}break;}if(!Combat->IsDefeated())GetCharacterMovement()->MaxWalkSpeed=(bIsCrouched?260:600)*PlayerBuildSpeed*Combat->MoveMultiplier(GetWorld()->GetTimeSeconds());float health=Combat->GetHealth();if(LastPresentationHealth>=0&&health<LastPresentationHealth){AnimatePlayer(FName(Combat->IsDefeated()?TEXT("Death"):TEXT("Stagger")));PlayerAttackMotionUntil=GetWorld()->GetTimeSeconds()+(Combat->IsDefeated()?1.5:.6);UnmadePresentation::Sound(this,Combat->IsDefeated()?TEXT("death"):TEXT("injury"));}LastPresentationHealth=health;
      if(!Combat->IsDefeated()&&GetCharacterMovement()->IsMovingOnGround()&&GetVelocity().SizeSquared2D()>FMath::Square(20.f)&&GetWorld()->GetTimeSeconds()>=NextFootstepAt){NextFootstepAt=GetWorld()->GetTimeSeconds()+.45;UnmadePresentation::Sound(this,TEXT("footstep_stone"));}}
    if(UnmadeSave::IsSessionWritable(this))FractureModel.Recover(static_cast<double>(DeltaSeconds) *
        (IsValid(Equipment) ? Equipment->StrainRecoveryMultiplier() : 1.0));
    // The world's clock drives a readable three-stage pulse. A guard reduces
    // damage through the combat rules; moving out of the marked lane avoids it.
    if(GetWorld() && !Combat->IsDefeated()&&UnmadeSave::IsSessionWritable(this))
    {
        for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
        {
            const auto Hazard=Hub->GetNearbyFrontierHazard(GetActorLocation());
            if(Hazard.pulseId==0)break;
            const int32 Index=GetActorLocation().Y<0?0:1;
            if(Hazard.phase==UnmadeCore::FrontierHazardPhase::Warning &&
               Hazard.pulseId!=LastHazardWarningPulse[Index])
            {
                LastHazardWarningPulse[Index]=Hazard.pulseId;
                if(GEngine)UnmadeRpgFeedback(-1,3.f,FColor::Yellow,
                    FString::Printf(TEXT("WARNING: storm/heat surge at world-second %.1f. Guard or leave the marked lane."),
                        Hub->GetWorldClockSeconds()));
            }
            if(Hazard.phase==UnmadeCore::FrontierHazardPhase::Impact &&
               Hazard.pulseId!=LastHazardImpactPulse[Index])
            {
                LastHazardImpactPulse[Index]=Hazard.pulseId;
                if(Combat->ReceiveHazardPulse(
                    static_cast<uint64>(0xF0100+Index),Hazard.pulseId,Hazard.damage *
                        (static_cast<double>(Hub->GetRealmHazardPercent(GetActorLocation()))/100.0)) && GEngine)
                    UnmadeRpgFeedback(-1,4.f,FColor::Orange,
                        TEXT("FRONTIER SURGE: you were exposed. Guard next pulse or operate the control."));
            }
            break;
        }
    }

    // Six land-specific warnings share the source world clock, not a random
    // per-frame roll. One pulse can affect a player at most once.
    if(GetWorld() && !Combat->IsDefeated()&&UnmadeSave::IsSessionWritable(this))
    {
        for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
        {
            const auto Hazard=Hub->GetNearbyLaterRealmHazard(GetActorLocation());
            if(Hazard.index<0 || Hazard.pulseId==0)break;
            const int32 Index=Hazard.index;
            if(Hazard.phase==UnmadeCore::FrontierHazardPhase::Warning &&
               LastLaterHazardWarningPulse[Index]!=Hazard.pulseId)
            {
                LastLaterHazardWarningPulse[Index]=Hazard.pulseId;
                if(GEngine)UnmadeRpgFeedback(-1,4.f,FColor::Yellow,
                    FString::Printf(TEXT("%s: %s"),
                        UTF8_TO_TCHAR(UnmadeCore::LaterHazards[Index].label),
                        UTF8_TO_TCHAR(Hazard.warning)));
            }
            if(Hazard.phase==UnmadeCore::FrontierHazardPhase::Impact &&
               LastLaterHazardPulse[Index]!=Hazard.pulseId)
            {
                LastLaterHazardPulse[Index]=Hazard.pulseId;
                const double AdjustedSeverity=Hazard.severity *
                    (static_cast<double>(Hub->GetRealmHazardPercent(GetActorLocation()))/100.0);
                bool bApplied=false;
                if(Hazard.effect==UnmadeCore::LaterHazardEffect::Injury)
                    bApplied=Combat->ReceiveHazardPulse(
                        Hazard.sourceId,Hazard.pulseId,AdjustedSeverity);
                else if(Hazard.effect==UnmadeCore::LaterHazardEffect::Strain)
                {
                    const auto Before=FractureModel;
                    bApplied=FractureModel.ApplyEnvironmentalStrain(AdjustedSeverity);
                    if(bApplied && !SaveFractureState())
                    {
                        FractureModel=Before;
                        bApplied=false;
                        if(GEngine)UnmadeRpgFeedback(-1,5.f,FColor::Red,
                           TEXT("Unmade could not persist reality strain; the exposure was undone."));
                    }
                }
                if(bApplied && GEngine)
                    UnmadeRpgFeedback(-1,5.f,FColor::Orange,
                        FString::Printf(TEXT("%s: %s"),
                            UTF8_TO_TCHAR(UnmadeCore::LaterHazards[Index].label),
                            UTF8_TO_TCHAR(Hazard.counterplay)));
            }
            break;
        }
    }
    if (Combat->IsDefeated() && !bPlayerDefeatHandled)
    {
        bPlayerDefeatHandled=true;DeathRecoveryRetry.Reset();
        DefeatRecordedAt=GetWorld()?GetWorld()->GetTimeSeconds():0.0;
        GetCharacterMovement()->DisableMovement();
        if (GEngine) UnmadeRpgFeedback(-1,5.f,FColor::Red,
            TEXT("YOU FELL: returning to this realm's safe entry in four seconds."));
    }
    if(bPlayerDefeatHandled)RecoverAtSafeCheckpoint();
    ApplyFractureVisuals();
}

int32 AUnmadeCharacter::StageLocalWitnesses(FName EventKind,FName SubjectId)
{
    UWorld* World = GetWorld();
    if (!World||!UnmadeSave::IsWorldWritable(World)) return 0;
    const FGuid EventId = FGuid::NewGuid();
    int32 Witnesses = 0;
    for (TActorIterator<AUnmadeNpcCharacter> It(World); It; ++It)
    {
        if(It->GetStableId().IsNone())continue;
        if (FVector::DistSquared(GetActorLocation(), It->GetActorLocation()) > FMath::Square(720.f))
            continue;

        // Simple sight approximation: solid level geometry blocks direct observation.
        FCollisionQueryParams VisibilityParams(SCENE_QUERY_STAT(UnmadeNPCWitness), false);
        VisibilityParams.AddIgnoredActor(this);
        VisibilityParams.AddIgnoredActor(*It);
        const FVector Observer = It->GetActorLocation() + FVector(0, 0, 70);
        const FVector Performer = GetActorLocation() + FVector(0, 0, 70);
        if (World->LineTraceTestByChannel(Observer, Performer, ECC_Visibility, VisibilityParams))
            continue;
        if (It->GetMemory()->Witness(EventId, EventKind, SubjectId)) ++Witnesses;
    }
    return Witnesses;
}

void AUnmadeCharacter::ReportLocalEvent(FName EventKind,FName SubjectId)
{
    if(!UnmadeSave::IsWorldWritable(GetWorld()))return;
    const int32 Witnesses=StageLocalWitnesses(EventKind,SubjectId);
    if(IsValid(Tenfold))
    {
        if(EventKind==FName("Player.Helped") || EventKind==FName("Player.HelpedVillage"))
            Tenfold->RecordLegacyDeed(UnmadeCore::Deed::Protected);
        else if(EventKind==FName("World.FactionResolved"))
            Tenfold->RecordLegacyDeed(UnmadeCore::Deed::Reconciled);
        else if(EventKind==FName("Player.Threatened"))
            Tenfold->RecordThreatenedOath();
    }
    if(Witnesses>0)SaveNearbyNpcMemories();
    const FString Line = FString::Printf(TEXT("%s witnessed by %d nearby resident(s)."),
        *EventKind.ToString(), Witnesses);
    if (GEngine) UnmadeRpgFeedback(-1, 6.f, FColor::Yellow, Line);
}

bool AUnmadeCharacter::CommitLocalEvent(FName Event,FName Subject,TFunctionRef<bool()> Write)
{
    TArray<TPair<FName,FName>> Events;Events.Emplace(Event,Subject);
    return CommitLocalEvents(Events,Write);
}

bool AUnmadeCharacter::CommitLocalEvents(const TArray<TPair<FName,FName>>& Events,TFunctionRef<bool()> Write)
{
    if(!UnmadeSave::IsSessionWritable(this))return false;
    for(const auto& Event:Events)if(Event.Key.IsNone())return false;
    struct FObserverBefore {TWeakObjectPtr<UUnmadeMemoryComponent> Memory;FUnmadeNpcSnapshot State;};
    int32 Witnesses=0;unsigned Deeds=0;
    for(const auto& Event:Events){const FTCHARToUTF8 Kind(*Event.Key.ToString());Deeds|=UnmadeCore::ActionDeedMask(Kind.Get());}
    const bool Saved=UnmadeCore::CommitActionConsequences([&]{
        TArray<FObserverBefore> Before;
        for(TActorIterator<AUnmadeNpcCharacter> Npc(GetWorld());Npc;++Npc)
            if(!Npc->GetStableId().IsNone()&&IsValid(Npc->GetMemory()))
                Before.Add(FObserverBefore{Npc->GetMemory(),Npc->GetMemory()->WriteSnapshot(Npc->GetStableId())});
        return Before;
    },[&]{for(const auto& Event:Events)Witnesses+=StageLocalWitnesses(Event.Key,Event.Value);},[&]{
        return IsValid(Tenfold)?Tenfold->CommitActionDeeds(Deeds,Write):Write();
    },[&](const auto& Before){
        for(const auto& Observer:Before)if(Observer.Memory.IsValid())
            Observer.Memory->ReadSnapshot(Observer.State,Observer.State.NpcId);
    });
    if(!Saved){
        UnmadeRpgFeedback(-1,8.f,FColor::Red,TEXT("ACTION NOT SAVED: its witness memories and deed credit were rolled back with the action. Retry when saving is available."));
        return false;
    }
    if(Witnesses>0)UnmadeRpgFeedback(-1,6.f,FColor::Yellow,TEXT("The action and its local witness memories were saved together."));
    return true;
}

bool AUnmadeCharacter::CommitRiteEvents(UnmadeCore::RiteId Id,bool CivicApplied,TFunctionRef<bool()> Write)
{
    TArray<TPair<FName,FName>> Events;
    if(CivicApplied){
        const int Index=static_cast<int>(Id);
        const auto* Policy=UnmadeCore::RealmReactionAt(GetActorLocation().X,GetActorLocation().Y);
        if(Policy&&Index>=0&&Index<static_cast<int>(UnmadeCore::RealityProfiles.size()))
            Events.Emplace(FName(UnmadeCore::RealityProfiles[Index].care?TEXT("Reality.CareWitnessed"):TEXT("Reality.InquiryWitnessed")),
                FName(*FString::Printf(TEXT("Reality.Realm.%d"),static_cast<int32>(Policy->realm))));
    }
    Events.Emplace(FName("Reality.Rite"),FName("TheUnmade.Tenfold"));
    return CommitLocalEvents(Events,Write);
}

void AUnmadeCharacter::SaveNearbyNpcMemories()
{
    if(!GetWorld()||!UnmadeSave::IsWorldWritable(GetWorld()))return;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub){
        if(!Hub->SaveNpcMemories())UnmadeRpgFeedback(-1,8.f,FColor::Red,
            TEXT("NPC MEMORIES NOT SAVED: retained for automatic retry while the game continues."));
        return;
    }
}

void AUnmadeCharacter::RetryCombatRewards()
{
    if(!GetWorld()||!IsValid(Equipment)||!CombatRewards.Pending())return;
    const auto Result=CombatRewards.Attempt(UnmadeSave::IsSessionWritable(this),GetWorld()->GetTimeSeconds(),
        [&](auto reward){return Equipment->HasClaimed(reward);},
        [&](auto reward){return Equipment->Claim(reward);});
    if(Result==UnmadeCore::DefeatSaveResult::FirstFailure)
        UnmadeRpgFeedback(-1,10.f,FColor::Orange,TEXT("COMBAT REWARD NOT SAVED: retained for five-second retries. Make inventory room if needed; a complete checkpoint waits for this reward."));
}

bool AUnmadeCharacter::IsRpgMenuOpen()const{return RpgWidget&&RpgWidget->IsOpen();}
void AUnmadeCharacter::OpenRpgPage(int Page){if(RpgWidget)RpgWidget->Open(Page);}
void AUnmadeCharacter::OpenWorldMap(){OpenRpgPage(2);}
void AUnmadeCharacter::PersonalChoiceA(){if(GetWorld())for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)if(Hub->TryDepthInteraction(this,1))return;}
void AUnmadeCharacter::PersonalChoiceB(){if(GetWorld())for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)if(Hub->TryDepthInteraction(this,2))return;}
void AUnmadeCharacter::UseFieldTechnique(){if(!GetWorld()||IsRpgMenuOpen())return;for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub){if(!Hub->UseDepthPower(this))Hub->NotifyRpg(TEXT("Technique needs a ready world, earned mastery, stamina, recovery time and a suitable nearby target. Wait for active stagger to end."));return;}}
void AUnmadeCharacter::DodgeStep(){if(!GetWorld()||IsRpgMenuOpen()||!Combat->Dodge(GetWorld()->GetTimeSeconds()))return;FVector Direction=GetLastMovementInputVector().GetSafeNormal2D();if(Direction.IsNearlyZero())Direction=GetActorForwardVector();LaunchCharacter(Direction*650,false,false);PlayIdentityMotion(FName(TEXT("Dodge")));UnmadePresentation::Sound(this,TEXT("guard"));}
void AUnmadeCharacter::RpgMenuAction(FName Action){if(!Combat||Combat->IsDefeated())return;if(Action==FName(TEXT("StudyObserve"))){StudyObserve();return;}if(Action==FName(TEXT("StudyProgress"))){StudyProgress();return;}if(Action==FName(TEXT("StudyUse"))){StudyUse();return;}if(Action==FName(TEXT("StudyNext"))){StudyNext();return;}if(Action==FName(TEXT("Weapon")))EquipNextWeapon();else if(Action==FName(TEXT("Armor")))EquipNextArmor();else if(Action==FName(TEXT("Charm")))EquipNextCharm();else if(Action==FName(TEXT("Heal")))UseHealthPotion();else if(Action==FName(TEXT("ChoiceA")))PersonalChoiceA();else if(Action==FName(TEXT("ChoiceB")))PersonalChoiceB();else if(Action==FName(TEXT("AnomalyPulse"))){DemonstrateAnomaly();}else if(Action==FName(TEXT("FoldReality"))){FoldReality();}else if(Action==FName(TEXT("RewriteOpen"))){RewriteOpen();}else if(Action==FName(TEXT("RewriteSealed"))){RewriteSealed();}else if(Action==FName(TEXT("FrontierTravel"))){CrossFrontierGateway();}else if(Action==FName(TEXT("RiteNext"))){if(Tenfold)Tenfold->NextRite();}else if(Action==FName(TEXT("RitePrevious"))){if(Tenfold)Tenfold->PreviousRite();}else if(Action==FName(TEXT("RiteStudy"))){if(Tenfold)Tenfold->StudyRite();}else if(Action==FName(TEXT("RiteUse"))){if(Tenfold)Tenfold->InvokeRite();}else if(Action==FName(TEXT("RiteChoice1"))){if(Tenfold)Tenfold->DecideSolidarity();}else if(Action==FName(TEXT("RiteChoice2"))){if(Tenfold)Tenfold->DecideTruth();}else if(Action==FName(TEXT("RiteBreak"))){if(Tenfold)Tenfold->BreakChosenOath();}else if(Action==FName(TEXT("RiteLaw"))){if(Tenfold)Tenfold->CycleWorldLaw();}else if(Action==FName(TEXT("ConfluenceCycle"))){if(Tenfold)Tenfold->CycleConfluence();}else if(Action==FName(TEXT("ConfluenceStudy"))){if(Tenfold)Tenfold->StudyConfluence();}else if(Action==FName(TEXT("ConfluenceOption1"))){if(Tenfold)Tenfold->ConfluenceChoice1();}else if(Action==FName(TEXT("ConfluenceOption2"))){if(Tenfold)Tenfold->ConfluenceChoice2();}else if(Action==FName(TEXT("FactionSolidarity"))){CommitSolidarity();}else if(Action==FName(TEXT("FactionTruth"))){CommitTruth();}else if(Action==FName(TEXT("StoryShelter"))){ChooseShelter();}else if(Action==FName(TEXT("StoryResearch"))){ChooseResearch();}else if(Action==FName(TEXT("OfferAid"))){OfferAid();}else if(Action==FName(TEXT("SupplyActivity"))){ProgressSupplyActivity();}else if(Action==FName(TEXT("FieldKit"))){UseFieldKit();}else if(Action==FName(TEXT("FieldTechnique"))){UseFieldTechnique();}else if(Action==FName(TEXT("ItemStrain"))){UseStrainPotion();}else if(Action==FName(TEXT("ItemForge"))){ForgeMythicGear();}else if(Action==FName(TEXT("Sneak"))){if(bIsCrouched)StopSneak();else StartSneak();}}

void AUnmadeCharacter::UseFieldKit(){if(!GetWorld()||IsRpgMenuOpen())return;for(TActorIterator<AUnmadePrototypeHub> h(GetWorld());h;++h){if(h->UseDepthFieldKit(this)==UnmadeCore::FieldKitResult::Unavailable)h->NotifyRpg(TEXT("Use an earned field kit from any realm inside an unresolved dungeon danger chamber. No kit is consumed without a benefit."));return;}}

void AUnmadeCharacter::RestoreSessionWhenReady(){
 if(bSessionRestored||!GetWorld())return;
 AUnmadePrototypeHub* Hub=nullptr;for(TActorIterator<AUnmadePrototypeHub> H(GetWorld());H;++H){Hub=*H;break;}
 if(!Hub||!Hub->IsWorldReady())return;
 auto* Save=Cast<UUnmadePrototypeSave>(UnmadeSave::LoadGameFromSlot());
 if(Save&&Save->bHasPlayerResume){
  Hub->PrepareRealmGeometry(Save->ResumePosition,100000);
  if(!SetActorLocation(Save->ResumePosition,false,nullptr,ETeleportType::TeleportPhysics))return;
  if(!Combat->RestoreSession(Save->ResumeHealth,Save->ResumeStamina))return;
 }
 bSessionRestored=true;NextSessionSave=GetWorld()->GetTimeSeconds()+20;
}
void AUnmadeCharacter::CaptureSession(UUnmadePrototypeSave* Save)const{
 if(!Save||!bSessionRestored||!GetWorld()||!Combat)return;
 Save->bHasPlayerResume=true;
 Save->ResumePosition=bCaptureCheckpointPosition?GetActorLocation():ResolveSafeCheckpoint(GetActorLocation());
 Save->ResumeHealth=Combat->GetHealth();Save->ResumeStamina=Combat->GetStamina();
}
bool AUnmadeCharacter::InitializeStartupRewards(){
 if(bStartupRewardsReady)return true;
 if(!IsValid(Equipment)||!UnmadeSave::IsSessionWritable(this))return false;
 bStartupRewardsReady=Equipment->ReconcileEarnedMilestones();
 if(!bStartupRewardsReady&&!bStartupNoticeShown){bStartupNoticeShown=true;UnmadeRpgFeedback(-1,10.f,FColor::Orange,TEXT("EARNED REWARDS PENDING: save failed or inventory full. Retrying; make room in Items if needed."));}
 return bStartupRewardsReady;
}
bool AUnmadeCharacter::SaveSessionNow(){RestoreSessionWhenReady();if(!bSessionRestored||!GetWorld()||!InitializeStartupRewards())return false;for(TActorIterator<AUnmadePrototypeHub> h(GetWorld());h;++h)return h->SavePlayerSession(this);return false;}

void AUnmadeCharacter::TradeSelection(UnmadeCore::ItemId item,bool buy){SelectedTrade=item;if(buy)BuyMarketSupplies();else SellMarketSupplies();SelectedTrade=UnmadeCore::ItemId::Count;}

int AUnmadeCharacter::GetCharacterFeature(int aspect)const{return aspect>=0&&aspect<8?Identity.Snapshot().options[aspect]:-1;}


bool AUnmadeCharacter::PersistStudy()
{
    if(bStudySaveRejected)return false;
    UUnmadePrototypeSave* Save=UUnmadePrototypeSave::LoadOrCreate();
    if(!Save)return false;
    const auto& S=AbilityStudy.Snapshot();
    Save->AbilityStudyStages.Reset();
    Save->AbilityStudyEvidence.Reset();
    Save->AbilityStudyTrials.Reset();
    for(int i=0;i<UnmadeCore::StudyCount;++i)
    {
        Save->AbilityStudyStages.Add(S.stages[i]);
        Save->AbilityStudyEvidence.Add(S.evidence[i]);
        Save->AbilityStudyTrials.Add(S.trials[i]);
    }
    Save->AbilityStudySelection=SelectedStudy;
    Save->bHasAbilityStudy=true;
    Save->PlayerStrain=FractureModel.CurrentStrain();
    Save->bHasFractureSnapshot=true;
    return UnmadeSave::SaveGameToSlot(Save,this);
}

bool AUnmadeCharacter::RecordStudyEvidence(UnmadeCore::StudyId Id,int Bit)
{
    if(bStudySaveRejected)return false;
    const auto Before=AbilityStudy.Snapshot();
    if(AbilityStudy.Observe(Id,Bit)!=UnmadeCore::StudyResult::EvidenceRecorded)return false;
    if(!PersistStudy()){AbilityStudy.Restore(Before);return false;}
    if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Cyan,
        FString::Printf(TEXT("EVIDENCE: %s / firsthand signal %d committed to the Study Codex."),
            UTF8_TO_TCHAR(UnmadeCore::StudyNames[static_cast<int>(Id)]),Bit));
    return true;
}

AUnmadeEnemyCharacter* AUnmadeCharacter::FindStudyEnemy(int32 Subject,bool RequireWarning)const
{
    if(!GetWorld())return nullptr;
    AUnmadeEnemyCharacter* Nearest=nullptr;
    double Best=FMath::Square(700.0);
    const double Now=GetWorld()->GetTimeSeconds();
    for(TActorIterator<AUnmadeEnemyCharacter> It(GetWorld());It;++It)
    {
        AUnmadeEnemyCharacter* Enemy=*It;
        if(!Enemy||!Enemy->CanBeTargeted())continue;
        UnmadeCore::CreatureKind Species;
        if(Subject>=0 && Subject<3)
        {
            if(!Enemy->StudyCreature(Species))continue;
            const auto Want=Subject==0?UnmadeCore::CreatureKind::Cinderjaw:
                Subject==1?UnmadeCore::CreatureKind::PaleStrider:UnmadeCore::CreatureKind::RedactionEye;
            if(Species!=Want)continue;
        }
        if(RequireWarning&&!Enemy->IsActivelyTelegraphing(Now))continue;
        double Dist=FVector::DistSquared(GetActorLocation(),Enemy->GetActorLocation());
        if(Dist>=Best)continue;
        FCollisionQueryParams Query(SCENE_QUERY_STAT(UnmadeStudyLineOfSight),false);
        Query.AddIgnoredActor(this);Query.AddIgnoredActor(Enemy);
        if(GetWorld()->LineTraceTestByChannel(GetActorLocation()+FVector(0,0,50),
            Enemy->GetActorLocation()+FVector(0,0,50),ECC_Visibility,Query))continue;
        Best=Dist;Nearest=Enemy;
    }
    return Nearest;
}

bool AUnmadeCharacter::NearEchoStudySite()const
{
    if(!GetWorld())return false;
    for(TActorIterator<AUnmadeLoreSite> It(GetWorld());It;++It)
    {
        if(It->GetDistrict()!=UnmadeCore::District::EchoWell ||
           FVector::DistSquared(GetActorLocation(),It->GetActorLocation())>FMath::Square(320.0))continue;
        FCollisionQueryParams Query(SCENE_QUERY_STAT(UnmadeStudyEchoWell),false);
        Query.AddIgnoredActor(this);Query.AddIgnoredActor(*It);
        if(!GetWorld()->LineTraceTestByChannel(GetActorLocation()+FVector(0,0,50),
            It->GetActorLocation()+FVector(0,0,50),ECC_Visibility,Query))return true;
    }
    return false;
}

FString AUnmadeCharacter::GetStudyJournal()const
{
    FString Out=TEXT("ABILITY STUDY — REQUIREMENT CHECKLIST (5 Observe, 9 Progress, 0 Use, O Cycle)\n");
    static const TCHAR* StageNames[]={TEXT("unknown"),TEXT("observed"),TEXT("understood"),
                                      TEXT("experimented"),TEXT("mastered")};
    for(int i=0;i<UnmadeCore::StudyCount;++i)
    {
        const auto Id=static_cast<UnmadeCore::StudyId>(i);
        const int Stage=static_cast<int>(AbilityStudy.Stage(Id));
        Out+=FString::Printf(TEXT("\n%s%s | %s | trials %d/2\n"),
            i==SelectedStudy?TEXT("> "):TEXT("  "),
            UTF8_TO_TCHAR(UnmadeCore::StudyNames[i]),StageNames[Stage],AbilityStudy.Trials(Id));
        const auto Requirements=AbilityStudy.Checklist(Id);
        for(const auto& R:Requirements)
            Out+=FString::Printf(TEXT("  [%s] %s\n"),R.complete?TEXT("x"):TEXT(" "),
                UTF8_TO_TCHAR(R.label));
    }
    // Surface real, local learning sources beside the abstract checklist. The
    // journal never treats proximity as evidence; the existing action rules
    // still require the warning, damage, site inspection, or rite objective.
    Out+=TEXT("\nLIVING SOURCES NEARBY\n");
    bool bFoundSource=false;
    for(int32 Subject=0;Subject<3;++Subject)
    {
        if(AUnmadeEnemyCharacter* Enemy=FindStudyEnemy(Subject,false))
        {
            UnmadeCore::CreatureKind Species;
            if(Enemy->StudyCreature(Species))
            {
                const TCHAR* State=Enemy->IsActivelyTelegraphing(GetWorld()->GetTimeSeconds())
                    ? TEXT("warning active — press 5 to observe")
                    : TEXT("living subject — watch for its attack warning");
                Out+=FString::Printf(TEXT("  Creature: %s (%s)\n"),
                    UTF8_TO_TCHAR(UnmadeCore::StudyNames[Subject]),State);
                bFoundSource=true;
            }
        }
    }
    if(NearEchoStudySite())
    {
        Out+=TEXT("  Place: physical Echo Well (inspect its reflection; press 5)\n");
        bFoundSource=true;
    }
    if(GetWorld())
    {
        const FVector Here=GetActorLocation();
        for(const auto& Rite:UnmadeCore::RiteSpecs)
        {
            for(TActorIterator<AUnmadeNpcCharacter> It(GetWorld());It;++It)
            {
                AUnmadeNpcCharacter* Resident=*It;
                if(!Resident||Resident->GetStableId()!=FName(UTF8_TO_TCHAR(Rite.witness))||
                   FVector::DistSquared(Here,Resident->GetActorLocation())>FMath::Square(700.0))continue;
                FCollisionQueryParams Query(SCENE_QUERY_STAT(UnmadeStudyMentorVisibility),false);
                Query.AddIgnoredActor(this);Query.AddIgnoredActor(Resident);
                if(GetWorld()->LineTraceTestByChannel(Here+FVector(0,0,50),
                    Resident->GetActorLocation()+FVector(0,0,50),ECC_Visibility,Query))continue;
                Out+=FString::Printf(TEXT("  Person: %s — %s at %s (continue on the Rites page)\n"),
                    *Resident->GetDisplayLabel(),UTF8_TO_TCHAR(Rite.name),UTF8_TO_TCHAR(Rite.siteName));
                bFoundSource=true;
            }
        }
    }
    if(!bFoundSource)
        Out+=TEXT("  No named mentor, study creature, or Echo Well is in clear view. Explore, then check this journal again.\n");
    Out+=TEXT("People teach through their authored rite and witness path; creatures and places require their own firsthand evidence. Being nearby never grants a skill.\n");
    Out+=TEXT("\nATLAS OF ABSENCE — CANON / ACQUISITION CHECKLIST\n");
    const auto Atlas=CurrentAtlasRequirements();
    const auto Requirements=AbilityStudy.AtlasChecklist(Atlas);
    for(const auto& R:Requirements)
        Out+=FString::Printf(TEXT("  [%s] %s\n"),R.complete?TEXT("x"):TEXT(" "),
            UTF8_TO_TCHAR(R.label));
    Out+=FString::Printf(TEXT("  [%s] Reward legitimately claimed and saved\n"),
        Atlas.rewardAlreadyClaimed?TEXT("x"):TEXT(" "));
    if(AbilityStudy.CanClaimAtlas(Atlas))
        Out+=TEXT("All requirements complete: select Memory Lens at Echo Well and press 9 to claim.\n");
    else Out+=TEXT("Unmet requirements must be earned in the world; rumors, duplicates, and button presses are not substitutes.\n");
    if(bStudySaveRejected)Out+=TEXT("WARNING: Study save rejected; progression and rewards are disabled to protect the existing profile.\n");
    Out+=TEXT("Enemy experiments require a new live attack warning, 10 Strain, and 8 seconds recovery. Mastery costs 5 Strain; learned casts cost 9 Strain.\n");
    return Out;
}
void AUnmadeCharacter::StudyNext()
{
    if(IsRpgMenuOpen())return;
    const int32 Before=SelectedStudy;
    SelectedStudy=(SelectedStudy+1)%UnmadeCore::StudyCount;
    if(!PersistStudy()){SelectedStudy=Before;UE_LOG(LogTemp,Warning,TEXT("Study selection save failed; previous selection restored."));}
    if(GEngine)UnmadeRpgFeedback(-1,7.f,FColor::Cyan,GetStudyJournal());
}

void AUnmadeCharacter::StudyObserve()
{
    if(!Combat||Combat->IsDefeated()||IsRpgMenuOpen()||bStudySaveRejected)return;
    const auto Id=static_cast<UnmadeCore::StudyId>(SelectedStudy);
    bool Valid=SelectedStudy==3?NearEchoStudySite():FindStudyEnemy(SelectedStudy,true)!=nullptr;
    if(!Valid)
    {
        if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Silver,
            SelectedStudy==3?TEXT("Go to the actual Echo Well to observe its reflection."):
            TEXT("Observe a nearby living creature of the selected species during its actual attack warning."));
        return;
    }
    RecordStudyEvidence(Id,1);
}

UnmadeCore::AtlasStudyContext AUnmadeCharacter::CurrentAtlasRequirements()const
{
    UnmadeCore::AtlasStudyContext Context;
    const auto* Save=Cast<UUnmadePrototypeSave>(UnmadeSave::LoadGameFromSlot());
    Context.physicallyAtEchoWell=NearEchoStudySite();
    Context.saveHealthy=!HasRejectedSave()&&IsValid(Equipment)&&!Equipment->HasRejectedSave();
    Context.rewardAlreadyClaimed=IsValid(Equipment)&&Equipment->HasEarnedAtlas();
    if(!Save||Save->SchemaVersion!=1)return Context;
    Context.echoInspected=(Save->DiscoveredLoreMask&1)!=0;
    Context.cartographyMastered=Save->bHasTenfoldChronicle&&
        Save->RiteStages.Num()==10&&Save->RiteStages[9]==5;
    Context.routePersonallyVerified=Save->bHasTenfoldChronicle&&
        Save->VerifiedRoadBits>0;
    Context.firstAbsenceWitnessed=Save->bHasConfluenceSnapshot&&
        Save->ConfluenceStages.Num()==6&&Save->ConfluenceStages[5]==2;
    return Context;
}

void AUnmadeCharacter::TryClaimAtlas()
{
    if(!IsValid(Equipment)||
       !AbilityStudy.CanClaimAtlas(CurrentAtlasRequirements()))return;
    if(Equipment->Claim(UnmadeCore::Achievement::AtlasOfAbsenceMastery)&&GEngine)
        UnmadeRpgFeedback(-1,9.f,FColor::Yellow,
            TEXT("ATLAS OF ABSENCE earned: mastered Cartography, proven road, First Absence witnesses, Echo Well and two creature principles. Equip the charm for -4 Glimpse Strain and +2 Fold seconds."));
}

void AUnmadeCharacter::StudyProgress()
{
    if(!GetWorld()||!Combat||Combat->IsDefeated()||IsRpgMenuOpen()||bStudySaveRejected)return;
    const auto Id=static_cast<UnmadeCore::StudyId>(SelectedStudy);
    const auto Stage=AbilityStudy.Stage(Id);
    const bool AtSubject=SelectedStudy==3?NearEchoStudySite():FindStudyEnemy(SelectedStudy)!=nullptr;
    const double Now=GetWorld()->GetTimeSeconds();
    if(Stage==UnmadeCore::StudyStage::Mastered)
    {
        if(SelectedStudy==3)TryClaimAtlas();
        if(GEngine)UnmadeRpgFeedback(-1,5.f,FColor::Cyan,GetStudyJournal());
        return;
    }
    if(Stage==UnmadeCore::StudyStage::Unknown)
    {
        if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Silver,TEXT("First observe the real subject (press 5)."));
        return;
    }
    if(Stage==UnmadeCore::StudyStage::Observed && SelectedStudy==3 && AbilityStudy.Evidence(Id)==1)
    {
        if(!AtSubject||!FractureModel.SpendStrain(8))
        {
            if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Silver,TEXT("Echo Well corroboration needs its physical site and 8 available Strain."));
            return;
        }
        if(!RecordStudyEvidence(Id,2))
        {
            FractureModel.Recover(8.0/UnmadeCore::FractureModel::RecoveryPerSecond);
            return;
        }
        if(GEngine)UnmadeRpgFeedback(-1,5.f,FColor::Cyan,TEXT("Echo Well corroborated. Press 9 again to understand."));
        return;
    }
    if(Stage==UnmadeCore::StudyStage::Observed)
    {
        const auto Before=AbilityStudy.Snapshot();
        if(AbilityStudy.Understand(Id)!=UnmadeCore::StudyResult::Advanced)
        {
            if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Silver,
                TEXT("Understanding needs both firsthand signals: observe the warning, then strike the observed enemy."));
            return;
        }
        if(!PersistStudy())AbilityStudy.Restore(Before);
        else if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Cyan,TEXT("Understanding earned. Return to the real subject for two experiments."));
        return;
    }
    if(!AtSubject)
    {
        if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Silver,TEXT("Revisit your live subject / Echo Well to perform the physical trial."));
        return;
    }
    if(Stage==UnmadeCore::StudyStage::Understood)
    {
        // Reproduce the studied principle during a fresh live telegraph, not by
        // standing beside an idle enemy and pressing the experiment key.
        if(SelectedStudy!=3 && !FindStudyEnemy(SelectedStudy,true))
        {
            if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Silver,
                TEXT("Wait for the real creature to demonstrate its attack again before experimenting."));
            return;
        }
        if(Now<NextStudyTrialAt||!FractureModel.SpendStrain(10))
        {
            if(GEngine)UnmadeRpgFeedback(-1,5.f,FColor::Silver,TEXT("Trial needs 10 available Strain and 8 seconds recovery."));
            return;
        }
        const auto Before=AbilityStudy.Snapshot();
        if(AbilityStudy.Experiment(Id)!=UnmadeCore::StudyResult::Advanced||!PersistStudy())
        {
            AbilityStudy.Restore(Before);
            FractureModel.Recover(10.0/UnmadeCore::FractureModel::RecoveryPerSecond);
            return;
        }
        NextStudyTrialAt=Now+8;
        if(SelectedStudy!=3)if(auto* Enemy=FindStudyEnemy(SelectedStudy))
            Enemy->ExposeToFold(Now,2.0); // A real, interrupted field experiment.
        if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Cyan,
            FString::Printf(TEXT("Field experiment %d/2 saved."),AbilityStudy.Trials(Id)));
        return;
    }
    if(Stage==UnmadeCore::StudyStage::Experimented)
    {
        if(!FractureModel.SpendStrain(5))
        {
            if(GEngine)UnmadeRpgFeedback(-1,5.f,FColor::Silver,TEXT("Final embodied mastery needs 5 available Strain."));
            return;
        }
        const auto Before=AbilityStudy.Snapshot();
        if(AbilityStudy.Master(Id)!=UnmadeCore::StudyResult::Advanced||!PersistStudy())
        {
            AbilityStudy.Restore(Before);
            FractureModel.Recover(5.0/UnmadeCore::FractureModel::RecoveryPerSecond);
            return;
        }
        if(GEngine)UnmadeRpgFeedback(-1,8.f,FColor::Yellow,
            FString::Printf(TEXT("MASTERED: %s. The earned technique is now usable (press 0)."),
                UTF8_TO_TCHAR(UnmadeCore::StudyNames[SelectedStudy])));
        TryClaimAtlas();
    }
}

void AUnmadeCharacter::StudyUse()
{
    if(!GetWorld()||!Combat||Combat->IsDefeated()||IsRpgMenuOpen()||bStudySaveRejected)return;
    const auto Id=static_cast<UnmadeCore::StudyId>(SelectedStudy);
    const double Now=GetWorld()->GetTimeSeconds();
    if(!AbilityStudy.HasMastered(Id)||Now<NextStudyPowerAt)return;
    AUnmadeEnemyCharacter* Enemy=FindStudyEnemy(-1);
    if((SelectedStudy==2||SelectedStudy==3)&&!Enemy)return;
    if(!FractureModel.SpendStrain(9))return;
    int Ward=0;
    double WardTime=6.0,ExposeSeconds=0.0;
    if(SelectedStudy==0)
    {
        Ward=5;
        if(AbilityStudy.CanCombine(UnmadeCore::StudyId::Cinderjaw,UnmadeCore::StudyId::EchoWell))
            WardTime=9;
        if(Enemy&&AbilityStudy.CanCombine(UnmadeCore::StudyId::Cinderjaw,UnmadeCore::StudyId::RedactionEye))
            ExposeSeconds=3;
    }
    else if(SelectedStudy==1)
    {
        if(!Combat->Dodge(Now))
        {
            FractureModel.Recover(9.0/UnmadeCore::FractureModel::RecoveryPerSecond);
            return;
        }
        Ward=AbilityStudy.CanCombine(UnmadeCore::StudyId::PaleStrider,UnmadeCore::StudyId::EchoWell)?2:0;
        WardTime=3;
    }
    else
    {
        ExposeSeconds=SelectedStudy==2?6:4;
        if(AbilityStudy.CanCombine(UnmadeCore::StudyId::RedactionEye,UnmadeCore::StudyId::EchoWell))
            ExposeSeconds=8;
        Ward=SelectedStudy==3?2:0;
    }
    // Save the paid Reality Strain before granting an encounter effect. No free
    // attack/ward is produced when persistence refuses this profile.
    if(!PersistStudy())
    {
        FractureModel.Recover(9.0/UnmadeCore::FractureModel::RecoveryPerSecond);
        return;
    }
    if(SelectedStudy==1)
    {
        LaunchCharacter(GetActorForwardVector()*650,false,false);
        PlayIdentityMotion(FName(TEXT("Dodge")));
    }
    if(Enemy&&ExposeSeconds>0)Enemy->ExposeToFold(Now,ExposeSeconds);
    if(Ward>0&&IsValid(Equipment))
    {
        Equipment->SetStudyBonuses(0,Ward);
        StudyWardUntil=FMath::Max(StudyWardUntil,Now+WardTime);
    }
    NextStudyPowerAt=Now+18;
    if(GEngine)UnmadeRpgFeedback(-1,6.f,FColor::Cyan,
        FString::Printf(TEXT("Earned technique: %s (9 Strain, 18s recovery)."),
            UTF8_TO_TCHAR(UnmadeCore::StudyNames[SelectedStudy])));
}

