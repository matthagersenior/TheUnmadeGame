#include "World/UnmadeWitnessBraidRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main() {
    WitnessEchoLedger evidence;
    WitnessBraidChronicle braid;
    assert(!WitnessBraidChronicle::Ready(evidence));
    assert(braid.Commit(1,evidence)==WitnessBraidEvent::NotReady);
    assert(braid.Commit(0,evidence)==WitnessBraidEvent::InvalidChoice);
    assert(evidence.Read("site.callback.uncounted_cup",0)==WitnessEchoEvent::FirstReading);
    assert(evidence.Read("site.callback.inkless_nail",0)==WitnessEchoEvent::FirstReading);
    assert(!WitnessBraidChronicle::Ready(evidence));
    assert(evidence.Read("site.callback.two_faced_press",0)==WitnessEchoEvent::FirstReading);
    assert(!WitnessBraidChronicle::Ready(evidence)); // repeated presses cannot fake a later meaning
    assert(evidence.Read("site.callback.two_faced_press",2)==WitnessEchoEvent::LaterMeaning);
    assert(WitnessBraidChronicle::Ready(evidence));
    assert(std::strstr(WitnessBraidChronicle::Investigation(),"none proves")!=nullptr);
    assert(braid.Commit(2,evidence)==WitnessBraidEvent::Committed);
    assert(braid.Commit(1,evidence)==WitnessBraidEvent::AlreadyCommitted);
    assert(braid.Outcome()==WitnessBraidOutcome::PublicDocket);
    assert(std::strstr(braid.OutcomeText(),"protected guests")!=nullptr);
    const auto saved=braid.Snapshot();
    WitnessBraidChronicle restored;
    assert(restored.Restore(saved,evidence));
    assert(restored.Outcome()==WitnessBraidOutcome::PublicDocket);
    assert(!restored.Restore({3},evidence));
    assert(!restored.Restore({-1},evidence));
    assert(restored.Snapshot().outcome==2);
    WitnessEchoLedger empty;
    assert(!restored.Restore({1},empty));
    assert(restored.Restore({0},empty)); // existing saves with no braid are valid
    assert(restored.Outcome()==WitnessBraidOutcome::Unresolved);
    WitnessBraidChronicle alternative;
    assert(alternative.Commit(1,evidence)==WitnessBraidEvent::Committed);
    assert(alternative.Outcome()==WitnessBraidOutcome::ShelteredThread);
    assert(std::strstr(alternative.OutcomeText(),"refuge cord")!=nullptr);
    std::cout<<"PASS: real three-city testimony and non-omniscient two-path return\n";
}
