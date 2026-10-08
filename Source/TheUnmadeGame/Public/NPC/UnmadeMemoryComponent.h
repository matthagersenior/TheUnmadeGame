#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnmadeMemoryComponent.generated.h"

/** The source of a belief is persisted; hearing something is not witnessing it. */
UENUM(BlueprintType)
enum class EUnmadeEvidenceKind : uint8
{
    Witnessed,
    Rumor
};

USTRUCT(BlueprintType)
struct FUnmadeNpcObservation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FGuid EventId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FName EventKind = NAME_None;

    /** Stable ID of the person/place the event concerns. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FName SubjectId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    EUnmadeEvidenceKind Evidence = EUnmadeEvidenceKind::Witnessed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FName SpeakerId = NAME_None;
};

USTRUCT(BlueprintType)
struct FUnmadeNpcSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FName NpcId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    TArray<FUnmadeNpcObservation> Observations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    int32 Trust = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    int32 Fear = 0;
};

/** Bounded, deduplicated personal recollection. No automatic omniscient broadcasts. */
UCLASS(ClassGroup=(Unmade), meta=(BlueprintSpawnableComponent))
class THEUNMADEGAME_API UUnmadeMemoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UUnmadeMemoryComponent();

    UFUNCTION(BlueprintCallable, Category="Unmade|Memory")
    bool Witness(const FGuid& EventId, FName EventKind, FName SubjectId = NAME_None);

    UFUNCTION(BlueprintCallable, Category="Unmade|Memory")
    bool HearRumor(const FGuid& EventId, FName EventKind, FName SpeakerId, FName SubjectId = NAME_None);

    UFUNCTION(BlueprintPure, Category="Unmade|Memory")
    int32 GetTrust() const { return Trust; }

    UFUNCTION(BlueprintPure, Category="Unmade|Memory")
    int32 GetFear() const { return Fear; }

    UFUNCTION(BlueprintPure, Category="Unmade|Memory")
    int32 GetObservationCount() const { return Observations.Num(); }

    UFUNCTION(BlueprintPure, Category="Unmade|Memory")
    bool KnowsEvent(const FGuid& EventId) const;

    FUnmadeNpcSnapshot WriteSnapshot(FName NpcId) const;
    bool ReadSnapshot(const FUnmadeNpcSnapshot& Snapshot, FName ExpectedNpcId);

    const TArray<FUnmadeNpcObservation>& GetObservations() const { return Observations; }

private:
    UPROPERTY(SaveGame)
    TArray<FUnmadeNpcObservation> Observations;

    UPROPERTY(SaveGame)
    int32 Trust = 0;

    UPROPERTY(SaveGame)
    int32 Fear = 0;

    bool AddObservation(const FUnmadeNpcObservation& Observation);
    void RecalculateReactions();
};
