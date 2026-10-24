#include "rite_cast_fixture.h"
#include "Save/UnmadeActionCommitRules.h"
#include "Save/UnmadeCaptureRules.h"
#include "Items/UnmadeMilestoneRules.h"
#include "Story/UnmadeCommitmentRules.h"
#include <iostream>
using namespace UnmadeCore;
using SaveFixture::Record;
struct ActionState {
 RiteFixture::State world;FactionChronicle factions;FrontierJourney frontier;BellwoldAfterlight afterlight;
 RegionalTaskModel tasks;DiscoveryLedger places;ConflictModel conflict;ResidentContinuity residents;
 WitnessEchoLedger echoes;WitnessBraidChronicle braid;WitnessDispatch dispatch;WitnessReturn reply;
 InventoryModel inventory;SaveFixture::Array<SaveFixture::Npc> memories;
 Record Capture()const{
  auto r=world.Capture();r.NpcSnapshots=memories;
  auto f=factions.Snapshot();r.bHasFactionChronicle=true;r.FactionStages.assign(f.stages.begin(),f.stages.end());r.FactionEndings.assign(f.endings.begin(),f.endings.end());
  auto j=frontier.Snapshot();r.bHasFrontierSnapshot=true;r.VisitedFrontierRealms=j.visits;r.DiscoveredFrontierClues=j.discoveries;r.FrontierStages.assign(j.stages.begin(),j.stages.end());r.FrontierEndings.assign(j.endings.begin(),j.endings.end());
  auto a=afterlight.Snapshot();r.bHasAfterlightSnapshot=true;r.BellwoldAfterlightStage=a.stage;r.BellwoldAfterlightApproach=a.approach;r.BellwoldAfterlightOutcome=a.outcome;
  auto t=tasks.Snapshot();r.BellwoldTaskStage=t.bellwold;r.PaperhavenTaskStage=t.paperhaven;r.DiscoveredLoreMask=places.Snapshot();
  auto c=conflict.Snapshot();r.bHasConflictSnapshot=true;r.LocalConflictChoice=c.choice;r.SupplyActivityStage=c.supplies;
  auto e=echoes.Snapshot();r.bHasWitnessEchoSnapshot=true;r.WitnessEchoFirstMask=e.first;r.WitnessEchoReturnMask=e.returnRead;r.bHasWitnessBraidSnapshot=true;r.WitnessBraidOutcome=braid.Snapshot().outcome;
  auto d=dispatch.Snapshot();r.bHasWitnessDispatchSnapshot=true;r.WitnessDispatchStage=d.stage;r.WitnessDispatchCollectedDay=d.collectedDay;r.WitnessDispatchDeliveredDay=d.deliveredDay;
  auto q=reply.Snapshot();r.bHasWitnessReturnSnapshot=true;r.WitnessReturnStage=q.stage;r.WitnessReturnRoute=q.route;r.WitnessReturnChosenDay=q.chosenDay;r.WitnessReturnCollectedDay=q.collectedDay;r.WitnessReturnDeliveredDay=q.deliveredDay;
  auto p=residents.Snapshot();r.bHasResidentContinuitySnapshot=true;r.ResidentEncounterVisits.assign(p.visits.begin(),p.visits.end());r.ResidentEncounterLastDays.assign(p.lastDay.begin(),p.lastDay.end());r.ResidentEncounterAidFlags.assign(p.aids.begin(),p.aids.end());
  auto i=inventory.Snapshot();r.bHasInventorySnapshot=true;r.ItemQuantities.assign(i.quantities.begin(),i.quantities.end());r.EquippedItems.assign(i.equipped.begin(),i.equipped.end());r.AwardedMilestoneBits=i.milestones;
  assert(SaveFixture::Valid(r));return r;
 }
 void Load(const Record& r){
  world.Load(r);memories=r.NpcSnapshots;
  FactionSnapshot f;for(int i=0;i<3;++i){f.stages[i]=r.FactionStages[i];f.endings[i]=r.FactionEndings[i];}assert(factions.Restore(f));
  FrontierSnapshot j;j.visits=r.VisitedFrontierRealms;j.discoveries=r.DiscoveredFrontierClues;for(int i=0;i<2;++i){j.stages[i]=r.FrontierStages[i];j.endings[i]=r.FrontierEndings[i];}assert(frontier.Restore(j));
  assert(afterlight.Restore({r.BellwoldAfterlightStage,r.BellwoldAfterlightApproach,r.BellwoldAfterlightOutcome}));assert(tasks.Restore({r.BellwoldTaskStage,r.PaperhavenTaskStage}));assert(places.Restore(r.DiscoveredLoreMask));assert(conflict.Restore({r.LocalConflictChoice,r.SupplyActivityStage}));
  assert(echoes.Restore({static_cast<std::uint32_t>(r.WitnessEchoFirstMask),static_cast<std::uint32_t>(r.WitnessEchoReturnMask)}));assert(braid.Restore({r.WitnessBraidOutcome},echoes));assert(dispatch.Restore({r.WitnessDispatchStage,r.WitnessDispatchCollectedDay,r.WitnessDispatchDeliveredDay},braid.Outcome()));assert(reply.Restore({r.WitnessReturnStage,r.WitnessReturnRoute,r.WitnessReturnChosenDay,r.WitnessReturnCollectedDay,r.WitnessReturnDeliveredDay},dispatch,braid.Outcome(),4));
  ResidentContinuitySnapshot p;for(int i=0;i<ResidentContinuityCount;++i){p.visits[i]=r.ResidentEncounterVisits[i];p.lastDay[i]=r.ResidentEncounterLastDays[i];p.aids[i]=r.ResidentEncounterAidFlags[i];}assert(residents.Restore(p));
  InventorySnapshot inv;for(int i=0;i<r.ItemQuantities.Num();++i)inv.quantities[i]=r.ItemQuantities[i];for(int i=0;i<3;++i)inv.equipped[i]=r.EquippedItems[i];inv.milestones=r.AwardedMilestoneBits;assert(inventory.Restore(inv));
 }
};
static void Same(const Record& a,const Record& b){
 RiteFixture::Same(a,b);assert(a.LegacyDeedBits==b.LegacyDeedBits&&a.BrokenOathCount==b.BrokenOathCount&&a.bOathRedeemed==b.bOathRedeemed&&a.bOathActive==b.bOathActive);
 assert(a.FactionStages==b.FactionStages&&a.FactionEndings==b.FactionEndings&&a.FrontierStages==b.FrontierStages&&a.FrontierEndings==b.FrontierEndings);
 assert(a.BellwoldTaskStage==b.BellwoldTaskStage&&a.PaperhavenTaskStage==b.PaperhavenTaskStage&&a.DiscoveredLoreMask==b.DiscoveredLoreMask&&a.LocalConflictChoice==b.LocalConflictChoice&&a.SupplyActivityStage==b.SupplyActivityStage);
 assert(a.BellwoldAfterlightStage==b.BellwoldAfterlightStage&&a.BellwoldAfterlightOutcome==b.BellwoldAfterlightOutcome&&a.WitnessBraidOutcome==b.WitnessBraidOutcome&&a.WitnessReturnRoute==b.WitnessReturnRoute);
 assert(a.ResidentEncounterAidFlags==b.ResidentEncounterAidFlags&&a.NpcSnapshots.size()==b.NpcSnapshots.size());
 for(std::size_t i=0;i<a.NpcSnapshots.size();++i){assert(a.NpcSnapshots[i].NpcId==b.NpcSnapshots[i].NpcId&&a.NpcSnapshots[i].Observations.size()==b.NpcSnapshots[i].Observations.size());for(std::size_t n=0;n<a.NpcSnapshots[i].Observations.size();++n){const auto& x=a.NpcSnapshots[i].Observations[n];const auto& y=b.NpcSnapshots[i].Observations[n];assert(x.EventId==y.EventId&&x.EventKind==y.EventKind&&x.SubjectId==y.SubjectId&&x.Evidence==y.Evidence&&x.SpeakerId==y.SpeakerId);}}
 assert(a.ItemQuantities==b.ItemQuantities&&a.AwardedMilestoneBits==b.AwardedMilestoneBits);
}
static const char* Event(int kind,int choice){
 if(kind>=26)return "Reality.Rite";
 switch(kind){case 0:return "Player.Helped";case 1:case 2:return "Player.HelpedVillage";case 3:case 4:case 5:case 6:case 7:return "World.FactionResolved";case 8:return "";case 9:return choice==1?"World.ConflictShelter":"World.ConflictResearch";case 10:return "Player.DeliveredSupplies";case 11:return choice==1?"World.AfterlightShelter":"World.AfterlightNames";case 12:return choice==1?"World.WitnessBraidShelter":"World.WitnessBraidDocket";case 13:return choice==1?"World.HessaPrivateCounsel":"World.HessaPublicHearing";case 22:return choice==1?"World.RiteProtect":"World.RiteReveal";case 23:return "World.BrokenOath";case 24:return "World.RedeemedOath";case 25:return "Reality.Rite";default:return "World.LaterRealmResolved";}
}
static void Prepare(ActionState& s,int kind,int choice){
 s.world.now=3600;RiteFixture::MasterAll(s.world);
 // Existing absent/rumor histories must survive success and failed action undo.
 SaveFixture::Npc absent;absent.NpcId="npc.unloaded";absent.Observations.push_back({{99},"Reality.Anomaly","old","npc.courier",1});s.memories.push_back(absent);
 auto f=s.factions.Snapshot();f.stages.fill(3);assert(s.factions.Restore(f));auto j=s.frontier.Snapshot();j.stages.fill(3);j.visits=3;j.discoveries=3;assert(s.frontier.Restore(j));
 if(kind==1||kind==2)assert(s.tasks.Restore({kind==1?1:0,kind==2?1:0}));
 if(kind==10)assert(s.conflict.CollectSupplies(true));
 if(kind==11)assert(s.afterlight.Restore({3,choice,0}));
 if(kind==12||kind==13){assert(s.echoes.Read("site.callback.uncounted_cup",0)==WitnessEchoEvent::FirstReading);assert(s.echoes.Read("site.callback.inkless_nail",0)==WitnessEchoEvent::FirstReading);assert(s.echoes.Read("site.callback.two_faced_press",0)==WitnessEchoEvent::FirstReading);assert(s.echoes.Read("site.callback.two_faced_press",2)==WitnessEchoEvent::LaterMeaning);if(kind==13){assert(s.braid.Commit(choice,s.echoes)==WitnessBraidEvent::Committed);assert(s.dispatch.Collect(s.braid.Outcome(),2)==WitnessDispatchEvent::Collected);assert(s.dispatch.HandToHessa(s.braid.Outcome(),3)==WitnessDispatchEvent::Delivered);}}
 if(kind>=14&&kind<=21){const auto& a=RealmAftermathSpecs[kind-13];if(kind>=16){const auto& r=LaterRealms[kind-16];assert(s.world.chapters.first.Visit(r.realm)==LaterResult::Visited);assert(s.world.chapters.first.Begin(r.realm,r.initiator)==LaterResult::Started);assert(s.world.chapters.first.Study(r.realm,1,r.trialEvidenceA)==LaterResult::EvidenceFound);assert(s.world.chapters.first.Operate(r.realm,r.mechanism)==LaterResult::Completed);}else {j.endings[kind-14]=1;assert(s.frontier.Restore(j));}assert(s.world.chapters.aftermath.Begin(a.realm,true,true,a.initiatingWitness)==RealmAftermathResult::Started);assert(s.world.chapters.aftermath.Prepare(a.realm,a.mechanismSite)==RealmAftermathResult::Prepared);assert(s.world.chapters.aftermath.Inspect(a.realm,choice,choice==1?a.evidenceForCare:a.evidenceForTruth)==RealmAftermathResult::EvidenceFound);assert(s.world.chapters.aftermath.Testify(a.realm,choice==1?a.careWitness:a.truthWitness)==RealmAftermathResult::Witnessed);}
 if(kind>=26){for(int i=0;i<=kind-26;++i){const auto id=static_cast<ConfluenceId>(i);assert(s.world.trials.Discover(id,i,1023,3)==ConfluenceResult::Advanced);assert(s.world.trials.RecordCast(id,Confluences[i].first,s.world.now,i)==ConfluenceResult::CastRecorded);assert(s.world.trials.RecordCast(id,Confluences[i].second,s.world.now,i)==ConfluenceResult::ReadyToDecide);if(i<kind-26)assert(s.world.trials.Resolve(id,i,3,1,s.world.now)==ConfluenceResult::Completed);}}
 if(kind==22){auto t=s.world.rites.Snapshot();t.stage[0]=3;t.choice[0]=0;assert(s.world.rites.Restore(t));}
 if(kind==23||kind==24){auto t=s.world.rites.Snapshot();t.oathActive=true;t.deeds=7;assert(s.world.rites.Restore(t));if(kind==24)assert(s.world.rites.BreakOath());}
}
static bool Advance(ActionState& s,int kind,int choice){
 if(kind>=26)return s.world.trials.Resolve(static_cast<ConfluenceId>(kind-26),kind-26,3,choice,s.world.now)==ConfluenceResult::Completed;
 switch(kind){case 0:return s.residents.Aid(ResidentContinuityId(0))==EncounterEvent::Aided;
 case 1:return s.tasks.Converse(SettlementId::Bellwold,"npc.bellwold.matron.001")==TaskResult::Completed;
 case 2:return s.tasks.Converse(SettlementId::Paperhaven,"npc.paperhaven.registrar.001")==TaskResult::Completed;
 case 3:case 4:case 5:return s.factions.Decide(static_cast<Faction>(kind-3),static_cast<FactionEnding>(choice))==FactionResult::Resolved;
 case 6:case 7:return s.frontier.Resolve(kind==6?Realm::WidowedRain:FrontierOutposts[1].realm,choice)==FrontierEvent::Resolved;
 case 8:return s.places.Discover(District::EchoWell);
 case 9:return s.conflict.Commit(static_cast<ConflictChoice>(choice),true)==ConflictResult::Committed;
 case 10:return s.conflict.DeliverSupplies(true);
 case 11:return s.afterlight.Resolve(static_cast<AfterlightChoice>(choice))==AfterlightResult::Resolved;
 case 12:return s.braid.Commit(choice,s.echoes)==WitnessBraidEvent::Committed;
 case 13:return s.reply.Choose(choice,4,s.dispatch,s.braid.Outcome())==WitnessReturnEvent::Chosen;
 case 22:{RiteContext c;c.verifiedWitnesses=3;c.lineOfSight=true;c.witness=RiteSpecs[0].witness;return s.world.rites.Advance(RiteId::UnwriteLaw,RiteAction::Decide,c,choice)==RiteResult::Advanced;}
 case 23:return s.world.rites.BreakOath();
 case 24:{RiteContext c;c.factionEndings=1;c.bellQuestComplete=true;c.verifiedWitnesses=3;return s.world.rites.RedeemOath(c);}
 case 25:return RiteFixture::Cast(s.world,Realm::ThreefoldReach,RiteId::UnwriteLaw,true,true,false,[]{return true;});
 default:return s.world.chapters.aftermath.Decide(RealmAftermathSpecs[kind-13].realm,choice)==RealmAftermathResult::Resolved;}
}
int main(){int paths=0,reloads=0;
 assert(ActionDeedMask(nullptr)==0&&ActionDeedMask("Player.Helped")==1&&ActionDeedMask("Player.HelpedVillage")==1&&ActionDeedMask("World.FactionResolved")==4&&ActionDeedMask("World.RiteProtect")==0);
 for(int kind=0;kind<32;++kind)for(int choice=1;choice<=2;++choice)for(int fault=1;fault<=2;++fault){
  ActionState live;Prepare(live,kind,choice);const auto before=live.Capture();SaveFixture::Storage store;assert(store.Commit(before));assert(store.Commit(before));
  int writeCalls=0;auto action=[&]{
   const auto ownerBefore=live;if(!Advance(live,kind,choice))return false;
   const char* event=Event(kind,choice);const unsigned deeds=kind==8?2:ActionDeedMask(event);
   const bool saved=CommitActionConsequences([&]{return live.memories;},[&]{if(*event){SaveFixture::Npc witness;witness.NpcId="npc.local";witness.Observations.push_back({{1000+kind},event,"existing.subject","",0});live.memories.push_back(witness);if(kind==25)live.memories.back().Observations.push_back({{2000+kind},"Reality.CareWitnessed","Reality.Realm.0","",0});}},[&]{return CommitActionDeeds(live.world.rites,deeds,[&]{++writeCalls;const auto r=live.Capture();assert((r.LegacyDeedBits&deeds)==deeds);assert(kind==8||r.NpcSnapshots.size()==2);return store.Commit(r);});},[&](const auto& prior){live.memories=prior;});
   if(!saved){
    // Native world owners do not roll back observer memory or automatic deed
    // credit themselves; those must already have unwound through the helper.
    const auto memories=live.memories;const auto chronicle=live.world.rites;
    live=ownerBefore;live.memories=memories;if(kind<22)live.world.rites=chronicle;
   }
   return saved;
  };
  // Explicit permanent choices still demand fresh confirmation; preview alone writes nothing.
  CommitmentGate gate;assert(gate.Attempt("existing.scope",choice,0)!=CommitmentAttempt::Confirmed);assert(writeCalls==0);assert(gate.Attempt("existing.scope",choice,1)==CommitmentAttempt::Confirmed);
  store.failAt=store.writes+fault;store.damageOnFailure=true;assert(!action());Same(before,live.Capture());assert(store.Load());Same(before,*store.Load());assert(writeCalls==1);
  assert(gate.Attempt("existing.scope",choice,2)!=CommitmentAttempt::Confirmed);assert(gate.Attempt("existing.scope",choice,3)==CommitmentAttempt::Confirmed);
  assert(action());const auto complete=live.Capture();assert(store.Load());Same(complete,*store.Load());assert(writeCalls==2);
  for(int repeat=0;repeat<12;++repeat){ActionState loaded;loaded.Load(*store.Load());Same(complete,loaded.Capture());++reloads;}
  const int writes=store.writes;
  if(kind!=22&&kind!=25)assert(!action());
  assert(store.writes==writes);Same(complete,live.Capture());++paths;
 }
 // A confirmed failed owner cannot fall through to another nearby family.
 for(const char* scope:{"Faction.0","Frontier.1","Bellwold.Afterlight","RealmAftermath.8","WitnessBraid.Crossings","WitnessReturn.HessaReply"}){
  int calls=0;assert(!ResolveConfirmedAction(scope,scope,[&](auto family){assert(family!=ConfirmedAction::None);++calls;return false;}));assert(calls==1);
  assert(!ResolveConfirmedAction(scope,"Faction.2",[&](auto){assert(false);return true;}));
  assert(ResolveConfirmedAction(scope,scope,[&](auto){++calls;return true;}));assert(calls==2);
 }
 for(const char* invalid:{"", "Faction.","Faction.3","Faction.01","Frontier.2","RealmAftermath.0","RealmAftermath.9","WitnessBraid.Crossings.extra"})assert(!ResolveConfirmedAction(invalid,invalid,[](auto){assert(false);return true;}));
 assert(!ResolveConfirmedAction(nullptr,"Faction.0",[](auto){assert(false);return true;}));
 // Repair older split aid commits using existing direct history. No replacement
 // event is appended, and failed repair restores relationship/deed state together.
 for(int fault=1;fault<=2;++fault){
  ActionState legacy;Prepare(legacy,0,1);SaveFixture::Npc witness;witness.NpcId=ResidentContinuityId(0);witness.Observations.push_back({{5000},"Player.Helped",ResidentContinuityId(0),"",0});legacy.memories.push_back(witness);
  SaveFixture::Storage disk;auto before=legacy.Capture();assert(disk.Commit(before));assert(disk.Commit(before));disk.failAt=disk.writes+fault;disk.damageOnFailure=true;
  auto repair=[&]{const auto prior=legacy.residents;assert(legacy.residents.Aid(ResidentContinuityId(0))==EncounterEvent::Aided);if(CommitActionDeeds(legacy.world.rites,1,[&]{return disk.Commit(legacy.Capture());}))return true;legacy.residents=prior;return false;};
  assert(!repair());Same(before,legacy.Capture());assert(disk.Load());assert(repair());assert(legacy.residents.Aided(ResidentContinuityId(0))&&legacy.memories.back().Observations.size()==1&&legacy.memories.back().Observations[0].EventId.value==5000);assert(legacy.world.rites.Snapshot().deeds==1);
 }
 // Earned rewards are reconciled from committed source facts. A failed grant
 // remains unclaimed; repeated checks/reloads never refill spent supplies.

 for(int fault=1;fault<=2;++fault){
  ActionState earned;Prepare(earned,1,1);assert(Advance(earned,1,1));SaveFixture::Storage disk;assert(disk.Commit(earned.Capture()));assert(disk.Commit(earned.Capture()));disk.failAt=disk.writes+fault;disk.damageOnFailure=true;
  auto reconcile=[&]{const Record facts=*disk.Load();bool complete=true;ForEachEarnedMilestone(facts,[](const char*){return false;},[&](auto reward){if(earned.inventory.HasClaimed(reward))return;const auto before=earned.inventory.Snapshot();if(earned.inventory.Claim(reward)!=RewardResult::Awarded){complete=false;return;}if(!disk.Commit(earned.Capture())){assert(earned.inventory.Restore(before));complete=false;}assert(disk.Load());});return complete;};
  assert(!reconcile());assert(reconcile());assert(earned.inventory.HasClaimed(Achievement::BellwoldLanterns));
  int health=0,strain=0;if(earned.inventory.Quantity(ItemId::HearthSalve)>0){assert(earned.inventory.Consume(ItemId::HearthSalve,80,0,health,strain)==ConsumeResult::Used);assert(disk.Commit(earned.Capture()));}
  const auto settled=earned.inventory.Snapshot();const int writes=disk.writes;
  for(int i=0;i<24;++i){ActionState loaded;loaded.Load(*disk.Load());earned=loaded;assert(reconcile());assert(earned.inventory.Snapshot().quantities==settled.quantities);}assert(disk.writes==writes);
 }
 // Capacity failure performs no write and cannot mark an ungranted reward.
 {InventoryModel full;auto snap=full.Snapshot();for(const auto& item:ItemCatalog)snap.quantities[static_cast<int>(item.id)]=item.maxStack;assert(full.Restore(snap));assert(full.Claim(Achievement::BellwoldLanterns)!=RewardResult::Awarded);assert(!full.HasClaimed(Achievement::BellwoldLanterns)&&full.Snapshot().quantities==snap.quantities);}
 // Full observer histories evicted during staging must be restored on failure.
 {SaveFixture::Array<SaveFixture::Observation> observations;for(int i=1;i<=64;++i)observations.push_back({{i},"Player.Helped","old","",0});assert(!CommitActionConsequences([&]{return observations;},[&]{observations.erase(observations.begin());observations.push_back({{65},"World.FactionResolved","new","",0});},[]{return false;},[&](const auto& before){observations=before;}));assert(observations.size()==64&&observations.front().EventId.value==1&&observations.back().EventId.value==64);}
 // A malformed later participant rejects the whole action before backup writes.
 ActionState live;Prepare(live,0,1);SaveFixture::Storage store;const auto initial=live.Capture();assert(store.Commit(initial));assert(store.Commit(initial));const int writes=store.writes;const auto before=live.world.rites;
 assert(!CommitActionDeeds(live.world.rites,1,[&]{auto bad=live.Capture();bad.NpcSnapshots[0].Observations[0].Evidence=99;return store.Commit(bad);}));assert(live.world.rites.Snapshot().deeds==before.Snapshot().deeds&&store.writes==writes);Same(initial,*store.Load());assert(!CommitActionDeeds(live.world.rites,8,[]{assert(false);return true;}));
 std::cout<<"PASS: "<<paths<<" action/consequence fault paths across 32 action cases, "<<reloads<<" reloads, fresh consent, single action writer, observer/deed rollback, legacy aid repair, earned reward faults and backup integrity\n";
}
