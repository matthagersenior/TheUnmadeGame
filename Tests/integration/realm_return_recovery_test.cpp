#include "../save/combined_record_store.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main(){
 int uses=0,returns=0;
 for(int source=0;source<9;++source){
  Expeditions e;AdventureDepth d;
  assert(e.Brief(source));assert(e.Visit(source,0));
  for(int r=0;r<5;++r){assert(e.Enter(source,r));if(r>0&&r<4)assert(e.Inspect(source,r-1));}
  assert(e.Choose(source,2));assert(e.Enter(source,5));assert(e.Boss(source,false));
  assert(d.Offer(source,true));assert(d.Witness(source));assert(d.Evidence(source,true));assert(d.Choose(source,1,true));assert(d.Buy(source,1,1,true));
  for(int target=0;target<9;++target){
   auto depth=d;SaveFixture::Record record;record.bHasLivingWorldSnapshot=true;
   record.bHasExpeditions=true;record.ExpeditionState=e.Pack();
   record.bHasAdventureDepth=true;record.AdventureDepthState=depth.Pack();
   SaveFixture::Storage store;assert(store.Commit(record));
   auto save=[&]{record.AdventureDepthState=depth.Pack();return store.Commit(record);};
   int supplier=-1;const auto before=depth.Pack();int calls=0;
   assert(CommitFieldKit(depth,target,false,[&]{++calls;return true;},supplier)==FieldKitResult::Unavailable);assert(calls==0&&depth.Pack()==before);
   for(int fault=1;fault<=2;++fault){
    auto damaged=store;damaged.failAt=damaged.writes+fault;damaged.damageOnFailure=true;
    assert(CommitFieldKit(depth,target,true,[&]{record.AdventureDepthState=depth.Pack();return damaged.Commit(record);},supplier)==FieldKitResult::SaveFailed);
    assert(depth.Pack()==before&&supplier==-1);assert(damaged.Load());
    AdventureDepth recovered;assert(recovered.Restore(damaged.Load()->AdventureDepthState,e,1));assert(recovered.Pack()==before);
   }
   assert(CommitFieldKit(depth,target,true,save,supplier)==FieldKitResult::Used);assert(supplier==source);assert(depth.Arc(source)->kits==0);assert(depth.Arc(source)->favor==d.Arc(source)->favor);
   for(int repeat=0;repeat<3;++repeat){assert(store.Load());assert(depth.Restore(store.Load()->AdventureDepthState,e,1));assert(CommitFieldKit(depth,target,true,save,supplier)==FieldKitResult::Unavailable);}
   ++uses;
  }
 }
 for(int realm=0;realm<9;++realm)for(int ending=1;ending<=2;++ending){
  RealmAftermathChronicle aftermath;ResidentContinuity residents;SaveFixture::Storage store;
  const auto r=static_cast<Realm>(realm);const auto& spec=RealmAftermathSpecs[realm];
  const char* person=ResidentContinuityId(realm); // Individual encounter ledger, not inferred witness knowledge.
  auto checkpoint=[&]{
   SaveFixture::Record record;record.bHasLivingWorldSnapshot=true;const auto a=aftermath.Snapshot();const auto n=residents.Snapshot();
   record.bHasRealmAftermathSnapshot=true;record.RealmAftermathStages.assign(a.stage.begin(),a.stage.end());record.RealmAftermathPrepared.assign(a.prepared.begin(),a.prepared.end());record.RealmAftermathApproaches.assign(a.approach.begin(),a.approach.end());record.RealmAftermathEndings.assign(a.ending.begin(),a.ending.end());
   record.bHasResidentContinuitySnapshot=true;record.ResidentEncounterVisits.assign(n.visits.begin(),n.visits.end());record.ResidentEncounterLastDays.assign(n.lastDay.begin(),n.lastDay.end());record.ResidentEncounterAidFlags.assign(n.aids.begin(),n.aids.end());
   assert(store.Commit(record));
   for(int repeat=0;repeat<3;++repeat){const auto* loaded=store.Load();assert(loaded&&SaveFixture::Valid(*loaded));RealmAftermathSnapshot back;ResidentContinuitySnapshot neighbors;
    for(int i=0;i<9;++i){back.stage[i]=loaded->RealmAftermathStages[i];back.prepared[i]=loaded->RealmAftermathPrepared[i];back.approach[i]=loaded->RealmAftermathApproaches[i];back.ending[i]=loaded->RealmAftermathEndings[i];}
    for(int i=0;i<ResidentContinuityCount;++i){neighbors.visits[i]=loaded->ResidentEncounterVisits[i];neighbors.lastDay[i]=loaded->ResidentEncounterLastDays[i];neighbors.aids[i]=loaded->ResidentEncounterAidFlags[i];}
    assert(aftermath.Restore(back)&&residents.Restore(neighbors));assert(back.stage==a.stage&&back.ending==a.ending&&neighbors.visits==n.visits);
   }
   auto malformed=record;malformed.ResidentEncounterVisits.back()=6;auto old=store.primary;const int writes=store.writes;assert(!store.Commit(malformed)&&store.writes==writes);assert(store.primary->RealmAftermathStages==old->RealmAftermathStages);
  };
  assert(residents.Talk(person,1)==EncounterEvent::FirstHello);checkpoint();
  assert(aftermath.Begin(r,true,true,spec.initiatingWitness)==RealmAftermathResult::Started);checkpoint();
  assert(aftermath.Prepare(r,spec.mechanismSite)==RealmAftermathResult::Prepared);checkpoint();
  assert(aftermath.Inspect(r,ending,ending==1?spec.evidenceForCare:spec.evidenceForTruth)==RealmAftermathResult::EvidenceFound);checkpoint();
  assert(aftermath.Testify(r,ending==1?spec.careWitness:spec.truthWitness)==RealmAftermathResult::Witnessed);checkpoint();
  assert(aftermath.Decide(r,ending)==RealmAftermathResult::Resolved);checkpoint();
  assert(residents.Talk(person,1)==EncounterEvent::SameDay);assert(residents.Talk(person,2)==EncounterEvent::ReturnHello);checkpoint();
  assert(aftermath.Decide(r,3-ending)==RealmAftermathResult::NoChange);assert(aftermath.Ending(r)==ending);assert(residents.Visits(person)==2);assert(!residents.Aided(person));++returns;
 }
 std::cout<<"PASS: "<<uses<<" source/target field-kit transactions; "<<returns<<" aftermath endings with individual resident returns and combined reloads\n";
}
