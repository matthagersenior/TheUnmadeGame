#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnmadePrototypeSave.generated.h"

/** Prototype save: NPC observations and one authored world choice; not yet a full RPG save system. */
UCLASS()
class THEUNMADEGAME_API UUnmadePrototypeSave : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame)
    int32 SchemaVersion = 1;

    UPROPERTY(SaveGame)
    TArray<FUnmadeNpcSnapshot> NpcSnapshots;

    /** Optional language evidence; schema v1 files treat absence as no clues. */
    UPROPERTY(SaveGame)
    TArray<FName> LexiconEvidence;

    UPROPERTY(SaveGame)
    bool bHasFractureSnapshot = false;

    UPROPERTY(SaveGame)
    FName WorldVariantId = NAME_None;

    UPROPERTY(SaveGame)
    double PlayerStrain = 0.0;

    /** Merges prototype sub-system writes instead of erasing unrelated state. */
    static UUnmadePrototypeSave* LoadOrCreate()
    {
        UUnmadePrototypeSave* Existing = Cast<UUnmadePrototypeSave>(
            UGameplayStatics::LoadGameFromSlot(TEXT("UnmadePrototypeNPC"), 0));
        if (Existing) return Existing->SchemaVersion == 1 ? Existing : nullptr;
        // Never replace an existing but unreadable save with an empty one.
        if (UGameplayStatics::DoesSaveGameExist(TEXT("UnmadePrototypeNPC"), 0))
            return nullptr;
        return Cast<UUnmadePrototypeSave>(UGameplayStatics::CreateSaveGameObject(StaticClass()));
    }
};
