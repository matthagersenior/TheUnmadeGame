#include "World/UnmadeWorldAtlas.h"
#include "World/UnmadeCodexGenerated.h"
#include <cassert>
#include <cstring>
using namespace UnmadeCore;
int main() {
    static_assert(WorldAtlas.size()==9,"Canonical realm count drift");
    static_assert(AtlasPassages.size()==13,"Passage graph must stay authored");
    for(int i=0;i<9;++i) {
        assert(std::strcmp(WorldAtlas[i].name,CodexRealmSummaries[i].name)==0);
        assert(WorldAtlas[i].stage==CodexRealmSummaries[i].stage);
        assert(static_cast<int>(WorldAtlas[i].id)==i);
    }
    const auto shore=PlanRealmRoute(Realm::ThreefoldReach,Realm::WidowedRain,0);
    assert(shore.size()==2 && shore.front()==Realm::ThreefoldReach);
    assert(shore.back()==Realm::WidowedRain);
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::SkyBelow,0).empty());
    assert(!PlanRealmRoute(Realm::ThreefoldReach,Realm::SkyBelow,1).empty());
    for(int tier=0;tier<4;++tier) {
        assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,tier).empty());
    }
    const auto journey=PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,4);
    assert(!journey.empty() && journey.front()==Realm::ThreefoldReach);
    assert(journey.back()==Realm::FirstAbsence);
    for(int i=1;i<static_cast<int>(journey.size());++i) {
        bool adjacent=false;
        for(const auto& edge:AtlasPassages)
            if((edge.a==journey[i-1]&&edge.b==journey[i]) ||
               (edge.b==journey[i-1]&&edge.a==journey[i]))
                adjacent= edge.attunement<=4;
        assert(adjacent);
    }
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,-1).empty());
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,5).empty());
}
