#include "World/UnmadeLaterRealmRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main() {
    LaterRealmJourney journey;
    for(int i=3;i<9;++i) {
        const Realm r=static_cast<Realm>(i);
        const auto& spec=LaterRealms[i-3];
        assert(spec.realm==r && spec.centerX!=0);
        assert(std::strlen(spec.name)>10 && std::strlen(spec.trialChoiceA)>10);
        assert(std::strlen(spec.trialChoiceB)>10 && std::strlen(spec.mechanism)>12);
        assert(journey.Begin(r,spec.initiator)==LaterResult::Locked);
        assert(journey.Visit(r)==LaterResult::Visited);
        assert(journey.Visit(r)==LaterResult::NoChange);
        assert(journey.Begin(r,"npc.false")==LaterResult::WrongWitness);
        assert(journey.Begin(r,spec.initiator)==LaterResult::Started);
        assert(journey.Operate(r,spec.mechanism)==LaterResult::NeedEvidence);
        assert(journey.Study(r,1,spec.trialEvidenceB)==LaterResult::WrongSite);
        assert(journey.Study(r,1,spec.trialEvidenceA)==LaterResult::EvidenceFound);
        assert(journey.Study(r,2,spec.trialEvidenceB)==LaterResult::EvidenceFound);
        assert(journey.Operate(r,"Wrong.Control")==LaterResult::WrongMechanism);
        assert(journey.Operate(r,spec.mechanism)==LaterResult::Completed);
        assert(journey.Outcome(r)==2 && journey.IsComplete(r));
        assert(journey.Operate(r,spec.mechanism)==LaterResult::NoChange);
        assert(journey.Begin(r,spec.initiator)==LaterResult::NoChange);
    }
    const auto saved=journey.Snapshot();
    LaterRealmJourney restored;
    assert(restored.Restore(saved));
    assert(restored.Outcome(Realm::FirstAbsence)==2);
    auto bad=saved;
    bad.stage[0]=4;assert(!restored.Restore(bad));
    bad=saved;bad.stage[5]=1;bad.choice[5]=2;assert(!restored.Restore(bad));
    bad=saved;bad.visits=128;assert(!restored.Restore(bad));
    bad=saved;bad.stage[2]=2;bad.choice[2]=0;assert(!restored.Restore(bad));
    assert(restored.Snapshot().visits==saved.visits);
    LaterRealmJourney alt;
    assert(alt.Visit(Realm::TidalLedger)==LaterResult::Visited);
    assert(alt.Begin(Realm::TidalLedger,LaterRealms[0].initiator)==LaterResult::Started);
    assert(alt.Study(Realm::TidalLedger,1,LaterRealms[0].trialEvidenceA)==LaterResult::EvidenceFound);
    assert(alt.Operate(Realm::TidalLedger,LaterRealms[0].mechanism)==LaterResult::Completed);
    assert(alt.Outcome(Realm::TidalLedger)==1);
    // Both first-visit route options are separately playable in all six.
    for(const auto& Spec:LaterRealms) {
        LaterRealmJourney Alternate;
        assert(Alternate.Visit(Spec.realm)==LaterResult::Visited);
        assert(Alternate.Begin(Spec.realm,Spec.initiator)==LaterResult::Started);
        assert(Alternate.Study(Spec.realm,1,Spec.trialEvidenceA)==LaterResult::EvidenceFound);
        assert(Alternate.Operate(Spec.realm,Spec.mechanism)==LaterResult::Completed);
        assert(Alternate.Outcome(Spec.realm)==1);
        LaterRealmJourney Loaded;
        assert(Loaded.Restore(Alternate.Snapshot()));
        assert(Loaded.Outcome(Spec.realm)==1);
    }
    assert(PlanRealmRoute(Realm::WidowedRain,Realm::TidalLedger,0).empty());
    const auto SeaPath=PlanRealmRoute(Realm::WidowedRain,Realm::TidalLedger,1);
    assert(SeaPath.size()==2 && SeaPath.back()==Realm::TidalLedger);
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,0).empty());
    assert(!PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,4).empty());
    // Six later realms must each inherit physical aftershock evidence and witnesses.
    RealmAftermathChronicle aftermath;
    for(int i=3;i<9;++i){
        const Realm r=static_cast<Realm>(i);
        const auto& a=RealmAftermathSpecs[i];
        assert(aftermath.Begin(r,false,true,a.initiatingWitness)==RealmAftermathResult::Locked);
        assert(aftermath.Begin(r,true,true,a.initiatingWitness)==RealmAftermathResult::Started);
        assert(aftermath.Prepare(r,a.mechanismSite)==RealmAftermathResult::Prepared);
        assert(aftermath.Inspect(r,1,a.evidenceForCare)==RealmAftermathResult::EvidenceFound);
        assert(aftermath.Testify(r,a.careWitness)==RealmAftermathResult::Witnessed);
        assert(aftermath.Decide(r,1)==RealmAftermathResult::Resolved);
    }
    std::cout<<"PASS: six later first-visit investigations, alternatives, checkpoints and restore\n";
}
