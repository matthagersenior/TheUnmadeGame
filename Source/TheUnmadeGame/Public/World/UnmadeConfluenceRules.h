#pragma once
// Cross-discipline authored challenges. Every chamber needs multiple mastered
// rites, two distinct casts within a real window, and a witnessed conclusion.
#include "World/UnmadeTenfoldChronicle.h"
#include <array>
#include <cmath>
#include <cstdint>
namespace UnmadeCore {
enum class ConfluenceId : int {
    SilentAlarm, TwoNames, UnmappedWay, TomorrowPromise, TwoKeepers, FirstAbsence, Count
};
enum class ConfluenceResult {
    Advanced, CastRecorded, ReadyToDecide, Completed, AlreadyCompleted,
    NeedsMastery, WrongSite, NeedDualCast, NeedsWitnesses, NotReady, Invalid
};
struct ConfluenceSpec {
    ConfluenceId id;
    const char* name, *lore, *reward;
    RiteId first, second;
    int region;
    std::array<const char*,3> chapters;
};
inline constexpr std::array<ConfluenceSpec,6> Confluences = {{
    {ConfluenceId::SilentAlarm, "The Alarm Nobody Heard",
     "Bellwold's warning bell must fall silent long enough to stop the keeper, but witnesses must restore its message before the town sleeps.",
     "Aegis of the Last Warning", RiteId::UnwriteLaw,RiteId::Witnesscraft,0,
     {{"Find the warning stone beneath the broken shelter bell.",
       "Use Unwrite and Witnesscraft together at the stone before the signal vanishes.",
       "Ask two people who witnessed the change whether silence saved or abandoned them."}}},
    {ConfluenceId::TwoNames, "A Blade for Both Your Names",
     "The neverborn orchard sent a smith who was never apprenticed. Two versions of the hero must agree on which acts a weapon should remember.",
     "Edge of the Elseborn Smith",RiteId::BorrowedLives,RiteId::LegacyForging,1,
     {{"Discover the forge that exists only when two lives overlap.",
       "Borrow your other craft and temper a distinct remembered deed together.",
       "Choose whether the first or second self signs the weapon's history."}}},
    {ConfluenceId::UnmappedWay, "The Route Nobody Owned",
     "A passage with no legal owner will remain unstable until cartographers preserve both the safe road and the dangerous rumor.",
     "Chart of the Common Path",RiteId::LivingRoads,RiteId::Cartography,2,
     {{"Find a silent road ending where all three settlements disagree.",
       "Weave a road and trace a real route in the same fading interval.",
       "Take the verified map to witnesses who have crossed by different paths."}}},
    {ConfluenceId::TomorrowPromise, "Tomorrow Is Not Yours to Spend",
     "A borrowed dawn could light an entire refuge, but its lender has promised not to push the bill onto the helpless.",
     "Heart of the Borrowed Dawn",RiteId::TomorrowDebt,RiteId::Oathbinding,3,
     {{"Read the price of one more sunrise in the debt market.",
       "Borrow future strength while the protection oath remains unbroken.",
       "Choose an obligation and carry the coming cost without shifting it."}}},
    {ConfluenceId::TwoKeepers, "The Bell Fell and Stood",
     "A monster and its innocent keeper may stand in contradictory versions of the same place; mercy and truth need not annihilate one another.",
     "Mirror of the Two Keepers",RiteId::UnderstandingBosses,RiteId::ParadoxConvergence,4,
     {{"Visit the shattered ossuary and read both histories of the keeper.",
       "Hold the wounded monster and the unburied keeper in an overlap.",
       "Choose which testimony future witnesses must protect."}}},
    {ConfluenceId::FirstAbsence, "The Place Before a First Story",
     "Beyond the coast is a border with neither maps nor ownership. Ten learned disciplines offer choices but no automatic victory over existence itself.",
     "Witness of the First Absence",RiteId::UnwriteLaw,RiteId::Cartography,5,
     {{"Reach the final inscription only after every discipline and frontier is known.",
       "Unwrite one limitation and preserve the truthful way back before the interval ends.",
       "Choose whether the last crossing belongs to one history or many."}}}
}};
struct ConfluenceSnapshot {
    std::array<int,6> stage{}; // 0 unvisited, 1 practicing, 2 concluded
    std::array<int,6> decisions{};
    std::array<int,6> casts{}; // bit 1 first, bit 2 second
    std::array<double,6> windowEnds{};
};
class ConfluenceJourney final {
public:
    const ConfluenceSnapshot& Snapshot() const noexcept {return state_;}
    bool Restore(const ConfluenceSnapshot& s) noexcept {
        for(int i=0;i<6;++i){
            if(s.stage[i]<0 || s.stage[i]>2 ||
               s.decisions[i]<0 || s.decisions[i]>2 ||
               s.casts[i]<0 || s.casts[i]>3 ||
               !std::isfinite(s.windowEnds[i]) || s.windowEnds[i]<0 ||
               (s.stage[i]==2 && (s.decisions[i]==0 || s.casts[i]!=3)) ||
               (s.stage[i]<2 && s.decisions[i]!=0)) return false;
        }
        state_=s;return true;
    }
    int Stage(ConfluenceId id) const noexcept {
        const int i=static_cast<int>(id);return i>=0 && i<6?state_.stage[i]:0;
    }
    ConfluenceResult Discover(ConfluenceId id,int atSite,std::uint16_t mastered,
                               int frontierVisits) noexcept {
        const int i=static_cast<int>(id);
        if(i<0 || i>=6)return ConfluenceResult::Invalid;
        if(state_.stage[i]==2)return ConfluenceResult::AlreadyCompleted;
        if(state_.stage[i]!=0)return ConfluenceResult::NotReady;
        if(atSite!=i)return ConfluenceResult::WrongSite;
        const auto& spec=Confluences[i];
        const std::uint16_t required=std::uint16_t{1}<<static_cast<int>(spec.first) |
                                      std::uint16_t{1}<<static_cast<int>(spec.second);
        if((mastered & required)!=required)return ConfluenceResult::NeedsMastery;
        if(id==ConfluenceId::FirstAbsence) {
            if(mastered!=1023 || (frontierVisits&3)!=3)return ConfluenceResult::NeedsMastery;
            for(int x=0;x<5;++x)if(state_.stage[x]!=2)return ConfluenceResult::NeedsMastery;
        }
        state_.stage[i]=1;return ConfluenceResult::Advanced;
    }
    ConfluenceResult RecordCast(ConfluenceId id,RiteId spell,double now,int atSite) noexcept {
        const int i=static_cast<int>(id);
        if(i<0 || i>=6 || !std::isfinite(now) || now<0)return ConfluenceResult::Invalid;
        if(state_.stage[i]==2)return ConfluenceResult::AlreadyCompleted;
        if(state_.stage[i]!=1)return ConfluenceResult::NotReady;
        if(atSite!=i)return ConfluenceResult::WrongSite;
        const auto& spec=Confluences[i];
        int bit=0;
        if(spell==spec.first)bit=1;
        if(spell==spec.second)bit=2;
        if(bit==0)return ConfluenceResult::Invalid;
        if(now>state_.windowEnds[i]) {
            state_.casts[i]=0;
            state_.windowEnds[i]=now+120;
        }
        state_.casts[i]|=bit;
        return state_.casts[i]==3?ConfluenceResult::ReadyToDecide:ConfluenceResult::CastRecorded;
    }
    ConfluenceResult Resolve(ConfluenceId id,int atSite,int witnesses,
                              int choice,double now) noexcept {
        const int i=static_cast<int>(id);
        if(i<0 || i>=6 || !std::isfinite(now) || now<0 || (choice!=1 && choice!=2))
            return ConfluenceResult::Invalid;
        if(state_.stage[i]==2)return ConfluenceResult::AlreadyCompleted;
        if(state_.stage[i]!=1)return ConfluenceResult::NotReady;
        if(atSite!=i)return ConfluenceResult::WrongSite;
        if(state_.casts[i]!=3 || now>state_.windowEnds[i])
            return ConfluenceResult::NeedDualCast;
        if(witnesses<2)return ConfluenceResult::NeedsWitnesses;
        state_.stage[i]=2;
        state_.decisions[i]=choice;
        return ConfluenceResult::Completed;
    }
private:
    ConfluenceSnapshot state_{};
};
}
