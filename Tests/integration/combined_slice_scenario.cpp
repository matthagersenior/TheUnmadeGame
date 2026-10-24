// Composed production-rule scenario. The in-memory store models failed writes;
// this does not run actors, collision, Unreal serialization or platform storage.
#include "Save/UnmadeCombinedSaveRules.h"
#include "Items/UnmadeMilestoneRules.h"
#include "World/UnmadeCampaignSpineRules.h"
#include "Story/UnmadeCommitmentRules.h"
#include "Combat/UnmadeCombatRules.h"
#include "../save/combined_record_store.h"
#include <cassert>
#include <functional>
#include <iostream>
using namespace UnmadeCore;
using SaveFixture::Record;
using SaveFixture::Storage;
struct Slice {
    FractureModel fracture{"region.prototype.hub",{"variant.open","variant.sealed"}};
    LexiconModel lexicon;ConflictModel conflict;LivingWorldClock clock;
    DiscoveryLedger places;SettlementVisits visits;RegionalTaskModel tasks;
    FactionChronicle factions;FrontierJourney frontier;BellwoldAfterlight afterlight;
    InventoryModel inventory;RegionalEconomy playerEconomy;
    MaterialWorld materials;LivingEconomy wages;BesHousehold bes;
    Combatant combat{100,24,.75};
};
static void CaptureEquipment(const Slice& s,Record& r){
    const auto i=s.inventory.Snapshot();const auto p=s.playerEconomy.Snapshot();
    r.bHasInventorySnapshot=true;r.ItemQuantities.assign(i.quantities.begin(),i.quantities.end());
    r.EquippedItems.assign(i.equipped.begin(),i.equipped.end());r.AwardedMilestoneBits=i.milestones;
    r.bHasEconomySnapshot=true;r.TradeMarks=p.marks;r.PaidContractMask=p.completedContracts;
    r.ProfessionSkills.assign(p.craftsmanship.begin(),p.craftsmanship.end());
}
static Record Capture(const Slice& s){
    Record r;CaptureEquipment(s,r);
    r.bHasPlayerResume=true;r.ResumeHealth=s.combat.Health();r.ResumeStamina=100;
    r.bHasFractureSnapshot=true;const auto f=s.fracture.TakeSnapshot();
    r.WorldVariantId=f.variant;r.PlayerStrain=f.strain;
    r.bHasConflictSnapshot=true;const auto c=s.conflict.Snapshot();
    r.LocalConflictChoice=c.choice;r.SupplyActivityStage=c.supplies;
    r.bHasLivingWorldSnapshot=true;r.LivingWorldSeconds=s.clock.ElapsedSeconds();
    r.DiscoveredLoreMask=s.places.Snapshot();r.VisitedSettlementsMask=s.visits.Snapshot();
    const auto t=s.tasks.Snapshot();r.BellwoldTaskStage=t.bellwold;r.PaperhavenTaskStage=t.paperhaven;
    for(const auto& e:s.lexicon.Snapshot())r.LexiconEvidence.push_back(e);
    r.bHasFactionChronicle=true;const auto fc=s.factions.Snapshot();
    r.FactionStages.assign(fc.stages.begin(),fc.stages.end());r.FactionEndings.assign(fc.endings.begin(),fc.endings.end());
    r.bHasFrontierSnapshot=true;const auto fj=s.frontier.Snapshot();
    r.VisitedFrontierRealms=fj.visits;r.DiscoveredFrontierClues=fj.discoveries;
    r.FrontierStages.assign(fj.stages.begin(),fj.stages.end());r.FrontierEndings.assign(fj.endings.begin(),fj.endings.end());
    r.bHasAfterlightSnapshot=true;const auto a=s.afterlight.Snapshot();
    r.BellwoldAfterlightStage=a.stage;r.BellwoldAfterlightApproach=a.approach;r.BellwoldAfterlightOutcome=a.outcome;
    r.bHasMaterialWorld=true;
    for(const auto& m:s.materials.Snapshot()){
        r.MaterialDays.push_back(m.day);r.MaterialStores.push_back(m.store);
        r.MaterialHouseholds.push_back(m.households);r.MaterialIntegrity.push_back(m.integrity);
        r.MaterialProduced.push_back(m.produced);r.MaterialConsumed.push_back(m.consumed);
        r.MaterialShortageDays.push_back(m.shortageDays);r.MaterialLaborHours.push_back(m.laborHours);
        r.MaterialDeliveries.push_back(m.deliveries);r.MaterialRepairs.push_back(m.repairs);
    }
    const auto cargo=s.materials.Cargo();r.MaterialCargoRealm=cargo.realm;
    r.MaterialCargoUnits=cargo.units;r.MaterialCargoDay=cargo.collectedDay;
    r.bHasLivingEconomy=true;const auto living=s.wages.Save();
    for(const auto& w:living.workers){
        r.LivingEmployerMarks.push_back(w.employerMarks);r.LivingEarnedMarks.push_back(w.earnedMarks);
        r.LivingOwedMarks.push_back(w.owedMarks);r.LivingPaidShifts.push_back(w.shiftsPaid);
        r.LivingPaidDays.push_back(w.lastPaidDay);r.LivingWorkerReceipts.push_back(w.deliveriesWitnessed);
    }
    for(const auto& route:living.routes){r.LivingForeignStock.push_back(route.foreignStock);
        r.LivingRouteReceipts.push_back(route.deliveries);r.LivingTradePayments.push_back(route.paidMarks);
        r.LivingConsumedSupport.push_back(route.consumedSupport);}
    r.LivingTradeCargoRoute=living.cargo.route;r.LivingTradeCargoDay=living.cargo.collectedDay;
    r.bHasBesHousehold=true;const auto b=s.bes.Save();
    r.BesQuestPhase=b.phase;r.BesHouseholdDay=b.day;r.BesHeatBaskets=b.heatBaskets;
    r.BesColdDays=b.daysCold;r.BesWalkoutDays=b.daysWalkedOut;r.BesReliefPacks=b.reliefPacks;
    r.BesFirstReportDay=b.firstReportDay;r.BesMeraGuaranteeOwed=b.guaranteeOwed;
    r.bBesMeraConsented=b.meraConsented;r.bBesMeraGuaranteeUsed=b.meraGuaranteeUsed;
    r.bBesHomeVisited=b.homeVisited;r.bBesRhevaInterviewed=b.wardenInterviewed;
    assert(SaveFixture::Valid(r));return r;
}
static Slice Restore(const Record& r){
    assert(SaveFixture::Valid(r));Slice s;
    assert(s.clock.Restore(r.LivingWorldSeconds));const int day=s.clock.DayIndex()+1;
    assert(s.combat.RestoreHealth(r.ResumeHealth));
    assert(s.fracture.Restore({r.WorldVariantId,r.PlayerStrain}));
    assert(s.conflict.Restore({r.LocalConflictChoice,r.SupplyActivityStage}));
    assert(s.places.Restore(r.DiscoveredLoreMask));assert(s.visits.Restore(r.VisitedSettlementsMask));
    assert(s.tasks.Restore({r.BellwoldTaskStage,r.PaperhavenTaskStage}));
    std::vector<std::string> clues(r.LexiconEvidence.begin(),r.LexiconEvidence.end());assert(s.lexicon.Restore(clues));
    InventorySnapshot items;
    for(int i=0;i<r.ItemQuantities.Num();++i)items.quantities[i]=r.ItemQuantities[i];
    for(int i=0;i<3;++i)items.equipped[i]=r.EquippedItems[i];
    items.milestones=r.AwardedMilestoneBits;assert(s.inventory.Restore(items));
    EconomySnapshot money;money.marks=r.TradeMarks;money.completedContracts=r.PaidContractMask;
    for(int i=0;i<3;++i)money.craftsmanship[i]=r.ProfessionSkills[i];
    assert(s.playerEconomy.Restore(money));
    FactionSnapshot faction;
    for(int i=0;i<3;++i){faction.stages[i]=r.FactionStages[i];faction.endings[i]=r.FactionEndings[i];}
    assert(s.factions.Restore(faction));FrontierSnapshot frontier;
    frontier.visits=r.VisitedFrontierRealms;frontier.discoveries=r.DiscoveredFrontierClues;
    for(int i=0;i<2;++i){frontier.stages[i]=r.FrontierStages[i];frontier.endings[i]=r.FrontierEndings[i];}
    assert(s.frontier.Restore(frontier));
    assert(s.afterlight.Restore({r.BellwoldAfterlightStage,r.BellwoldAfterlightApproach,r.BellwoldAfterlightOutcome}));
    MaterialWorld::Towns towns{};
    for(int i=0;i<9;++i)towns[i]={r.MaterialDays[i],r.MaterialStores[i],r.MaterialHouseholds[i],
        r.MaterialIntegrity[i],r.MaterialProduced[i],r.MaterialConsumed[i],r.MaterialShortageDays[i],
        r.MaterialLaborHours[i],r.MaterialDeliveries[i],r.MaterialRepairs[i]};
    assert(s.materials.Restore(towns,{r.MaterialCargoRealm,r.MaterialCargoUnits,r.MaterialCargoDay},day));
    LivingEconomy::Snapshot wages;
    for(int i=0;i<9;++i)wages.workers[i]={r.LivingEmployerMarks[i],r.LivingEarnedMarks[i],r.LivingOwedMarks[i],
        r.LivingPaidShifts[i],r.LivingPaidDays[i],r.LivingWorkerReceipts[i]};
    for(int i=0;i<2;++i)wages.routes[i]={r.LivingForeignStock[i],r.LivingRouteReceipts[i],
        r.LivingTradePayments[i],r.LivingConsumedSupport[i]};
    wages.cargo={r.LivingTradeCargoRoute,r.LivingTradeCargoDay};assert(s.wages.Restore(wages,s.materials,day));
    assert(s.bes.Restore({r.BesQuestPhase,r.BesHouseholdDay,r.BesHeatBaskets,r.BesColdDays,
        r.BesWalkoutDays,r.BesReliefPacks,r.BesFirstReportDay,r.BesMeraGuaranteeOwed,
        r.bBesMeraConsented,r.bBesMeraGuaranteeUsed,r.bBesHomeVisited,r.bBesRhevaInterviewed},day));
    return s;
}
static void Same(const Record& a,const Record& b){
    // Compare every field this scenario captures, not just a completion flag.
    assert(a.AwardedMilestoneBits==b.AwardedMilestoneBits);
    assert(a.BellwoldAfterlightApproach==b.BellwoldAfterlightApproach);
    assert(a.BellwoldAfterlightOutcome==b.BellwoldAfterlightOutcome);
    assert(a.BellwoldAfterlightStage==b.BellwoldAfterlightStage);
    assert(a.BellwoldTaskStage==b.BellwoldTaskStage);
    assert(a.BesColdDays==b.BesColdDays);
    assert(a.BesFirstReportDay==b.BesFirstReportDay);
    assert(a.BesHeatBaskets==b.BesHeatBaskets);
    assert(a.BesHouseholdDay==b.BesHouseholdDay);
    assert(a.BesMeraGuaranteeOwed==b.BesMeraGuaranteeOwed);
    assert(a.BesQuestPhase==b.BesQuestPhase);
    assert(a.BesReliefPacks==b.BesReliefPacks);
    assert(a.BesWalkoutDays==b.BesWalkoutDays);
    assert(a.DiscoveredFrontierClues==b.DiscoveredFrontierClues);
    assert(a.DiscoveredLoreMask==b.DiscoveredLoreMask);
    assert(a.EquippedItems==b.EquippedItems);
    assert(a.FactionEndings==b.FactionEndings);
    assert(a.FactionStages==b.FactionStages);
    assert(a.FrontierEndings==b.FrontierEndings);
    assert(a.FrontierStages==b.FrontierStages);
    assert(a.ItemQuantities==b.ItemQuantities);
    assert(a.LexiconEvidence==b.LexiconEvidence);
    assert(a.LivingConsumedSupport==b.LivingConsumedSupport);
    assert(a.LivingEarnedMarks==b.LivingEarnedMarks);
    assert(a.LivingEmployerMarks==b.LivingEmployerMarks);
    assert(a.LivingForeignStock==b.LivingForeignStock);
    assert(a.LivingOwedMarks==b.LivingOwedMarks);
    assert(a.LivingPaidDays==b.LivingPaidDays);
    assert(a.LivingPaidShifts==b.LivingPaidShifts);
    assert(a.LivingRouteReceipts==b.LivingRouteReceipts);
    assert(a.LivingTradeCargoDay==b.LivingTradeCargoDay);
    assert(a.LivingTradeCargoRoute==b.LivingTradeCargoRoute);
    assert(a.LivingTradePayments==b.LivingTradePayments);
    assert(a.LivingWorkerReceipts==b.LivingWorkerReceipts);
    assert(a.LivingWorldSeconds==b.LivingWorldSeconds);
    assert(a.LocalConflictChoice==b.LocalConflictChoice);
    assert(a.MaterialCargoDay==b.MaterialCargoDay);
    assert(a.MaterialCargoRealm==b.MaterialCargoRealm);
    assert(a.MaterialCargoUnits==b.MaterialCargoUnits);
    assert(a.MaterialConsumed==b.MaterialConsumed);
    assert(a.MaterialDays==b.MaterialDays);
    assert(a.MaterialDeliveries==b.MaterialDeliveries);
    assert(a.MaterialHouseholds==b.MaterialHouseholds);
    assert(a.MaterialIntegrity==b.MaterialIntegrity);
    assert(a.MaterialLaborHours==b.MaterialLaborHours);
    assert(a.MaterialProduced==b.MaterialProduced);
    assert(a.MaterialRepairs==b.MaterialRepairs);
    assert(a.MaterialShortageDays==b.MaterialShortageDays);
    assert(a.MaterialStores==b.MaterialStores);
    assert(a.PaidContractMask==b.PaidContractMask);
    assert(a.PaperhavenTaskStage==b.PaperhavenTaskStage);
    assert(a.PlayerStrain==b.PlayerStrain);
    assert(a.ProfessionSkills==b.ProfessionSkills);
    assert(a.ResumeHealth==b.ResumeHealth);
    assert(a.ResumeStamina==b.ResumeStamina);
    assert(a.SupplyActivityStage==b.SupplyActivityStage);
    assert(a.TradeMarks==b.TradeMarks);
    assert(a.VisitedFrontierRealms==b.VisitedFrontierRealms);
    assert(a.VisitedSettlementsMask==b.VisitedSettlementsMask);
    assert(a.WorldVariantId==b.WorldVariantId);
    assert(a.bBesHomeVisited==b.bBesHomeVisited);
    assert(a.bBesMeraConsented==b.bBesMeraConsented);
    assert(a.bBesMeraGuaranteeUsed==b.bBesMeraGuaranteeUsed);
    assert(a.bBesRhevaInterviewed==b.bBesRhevaInterviewed);
    assert(a.bHasAfterlightSnapshot==b.bHasAfterlightSnapshot);
    assert(a.bHasBesHousehold==b.bHasBesHousehold);
    assert(a.bHasConflictSnapshot==b.bHasConflictSnapshot);
    assert(a.bHasEconomySnapshot==b.bHasEconomySnapshot);
    assert(a.bHasFactionChronicle==b.bHasFactionChronicle);
    assert(a.bHasFractureSnapshot==b.bHasFractureSnapshot);
    assert(a.bHasFrontierSnapshot==b.bHasFrontierSnapshot);
    assert(a.bHasInventorySnapshot==b.bHasInventorySnapshot);
    assert(a.bHasLivingEconomy==b.bHasLivingEconomy);
    assert(a.bHasLivingWorldSnapshot==b.bHasLivingWorldSnapshot);
    assert(a.bHasMaterialWorld==b.bHasMaterialWorld);
    assert(a.bHasPlayerResume==b.bHasPlayerResume);
}
static bool PersistEquipment(Storage& disk,const Slice& s){
    const auto* selected=disk.Load();assert(selected);Record next=*selected;
    CaptureEquipment(s,next);return disk.Commit(next);
}
static void Reconcile(Storage& disk,Slice& s){
    const Record committed=*disk.Load();
    ForEachEarnedMilestone(committed,[&](const char* clue){
        return std::find(committed.LexiconEvidence.begin(),committed.LexiconEvidence.end(),clue)!=committed.LexiconEvidence.end();
    },[&](Achievement reward){
        const auto before=s.inventory.Snapshot();
        if(s.inventory.Claim(reward)==RewardResult::Awarded&&!PersistEquipment(disk,s))
            assert(s.inventory.Restore(before));
    });
    ForEachCivicContract(committed,[&](SettlementId town,int stage){
        const auto before=s.playerEconomy.Snapshot();
        if(s.playerEconomy.PayContract(town,stage)==EconomyResult::Completed&&!PersistEquipment(disk,s))
            assert(s.playerEconomy.Restore(before));
    });
}
static void Checkpoint(Storage& disk,Slice& s){const auto* r=disk.Load();assert(r);s=Restore(*r);Same(Capture(s),*r);}
using Action=std::function<void(Slice&)>;
static int boundaries=0;
static void CommitStep(Storage& disk,Slice& s,const Action& change){
    const Record before=Capture(s);
    // Each owner stages its change and rolls back on failed persistence. Inject
    // both failure positions; the shared boundary must expose only the old record.
    for(int fail=1;fail<=2;++fail){
        Slice staged=s;change(staged);disk.writes=0;disk.failAt=fail;disk.damageOnFailure=true;
        assert(!disk.Commit(Capture(staged)));++boundaries;
        Same(*disk.Load(),before);Checkpoint(disk,s);
    }
    change(s);disk.failAt=0;assert(disk.Commit(Capture(s)));Checkpoint(disk,s);++boundaries;
}
static RoadGuidance Guidance(const Slice& s){
    RoadEvidence evidence;evidence.openingWitnessed=s.conflict.Choice()!=ConflictChoice::None||s.visits.Count()>=2||s.places.Count()>=2;
    if(s.frontier.IsResolved(Realm::WidowedRain))evidence.firstStories|=RoadMask(Realm::WidowedRain);
    if(s.frontier.IsResolved(Realm::HearthBeneath))evidence.firstStories|=RoadMask(Realm::HearthBeneath);
    return EvaluateUnansweredRoad(evidence);
}
static int rewardFaults=0;
static void ReconcileWithFailures(Storage& disk,Slice& state){
    const Record pending=Capture(state);
    Storage allPaid;allPaid.primary=pending;allPaid.backup=pending;auto paid=state;
    Reconcile(allPaid,paid);Checkpoint(allPaid,paid);const Record expected=Capture(paid);
    const int writes=allPaid.writes;
    // A failure anywhere in reward or civic contract writes must converge to
    // the same once-only quantities after retry/reload, with no wage changes.
    for(bool partial:{false,true})for(int failure=1;failure<=writes;++failure){
        Storage retry;retry.primary=pending;retry.backup=pending;retry.failAt=failure;retry.damageOnFailure=partial;
        auto reloaded=Restore(pending);Reconcile(retry,reloaded);
        assert(retry.writes>=failure);++rewardFaults;
        retry.failAt=0;Checkpoint(retry,reloaded);Reconcile(retry,reloaded);Checkpoint(retry,reloaded);
        Same(Capture(reloaded),expected);
        for(int reload=0;reload<3;++reload){Reconcile(retry,reloaded);Checkpoint(retry,reloaded);Same(Capture(reloaded),expected);}
    }
    disk=allPaid;state=paid;
}
static void OpeningAndReturn(int branch){
    Slice s;assert(s.inventory.Claim(Achievement::Starter)==RewardResult::Awarded);
    assert(s.inventory.Equip(ItemId::SalvagerBlade));assert(s.inventory.Equip(ItemId::TravelerMantle));
    Storage disk;disk.primary=Capture(s);disk.backup=disk.primary;
    assert(Guidance(s).act==RoadAct::Opening);
    CommitStep(disk,s,[](auto& state){
        assert(state.fracture.Glimpse(true,1)==Result::Applied);
        assert(state.lexicon.RecordEvidence("term.veyl","evidence.glimpse"));
    });
    CommitStep(disk,s,[](auto& state){assert(state.places.Discover(District::EchoWell));});
    assert(Guidance(s).act==RoadAct::Opening);
    CommitStep(disk,s,[](auto& state){assert(state.places.Discover(District::PaperOrchard));});
    assert(Guidance(s).act==RoadAct::MissingWorld);
    CommitStep(disk,s,[](auto& state){assert(state.lexicon.RecordEvidence("term.veyl","evidence.archivist"));});
    // Combat receipts affect real saved health. One swing cannot hit twice.
    CommitStep(disk,s,[](auto& state){
        assert(state.combat.SetGuarding(true));Combatant enemy(65,12,1.4);
        const auto swing=enemy.TryAttack(1);assert(swing.outcome==AttackOutcome::Started);
        assert(state.combat.ReceiveHit(901,swing.swingId,swing.baseDamage,false)==HitOutcome::Applied);
        assert(state.combat.ReceiveHit(901,swing.swingId,swing.baseDamage,false)==HitOutcome::Duplicate);
        assert(state.combat.SetGuarding(false));assert(state.combat.Health()==97);
    });
    const auto choice=static_cast<ConflictChoice>(branch);
    assert(s.conflict.Commit(choice,false)==ConflictResult::NeedsConfirmation);
    assert(s.conflict.Choice()==ConflictChoice::None);
    CommitStep(disk,s,[&](auto& state){assert(state.conflict.Commit(choice,true)==ConflictResult::Committed);});
    assert(s.conflict.ShelterOpen()==(branch==1));assert(s.conflict.ArchiveOpen()==(branch==2));
    assert(s.conflict.Commit(static_cast<ConflictChoice>(3-branch),true)==ConflictResult::AlreadyCommitted);
    CommitStep(disk,s,[](auto& state){assert(state.conflict.CollectSupplies(true));});
    CommitStep(disk,s,[](auto& state){assert(state.conflict.DeliverSupplies(true));});
    assert(!s.conflict.DeliverSupplies(true));
    for(const auto village:{SettlementId::Crossings,SettlementId::Bellwold,SettlementId::Paperhaven})
        CommitStep(disk,s,[&](auto& state){assert(state.visits.Visit(village));});
    const char* speakers[]={"npc.bellwold.lamplighter.001","npc.bellwold.matron.001",
        "npc.paperhaven.scribe.001","npc.paperhaven.registrar.001"};
    for(int i=0;i<4;++i)CommitStep(disk,s,[&](auto& state){
        assert(state.tasks.Converse(i<2?SettlementId::Bellwold:SettlementId::Paperhaven,speakers[i])==
            (i%2==0?TaskResult::Advanced:TaskResult::Completed));
    });
    // The shared production predicates pay each real player contract once.
    ReconcileWithFailures(disk,s);
    assert(s.playerEconomy.Marks()==45+15+36+32);
    assert(s.inventory.HasClaimed(branch==1?Achievement::ShelterChoice:Achievement::ResearchChoice));
    assert(!s.inventory.HasClaimed(branch==1?Achievement::ResearchChoice:Achievement::ShelterChoice));
    assert(AttunementFromRewards(s.inventory)==1);
    const auto destination=branch==1?Realm::WidowedRain:Realm::HearthBeneath;
    assert(!RoadRoute(Realm::ThreefoldReach,destination,AttunementFromRewards(s.inventory)).empty());
    CommitStep(disk,s,[&](auto& state){assert(state.frontier.Visit(destination)==FrontierEvent::Advanced);});
    CommitStep(disk,s,[&](auto& state){assert(state.frontier.FindClue(destination)==FrontierEvent::Advanced);});
    const char* rain[]={"npc.saltwake.navigator.001","npc.saltwake.rainkeeper.001","npc.saltwake.harborwarden.001"};
    const char* hearth[]={"npc.cinderhold.hearthreader.001","npc.cinderhold.healer.001","npc.cinderhold.emberwarden.001"};
    for(int i=0;i<3;++i)CommitStep(disk,s,[&](auto& state){
        assert(state.frontier.Converse(branch==1?rain[i]:hearth[i])==
            (i==2?FrontierEvent::FinalChoice:FrontierEvent::Advanced));
    });
    CommitStep(disk,s,[&](auto& state){assert(state.frontier.Resolve(destination,branch)==FrontierEvent::Resolved);});
    assert(Guidance(s).act==RoadAct::CostOfAbsence);
    // A return chapter depends on actual Bellwold and Refuge evidence.
    assert(s.afterlight.Begin(2,0,"npc.bellwold.matron.001")==AfterlightResult::Locked);
    const char* refuge[]={"npc.bellwold.lamplighter.001","npc.bellwold.matron.001","npc.bellwold.guard.001"};
    for(int i=0;i<3;++i)CommitStep(disk,s,[&](auto& state){
        assert(state.factions.Converse(refuge[i],true)==(i==2?FactionResult::ChoiceRequired:FactionResult::Advanced));
    });
    CommitmentGate consent;
    assert(consent.Attempt("faction.refuge",branch,10)==CommitmentAttempt::Preview);
    assert(s.factions.Ending(Faction::Refuge)==FactionEnding::Unresolved);
    assert(consent.Attempt("faction.refuge",branch,11)==CommitmentAttempt::Confirmed);
    CommitStep(disk,s,[&](auto& state){assert(state.factions.Decide(Faction::Refuge,
        static_cast<FactionEnding>(branch))==FactionResult::Resolved);});
    CommitStep(disk,s,[](auto& state){assert(state.afterlight.Begin(
        static_cast<int>(state.tasks.Progress(SettlementId::Bellwold)),
        static_cast<int>(state.factions.Ending(Faction::Refuge)),"npc.bellwold.matron.001")==AfterlightResult::Started);});
    const auto path=static_cast<AfterlightChoice>(branch);
    CommitStep(disk,s,[&](auto& state){assert(state.afterlight.Inspect(path,
        branch==1?"Bellwold.Afterlight.Relief":"Bellwold.Afterlight.Census")==AfterlightResult::EvidenceFound);});
    CommitStep(disk,s,[&](auto& state){assert(state.afterlight.Converse(
        branch==1?"npc.bellwold.healer.001":"npc.bellwold.childtutor.001")==AfterlightResult::Witnessed);});
    assert(s.afterlight.Resolve(static_cast<AfterlightChoice>(3-branch))==AfterlightResult::WrongEvidence);
    CommitStep(disk,s,[&](auto& state){assert(state.afterlight.Resolve(path)==AfterlightResult::Resolved);});
    const Record earned=Capture(s);
    ReconcileWithFailures(disk,s);const Record expected=Capture(s);
    assert(s.inventory.Quantity(branch==1?ItemId::AfterlightWard:ItemId::UnredactedLantern)==1);
    assert(s.inventory.Quantity(branch==1?ItemId::UnredactedLantern:ItemId::AfterlightWard)==0);
    // A malformed late financial field cannot supply even otherwise valid rewards.
    auto bad=expected;bad.LivingWorkerReceipts.back()=-1;
    Storage recover;recover.primary=bad;recover.backup=earned;
    assert(recover.Load()==&*recover.backup);auto recovered=Restore(*recover.Load());
    Reconcile(recover,recovered);Checkpoint(recover,recovered);Same(Capture(recovered),expected);
    // Payroll/consent remains a separate, conserved ledger through the same file.
    for(int day=2;day<=18;++day)CommitStep(disk,s,[&](auto& state){
        assert(state.clock.Restore((day-1)*LivingWorldClock::SecondsPerDay));
        const std::array<RealityPublicResponse,9> civic{};
        assert(state.materials.AdvanceAllOneDay(civic,state.bes.OnWalkout()));
        assert(state.wages.AdvanceOneDay(day,state.bes.OnWalkout()));assert(state.bes.AdvanceOneDay(day));
    });
    assert(s.wages.Worker(Realm::HearthBeneath)->owedMarks==2);
    CommitStep(disk,s,[](auto& state){assert(state.bes.HearBes(state.wages,18)==BesOutcome::Reported);});
    CommitStep(disk,s,[](auto& state){assert(state.bes.InspectHome(18)==BesOutcome::HomeInspected);});
    CommitStep(disk,s,[](auto& state){assert(state.bes.HearRheva(18)==BesOutcome::LedgerCorroborated);});
    assert(s.bes.AcceptMeraGuarantee(s.wages,18)==BesOutcome::Blocked);
    CommitStep(disk,s,[](auto& state){assert(state.bes.OfferMeraGuarantee(18)==BesOutcome::ConsentRecorded);});
    CommitStep(disk,s,[](auto& state){assert(state.bes.AcceptMeraGuarantee(state.wages,18)==BesOutcome::GuaranteePaid);});
    const Record underwritten=Capture(s);
    for(int reload=0;reload<4;++reload){
        Checkpoint(disk,s);assert(s.bes.Save().meraConsented&&s.bes.Save().meraGuaranteeUsed);
        assert(s.bes.Save().guaranteeOwed==2);assert(s.bes.AcceptMeraGuarantee(s.wages,18)==BesOutcome::Blocked);
        Reconcile(disk,s);Checkpoint(disk,s);Same(Capture(s),underwritten);
        assert(s.wages.Worker(Realm::WidowedRain)->earnedMarks==30);
        assert(s.wages.Worker(Realm::HearthBeneath)->earnedMarks==34);
        assert(s.playerEconomy.Marks()==expected.TradeMarks);
    }
}
int main(){OpeningAndReturn(1);OpeningAndReturn(2);
    std::cout<<"PASS: two civic/frontier/return routes; "<<boundaries
        <<" successful/faulted combined commits; "<<rewardFaults<<" reward/contract write failures; conserved consenting wages\n";
}
