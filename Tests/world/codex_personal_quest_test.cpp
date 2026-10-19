#include "World/UnmadeAdventureDepthRegistry.h"
#include <cassert>
#include <cstring>
using namespace UnmadeCore;
int main() {
    static_assert(sizeof(DepthSpecs)/sizeof(DepthSpecs[0])==9,"Exactly nine authored personal quests");
    for(const auto& quest:DepthSpecs) {
        assert(std::strlen(quest.quest)>8);
        assert(std::strcmp(quest.option_a,quest.option_b)!=0);
        assert(std::strcmp(quest.return_a,quest.return_b)!=0);
        assert(std::strcmp(quest.witness_return_a,quest.witness_return_b)!=0);
        assert(std::strlen(quest.evidence)>15);
        assert(std::strlen(quest.witness)>20);
    }
    assert(std::strcmp(DepthSpecs[0].quest,"The Last Lantern Charter")==0);
    assert(std::strcmp(DepthSpecs[8].quest,"The Door We Can Describe")==0);
}
