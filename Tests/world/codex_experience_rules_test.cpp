#include "World/UnmadeExpeditionRegistry.h"
#include "World/UnmadePlayerExperienceRules.h"
#include <cassert>
#include <cstring>
using namespace UnmadeCore;
int main() {
    static_assert(sizeof(ExperienceSpecs)/sizeof(ExperienceSpecs[0])==9,"Nine Codex wings required");
    for(int i=0;i<9;++i) {
        const auto& w=ExperienceSpecs[i];
        assert(std::strcmp(w.city,ExpeditionSpecs[i].city)==0);
        assert(std::strcmp(w.reserve_return,w.witness_return)!=0);
        assert(std::strlen(w.guardian)>4);
        for(const auto* scene:w.scenes)assert(std::strlen(scene)>25);
        for(int room=0;room<3;++room) {
            auto point=WingRoom(i,room);
            assert(point.x==w.wing_grid[room][0]*2600);
            assert(point.y==w.wing_grid[room][1]*2600);
        }
    }
    assert(!std::strcmp(ExperienceSpecs[0].wing,"Lantern Survey Gallery"));
    assert(!std::strcmp(ExperienceSpecs[8].city,"Auvren"));
}
