#include "World/UnmadeWorldAtlas.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main() {
    assert(WorldAtlas.size()==9 && AtlasPassages.size()>=12);
    for(int i=0;i<9;++i) {
        const auto& region=WorldAtlas[i];
        assert(static_cast<int>(region.id)==i);
        assert(std::strlen(region.name)>9);
        assert(std::strlen(region.landscape)>14);
        assert(std::strlen(region.civilization)>12);
        assert(std::strlen(region.centralConflict)>10);
        assert(std::strlen(region.rareRewardTheme)>8);
        for(int j=0;j<i;++j)assert(std::strcmp(region.name,WorldAtlas[j].name)!=0);
        auto route=PlanRealmRoute(Realm::ThreefoldReach,region.id,4);
        assert(!route.empty() && route.front()==Realm::ThreefoldReach && route.back()==region.id);
    }
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,0).empty());
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,3).empty());
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::FirstAbsence,4).size()>3);
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::ThreefoldReach,0).size()==1);
    assert(PlanRealmRoute(static_cast<Realm>(-1),Realm::SkyBelow,4).empty());
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::SkyBelow,-1).empty());
    assert(PlanRealmRoute(Realm::ThreefoldReach,Realm::SkyBelow,99).empty());
    InventoryModel player;
    assert(AttunementFromRewards(player)==0);
    assert(player.Claim(Achievement::ThreeVillages)==RewardResult::Awarded);
    assert(AttunementFromRewards(player)==1);
    assert(player.Claim(Achievement::AllLandmarks)==RewardResult::Awarded);
    assert(AttunementFromRewards(player)==2);
    assert(player.Claim(Achievement::BellwoldLanterns)==RewardResult::Awarded);
    assert(player.Claim(Achievement::PaperhavenTestimony)==RewardResult::Awarded);
    assert(player.Claim(Achievement::EchoWell)==RewardResult::Awarded);
    assert(player.Claim(Achievement::FirstStalker)==RewardResult::Awarded);
    assert(player.ForgeWaybreaker());
    assert(AttunementFromRewards(player)==4);
    std::cout<<"PASS: nine distinct future realms, connected atlas, earned progression and gated routes\n";
}
