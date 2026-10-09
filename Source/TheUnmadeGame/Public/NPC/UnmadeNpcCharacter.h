#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC/UnmadeNpcDecisionRules.h"
#include "UnmadeNpcCharacter.generated.h"

class UStaticMeshComponent;
class UUnmadeMemoryComponent;
class AUnmadePrototypeHub;

UCLASS()
class THEUNMADEGAME_API AUnmadeNpcCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AUnmadeNpcCharacter();
    virtual void Tick(float DeltaSeconds) override;
    void ConfigureIdentity(FName StableId, const FString& DisplayLabel,
        UnmadeCore::NpcRole InRole, UnmadeCore::NpcTemperament InTemperament);
    UnmadeCore::NpcAction DecideForPlayer(bool bPlayerNearby) const;
    UFUNCTION(BlueprintPure, Category="Unmade|NPC")
    FName GetCurrentActionId(bool bPlayerNearby = true) const;
    /** Entry-point for future trade UI: independent of dialogue model text. */
    UFUNCTION(BlueprintPure, Category="Unmade|NPC")
    bool CanTradeWithPlayer() const;
    FString GetReactionText() const;
    FName GetStableId() const { return NpcId; }
    const FString& GetDisplayLabel() const { return NpcDisplayLabel; }
    UUnmadeMemoryComponent* GetMemory() const { return Memory; }

private:
    UPROPERTY(VisibleAnywhere, Category="Unmade|Memory")
    TObjectPtr<UUnmadeMemoryComponent> Memory;

    UPROPERTY(VisibleAnywhere, Category="Unmade|Visual")
    TObjectPtr<UStaticMeshComponent> PlaceholderVisual;

    UPROPERTY()
    FName NpcId = NAME_None;

    UPROPERTY()
    FString NpcDisplayLabel;

    TWeakObjectPtr<AUnmadePrototypeHub> CachedHub;
    FVector HomeLocation = FVector::ZeroVector;

    UnmadeCore::NpcRole Role = UnmadeCore::NpcRole::Wanderer;
    UnmadeCore::NpcTemperament Temperament = UnmadeCore::NpcTemperament::Steady;
};
