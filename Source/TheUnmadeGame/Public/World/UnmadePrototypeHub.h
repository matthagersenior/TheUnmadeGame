#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC/UnmadeNpcDecisionRules.h"
#include "World/UnmadeLivingWorldRules.h"
#include "TimerManager.h"
#include "UnmadePrototypeHub.generated.h"

class AUnmadeLoreSite;
class ADirectionalLight;

UCLASS()
class THEUNMADEGAME_API AUnmadePrototypeHub : public AActor
{
    GENERATED_BODY()

public:
    AUnmadePrototypeHub();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    UnmadeCore::DayPhase GetCurrentPhase() const { return Clock.Phase(); }
    int32 GetGameDay() const { return Clock.DayIndex() + 1; }
    int32 GetMinuteOfDay() const { return Clock.MinuteOfDay(); }
    int32 GetDiscoveredCount() const { return Discoveries.Count(); }
    bool InspectSite(AUnmadeLoreSite* Site);
    void RefreshDistrictMood();

    /** Created at runtime so the initial experiment needs no hand-authored .umap. */
    void BuildForPrototype();

private:
    bool bBuilt = false;
    FTimerHandle GossipTimer;
    FTimerHandle WorldSaveTimer;
    UPROPERTY(Transient)
    TObjectPtr<ADirectionalLight> Sunlight;
    UnmadeCore::LivingWorldClock Clock;
    UnmadeCore::DiscoveryLedger Discoveries;
    FName LastAmbientSite = NAME_None;
    UnmadeCore::DayPhase LastAmbientPhase = UnmadeCore::DayPhase::Day;
    void RestoreLivingWorld();
    void SaveLivingWorld();
    bool WriteWorldSnapshot();
    int32 ReadStoryChoice() const;
    void SpawnLoreSite(UnmadeCore::District District, FName SiteId,
        const TCHAR* Name, const TCHAR* DayLine, const TCHAR* NightLine, FVector Position);
    void SpreadLocalRumors();
    void SaveCitizens();
    void SpawnBlock(FVector Center, FVector Scale, FName Label);
    void SpawnCitizen(FName Id, const TCHAR* DisplayName, FVector Position,
        UnmadeCore::NpcRole Role, UnmadeCore::NpcTemperament Temperament);
    void RestoreCitizens();
};
