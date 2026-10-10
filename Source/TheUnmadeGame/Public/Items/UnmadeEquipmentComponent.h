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
    int32 GlimpseDiscount() const { return Inventory.GlimpseStrainDiscount(); }
    int32 FoldBonusSeconds() const { return Inventory.FoldDurationBonus(); }
    double StrainRecoveryMultiplier() const { return Inventory.RecoveryMultiplier(); }
    void SetTemporaryBonuses(int32 Attack,int32 Armor) {
        TemporaryAttack=FMath::Clamp(Attack,-20,80);
        TemporaryArmor=FMath::Clamp(Armor,0,80);
        RefreshCombatBonuses();
    }
private:
    bool Persist(double UpdatedStrain = -1.0);
    bool bSaveRejected = false;
    void RefreshCombatBonuses();
    int32 TemporaryAttack=0;
    int32 TemporaryArmor=0;
    UnmadeCore::InventoryModel Inventory;
    UnmadeCore::RegionalEconomy Economy;
    void ReconcileCivicContracts(const UUnmadePrototypeSave* Save);
};
