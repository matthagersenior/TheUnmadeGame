#include "World/UnmadeRealmAftermathRules.h"
#include <cassert>
#include <cstring>
using namespace UnmadeCore;
int main() {
    static_assert(RealmAftermathSpecs.size()==9,"Nine realm aftermaths must remain canonical");
    for(int i=0;i<9;++i) {
        const auto& spec=RealmAftermathSpecs[i];
        assert(static_cast<int>(spec.realm)==i);
        assert(std::strcmp(spec.careConsequence,spec.truthConsequence)!=0);
        assert(std::strcmp(spec.evidenceForCare,spec.evidenceForTruth)!=0);
        assert(std::strcmp(spec.careWitness,spec.truthWitness)!=0);
        assert(std::strlen(spec.mechanismSite)>8);
        assert(std::strlen(spec.aftershock)>16);
        assert(spec.initialLevelExists==(i<3));
    }
    assert(std::strcmp(RealmAftermathSpecs[0].title,"The Road After the Rescue")==0);
    assert(std::strcmp(RealmAftermathSpecs[8].title,"The Place After the Ending")==0);
}
