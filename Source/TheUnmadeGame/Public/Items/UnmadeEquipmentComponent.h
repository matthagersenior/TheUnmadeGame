#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/UnmadeItemRules.h"
#include "Save/UnmadeRecoveryRules.h"
#include "Items/UnmadeUnifiedLoadoutRules.h"
#include "Items/UnmadeCraftEconomyRules.h"
class UUnmadePrototypeSave;
#include "UnmadeEquipmentComponent.generated.h"

UCLASS(ClassGroup=(Unmade), meta=(BlueprintSpawnableComponent))
class THEUNMADEGAME_API UUnmadeEquipmentComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    bool HasRejectedSave()const{return bSaveRejected;}
    UUnmadeEquipmentComponent();
    virtual void BeginPlay() override;
    bool Claim(UnmadeCore::Achievement Reward);
    void ReconcileEarnedMilestones();
    bool SelectRealmReward(int Arc);
    int RealmWeapon()const{return Loadout.weapon;}
    int RealmArmor()const{return Loadout.armor;}
    UnmadeCore::ExpeditionBenefit SelectedBenefit()const{return Loadout.Benefit();}
    FString DescribeComparison()const;
    void SetBuildBonuses(int Attack,int Armor){BuildAttack=Attack;BuildArmor=Armor;RefreshCombatBonuses();}
    bool EquipNext(UnmadeCore::GearSlot Slot);
    UnmadeCore::ConsumableCommit UseConsumable(UnmadeCore::ItemId Item, double CurrentStrain, int32& OutStrain);
    bool ForgeWaybreaker();
    bool Buy(UnmadeCore::ItemId Id,UnmadeCore::SettlementId Village,int Trust=0);
    bool Sell(UnmadeCore::ItemId Id,UnmadeCore::SettlementId Village);
    bool CraftAt(UnmadeCore::SettlementId Village,int Recipe=0);
    bool CraftFieldRecipe(int Realm,bool Learned);
    FString DescribeRecipe(int Recipe)const;
    UnmadeCore::ItemId EquippedWeapon()const{return Inventory.Equipped(UnmadeCore::GearSlot::Weapon);}
    int32 GetMarks() const { return Economy.Marks(); }
    int32 GetAtlasAttunement() const;
    FString DescribeInventory() const;
    int32 GlimpseDiscount() const { return Inventory.GlimpseStrainDiscount()+ExpeditionGlimpse; }
    int32 FoldBonusSeconds() const { return (Loadout.weapon>=0?0:Inventory.FoldDurationBonus())+ExpeditionFold; }
    double StrainRecoveryMultiplier() const { return Inventory.RecoveryMultiplier(); }
    void SetTemporaryBonuses(int32 Attack,int32 Armor) {
        TemporaryAttack=FMath::Clamp(Attack,-20,80);
        TemporaryArmor=FMath::Clamp(Armor,0,80);
        RefreshCombatBonuses();
    }
    void SetExpeditionBonuses(int Attack,int Armor,int Glimpse,int Fold){ExpeditionAttack=FMath::Clamp(Attack,0,26);ExpeditionArmor=FMath::Clamp(Armor,0,16);ExpeditionGlimpse=FMath::Clamp(Glimpse,0,3);ExpeditionFold=FMath::Clamp(Fold,0,2);RefreshCombatBonuses();}
private:
    UnmadeCore::UnifiedLoadout Loadout;
    int BuildAttack=0,BuildArmor=0;
    bool ReadOwnedExpeditions(UnmadeCore::Expeditions& Out)const;
    int ExpeditionAttack=0,ExpeditionArmor=0,ExpeditionGlimpse=0,ExpeditionFold=0;
    bool Persist(double UpdatedStrain = -1.0);
    bool bSaveRejected = false;
    FIntPoint NativeGearBonuses()const;
    void RefreshCombatBonuses();
    int32 TemporaryAttack=0;
    int32 TemporaryArmor=0;
    UnmadeCore::InventoryModel Inventory;
    UnmadeCore::RegionalEconomy Economy;
    void ReconcileCivicContracts(const UUnmadePrototypeSave* Save);
};
