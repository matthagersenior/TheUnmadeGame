#include "World/UnmadeCampaignSpineRules.h"
#include <cassert>
#include <cstring>
using namespace UnmadeCore;
int main() {
    static_assert(UnansweredRoadBeats.size()==9,"The nine authored first-story beats must remain ordered");
    for(int i=0;i<9;++i){
        const auto& b=UnansweredRoadBeats[i];
        assert(static_cast<int>(b.realm)==i);
        assert(std::strlen(b.chapter)>5);
        assert(std::strlen(b.localQuestion)>20);
        assert(std::strlen(b.witnessedDiscovery)>25);
        assert(std::strlen(b.nextDirection)>20);
        assert(std::strncmp(b.publicTag,"Campaign.Thread.",16)==0);
        assert(b.publicTag[16]=='0'+i && b.publicTag[17]=='\0');
    }
    assert(std::strcmp(UnansweredRoadBeats[0].chapter,"A Bell That Had No Last Note")==0);
    assert(std::strcmp(UnansweredRoadBeats[8].chapter,"The Answer With No Question")==0);
}
