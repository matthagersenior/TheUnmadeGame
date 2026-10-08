#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnmadeGameMode.generated.h"

/** Bootstrap game mode. Authored maps and game systems are separate milestones. */
UCLASS()
class THEUNMADEGAME_API AUnmadeGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AUnmadeGameMode();
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
