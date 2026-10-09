#pragma once
// Three faction narratives with personal NPC interactions and explicit endings.
#include "World/UnmadeSettlementRegistry.h"
#include <array>
#include <cstring>
namespace UnmadeCore {
enum class Faction { Refuge=0, Archive=1, Roadbound=2, Count=3 };
enum class FactionEnding { Unresolved=0, Solidarity=1, Truth=2 };
enum class FactionResult { NoChange, Advanced, NeedsEvidence, ChoiceRequired, Resolved };
struct FactionArc {
    Faction faction;
    const char* name;
    const char* first;
    const char* second;
    const char* third;
    const char* purpose;
};
inline constexpr std::array<FactionArc,3> FactionArcs = {{
    {Faction::Refuge,"The Refuge Compact",
     "npc.bellwold.lamplighter.001","npc.bellwold.matron.001","npc.bellwold.guard.001",
     "Find shared light; record witnesses; decide who may shelter."},
    {Faction::Archive,"The Redacted Charter",
     "npc.paperhaven.scribe.001","npc.paperhaven.registrar.001","npc.paperhaven.archivist.001",
     "Rescue testimony; verify the record; decide whether to publish."},
    {Faction::Roadbound,"The Roadbound Assembly",
     "npc.guard.001","npc.roadwarden.001","npc.welllistener.001",
     "Gather three roads' testimonies; debate who owns the crossings."}
}};
struct FactionSnapshot {
    std::array<int,3> stages{};
    std::array<int,3> endings{};
};
class FactionChronicle final {
public:
    const FactionSnapshot& Snapshot() const noexcept {return state_;}
    bool Restore(const FactionSnapshot& other) noexcept {
        for(int i=0;i<3;++i) {
            if(other.stages[i]<0 || other.stages[i]>3 ||
               other.endings[i]<0 || other.endings[i]>2 ||
               (other.endings[i]!=0 && other.stages[i]!=3)) return false;
        }
        state_=other;return true;
    }
    int Stage(Faction faction) const noexcept {
        const int i=static_cast<int>(faction);
        return i>=0 && i<3?state_.stages[i]:0;
    }
    FactionEnding Ending(Faction faction) const noexcept {
        const int i=static_cast<int>(faction);
        return i>=0 && i<3?static_cast<FactionEnding>(state_.endings[i]):FactionEnding::Unresolved;
    }
    FactionResult Converse(const char* residentId,bool hasLocalEvidence) noexcept {
        if(!residentId)return FactionResult::NoChange;
        for(const auto& arc:FactionArcs) {
            const int i=static_cast<int>(arc.faction);
            const int stage=state_.stages[i];
            if(stage==3) {
                if(state_.endings[i]==0 &&
                   std::strcmp(residentId,arc.third)==0)return FactionResult::ChoiceRequired;
                continue;
            }
            const char* expected=stage==0?arc.first:stage==1?arc.second:arc.third;
            if(std::strcmp(residentId,expected)!=0)continue;
            if(stage==2 && !hasLocalEvidence)return FactionResult::NeedsEvidence;
            ++state_.stages[i];
            return stage==2?FactionResult::ChoiceRequired:FactionResult::Advanced;
        }
        return FactionResult::NoChange;
    }
    FactionResult Decide(Faction faction,FactionEnding ending) noexcept {
        const int i=static_cast<int>(faction);
        if(i<0 || i>=3 || ending==FactionEnding::Unresolved ||
           static_cast<int>(ending)>2 || state_.stages[i]!=3 ||
           state_.endings[i]!=0)return FactionResult::NoChange;
        state_.endings[i]=static_cast<int>(ending);
        return FactionResult::Resolved;
    }
    int Reputation(Faction faction) const noexcept {
        const auto ending=Ending(faction);
        if(ending==FactionEnding::Unresolved)return Stage(faction)*5;
        if(ending==FactionEnding::Solidarity)return 40;
        return 25; // transparency also matters, without making either side evil
    }
    bool AllResolved() const noexcept {
        for(int outcome:state_.endings)if(outcome==0)return false;
        return true;
    }
private:
    FactionSnapshot state_{};
};
}
