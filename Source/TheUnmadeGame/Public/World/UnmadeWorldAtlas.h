#pragma once
// Future-world atlas, independent of Unreal. Named territories are DESIGN DATA,
// not loaded .umap files or claims that those locations are already playable.
#include "Items/UnmadeItemRules.h"
#include <array>
#include <vector>
#include <queue>
#include <algorithm>
namespace UnmadeCore {
enum class Realm : int {
    ThreefoldReach, WidowedRain, HearthBeneath, TidalLedger, SkyBelow,
    CinderSpine, HundredUnlived, OrchardOfKings, FirstAbsence, Count
};
struct RealmSpec {
    Realm id;
    const char* name;
    const char* landscape;
    const char* civilization;
    const char* centralConflict;
    const char* rareRewardTheme;
    int stage;
};
struct AtlasPassage { Realm a,b; int attunement; };
#include "World/UnmadeCodexAtlasGenerated.inc"
inline int AttunementFromRewards(const InventoryModel& inventory) noexcept {
    if (inventory.Quantity(ItemId::Waybreaker)>0 &&
        inventory.Quantity(ItemId::UnwrittenCrown)>0) return 4;
    if (inventory.Quantity(ItemId::Waybreaker)>0) return 3;
    if (inventory.HasClaimed(Achievement::AllLandmarks)) return 2;
    if (inventory.HasClaimed(Achievement::ThreeVillages)) return 1;
    return 0;
}
inline std::vector<Realm> PlanRealmRoute(Realm from, Realm to, int attunement) {
    const int count=static_cast<int>(Realm::Count);
    const int origin=static_cast<int>(from), goal=static_cast<int>(to);
    if(origin<0 || goal<0 || origin>=count || goal>=count || attunement<0 || attunement>4)
        return {};
    std::array<int,static_cast<int>(Realm::Count)> parent{};
    parent.fill(-1);
    std::queue<int> frontier;
    parent[origin]=origin;
    frontier.push(origin);
    while(!frontier.empty() && parent[goal]<0) {
        const int here=frontier.front();
        frontier.pop();
        for(const auto& edge:AtlasPassages) {
            if(edge.attunement>attunement) continue;
            int neighbor=-1;
            if(static_cast<int>(edge.a)==here) neighbor=static_cast<int>(edge.b);
            if(static_cast<int>(edge.b)==here) neighbor=static_cast<int>(edge.a);
            if(neighbor<0 || parent[neighbor]>=0) continue;
            parent[neighbor]=here;
            frontier.push(neighbor);
        }
    }
    if(parent[goal]<0) return {};
    std::vector<Realm> route;
    for(int cursor=goal;cursor!=origin;cursor=parent[cursor])
        route.push_back(static_cast<Realm>(cursor));
    route.push_back(from);
    std::reverse(route.begin(),route.end());
    return route;
}
}
