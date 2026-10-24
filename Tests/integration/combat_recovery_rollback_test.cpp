#include "Save/UnmadeRecoveryRules.h"
#include "Combat/UnmadeCombatRecoveryRules.h"
#include "../save/combined_record_store.h"
#include <cassert>
#include <iostream>
#include <limits>

using namespace UnmadeCore;
using SaveFixture::Record;
// A portable adapter of the native recovery API; all mutable combat models are
// production types. This does not execute actors, animation or platform IO.
struct Player {
 Combatant body;CombatTempo tempo;ContactWindow contact;
 CombatStatus status=CombatStatus::None;double staggerUntil=0,statusUntil=0;
 double GetHealth()const{return body.Health();}
 double GetStamina()const{return tempo.Stamina();}
 bool Heal(double amount){return body.Heal(amount);}
 bool RestoreSession(double health,double stamina){return body.RestoreHealth(health)&&tempo.RestoreEnergy(stamina);}
 CombatRecoveryState CaptureRecoveryState()const{return {body,tempo,contact,status,staggerUntil,statusUntil};}
 void RestoreRecoveryState(const CombatRecoveryState& before){body=before.body;tempo=before.tempo;contact=before.contact;status=before.status;staggerUntil=before.staggerUntil;statusUntil=before.statusUntil;}
 bool ReviveAtCheckpoint(){
  if(!body.ReviveAtCheckpoint(.75))return false;
  tempo.Reset();contact.Cancel();status=CombatStatus::None;staggerUntil=0;statusUntil=0;return true;
 }
 Record Capture()const{Record r;r.bHasPlayerResume=true;r.ResumeHealth=GetHealth();r.ResumeStamina=GetStamina();assert(SaveFixture::Valid(r));return r;}
 void Load(const Record& r){assert(RestoreSession(r.ResumeHealth,r.ResumeStamina));}
};
static Player Seed(int realm,int mode,bool dead){
 Player p;assert(p.body.RestoreHealth(40+realm));assert(p.body.AcknowledgeHit(7,5));
 const auto swing=p.body.TryAttack(10);assert(swing.outcome==AttackOutcome::Started);
 assert(p.contact.Begin(swing.swingId,10,RealmWeapon(realm).cooldown));
 p.status=CombatStatus::Burn;p.staggerUntil=11.75;p.statusUntil=14;
 if(mode==0)p.tempo.GrantEvasion(10,1);
 if(mode==1){assert(p.tempo.Guard(10,realm%4));assert(p.body.SetGuarding(true));}
 if(mode==2){assert(p.tempo.Attack(8.4,RealmWeapon(realm),false).started);assert(p.tempo.Attack(9.6,RealmWeapon(realm),false).combo==2);}
 if(mode==3)assert(p.tempo.Power(10));
 if(mode==4)assert(p.tempo.Attack(10,RealmWeapon(realm),false).started);
 if(mode==5){assert(p.tempo.Power(9.1));assert(p.tempo.Guard(10,3));assert(p.tempo.Attack(10,RealmWeapon(realm),false).started);p.tempo.GrantEvasion(10,1.2);}
 if(dead)assert(p.body.ReceiveHit(99,1,999,false)==HitOutcome::Applied);
 return p;
}
// Compare behavior at both sides of every relevant boundary. Copies keep these
// observations from spending the live parry, swing or stamina a second time.
static void SameRuntime(const Player& actual,const Player& before,int realm){
 assert(actual.GetHealth()==before.GetHealth()&&actual.GetStamina()==before.GetStamina());
 assert(actual.body.IsGuarding()==before.body.IsGuarding());
 assert(actual.status==before.status&&actual.staggerUntil==before.staggerUntil&&actual.statusUntil==before.statusUntil);
 assert(actual.contact.Id()==before.contact.Id());
 for(double now:{10.,10.01,10.11,10.19,10.21,10.27,10.32,10.44,10.46,10.61,10.76,10.96,11.,11.15,11.21,11.59,11.61,21.09,21.11,21.99,22.}){
  auto a=actual.tempo,b=before.tempo;assert(a.Defense(now,true)==b.Defense(now,true));assert(a.Stamina()==b.Stamina());
  a=actual.tempo;b=before.tempo;assert(a.Power(now)==b.Power(now));assert(a.Stamina()==b.Stamina());
  a=actual.tempo;b=before.tempo;const auto x=a.Attack(now,RealmWeapon(realm),false),y=b.Attack(now,RealmWeapon(realm),false);
  assert(x.started==y.started&&x.combo==y.combo&&x.multiplier==y.multiplier&&a.Stamina()==b.Stamina());
  a=actual.tempo;b=before.tempo;assert(a.Dodge(now,realm%4)==b.Dodge(now,realm%4));assert(a.Stamina()==b.Stamina());
  a=actual.tempo;b=before.tempo;assert(a.Guard(now,realm%4)==b.Guard(now,realm%4));assert(a.Stamina()==b.Stamina());
  a=actual.tempo;b=before.tempo;a.Tick(now,false,realm%4);b.Tick(now,false,realm%4);assert(a.Stamina()==b.Stamina());
  auto ac=actual.contact,bc=before.contact;assert(ac.Active(now)==bc.Active(now));assert(ac.Claim(29,now)==bc.Claim(29,now));
 }
 for(int source:{7,99})for(int pulse=1;pulse<=6;++pulse)assert(actual.body.DuplicateHit(source,pulse)==before.body.DuplicateHit(source,pulse));
 auto a=actual.body,b=before.body;const auto x=a.TryAttack(30),y=b.TryAttack(30);assert(x.outcome==y.outcome&&x.swingId==y.swingId&&x.baseDamage==y.baseDamage);
 assert(a.ReceiveHit(7,5,10,false)==b.ReceiveHit(7,5,10,false));assert(a.Health()==b.Health());
}
static void SameSaved(const Record& a,const Record& b){assert(a.ResumeHealth==b.ResumeHealth&&a.ResumeStamina==b.ResumeStamina&&a.bHasPlayerResume==b.bHasPlayerResume);}
static void FreshSession(const Player& restored,const Record& r){
 SameSaved(restored.Capture(),r);assert(restored.status==CombatStatus::None&&restored.staggerUntil==0&&restored.statusUntil==0&&!restored.contact.Active(10.2));
 assert(!restored.body.DuplicateHit(7,5));auto t=restored.tempo;assert(t.Defense(10.1,true)==0);
 if(r.ResumeStamina>=30){assert(t.Power(10.1));assert(!t.Power(10.2));}
}
int main(){int faults=0,reloads=0,malformed=0,successes=0;
 // Preserve the original focused red regression: failed healing must neither
 // erase purchased evasion nor allow immediate reuse of an expended power.
 {Player p;assert(p.body.RestoreHealth(40));p.tempo.GrantEvasion(1,1);assert(p.tempo.Power(1));
  assert(!CommitHealing(p,25,[]{return false;}));assert(p.GetHealth()==40&&p.GetStamina()==70);
  assert(p.tempo.Defense(1.1,false)==1);assert(!p.tempo.Power(1.1));}
 for(int realm=0;realm<9;++realm)for(int mode=0;mode<6;++mode)for(bool revive:{false,true})for(int fault=1;fault<=2;++fault){
  Player live=Seed(realm,mode,revive);const auto before=live;const auto initial=live.Capture();
  SaveFixture::Storage disk;assert(disk.Commit(initial));assert(disk.Commit(initial));
  const auto stage=[&]{return revive?live.ReviveAtCheckpoint():live.Heal(25);};
  const auto commit=[&]{return CommitCombatRecovery(live,stage,[&]{return disk.Commit(live.Capture());});};
  disk.failAt=disk.writes+fault;disk.damageOnFailure=true;
  assert(!commit());SameRuntime(live,before,realm);assert(disk.Load());SameSaved(*disk.Load(),initial);++faults;
  if(fault==2)assert(disk.recovery);else assert(!disk.recovery);
  // Recovery-selected backups stay intact across another damaged primary write.
  disk.failAt=disk.writes+1;assert(!commit());SameRuntime(live,before,realm);assert(disk.Load());SameSaved(*disk.Load(),initial);++faults;
  for(int i=0;i<8;++i){Player loaded;loaded.Load(*disk.Load());SameSaved(loaded.Capture(),initial);++reloads;}
  disk.failAt=0;assert(commit());assert(!disk.recovery);++successes;
  const auto saved=live.Capture();assert(saved.ResumeHealth==(revive?75:65+realm));
  assert(saved.ResumeStamina==(revive?100:initial.ResumeStamina));
  if(revive){assert(live.status==CombatStatus::None&&live.staggerUntil==0&&live.statusUntil==0&&!live.body.DuplicateHit(7,5));auto tempo=live.tempo;assert(tempo.Defense(10.1,true)==0);}
  else{auto expected=before;assert(expected.Heal(25));SameRuntime(live,expected,realm);}
  const auto writes=disk.writes;
  for(int i=0;i<16;++i){Player loaded;loaded.Load(*disk.Load());FreshSession(loaded,saved);++reloads;}
  assert(disk.writes==writes);
 }
 // A malformed present subrecord must reject the entire outgoing record before
 // rotating the good backup, and restore healing/revival runtime state as well.
 for(int realm=0;realm<9;++realm)for(int mode=0;mode<6;++mode)for(bool revive:{false,true}){
  Player live=Seed(realm,mode,revive);const auto before=live;SaveFixture::Storage disk;const auto initial=live.Capture();
  assert(disk.Commit(initial));assert(disk.Commit(initial));const auto writes=disk.writes;
  assert(!CommitCombatRecovery(live,[&]{return revive?live.ReviveAtCheckpoint():live.Heal(25);},[&]{
   auto bad=live.Capture();if(realm%3==0){bad.bHasAdventureDepth=true;bad.AdventureDepthState={1};}
   else if(realm%3==1){bad.bHasPlayerExperience=true;bad.PlayerExperienceState.clear();}
   else{bad.bHasExpeditions=true;bad.ExpeditionState={1};}
   return disk.Commit(bad);
  }));
  SameRuntime(live,before,realm);assert(disk.writes==writes&&disk.Load());SameSaved(*disk.Load(),initial);++malformed;
 }
 for(int mode=0;mode<6;++mode){Player p=Seed(0,mode,false);const auto before=p;int writes=0;
  assert(!CommitHealing(p,0,[&]{++writes;return false;}));SameRuntime(p,before,0);
  assert(CommitHealing(p,0,[&]{++writes;return true;}));SameRuntime(p,before,0);
  for(double bad:{-1.,std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity()})assert(!CommitHealing(p,bad,[&]{++writes;return true;}));
  assert(writes==2);SameRuntime(p,before,0);
  assert(!CommitCombatRecovery(p,[&]{p.body.RestoreHealth(1);p.tempo.Reset();p.contact.Cancel();p.status=CombatStatus::None;p.staggerUntil=0;p.statusUntil=0;return false;},[&]{++writes;return true;}));
  assert(writes==2);SameRuntime(p,before,0);
  assert(p.body.RestoreHealth(100));const auto full=p;assert(!CommitHealing(p,25,[&]{++writes;return true;}));assert(writes==2);SameRuntime(p,full,0);
  // A living character cannot revive, and rejection cannot touch its windows.
  assert(!CommitCombatRecovery(p,[&]{return p.ReviveAtCheckpoint();},[&]{++writes;return true;}));assert(writes==2);SameRuntime(p,full,0);
 }
 std::cout<<"combat recovery: "<<faults<<" failed writes, "<<malformed<<" rejected subrecords, "<<successes<<" recoveries and "<<reloads<<" reloads; active windows and receipts preserved\n";
}
