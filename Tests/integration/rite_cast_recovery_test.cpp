#include "rite_cast_fixture.h"
#include <iostream>
using namespace RiteFixture;
int main(){int cases=0,reloads=0,resonance=0;
 for(int realm=0;realm<9;++realm)for(int rite=0;rite<10;++rite)for(int mastered=0;mastered<2;++mastered)
 for(int witnessed=0;witnessed<2;++witnessed)for(int recovery=0;recovery<2;++recovery)for(int failure=1;failure<=(recovery?1:2);++failure){
  State s;const auto id=static_cast<RiteId>(rite);const auto place=static_cast<Realm>(realm);
  if(mastered){MasterAll(s);assert(s.trials.Discover(ConfluenceId::SilentAlarm,0,1023,3)==ConfluenceResult::Advanced);}else{auto t=s.rites.Snapshot();t.stage[rite]=2;assert(s.rites.Restore(t));}
  const auto before=s.Capture();SaveFixture::Storage store;store.primary=before;store.backup=before;store.recovery=recovery;store.failAt=failure;store.damageOnFailure=true;int writes=0;
  auto save=[&]{++writes;return store.Commit(s.Capture());};
  assert(!Cast(s,place,id,witnessed,true,false,save));assert(writes==1);Same(before,s.Capture());Same(before,*store.Load());
  store.failAt=0;assert(Cast(s,place,id,witnessed,true,false,save));assert(writes==2);
  const auto saved=s.Capture();Same(saved,*store.Load());assert(saved.PlayerStrain>before.PlayerStrain);
  const auto care=realm==0?saved.RealityCareWitnessMask:saved.OuterCareMasks[realm-1];
  const auto inquiry=realm==0?saved.RealityInquiryWitnessMask:saved.OuterInquiryMasks[realm-1];
  assert((care||inquiry)==(mastered&&witnessed));
  for(int repeat=0;repeat<3;++repeat){State back;back.Load(*store.Load());Same(saved,back.Capture());++reloads;RiteContext c;c.site=rite;c.now=s.now;c.day=2;c.verifiedWitnesses=3;c.strain=back.fracture.CurrentStrain();assert(back.rites.Invoke(id,c).result!=RiteResult::Applied);}
  // Invalid late subrecords reject the whole captured candidate before any target writes.
  auto malformed=saved;malformed.ResidentEncounterVisits.back()=6;const int prior=store.writes;assert(!store.Commit(malformed));assert(store.writes==prior);Same(saved,*store.Load());++cases;
 }
 for(int i=0;i<6;++i)for(int order=0;order<2;++order)for(int expired=0;expired<2;++expired){
  State s;MasterAll(s);CompleteEcho(s,i);const auto& spec=RealmResonances[i];SaveFixture::Storage store;store.primary=s.Capture();
  const auto first=order?spec.second:spec.first,second=order?spec.first:spec.second;
  assert(Cast(s,spec.realm,first,true,true,true,[&]{return store.Commit(s.Capture());}));assert(s.chapters.resonance.Stage(spec.realm)==1);
  s.now+=expired?121:10;const auto before=s.Capture();const auto stable=*store.Load();
  store.failAt=store.writes+1;assert(!Cast(s,spec.realm,second,true,true,true,[&]{return store.Commit(s.Capture());}));Same(before,s.Capture());Same(stable,*store.Load());
  store.failAt=0;assert(Cast(s,spec.realm,second,true,true,true,[&]{return store.Commit(s.Capture());}));assert(s.chapters.resonance.Stage(spec.realm)==(expired?1:2));
  State loaded;loaded.Load(*store.Load());Same(s.Capture(),loaded.Capture());++resonance;
 }
 State outside;MasterAll(outside);assert(Cast(outside,Realm::ThreefoldReach,RiteId::Cartography,true,false,false,[]{return true;}));assert(outside.civic.Snapshot().activeRite==-1);
 std::cout<<"PASS: "<<cases<<" nine-realm/ten-rite fault cases; "<<reloads<<" reloads; "<<resonance<<" paired/expired resonance paths; optional focus and witness gates retained\n";
}
