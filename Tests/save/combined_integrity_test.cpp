#include "Save/UnmadeCombinedSaveRules.h"
#include "Save/UnmadeRecoveryRules.h"
#include "combined_record_fixture.h"
#include "combined_record_store.h"
#include <cassert>
#include <functional>
#include <limits>
#include <optional>
using namespace UnmadeCore;
using SaveFixture::Record;
using SaveFixture::Storage;
using SaveFixture::Valid;
static Record Earned(){
    Record r;r.bHasLivingWorldSnapshot=true;r.LivingWorldSeconds=120;
    r.BellwoldTaskStage=2;r.bHasPlayerResume=true;r.ResumeHealth=37;r.ResumeStamina=19;
    InventoryModel inventory;assert(inventory.Claim(Achievement::Starter)==RewardResult::Awarded);
    const auto state=inventory.Snapshot();r.bHasInventorySnapshot=true;
    r.ItemQuantities.assign(state.quantities.begin(),state.quantities.end());
    r.EquippedItems.assign(state.equipped.begin(),state.equipped.end());
    r.AwardedMilestoneBits=static_cast<std::int64_t>(state.milestones);
    r.bHasEconomySnapshot=true;r.ProfessionSkills={0,0,0};
    r.bHasFieldSurveys=true;r.FieldSurveyStages={0,1,2,0,1,2};
    r.LexiconEvidence={"evidence.glimpse"};
    assert(Valid(r));return r;
}
static Record Expanded(){
    auto r=Earned();
    r.bHasFractureSnapshot=true;r.WorldVariantId="variant.open";r.PlayerStrain=24;
    r.bHasConflictSnapshot=true;r.LocalConflictChoice=1;r.SupplyActivityStage=2;
    r.bHasCharacterIdentity=true;r.CharacterChosenName="The Unmade";r.CharacterIdentityChoices.assign(8,0);
    r.bHasFactionChronicle=true;r.FactionStages.assign(3,0);r.FactionEndings.assign(3,0);
    r.bHasFrontierSnapshot=true;r.FrontierStages.assign(2,0);r.FrontierEndings.assign(2,0);
    r.bHasAfterlightSnapshot=true;
    r.bHasRealmAftermathSnapshot=true;r.RealmAftermathStages.assign(9,0);
    r.RealmAftermathPrepared.assign(9,0);r.RealmAftermathApproaches.assign(9,0);r.RealmAftermathEndings.assign(9,0);
    r.bHasLaterRealmSnapshot=true;r.LaterRealmStages.assign(6,0);r.LaterRealmChoices.assign(6,0);
    r.bHasEchoQuestSnapshot=true;r.EchoQuestStages.assign(6,0);r.EchoQuestEvidence.assign(6,0);
    r.EchoQuestTestimonies.assign(6,0);r.EchoQuestEndings.assign(6,0);
    r.bHasRealmResonanceSnapshot=true;r.RealmResonanceStages.assign(6,0);
    r.RealmResonanceFirstCasts.assign(6,0);r.RealmResonanceDeadlines.assign(6,0);
    r.bHasGuardianSnapshot=true;r.RealmGuardianOutcomes.assign(6,0);
    r.bHasFinalEncounterSnapshot=true;
    r.bHasResidentContinuitySnapshot=true;r.ResidentEncounterVisits.assign(ResidentContinuityCount,0);
    r.ResidentEncounterLastDays.assign(ResidentContinuityCount,0);r.ResidentEncounterAidFlags.assign(ResidentContinuityCount,0);
    r.bHasWitnessEchoSnapshot=true;r.bHasWitnessBraidSnapshot=true;
    r.bHasWitnessDispatchSnapshot=true;r.bHasWitnessReturnSnapshot=true;
    r.bHasRealityConsequenceSnapshot=true;r.bHasOuterRealmReactions=true;
    r.OuterCareMasks.assign(8,0);r.OuterInquiryMasks.assign(8,0);
    r.OuterActiveRites.assign(8,-1);r.OuterActiveUntil.assign(8,0);
    r.bHasMaterialWorld=true;MaterialWorld materials;
    for(const auto& t:materials.Snapshot()){
        r.MaterialDays.push_back(t.day);r.MaterialStores.push_back(t.store);
        r.MaterialHouseholds.push_back(t.households);r.MaterialIntegrity.push_back(t.integrity);
        r.MaterialProduced.push_back(t.produced);r.MaterialConsumed.push_back(t.consumed);
        r.MaterialShortageDays.push_back(t.shortageDays);r.MaterialLaborHours.push_back(t.laborHours);
        r.MaterialDeliveries.push_back(t.deliveries);r.MaterialRepairs.push_back(t.repairs);
    }
    r.bHasLivingEconomy=true;LivingEconomy economy;
    for(const auto& w:economy.Save().workers){
        r.LivingEmployerMarks.push_back(w.employerMarks);r.LivingEarnedMarks.push_back(w.earnedMarks);
        r.LivingOwedMarks.push_back(w.owedMarks);r.LivingPaidShifts.push_back(w.shiftsPaid);
        r.LivingPaidDays.push_back(w.lastPaidDay);r.LivingWorkerReceipts.push_back(w.deliveriesWitnessed);
    }
    r.LivingForeignStock.assign(2,0);r.LivingRouteReceipts.assign(2,0);
    r.LivingTradePayments.assign(2,0);r.LivingConsumedSupport.assign(2,0);
    r.bHasBesHousehold=true;r.bHasCinderholdAdventure=true;
    CinderholdAdventure cinder;const auto c=cinder.Save();
    r.CinderholdAdventureState={c.version,c.memories,c.clues,c.resolution,c.route,
        c.firstDay,c.returnDay,c.returnedHeat,c.returnedColdDays,c.returnedOwed};
    r.bHasExpeditions=true;r.ExpeditionState=Expeditions{}.Pack();
    r.bHasWorldIdentity=true;r.WorldIdentityState=WorldIdentity{}.Pack();
    r.bHasAdventureDepth=true;r.AdventureDepthState=AdventureDepth{}.Pack();
    r.bHasPlayerExperience=true;r.PlayerExperienceState=PlayerExperience{}.Pack();
    r.bHasRememberedRoads=true;r.RememberedRoadState=RememberedRoads{}.Pack();
    r.bHasUnifiedLoadout=true;r.UnifiedRealmLoadout={-1,-1,-1};
    r.bHasTenfoldChronicle=true;r.RiteStages.assign(10,0);r.RiteChoices.assign(10,0);
    r.RiteReadyAt.assign(10,0);r.DisciplineMastery.assign(6,0);
    r.bHasConfluenceSnapshot=true;r.ConfluenceStages.assign(6,0);r.ConfluenceDecisions.assign(6,0);
    r.ConfluenceCastMasks.assign(6,0);r.ConfluenceWindowEnds.assign(6,0);
    assert(Valid(r));return r;
}
static InventoryModel RestoreItems(const Record& record){
    InventorySnapshot state;
    for(int i=0;i<record.ItemQuantities.Num();++i)state.quantities[i]=record.ItemQuantities[i];
    for(int i=0;i<3;++i)state.equipped[i]=record.EquippedItems[i];
    state.milestones=static_cast<std::uint64_t>(record.AwardedMilestoneBits);
    InventoryModel model;assert(model.Restore(state));return model;
}
static void CaptureItems(Record& record,const InventoryModel& model){
    const auto state=model.Snapshot();
    record.ItemQuantities.assign(state.quantities.begin(),state.quantities.end());
    record.EquippedItems.assign(state.equipped.begin(),state.equipped.end());
    record.AwardedMilestoneBits=static_cast<std::int64_t>(state.milestones);
}

int main(){
    Record legacy;assert(Valid(legacy));
    // Optional petition is entirely absent from older schema-1 profiles.
    auto unchosen=Earned();
    unchosen.bHasHearthPetition=true;
    unchosen.HearthPetitionState={0,0,0,0,0,0};
    assert(Valid(unchosen));
    auto incomplete=unchosen;
    incomplete.HearthPetitionState={3,1,1,1,1};
    assert(!Valid(incomplete)); // missing serialized field
    incomplete=unchosen;
    incomplete.HearthPetitionState={3,1,1,1,1,0};
    assert(!Valid(incomplete)); // no Bes/home/warden verified
    auto orphan=legacy;
    orphan.bHasHearthPetition=true;
    orphan.HearthPetitionState={0,0,0,0,0,0};
    assert(!Valid(orphan)); // no parent living world

    // Original inventory length remains readable; appended quantities default to zero.
    auto old=Earned();old.ItemQuantities.resize(28);assert(Valid(old));
    const auto good=Earned();
    const auto complete=Expanded();
    auto malformed=complete;malformed.EchoQuestEndings[5]=999;assert(!Valid(malformed));
    malformed=complete;malformed.OuterActiveUntil[7]=std::numeric_limits<double>::quiet_NaN();assert(!Valid(malformed));
    malformed=complete;malformed.MaterialStores[8]=-1;assert(!Valid(malformed));
    malformed=complete;malformed.LivingOwedMarks[8]=-1;assert(!Valid(malformed));
    malformed=complete;malformed.PlayerExperienceState.back()=999;assert(!Valid(malformed));
    malformed=complete;malformed.RiteTrialBits=65536;assert(!Valid(malformed));
    malformed=complete;malformed.bHasTenfoldChronicle=false;assert(!Valid(malformed));
    malformed=complete;malformed.bHasLivingWorldSnapshot=false;assert(!Valid(malformed));
    assert(ValidSurveyRecord(1,good.FieldSurveyStages));
    assert(!ValidSurveyRecord(2,good.FieldSurveyStages));
    SaveFixture::Array<int> shortSurvey{0,1};assert(!ValidSurveyRecord(1,shortSurvey));
    SaveFixture::Array<int> badSurvey{0,0,0,0,0,3};assert(!ValidSurveyRecord(1,badSurvey));
    const std::function<void(Record&)> faults[]={
        [](auto& r){r.SchemaVersion=2;},
        [](auto& r){r.ResumeHealth=std::numeric_limits<float>::quiet_NaN();},
        [](auto& r){r.ItemQuantities[0]=-1;},
        [](auto& r){r.EquippedItems={999,0,0};},
        [](auto& r){r.ProfessionSkills={0,-1,0};},
        [](auto& r){r.LexiconEvidence={"unknown.evidence"};},
        [](auto& r){r.bHasFractureSnapshot=true;r.PlayerStrain=std::numeric_limits<double>::infinity();},
        [](auto& r){r.bHasConflictSnapshot=true;r.LocalConflictChoice=3;},
        [](auto& r){r.bHasCharacterIdentity=true;r.CharacterIdentityChoices={0};},
        [](auto& r){r.LivingWorldSeconds=-1;},
        [](auto& r){r.BellwoldTaskStage=99;},
        [](auto& r){r.bHasFactionChronicle=true;},
        [](auto& r){r.bHasFrontierSnapshot=true;},
        [](auto& r){r.bHasAfterlightSnapshot=true;r.BellwoldAfterlightOutcome=9;},
        [](auto& r){r.bHasRealmAftermathSnapshot=true;},
        [](auto& r){r.bHasLaterRealmSnapshot=true;},
        [](auto& r){r.bHasEchoQuestSnapshot=true;},
        [](auto& r){r.bHasRealmResonanceSnapshot=true;},
        [](auto& r){r.bHasGuardianSnapshot=true;},
        [](auto& r){r.bHasFinalEncounterSnapshot=true;r.FinalMorningChoice=9;},
        [](auto& r){r.bHasResidentContinuitySnapshot=true;},
        [](auto& r){r.bHasWitnessEchoSnapshot=true;r.WitnessEchoReturnMask=1;},
        [](auto& r){r.bHasWitnessBraidSnapshot=true;r.WitnessBraidOutcome=1;},
        [](auto& r){r.bHasWitnessDispatchSnapshot=true;r.WitnessDispatchStage=2;},
        [](auto& r){r.bHasWitnessReturnSnapshot=true;r.WitnessReturnStage=4;},
        [](auto& r){r.bHasRealityConsequenceSnapshot=true;r.RealityCareWitnessMask=-1;},
        [](auto& r){r.bHasOuterRealmReactions=true;},
        [](auto& r){r.bHasMaterialWorld=true;},
        [](auto& r){r.bHasLivingEconomy=true;},
        [](auto& r){r.bHasBesHousehold=true;r.bBesMeraGuaranteeUsed=true;},
        [](auto& r){r.bHasCinderholdAdventure=true;},
        [](auto& r){r.bHasExpeditions=true;},
        [](auto& r){r.bHasWorldIdentity=true;},
        [](auto& r){r.bHasAdventureDepth=true;},
        [](auto& r){r.bHasPlayerExperience=true;},
        [](auto& r){r.bHasRememberedRoads=true;},
        [](auto& r){r.bHasUnifiedLoadout=true;r.UnifiedRealmLoadout={0,0,0};},
        [](auto& r){r.FieldSurveyStages[5]=3;},
        [](auto& r){r.bHasTenfoldChronicle=true;},
        [](auto& r){r.bHasConfluenceSnapshot=true;}
    };
    for(const auto& fault:faults){
        Record bad=good;fault(bad);assert(!Valid(bad));
        Storage disk;disk.primary=bad;disk.backup=good;
        const int originalStage=bad.BellwoldTaskStage;
        for(int reload=0;reload<4;++reload){
            const Record* loaded=disk.Load();
            if(bad.SchemaVersion==1){assert(loaded==&*disk.backup);assert(disk.recovery);}
            else assert(!loaded); // Future schemas are protected, even with recovery requested.
            assert(disk.writes==0&&disk.primary->BellwoldTaskStage==originalStage);
        }
        assert(!disk.Commit(bad)&&disk.writes==0);
        Storage absent;absent.primary=bad;assert(!absent.Load());assert(!absent.Commit(good));
        assert(absent.writes==0); // Cannot overwrite an unvalidated original without recovery.
    }
    // NPC provenance: no anonymous rumor, duplicate events/residents or forged kind.
    Record social=good;SaveFixture::Npc npc;npc.NpcId="npc.test";
    npc.Observations.push_back({{1},"Player.Helped","","",0});
    social.NpcSnapshots.push_back(npc);assert(Valid(social));
    social.NpcSnapshots[0].Observations[0].Evidence=2;assert(!Valid(social));
    social.NpcSnapshots[0].Observations[0].Evidence=1;assert(!Valid(social));
    social.NpcSnapshots[0].Observations[0].SpeakerId="npc.witness";assert(Valid(social));
    social.NpcSnapshots[0].Observations.push_back(social.NpcSnapshots[0].Observations[0]);assert(!Valid(social));
    social.NpcSnapshots[0].Observations.pop_back();social.NpcSnapshots.push_back(npc);assert(!Valid(social));
    // Late invalid optional data cannot leak an otherwise plausible reward into an owner.
    Record corrupt=good;corrupt.bHasWitnessReturnSnapshot=true;corrupt.WitnessReturnStage=4;
    Record earlier=good;earlier.BellwoldTaskStage=0;
    Storage recovery;recovery.primary=corrupt;recovery.backup=earlier;
    const auto* chosen=recovery.Load();assert(chosen&&chosen->BellwoldTaskStage==0);
    InventoryModel owner;InventorySnapshot snapshot;
    for(int i=0;i<chosen->ItemQuantities.Num();++i)snapshot.quantities[i]=chosen->ItemQuantities[i];
    for(int i=0;i<3;++i)snapshot.equipped[i]=chosen->EquippedItems[i];
    snapshot.milestones=chosen->AwardedMilestoneBits;assert(owner.Restore(snapshot));
    assert(owner.Claim(Achievement::Starter)==RewardResult::AlreadyAwarded);
    assert(chosen->BellwoldTaskStage!=2); // ReconcileEarnedMilestones has no Bellwold reward input.
    // Backup failure prevents primary write; primary failure leaves a usable prior record.
    for(int fail=1;fail<=2;++fail){
        Storage disk;disk.primary=good;disk.backup=earlier;disk.failAt=fail;
        Record next=good;next.ResumeHealth=88;
        assert(!disk.Commit(next));assert(disk.writes==fail);
        assert(disk.primary->ResumeHealth==37);assert(disk.Load()->ResumeHealth==37);
    }
    // Fault after a partial primary write during recovery: never rotate over backup.
    for(int reload=0;reload<3;++reload){
        recovery.writes=0;recovery.failAt=1;recovery.damageOnFailure=true;
        assert(!recovery.Commit(good));assert(recovery.writes==1);
        assert(recovery.backup->BellwoldTaskStage==0);
        assert(recovery.Load()==&*recovery.backup);
    }
    recovery.failAt=0;recovery.writes=0;
    assert(recovery.Commit(earlier));assert(!recovery.recovery&&recovery.writes==1);
    for(int reload=0;reload<4;++reload){assert(recovery.Load()==&*recovery.primary);
        assert(owner.Claim(Achievement::Starter)==RewardResult::AlreadyAwarded);}
    // Explicit recovery wins over a valid newer primary, then clears only on success.
    Storage explicitRecovery;explicitRecovery.primary=good;explicitRecovery.backup=earlier;explicitRecovery.recovery=true;
    assert(explicitRecovery.Load()==&*explicitRecovery.backup);
    assert(explicitRecovery.Commit(earlier)&&explicitRecovery.writes==1);
    // Reconcile a real milestone through save/reload; reward quantities stay fixed.
    Storage rewards;rewards.primary=good;rewards.backup=earlier;
    std::array<int,static_cast<int>(ItemId::Count)> awardedQuantities{};
    for(int reload=0;reload<5;++reload){
        auto loaded=*rewards.Load();auto inventory=RestoreItems(loaded);
        assert(loaded.BellwoldTaskStage==2);
        const auto result=inventory.Claim(Achievement::BellwoldLanterns);
        assert(result==(reload==0?RewardResult::Awarded:RewardResult::AlreadyAwarded));
        if(reload==0)awardedQuantities=inventory.Snapshot().quantities;
        else assert(inventory.Snapshot().quantities==awardedQuantities);
        CaptureItems(loaded,inventory);assert(rewards.Commit(loaded));
    }
    // A bad backup never overrides a valid primary or satisfies explicit recovery.
    Storage badBackup;badBackup.primary=good;badBackup.backup=corrupt;
    assert(badBackup.Load()==&*badBackup.primary);badBackup.recovery=true;assert(!badBackup.Load());
    // No subsequent potion attempt after a storage failure.
    int attempts=0;const int potions[]={0,1,2};
    assert(UseFirstConsumable(potions,[&](int id){++attempts;return id==0?ConsumableCommit::Unavailable:ConsumableCommit::SaveFailed;})==ConsumableCommit::SaveFailed);
    assert(attempts==2);
}
