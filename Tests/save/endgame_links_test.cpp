#include "combined_record_store.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
static SaveFixture::Record Base(){
 SaveFixture::Record r;r.bHasLivingWorldSnapshot=true;
 r.bHasLaterRealmSnapshot=true;r.LaterRealmStages.assign(6,0);r.LaterRealmChoices.assign(6,0);
 r.bHasFinalEncounterSnapshot=true;r.FinalEncounterStage=1;
 return r;
}
int main(){
 auto r=Base();assert(!SaveFixture::Valid(r));
 r.bHasLaterRealmSnapshot=false;assert(SaveFixture::Valid(r));
 for(int act=0;act<=5;++act){auto final=Base();final.FinalEncounterStage=act;final.FinalMorningChoice=act>=3?1:0;
  assert(SaveFixture::Valid(final)==(act==0));final.LaterVisitedMask=32;final.LaterRealmStages[5]=3;final.LaterRealmChoices[5]=2;assert(SaveFixture::Valid(final));
 }

 for(int a=0;a<6;++a)for(int order=1;order<=2;++order)for(int stage=1;stage<=2;++stage){
  SaveFixture::Record record;record.bHasLivingWorldSnapshot=true;
  record.bHasRealmResonanceSnapshot=true;record.RealmResonanceStages.assign(6,0);record.RealmResonanceStages[a]=stage;
  record.RealmResonanceFirstCasts.assign(6,0);record.RealmResonanceFirstCasts[a]=order;
  record.RealmResonanceDeadlines.assign(6,0);record.RealmResonanceDeadlines[a]=stage==1?120:0;
  assert(SaveFixture::Valid(record)); // Older optional rite section absent.
  record.bHasTenfoldChronicle=true;record.RiteStages.assign(10,0);record.RiteChoices.assign(10,0);record.RiteReadyAt.assign(10,0);record.DisciplineMastery.assign(6,0);
  const auto& spec=RealmResonances[a];
  auto master=[&](RiteId id){const int i=static_cast<int>(id);record.RiteStages[i]=5;record.RiteChoices[i]=1;record.RiteTrialBits|=1<<i;};
  assert(!SaveFixture::Valid(record));
  master(order==1?spec.first:spec.second);
  assert(SaveFixture::Valid(record)==(stage==1));
  master(order==1?spec.second:spec.first);assert(SaveFixture::Valid(record));
  if(stage==1){
   auto future=record;future.RealmResonanceDeadlines[a]=121;assert(!SaveFixture::Valid(future));
   auto expired=record;expired.LivingWorldSeconds=121;assert(SaveFixture::Valid(expired));
   ResonanceSnapshot state;state.stage[a]=1;state.firstCast[a]=order;state.deadline[a]=120;
   RealmResonanceJourney restarted;assert(restarted.Restore(state));
   assert(restarted.Record(spec.realm,order==1?spec.second:spec.first,121,true,true)==ResonanceResult::ExpiredRestarted);
   expired.RealmResonanceFirstCasts[a]=3-order;expired.RealmResonanceDeadlines[a]=241;assert(SaveFixture::Valid(expired));
   SaveFixture::Storage reopened;assert(reopened.Commit(expired));assert(reopened.Load());assert(reopened.Load()->RealmResonanceDeadlines[a]==241);
   assert(restarted.Record(spec.realm,order==1?spec.first:spec.second,240,true,true)==ResonanceResult::Opened);
  }

  SaveFixture::Storage store;assert(store.Commit(record));
  auto bad=record;bad.RiteStages[static_cast<int>(spec.first)]=4;
  if(stage==1&&order==2){ // The other, not-yet-cast rite need not be mastered yet.
   assert(SaveFixture::Valid(bad));
  }else assert(!SaveFixture::Valid(bad));
 }
 std::cout<<"PASS: final Auvren gate and 24 resonance/mastery/optional-record combinations\n";
}
