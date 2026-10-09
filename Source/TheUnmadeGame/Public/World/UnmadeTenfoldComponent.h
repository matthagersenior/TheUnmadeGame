#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "World/UnmadeTenfoldChronicle.h"
#include <string>
#include "UnmadeTenfoldComponent.generated.h"

/** Story-authoritative ten-rite quest and power component. AI entirely optional. */
UCLASS(ClassGroup=(Unmade),meta=(BlueprintSpawnableComponent))
class THEUNMADEGAME_API UUnmadeTenfoldComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UUnmadeTenfoldComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaSeconds,ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;
    void NextRite();
    void PreviousRite();
    void ShowRite();
    void StudyRite();
    void InvokeRite();
    void DecideSolidarity();
    void DecideTruth();
    void BreakChosenOath();
    void CycleWorldLaw();
    void RecordLegacyDeed(UnmadeCore::Deed Deed);
    void VerifyExploredFrontier(int32 RealmIndex);
    int32 RiteStage(UnmadeCore::RiteId Rite) const {return Chronicle.Stage(Rite);}
    bool IsMastered(UnmadeCore::RiteId Rite) const {return Chronicle.IsMastered(Rite);}
    int32 CurrentSelectedRite() const {return SelectedRite;}
    bool IsBorrowedIdentityActive() const {
        return BonusExpiresAt>0 && ActiveRite==UnmadeCore::RiteId::BorrowedLives;
    }
private:
    UnmadeCore::RiteContext GatherContext();
    bool Persist(bool bWriteStrain=false);
    void DecideRite(int32 Choice);
    void ExplainResult(UnmadeCore::RiteResult Result) const;
    void RefreshTemporaryPowers(double CurrentTime,int32 Day);
    UnmadeCore::TenfoldChronicle Chronicle;
    int32 SelectedRite=0;
    int32 SelectedLaw=0;
    bool bSaveRejected=false;
    std::string WitnessBuffer;
    double BonusExpiresAt=0;
    int32 ActiveAttackBonus=0;
    int32 ActiveArmorBonus=0;
    int32 LastAppliedAttack=0;
    int32 LastAppliedArmor=0;
    UnmadeCore::RiteId ActiveRite=UnmadeCore::RiteId::Count;
    double LastTickDayCheck=0;
};
