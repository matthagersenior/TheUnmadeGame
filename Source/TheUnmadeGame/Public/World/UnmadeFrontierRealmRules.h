#pragma once
// Two authored frontier realms beyond the Threefold Reach. The atlas also defines
// seven additional future destinations; these two are the first graybox footholds.
#include "World/UnmadeWorldAtlas.h"
#include "NPC/UnmadeNpcDecisionRules.h"
#include <array>
#include <cstring>
#include <cmath>
#include <cstdint>
namespace UnmadeCore {
struct FrontierOutpost {
    Realm realm;
    const char* settlementName;
    double centerX,centerY;
    const char* dayDescription;
    const char* nightDescription;
    const char* mystery;
};
inline constexpr std::array<FrontierOutpost,2> FrontierOutposts = {{
    {Realm::WidowedRain,"Saltwake, the Rainward Port",0.0,-50000.0,
     "Ships lie beached under rain that has forgotten its sea.",
     "Every roof rings with a tide that no shore remembers.",
     "A wet ledger names an ocean that no map contains."},
    {Realm::HearthBeneath,"Cinderhold, the Borrowed Hearth",0.0,50000.0,
     "Cavern roofs cradle a sun that never rose.",
     "Warmth travels from home to home like a living promise.",
     "A common ember burns without consuming its fuel."}
}};
struct FrontierResident {
    const char* id;const char* name;Realm home;NpcRole role;NpcTemperament temperament;
    double localX,localY;const char* authoredLine;
};
inline constexpr std::array<FrontierResident,16> FrontierResidents = {{
    {"npc.saltwake.navigator.001","Sella the drowned navigator",Realm::WidowedRain,NpcRole::Scholar,NpcTemperament::Curious,-550,400,"I chart rainfall now; the sea is an unreliable witness."},
    {"npc.saltwake.harborwarden.001","Harrow the harbor warden",Realm::WidowedRain,NpcRole::Guard,NpcTemperament::Steady,690,-500,"I guard the harbor even though no ship can dock."},
    {"npc.saltwake.netmender.001","Olsa the net mender",Realm::WidowedRain,NpcRole::Merchant,NpcTemperament::Cautious,-850,-300,"Every net catches a different yesterday."},
    {"npc.saltwake.rainkeeper.001","Thenn the rain keeper",Realm::WidowedRain,NpcRole::Scholar,NpcTemperament::Steady,350,800,"We count each drop so nobody sells us the same storm twice."},
    {"npc.saltwake.courier.001","Mera the tide courier",Realm::WidowedRain,NpcRole::Courier,NpcTemperament::Curious,1000,350,"A letter crossed the sea last week. There is no sea."},
    {"npc.saltwake.gatherer.001","Donn the salt gatherer",Realm::WidowedRain,NpcRole::Wanderer,NpcTemperament::Cautious,-980,800,"Salt grows under the grass where the ocean died."},
    {"npc.saltwake.broker.001","Iva the water broker",Realm::WidowedRain,NpcRole::Merchant,NpcTemperament::Steady,850,-1050,"Water has a price. Remembering it should not."},
    {"npc.saltwake.watcher.001","Lorn the wave watcher",Realm::WidowedRain,NpcRole::Guard,NpcTemperament::Curious,-200,-1120,"When the waves return, they will return all at once."},
    {"npc.cinderhold.emberwarden.001","Rheva the ember warden",Realm::HearthBeneath,NpcRole::Guard,NpcTemperament::Steady,480,340,"Each family may keep the flame. No family may own it."},
    {"npc.cinderhold.stonecarver.001","Garn the stonecarver",Realm::HearthBeneath,NpcRole::Merchant,NpcTemperament::Cautious,-900,-230,"Every wall carries the mark of an older roof."},
    {"npc.cinderhold.hearthreader.001","Tarin the hearth reader",Realm::HearthBeneath,NpcRole::Scholar,NpcTemperament::Curious,-530,850,"The fire shows lives we might have lived."},
    {"npc.cinderhold.courier.001","Bes the coal runner",Realm::HearthBeneath,NpcRole::Courier,NpcTemperament::Steady,950,650,"I carry heat where the mountain refuses to share it."},
    {"npc.cinderhold.gardener.001","Irel the ash gardener",Realm::HearthBeneath,NpcRole::Wanderer,NpcTemperament::Curious,150,-1000,"My roots grow toward light they have never seen."},
    {"npc.cinderhold.healer.001","Ovenna the warmth keeper",Realm::HearthBeneath,NpcRole::Scholar,NpcTemperament::Steady,-1040,450,"The warmest shelter is the one nobody is denied."},
    {"npc.cinderhold.shieldsmith.001","Orven the shieldsmith",Realm::HearthBeneath,NpcRole::Merchant,NpcTemperament::Cautious,870,-800,"A shield is a promise that the person behind it matters."},
    {"npc.cinderhold.sentry.001","Kael the deep sentry",Realm::HearthBeneath,NpcRole::Guard,NpcTemperament::Steady,-880,-900,"We keep the tunnel open even to those we fear."}
}};
inline const FrontierOutpost* FindFrontier(Realm realm) noexcept {
    for(const auto& outpost:FrontierOutposts)if(outpost.realm==realm)return &outpost;
    return nullptr;
}
inline const FrontierResident* FindFrontierResident(const char* id) noexcept {
    if(!id)return nullptr;
    for(const auto& resident:FrontierResidents)
        if(std::strcmp(resident.id,id)==0)return &resident;
    return nullptr;
}
inline int FrontierIndex(Realm realm) noexcept {
    if(realm==Realm::WidowedRain)return 0;
    if(realm==Realm::HearthBeneath)return 1;
    return -1;
}
struct FrontierSnapshot {
    int visits=0, discoveries=0;
    std::array<int,2> stages{};
    std::array<int,2> endings{};
};
enum class FrontierEvent { NoChange, Advanced, NeedClue, FinalChoice, Resolved };
class FrontierJourney final {
public:
    const FrontierSnapshot& Snapshot() const noexcept {return state_;}
    bool Restore(const FrontierSnapshot& s) noexcept {
        if(s.visits<0 || (s.visits&~3) || s.discoveries<0 || (s.discoveries&~3))
            return false;
        for(int i=0;i<2;++i)
            if(s.stages[i]<0 || s.stages[i]>3 || s.endings[i]<0 || s.endings[i]>2 ||
               (s.endings[i]>0 && s.stages[i]!=3))return false;
        state_=s;return true;
    }
    FrontierEvent Visit(Realm realm) noexcept {
        const int i=FrontierIndex(realm);if(i<0)return FrontierEvent::NoChange;
        const int bit=1<<i;if(state_.visits&bit)return FrontierEvent::NoChange;
        state_.visits|=bit;return FrontierEvent::Advanced;
    }
    FrontierEvent FindClue(Realm realm) noexcept {
        const int i=FrontierIndex(realm);if(i<0 || !(state_.visits&(1<<i)))return FrontierEvent::NoChange;
        const int bit=1<<i;if(state_.discoveries&bit)return FrontierEvent::NoChange;
        state_.discoveries|=bit;return FrontierEvent::Advanced;
    }
    FrontierEvent Converse(const char* residentId) noexcept {
        const auto* r=FindFrontierResident(residentId);
        if(!r)return FrontierEvent::NoChange;
        const int i=FrontierIndex(r->home),stage=state_.stages[i];
        const char* first=i==0?"npc.saltwake.navigator.001":"npc.cinderhold.hearthreader.001";
        const char* second=i==0?"npc.saltwake.rainkeeper.001":"npc.cinderhold.healer.001";
        const char* third=i==0?"npc.saltwake.harborwarden.001":"npc.cinderhold.emberwarden.001";
        if(stage==3) return state_.endings[i]==0 && std::strcmp(residentId,third)==0
            ? FrontierEvent::FinalChoice:FrontierEvent::NoChange;
        const char* expected=stage==0?first:stage==1?second:third;
        if(std::strcmp(residentId,expected)!=0)return FrontierEvent::NoChange;
        if(stage==2 && !(state_.discoveries&(1<<i)))return FrontierEvent::NeedClue;
        ++state_.stages[i];
        return stage==2?FrontierEvent::FinalChoice:FrontierEvent::Advanced;
    }
    FrontierEvent Resolve(Realm realm,int choice) noexcept {
        const int i=FrontierIndex(realm);
        if(i<0 || (choice!=1 && choice!=2) || state_.stages[i]!=3 || state_.endings[i]!=0)
            return FrontierEvent::NoChange;
        state_.endings[i]=choice;
        return FrontierEvent::Resolved;
    }
    bool IsResolved(Realm realm) const noexcept {
        const int i=FrontierIndex(realm);
        return i>=0 && state_.endings[i]!=0;
    }
private:
    FrontierSnapshot state_{};
};
}
