#include "Combat/UnmadeCreatureRoutineRules.h"
#include <cassert>
#include <limits>

int main() {
    using namespace UnmadeCore;
    // A creature can walk to its authored forage/patrol destination even when
    // nobody is chasing it. A resting creature stays home unless returning.
    assert(WantsCreatureNavigation(false, false, true));
    assert(WantsCreatureNavigation(false, true, false));
    assert(WantsCreatureNavigation(true, false, false));
    assert(!WantsCreatureNavigation(false, false, false));

    const auto forage=CreatureRoutine(EnemyStyle::Stalker,0,false,false);
    const auto patrol=CreatureRoutine(EnemyStyle::Stalker,13,false,false);
    assert(forage.activity==WildActivity::Forage);
    assert(patrol.activity==WildActivity::Patrol);
    assert(CreatureRoutine(EnemyStyle::Stalker,13,false,true).activity==WildActivity::Rest);
    assert(CreatureRoutine(EnemyStyle::Stalker,13,true,false).activity==WildActivity::Rest);

    // A second creature should circle for an opening rather than begin an
    // overlapping warning/strike. Existing windups are never canceled by peers.
    assert(MayBeginOrdinaryWindup(true,0));
    assert(!MayBeginOrdinaryWindup(true,1));
    assert(!MayBeginOrdinaryWindup(false,0));
    assert(!MayBeginOrdinaryWindup(true,-1));
    assert(ShouldCircleForOpening(true,false,1));
    assert(!ShouldCircleForOpening(true,true,1));
    assert(!ShouldCircleForOpening(false,false,1));
    assert(!ShouldCircleForOpening(true,false,0));
}
