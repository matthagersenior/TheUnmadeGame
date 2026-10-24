#include "Combat/UnmadeEncounterInterruptionRules.h"
#include "Combat/UnmadeCombatRules.h"
#include "Combat/UnmadeCreatureRules.h"
#include "Save/UnmadeDefeatSaveRetryRules.h"
#include <cassert>
#include <iostream>
#include <limits>
using namespace UnmadeCore;

namespace {
// A portable adapter supplies the native component's public damage operations;
// actual outgoing/incoming attacks, cooldown and dedup use production Combatant.
struct StrikeAdapter {
 Combatant state;
 int gearAttack=50,gearArmor=50;
 explicit StrikeAdapter(double phaseBase):state(330,phaseBase,.4){}
 bool SetAttackDamage(double damage){return state.SetDamage(damage);}
 void SetGearBonuses(int attack,int armor){gearAttack=attack;gearArmor=armor;}
};
struct Warning {
 bool shown=true;double aim=99;int clears=0;
 void Clear(){shown=false;aim=0;++clears;}
};
template<class Pattern>
void MissingPlayer(Pattern& pattern,Warning& cue,int reason){
 const bool present=reason!=0,alive=reason!=1,accepted=reason!=2;
 for(int tick=0;tick<8;++tick){
  assert(!RequireEncounterPlayer(present&&alive&&accepted,[&]{InterruptEncounter(pattern);cue.Clear();}));
 }
 assert(!cue.shown&&cue.aim==0&&cue.clears==8);
 assert(RequireEncounterPlayer(true,[&]{InterruptEncounter(pattern);cue.Clear();}));
 assert(cue.clears==8);
}
}

int main(){
 int resumed=0,damageChecks=0,receiptRetries=0;
 // Normal creatures, champions and the Index observer share scalar windups.
 for(int kind=0;kind<12;++kind)for(bool champion:{false,true})for(int reason=0;reason<3;++reason){
  const auto profile=CreatureStats(static_cast<CreatureKind>(kind),champion);
  double windup=1+profile.windup,next=3,recovery=4;Warning cue;
  for(int tick=0;tick<8;++tick){
   assert(!RequireEncounterPlayer(false,[&]{InterruptAttackWindup(windup,next,recovery);cue.Clear();}));
  }
  assert(windup==-1&&next==0&&recovery==0&&!cue.shown&&cue.aim==0);
  const double resume=50+reason;windup=resume+profile.windup;
  assert(windup>resume&&resume+profile.windup*.99<windup);++resumed;
 }
 for(int reason=0;reason<3;++reason){
  double windup=2.5,next=4.5,recovery=3;Warning cue;
  assert(!RequireEncounterPlayer(false,[&]{InterruptAttackWindup(windup,next,recovery);cue.Clear();}));
  assert(windup==-1&&next==0&&recovery==0);windup=50+1.5;
  assert(windup==51.5&&!cue.shown);++resumed;
 }
 // All three named bosses retain their identity and every health phase.
 for(int id=0;id<3;++id)for(double health:{1.,.6,.25})for(int reason=0;reason<3;++reason){
  BossEncounter pattern(static_cast<BossId>(id));const auto* spec=FindBoss(pattern.Kind());
  const double distance=spec->rangeCm-5;
  assert(pattern.Advance(1,health,distance,false).action==BossAction::Telegraph);
  Warning cue;MissingPlayer(pattern,cue,reason);
  assert(pattern.Kind()==static_cast<BossId>(id));
  const double resume=50;assert(pattern.Advance(resume,health,distance,false).action==BossAction::Telegraph);
  const double duration=spec->windupSeconds*(health<=.3?.7:1.);
  assert(pattern.Advance(resume+duration*.99,health,distance,false).action==BossAction::Telegraph);
  const auto hit=pattern.Advance(resume+duration+.001,health,distance,false);
  assert(hit.action==(id==0?BossAction::Shockwave:BossAction::Strike));++resumed;
 }
 // Six optional guardians retain their realm and always warn after recovery.
 for(const auto& spec:RealmGuardians)for(int reason=0;reason<3;++reason){
  GuardianBeat pattern(spec.realm);assert(pattern.Advance(1,200,false,true)==GuardianAction::Telegraph);
  Warning cue;MissingPlayer(pattern,cue,reason);
  assert(pattern.Advance(50,200,false,true)==GuardianAction::Telegraph);
  assert(pattern.Advance(50+spec.windup*.99,200,false,true)==GuardianAction::Telegraph);
  assert(pattern.Advance(50+spec.windup+.001,200,false,true)==GuardianAction::Strike);++resumed;
 }
 // Nine realm roles/health phases and all three boss turns, including signatures.
 for(int realm=0;realm<9;++realm)for(int role=0;role<5;++role)for(bool phase:{false,true})for(int turn=0;turn<3;++turn)for(int reason=0;reason<3;++reason){
  ExpeditionPattern pattern;const auto type=static_cast<ExpeditionRole>(role);double now=1;
  for(int prior=0;prior<turn;++prior){
   const auto warn=pattern.Step(now,type,realm,phase,true,200,false);assert(warn.started);
   const auto strike=pattern.Step(now+warn.move.windup+.001,type,realm,phase,true,200,false);assert(strike.strike);
   now+=warn.move.windup+warn.move.recovery+.01;
  }
  const auto old=pattern.Step(now,type,realm,phase,true,200,false);assert(old.started);
  Warning cue;MissingPlayer(pattern,cue,reason);const double resume=now+50;
  const auto warn=pattern.Step(resume,type,realm,phase,true,200,false);assert(warn.started&&!warn.strike);
  assert(!pattern.Step(resume+warn.move.windup*.99,type,realm,phase,true,200,false).strike);
  assert(pattern.Step(resume+warn.move.windup+.001,type,realm,phase,true,200,false).strike);++resumed;
 }
 // The nest elder's slam and half-health sweep cannot survive a player gap.
 for(bool halfHealth:{false,true})for(int turn=0;turn<2;++turn)for(int reason=0;reason<3;++reason){
  SlagElderPattern pattern;double now=1;
  if(turn){assert(!pattern.Step(now,halfHealth,true,true,false).strike);assert(pattern.Step(3.01,halfHealth,true,true,false).strike);now=6;}
  const auto old=pattern.Step(now,halfHealth,true,true,false);assert(old.warning!=SlagAttack::None&&!old.strike);
  Warning cue;MissingPlayer(pattern,cue,reason);const double resume=now+50;
  const auto warn=pattern.Step(resume,halfHealth,true,true,false);assert(warn.warning!=SlagAttack::None&&!warn.strike);
  const double duration=warn.warning==SlagAttack::GroundSlam?1.6:2.;
  assert(!pattern.Step(resume+duration*.99,halfHealth,true,true,false).strike);
  assert(pattern.Step(resume+duration+.001,halfHealth,true,true,false).strike);++resumed;
 }
 // All active acts/seeds, both authored rematch mornings and each reachable move.
 for(auto act:{FinalAct::Veil,FinalAct::Unmasked,FinalAct::EchoAwake})for(int seed=0;seed<6;++seed)for(auto morning:{Morning::Unchosen,Morning::Anchor,Morning::Many}){
  if((act==FinalAct::EchoAwake)==(morning==Morning::Unchosen))continue;
  for(int turn=0;turn<4;++turn)for(int reason=0;reason<3;++reason){
   FinalBattleRhythm pattern;double now=1;
   for(int prior=0;prior<turn;++prior){
    const auto warn=pattern.Advance(now,200,false,act,seed,morning);assert(warn.move==FinalMove::Telegraph);
    assert(pattern.Advance(now+warn.warningSeconds+.001,200,false,act,seed,morning).move==FinalMove::Strike);
    now+=warn.warningSeconds+(act==FinalAct::EchoAwake?2.3:3.)+.01;
   }
   assert(pattern.Advance(now,200,false,act,seed,morning).move==FinalMove::Telegraph);
   Warning cue;MissingPlayer(pattern,cue,reason);const double resume=now+50;
   const auto warn=pattern.Advance(resume,200,false,act,seed,morning);assert(warn.move==FinalMove::Telegraph);
   assert(pattern.Advance(resume+warn.warningSeconds*.99,200,false,act,seed,morning).move==FinalMove::Telegraph);
   assert(pattern.Advance(resume+warn.warningSeconds+.001,200,false,act,seed,morning).move==FinalMove::Strike);++resumed;
  }
  FinalBattleRhythm pattern;double now=1;
  StrikeAdapter attacker(act==FinalAct::Veil?17:act==FinalAct::Unmasked?21:24);
  for(int turn=0;turn<4;++turn){
   const auto warn=pattern.Advance(now,200,false,act,seed,morning);assert(warn.move==FinalMove::Telegraph);
   const auto beat=pattern.Advance(now+warn.warningSeconds+.001,200,false,act,seed,morning);assert(beat.move==FinalMove::Strike);
   attacker.gearAttack=50;assert(ConfigureAuthoredFinalStrike(attacker,beat));
   assert(attacker.gearAttack==0&&attacker.gearArmor==0);
   const auto swing=attacker.state.TryAttack(now+warn.warningSeconds+.001);assert(swing.outcome==AttackOutcome::Started);
   assert(swing.baseDamage+attacker.gearAttack==beat.damage);
   const double expected=(act==FinalAct::Veil?17:act==FinalAct::Unmasked?21:24)+(beat.attack==FinalAttack::Names?3:0);
   assert(beat.damage==expected);
   for(bool guarding:{false,true})for(double armor:{0.,4.}){
    Combatant target(1000);if(guarding)assert(target.SetGuarding(true));
    assert(target.ReceiveHit(77,swing.swingId,swing.baseDamage,false,armor)==HitOutcome::Applied);
    assert(target.Health()==1000-(expected-armor)*(guarding?.25:1.));
    assert(target.ReceiveHit(77,swing.swingId,swing.baseDamage,false,armor)==HitOutcome::Duplicate);++damageChecks;
   }
   now+=warn.warningSeconds+(act==FinalAct::EchoAwake?2.3:3.)+.01;
  }
 }
 StrikeAdapter invalid(21);const auto noChange=invalid.state.TryAttack(0);assert(noChange.baseDamage==21);
 for(double damage:{0.,-1.,std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::infinity()}){
  FinalBeat beat;beat.move=FinalMove::Strike;beat.damage=damage;
  assert(!ConfigureAuthoredFinalStrike(invalid,beat));assert(invalid.gearAttack==50);
 }
 FinalBeat warning;warning.move=FinalMove::Telegraph;warning.damage=21;assert(!ConfigureAuthoredFinalStrike(invalid,warning));
 // Durable defeated receipts are separate owners. Interruptions cannot erase a
 // failed attempt, heal the enemy, reschedule the gate or duplicate success.
 for(int scope=0;scope<18;++scope){
  Combatant defeated(100,21,.4);assert(defeated.ReceiveHit(5,1,100,false)==HitOutcome::Applied);
  DefeatSaveRetry receipt;int writes=0;assert(receipt.Attempt(scope,1,[&]{++writes;return false;})==DefeatSaveResult::FirstFailure);
  FinalBattleRhythm transient;
  for(int tick=0;tick<8;++tick)assert(!RequireEncounterPlayer(false,[&]{InterruptEncounter(transient);}));
  assert(!defeated.IsAlive());assert(receipt.Attempt(scope,5.99,[&]{++writes;return true;})==DefeatSaveResult::Waiting);
  assert(receipt.Attempt(scope,6,[&]{++writes;return true;})==DefeatSaveResult::Saved);
  assert(receipt.Attempt(scope,50,[&]{++writes;return true;})==DefeatSaveResult::Waiting);
  assert(writes==2&&!defeated.IsAlive());++receiptRetries;
 }
 std::cout<<"encounter interruption: "<<resumed<<" resumed warning paths, "<<damageChecks<<" authored damage/guard/armor checks, "<<receiptRetries<<" preserved receipt retries\n";
}
