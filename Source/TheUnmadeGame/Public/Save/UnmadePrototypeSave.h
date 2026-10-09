#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnmadePrototypeSave.generated.h"

/** Narrow early playtest save: NPC memories only, not yet a full RPG save system. */
UCLASS()
class THEUNMADEGAME_API UUnmadePrototypeSave : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame)
    int32 SchemaVersion = 1;

    UPROPERTY(SaveGame)
    TArray<FUnmadeNpcSnapshot> NpcSnapshots;

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
        return Cast<UUnmadePrototypeSave>(UGameplayStatics::CreateSaveGameObject(StaticClass()));
    }
};
