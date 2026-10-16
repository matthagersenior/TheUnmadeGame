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
#include "World/UnmadeRememberedRoadRules.h"
#include "World/UnmadeRegionalTaskRules.h"
#include "World/UnmadeFactionChronicleRules.h"
#include "World/UnmadeFrontierRealmRules.h"
#include "World/UnmadeTenfoldChronicle.h"
#include "World/UnmadeRealityConsequenceRules.h"
#include "World/UnmadeRealmReactionRules.h"
#include "World/UnmadeMaterialConsequencesRules.h"
#include "World/UnmadeLivingEconomyRules.h"
#include "World/UnmadeBesHouseholdRules.h"
#include "World/UnmadeCinderholdAdventureRules.h"
#include "World/UnmadeUnpaidShiftSceneRules.h"
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
#include "World/UnmadeExpeditionRegistry.h"
#include "World/UnmadeRealmJourneyRules.h"
#include "World/UnmadeExpeditionRules.h"
#include "World/UnmadeIdentityRules.h"
#include "World/UnmadeAdventureDepthRules.h"
#include "Items/UnmadeItemRules.h"
#include "Combat/UnmadeCombatStyleRules.h"
#include "World/UnmadePlayerExperienceRules.h"
#include "UnmadePrototypeHub.generated.h"

class AUnmadeLoreSite;
class ADirectionalLight;
class AUnmadeCharacter;
class AStaticMeshActor;
class ACameraActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(
    FUnmadeBesDirectedBeat, FName, SpeakerId,
    const FString&, Subtitle, FName, CameraCue,
    FName, FoleyCue, const FString&, Blocking,
    const FString&, VoiceDirection, float, DurationSeconds);

UCLASS()
class THEUNMADEGAME_API AUnmadePrototypeHub : public AActor
{
    GENERATED_BODY()

public:
    bool TryFieldCraft(AUnmadeCharacter* Player,int Realm);
    bool KnowsFieldRecipe(int Realm)const{return Experience.Arc(Realm)&&Experience.Arc(Realm)->complete;}
    void PrepareRealmGeometry(const FVector& At,int Limit=48);
    const TArray<TWeakObjectPtr<class AUnmadeEnemyCharacter>>& CachedEnemies()const{return RuntimeEnemies;}
    FName ServiceWorkCue(FName Person,const FVector& At)const;
    bool HasRejectedWorld()const{return !bFieldSurveySaveUsable||bExpeditionSaveRejected||bIdentitySaveRejected||bDepthSaveRejected||bExperienceRejected||bRoadSaveRejected||bCinderholdSaveRejected||bAfterlightSaveRejected||bRealmAftermathSaveRejected||bLaterRealmSaveRejected||bEchoSaveRejected||bResonanceSaveRejected||bGuardianSaveRejected||bFinalSaveRejected||bResidentContinuityRejected||bWitnessEchoRejected||bWitnessBraidRejected||bWitnessDispatchRejected||bWitnessReturnRejected||bRealityConsequenceRejected||bOuterReactionsRejected||bMaterialSaveRejected||bLivingEconomyRejected||bBesHouseholdRejected;}
    bool IsWorldReady()const{return bWorldReady;}
    bool SavePlayerSession(AUnmadeCharacter* Player){return WriteWorldSnapshot(Player);}
    AUnmadePrototypeHub();
    bool TryExperienceInteraction(AUnmadeCharacter* Player,double Competing);
    bool RecordExperienceConversation(FName Id,AUnmadeCharacter* Player);
    bool ExperienceRoutineTarget(FName Id,const FVector& Current,FVector& Target)const;
    FString ExperienceReaction(FName Id)const;
    FString GetExperienceJournal()const;
    FString GetExperienceMap(int Realm,AUnmadeCharacter* Player)const;
    FString GetTrackedObjective()const;
    bool IsRealmKnown(int Realm)const;
    bool TrackExperienceRealm(int Realm,AUnmadeCharacter* Player);
    bool SelectExperienceBuild(int Realm,AUnmadeCharacter* Player);
    bool ExperienceAction(int Action,AUnmadeCharacter* Player);
    bool ExperienceServicesOpen(int Realm)const;
    FString ExperienceServiceHours(int Realm)const;
    bool WingObserverInactive()const{return bExperienceRejected||!Experience.Arc(6)->offered||Experience.ObserverCleared();}
    bool RecordWingObserver(class AUnmadeEnemyCharacter* Observer);
    bool WingGuardianInactive(int Realm)const;
    bool WingGuardianYielded(int Realm)const;
    bool RecordWingGuardian(class AUnmadeEnemyCharacter* Guardian,int Realm);
    UnmadeCore::BuildEffect ActiveBuild()const;
    bool ExperienceCheckpoint(const FVector& Current,FVector& Destination)const;
    void LearnExperienceTutorial(int Bit,AUnmadeCharacter* Player);

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
    bool TryRoadInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool TryRoadAbility(AUnmadeCharacter* Player,int32 Ability,int32 Choice=0);
    bool RecordRoadConversation(FName StableId);
    FString GetRoadResidentLine(FName StableId) const;
    FString GetRoadJournal() const;
    bool RoadRoutineTarget(FName StableId,bool bNight,FVector& Target) const;
    int32 GetRoadElderResolution(int32 City) const;
    bool IsRoadCooling(int32 City) const;
    bool RecordRoadElderDefeat(class AUnmadeEnemyCharacter* Elder,int32 City);
    void PlayRoadScene(int32 City,bool bReturn);
    bool TryCinderholdInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool TryCinderholdAbility(AUnmadeCharacter* Player,int32 Ability,int32 Choice=0);
    bool RecordCinderholdConversation(FName StableId);
    FString GetCinderholdResidentLine(FName StableId) const;
    FString GetCinderholdJournal() const;
    bool RecordCinderholdElderDefeat(class AUnmadeEnemyCharacter* Elder);
    bool IsCinderholdElderResolved() const { return !bCinderholdSaveRejected&&Cinderhold.Resolved(); }
    int32 GetCinderholdElderResolution() const { return bCinderholdSaveRejected?0:Cinderhold.Save().resolution; }
    bool IsCinderholdVentActive() const;
    void PlayCinderholdScene(int32 Scene);
    bool TryFieldSurveyInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool TryMaterialInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool TryLivingTradeInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    FString GetLivingEconomyResidentLine(FName StableId) const;
    FString GetBesHouseholdResidentLine(FName StableId) const;
    bool RecordBesHouseholdConversation(FName StableId);
    void MaybePlayBesReturnScene(FName StableId);
    bool TryBesHouseholdInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    /** Authored timed subtitle/shot/foley events; binds to UMG and Sequencer.
        Without production sound assets the captions still work offline. */
    UPROPERTY(BlueprintAssignable,Category="Unmade|Unpaid Shift")
    FUnmadeBesDirectedBeat OnBesDirectedBeat;
    UFUNCTION(BlueprintCallable,Category="Unmade|Unpaid Shift")
    void PlayBesDirectedScene(int32 SceneIndex);
    UFUNCTION(BlueprintCallable,Category="Unmade|Unpaid Shift")
    void SkipBesDirectedScene();
    bool IsBesDirectedScenePlaying() const { return BesActiveScene>=0; }
    UFUNCTION(BlueprintPure,Category="Unmade|Unpaid Shift")
    FString GetBesDecisionOptions() const;
    UFUNCTION(BlueprintPure,Category="Unmade|Unpaid Shift")
    FString GetBesDecisionPreviewForStation(FName StationTag) const;
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
    void BuildPlayableEncounters();
    bool bFieldSurveySaveUsable=true;
    TArray<int32> FieldSurveyStages;
    TArray<TWeakObjectPtr<AStaticMeshActor>> FieldSurveyGates;
    TArray<TWeakObjectPtr<class ATextRenderActor>> FieldSurveySigns;

    bool TryExpeditionInteraction(AUnmadeCharacter* Player,double CompetingNpcDistanceSq);
    bool TryExpeditionAbility(AUnmadeCharacter* Player,int32 Kind);
    bool RecordExpeditionDefeat(class AUnmadeEnemyCharacter* Enemy,int32 Arc,int32 Slot);
    bool ExpeditionCaptiveWaiting(int Arc,int Captive)const{auto* T=Expeditions.Arc(Arc);return !T||Captive<0||Captive>1||!T->returned||!(T->rescued&(1<<Captive));}
    bool ExpeditionEnemyInactive(int32 Arc,int32 Slot)const;
    FString GetExpeditionJournal()const;
    FString GetExpeditionReaction(FName Person)const;
    UnmadeCore::ExpeditionBenefit ExpeditionBenefit()const{return bExpeditionSaveRejected?UnmadeCore::ExpeditionBenefit{}:Expeditions.Benefit();}
    bool ExpeditionBossYielded(int a)const{auto* t=Expeditions.Arc(a);return !bExpeditionSaveRejected&&t&&t->boss==2;}
    void BuildAdventureDepth();
    void RefreshAdventureDepth();
    bool TryDepthInteraction(AUnmadeCharacter* Player,int Action,double Competing=1000000.);
    UnmadeCore::FieldKitResult UseDepthFieldKit(AUnmadeCharacter* Player);
    bool UseDepthPower(AUnmadeCharacter* Player);
    bool SelectDepthTechnique(int Arc,AUnmadeCharacter* Player);
    FString GetDepthJournal()const;
    FString GetDepthReaction(FName Person)const;
    FString GetDepthPrompt(AUnmadeCharacter* Player)const;
    UnmadeCore::WeaponStyle PlayerWeaponStyle(UnmadeCore::ItemId Legacy)const;
    int GetActiveMastery()const{auto* S=Depth.Arc(Depth.Active());return bDepthSaveRejected||!S?0:S->training;}
    TArray<FString> RpgHistory;
    FString RpgSceneNotice;double RpgSceneUntil=0;
    FString RpgNotice;
    uint64 RpgNoticeSerial=0;
    void NotifyRpg(const FString& Text){RpgNotice=Text;++RpgNoticeSerial;if(RpgHistory.IsEmpty()||RpgHistory.Last()!=Text){RpgHistory.Add(Text);if(RpgHistory.Num()>100)RpgHistory.RemoveAt(0);}}
    int DepthPendingArc=-1,DepthPendingChoice=0;
    double DepthConfirmUntil=0;
    bool bDepthSaveRejected=false;
    UnmadeCore::AdventureDepth Depth;
    std::array<UnmadeCore::RealmChallenge,9> DepthChallenges;
    TArray<TWeakObjectPtr<class AUnmadeEnemyCharacter>> DepthRetaliators;
    std::array<double,9> NextDepthThreatSave{};
    TArray<TWeakObjectPtr<class AStaticMeshActor>> DepthEvidenceActors,DepthCacheActors;
    TArray<TWeakObjectPtr<class AStaticMeshActor>> DepthSecretGates;
    TArray<TWeakObjectPtr<class ATextRenderActor>> DepthChallengeSigns;
    void BuildIdentity();
    void RefreshIdentity();
    bool TryIdentityInteraction(AUnmadeCharacter* Player,double Competing,bool Survey=false);
    bool bIdentitySaveRejected=false;
    UnmadeCore::WorldIdentity Identity;
    TArray<TWeakObjectPtr<class AStaticMeshActor>> IdentityRepairs;
    TArray<TWeakObjectPtr<class ATextRenderActor>> IdentityPuzzleSigns;
    TArray<TWeakObjectPtr<class AUnmadeNpcCharacter>> IdentityPatrols;
    std::array<double,9> IdentityNextService{};
    void BuildExpeditions();
    void RefreshExpeditions();
    bool CommitExpeditions(const UnmadeCore::Expeditions& Before,AUnmadeCharacter* Player=nullptr);
private:
    struct DeferredBlock {FVector Center,Scale;FName Label;};
    TArray<DeferredBlock> DeferredBlocks;
    TArray<TWeakObjectPtr<class AUnmadeEnemyCharacter>> RuntimeEnemies;
    TArray<TWeakObjectPtr<class AUnmadeNpcCharacter>> RuntimePeople;
    TArray<TWeakObjectPtr<class ATextRenderActor>> RuntimeSigns;
    TMap<TWeakObjectPtr<class ATextRenderActor>,FString> SignOriginal,SignFormatted;
    FVector GeometryFocus=FVector::ZeroVector;
    double NextRuntimeCache=0,NextSimulationBudget=0;
    bool SpawningDeferred=false;
    void RefreshSimulationBudget();
    bool bWorldReady=false;
    UnmadeCore::ExpeditionConfirmation ExpeditionPendingDecision;
    bool ConfirmExpeditionDecision(int Arc,int Kind,int Value);
    UnmadeCore::PlayerExperience Experience;
    bool bExperienceRejected=false;
    double NextExperienceRefresh=0,ExperienceWaitConfirm=-1;
    std::array<double,9> WingRelayDeadline{};
    mutable TArray<FVector> EscortTrail;
    TWeakObjectPtr<class AUnmadeEnemyCharacter> WingObserver;
    TArray<TWeakObjectPtr<class AUnmadeEnemyCharacter>> WingGuardians;
    TArray<TWeakObjectPtr<class AStaticMeshActor>> WingExitGates,SettlementRoomGates,SettlementRepairs,WingLift,WingRootGates;
    TArray<TWeakObjectPtr<class ATextRenderActor>> ExperienceSigns;
    void BuildPlayerExperience();
    void RefreshPlayerExperience();
    bool CommitExperience(const UnmadeCore::PlayerExperience& Before,AUnmadeCharacter* Player);
    bool ExperienceScene(int Realm,int Scene,AUnmadeCharacter* Player);
    bool bBuilt = false;
    bool bExpeditionSaveRejected=false;
    UnmadeCore::Expeditions Expeditions;
    TArray<TWeakObjectPtr<class AStaticMeshActor>> ExpeditionReturnGates,ExpeditionBossGates;
    TArray<TWeakObjectPtr<class ATextRenderActor>> ExpeditionSigns;
    double NextExpeditionRefresh=0;
    std::array<double,9> ExpeditionReleaseUntil{};
    TArray<TWeakObjectPtr<class ATextRenderActor>> ExpeditionHazardSigns;
    std::array<double,9> ExpeditionHazardQuiet{};
    std::array<double,9> ExpeditionReleaseReady{};

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
    UnmadeCore::CinderholdAdventure Cinderhold;
    bool bCinderholdSaveRejected=false;
    int32 CinderholdMaintenanceTurns=0;
    double CinderholdNextMaintenanceAt=0;
    int32 CinderholdPendingRoute=0;
    double CinderholdConfirmUntil=-1;
    double NextCinderholdRefreshAt=0;
    void BuildCinderholdAdventure();
    void RefreshCinderholdAdventure();
    bool CommitCinderhold(const UnmadeCore::CinderholdAdventure& Before,AUnmadeCharacter* StrainOwner=nullptr);
    TWeakObjectPtr<AStaticMeshActor> CinderholdVentWheel;
    TWeakObjectPtr<AStaticMeshActor> CinderholdKilnGate;
    TWeakObjectPtr<AStaticMeshActor> CinderholdArchiveGate;
    TWeakObjectPtr<class ATextRenderActor> CinderholdVentSign;
    TWeakObjectPtr<class ATextRenderActor> CinderholdHomeSign;
    double LastBesReturningSceneAt=-9999.0;
    FName BesPendingDecision;
    double BesDecisionExpiresAt=0.0;
    // Client-side caption overlay, no required Blueprint/audio content.
    UPROPERTY(Transient)
    TObjectPtr<class UUnmadeUnpaidShiftSubtitleWidget> BesSubtitleOverlay;
    FTimerHandle BesSubtitleTimer;
    int32 BesActiveScene=-1;
    int32 BesActiveBeat=0;
    void AdvanceBesDirectedBeat();
    void FrameBesDirectedBeat(FName Speaker,const FString& Cue,const FString& Blocking,float Seconds);
    void RestoreBesCamera();
    UPROPERTY(Transient)
    TObjectPtr<ACameraActor> BesShotCamera;
    TWeakObjectPtr<AActor> BesOriginalViewTarget;
    TWeakObjectPtr<class AUnmadeNpcCharacter> BesStagedSpeaker;
    TWeakObjectPtr<AStaticMeshActor> BesWorkingCart;
    TWeakObjectPtr<AStaticMeshActor> BesVisibleBasket;
    FVector BesCartRest=FVector::ZeroVector;
    void AnimateBesWorkingCart();
    void UpdateBesQuestTracker();
    double NextBesTrackerRefreshAt=0.0;
    UPROPERTY(Transient)
    TObjectPtr<class UUnmadeBesQuestTrackerWidget> BesQuestTracker;
    FString BesDecisionPreview(FName Tag) const;
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
    UnmadeCore::RememberedRoads RememberedRoads;
    bool bRoadSaveRejected=false;
    double NextRoadRefreshAt=0;
    int32 PendingRoadCity=-1,PendingRoadChoice=0;
    double PendingRoadExpiresAt=0;
    TArray<TWeakObjectPtr<class AStaticMeshActor>> RoadLaneGates,RoadRecessGates;
    TArray<TWeakObjectPtr<class ATextRenderActor>> RoadSigns;
    TWeakObjectPtr<class AStaticMeshActor> RoadCargoVisual;
    void BuildRememberedRoads();
    void RefreshRememberedRoads();
    bool CommitRoads(const UnmadeCore::RememberedRoads& Before,AUnmadeCharacter* StrainOwner=nullptr);

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
    bool WriteWorldSnapshot(AUnmadeCharacter* StrainOwner=nullptr);
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
