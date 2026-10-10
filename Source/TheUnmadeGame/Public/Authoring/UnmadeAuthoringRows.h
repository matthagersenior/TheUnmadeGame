#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UnmadeAuthoringRows.generated.h"

// On first Unreal PC: Content Browser > Data Table > Import CSV.
// Name is the first CSV field and is owned by FTableRowBase automatically.
// These rows are authoring content, not authoritative quest/save state.
USTRUCT(BlueprintType)
struct THEUNMADEGAME_API FUnmadeNpcAuthoringRow : public FTableRowBase {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString StableId;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString DisplayName;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Realm;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString DayLine;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString NightLine;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Desire;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Fear;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString KnowledgeBoundary;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString VoiceDirection;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString VisualDirection;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Gesture;
};
USTRUCT(BlueprintType)
struct THEUNMADEGAME_API FUnmadeRealmAuthoringRow : public FTableRowBase {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Realm;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Culture;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Architecture;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString PublicPrinciple;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Ritual;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString SoundDirection;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString VisualDirection;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString OptionalActivity;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString CentralDispute;
};
USTRUCT(BlueprintType)
struct THEUNMADEGAME_API FUnmadeRiteAuthoringRow : public FTableRowBase {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Rite;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString ExistingSite;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString PhysicalVerb;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Telegraph;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Limitation;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString Synergy;
    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Unmade|Authoring") FString OuterRealmUses;
};
