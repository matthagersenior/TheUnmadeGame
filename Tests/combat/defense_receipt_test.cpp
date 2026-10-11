#include "Combat/UnmadeCombatRules.h"
#include "Combat/UnmadeCombatStyleRules.h"
#include <cassert>
using namespace UnmadeCore;
int main(){Combatant p,enemy;auto a=p.TryAttack(0);assert(enemy.ReceiveHit(1,a.swingId,10,false)==HitOutcome::Applied);assert(enemy.AcknowledgeHit(1,2));assert(enemy.ReceiveHit(1,2,20,false)==HitOutcome::Duplicate);p.ReceiveHit(2,1,999,false);assert(p.ReviveAtCheckpoint(.75));auto b=p.TryAttack(1);assert(b.swingId>a.swingId);assert(p.SetCooldown(.45));assert(!p.SetCooldown(0));Combatant recovering,victim;auto first=recovering.TryAttack(0);assert(victim.ReceiveHit(13,first.swingId,10,false)==HitOutcome::Applied);recovering.ReceiveHit(7,1,999,false);assert(recovering.ReviveAtCheckpoint(.75));auto next=recovering.TryAttack(1);assert(victim.ReceiveHit(13,next.swingId,10,false)==HitOutcome::Applied);CombatTempo t;t.GrantEvasion(2,.7);assert(t.Defense(2.5,false)==1);assert(t.Defense(2.8,false)==0);}
