#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC/UnmadeNpcDecisionRules.h"
#include "TimerManager.h"
#include "UnmadePrototypeHub.generated.h"

UCLASS()
class THEUNMADEGAME_API AUnmadePrototypeHub : public AActor
{
    GENERATED_BODY()

public:
    AUnmadePrototypeHub();
    virtual void BeginPlay() override;

    /** Created at runtime so the initial experiment needs no hand-authored .umap. */
    void BuildForPrototype();

private:
    bool bBuilt = false;
    FTimerHandle GossipTimer;
    void SpreadLocalRumors();
    void SaveCitizens();
    void SpawnBlock(FVector Center, FVector Scale, FName Label);
    void SpawnCitizen(FName Id, const TCHAR* DisplayName, FVector Position,
        UnmadeCore::NpcRole Role, UnmadeCore::NpcTemperament Temperament);
    void RestoreCitizens();
};
