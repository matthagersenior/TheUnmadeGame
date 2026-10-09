#include "Player/UnmadeCharacter.h"
#include "Fracture/UnmadeFractureAnchor.h"
#include "Story/UnmadeConflictGate.h"
#include "World/UnmadePrototypeHub.h"
#include "World/UnmadeLoreSite.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeBossCharacter.h"
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
}

void AUnmadeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Early bootstrap mappings. Replace with data-driven Enhanced Input contexts in an engine editor.
    PlayerInputComponent->BindAction("StoryShelter", IE_Pressed, this, &AUnmadeCharacter::ChooseShelter);
    PlayerInputComponent->BindAction("StoryResearch", IE_Pressed, this, &AUnmadeCharacter::ChooseResearch);
    PlayerInputComponent->BindAction("SupplyActivity", IE_Pressed, this, &AUnmadeCharacter::ProgressSupplyActivity);
    PlayerInputComponent->BindAction("StoryJournal", IE_Pressed, this, &AUnmadeCharacter::ShowStoryJournal);
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
        PlayerInputComponent->BindAction("RiteNext",IE_Pressed,Tenfold,&UUnmadeTenfoldComponent::NextRite);
        PlayerInputComponent->BindAction("RitePrevious",IE_Pressed,Tenfold,&UUnmadeTenfoldComponent::PreviousRite);
        PlayerInputComponent->BindAction("RiteJournal",IE_Pressed,Tenfold,&UUnmadeTenfoldComponent::ShowRite);
        PlayerInputComponent->BindAction("RiteStudy",IE_Pressed,Tenfold,&UUnmadeTenfoldComponent::StudyRite);
        PlayerInputComponent->BindAction("RiteUse",IE_Pressed,Tenfold,&UUnmadeTenfoldComponent::InvokeRite);
        PlayerInputComponent->BindAction("RiteChoice1",IE_Pressed,Tenfold,&UUnmadeTenfoldComponent::DecideSolidarity);
        PlayerInputComponent->BindAction("RiteChoice2",IE_Pressed,Tenfold,&UUnmadeTenfoldComponent::DecideTruth);
        PlayerInputComponent->BindAction("RiteBreak",IE_Pressed,Tenfold,&UUnmadeTenfoldComponent::BreakChosenOath);
        PlayerInputComponent->BindAction("RiteLaw",IE_Pressed,Tenfold,&UUnmadeTenfoldComponent::CycleWorldLaw);
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
void AUnmadeCharacter::ReportBrokenOathEvent()
{
    ReportLocalEvent(FName("World.BrokenOath"),FName("Bellwold.Refuge"));
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
    const auto* Save=Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"),0));
    if(Save && Save->bHasFactionChronicle && Save->FactionEndings.Num()==3)
    {
        const int Index=Village==UnmadeCore::SettlementId::Bellwold?0
            : Village==UnmadeCore::SettlementId::Paperhaven?1:2;
        Trust=Save->FactionEndings[Index]==1?40:Save->FactionEndings[Index]==2?25:0;
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
        const bool bResolvedFaction=Hub->ResolveNearbyFaction(Ending);
        const bool bResolvedFrontier=!bResolvedFaction &&
            Hub->ResolveNearbyFrontier(static_cast<int32>(Ending));
        if(bResolvedFaction || bResolvedFrontier)
        {
            ReconcileEarnedRewards();
            ReportLocalEvent(FName("World.FactionResolved"),FName("region.prototype.hub"));
        }
        else if(GEngine)GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Silver,
            TEXT("Find your final faction or frontier representative before choosing."));
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
        Hub->RefreshDistrictMood();
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
    if (!Target)
    {
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

void AUnmadeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    FractureModel.Recover(static_cast<double>(DeltaSeconds) *
        (IsValid(Equipment) ? Equipment->StrainRecoveryMultiplier() : 1.0));
    if (Combat->IsDefeated() && !bPlayerDefeatHandled)
    {
        bPlayerDefeatHandled = true;
        GetCharacterMovement()->DisableMovement();
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red,
            TEXT("You fell in the prototype encounter. Checkpoint/respawn is not implemented yet."));
    }
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
