#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "World/UnmadeTenfoldChronicle.h"
#include "World/UnmadeTimedProgressRules.h"
#include "World/UnmadeConfluenceRules.h"
#include <string>
#include "World/UnmadeChronicleHistoryRules.h"
#include "UnmadeTenfoldComponent.generated.h"

/** Story-authoritative ten-rite quest and power component. AI entirely optional. */
UCLASS(ClassGroup=(Unmade),meta=(BlueprintSpawnableComponent))
class THEUNMADEGAME_API UUnmadeTenfoldComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    bool HasRejectedSave()const{return bSaveRejected;}
    UUnmadeTenfoldComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaSeconds,ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;
    void NextRite();
    void PreviousRite();
    void ShowRite();
    void StudyRite();
    bool TryWitnessConversation(FName ResidentId);
    bool TryInspectRiteStone();
    void InvokeRite();
    void DecideSolidarity();
    void DecideTruth();
    void BreakChosenOath();
    void CycleWorldLaw();
    void CycleConfluence();
    void StudyConfluence();
    void ConfluenceChoice1();
    void ConfluenceChoice2();
    bool HasLegacyDeed(UnmadeCore::Deed Deed)const {
        const int Index=static_cast<int>(Deed);
        return Index>=0&&Index<3&&(Chronicle.Snapshot().deeds&(1u<<Index))!=0;
    }
    bool CommitActionDeeds(unsigned Mask,TFunctionRef<bool()> Write);
    bool CaptureChronicle(class UUnmadePrototypeSave* Save)const;
    bool HasPendingHistory()const{return History.Pending();}
    void RecordThreatenedOath();
    void RecordLegacyDeed(UnmadeCore::Deed Deed);
    bool CommitExploredFrontier(int32 RealmIndex,TFunctionRef<bool()> Write);
    void VerifyExploredFrontier(int32 RealmIndex);
    int32 RiteStage(UnmadeCore::RiteId Rite) const {return Chronicle.Stage(Rite);}
    bool IsMastered(UnmadeCore::RiteId Rite) const {return Chronicle.IsMastered(Rite);}
    int32 CurrentSelectedRite() const {return SelectedRite;}
    int32 BorrowedLifeRole() const {
        return IsBorrowedIdentityActive()?SelectedLife:-1;
    }
    bool IsBorrowedIdentityActive() const {
        return BonusExpiresAt>0 && ActiveRite==UnmadeCore::RiteId::BorrowedLives;
    }
private:
    UnmadeCore::RiteContext GatherContext();
    bool Persist(bool bWriteStrain=false);
    void DecideRite(int32 Choice);
    void ExplainResult(UnmadeCore::RiteResult Result) const;
    int32 FindNearbyConfluence() const;
    void ChooseConfluence(int32 Outcome);
    void RefreshTemporaryPowers(double CurrentTime,int32 Day);
    UnmadeCore::TenfoldChronicle Chronicle;
    UnmadeCore::ConfluenceJourney Confluence;
    int32 SelectedConfluence=0;
    int32 SelectedRite=0;
    int32 SelectedLaw=0;
    int32 SelectedLife=0;
    bool bSaveRejected=false;
    std::string WitnessBuffer;
    double BonusExpiresAt=0;
    int32 ActiveAttackBonus=0;
    int32 ActiveArmorBonus=0;
    int32 LastAppliedAttack=0;
    int32 LastAppliedArmor=0;
    UnmadeCore::RiteId ActiveRite=UnmadeCore::RiteId::Count;
    UnmadeCore::TimedProgressRetry DaySaveRetry;
    UnmadeCore::ChronicleHistory History;
    bool bRoutePresentationPending=false;
    void RetryHistorySave();
    void RemoveOathPowers();
};
