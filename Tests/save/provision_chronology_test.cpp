#include "combined_record_store.h"
#include "Save/UnmadeRecoveryRules.h"
#include "Combat/UnmadeCombatRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;

namespace {
struct Player {
 Combatant body;
 double GetHealth()const{return body.Health();}
 double GetStamina()const{return 40;}
 bool Heal(double amount){return body.Heal(amount);}
 bool RestoreSession(double health,double){return body.RestoreHealth(health);}
 Player CaptureRecoveryState()const{return *this;}
 void RestoreRecoveryState(const Player& before){*this=before;}
};
struct Story {
 Expeditions expedition;
 AdventureDepth depth;
 Player player;
 Story(int realm,int accord){
  assert(expedition.Brief(realm));assert(expedition.Visit(realm,0));
  for(int room=0;room<5;++room){assert(expedition.Enter(realm,room));if(room>0&&room<4)assert(expedition.Inspect(realm,room-1));}
  assert(expedition.Choose(realm,2));assert(expedition.Enter(realm,5));assert(expedition.Boss(realm,false));
  assert(depth.Offer(realm,true));assert(depth.Witness(realm));assert(depth.Evidence(realm,true));assert(depth.Choose(realm,accord,true));
  assert(depth.Route(realm,true));assert(depth.Secret(realm,true));if(accord==2)assert(depth.CounterThreat(realm));
 }
 SaveFixture::Record Record(int day)const{
  SaveFixture::Record r;r.bHasLivingWorldSnapshot=true;r.LivingWorldSeconds=(day-1)*1200.;
  r.bHasPlayerResume=true;r.ResumeHealth=static_cast<float>(player.GetHealth());r.ResumeStamina=40;
  r.bHasExpeditions=true;r.ExpeditionState=expedition.Pack();r.bHasAdventureDepth=true;r.AdventureDepthState=depth.Pack();return r;
 }
};
}
int main(){
 int malformed=0,reloads=0,faults=0;
 for(int realm=0;realm<9;++realm)for(int accord=1;accord<=2;++accord){
  Story story(realm,accord);const auto original=story.Record(1);assert(SaveFixture::Valid(original));
  // The last purchase day must accommodate every once-per-day purchase.
  for(int count=2;count<=4;++count)for(int lastDay=1;lastDay<count;++lastDay){
   auto bad=original;const int base=2+realm*12;
   bad.AdventureDepthState[base+6]-=count*story.depth.Price(realm,0);
   bad.AdventureDepthState[base+9]=count;bad.AdventureDepthState[base+10]=lastDay;
   bad.LivingWorldSeconds=(lastDay-1)*1200.;
   AdventureDepth restored=story.depth;const auto unchanged=restored.Pack();
   assert(!restored.Restore(bad.AdventureDepthState,story.expedition,lastDay));assert(restored.Pack()==unchanged);
   assert(!SaveFixture::Valid(bad));++malformed;
   SaveFixture::Storage disk;disk.primary=original;disk.backup=original;
   assert(!disk.Commit(bad));assert(disk.writes==0);assert(disk.primary->AdventureDepthState==original.AdventureDepthState);
   assert(disk.backup->AdventureDepthState==original.AdventureDepthState);
   disk.primary=bad;
   for(int repeat=0;repeat<24;++repeat){const auto* selected=disk.Load();assert(selected&&disk.recovery);assert(selected->AdventureDepthState==original.AdventureDepthState);assert(disk.primary->AdventureDepthState==bad.AdventureDepthState);++reloads;}
   // A failed recovery write never replaces the selected good backup.
   auto trial=disk;trial.writes=0;trial.failAt=1;trial.damageOnFailure=true;
   assert(!trial.Commit(original));assert(trial.writes==1);const auto* selected=trial.Load();assert(selected&&selected->AdventureDepthState==original.AdventureDepthState);assert(trial.backup->AdventureDepthState==original.AdventureDepthState);++faults;
   disk.backup=bad;assert(!disk.Load());
   disk.primary=original;assert(!disk.Load()); // Explicit recovery keeps a rejected backup unavailable.
   disk.recovery=false;assert(disk.Load()==&*disk.primary);
  }
  // Real older saves with early, late and skipped purchase days stay valid.
  for(int count=1;count<=4;++count){const int day=count*3;
   assert(story.player.body.RestoreHealth(50));const auto before=story.depth;
   const auto previous=story.Record(day);
   for(int failed=1;failed<=2;++failed){SaveFixture::Storage disk;disk.primary=previous;disk.backup=previous;disk.failAt=failed;disk.damageOnFailure=true;
    assert(story.depth.Buy(realm,0,day,true));
    assert(!CommitHealing(story.player,35,[&]{return disk.Commit(story.Record(day));}));
    story.depth=before;assert(story.player.GetHealth()==50);assert(story.depth.Pack()==previous.AdventureDepthState);
    const auto* recovered=disk.Load();assert(recovered&&recovered->AdventureDepthState==previous.AdventureDepthState&&recovered->ResumeHealth==50);++faults;
   }
   SaveFixture::Storage disk;disk.primary=previous;disk.backup=previous;
   assert(story.depth.Buy(realm,0,day,true));assert(!story.depth.Buy(realm,0,day,true));
   assert(CommitHealing(story.player,35,[&]{return disk.Commit(story.Record(day));}));assert(story.player.GetHealth()==85);
   auto valid=story.Record(day);assert(SaveFixture::Valid(valid));
   AdventureDepth restored;assert(restored.Restore(valid.AdventureDepthState,story.expedition,day));assert(restored.Pack()==story.depth.Pack());
   for(int repeat=0;repeat<16;++repeat){const auto* selected=disk.Load();assert(selected&&selected->AdventureDepthState==valid.AdventureDepthState&&selected->ResumeHealth==85);Player loaded;assert(loaded.RestoreSession(selected->ResumeHealth,selected->ResumeStamina));assert(loaded.GetHealth()==85);++reloads;}
  }
 }
 std::cout<<malformed<<" malformed provision histories, "<<faults<<" failed save/recovery writes and "<<reloads<<" reload checks passed\n";
}
