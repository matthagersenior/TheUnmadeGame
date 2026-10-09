#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/UnmadeItemRules.h"
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
    FString DescribeInventory() const;
private:
    bool Persist(double UpdatedStrain = -1.0);
    bool bSaveRejected = false;
    void RefreshCombatBonuses();
    UnmadeCore::InventoryModel Inventory;
};
