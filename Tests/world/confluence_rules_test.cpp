#include "World/UnmadeConfluenceRules.h"
#include <cassert>
#include <iostream>
#include <cstring>
using namespace UnmadeCore;
int main(){
    assert(Confluences.size()==6);
    ConfluenceJourney trials;
    for(int i=0;i<6;++i) {
        const auto id=static_cast<ConfluenceId>(i);
        const auto& data=Confluences[i];
        assert(data.id==id && std::strlen(data.lore)>45 && std::strlen(data.reward)>15);
        assert(trials.Discover(id,i,0,3)==ConfluenceResult::NeedsMastery);
        assert(trials.Discover(id,i,1023,3)==ConfluenceResult::Advanced);
        assert(trials.RecordCast(id,data.first,300.0,i)==ConfluenceResult::CastRecorded);
        assert(trials.Resolve(id,i,3,1,301.0)==ConfluenceResult::NeedDualCast);
        assert(trials.RecordCast(id,data.second,315.0,i)==ConfluenceResult::ReadyToDecide);
        assert(trials.Resolve(id,i,0,1,316.0)==ConfluenceResult::NeedsWitnesses);
        assert(trials.Resolve(id,i,3,2,316.0)==ConfluenceResult::Completed);
        assert(trials.Stage(id)==2);
        assert(trials.Discover(id,i,1023,3)==ConfluenceResult::AlreadyCompleted);
    }
    ConfluenceJourney restored;
    assert(restored.Restore(trials.Snapshot()));
    for(int i=0;i<6;++i)assert(restored.Stage(static_cast<ConfluenceId>(i))==2);
    auto broken=trials.Snapshot();
    broken.stage[0]=4;
    assert(!restored.Restore(broken) && restored.Stage(ConfluenceId::SilentAlarm)==2);
    ConfluenceJourney early;
    assert(early.Discover(ConfluenceId::FirstAbsence,5,1023,3)
           ==ConfluenceResult::NeedsMastery);
    assert(early.Discover(ConfluenceId::SilentAlarm,0,1023,3)==ConfluenceResult::Advanced);
    assert(early.RecordCast(ConfluenceId::SilentAlarm,RiteId::Witnesscraft,20,0)
           ==ConfluenceResult::CastRecorded);
    assert(early.RecordCast(ConfluenceId::SilentAlarm,RiteId::UnwriteLaw,141,0)
           ==ConfluenceResult::CastRecorded);
    assert(early.Resolve(ConfluenceId::SilentAlarm,0,2,1,142)
           ==ConfluenceResult::NeedDualCast);
    std::cout<<"PASS: six multi-discipline confluence chambers with timed dual casting, witnesses, save and late-campaign gates\n";
}
