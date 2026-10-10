#include "Combat/UnmadeRealmGuardianRules.h"
#include "Combat/UnmadeCombatRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <string>
using namespace UnmadeCore;
int main() {
    GuardianChronicle journey;
    std::set<std::string> names;
    for(int i=0;i<6;++i) {
        const auto& s=RealmGuardians[i];
        assert(s.realm==LaterRealms[i].realm);
        assert(names.insert(s.name).second);
        assert(std::strlen(s.name)>15 && std::strlen(s.origin)>65 &&
               std::strlen(s.warning)>60 && std::strlen(s.careMemory)>50 &&
               std::strlen(s.truthMemory)>50);
        assert(s.maxHealth>=100 && s.damage>0 && s.rangeCm>=300);
        assert(s.windup>=1.5 && s.recovery>=2);
        assert(journey.Resolve(s.realm,false,true)==GuardianResolution::Locked);
        GuardianBeat beats(s.realm);
        const double t=100.0+i*15;
        assert(beats.Advance(t,280,false,true)==GuardianAction::Telegraph);
        assert(beats.Advance(t+.8,280,false,true)==GuardianAction::Telegraph);
        assert(beats.Advance(t+s.windup+.01,280,false,true)==GuardianAction::Strike);
        assert(beats.Advance(t+s.windup+.1,280,false,true)==GuardianAction::Idle);
        assert(beats.Advance(t+s.windup+s.recovery+.01,280,false,true)
               ==GuardianAction::Telegraph);
        assert(beats.Advance(t+s.windup+s.recovery+.11,280,true,true)
               ==GuardianAction::Stagger);
        assert(beats.Advance(t+s.windup+s.recovery+.25,280,false,true)
               ==GuardianAction::Idle);
        beats.Reset();
        assert(beats.Advance(t,280,false,false)==GuardianAction::Idle);
        assert(beats.Advance(std::numeric_limits<double>::quiet_NaN(),280,false,true)==GuardianAction::Idle);
        assert(beats.Advance(t,-1,false,true)==GuardianAction::Idle);
        assert(beats.Advance(t,1900,false,true)==GuardianAction::Idle);
        assert(beats.Advance(t,1650,false,true)==GuardianAction::Approach);
        // Mercy is available because the three-chapter witness evidence is
        // already resolved; it is never gated on choosing a particular weapon.
        const auto result=journey.Resolve(s.realm,true,(i%2)==0);
        assert(result==((i%2)==0?GuardianResolution::Pacified:
                         GuardianResolution::Defeated));
        assert(journey.Outcome(s.realm)==((i%2)==0?
            GuardianOutcome::Pacified:GuardianOutcome::Defeated));
        assert(journey.Resolve(s.realm,true,true)==GuardianResolution::AlreadyResolved);
    }
    const auto snapshot=journey.Snapshot();
    GuardianChronicle loaded;
    assert(loaded.Restore(snapshot));
    assert(loaded.Outcome(Realm::FirstAbsence)==GuardianOutcome::Defeated);
    auto bad=snapshot;
    bad.outcomes[4]=3;assert(!loaded.Restore(bad));
    bad=snapshot;
    bad.outcomes[1]=-1;assert(!loaded.Restore(bad));
    assert(loaded.Outcome(Realm::FirstAbsence)==GuardianOutcome::Defeated);
    GuardianChronicle clean;
    assert(clean.Restore({}));
    assert(clean.Outcome(Realm::TidalLedger)==GuardianOutcome::Unresolved);
    assert(clean.Resolve(Realm::ThreefoldReach,true,true)==GuardianResolution::Invalid);
    Combatant player;
    assert(player.SetGuarding(true));
    assert(player.ReceiveHit(0xFAB1,1,20,false)==HitOutcome::Applied);
    assert(player.Health()==95.0);
    std::cout<<"PASS: six warning-based guardians, guard mitigation, mercy/force, immutable saved outcomes\n";
}
