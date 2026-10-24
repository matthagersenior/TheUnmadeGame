#include "Save/UnmadeServiceCommitRules.h"
#include "Save/UnmadeActionCommitRules.h"
#include "../save/combined_record_store.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
struct Session {
 Expeditions e;AdventureDepth d;PlayerExperience x;LivingWorldClock clock;
 SaveFixture::Record Capture()const{
  SaveFixture::Record r;r.bHasLivingWorldSnapshot=true;r.LivingWorldSeconds=clock.ElapsedSeconds();
  r.bHasExpeditions=true;r.ExpeditionState=e.Pack();r.bHasAdventureDepth=true;r.AdventureDepthState=d.Pack();
  r.bHasPlayerExperience=true;r.PlayerExperienceState=x.Pack();assert(SaveFixture::Valid(r));return r;
 }
 void Restore(const SaveFixture::Record& r){
  assert(e.Restore(r.ExpeditionState));assert(clock.Restore(r.LivingWorldSeconds));
  const int day=1+static_cast<int>(r.LivingWorldSeconds/1200);
  assert(d.Restore(r.AdventureDepthState,e,day));assert(x.Restore(r.PlayerExperienceState,e,d,day));
 }
 void Prepare(int a){
  assert(e.Brief(a));assert(e.Visit(a,0));
  for(int room=0;room<5;++room){assert(e.Enter(a,room));if(room<4)assert(e.Kill(a,room));if(room>0&&room<4)assert(e.Inspect(a,room-1));}
  for(int k=4;k<8;++k)assert(e.Kill(a,k));
  assert(e.Rescue(a,0));assert(e.Rescue(a,1));assert(e.Choose(a,1));assert(e.Enter(a,5));assert(e.Boss(a,true));
  assert(d.Offer(a,true));assert(d.Witness(a));assert(d.Evidence(a,true));
 }
};
int main(){
 int faults=0,reloads=0;
 auto reload=[&](SaveFixture::Storage& disk,const Session& expected){
  for(int n=0;n<12;++n){const auto* r=disk.Load();assert(r);Session restored;restored.Restore(*r);
   assert(restored.d.Pack()==expected.d.Pack());assert(restored.x.Pack()==expected.x.Pack());
   assert(restored.clock.ElapsedSeconds()==expected.clock.ElapsedSeconds());++reloads;}
 };
 for(int realm=0;realm<9;++realm)for(int choice=1;choice<=2;++choice){
  Session initial;initial.Prepare(realm);
  for(int fail=0;fail<=2;++fail){
   auto live=initial;SaveFixture::Storage disk;disk.primary=initial.Capture();disk.backup=disk.primary;
   disk.failAt=fail;disk.damageOnFailure=true;ScopedServiceConfirmation consent;
   assert(!consent.Confirm(realm,choice,1));assert(consent.Confirm(realm,choice,2));
   const bool saved=CommitActionConsequences([&]{return live;},[&]{assert(StagePersonalAccord(live.d,live.x,realm,choice,true));},[&]{return disk.Commit(live.Capture());},[&](const auto& old){live=old;});
   assert(saved==(fail==0));assert(!consent.Confirm(realm,choice,3));
   if(fail){++faults;assert(live.d.Pack()==initial.d.Pack());assert(live.x.Pack()==initial.x.Pack());}
   else {assert(live.d.Arc(realm)->choice==choice);assert(live.x.Tutorial()&16);assert(!StagePersonalAccord(live.d,live.x,realm,choice,true));}
   reload(disk,live);
   // A valid-looking earlier subrecord cannot hide malformed later mastery.
   auto bad=live.Capture();bad.PlayerExperienceState.pop_back();auto old=*disk.Load();
   assert(!disk.Commit(bad));assert(disk.Load()->AdventureDepthState==old.AdventureDepthState);
   disk.primary=bad;disk.backup=live.Capture();reload(disk,live);
  }
 }
 // A guide's finite use and its persistent tracked objective share one write.
 Session guide;guide.Prepare(0);assert(StagePersonalAccord(guide.d,guide.x,0,1,true));
 assert(guide.x.OfferWing(0,guide.e));assert(guide.x.Visit(0,4,1));
 for(int k=0;k<3;++k)assert(guide.x.WingStep(0,k,true));
 assert(guide.x.Guardian(0));assert(guide.x.CompleteWing(0));assert(guide.x.Upgrade(0,6));
 for(int fail=0;fail<=2;++fail){auto live=guide;SaveFixture::Storage disk;disk.primary=live.Capture();disk.backup=disk.primary;disk.failAt=fail;disk.damageOnFailure=true;
  bool ok=CommitActionConsequences([&]{return live.x;},[&]{assert(StageGuideService(live.x,0,1,live.e));},[&]{return disk.Commit(live.Capture());},[&](const auto& old){live.x=old;});
  assert(ok==(fail==0));if(fail)++faults;else {auto before=live.x.Pack();assert(!StageGuideService(live.x,0,2,live.e));assert(live.x.Pack()==before);}
  reload(disk,live);
 }
 // Every realm's repair spend and shared-access mediation survive failure/reload.
 for(int realm=0;realm<9;++realm)for(int mediation=0;mediation<2;++mediation){
  Session initial;initial.Prepare(realm);assert(StagePersonalAccord(initial.d,initial.x,realm,2,true));
  assert(initial.x.OfferWing(realm,initial.e));assert(initial.x.Visit(realm,4,1));
  for(int k=0;k<3;++k)assert(initial.x.WingStep(realm,k,true));
  assert(initial.x.Guardian(realm));assert(initial.x.CompleteWing(realm));
  if(mediation){assert(initial.x.Repair(realm,initial.d));assert(initial.x.Visit(realm,3,1));assert(initial.x.Visit(realm,3,2));assert(initial.clock.Restore(1200));}
  for(int fail=0;fail<=2;++fail){auto live=initial;SaveFixture::Storage disk;disk.primary=live.Capture();disk.backup=disk.primary;disk.failAt=fail;disk.damageOnFailure=true;
   bool ok=CommitActionConsequences([&]{return live.x;},[&]{assert(mediation?live.x.Reconcile(realm,live.d):live.x.Repair(realm,live.d));},[&]{return disk.Commit(live.Capture());},[&](const auto& old){live.x=old;});
   assert(ok==(fail==0));if(fail){++faults;assert(live.x.Pack()==initial.x.Pack());}
   else {assert(live.d.Arc(realm)->choice==2);assert(!(mediation?live.x.Reconcile(realm,live.d):live.x.Repair(realm,live.d)));}
   reload(disk,live);
  }
 }
 // Consumed scoped rest attempts cannot reapply failed world time without preview.
 for(int realm=0;realm<9;++realm)for(int fail=0;fail<=2;++fail){Session live;live.Prepare(realm);auto before=live;
  SaveFixture::Storage disk;disk.primary=live.Capture();disk.backup=disk.primary;disk.failAt=fail;disk.damageOnFailure=true;
  ScopedServiceConfirmation rest;assert(!rest.Confirm(realm*5,11,0));assert(!rest.Confirm(realm*5+1,11,1));assert(rest.Confirm(realm*5+1,11,2));
  bool ok=CommitActionConsequences([&]{return live.clock;},[&]{for(int i=0;i<300;++i)assert(live.clock.Advance(1));},[&]{return disk.Commit(live.Capture());},[&](const auto& old){live.clock=old;});
  assert(ok==(fail==0));if(fail){++faults;assert(live.clock.ElapsedSeconds()==before.clock.ElapsedSeconds());}else assert(live.clock.ElapsedSeconds()==300);
  assert(!rest.Confirm(realm*5+1,11,3));reload(disk,live);
 }
 std::cout<<"service/mastery: "<<faults<<" write faults, "<<reloads<<" reloads\n";
}
