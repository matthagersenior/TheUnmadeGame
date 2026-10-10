#include "World/UnmadeEchoQuestRules.h"
#include "World/UnmadeRealmGeometryRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
using namespace UnmadeCore;

static void Complete(EchoChronicle& journal,int i,int choice) {
    const auto& q=EchoQuests[i];
    assert(journal.Begin(q.realm,true,true,q.openingWitness)==EchoResult::Started);
    assert(journal.Inspect(q.realm,q.evidenceA)==EchoResult::Discovered);
    assert(journal.Inspect(q.realm,q.evidenceA)==EchoResult::NoChange);
    const char* w=choice==1?q.witnessCare:q.witnessTruth;
    assert(journal.Testify(q.realm,w)==EchoResult::NeedEvidence);
    assert(journal.Inspect(q.realm,q.evidenceB)==EchoResult::Discovered);
    assert(journal.Evidence(q.realm)==3);
    assert(journal.Inspect(q.realm,q.evidenceB)==EchoResult::NoChange);
    assert(journal.Testify(q.realm,w)==EchoResult::Testified);
    assert(journal.Stage(q.realm)==2);
    const char* site=choice==1?q.ritualCare:q.ritualTruth;
    assert(journal.Commit(q.realm,3-choice,site)==EchoResult::WrongChoice);
    assert(journal.Commit(q.realm,choice,"Wrong.Site")==EchoResult::WrongSite);
    assert(journal.Commit(q.realm,choice,site)==EchoResult::Committed);
    assert(journal.Ending(q.realm)==choice);
    assert(journal.Commit(q.realm,choice,site)==EchoResult::NoChange);
    assert(journal.Inspect(q.realm,q.evidenceA)==EchoResult::NoChange);
}
int main() {
    static_assert(LaterArenaLayout::Sound(),"physical gap/bridge/gates invalid");
    const auto a=LaterArenaLayout::South(), b=LaterArenaLayout::North();
    assert(!Overlaps(a,b) && a.maxY<b.minY);
    assert(Overlaps(a,LaterArenaLayout::BridgeCare()));
    assert(Overlaps(b,LaterArenaLayout::BridgeTruth()));
    assert(!Overlaps(LaterArenaLayout::WallLeft(),LaterArenaLayout::GateCare()));
    assert(!Overlaps(LaterArenaLayout::WallMiddle(),LaterArenaLayout::GateTruth()));
    assert(!Overlaps(LaterArenaLayout::WallRight(),LaterArenaLayout::GateTruth()));
    EchoChronicle journal;
    std::set<std::string> titles, tags;
    for(int i=0;i<6;++i) {
        const auto& q=EchoQuests[i];
        assert(q.realm==LaterRealms[i].realm);
        assert(std::strlen(q.title)>12 && std::strlen(q.evidenceALine)>60);
        assert(std::strlen(q.evidenceBLine)>60 && std::strlen(q.resultCare)>55);
        assert(std::strlen(q.resultTruth)>55 && std::strlen(q.farRealmMessage)>45);
        assert(titles.insert(q.title).second);
        for(const char* tag:{q.evidenceA,q.evidenceB,q.ritualCare,q.ritualTruth})
            assert(tags.insert(tag).second);
        assert(journal.Begin(q.realm,false,true,q.openingWitness)==EchoResult::Locked);
        assert(journal.Begin(q.realm,true,false,q.openingWitness)==EchoResult::Locked);
        assert(journal.Begin(q.realm,true,true,"npc.unknown")==EchoResult::WrongWitness);
        assert(journal.Inspect(q.realm,q.evidenceA)==EchoResult::NoChange);
        Complete(journal,i,(i%2)+1);
    }
    auto saved=journal.Snapshot();
    EchoChronicle restored;
    assert(restored.Restore(saved));
    assert(restored.Ending(Realm::FirstAbsence)==2);
    auto invalid=saved;
    invalid.stage[2]=4;assert(!restored.Restore(invalid));
    invalid=saved;invalid.evidence[4]=2;assert(!restored.Restore(invalid));
    invalid=saved;invalid.testimony[5]=0;assert(!restored.Restore(invalid));
    invalid=saved;invalid.ending[1]=1;assert(!restored.Restore(invalid));
    assert(restored.Ending(Realm::FirstAbsence)==2);
    EchoChronicle fresh;
    assert(fresh.Restore({}));
    assert(fresh.Stage(Realm::TidalLedger)==0);
    for(int i=0;i<6;++i) {
        EchoChronicle care,truth;
        Complete(care,i,1);Complete(truth,i,2);
        assert(care.Ending(EchoQuests[i].realm)!=truth.Ending(EchoQuests[i].realm));
        assert(std::strcmp(EchoQuests[i].resultCare,EchoQuests[i].resultTruth)!=0);
    }
    std::cout<<"PASS: six authored third-chapter quests, both outcomes, save guards and actual two-gap geometry contract\n";
}
