#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC/UnmadeNpcDecisionRules.h"
#include "NPC/UnmadeResidentContinuityRules.h"
#include "World/UnmadeLivingWorldRules.h"
#include "World/UnmadeWitnessEchoRules.h"
#include "World/UnmadeWitnessBraidRules.h"
#include "World/UnmadeWitnessDispatchRules.h"
#include "World/UnmadeWitnessReturnRules.h"
#include "World/UnmadeSettlementRegistry.h"
#include "World/UnmadeRegionalTaskRules.h"
#include "World/UnmadeFactionChronicleRules.h"
#include "World/UnmadeFrontierRealmRules.h"
#include "World/UnmadeTenfoldChronicle.h"
#include "World/UnmadeRealityConsequenceRules.h"
#include "World/UnmadeRealmReactionRules.h"
#include "World/UnmadeMaterialConsequencesRules.h"
#include "World/UnmadeLivingEconomyRules.h"
#include "World/UnmadeBesHouseholdRules.h"
#include "World/UnmadeCommunityConsequences.h"
#include "World/UnmadeAfterlightRules.h"
#include "World/UnmadeFrontierHazardRules.h"
#include "World/UnmadeRealmAftermathRules.h"
#include "World/UnmadeLaterRealmRules.h"
#include "World/UnmadeCampaignSpineRules.h"
#include "World/UnmadeEchoQuestRules.h"
#include "World/UnmadeRealmResonanceRules.h"
#include "Combat/UnmadeRealmGuardianRules.h"
#include "Combat/UnmadeFinalEncounterRules.h"
#include "Story/UnmadeCommitmentRules.h"
#include "World/UnmadeLaterHazardRules.h"
#include "TimerManager.h"
#include "UnmadePrototypeHub.generated.h"

class AUnmadeLoreSite;
class ADirectionalLight;
class AUnmadeCharacter;

UCLASS()
class THEUNMADEGAME_API AUnmadePrototypeHub : public AActor
{
    GENERATED_BODY()

public:
    AUnmadePrototypeHub();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    UnmadeCore::DayPhase GetCurrentPhase() const { return Clock.Phase(); }
    int32 GetGameDay() const { return Clock.DayIndex() + 1; }
    double GetWorldClockSeconds() const { return Clock.ElapsedSeconds(); }
    void ApplyRiteEnvironment(UnmadeCore::RiteId Id,double Duration,bool Mastered);
    /** One witnessed, local Tenfold cast updates architecture, combat and civic memory. */
    bool RecordRealityConsequence(UnmadeCore::RiteId Id,double Duration,bool Mastered,
                                  const FVector& CastPosition);
    UnmadeCore::RealityConsequenceView RealityConsequenceAt(FVector Position) const;
    int32 GetRealmHazardPercent(FVector Position) const;
    int32 GetRealmTradeTrustBonus(FVector Position) const;
    UFUNCTION(BlueprintPure,Category="Unmade|Reality")
    FString GetRealityConsequenceJournal() const;
    UFUNCTION(BlueprintPure,Category="Unmade|Economy")
    FString GetLocalMaterialJournal() const;
    bool TryMaterialInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool TryLivingTradeInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    FString GetLivingEconomyResidentLine(FName StableId) const;
    FString GetBesHouseholdResidentLine(FName StableId) const;
    bool RecordBesHouseholdConversation(FName StableId);
    bool TryBesHouseholdInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    UFUNCTION(BlueprintPure,Category="Unmade|Economy")
    FString GetBesHouseholdJournal() const;
    bool HasBesHouseholdWitness() const { return BesHousehold.EndgameWitness(); }
    bool IsBesOnWalkout() const { return BesHousehold.OnWalkout(); }
    UFUNCTION(BlueprintPure,Category="Unmade|Economy")
    FString GetLivingEconomyJournal() const;
    UnmadeCore::MaterialDuty GetMaterialDuty(FVector At,UnmadeCore::NpcRole Role) const;
    FVector GetMaterialDutyDestination(FVector At,UnmadeCore::MaterialDuty Duty) const;
    FString GetMaterialResidentLine(FVector At) const;
    int32 GetMinuteOfDay() const { return Clock.MinuteOfDay(); }
    int32 GetDiscoveredCount() const { return Discoveries.Count(); }
    int32 GetVisitedVillageCount() const { return VillagesVisited.Count(); }
    FString GetCurrentVillageName() const;
    bool InspectSite(AUnmadeLoreSite* Site);
    /** Source-authored optional physical callback observations, visible to the journal. */
    FString GetWitnessEchoJournal() const;
    /** Return only firsthand evidence actually inspected, with attributed theories. */
    UFUNCTION(BlueprintPure,Category="Unmade|Investigation")
    FString GetEvidenceNotebook() const;
    bool TryWitnessDispatch(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    /** Only Hessa can read the physically delivered document from the world state. */
    FString GetWitnessDispatchRecipientLine(FName NpcId) const;
    bool TryWitnessReturn(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool ResolveNearbyWitnessReturn(int32 Choice);
    FString GetWitnessReturnLine(FName NpcId) const;
    /** Optional three-city public response near Orrel's physical bridge nail. */
    bool ResolveNearbyWitnessBraid(int32 Choice);
    void RefreshDistrictMood();
    /** Rebuild public village state from already-saved choices; no extra save flags. */
    void RefreshCommunityConsequences();
    UnmadeCore::CommunityConsequence GetCommunityOutcome(UnmadeCore::Community Id) const;
    FString DescribeCommunityAt(FVector Position) const;
    /** True only when a real interaction advanced a quest; completion flag is output. */
    bool TryResidentVillageTask(FName ResidentId, bool& bCompleted);
    void TryFactionConversation(FName ResidentId);
    bool ResolveNearbyFaction(UnmadeCore::FactionEnding Outcome);
    bool GetNearbyCommitPreview(int32 Choice,FName& Scope,FString& Warning) const;
    bool TryTravelFrontier(AUnmadeCharacter* Player);
    bool TryTravelAtlas(AUnmadeCharacter* Player);
    bool InspectFrontierClue(AUnmadeCharacter* Player);
    void TryFrontierConversation(FName ResidentId);
    bool ResolveNearbyFrontier(int32 Ending);
    bool TryAfterlightConversation(FName ResidentId);
    bool InspectAfterlightClue(AUnmadeCharacter* Player);
    bool ResolveNearbyAfterlight(int32 Choice);
    /** Returns true when a closer physical aftershock control/clue handled E. */
    bool InspectRealmAftermathSite(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool TryRealmAftermathConversation(FName ResidentId);
    bool ResolveNearbyRealmAftermath(int32 Choice);
    bool InspectLaterRealmSite(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool TryLaterRealmConversation(FName ResidentId);
    bool InspectEchoQuestSite(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool TryEchoQuestConversation(FName ResidentId);
    int32 GetEchoQuestStage(UnmadeCore::Realm Realm) const {return EchoQuest.Stage(Realm);}
    int32 GetEchoQuestEnding(UnmadeCore::Realm Realm) const {return EchoQuest.Ending(Realm);}
    int32 GetEchoQuestEvidence(UnmadeCore::Realm Realm) const {return EchoQuest.Evidence(Realm);}
    int32 GetRealmResonanceStage(UnmadeCore::Realm Realm) const {return Resonance.Stage(Realm);}
    bool RecordNearbyRealmRite(AUnmadeCharacter* Player,UnmadeCore::RiteId Id);
    bool InspectResonanceArchive(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    UnmadeCore::GuardianOutcome GetRealmGuardianOutcome(UnmadeCore::Realm Realm) const {
        return Guardians.Outcome(Realm);
    }
    bool ResolveRealmGuardian(UnmadeCore::Realm Realm,bool bMercy);
    bool TryCalmNearbyGuardian(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    UnmadeCore::FinalAct GetFinalAct() const {return FinalStory.Act();}
    UnmadeCore::Morning GetNewMorning() const {return FinalStory.World();}
    int32 GetFinalMemorySeed() const {return FinalStory.MemorySeed();}
    bool BreakFinalMask();
    bool BreakFinalCore();
    bool SettleFinalEcho();
    bool TryFinalInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    UnmadeCore::RoadEvidence GetUnansweredRoadEvidence() const;
    UnmadeCore::RoadGuidance GetUnansweredRoadGuidance() const;
    FString DescribeUnansweredRoad(AUnmadeCharacter* Player) const;
    bool InspectUnansweredRoad(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool RecordResidentConversation(FName ResidentId);
    bool RecordResidentAid(FName ResidentId);
    FString GetResidentReturnLine(FName ResidentId,UnmadeCore::Realm Home) const;
    bool CommitFinalMorning(AUnmadeCharacter* Player,UnmadeCore::Morning Choice);
    bool WakeFinalEcho();
    int32 GetLaterRealmStage(UnmadeCore::Realm Id) const {return LaterRealm.Stage(Id);}
    int32 GetLaterRealmOutcome(UnmadeCore::Realm Id) const {return LaterRealm.Outcome(Id);}
    int32 GetLaterRealmVisitMask() const {return LaterRealm.Snapshot().visits;}
    int32 GetRealmAftermathStage(UnmadeCore::Realm Id) const { return RealmAftermath.Stage(Id); }
    int32 GetRealmAftermathEnding(UnmadeCore::Realm Id) const { return RealmAftermath.Ending(Id); }
    /** Clock-synchronized local hazard, no update or damage outside active route. */
    UnmadeCore::FrontierHazardSample GetNearbyFrontierHazard(FVector Position) const;
    UnmadeCore::LaterHazardSample GetNearbyLaterRealmHazard(FVector Position) const;
    int32 GetAfterlightStage() const { return Afterlight.Stage(); }
    UnmadeCore::AfterlightBenefit GetAfterlightBenefit() const { return Afterlight.Effect(); }
    FString GetCurrentRealmName(FVector Position) const;
    int32 GetFrontierVisitMask() const { return Frontier.Snapshot().visits; }
    int32 FactionStage(UnmadeCore::Faction Id) const { return Chronicle.Stage(Id); }
    UnmadeCore::FactionEnding FactionOutcome(UnmadeCore::Faction Id) const {
        return Chronicle.Ending(Id);
    }
    UnmadeCore::TaskProgress GetRegionalTask(UnmadeCore::SettlementId Village) const {
        return RegionalTasks.Progress(Village);
    }

    /** Created at runtime so the initial experiment needs no hand-authored .umap. */
    void BuildForPrototype();

private:
    bool bBuilt = false;
    FTimerHandle GossipTimer;
    FTimerHandle WorldSaveTimer;
    UPROPERTY(Transient)
    TObjectPtr<ADirectionalLight> Sunlight;
    UnmadeCore::LivingWorldClock Clock;
    UnmadeCore::RealityConsequenceLedger RealityLedger; // v1: Reach, do not migrate
    UnmadeCore::OuterRealmReactions OuterReactions; // eight isolated world histories
    UnmadeCore::MaterialWorld Materials;
    UnmadeCore::LivingEconomy LivingTrades;
    UnmadeCore::BesHousehold BesHousehold;
    bool bBesHouseholdRejected=false;
    void RefreshBesHouseholdWorld();
    bool bLivingEconomyRejected=false;
    bool bMaterialSaveRejected=false;
    void RefreshMaterialWorld();
    void AdvanceMaterialDayIfNeeded();
    bool bOuterReactionsRejected=false;
    void RefreshOuterReactionWorld();
    bool bOuterReactionVisualActive=false;
    uint8 OuterActiveMask=0;
    bool bRealityConsequenceRejected=false;
    void RefreshRealityConsequenceWorld();
    bool bRealityVisualActive=false;
    UnmadeCore::DiscoveryLedger Discoveries;
    UnmadeCore::SettlementVisits VillagesVisited;
    UnmadeCore::RegionalTaskModel RegionalTasks;
    UnmadeCore::ResidentContinuity ResidentRelationships;
    UnmadeCore::WitnessEchoLedger WitnessEchoes;
    UnmadeCore::WitnessBraidChronicle WitnessBraid;
    UnmadeCore::WitnessDispatch Dispatch;
    UnmadeCore::WitnessReturn ReturnWitness;
    bool bWitnessReturnRejected=false;
    void RefreshWitnessReturnWorld();
    bool bWitnessDispatchRejected=false;
    void RefreshWitnessDispatchWorld();
    bool bWitnessEchoRejected=false;
    bool bWitnessBraidRejected=false;
    void RefreshWitnessBraidWorld();
    bool bResidentContinuityRejected=false;
    UnmadeCore::FactionChronicle Chronicle;
    UnmadeCore::FrontierJourney Frontier;
    UnmadeCore::BellwoldAfterlight Afterlight;
    bool bAfterlightSaveRejected=false;
    UnmadeCore::RealmAftermathChronicle RealmAftermath;
    bool bRealmAftermathSaveRejected=false;
    UnmadeCore::LaterRealmJourney LaterRealm;
    bool bLaterRealmSaveRejected=false;
    void BuildLaterRealms();
    void BuildAtlasGateways();
    void RefreshLaterRealmWorld();
    UnmadeCore::EchoChronicle EchoQuest;
    UnmadeCore::CommitmentGate EchoCommitGate;
    bool bEchoSaveRejected=false;
    void BuildEchoQuests();
    void RefreshEchoQuestWorld();
    UnmadeCore::RealmResonanceJourney Resonance;
    bool bResonanceSaveRejected=false;
    void BuildRealmResonance();
    void RefreshRealmResonanceWorld();
    UnmadeCore::GuardianChronicle Guardians;
    UnmadeCore::CommitmentGate GuardianCommitGate;
    bool bGuardianSaveRejected=false;
    void BuildRealmGuardians();
    void RefreshRealmGuardians();
    UnmadeCore::FinalJourney FinalStory;
    UnmadeCore::CommitmentGate FinalCommitGate;
    bool bFinalSaveRejected=false;
    void BuildUnansweredRoad();
    void BuildFinalWorld();
    void RefreshFinalWorld();
    bool bLaterHazardCueVisible[6]={false,false,false,false,false,false};
    void RefreshLaterHazardCues();
    void BuildFrontierAftermath();
    void RefreshRealmAftermathWorld();
    bool bHazardCueVisible[2]={false,false};
    void RefreshFrontierHazardCues();
    void BuildBellwoldAfterlight();
    void RefreshAfterlightWorld();
    std::array<UnmadeCore::CommunityConsequence,5> CommunityEffects{};
    void BuildCommunityConsequences();
    UnmadeCore::SettlementId LastVisitedVillage = UnmadeCore::SettlementId::None;
    TMap<FName,double> TemporaryRiteWorldEffects;
    void SetRiteWorldActorState(FName Tag,bool bEnabled);
    void RefreshRiteWorldFromSave();
    FName LastAmbientSite = NAME_None;
    UnmadeCore::DayPhase LastAmbientPhase = UnmadeCore::DayPhase::Day;
    void RestoreLivingWorld();
    void SaveLivingWorld();
    bool WriteWorldSnapshot();
    int32 ReadStoryChoice() const;
    void SpawnLoreSite(UnmadeCore::District District, FName SiteId,
        const TCHAR* Name, const TCHAR* DayLine, const TCHAR* NightLine, FVector Position);
    void SpreadLocalRumors();
    void SaveCitizens();
    void SpawnBlock(FVector Center, FVector Scale, FName Label);
    void SpawnCitizen(const UnmadeCore::ResidentSpec& Resident);
    void BuildVillages();
    void BuildFrontiers();
    void RestoreCitizens();
};
