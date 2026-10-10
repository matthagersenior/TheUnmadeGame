#pragma once
#include "CoreMinimal.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeFinalEncounterRules.h"
#include "UnmadeFinalBoss.generated.h"

/** Final battle actor is a graybox, NOT a finished cinematic/animated boss. */
UCLASS()
class THEUNMADEGAME_API AUnmadeFinalBoss : public AUnmadeEnemyCharacter
{
    GENERATED_BODY()
public:
    AUnmadeFinalBoss();
    virtual void Tick(float DeltaSeconds) override;
private:
    void ConfigureAct(UnmadeCore::FinalAct Act);
    UnmadeCore::FinalBattleRhythm Rhythm;
    int32 ConfiguredAct=-1;
    bool bWarningShown=false;
};
