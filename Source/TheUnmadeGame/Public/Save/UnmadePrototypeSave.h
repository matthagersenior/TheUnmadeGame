#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NPC/UnmadeMemoryComponent.h"
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
};
