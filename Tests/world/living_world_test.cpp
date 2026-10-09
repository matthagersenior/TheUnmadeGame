#include "World/UnmadeLivingWorldRules.h"
#include <cassert>
#include <limits>
#include <iostream>
using namespace UnmadeCore;
int main() {
    LivingWorldClock clock;
    assert(clock.Phase()==DayPhase::Day && clock.DayIndex()==0 && clock.MinuteOfDay()==420);
    assert(!clock.Advance(-1) && !clock.Advance(std::numeric_limits<double>::quiet_NaN()));
    assert(clock.MinuteOfDay()==420);
    assert(clock.Restore(500) && clock.Phase()==DayPhase::Dusk);
    assert(clock.Restore(780) && clock.Phase()==DayPhase::Night);
    assert(clock.Restore(1199) && clock.Phase()==DayPhase::Dawn);
    assert(!clock.Restore(-1) && !clock.Restore(std::numeric_limits<double>::infinity()));
    assert(clock.Phase()==DayPhase::Dawn);
    assert(clock.Restore(1200) && clock.Phase()==DayPhase::Day && clock.DayIndex()==1);
    assert(clock.Advance(99) && clock.ElapsedSeconds()==1201);
    const auto daytime=RoutineTarget(NpcRole::Merchant,DayPhase::Day);
    const auto night=RoutineTarget(NpcRole::Merchant,DayPhase::Night);
    assert(daytime.x!=night.x || daytime.y!=night.y);
    assert(RoutineTarget(NpcRole::Guard,DayPhase::Night).y>RoutineTarget(NpcRole::Guard,DayPhase::Day).y);
    assert(RoutineTarget(NpcRole::Courier,DayPhase::Dusk).x!=RoutineTarget(NpcRole::Courier,DayPhase::Day).x);
    const Vec2 farHome{1700,800};
    const Vec2 dayTarget=RoutineTargetForHome(NpcRole::Scholar,DayPhase::Day,farHome);
    const Vec2 nightTarget=RoutineTargetForHome(NpcRole::Scholar,DayPhase::Night,farHome);
    assert(dayTarget.x==farHome.x && dayTarget.y==farHome.y);
    assert(nightTarget.x!=farHome.x || nightTarget.y!=farHome.y);
    DiscoveryLedger explored;
    assert(explored.Count()==0 && explored.Discover(District::EchoWell));
    assert(explored.Count()==1 && !explored.Discover(District::EchoWell));
    assert(explored.Discover(District::BellGrave) && explored.Count()==2);
    const int snapshot=explored.Snapshot();
    DiscoveryLedger loaded;
    assert(loaded.Restore(snapshot) && loaded.Count()==2);
    assert(!loaded.Restore(1<<12) && loaded.Count()==2);
    assert(!loaded.Discover(static_cast<District>(99)) && loaded.Count()==2);
    assert(SelectAmbientCue(District::EchoWell,DayPhase::Night,0)!=SelectAmbientCue(District::EchoWell,DayPhase::Day,0));
    assert(SelectAmbientCue(District::ShelterThreshold,DayPhase::Day,1)!=SelectAmbientCue(District::ShelterThreshold,DayPhase::Day,2));
    assert(SelectAmbientCue(District::PaperOrchard,DayPhase::Day,0)!=SelectAmbientCue(District::BellGrave,DayPhase::Day,0));
    std::cout<<"PASS: offline day/night schedules, 6 discoveries, atmosphere variants and save validation\n";
}
