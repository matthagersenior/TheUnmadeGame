#include "Save/UnmadeRecoveryRules.h"
#include "Combat/UnmadeCombatRules.h"
#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
using namespace UnmadeCore;
int main(){
 Combatant c(100,12,1);
 assert(c.RestoreHealth(43));
 bool observed=false;
 assert(CommitCombatHealing(c,25.0,[&]{ observed=(c.Health()==68);return true;}));
 assert(observed&&c.Health()==68);
 const auto before=c;
 assert(!CommitCombatHealing(c,10.0,[&]{ assert(c.Health()==78);return false;}));
 assert(c.Health()==before.Health());
 // Failed save cannot grant duplicate-hit replay, reset swing or guard.
 assert(c.SetGuarding(true));
 assert(c.ReceiveHit(51,1,20,false)==HitOutcome::Applied);
 const auto guarded=c;
 assert(!CommitCombatHealing(c,10,[&]{return false;}));
 assert(c.IsGuarding()==guarded.IsGuarding());
 assert(c.ReceiveHit(51,1,20,false)==HitOutcome::Duplicate);
 assert(c.Health()==guarded.Health());
 assert(!CommitCombatHealing(c,-1,[&]{assert(false);return true;}));
 assert(!CommitCombatHealing(c,std::numeric_limits<double>::quiet_NaN(),
     [&]{assert(false);return true;}));
 assert(!CommitCombatHealing(c,200,[&]{return false;}));
 assert(c.Health()==guarded.Health());
 bool strainOnly=false;
 assert(CommitCombatHealing(c,0,[&]{strainOnly=true;return true;}));
 assert(strainOnly);
 c.RestoreHealth(100);
 assert(!CommitCombatHealing(c,1,[&]{assert(false);return true;}));
 // A missing/unusable consumable allows a later fallback; a save failure does not.
 int checks=0;
 const int items[]={1,2,3,4};
 auto unavailable=[](int n){return n==1?ConsumableAttempt::Unavailable:
      n==2?ConsumableAttempt::SaveFailed:ConsumableAttempt::Used;};
 auto v=UseFirstConsumable(items,[&](int n){++checks;return unavailable(n);});
 assert(v==ConsumableAttempt::SaveFailed&&checks==2);
 checks=0;
 auto success=UseFirstConsumable(items,[&](int n){++checks;return n==3?
      ConsumableAttempt::Used:ConsumableAttempt::Unavailable;});
 assert(success==ConsumableAttempt::Used&&checks==3);
 std::cout<<"PASS: health is captured before saving; failure restores combat, "
             "preserves dedup and halts the consumable chain\n";
}