#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UnmadeCinematicAuthoringRows.generated.h"

// Offline-editable authoring only; import the generated CSVs to Unreal DataTables.
// Timed shot rows DO NOT automatically construct Sequencer, 3D levels or cameras.
USTRUCT(BlueprintType)
struct THEUNMADEGAME_API FUnmadeCinematicShotAuthoringRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Title;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Subtitle;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Narration;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString ArtSource;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") float StartSeconds=0;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") float DurationSeconds=0;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") float EndSeconds=0;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString DirectorNote;
};
USTRUCT(BlueprintType)
struct THEUNMADEGAME_API FUnmadeCinematicRealmAuthoringRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString RealmId;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Display;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Signature;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Landscape;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString MainQuestion;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Mechanism;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Danger;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString CareOutcome;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString TruthOutcome;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Enemy;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Ability;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString Foreshadow;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString ReturnChapter;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString HeldMorning;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString ManyMornings;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Cinematic") FString ManualTest;
};
