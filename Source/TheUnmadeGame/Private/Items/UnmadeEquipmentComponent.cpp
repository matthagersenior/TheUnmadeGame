#include "Items/UnmadeEquipmentComponent.h"
#include "Save/UnmadePrototypeSave.h"
#include "Player/UnmadeCharacter.h"
#include "Combat/UnmadeCombatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UUnmadeEquipmentComponent::UUnmadeEquipmentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UUnmadeEquipmentComponent::BeginPlay()
{
    Super::BeginPlay();
    const UUnmadePrototypeSave* Save = Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"),0));
    if (Save && Save->SchemaVersion == 1 && Save->bHasInventorySnapshot)
    {
        UnmadeCore::InventorySnapshot Snapshot;
        bool bValid = Save->ItemQuantities.Num() >= 28 &&
            Save->ItemQuantities.Num() <= static_cast<int32>(UnmadeCore::ItemId::Count)
            && Save->EquippedItems.Num() == 3 && Save->AwardedMilestoneBits >= 0;
        if (bValid)
        {
            for (int32 i=0;i<Save->ItemQuantities.Num();++i)
                Snapshot.quantities[i]=Save->ItemQuantities[i];
            for (int32 i=0;i<3;++i)
                Snapshot.equipped[i]=Save->EquippedItems[i];
            Snapshot.milestones=static_cast<std::uint64_t>(Save->AwardedMilestoneBits);
            bValid=Inventory.Restore(Snapshot);
        }
        if (!bValid)
        {
            bSaveRejected = true;
            UE_LOG(LogTemp,Error,TEXT("Invalid inventory save: refused to overwrite existing data"));
            return;
        }
    }

    if(Save && Save->SchemaVersion==1 && Save->bHasEconomySnapshot)
    {
        UnmadeCore::EconomySnapshot PreviousEconomy;
        bool bValid=Save->ProfessionSkills.Num()==3 && Save->PaidContractMask>=0;
        if(bValid)
        {
            PreviousEconomy.marks=Save->TradeMarks;
            for(int32 i=0;i<3;++i) PreviousEconomy.craftsmanship[i]=Save->ProfessionSkills[i];
            PreviousEconomy.completedContracts=static_cast<uint32>(Save->PaidContractMask);
            bValid=Economy.Restore(PreviousEconomy);
        }
        if(!bValid)
        {
            bSaveRejected=true;
            UE_LOG(LogTemp,Error,TEXT("Corrupt economy state rejected; no overwrite"));
            return;
        }
    }
    const auto Previous=Inventory.Snapshot();
    if (Inventory.Claim(UnmadeCore::Achievement::Starter)==UnmadeCore::RewardResult::Awarded)
    {
        Inventory.Equip(UnmadeCore::ItemId::SalvagerBlade);
        Inventory.Equip(UnmadeCore::ItemId::TravelerMantle);
        if (!Persist()) Inventory.Restore(Previous);
    }
    RefreshCombatBonuses();
}

bool UUnmadeEquipmentComponent::Persist(double UpdatedStrain)
{
    if (bSaveRejected) return false;
    UUnmadePrototypeSave* Save=UUnmadePrototypeSave::LoadOrCreate();
    if (!Save) return false;
    const auto& Snapshot=Inventory.Snapshot();
    Save->ItemQuantities.Reset();
    for (int Amount : Snapshot.quantities) Save->ItemQuantities.Add(Amount);
    Save->EquippedItems.Reset();
    for (int Id : Snapshot.equipped) Save->EquippedItems.Add(Id);
    Save->AwardedMilestoneBits=static_cast<int64>(Snapshot.milestones);
    Save->bHasInventorySnapshot=true;
    const auto EconomyState=Economy.Snapshot();
    Save->bHasEconomySnapshot=true;
    Save->TradeMarks=EconomyState.marks;
    Save->ProfessionSkills.Reset();
    for(int Rank:EconomyState.craftsmanship)Save->ProfessionSkills.Add(Rank);
    Save->PaidContractMask=static_cast<int32>(EconomyState.completedContracts);
    if (UpdatedStrain >= 0.0)
    {
        Save->PlayerStrain = UpdatedStrain;
        Save->bHasFractureSnapshot = true;
    }
    return UGameplayStatics::SaveGameToSlot(Save,TEXT("UnmadePrototypeNPC"),0);
}

void UUnmadeEquipmentComponent::RefreshCombatBonuses()
{
    AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner());
    if (IsValid(Player) && IsValid(Player->GetCombat()))
        Player->GetCombat()->SetGearBonuses(Inventory.AttackBonus(),Inventory.ArmorBonus());
}

bool UUnmadeEquipmentComponent::Claim(UnmadeCore::Achievement Reward)
{
    if (bSaveRejected) return false;
    const auto Before=Inventory.Snapshot();
    if (Inventory.Claim(Reward)!=UnmadeCore::RewardResult::Awarded) return false;
    if (!Persist()) { Inventory.Restore(Before); return false; }
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1,6.f,FColor::Yellow,
        TEXT("EARNED REWARD: press I for items, weapon and armor."));
    return true;
}

void UUnmadeEquipmentComponent::ReconcileEarnedMilestones()
{
    const UUnmadePrototypeSave* Save=Cast<UUnmadePrototypeSave>(
        UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"),0));
    if (bSaveRejected || !Save || Save->SchemaVersion!=1) return;
    const int32 Sites=Save->DiscoveredLoreMask;
    struct Entry { UnmadeCore::Achievement Id; bool Earned; };
    const Entry Earned[]={
        {UnmadeCore::Achievement::BellwoldLanterns,Save->BellwoldTaskStage==2},
        {UnmadeCore::Achievement::PaperhavenTestimony,Save->PaperhavenTaskStage==2},
        {UnmadeCore::Achievement::ThreeVillages,(Save->VisitedSettlementsMask&7)==7},
        {UnmadeCore::Achievement::EchoWell,(Sites&1)!=0},
        {UnmadeCore::Achievement::PaperOrchard,(Sites&2)!=0},
        {UnmadeCore::Achievement::SilentMile,(Sites&4)!=0},
        {UnmadeCore::Achievement::BellGrave,(Sites&8)!=0},
        {UnmadeCore::Achievement::DebtMarket,(Sites&16)!=0},
        {UnmadeCore::Achievement::AllLandmarks,(Sites&63)==63},
        {UnmadeCore::Achievement::ShelterSupplies,Save->bHasConflictSnapshot&&Save->SupplyActivityStage==2},
        {UnmadeCore::Achievement::ShelterChoice,Save->bHasConflictSnapshot&&Save->LocalConflictChoice==1},
        {UnmadeCore::Achievement::ResearchChoice,Save->bHasConflictSnapshot&&Save->LocalConflictChoice==2},
        {UnmadeCore::Achievement::LanguageVeyl,
            Save->LexiconEvidence.Contains(FName("evidence.glimpse")) &&
            Save->LexiconEvidence.Contains(FName("evidence.archivist"))}
    };
    for(const Entry& Reward: Earned)
        if(Reward.Earned) Claim(Reward.Id); // idempotent; never trusts model text
    ReconcileCivicContracts(Save);
}

bool UUnmadeEquipmentComponent::EquipNext(UnmadeCore::GearSlot Slot)
{
    if (bSaveRejected) return false;
    const auto Before=Inventory.Snapshot();
    const int32 count=static_cast<int32>(UnmadeCore::ItemId::Count);
    const auto Current=Inventory.Equipped(Slot);
    const int32 currentIndex = Current == UnmadeCore::ItemId::Count ? -1 : static_cast<int32>(Current);
    for(int32 step=1;step<=count;++step)
    {
        const int32 index=(currentIndex+step)%count;
        const auto* Def=UnmadeCore::FindItem(static_cast<UnmadeCore::ItemId>(index));
        if(!Def || Def->slot!=Slot || Inventory.Quantity(Def->id)<1) continue;
        if(!Inventory.Equip(Def->id)) return false;
        if(!Persist()) { Inventory.Restore(Before); return false; }
        RefreshCombatBonuses();
        return true;
    }
    return false;
}

bool UUnmadeEquipmentComponent::UseConsumable(UnmadeCore::ItemId Id,
    double CurrentStrain,int32& OutStrain)
{
    OutStrain=0;
    if (bSaveRejected) return false;
    AUnmadeCharacter* Player=Cast<AUnmadeCharacter>(GetOwner());
    if(!IsValid(Player) || !IsValid(Player->GetCombat()) || Player->GetCombat()->IsDefeated())
        return false;
    const auto Before=Inventory.Snapshot();
    int heal=0,restore=0;
    if(Inventory.Consume(Id,Player->GetCombat()->GetMissingHealth(),
        CurrentStrain,heal,restore)!=UnmadeCore::ConsumeResult::Used) return false;
    if(!Persist(FMath::Max(0.0,CurrentStrain-static_cast<double>(restore))))
    {
        Inventory.Restore(Before);
        return false;
    }
    if(heal>0) Player->GetCombat()->Heal(heal);
    OutStrain=restore;
    return true;
}

bool UUnmadeEquipmentComponent::ForgeWaybreaker()
{
    if (bSaveRejected) return false;
    const auto Before=Inventory.Snapshot();
    if(!Inventory.ForgeWaybreaker()) return false;
    if(!Persist()) { Inventory.Restore(Before); return false; }
    if(GEngine) GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Orange,
        TEXT("MYTHIC FORGED: WAYBREAKER, THE IMPOSSIBLE ROAD"));
    return true;
}

void UUnmadeEquipmentComponent::ReconcileCivicContracts(const UUnmadePrototypeSave* Save)
{
    if(bSaveRejected || !Save || Save->SchemaVersion!=1)return;
    const struct Contract { UnmadeCore::SettlementId Town;int Stage; } Contracts[]={
        {UnmadeCore::SettlementId::Crossings,
            Save->bHasConflictSnapshot&&Save->LocalConflictChoice!=0?2:0},
        {UnmadeCore::SettlementId::Bellwold,Save->BellwoldTaskStage},
        {UnmadeCore::SettlementId::Paperhaven,Save->PaperhavenTaskStage}
    };
    for(const Contract& Contract:Contracts)
    {
        const auto Before=Economy.Snapshot();
        if(Economy.PayContract(Contract.Town,Contract.Stage)!=UnmadeCore::EconomyResult::Completed)
            continue;
        if(!Persist())Economy.Restore(Before);
    }
}

bool UUnmadeEquipmentComponent::Buy(UnmadeCore::ItemId Id,
    UnmadeCore::SettlementId Village,int Trust)
{
    if(bSaveRejected)return false;
    const auto BeforeItems=Inventory.Snapshot();
    const auto BeforeMoney=Economy.Snapshot();
    if(Economy.Buy(Inventory,Id,Village,1,Trust)!=UnmadeCore::EconomyResult::Completed)
        return false;
    if(!Persist())
    {
        Inventory.Restore(BeforeItems);
        Economy.Restore(BeforeMoney);
        return false;
    }
    return true;
}

bool UUnmadeEquipmentComponent::Sell(UnmadeCore::ItemId Id,
    UnmadeCore::SettlementId Village)
{
    if(bSaveRejected)return false;
    const auto BeforeItems=Inventory.Snapshot();
    const auto BeforeMoney=Economy.Snapshot();
    if(Economy.Sell(Inventory,Id,Village,1)!=UnmadeCore::EconomyResult::Completed)
        return false;
    if(!Persist())
    {
        Inventory.Restore(BeforeItems);
        Economy.Restore(BeforeMoney);
        return false;
    }
    return true;
}

bool UUnmadeEquipmentComponent::CraftAt(UnmadeCore::SettlementId Village)
{
    if(bSaveRejected)return false;
    for(const auto& Recipe:UnmadeCore::Recipes)
    {
        if(Recipe.workshop!=Village)continue;
        const auto BeforeItems=Inventory.Snapshot();
        const auto BeforeMoney=Economy.Snapshot();
        if(Economy.Craft(Inventory,Recipe.id,Village)!=UnmadeCore::EconomyResult::Completed)
            continue;
        if(!Persist())
        {
            Inventory.Restore(BeforeItems);
            Economy.Restore(BeforeMoney);
            return false;
        }
        if(GEngine)GEngine->AddOnScreenDebugMessage(-1,7.f,FColor::Yellow,
            FString::Printf(TEXT("CRAFTED: %s. Profession progress saved."),
                UTF8_TO_TCHAR(Recipe.title)));
        return true;
    }
    return false;
}

FString UUnmadeEquipmentComponent::DescribeInventory() const
{
    FString Summary=FString::Printf(TEXT("GEAR: +%d attack, +%d armor | "),
        Inventory.AttackBonus(),Inventory.ArmorBonus());
    for(int32 i=0;i<3;++i)
    {
        const auto* Def=UnmadeCore::FindItem(
            Inventory.Equipped(static_cast<UnmadeCore::GearSlot>(i)));
        Summary+=FString::Printf(TEXT("%s: %s | "),
            i==0?TEXT("Weapon"):i==1?TEXT("Armor"):TEXT("Charm"),
            Def?UTF8_TO_TCHAR(Def->name):TEXT("Empty"));
    }
    Summary+=FString::Printf(TEXT("Marks %d | Smith %d, Apothecary %d, Scribe %d | BAG: "),
        Economy.Marks(),Economy.Skill(UnmadeCore::Profession::Smith),
        Economy.Skill(UnmadeCore::Profession::Apothecary),
        Economy.Skill(UnmadeCore::Profession::Scribe));
    for(const auto& Def:UnmadeCore::ItemCatalog)
        if(Inventory.Quantity(Def.id)>0)
            Summary+=FString::Printf(TEXT("%s x%d; "),
                UTF8_TO_TCHAR(Def.name),Inventory.Quantity(Def.id));
    return Summary;
}
