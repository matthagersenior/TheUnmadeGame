#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UnmadeQuestSceneRows.generated.h"

// Metadata authoring imports ONLY. Original runtime quests and SaveGame win.
USTRUCT(BlueprintType)
struct THEUNMADEGAME_API FUnmadeQuestStepAuthoringRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString Realm;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString Chapter;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString StepId;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString Trigger;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString Interaction;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString Objective;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString SpeakerId;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString ExactDialogue;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString Failure;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString Recovery;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString SavedState;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString Outcome;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString PhysicalTag;
};
USTRUCT(BlueprintType)
struct THEUNMADEGAME_API FUnmadeResidentReturnAuthoringRow : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString StableId;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString DisplayName;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString Realm;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString ReturnLine;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString AidLine;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString LocalPublicLine;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString NewMorningLine;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString PersonalConcern;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Story") FString NextVisitHook;
};
