#include "World/UnmadeExpeditionRegistry.h"
#include "World/UnmadeCodexGenerated.h"
#include <cassert>
#include <cstring>
using namespace UnmadeCore;
int main() {
    static_assert(ExpeditionSpecs.size()==9,"Nine expedition rules must be Codex-generated");
    for(int i=0;i<9;++i) {
        const auto& e=ExpeditionSpecs[i];
        assert(e.realm==WorldAtlas[i].id);
        assert(std::strcmp(e.city,CodexRealmSummaries[i].hub)==0);
        assert(std::strlen(e.boss)>4);
        assert(std::strcmp(e.kill_return,e.mercy_return)!=0);
        for(const auto* person:e.people)assert(std::strlen(person)>1);
        for(const auto* clue:e.clues)assert(std::strlen(clue)>12);
    }
    assert(std::strcmp(ExpeditionSpecs[0].boss,"The Tollkeeper Without a Road")==0);
    assert(std::strcmp(ExpeditionSpecs[8].boss,"The Keeper of the Single Door")==0);
    assert(ExpeditionSpecs[0].x==0 && ExpeditionSpecs[0].y==0);
    assert(ExpeditionSpecs[8].x==-200000 && ExpeditionSpecs[8].y==0);
}
