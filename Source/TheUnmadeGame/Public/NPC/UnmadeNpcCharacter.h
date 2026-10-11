#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC/UnmadeNpcDecisionRules.h"
#include "World/UnmadeSettlementRegistry.h"
#include "World/UnmadeFrontierRealmRules.h"
#include "World/UnmadeLaterRealmRules.h"
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
    virtual void BeginPlay() override;
    void ConfigureIdentity(FName StableId, const FString& DisplayLabel,
        UnmadeCore::NpcRole InRole, UnmadeCore::NpcTemperament InTemperament,
        UnmadeCore::SettlementId InVillage, const FString& InAuthoredLine);
    void ConfigureFrontier(const UnmadeCore::FrontierResident& Resident);
    void ConfigureLaterRealm(const UnmadeCore::LaterRealmSpec& Realm,
        const char* Identity,const FString& Display,
        double BlockedUntil=-1.0;
    FRotator BeforeBlocking=FRotator::ZeroRotator;
    bool bSkeletalRigLoaded=false;
    FName MotionCue=NAME_None;
    void TryLoadResidentRig();
    void AnimateResident(FName Cue);
    UnmadeCore::NpcRole Role,UnmadeCore::NpcTemperament Temperament,
        const char* AuthoredLine);
    void BeginDirectedBlocking(FVector LookAt, float Duration, FName Gesture);
    void EndDirectedBlocking();
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
    UnmadeCore::SettlementId GetHomeSettlement() const { return HomeSettlement; }

private:
    double NextRoutinePathAt=0;
    FVector RoutineWaypoint=FVector::ZeroVector;
    bool bRoutinePathValid=false;
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
    UnmadeCore::SettlementId HomeSettlement = UnmadeCore::SettlementId::Crossings;
    FString AuthoredLine;
    bool bFrontierResident = false;
    bool bLaterRealmResident = false;
    UnmadeCore::Realm LaterHome = UnmadeCore::Realm::Count;

    UnmadeCore::NpcRole Role = UnmadeCore::NpcRole::Wanderer;
    UnmadeCore::NpcTemperament Temperament = UnmadeCore::NpcTemperament::Steady;
};
