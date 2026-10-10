#pragma once
// Six optional, offline dual-discipline sites. No remote AI, no random gates.
// These depend on existing earned RiteId and each realm's resolved Echo story.
#include "World/UnmadeEchoQuestRules.h"
#include "World/UnmadeTenfoldChronicle.h"
#include <array>
#include <cmath>
#include <cstdint>
namespace UnmadeCore {
struct RealmResonanceSpec {
    Realm realm;
    RiteId first,second;
    const char* title;
    const char* question;
    const char* openedArchive;
    const char* responseCare;
    const char* responseTruth;
};
inline constexpr std::array<RealmResonanceSpec,6> RealmResonances={{
    {Realm::TidalLedger,RiteId::TomorrowDebt,RiteId::Oathbinding,
      "The Calendar Without a Creditor",
      "Can a vow bind a future loan without making tomorrow's children pay?",
      "A sealed estuary calendar contains the names of people who declined to borrow, and every absence is preserved.",
      "The child-safe ferry keeps those unnamed dates under community guardianship.",
      "The public fraud archive shows where the impossible contracts began."},
    {Realm::SkyBelow,RiteId::UnwriteLaw,RiteId::ParadoxConvergence,
      "The Gravity of the Unheard",
      "Can two versions of the choir support the same house without erasing a singer?",
      "Two playable supporting harmonies coexist in the upper observatory; a builder records their common load.",
      "The shared meter supports households while forgotten parts are taught again.",
      "The open stage lets descendants hear names deliberately left out of the score."},
    {Realm::CinderSpine,RiteId::LegacyForging,RiteId::BorrowedLives,
      "The Ancestor Under the Anvil",
      "Can a borrowed craft repair a house without recasting a missing person's life as a tool?",
      "An observatory of named masonry discloses who shaped each stone and which families were compensated.",
      "A hearth brace stays standing without extracting another borrowed yesterday.",
      "The ancestor wall admits those whose names quarry records dismissed."},
    {Realm::HundredUnlived,RiteId::Witnesscraft,RiteId::BorrowedLives,
      "The Witness Without a Birth",
      "May a neverborn person testify without having to surrender their private life?",
      "A shelter-bound testimony desk verifies only consented statements, never a census key.",
      "Quiet homes keep testimony optional and unindexed.",
      "A public threshold recognizes consenting speakers, not uninvited identities."},
    {Realm::OrchardOfKings,RiteId::LivingRoads,RiteId::Oathbinding,
      "The Path the Crown Refused",
      "What happens when a vow to share a path outlives the ruler who denied it?",
      "An open arboretum records public access pledges alongside dissent; no crown appears as a quest reward.",
      "Shared harvest grows through agreement rather than inherited decrees.",
      "The vacant court keeps every disputed title within sight of a public road."},
    {Realm::FirstAbsence,RiteId::UnwriteLaw,RiteId::Cartography,
      "The Map That Kept Both Doors",
      "Can a map of incompatible beginnings make room for a safe return?",
      "An impossible cartographic chamber preserves both origin maps and one clearly marked safe departure.",
      "The harbor of starts remains a usable crossing for every visitor.",
      "The many mornings hall records incompatible histories without setting a victor."}
}};
enum class ResonanceResult { Locked, WrongAbility, Started, AlreadyRecorded,
    Opened, AlreadyOpened, ExpiredRestarted, ClockInvalid };
struct ResonanceSnapshot {
    std::array<int,6> stage{};  // 0 untouched; 1 one rite; 2 opened
    std::array<int,6> firstCast{}; // 0 none; 1 spec.first; 2 spec.second
    std::array<double,6> deadline{}; // in persisted world seconds, not transient UE time
};
class RealmResonanceJourney final {
public:
    static constexpr double WindowSeconds=120;
    const ResonanceSnapshot& Snapshot()const noexcept {return state_;}
    bool Restore(const ResonanceSnapshot& s) noexcept {
        for(int i=0;i<6;++i){
            if(s.stage[i]<0 || s.stage[i]>2 ||
               s.firstCast[i]<0 || s.firstCast[i]>2 ||
               !std::isfinite(s.deadline[i]) || s.deadline[i]<0 ||
               (s.stage[i]==0 && (s.firstCast[i]!=0 || s.deadline[i]!=0)) ||
               (s.stage[i]==1 && (s.firstCast[i]==0 || s.deadline[i]<=0)) ||
               (s.stage[i]==2 && (s.firstCast[i]==0 || s.deadline[i]!=0)))return false;
        }
        state_=s;return true;
    }
    int Stage(Realm realm) const noexcept {
        const int idx=LaterIndex(realm);
        return idx<0?0:state_.stage[idx];
    }
    ResonanceResult Record(Realm realm,RiteId spell,double worldSeconds,
                            bool echoResolved,bool mastered) noexcept {
        const int idx=LaterIndex(realm);
        if(idx<0 || !echoResolved || !mastered)return ResonanceResult::Locked;
        if(!std::isfinite(worldSeconds) || worldSeconds<0 ||
           worldSeconds>1.0e10)return ResonanceResult::ClockInvalid;
        const auto& spec=RealmResonances[idx];
        const int cast=spell==spec.first?1:spell==spec.second?2:0;
        if(cast==0)return ResonanceResult::WrongAbility;
        if(state_.stage[idx]==2)return ResonanceResult::AlreadyOpened;
        if(state_.stage[idx]==1 && worldSeconds<=state_.deadline[idx]) {
            if(cast==state_.firstCast[idx])return ResonanceResult::AlreadyRecorded;
            state_.stage[idx]=2;state_.deadline[idx]=0;
            return ResonanceResult::Opened;
        }
        const bool restarted=state_.stage[idx]==1;
        state_.stage[idx]=1;state_.firstCast[idx]=cast;
        state_.deadline[idx]=worldSeconds+WindowSeconds;
        return restarted?ResonanceResult::ExpiredRestarted:ResonanceResult::Started;
    }
private: ResonanceSnapshot state_{};
};
} // namespace UnmadeCore
