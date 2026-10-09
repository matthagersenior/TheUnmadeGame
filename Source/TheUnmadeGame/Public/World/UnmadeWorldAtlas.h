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
inline constexpr std::array<RealmSpec, static_cast<int>(Realm::Count)> WorldAtlas = {{
    {Realm::ThreefoldReach, "The Threefold Reach", "roads stitched into contradictory maps",
     "the charterless settlements", "refuge or testimony", "witness gear and path relics", 0},
    {Realm::WidowedRain, "The Rain That Forgot the Sea", "permanent storm over hollow beaches",
     "the rain-counting flotillas", "whose absence buys fresh water", "weather-forged weapons", 1},
    {Realm::HearthBeneath, "The Hearth Beneath", "warm cave cities under a frozen sky",
     "keepers of the borrowed ember", "who owns a common flame", "bell and shelter armors", 1},
    {Realm::TidalLedger, "The Sea of Written Debts", "black tide that remembers future bargains",
     "the navigators of unsettled prices", "creditors foreclose on memories", "debt and oath blades", 2},
    {Realm::SkyBelow, "The Upside-Down Choir", "islands suspended beneath an inward sky",
     "the downward singers", "songs hold the gravity of cities", "gravity-changing relics", 2},
    {Realm::CinderSpine, "The Bones of Yesterdays", "a mountain range made from fossilized timelines",
     "the seam-walkers", "mining history erases descendants", "time-tempered smithing", 3},
    {Realm::HundredUnlived, "The Hundred Unlived", "cities built for people never born",
     "the unliving census", "who has the right to existence", "namebound artifacts", 3},
    {Realm::OrchardOfKings, "The Orchard of Unwritten Kings", "a forest of buried laws and crowns",
     "the untitled courts", "a kingdom without a past", "regalia that carries consequences", 4},
    {Realm::FirstAbsence, "The Place Before Place", "unformed land that refuses geometry",
     "the impossible witnesses", "whether reality should remain singular", "the Atlas of Absence", 5}
}};
struct AtlasPassage { Realm a,b; int attunement; };
inline constexpr std::array<AtlasPassage,13> AtlasPassages = {{
    {Realm::ThreefoldReach,Realm::WidowedRain,0},
    {Realm::ThreefoldReach,Realm::HearthBeneath,0},
    {Realm::WidowedRain,Realm::TidalLedger,1},
    {Realm::WidowedRain,Realm::SkyBelow,2},
    {Realm::HearthBeneath,Realm::SkyBelow,1},
    {Realm::HearthBeneath,Realm::CinderSpine,2},
    {Realm::TidalLedger,Realm::HundredUnlived,2},
    {Realm::SkyBelow,Realm::HundredUnlived,2},
    {Realm::SkyBelow,Realm::OrchardOfKings,3},
    {Realm::CinderSpine,Realm::OrchardOfKings,3},
    {Realm::HundredUnlived,Realm::FirstAbsence,4},
    {Realm::OrchardOfKings,Realm::FirstAbsence,4},
    {Realm::TidalLedger,Realm::CinderSpine,3}
}};
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
