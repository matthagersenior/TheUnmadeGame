#include "World/UnmadeWitnessReturnRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main() {
    WitnessDispatch first;
    WitnessReturn reply;
    const auto care=WitnessBraidOutcome::ShelteredThread;
    const auto docket=WitnessBraidOutcome::PublicDocket;
    assert(!WitnessReturn::CanChoose(first,care,2));
    assert(reply.Choose(1,1,first,care)==WitnessReturnEvent::NotReady);
    assert(first.Collect(care,2)==WitnessDispatchEvent::Collected);
    assert(first.HandToHessa(care,3)==WitnessDispatchEvent::Delivered);
    assert(!WitnessReturn::CanChoose(first,care,3));
    assert(WitnessReturn::CanChoose(first,care,4));
    assert(reply.Choose(0,4,first,care)==WitnessReturnEvent::InvalidChoice);
    assert(reply.Choose(3,4,first,care)==WitnessReturnEvent::InvalidChoice);
    assert(reply.Choose(1,3,first,care)==WitnessReturnEvent::NotReady);
    assert(std::strstr(reply.Journal(first,care,3),"morning")!=nullptr);
    assert(reply.Choose(1,4,first,care)==WitnessReturnEvent::Chosen);
    assert(reply.Choose(2,4,first,care)==WitnessReturnEvent::AlreadyDone);
    assert(reply.Take(3)==WitnessReturnEvent::InvalidClock);
    assert(reply.GiveToOrrel(4)==WitnessReturnEvent::NotReady);
    assert(reply.Take(4)==WitnessReturnEvent::PickedUp);
    assert(reply.Take(5)==WitnessReturnEvent::AlreadyDone);
    assert(reply.GiveToOrrel(3)==WitnessReturnEvent::InvalidClock);
    assert(reply.GiveToOrrel(6)==WitnessReturnEvent::Delivered);
    assert(reply.GiveToOrrel(6)==WitnessReturnEvent::AlreadyDone);
    assert(std::strstr(reply.OrrelOnReceipt(care),"builders")!=nullptr);
    const auto saved=reply.Snapshot();
    WitnessReturn loaded;
    assert(loaded.Restore(saved,first,care,6));
    assert(loaded.Stage()==WitnessReturnStage::OrrelReceived);
    assert(loaded.Route()==WitnessReturnRoute::PrivateCounsel);
    assert(!loaded.Restore(saved,first,WitnessBraidOutcome::Unresolved,6));
    assert(!loaded.Restore(saved,first,care,5)); // future date
    assert(!loaded.Restore({3,2,3,4,5},first,care,6)); // no full morning
    assert(!loaded.Restore({2,1,4,0,0},first,care,6)); // no collection
    assert(!loaded.Restore({3,1,4,5,4},first,care,6)); // impossible receipt
    assert(!loaded.Restore({0,1,0,0,0},first,care,6)); // stale choice
    assert(loaded.Snapshot().deliveredDay==6); // fail atomic
    WitnessReturn newGame;
    assert(newGame.Restore({0,0,0,0,0},WitnessDispatch{},WitnessBraidOutcome::Unresolved,1));
    WitnessReturn alternate;
    assert(alternate.Choose(2,4,first,docket)==WitnessReturnEvent::Chosen);
    assert(std::strstr(alternate.HessaAfterChoosing(),"read aloud")!=nullptr);
    assert(alternate.Take(4)==WitnessReturnEvent::PickedUp);
    assert(alternate.GiveToOrrel(5)==WitnessReturnEvent::Delivered);
    assert(std::strstr(alternate.OrrelOnReceipt(docket),"public question")!=nullptr);
    WitnessReturn crossed;
    assert(crossed.Choose(1,4,first,docket)==WitnessReturnEvent::Chosen);
    assert(crossed.Take(4)==WitnessReturnEvent::PickedUp);
    assert(crossed.GiveToOrrel(5)==WitnessReturnEvent::Delivered);
    assert(std::strstr(crossed.OrrelOnReceipt(docket),"both obligations")!=nullptr);
    WitnessReturn crossover;
    assert(crossover.Choose(2,4,first,care)==WitnessReturnEvent::Chosen);
    assert(crossover.Take(4)==WitnessReturnEvent::PickedUp);
    assert(crossover.GiveToOrrel(5)==WitnessReturnEvent::Delivered);
    assert(std::strstr(crossover.OrrelOnReceipt(care),"protect and accuse")!=nullptr);
    std::cout<<"PASS: next-day authored reply, no premature receipt, four endings, safe restore\n";
}
