// Real timed transactions + combined record validation/recovery. In-memory faults only.
#include "World/UnmadeTimedProgressRules.h"
#include "Save/UnmadeProfileHandoffRules.h"
#include "NPC/UnmadeMemorySaveRules.h"
#include "../save/combined_record_store.h"
#include <cassert>
#include <iostream>
#include <limits>
using namespace UnmadeCore;
using SaveFixture::Record;
struct State {
 LivingWorldClock clock;SettlementVisits visits;
 MaterialWorld materials;LivingEconomy wages;BesHousehold bes;TenfoldChronicle rites;
};
static Record Capture(const State& s){
 Record r;r.bHasLivingWorldSnapshot=true;r.LivingWorldSeconds=s.clock.ElapsedSeconds();
 r.VisitedSettlementsMask=s.visits.Snapshot();r.bHasMaterialWorld=true;

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
 const auto t=s.rites.Snapshot();r.bHasTenfoldChronicle=true;
 r.RiteStages.assign(t.stage.begin(),t.stage.end());r.RiteChoices.assign(t.choice.begin(),t.choice.end());
 r.RiteReadyAt.assign(t.readyAt.begin(),t.readyAt.end());
 r.DisciplineMastery.assign(t.disciplineMastery.begin(),t.disciplineMastery.end());
 r.RiteTrialBits=t.trialUsed;r.LegacyDeedBits=t.deeds;r.VerifiedRoadBits=t.verifiedRoutes;
 r.TomorrowDebtDueDay=t.debtDueDay;r.TomorrowDebtExhaustedUntil=t.exhaustedUntilDay;
 r.BrokenOathCount=t.brokenOaths;r.bOathActive=t.oathActive;r.bOathRedeemed=t.oathRedeemed;
 assert(SaveFixture::Valid(r));return r;
}
static State Load(const Record& r){
 assert(SaveFixture::Valid(r));State s;
 assert(s.clock.Restore(r.LivingWorldSeconds));const int day=s.clock.DayIndex()+1;
 assert(s.visits.Restore(r.VisitedSettlementsMask));MaterialWorld::Towns towns{};

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
 TenfoldSnapshot t;
 for(int i=0;i<10;++i){t.stage[i]=r.RiteStages[i];t.choice[i]=r.RiteChoices[i];t.readyAt[i]=r.RiteReadyAt[i];}
 for(int i=0;i<6;++i)t.disciplineMastery[i]=r.DisciplineMastery[i];
 t.trialUsed=r.RiteTrialBits;t.deeds=r.LegacyDeedBits;t.verifiedRoutes=r.VerifiedRoadBits;
 t.debtDueDay=r.TomorrowDebtDueDay;t.exhaustedUntilDay=r.TomorrowDebtExhaustedUntil;
 t.brokenOaths=r.BrokenOathCount;t.oathActive=r.bOathActive;t.oathRedeemed=r.bOathRedeemed;
 assert(s.rites.Restore(t));return s;
}
static void Same(const Record& a,const Record& b){
 assert(a.BesColdDays==b.BesColdDays);
 assert(a.BesFirstReportDay==b.BesFirstReportDay);
 assert(a.BesHeatBaskets==b.BesHeatBaskets);
 assert(a.BesHouseholdDay==b.BesHouseholdDay);
 assert(a.BesMeraGuaranteeOwed==b.BesMeraGuaranteeOwed);
 assert(a.BesQuestPhase==b.BesQuestPhase);
 assert(a.BesReliefPacks==b.BesReliefPacks);
 assert(a.BesWalkoutDays==b.BesWalkoutDays);
 assert(a.BrokenOathCount==b.BrokenOathCount);
 assert(a.DisciplineMastery==b.DisciplineMastery);
 assert(a.LegacyDeedBits==b.LegacyDeedBits);
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
 assert(a.RiteChoices==b.RiteChoices);
 assert(a.RiteReadyAt==b.RiteReadyAt);
 assert(a.RiteStages==b.RiteStages);
 assert(a.RiteTrialBits==b.RiteTrialBits);
 assert(a.TomorrowDebtDueDay==b.TomorrowDebtDueDay);
 assert(a.TomorrowDebtExhaustedUntil==b.TomorrowDebtExhaustedUntil);
 assert(a.VerifiedRoadBits==b.VerifiedRoadBits);
 assert(a.VisitedSettlementsMask==b.VisitedSettlementsMask);
 assert(a.bBesHomeVisited==b.bBesHomeVisited);
 assert(a.bBesMeraConsented==b.bBesMeraConsented);
 assert(a.bBesMeraGuaranteeUsed==b.bBesMeraGuaranteeUsed);
 assert(a.bBesRhevaInterviewed==b.bBesRhevaInterviewed);
 assert(a.bHasBesHousehold==b.bHasBesHousehold);
 assert(a.bHasLivingEconomy==b.bHasLivingEconomy);
 assert(a.bHasLivingWorldSnapshot==b.bHasLivingWorldSnapshot);
 assert(a.bHasMaterialWorld==b.bHasMaterialWorld);
 assert(a.bHasTenfoldChronicle==b.bHasTenfoldChronicle);
 assert(a.bOathActive==b.bOathActive);
 assert(a.bOathRedeemed==b.bOathRedeemed);
}
static void Memories(){
 Record record;SaveFixture::Npc absent,live;
 absent.NpcId="npc.cinderhold.courier.001";absent.Trust=37;
 absent.Observations.push_back({{1},"Player.Helped","npc.cinderhold.courier.001","",0});
 live.NpcId="npc.saltwake.courier.001";live.Trust=12;
 record.NpcSnapshots.push_back(absent);
 assert(!ReplaceNpcMemory(record.NpcSnapshots,live));record.NpcSnapshots.push_back(live);
 for(int i=0;i<50;++i){
  live.Trust=13;assert(ReplaceNpcMemory(record.NpcSnapshots,live));
  assert(record.NpcSnapshots.size()==2&&record.NpcSnapshots[0].Trust==37);
  assert(record.NpcSnapshots[0].Observations[0].EventId.value==1);
 }
 SaveFixture::Storage store;store.primary=record;store.backup=record;
 live.Observations.push_back({{2},"Player.Helped","npc.cinderhold.courier.001","npc.cinderhold.courier.001",1});
 assert(ReplaceNpcMemory(record.NpcSnapshots,live));
 store.failAt=1;assert(!store.Commit(record));assert(store.Load()->NpcSnapshots[1].Observations.empty());
 // A quiet timer retries the same captured histories, without manufacturing a new rumor.
 store.failAt=0;assert(store.Commit(record));assert(store.Load()->NpcSnapshots[1].Observations.size()==1);
 assert(store.Load()->NpcSnapshots[1].Observations[0].Evidence==1);
 auto malformed=record;malformed.NpcSnapshots[1].Observations[0].EventId.value=0;
 const int writes=store.writes;assert(!store.Commit(malformed));assert(store.writes==writes);
 assert(store.Load()->NpcSnapshots[0].Trust==37);
}
int main(){
 Memories();
 std::array<RealityPublicResponse,9> civic{};int cases=0,reloads=0;
 // Source-world identity remains mandatory after a new profile finishes arriving.
 int worlds[4]={};
 for(int owner=0;owner<4;++owner)for(int source=0;source<4;++source)
  assert(SameSessionWorld(&worlds[owner],&worlds[source])==(owner==source));
 assert(!SameSessionWorld(nullptr,&worlds[0])&&!SameSessionWorld(&worlds[0],nullptr));
 ProfileHandoff handoff;assert(handoff.Begin({0,false},{1,true}));
 State frozen;
 for(int i=0;i<100;++i)assert(!AdvanceWorldClock(frozen.clock,!handoff.WritesBlocked(),1));
 assert(frozen.clock.ElapsedSeconds()==0);
 assert(handoff.Fail(true));assert(AdvanceWorldClock(frozen.clock,!handoff.WritesBlocked(),1));
 for(double invalid:{-1.,std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity()})
  assert(!AdvanceWorldClock(frozen.clock,true,invalid));
 for(int profile=0;profile<4;++profile)for(int recover=0;recover<2;++recover)
 for(int failure=1;failure<=2;++failure)for(int operation=0;operation<3;++operation)for(int history=0;history<4;++history){
  ++cases;State s;
  for(int i=0;i<9;++i)civic[i]=profile%3==0?RealityPublicResponse::SharedPassage:
   profile%3==1?RealityPublicResponse::PublicInquiry:RealityPublicResponse::ContestedAccord;
  int targetDay=2;
  if(history==1||history==2){
   for(int day=2;day<=18;++day){
    assert(s.materials.AdvanceAllOneDay(civic));assert(s.wages.AdvanceOneDay(day));assert(s.bes.AdvanceOneDay(day));
   }
   assert(s.bes.HearBes(s.wages,18)==BesOutcome::Reported);
   assert(s.bes.InspectHome(18)==BesOutcome::HomeInspected);
   assert(s.bes.HearRheva(18)==BesOutcome::LedgerCorroborated);
   assert(s.bes.BeginWalkout(s.wages,18)==BesOutcome::WalkoutBegun);
   if(history==2){
    assert(s.bes.OfferMeraGuarantee(18)==BesOutcome::ConsentRecorded);
    assert(s.bes.AcceptMeraGuarantee(s.wages,18)==BesOutcome::GuaranteePaid);
   }
   targetDay=19;
  }
  if(history==3){
   assert(s.materials.AdvanceAllOneDay(civic));assert(s.wages.AdvanceOneDay(2));assert(s.bes.AdvanceOneDay(2));
   assert(s.wages.Collect(TradeRoute::SaltToHearth,2,s.materials)==TradeResult::Collected);
   assert(s.wages.Deliver(TradeRoute::SaltToHearth,2,s.materials)==TradeResult::Delivered);
   auto towns=s.materials.Snapshot();towns[2].households=0;
   assert(s.materials.Restore(towns,s.materials.Cargo(),2));targetDay=3;
  }
  assert(s.clock.Restore(850+(targetDay-2)*1200));assert(s.clock.DayIndex()+1==targetDay);
  auto debt=s.rites.Snapshot();debt.debtDueDay=targetDay;assert(s.rites.Restore(debt));
  const Record before=Capture(s);SaveFixture::Storage store;
  store.primary=before;store.backup=before;store.recovery=recover;
  store.failAt=recover?1:failure;store.damageOnFailure=true;
  TimedProgressRetry retry;int calls=0;
  auto attempt=[&](bool ready,double now, bool malformed=false){
   auto writer=[&]{++calls;auto r=Capture(s);
    if(malformed){r.bHasFieldSurveys=true;r.FieldSurveyStages.clear();}
    return store.Commit(r);
   };
   if(operation==0)return CommitSettlementVisit(s.visits,static_cast<SettlementId>(profile%3),ready,now,retry,writer);
   if(operation==1)return CommitMaterialDay(s.materials,s.wages,s.bes,civic,targetDay,ready,now,retry,writer);
   return CommitRiteDay(s.rites,targetDay,ready,now,retry,writer);
  };
  assert(attempt(false,0)==TimedProgress::Deferred);Same(before,Capture(s));assert(calls==0);
  assert(attempt(true,0)==TimedProgress::FirstFailure);Same(before,Capture(s));
  assert(calls==1&&store.Load());Same(before,*store.Load());
  assert(attempt(true,4.999)==TimedProgress::Deferred);assert(calls==1);
  store.failAt=0;const int writes=store.writes;
  // Late malformed owner rejects the outgoing record before either target is touched.
  assert(attempt(true,5,true)==TimedProgress::FailedAgain);
  assert(store.writes==writes);Same(before,Capture(s));Same(before,*store.Load());
  assert(attempt(true,10)==TimedProgress::Saved);const auto saved=Capture(s);Same(saved,*store.Load());
  if(operation==0)assert(s.visits.Count()==1);
  if(operation==1){
   assert(s.materials.Town(Realm::ThreefoldReach)->day==targetDay);assert(s.bes.Save().day==targetDay);
   if(history==1){
    assert(s.wages.Worker(Realm::HearthBeneath)->owedMarks==before.LivingOwedMarks[2]);
    assert(s.wages.Worker(Realm::HearthBeneath)->shiftsPaid==before.LivingPaidShifts[2]);
    assert(s.bes.Save().daysWalkedOut==before.BesWalkoutDays+1);
   }
   if(history==3){
    assert(s.wages.Route(TradeRoute::SaltToHearth)->foreignStock==before.LivingForeignStock[0]-1);
    assert(s.wages.Route(TradeRoute::SaltToHearth)->consumedSupport==before.LivingConsumedSupport[0]+1);
   }
  }
  assert(s.bes.Save().meraConsented==(history==2));
  if(operation==2){assert(s.rites.Snapshot().debtDueDay==0);assert(s.rites.Snapshot().exhaustedUntilDay==targetDay+1);}
  const int settledCalls=calls;
  for(int i=0;i<25;++i){
   s=Load(*store.Load());++reloads;Same(saved,Capture(s));
   retry=TimedProgressRetry{}; // Reload resets cadence; model receipts still suppress duplicates.
   assert(attempt(true,20+i)==TimedProgress::Unchanged);assert(calls==settledCalls);
  }
 }
 // Multiple lagging days catch up one authored production/payroll day at a time.
 State lag;assert(lag.clock.Restore(5650));const int day=lag.clock.DayIndex()+1;
 TimedProgressRetry retry;int writes=0;
 while(lag.materials.Town(Realm::ThreefoldReach)->day<day){
  const int previous=lag.materials.Town(Realm::ThreefoldReach)->day;
  assert(CommitMaterialDay(lag.materials,lag.wages,lag.bes,civic,day,true,writes,retry,[&]{++writes;return true;})==TimedProgress::Saved);
  assert(lag.materials.Town(Realm::ThreefoldReach)->day==previous+1);
  assert(lag.wages.Worker(Realm::ThreefoldReach)->lastPaidDay==previous+1);
  assert(lag.bes.Save().day==previous+1);
 }
 State legacy;assert(legacy.materials.InitializeAtDay(day));assert(legacy.wages.InitializeAtDay(day));assert(legacy.bes.InitializeAtDay(day));
 assert(CommitMaterialDay(legacy.materials,legacy.wages,legacy.bes,civic,day,true,0,retry,[]{assert(false);return false;})==TimedProgress::Unchanged);
 TimedProgressRetry outage;int attempts=0;
 for(int tick=0;tick<=600;++tick)outage.Attempt(true,tick/10.,[&]{++attempts;return false;});
 assert(attempts==13);
 std::cout<<"PASS: "<<cases<<" timed fault/recovery paths; "<<reloads<<" reloads; source-world isolation, blocked clock, late validation, bounded retries and lagging days\n";
}
