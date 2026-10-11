#include "Player/UnmadeCharacter.h"
#include "Fracture/UnmadeFractureAnchor.h"
#include "Story/UnmadeConflictGate.h"
#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeLoreSite.h"
#include "Combat/UnmadeCombatComponent.h"
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
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AUnmadeCharacter::AttemptMeleeAttack);
    PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &AUnmadeCharacter::StartGuard);
    PlayerInputComponent->BindAction("Guard", IE_Released, this, &AUnmadeCharacter::StopGuard);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AUnmadeCharacter::Interact);
    PlayerInputComponent->BindAction("OfferAid", IE_Pressed, this, &AUnmadeCharacter::OfferAid);
    PlayerInputComponent->BindAction("AnomalyPulse", IE_Pressed, this, &AUnmadeCharacter::DemonstrateAnomaly);
    PlayerInputComponent->BindAction("FoldReality", IE_Pressed, this, &AUnmadeCharacter::FoldReality);
    PlayerInputComponent->BindAction("RewriteOpen", IE_Pressed, this, &AUnmadeCharacter::RewriteOpen);
    PlayerInputComponent->BindAction("RewriteSealed", IE_Pressed, this, &AUnmadeCharacter::RewriteSealed);
    PlayerInputComponent->BindAxis("MoveForward", this, &AUnmadeCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AUnmadeCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AUnmadeCharacter::ReconcileEarnedRewards()
{
    if (IsValid(Equipment)) Equipment->ReconcileEarnedMilestones();
}

void AUnmadeCharacter::ShowInventory()
{
    ReconcileEarnedRewards();
    if (IsValid(Equipment) && GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, Equipment->DescribeInventory());
}

void AUnmadeCharacter::EquipNextWeapon()
{
    if (IsValid(Equipment) && Equipment->EquipNext(UnmadeCore::GearSlot::Weapon))
        ShowInventory();
}

void AUnmadeCharacter::EquipNextArmor()
{
    if (IsValid(Equipment) && Equipment->EquipNext(UnmadeCore::GearSlot::Armor))
        ShowInventory();
}

void AUnmadeCharacter::EquipNextCharm()
{
    if (IsValid(Equipment) && Equipment->EquipNext(UnmadeCore::GearSlot::Charm))
        ShowInventory();
}

void AUnmadeCharacter::UseHealthPotion()
{
    if (!IsValid(Equipment)) return;
    int32 Restored = 0;
    const UnmadeCore::ItemId Potions[] = {
        UnmadeCore::ItemId::HearthSalve, UnmadeCore::ItemId::NightwatchTonic,
        UnmadeCore::ItemId::RootboundPoultice, UnmadeCore::ItemId::AshOfPossibleLives
    };
    for (const auto Id : Potions)
        if (Equipment->UseConsumable(Id, FractureModel.CurrentStrain(), Restored))
        {
            if (Restored > 0) FractureModel.Recover(Restored / UnmadeCore::FractureModel::RecoveryPerSecond);
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
                TEXT("Healing consumed. Health and any Strain recovery applied."));
            return;
        }
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver,
        TEXT("No useful healing consumable in your bag."));
}

void AUnmadeCharacter::UseStrainPotion()
{
    if (!IsValid(Equipment)) return;
    int32 Restored = 0;
    const UnmadeCore::ItemId Potions[] = {
        UnmadeCore::ItemId::StrainVial, UnmadeCore::ItemId::AshOfPossibleLives
    };
    for (const auto Id : Potions)
        if (Equipment->UseConsumable(Id, FractureModel.CurrentStrain(), Restored))
        {
            if (Restored > 0) FractureModel.Recover(Restored / UnmadeCore::FractureModel::RecoveryPerSecond);
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
                TEXT("Stillness restored: Reality Strain diminished."));
            return;
        }
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver,
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
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Yellow,
            TEXT("Waybreaker can only be forged at Bellwold's old workshop."));
        return;
    }
    if (!Equipment->ForgeWaybreaker() && GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Yellow,
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
           It->GetCombat()->IsDefeated() ||
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
                if(It->GetCombat()->IsDefeated() ||
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
            if(It->GetBossId()!=UnmadeCore::BossId::HollowBell ||
               FVector::DistSquared(GetActorLocation(),It->GetActorLocation())>FMath::Square(1400.f))
                continue;
            It->SetActorEnableCollision(false);
            It->SetActorHiddenInGame(true);
            It->SetActorTickEnabled(false);
            if(IsValid(Equipment))Equipment->Claim(UnmadeCore::Achievement::HollowBell);
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,9.f,FColor::Green,
                TEXT("HOLLOW KEEPER SPARED: testimony ended the battle without a killing blow."));
            break;
        }
        for(TActorIterator<AUnmadeRealmGuardian> Guardian(World);Guardian;++Guardian)
            if(FVector::DistSquared(GetActorLocation(),Guardian->GetActorLocation())
                <FMath::Square(1000.f))
                Guardian->ExposeToFold(Now,5.0);
        break;
    case UnmadeCore::RiteId::BorrowedLives:
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,7.f,FColor::Cyan,
            TEXT("ANOTHER LIFE: an unlived fighter's instincts change your attack temporarily."));
        break;
    case UnmadeCore::RiteId::LegacyForging:
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,7.f,FColor::Cyan,
            TEXT("LEGACY STEEL: distinct deeds remembered by your blade increase attack."));
        break;
    case UnmadeCore::RiteId::TomorrowDebt:
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Orange,
            TEXT("FUTURE BORROWED: powerful for now; the debt must be repaid tomorrow."));
        break;
    case UnmadeCore::RiteId::Oathbinding:
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Cyan,
            TEXT("OATH ARMOR: a promise carries weight. Breaking it has permanent consequences."));
        break;
    default: break;
    }
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
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Yellow,
                TEXT("No reachable realm crossing. Look for the marked stone gateways."));
        }
        else
        {
            if(IsValid(Tenfold))
            {
                const FVector Destination=GetActorLocation();
                if(Destination.Y < -46000) Tenfold->VerifyExploredFrontier(0);
                else if(Destination.Y > 46000) Tenfold->VerifyExploredFrontier(1);
            }
            ReconcileEarnedRewards();
        }
        break;
    }
}

void AUnmadeCharacter::BuyMarketSupplies()
{
    if(!IsValid(Equipment) || !GetWorld())return;
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
            bMerchantNearby=true;
            break;
        }
    }
    if(!bMerchantNearby)
    {
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Yellow,
            TEXT("Trade: find a merchant in this village first."));
        return;
    }
    const auto Id=Village==UnmadeCore::SettlementId::Bellwold
        ? UnmadeCore::ItemId::WildHerbs
        : Village==UnmadeCore::SettlementId::Paperhaven
          ? UnmadeCore::ItemId::BlankParchment : UnmadeCore::ItemId::IronScrap;
    int Trust=0;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        const auto Home=static_cast<UnmadeCore::Community>(static_cast<int32>(Village));
        Trust=Hub->GetCommunityOutcome(Home).tradeTrust;
        break;
    }
    if(Equipment->Buy(Id,Village,Trust))
    {
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Green,
            TEXT("Local materials purchased. Press I to view remaining marks."));
    }
    else if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Yellow,
        TEXT("Trade failed: insufficient marks, no bag space, or unavailable stock."));
}

void AUnmadeCharacter::SellMarketSupplies()
{
    if(!IsValid(Equipment) || !GetWorld())return;
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
            bMerchantNearby=true;
            break;
        }
    }
    if(!bMerchantNearby)return;
    const auto Id=Village==UnmadeCore::SettlementId::Bellwold
        ? UnmadeCore::ItemId::WildHerbs
        : Village==UnmadeCore::SettlementId::Paperhaven
          ? UnmadeCore::ItemId::BlankParchment : UnmadeCore::ItemId::IronScrap;
    if(!Equipment->Sell(Id,Village) && GEngine)
        GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Yellow,
            TEXT("No tradable surplus of this village's commodity."));
}

void AUnmadeCharacter::CraftLocalRecipe()
{
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
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Yellow,
            TEXT("Find the village's workshop or scriptorium to practice a profession."));
        return;
    }
    if(!Equipment->CraftAt(Village) && GEngine)
        GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Yellow,
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
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,7.f,FColor::Yellow,
                TEXT("No matching final witness and evidence nearby for this outcome."));
            break;
        }
        const FString ScopeValue=Scope.ToString();
        const auto Attempt=CommitmentGate.Attempt(
            std::string(TCHAR_TO_UTF8(*ScopeValue)),static_cast<int32>(Ending),
            GetWorld()->GetTimeSeconds());
        if(Attempt!=UnmadeCore::CommitmentAttempt::Confirmed)
        {
            if(GEngine)GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Yellow,
                FString::Printf(TEXT("PERMANENT DECISION: %s | Press SAME choice by SAME witness within six seconds."),
                    *Consequence));
            break;
        }
        const bool bResolvedFaction=Hub->ResolveNearbyFaction(Ending);
        const bool bResolvedFrontier=!bResolvedFaction &&
            Hub->ResolveNearbyFrontier(static_cast<int32>(Ending));
        const bool bResolvedAfterlight=!bResolvedFaction && !bResolvedFrontier &&
            Hub->ResolveNearbyAfterlight(static_cast<int32>(Ending));
        const bool bResolvedRealmAftermath=!bResolvedFaction && !bResolvedFrontier &&
            !bResolvedAfterlight && Hub->ResolveNearbyRealmAftermath(static_cast<int32>(Ending));
        const bool bResolvedBraid=!bResolvedFaction && !bResolvedFrontier &&
            !bResolvedAfterlight && !bResolvedRealmAftermath &&
            Hub->ResolveNearbyWitnessBraid(static_cast<int32>(Ending));
        const bool bResolvedReturn=!bResolvedFaction && !bResolvedFrontier &&
            !bResolvedAfterlight && !bResolvedRealmAftermath && !bResolvedBraid &&
            Hub->ResolveNearbyWitnessReturn(static_cast<int32>(Ending));
        if(bResolvedFaction || bResolvedFrontier || bResolvedAfterlight ||
           bResolvedRealmAftermath || bResolvedBraid || bResolvedReturn)
        {
            ReconcileEarnedRewards();
            if(bResolvedAfterlight)
                ReportLocalEvent(Ending==UnmadeCore::FactionEnding::Solidarity?
                    FName("World.AfterlightShelter"):FName("World.AfterlightNames"),
                    FName("region.bellwold.refuge"));
            else if(bResolvedRealmAftermath)
            {
                bool bLater=false;
                for(const auto& Region:UnmadeCore::LaterRealms)
                {
                    if(FVector::DistSquared2D(GetActorLocation(),
                        FVector(Region.centerX,Region.centerY,GetActorLocation().Z))>
                        FMath::Square(3200.f))continue;
                    bLater=true;
                    ReportLocalEvent(FName("World.LaterRealmResolved"),
                        FName(UTF8_TO_TCHAR(Region.name)));
                    break;
                }
                if(!bLater)
                {
                const bool bRain=GetActorLocation().Y < -46000.f;
                ReportLocalEvent(FName(bRain
                    ?(Ending==UnmadeCore::FactionEnding::Solidarity
                      ?TEXT("World.SaltwakeCistern"):TEXT("World.SaltwakeLedger"))
                    :(Ending==UnmadeCore::FactionEnding::Solidarity
                      ?TEXT("World.CinderholdKiln"):TEXT("World.CinderholdDeed"))),
                    FName(bRain?TEXT("region.saltwake.port"):TEXT("region.cinderhold.hearth")));
                }
            }
            else if(bResolvedBraid)
                ReportLocalEvent(FName(Ending==UnmadeCore::FactionEnding::Solidarity
                    ?TEXT("World.WitnessBraidShelter"):TEXT("World.WitnessBraidDocket")),
                    FName("region.crossings.bridge"));
            else if(bResolvedReturn)
                ReportLocalEvent(FName(Ending==UnmadeCore::FactionEnding::Solidarity
                    ?TEXT("World.HessaPrivateCounsel"):TEXT("World.HessaPublicHearing")),
                    FName("region.bellwold.refuge"));
            else ReportLocalEvent(FName("World.FactionResolved"),FName("region.prototype.hub"));
        }
        else if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Silver,
            TEXT("Find your local final witness, or assemble all three echo relics and return to Orrel's nail."));
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
    return UGameplayStatics::SaveGameToSlot(Save, TEXT("UnmadePrototypeNPC"), 0);
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
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Yellow,
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
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Yellow,
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
            GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Orange, Warning);
        }
        return;
    }

    PendingStoryChoice = UnmadeCore::ConflictChoice::None;
    const UnmadeCore::ConflictSnapshot Previous = LocalConflict.Snapshot();
    if (LocalConflict.Commit(Choice, true) != UnmadeCore::ConflictResult::Committed)
        return;
    if (!SaveLocalConflict())
    {
        LocalConflict.Restore(Previous);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red,
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
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Yellow,
        bShelter ? TEXT("COMMUNITY ROUTE OPEN; archive closed. This decision persists.")
                 : TEXT("ARCHIVE ROUTE OPEN; community shelter closed. This decision persists."));
    ReportLocalEvent(bShelter ? FName("World.ConflictShelter") : FName("World.ConflictResearch"),
        FName("region.prototype.hub"));
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
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Yellow,
            TEXT("Supply errand: collect once at the market, then deliver at the shelter. Completed deliveries cannot be repeated."));
        return;
    }

    if (!SaveLocalConflict())
    {
        LocalConflict.Restore(Previous);
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Red,
            TEXT("Supply activity could not be saved; progress restored."));
        return;
    }
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Cyan,
        bCompletedDelivery ? TEXT("SUPPLIES DELIVERED: the shelter remembers your help.")
                           : TEXT("SUPPLIES COLLECTED: bring these provisions to the shelter."));
    if (bCompletedDelivery)
    {
        ReconcileEarnedRewards();
        ReportLocalEvent(FName("Player.DeliveredSupplies"), FName("Hub.Shelter"));
    }
}

void AUnmadeCharacter::ShowEvidenceNotebook()
{
    if(!GetWorld())return;
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
    {
        const FString Evidence=Hub->GetEvidenceNotebook();
        // Text-mode input remains useful in the graybox; this same function
        // is BlueprintPure on the hub for a proper accessible UMG notebook.
        UE_LOG(LogTemp,Display,TEXT("%s"),*Evidence);
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,16.f,FColor::Cyan,Evidence);
        return;
    }
}

void AUnmadeCharacter::ShowStoryJournal()
{
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
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan,
        FString::Printf(TEXT("JOURNAL | %s | %s | Villages %d/3, landmarks %d/6 | Bellwold lanterns %d/2 | Paperhaven testimony %d/2 | Dispute: %s | Supplies: %s | VEYL clues: %d/2 | Strain: %.0f/100"),
            *WorldTime, *VillageName, VillagesSeen, SitesSeen,
            BellwoldQuest, PaperhavenQuest, *Decision, *Supply, Clues, FractureModel.CurrentStrain()));
    if(GEngine && !FactionSummary.IsEmpty())
        GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Cyan,FactionSummary);
    if(GEngine && !RealmSummary.IsEmpty())
        GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Cyan,RealmSummary);
    if(GEngine && !MainRoadSummary.IsEmpty())
        GEngine->AddOnScreenDebugMessage(-1,18.f,FColor::Yellow,MainRoadSummary);
    if(GEngine && !WitnessEchoSummary.IsEmpty())
        GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Cyan,WitnessEchoSummary);
}

void AUnmadeCharacter::AttemptMeleeAttack()
{
    UWorld* World = GetWorld();
    if (!World || Combat->IsDefeated()) return;
    AUnmadeEnemyCharacter* Target = nullptr;
    double BestDistanceSq = FMath::Square(240.0);
    const FVector Facing = GetActorForwardVector().GetSafeNormal2D();
    for (TActorIterator<AUnmadeEnemyCharacter> It(World); It; ++It)
    {
        if (It->GetCombat()->IsDefeated()) continue;
        FVector Toward = It->GetActorLocation() - GetActorLocation();
        Toward.Z = 0;
        const double DistSq = Toward.SizeSquared();
        if (DistSq >= BestDistanceSq || FVector::DotProduct(Facing, Toward.GetSafeNormal()) < 0.2)
            continue;
        FCollisionQueryParams Params(SCENE_QUERY_STAT(UnmadeMeleeSight), false);
        Params.AddIgnoredActor(this);
        Params.AddIgnoredActor(*It);
        if (World->LineTraceTestByChannel(GetActorLocation() + FVector(0, 0, 60),
            It->GetActorLocation() + FVector(0, 0, 60), ECC_Visibility, Params))
            continue;
        Target = *It;
        BestDistanceSq = DistSq;
    }
    if (!IsValid(Target))
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Silver,
            TEXT("No enemy within melee reach and facing."));
        return;
    }
    const float Before = Target->GetCombat()->GetHealth();
    const double Now = World->GetTimeSeconds();
    if (Combat->TryStrikeTarget(Target->GetCombat(), Now, true,
        Target->IsFractureExposed(Now)))
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
            FString::Printf(TEXT("HIT: %.0f damage, enemy %.0f health."),
                Before - Target->GetCombat()->GetHealth(), Target->GetCombat()->GetHealth()));
        if (Target->GetCombat()->IsDefeated() && IsValid(Equipment))
        {
            const AUnmadeBossCharacter* Boss=Cast<AUnmadeBossCharacter>(Target);
            if(Boss)
            {
                UnmadeCore::Achievement Reward=UnmadeCore::Achievement::Count;
                switch(Boss->GetBossId())
                {
                case UnmadeCore::BossId::HollowBell: Reward=UnmadeCore::Achievement::HollowBell; break;
                case UnmadeCore::BossId::RedactedCurator: Reward=UnmadeCore::Achievement::RedactedCurator; break;
                case UnmadeCore::BossId::UnfinishedPilgrim: Reward=UnmadeCore::Achievement::UnfinishedPilgrim; break;
                default: break;
                }
                if(Reward!=UnmadeCore::Achievement::Count) Equipment->Claim(Reward);
            }
            else Equipment->Claim(Target->GetEnemyStyle()==UnmadeCore::EnemyStyle::Stalker
                ? UnmadeCore::Achievement::FirstStalker:UnmadeCore::Achievement::FirstWatcher);
        }
        ReportLocalEvent(FName("Player.Fought"), FName("combat.prototype.enemy"));
    }
}

void AUnmadeCharacter::StartGuard()
{
    if (IsValid(Combat)) Combat->SetGuarding(true);
}

void AUnmadeCharacter::StopGuard()
{
    if (IsValid(Combat)) Combat->SetGuarding(false);
}

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
            Nearest = *It;
            BestDistSq = DistSq;
        }
    }
    return Nearest;
}
}

void AUnmadeCharacter::Interact()
{
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
                if(IsValid(Tenfold))
                    Tenfold->RecordLegacyDeed(UnmadeCore::Deed::Discovered);
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
            GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Cyan, Inscription);
        }
        else if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Cyan,
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
                ReportLocalEvent(FName("Player.HelpedVillage"), Target->GetStableId());
            }
            Hub->TryFactionConversation(Target->GetStableId());
            Hub->TryFrontierConversation(Target->GetStableId());
            Hub->TryAfterlightConversation(Target->GetStableId());
            Hub->TryRealmAftermathConversation(Target->GetStableId());
            Hub->TryLaterRealmConversation(Target->GetStableId());
            Hub->TryEchoQuestConversation(Target->GetStableId());
            Hub->RecordResidentConversation(Target->GetStableId());
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
    if (Dialogue)
    {
        Dialogue->RequestDialogue(Target, TEXT("Hello."));
    }
    else if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Cyan, Target->GetReactionText());
    }
}

void AUnmadeCharacter::OfferAid()
{
    AUnmadeNpcCharacter* Target = FindNearbyCitizen(GetWorld(), GetActorLocation(), 260.f);
    if (!Target)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Approach a resident before offering aid."));
        return;
    }
    for (const FUnmadeNpcObservation& Observation : Target->GetMemory()->GetObservations())
    {
        if (Observation.EventKind == FName("Player.Helped") && Observation.SubjectId == Target->GetStableId())
        {
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                TEXT("You have already helped this resident."));
            return;
        }
    }
    ReportLocalEvent(FName("Player.Helped"), Target->GetStableId());
    for(TActorIterator<AUnmadePrototypeHub> Hub(GetWorld());Hub;++Hub)
        Hub->RecordResidentAid(Target->GetStableId());
}

void AUnmadeCharacter::DemonstrateAnomaly()
{
    AUnmadeFractureAnchor* Anchor = FindNearbyFractureAnchor();
    if (!GetWorld()) return;
    const int32 Discount = IsValid(Equipment) ? Equipment->GlimpseDiscount() : 0;
    const auto Result = FractureModel.Glimpse(IsValid(Anchor), GetWorld()->GetTimeSeconds(), Discount);
    if (Result != UnmadeCore::Result::Applied)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Glimpse unavailable. Get closer to the fracture or recover Strain."));
        return;
    }
    ApplyFractureVisuals();
    if (Lexicon) Lexicon->RecordEvidence(FName("evidence.glimpse"));
    ReconcileEarnedRewards();
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        FString::Printf(TEXT("GLIMPSE: another possible version flickers into view (+%d Strain)."),
            8 - Discount));
    ReportLocalEvent(FName("Reality.Anomaly"), FName("region.prototype.hub"));
    SaveFractureState();
}

void AUnmadeCharacter::FoldReality()
{
    AUnmadeFractureAnchor* Anchor = FindNearbyFractureAnchor();
    if (!GetWorld()) return;
    const int32 Extension = IsValid(Equipment) ? Equipment->FoldBonusSeconds() : 0;
    const auto Result = FractureModel.Fold(IsValid(Anchor), GetWorld()->GetTimeSeconds(), Extension);
    if (Result != UnmadeCore::Result::Applied)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Fold unavailable: already active, out of range, or Strain too high."));
        return;
    }
    ApplyFractureVisuals();
    for (TActorIterator<AUnmadeEnemyCharacter> It(GetWorld()); It; ++It)
    {
        if (!It->GetCombat()->IsDefeated() &&
            FVector::DistSquared(It->GetActorLocation(), Anchor->GetActorLocation()) < FMath::Square(800.f))
            It->ExposeToFold(GetWorld()->GetTimeSeconds(), 4.0);
    }
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        FString::Printf(TEXT("FOLD: barrier vanishes for %d seconds (+24 Strain)."),
            6 + Extension));
    ReportLocalEvent(FName("Reality.Anomaly"), FName("region.prototype.hub"));
    SaveFractureState();
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
    if (!GetWorld() || !IsValid(FindNearbyFractureAnchor()))
    {
        PendingRewriteChoice = NAME_None;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
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
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                TEXT("This world has already been committed or the choice is unavailable."));
            return;
        }
        PendingRewriteChoice = ChoiceId;
        PendingRewriteExpiresAt = Now + 6.0;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Orange,
            FString::Printf(TEXT("PERMANENT REWRITE %s (+60 Strain). Press the same key again within 6 seconds to confirm."), *ChoiceString));
        return;
    }
    PendingRewriteChoice = NAME_None;
    const UnmadeCore::FractureSnapshot Previous = FractureModel.TakeSnapshot();
    const auto Result = FractureModel.Rewrite("region.prototype.hub", Choice, true);
    if (Result != UnmadeCore::Result::Applied)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            TEXT("Rewrite refused. Strain may be too high."));
        return;
    }
    ApplyFractureVisuals();
    if (!SaveFractureState())
    {
        FractureModel.Restore(Previous);
        ApplyFractureVisuals();
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red,
            TEXT("Rewrite failed to save: world and Strain restored."));
        return;
    }
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Yellow,
        FString::Printf(TEXT("REWRITE COMMITTED: %s. This outcome persists across sessions."), *ChoiceString));
    ReportLocalEvent(FName("Reality.Anomaly"), FName("region.prototype.hub"));
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
    return UGameplayStatics::SaveGameToSlot(Save, TEXT("UnmadePrototypeNPC"), 0);
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
    return UGameplayStatics::SaveGameToSlot(Save,TEXT("UnmadePrototypeNPC"),0);
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
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,12.f,FColor::Cyan,
        FString::Printf(TEXT("IDENTITY | %s | Frame %d, Face %d, Hair %d, Voice %d, Palette %d, Mark %d, Gait %d, Calling %d | Origin: THE IMPOSSIBLE"),
            *GetCharacterChosenName(),V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7]));
    // A full creator UI can call SetCharacterFeature/SetCharacterChosenName
    // from future UMG widgets; keyboard debug text is not a shipped creator.
}

void AUnmadeCharacter::BeginPlay()
{
    Super::BeginPlay();
    const UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"), 0));
    if (Save && Save->SchemaVersion == 1 && Save->bHasFractureSnapshot)
    {
        const FString Variant = Save->WorldVariantId.IsNone()
            ? TEXT("") : Save->WorldVariantId.ToString();
        if (!FractureModel.Restore({std::string(TCHAR_TO_UTF8(*Variant)), Save->PlayerStrain}))
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid fracture save ignored"));
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
        UE_LOG(LogTemp, Warning, TEXT("Invalid local conflict snapshot ignored"));
    }
    ApplyFractureVisuals();
    ApplyConflictGates();
    ReconcileEarnedRewards();
}


void AUnmadeCharacter::RecoverAtSafeCheckpoint()
{
    if(!GetWorld() || !Combat->IsDefeated() ||
       GetWorld()->GetTimeSeconds()<DefeatRecordedAt+4.0)return;
    // Safe start platforms exist in the Threefold Reach and each real frontier.
    // No Story/Inventory/Memory snapshot is reset or rewritten on death.
    const FVector Current=GetActorLocation();
    const double Y=Current.Y;
    FVector Safe=(Y<-45000.0)?FVector(0,-50000,135)
        :(Y>45000.0)?FVector(0,50000,135):FVector(0,0,135);
    for(const auto& Realm:UnmadeCore::LaterRealms)
    {
        if(FVector::DistSquared2D(Current,
             FVector(Realm.centerX,Realm.centerY,Current.Z))>FMath::Square(3600.f))
            continue;
        Safe=FVector(Realm.centerX,Realm.centerY-1170,135);
        break;
    }
    if(!SetActorLocation(Safe,false,nullptr,ETeleportType::TeleportPhysics))
        return;
    if(!Combat->ReviveAtCheckpoint())return;
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    bPlayerDefeatHandled=false;
    DefeatRecordedAt=-1.0;
    if(GEngine)GEngine->AddOnScreenDebugMessage(-1,9.f,FColor::Green,
        TEXT("RETURNED TO SAFETY: wounds stabilized, but your choices and witnesses remain."));
}

void AUnmadeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    FractureModel.Recover(static_cast<double>(DeltaSeconds) *
        (IsValid(Equipment) ? Equipment->StrainRecoveryMultiplier() : 1.0));
    // The world's clock drives a readable three-stage pulse. A guard reduces
    // damage through the combat rules; moving out of the marked lane avoids it.
    if(GetWorld() && !Combat->IsDefeated())
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
                if(GEngine)GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Yellow,
                    FString::Printf(TEXT("WARNING: storm/heat surge at world-second %.1f. Guard or leave the marked lane."),
                        Hub->GetWorldClockSeconds()));
            }
            if(Hazard.phase==UnmadeCore::FrontierHazardPhase::Impact &&
               Hazard.pulseId!=LastHazardImpactPulse[Index])
            {
                LastHazardImpactPulse[Index]=Hazard.pulseId;
                if(Combat->ReceiveHazardPulse(
                    static_cast<uint64>(0xF0100+Index),Hazard.pulseId,Hazard.damage) && GEngine)
                    GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Orange,
                        TEXT("FRONTIER SURGE: you were exposed. Guard next pulse or operate the control."));
            }
            break;
        }
    }

    // Six land-specific warnings share the source world clock, not a random
    // per-frame roll. One pulse can affect a player at most once.
    if(GetWorld() && !Combat->IsDefeated())
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
                if(GEngine)GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Yellow,
                    FString::Printf(TEXT("%s: %s"),
                        UTF8_TO_TCHAR(UnmadeCore::LaterHazards[Index].label),
                        UTF8_TO_TCHAR(Hazard.warning)));
            }
            if(Hazard.phase==UnmadeCore::FrontierHazardPhase::Impact &&
               LastLaterHazardPulse[Index]!=Hazard.pulseId)
            {
                LastLaterHazardPulse[Index]=Hazard.pulseId;
                bool bApplied=false;
                if(Hazard.effect==UnmadeCore::LaterHazardEffect::Injury)
                    bApplied=Combat->ReceiveHazardPulse(
                        Hazard.sourceId,Hazard.pulseId,Hazard.severity);
                else if(Hazard.effect==UnmadeCore::LaterHazardEffect::Strain)
                {
                    const auto Before=FractureModel.TakeSnapshot();
                    bApplied=FractureModel.ApplyEnvironmentalStrain(Hazard.severity);
                    if(bApplied && !SaveFractureState())
                    {
                        FractureModel.Restore(Before);
                        bApplied=false;
                        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,
                           TEXT("Unmade could not persist reality strain; the exposure was undone."));
                    }
                }
                if(bApplied && GEngine)
                    GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Orange,
                        FString::Printf(TEXT("%s: %s"),
                            UTF8_TO_TCHAR(UnmadeCore::LaterHazards[Index].label),
                            UTF8_TO_TCHAR(Hazard.counterplay)));
            }
            break;
        }
    }
    if (Combat->IsDefeated() && !bPlayerDefeatHandled)
    {
        bPlayerDefeatHandled=true;
        DefeatRecordedAt=GetWorld()?GetWorld()->GetTimeSeconds():0.0;
        GetCharacterMovement()->DisableMovement();
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,
            TEXT("YOU FELL: returning to this realm's safe entry in four seconds."));
    }
    if(bPlayerDefeatHandled)RecoverAtSafeCheckpoint();
    ApplyFractureVisuals();
}

void AUnmadeCharacter::ReportLocalEvent(FName EventKind, FName SubjectId)
{
    UWorld* World = GetWorld();
    if (!World) return;
    const FGuid EventId = FGuid::NewGuid();
    int32 Witnesses = 0;
    for (TActorIterator<AUnmadeNpcCharacter> It(World); It; ++It)
    {
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
    SaveNearbyNpcMemories();
    if(IsValid(Tenfold))
    {
        if(EventKind==FName("Player.Helped") || EventKind==FName("Player.HelpedVillage"))
            Tenfold->RecordLegacyDeed(UnmadeCore::Deed::Protected);
        else if(EventKind==FName("World.FactionResolved"))
            Tenfold->RecordLegacyDeed(UnmadeCore::Deed::Reconciled);
        else if(EventKind==FName("Player.Threatened"))
            Tenfold->BreakChosenOath();
    }
    const FString Line = FString::Printf(TEXT("%s witnessed by %d nearby resident(s)."),
        *EventKind.ToString(), Witnesses);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Yellow, Line);
}

void AUnmadeCharacter::SaveNearbyNpcMemories()
{
    UWorld* World = GetWorld();
    if (!World) return;
    UUnmadePrototypeSave* Save = UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return;
    Save->NpcSnapshots.Reset(); // Keep one snapshot per stable NPC ID.
    for (TActorIterator<AUnmadeNpcCharacter> It(World); It; ++It)
    {
        if (!It->GetStableId().IsNone())
            Save->NpcSnapshots.Add(It->GetMemory()->WriteSnapshot(It->GetStableId()));
    }
    if (!UGameplayStatics::SaveGameToSlot(Save, TEXT("UnmadePrototypeNPC"), 0))
    {
        UE_LOG(LogTemp, Error, TEXT("Unmade prototype NPC memory save failed"));
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Red,
            TEXT("Could not save NPC memories."));
    }
}
