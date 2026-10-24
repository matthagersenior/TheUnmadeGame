#pragma once
#include "CoreMinimal.h"
#include "Combat/UnmadeEnemyCharacter.h"
#include "Combat/UnmadeBossRules.h"
#include "Combat/UnmadeNamedBossRewardRules.h"
#include "Save/UnmadeDefeatSaveRetryRules.h"
#include "UnmadeBossCharacter.generated.h"

/** Graybox named encounter with mandatory telegraph and fracture counterplay. */
UCLASS()
class THEUNMADEGAME_API AUnmadeBossCharacter : public AUnmadeEnemyCharacter
{
    GENERATED_BODY()
public:
    AUnmadeBossCharacter();
    void ConfigureBoss(UnmadeCore::BossId Boss);
    virtual void Tick(float DeltaSeconds) override;
    virtual bool HasPendingDefeatSave(const AUnmadePrototypeHub& Hub)const override;
    void CommitDefeatReceipt();
    bool ResolveByMercy();
    UnmadeCore::BossId GetBossId() const { return BossId; }
private:
    UnmadeCore::BossId BossId=UnmadeCore::BossId::None;
    UnmadeCore::BossEncounter Encounter;
    bool bDefeatedHandled=false;
    bool bMercyPending=false;
    UnmadeCore::DefeatSaveRetry DefeatSave;
    bool bTelegraphShown=false;
};
