#include "World/UnmadeWitnessEchoRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main() {
    WitnessEchoLedger ledger;
    assert(WitnessEchoSites.size()==3 && WitnessEchoRegistrySize==13);
    assert(WitnessEchoFromSite(nullptr)==nullptr);
    assert(WitnessEchoFromSite("site.not.physical")==nullptr);
    assert(ledger.Read("site.not.physical",1)==WitnessEchoEvent::NotAnEcho);
    assert(ledger.FirstCount()==0 && ledger.ReturnCount()==0);
    assert(ledger.Read("site.callback.uncounted_cup",1)==WitnessEchoEvent::FirstReading);
    assert(ledger.HasFirst("CALL.01") && !ledger.HasReturn("CALL.01"));
    assert(ledger.Read("site.callback.uncounted_cup",0)==WitnessEchoEvent::AlreadyRecorded);
    assert(ledger.Read("site.callback.uncounted_cup",9)==WitnessEchoEvent::AlreadyRecorded);
    assert(ledger.Read("site.callback.uncounted_cup",1)==WitnessEchoEvent::LaterMeaning);
    assert(ledger.HasReturn("CALL.01"));
    assert(ledger.Read("site.callback.uncounted_cup",2)==WitnessEchoEvent::AlreadyRecorded);
    assert(ledger.FirstCount()==1 && ledger.ReturnCount()==1);
    assert(ledger.Read("site.callback.two_faced_press",0)==WitnessEchoEvent::FirstReading);
    assert(ledger.Read("site.callback.inkless_nail",0)==WitnessEchoEvent::FirstReading);
    assert(ledger.FirstCount()==3 && ledger.ReturnCount()==1);
    auto prior=ledger.Snapshot();
    WitnessEchoLedger loaded;
    assert(loaded.Restore(prior));
    assert(loaded.HasFirst("CALL.03") && !loaded.HasReturn("CALL.03"));
    assert(loaded.Read("site.callback.two_faced_press",2)==WitnessEchoEvent::LaterMeaning);
    assert(loaded.Read("site.callback.inkless_nail",2)==WitnessEchoEvent::LaterMeaning);
    assert(loaded.FirstCount()==3 && loaded.ReturnCount()==3);
    // A saved return cannot exist before physical discovery.
    const auto good=loaded.Snapshot();
    assert(!loaded.Restore({0,1}) && loaded.Snapshot().first==good.first);
    assert(!loaded.Restore({uint32_t{1}<<19,0}) &&
           loaded.Snapshot().returnRead==good.returnRead);
    assert(!loaded.Restore({0,uint32_t{1}<<19}));
    assert(!loaded.Restore({1,2}));
    // Original item/quest milestones are not manipulated by these optional bits.
    assert(WitnessEchoIndex("MYST.01")==-1);
    assert(WitnessEchoIndex("CALL.99")==-1);
    assert(WitnessEchoIndex("CALL.01")==0);
    assert(WitnessEchoIndex("CALL.03")==1);
    assert(WitnessEchoIndex("CALL.04")==2);
    assert(std::strcmp(WitnessEchoText(WitnessEchoSites[0],WitnessEchoEvent::FirstReading,1),
                       WitnessEchoSites[0].firstObservation)==0);
    assert(std::strcmp(WitnessEchoText(WitnessEchoSites[0],WitnessEchoEvent::LaterMeaning,2),
                       WitnessEchoSites[0].truthInterpretation)==0);
    assert(std::strcmp(WitnessEchoText(WitnessEchoSites[0],WitnessEchoEvent::AlreadyRecorded,1),
                       WitnessEchoSites[0].careInterpretation)==0);
    std::cout<<"PASS: witnessed physical clues, deferred two-path callbacks, strict save restore, no reward farming\n";
}
