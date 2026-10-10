#include "World/UnmadeRealmAftermathRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main(){
    RealmAftermathChronicle campaign;
    assert(RealmAftermathSpecs.size()==WorldAtlas.size());
    for(int i=0;i<9;++i){
        const auto realm=static_cast<Realm>(i);
        const auto& spec=RealmAftermathSpecs[i];
        assert(spec.realm==realm);
        assert(std::strlen(spec.title)>12 && std::strlen(spec.ethicalDilemma)>40);
        assert(std::strlen(spec.aftershock)>40);
        assert(std::strlen(spec.signatureAbility)>8);
        assert(campaign.Begin(realm,false,true,spec.initiatingWitness)==RealmAftermathResult::Locked);
        assert(campaign.Begin(realm,true,false,spec.initiatingWitness)==RealmAftermathResult::Locked);
        assert(campaign.Begin(realm,true,true,"npc.unknown")==RealmAftermathResult::WrongWitness);
        assert(campaign.Begin(realm,true,true,spec.initiatingWitness)==RealmAftermathResult::Started);
        assert(campaign.Begin(realm,true,true,spec.initiatingWitness)==RealmAftermathResult::NoChange);
        assert(campaign.Inspect(realm,1,spec.evidenceForTruth)==RealmAftermathResult::WrongSite);
        assert(campaign.Inspect(realm,1,spec.evidenceForCare)==RealmAftermathResult::EvidenceFound);
        assert(campaign.Testify(realm,spec.truthWitness)==RealmAftermathResult::WrongWitness);
        assert(campaign.Testify(realm,spec.careWitness)==RealmAftermathResult::Witnessed);
        assert(campaign.Decide(realm,2)==RealmAftermathResult::WrongChoice);
        assert(campaign.Decide(realm,1)==RealmAftermathResult::Resolved);
        assert(campaign.Decide(realm,1)==RealmAftermathResult::NoChange);
        assert(campaign.Stage(realm)==4 && campaign.Ending(realm)==1);
        assert(std::strcmp(campaign.WorldConsequence(realm),spec.careConsequence)==0);
    }
    // Every realm supports the opposite ethical path; neither is an "evil" ending.
    RealmAftermathChronicle second;
    const auto& rain=RealmAftermathSpecs[1];
    assert(second.Begin(Realm::WidowedRain,true,true,rain.initiatingWitness)==RealmAftermathResult::Started);
    assert(second.Inspect(Realm::WidowedRain,2,rain.evidenceForTruth)==RealmAftermathResult::EvidenceFound);
    assert(second.Testify(Realm::WidowedRain,rain.truthWitness)==RealmAftermathResult::Witnessed);
    assert(second.Decide(Realm::WidowedRain,2)==RealmAftermathResult::Resolved);
    assert(std::strcmp(second.WorldConsequence(Realm::WidowedRain),rain.truthConsequence)==0);
    assert(campaign.WorldConsequence(Realm::WidowedRain)!=second.WorldConsequence(Realm::WidowedRain));
    RealmAftermathChronicle loaded;
    assert(loaded.Restore(campaign.Snapshot()));
    assert(loaded.Ending(Realm::FirstAbsence)==1);
    const auto before=loaded.Snapshot();
    auto bad=before;
    bad.approach[4]=2;
    assert(!loaded.Restore(bad));
    assert(loaded.Ending(Realm::FirstAbsence)==1);
    bad=before;bad.stage[8]=7;
    assert(!loaded.Restore(bad));
    RealmAftermathChronicle oldSave;
    assert(oldSave.Restore({}));
    assert(oldSave.Stage(Realm::ThreefoldReach)==0);
    std::cout<<"PASS: nine distinct non-farmable aftermath arcs, real clues/witnesses, both choices and durable snapshots\\n";
}
