#include "World/UnmadeWitnessDispatchRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main() {
    WitnessDispatch mail;
    const auto none=WitnessBraidOutcome::Unresolved;
    const auto care=WitnessBraidOutcome::ShelteredThread;
    const auto docket=WitnessBraidOutcome::PublicDocket;
    assert(mail.Stage()==WitnessDispatchStage::Waiting);
    assert(mail.HandToHessa(care,1)==WitnessDispatchEvent::NotReady);
    assert(mail.Collect(none,2)==WitnessDispatchEvent::NotReady);
    assert(mail.Collect(care,0)==WitnessDispatchEvent::InvalidClock);
    assert(mail.Collect(care,3)==WitnessDispatchEvent::Collected);
    assert(mail.Collect(docket,4)==WitnessDispatchEvent::AlreadyDone);
    assert(mail.HandToHessa(care,2)==WitnessDispatchEvent::InvalidClock);
    assert(mail.HandToHessa(care,4)==WitnessDispatchEvent::Delivered);
    assert(mail.HandToHessa(care,4)==WitnessDispatchEvent::AlreadyDone);
    assert(std::strstr(mail.HessaLine(care),"carried Orrel")!=nullptr);
    assert(std::strstr(mail.HessaLine(docket),"redacted docket")!=nullptr);
    WitnessDispatch loaded;
    const auto snap=mail.Snapshot();
    assert(loaded.Restore(snap,care));
    assert(loaded.Stage()==WitnessDispatchStage::HandDelivered);
    assert(loaded.Snapshot().deliveredDay==4);
    assert(!loaded.Restore(snap,none) && loaded.Snapshot().deliveredDay==4);
    assert(!loaded.Restore({2,0,4},care));
    assert(!loaded.Restore({2,5,4},care));
    assert(!loaded.Restore({1,0,0},care));
    assert(!loaded.Restore({0,1,0},care));
    assert(!loaded.Restore({3,3,4},care));
    assert(loaded.Restore({0,0,0},none));
    WitnessDispatch partial;
    assert(partial.Collect(docket,5)==WitnessDispatchEvent::Collected);
    assert(partial.Restore(partial.Snapshot(),docket));
    assert(partial.Stage()==WitnessDispatchStage::PlayerCarrying);
    assert(std::strstr(partial.PlayerLine(docket),"You carry")!=nullptr);
    assert(std::strlen(partial.HessaLine(docket))==0);
    assert(partial.HandToHessa(docket,7)==WitnessDispatchEvent::Delivered);
    std::cout<<"PASS: carried real note, delayed named recipient, save-safe custody and truthful branches\n";
}
