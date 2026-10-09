#include "Combat/UnmadeCombatRules.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
using namespace UnmadeCore;
int main() {
    Combatant attacker(100,24,0.75), target(80,10,1.5);
    assert(attacker.SetGuarding(true));
    assert(attacker.TryAttack(0).outcome == AttackOutcome::Guarding);
    assert(attacker.SetGuarding(false));
    auto a1=attacker.TryAttack(0);
    assert(a1.outcome==AttackOutcome::Started && a1.swingId==1);
    assert(target.ReceiveHit(11,a1.swingId,a1.baseDamage,false)==HitOutcome::Applied);
    assert(target.Health()==56);
    assert(target.ReceiveHit(11,a1.swingId,a1.baseDamage,false)==HitOutcome::Duplicate);
    assert(target.Health()==56);
    assert(attacker.TryAttack(0.1).outcome==AttackOutcome::CoolingDown);
    assert(attacker.TryAttack(-1).outcome==AttackOutcome::InvalidTime);
    assert(target.SetGuarding(true));
    auto a2=attacker.TryAttack(0.8);
    assert(a2.outcome==AttackOutcome::Started && a2.swingId==2);
    assert(target.ReceiveHit(11,a2.swingId,a2.baseDamage,false)==HitOutcome::Applied);
    assert(target.Health()==50);
    assert(target.ReceiveHit(11,a2.swingId,a2.baseDamage,false)==HitOutcome::Duplicate);
    assert(target.SetGuarding(false));
    assert(target.ReceiveHit(11,3,-1,false)==HitOutcome::InvalidDamage);
    assert(target.ReceiveHit(11,3,std::numeric_limits<double>::infinity(),false)==HitOutcome::InvalidDamage);
    assert(target.ReceiveHit(0,3,20,false)==HitOutcome::InvalidSource);
    assert(target.ReceiveHit(11,3,25,true)==HitOutcome::Applied);
    assert(target.Health()==12.5);
    assert(target.ReceiveHit(12,1,15,false)==HitOutcome::Applied);
    assert(target.Health()==0 && !target.IsAlive());
    assert(target.ReceiveHit(13,1,20,false)==HitOutcome::AlreadyDefeated);
    assert(target.TryAttack(5).outcome==AttackOutcome::Defeated);
    assert(!target.SetGuarding(true));
    assert(attacker.TryAttack(std::numeric_limits<double>::quiet_NaN()).outcome==AttackOutcome::InvalidTime);
    assert(ChooseEnemyIntent(EnemyStyle::Stalker,8,true)==EnemyIntent::Approach);
    assert(ChooseEnemyIntent(EnemyStyle::Stalker,1.5,true)==EnemyIntent::Attack);
    assert(ChooseEnemyIntent(EnemyStyle::Stalker,1.5,false)==EnemyIntent::Approach);
    assert(ChooseEnemyIntent(EnemyStyle::Watcher,2,true)==EnemyIntent::Retreat);
    assert(ChooseEnemyIntent(EnemyStyle::Watcher,5.5,true)==EnemyIntent::Attack);
    assert(ChooseEnemyIntent(EnemyStyle::Watcher,9,true)==EnemyIntent::Approach);
    assert(ChooseEnemyIntent(EnemyStyle::Watcher,5.5,false)==EnemyIntent::Approach);
    assert(ChooseEnemyIntent(EnemyStyle::Watcher,-1,true)==EnemyIntent::Hold);
    std::cout<<"PASS: AI-free combat/guard/cooldown/death/fracture counterplay/two tactics\n";
}
