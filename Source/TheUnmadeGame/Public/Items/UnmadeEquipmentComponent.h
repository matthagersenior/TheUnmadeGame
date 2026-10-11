#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/UnmadeItemRules.h"
#include "Items/UnmadeCraftEconomyRules.h"
class UUnmadePrototypeSave;
#include "UnmadeEquipmentComponent.generated.h"

UCLASS(ClassGroup=(Unmade), meta=(BlueprintSpawnableComponent))
class THEUNMADEGAME_API UUnmadeEquipmentComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UUnmadeEquipmentComponent();
    virtual void BeginPlay() override;
    bool Claim(UnmadeCore::Achievement Reward);
    void ReconcileEarnedMilestones();
    bool EquipNext(UnmadeCore::GearSlot Slot);
    bool UseConsumable(UnmadeCore::ItemId Item, double CurrentStrain, int32& OutStrain);
    bool ForgeWaybreaker();
    bool Buy(UnmadeCore::ItemId Id,UnmadeCore::SettlementId Village,int Trust=0);
    bool Sell(UnmadeCore::ItemId Id,UnmadeCore::SettlementId Village);
    bool CraftAt(UnmadeCore::SettlementId Village);
    int32 GetMarks() const { return Economy.Marks(); }
    int32 GetAtlasAttunement() const;
    FString DescribeInventory() const;
    int32 GlimpseDiscount() const { return Inventory.GlimpseStrainDiscount()+ExpeditionGlimpse; }
    int32 FoldBonusSeconds() const { return Inventory.FoldDurationBonus()+ExpeditionFold; }
    double StrainRecoveryMultiplier() const { return Inventory.RecoveryMultiplier(); }
    void SetTemporaryBonuses(int32 Attack,int32 Armor) {
        TemporaryAttack=FMath::Clamp(Attack,-20,80);
        TemporaryArmor=FMath::Clamp(Armor,0,80);
        RefreshCombatBonuses();
    }
    void SetExpeditionBonuses(int Attack,int Armor,int Glimpse,int Fold){ExpeditionAttack=FMath::Clamp(Attack,0,26);ExpeditionArmor=FMath::Clamp(Armor,0,16);ExpeditionGlimpse=FMath::Clamp(Glimpse,0,3);ExpeditionFold=FMath::Clamp(Fold,0,2);RefreshCombatBonuses();}
private:
    int ExpeditionAttack=0,ExpeditionArmor=0,ExpeditionGlimpse=0,ExpeditionFold=0;
    bool Persist(double UpdatedStrain = -1.0);
    bool bSaveRejected = false;
    void RefreshCombatBonuses();
    int32 TemporaryAttack=0;
    int32 TemporaryArmor=0;
    UnmadeCore::InventoryModel Inventory;
    UnmadeCore::RegionalEconomy Economy;
    void ReconcileCivicContracts(const UUnmadePrototypeSave* Save);
};
