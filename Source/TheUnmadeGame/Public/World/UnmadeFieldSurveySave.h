#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UnmadeFieldSurveySave.generated.h"

/** Independent optional fieldwork; never writes campaign, wages or inventory. */
UCLASS()
class THEUNMADEGAME_API UUnmadeFieldSurveySave : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY(SaveGame) int32 Version=1;
    UPROPERTY(SaveGame) TArray<int32> Stages;
};
