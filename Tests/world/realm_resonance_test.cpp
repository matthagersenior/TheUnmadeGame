#include "World/UnmadeRealmResonanceRules.h"
#include "World/UnmadeRealmGeometryRules.h"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <string>
using namespace UnmadeCore;
int main() {
    static_assert(LaterArenaLayout::Sound(),"observatory span and terrain must physically intersect");
    const auto last=LaterArenaLayout::North();
    const auto bridge=LaterArenaLayout::ResonanceSpan();
    const auto dock=LaterArenaLayout::ResonanceIsland();
    assert(Overlaps(last,bridge) && Overlaps(bridge,dock));
    assert(!Overlaps(last,dock));
    assert(Contains(dock,0,4380));
    std::set<std::string> titles;
    std::uint16_t covered=0;
    RealmResonanceJourney journey;
    for(int i=0;i<6;++i) {
        const auto& s=RealmResonances[i];
        assert(s.realm==LaterRealms[i].realm);
        assert(s.first!=s.second);
        assert(std::strlen(s.title)>18 && std::strlen(s.question)>55);
        assert(std::strlen(s.openedArchive)>75);
        assert(std::strcmp(s.responseCare,s.responseTruth)!=0);
        assert(titles.insert(s.title).second);
        covered|=1u<<static_cast<int>(s.first);
        covered|=1u<<static_cast<int>(s.second);
        const double now=1000+i*240;
        assert(journey.Record(s.realm,s.first,now,false,true)==ResonanceResult::Locked);
        assert(journey.Record(s.realm,s.first,now,true,false)==ResonanceResult::Locked);
        assert(journey.Record(s.realm,RiteId::Count,now,true,true)==ResonanceResult::WrongAbility);
        assert(journey.Record(s.realm,s.first,std::numeric_limits<double>::quiet_NaN(),true,true)==ResonanceResult::ClockInvalid);
        assert(journey.Record(s.realm,s.first,now,true,true)==ResonanceResult::Started);
        assert(journey.Stage(s.realm)==1);
        assert(journey.Record(s.realm,s.second,now-1,true,true)==ResonanceResult::ClockInvalid);
        assert(journey.Record(s.realm,s.first,now+40,true,true)==ResonanceResult::AlreadyRecorded);
        assert(journey.Record(s.realm,s.second,now+120,true,true)==ResonanceResult::Opened);
        assert(journey.Stage(s.realm)==2);
        assert(journey.Record(s.realm,s.first,now+121,true,true)==ResonanceResult::AlreadyOpened);
    }
    assert(covered==1023); // all ten original rite IDs appear in actual pairings
    const auto completed=journey.Snapshot();
    RealmResonanceJourney loaded;
    assert(loaded.Restore(completed));
    assert(loaded.Stage(Realm::FirstAbsence)==2);
    auto bad=completed;bad.stage[0]=3;assert(!loaded.Restore(bad));
    bad=completed;bad.firstCast[0]=0;assert(!loaded.Restore(bad));
    bad=completed;bad.deadline[0]=7;assert(!loaded.Restore(bad));
    bad=completed;bad.stage[0]=1;bad.deadline[0]=1;assert(!loaded.Restore(bad));
    bad=completed;bad.deadline[4]=std::numeric_limits<double>::infinity();assert(!loaded.Restore(bad));
    assert(loaded.Stage(Realm::FirstAbsence)==2);
    RealmResonanceJourney relaunch;
    const auto& a=RealmResonances[0];
    assert(relaunch.Record(a.realm,a.second,20,true,true)==ResonanceResult::Started);
    assert(relaunch.Record(a.realm,a.first,141,true,true)==ResonanceResult::ExpiredRestarted);
    assert(relaunch.Record(a.realm,a.second,142,true,true)==ResonanceResult::Opened);
    assert(relaunch.Stage(a.realm)==2);
    // Loading an old schema-1 slot with no new arrays means fresh independent state.
    RealmResonanceJourney oldSlot;
    assert(oldSlot.Restore({}));
    for(const auto& spec:RealmResonances)assert(oldSlot.Stage(spec.realm)==0);
    assert(oldSlot.Record(Realm::ThreefoldReach,RiteId::Oathbinding,12,true,true)==ResonanceResult::Locked);
    std::cout<<"PASS: six persisted paired-rite observatories, all ten powers, fair timeouts and corrupt-state refusal\n";
}
