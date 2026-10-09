#include "World/UnmadeFrontierRealmRules.h"
#include <cassert>
#include <iostream>
#include <cstring>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
    assert(FrontierOutposts.size()==2 && FrontierResidents.size()==16);
    std::set<std::string> ids;
    int counts[2]={};
    for(const auto& r:FrontierResidents){
        assert(ids.emplace(r.id).second);
        const int i=FrontierIndex(r.home);assert(i>=0 && i<2);++counts[i];
        assert(std::strlen(r.authoredLine)>18);
        assert(FindFrontierResident(r.id)==&r);
    }
    assert(counts[0]==8 && counts[1]==8);
    assert(FindFrontier(Realm::WidowedRain) && FindFrontier(Realm::HearthBeneath));
    assert(!FindFrontier(Realm::FirstAbsence));
    FrontierJourney journey;
    assert(journey.Converse("npc.saltwake.navigator.001")==FrontierEvent::Advanced);
    assert(journey.Converse("npc.saltwake.harborwarden.001")==FrontierEvent::NoChange);
    assert(journey.Converse("npc.saltwake.rainkeeper.001")==FrontierEvent::Advanced);
    assert(journey.Converse("npc.saltwake.harborwarden.001")==FrontierEvent::NeedClue);
    assert(journey.FindClue(Realm::WidowedRain)==FrontierEvent::NoChange);
    assert(journey.Visit(Realm::WidowedRain)==FrontierEvent::Advanced);
    assert(journey.FindClue(Realm::WidowedRain)==FrontierEvent::Advanced);
    assert(journey.Converse("npc.saltwake.harborwarden.001")==FrontierEvent::FinalChoice);
    assert(journey.Resolve(Realm::WidowedRain,0)==FrontierEvent::NoChange);
    assert(journey.Resolve(Realm::WidowedRain,1)==FrontierEvent::Resolved);
    assert(journey.Resolve(Realm::WidowedRain,2)==FrontierEvent::NoChange);
    assert(journey.Visit(Realm::HearthBeneath)==FrontierEvent::Advanced);
    assert(journey.Converse("npc.cinderhold.hearthreader.001")==FrontierEvent::Advanced);
    assert(journey.Converse("npc.cinderhold.healer.001")==FrontierEvent::Advanced);
    assert(journey.FindClue(Realm::HearthBeneath)==FrontierEvent::Advanced);
    assert(journey.Converse("npc.cinderhold.emberwarden.001")==FrontierEvent::FinalChoice);
    assert(journey.Resolve(Realm::HearthBeneath,2)==FrontierEvent::Resolved);
    assert(journey.IsResolved(Realm::WidowedRain) && journey.IsResolved(Realm::HearthBeneath));
    FrontierJourney loaded;assert(loaded.Restore(journey.Snapshot()));
    assert(loaded.IsResolved(Realm::WidowedRain));
    const auto prior=loaded.Snapshot();
    auto corrupted=prior;corrupted.endings[0]=9;
    assert(!loaded.Restore(corrupted) && loaded.Snapshot().endings[0]==prior.endings[0]);
    corrupted=prior;corrupted.discoveries=4;
    assert(!loaded.Restore(corrupted));
    std::cout<<"PASS: two distinct new realms, 16 adaptive identities, authored gated chronicles and snapshot checks\n";
}
