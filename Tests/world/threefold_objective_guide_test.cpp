#include "World/UnmadeThreefoldObjectiveGuide.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;

static void expect(const ThreefoldGuide& guide, ThreefoldBeat beat,
                   const char* city, const char* control) {
    assert(guide.beat==beat);
    assert(std::strcmp(guide.city,city)==0);
    assert(std::strcmp(guide.control,control)==0);
    assert(std::strlen(guide.prompt)>18);
}

static void Journey(int braidChoice,int replyChoice) {
    WitnessEchoLedger evidence;
    WitnessBraidChronicle braid;
    WitnessDispatch dispatch;
    WitnessReturn reply;
    auto guide=[&](int day) {return NextThreefoldGuide(evidence,braid,dispatch,reply,day);};
    expect(guide(1),ThreefoldBeat::InspectBellwold,"Bellwold","E");
    assert(std::strstr(guide(1).prompt,"cup")==nullptr); // unseen mystery intact
    assert(evidence.Read("site.callback.uncounted_cup",0)==WitnessEchoEvent::FirstReading);
    expect(guide(1),ThreefoldBeat::InspectPaperhaven,"Paperhaven","E");
    assert(evidence.Read("site.callback.two_faced_press",0)==WitnessEchoEvent::FirstReading);
    expect(guide(1),ThreefoldBeat::InspectCrossings,"The Crossings","E");
    assert(evidence.Read("site.callback.inkless_nail",0)==WitnessEchoEvent::FirstReading);
    expect(guide(1),ThreefoldBeat::ReturnAfterCivicChoice,"Previously visited site","E");
    // Firsthand follow-up only after a true local outcome, not instant read repetition.
    assert(evidence.Read("site.callback.two_faced_press",0)==WitnessEchoEvent::AlreadyRecorded);
    expect(guide(1),ThreefoldBeat::ReturnAfterCivicChoice,"Previously visited site","E");
    assert(evidence.Read("site.callback.two_faced_press",1)==WitnessEchoEvent::LaterMeaning);
    expect(guide(1),ThreefoldBeat::DecideAtOrrel,"The Crossings","F7/F8");
    assert(braid.Commit(braidChoice,evidence)==WitnessBraidEvent::Committed);
    expect(guide(1),ThreefoldBeat::CollectOrrelRecord,"The Crossings","E");
    assert(dispatch.Collect(braid.Outcome(),1)==WitnessDispatchEvent::Collected);
    expect(guide(1),ThreefoldBeat::DeliverToHessa,"Bellwold","E");
    assert(dispatch.HandToHessa(braid.Outcome(),1)==WitnessDispatchEvent::Delivered);
    expect(guide(1),ThreefoldBeat::WaitAMorning,"Bellwold","WAIT");
    expect(guide(2),ThreefoldBeat::DecideHessaReply,"Bellwold","F7/F8");
    assert(reply.Choose(replyChoice,2,dispatch,braid.Outcome())==WitnessReturnEvent::Chosen);
    expect(guide(2),ThreefoldBeat::CollectHessaReply,"Bellwold","E");
    assert(reply.Take(2)==WitnessReturnEvent::PickedUp);
    expect(guide(2),ThreefoldBeat::ReturnToOrrel,"The Crossings","E");
    assert(reply.GiveToOrrel(2)==WitnessReturnEvent::Delivered);
    auto done=guide(2);
    expect(done,ThreefoldBeat::Completed,"The Crossings","JOURNAL");
    assert(std::strstr(done.prompt,"unresolved")!=nullptr ||
           std::strstr(done.prompt,"disputed")!=nullptr ||
           std::strstr(done.prompt,"question")!=nullptr);

    const auto old=evidence.Snapshot();
    const auto oldBraid=braid.Snapshot();
    const auto oldDispatch=dispatch.Snapshot();
    const auto oldReply=reply.Snapshot();
    WitnessEchoLedger restored;
    WitnessBraidChronicle rb;
    WitnessDispatch rd;
    WitnessReturn rr;
    assert(restored.Restore(old));
    assert(rb.Restore(oldBraid,restored));
    assert(rd.Restore(oldDispatch,rb.Outcome()));
    assert(rr.Restore(oldReply,rd,rb.Outcome(),2));
    expect(NextThreefoldGuide(restored,rb,rd,rr,2),
           ThreefoldBeat::Completed,"The Crossings","JOURNAL");
    // The guide is strictly read-only.
    for(int i=0;i<3;++i)guide(2);
    assert(evidence.Snapshot().first==old.first &&
           evidence.Snapshot().returnRead==old.returnRead);
    assert(braid.Snapshot().outcome==oldBraid.outcome);
    assert(dispatch.Snapshot().stage==oldDispatch.stage);
    assert(reply.Snapshot().stage==oldReply.stage);
}

int main() {
    Journey(1,1); Journey(1,2); Journey(2,1); Journey(2,2);
    WitnessEchoLedger evidence;
    WitnessBraidChronicle braid;
    WitnessDispatch dispatch;
    WitnessReturn reply;
    expect(NextThreefoldGuide(evidence,braid,dispatch,reply,0),
           ThreefoldBeat::InvalidState,"","NONE");

    // A corrupted composite must never produce a misleading or spoiler-laden objective.
    assert(evidence.Restore({7,7}));
    assert(braid.Restore({1},evidence));
    assert(dispatch.Restore({1,1,0},braid.Outcome()));
    expect(NextThreefoldGuide(evidence,braid,dispatch,reply,1),
           ThreefoldBeat::DeliverToHessa,"Bellwold","E");
    assert(evidence.Restore({0,0}));
    expect(NextThreefoldGuide(evidence,braid,dispatch,reply,1),
           ThreefoldBeat::InvalidState,"","NONE");

    // Do not reveal branch-only text from a hypothetical future return.
    WitnessEchoLedger clean;
    WitnessBraidChronicle undecided;
    WitnessDispatch empty;
    WitnessReturn noReply;
    auto start=NextThreefoldGuide(clean,undecided,empty,noReply,1);
    assert(std::strstr(start.prompt,"Hessa's answer")==nullptr);
    assert(std::strstr(start.prompt,"docket")==nullptr);
    std::cout<<"PASS: four physical Threefold witness-return routes, safe day timing, "
                "no spoilers, non-mutating objectives, corrupt save refusal\n";
}
