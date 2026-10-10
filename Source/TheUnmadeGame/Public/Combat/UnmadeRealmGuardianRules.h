#pragma once
// Six offline guardian trials. Actual warnings precede each hit; a true
// local Echo decision may end hostility without victory by attrition.
#include "World/UnmadeEchoQuestRules.h"
#include <array>
#include <cmath>
namespace UnmadeCore {
struct RealmGuardianSpec {
    Realm realm;
    const char* name;
    const char* origin;
    const char* warning;
    const char* careMemory;
    const char* truthMemory;
    double maxHealth;
    double damage;
    double windup;
    double recovery;
    double rangeCm;
};
inline constexpr std::array<RealmGuardianSpec,6> RealmGuardians={{
    {Realm::TidalLedger,"The Collector of Unborn Days",
     "An oathbreaker was made into a creditor so nobody could question who really wrote the contract.",
     "The collector unwinds a future chain; step beyond the arc or Fold its strike.",
     "The ferry keeps the collector's chains as proof no child's future may be pledged.",
     "The published record names those who forged debts in another person's time.",
     140,18,1.8,2.5,340},
    {Realm::SkyBelow,"The Missing Chorister",
     "A voice erased to keep the suspended quarter upright returns as a moving chord.",
     "The invisible note tightens; break sight, guard, or Fold before it falls.",
     "The new meter counts the erased singer among willing supporters.",
     "The open stage gives a name to the voice the council denied.",
     110,14,2.2,2.0,510},
    {Realm::CinderSpine,"The Mortar Ancestor",
     "A missing mason was conscripted into stone and left to guard the very houses they built.",
     "The ancestor raises a quarry hammer; retreat behind the seam or interrupt.",
     "The protected homes use stone no longer borrowed from erased families.",
     "The ancestor wall carries the worker's name in daylight.",
     175,21,2.4,3.0,350},
    {Realm::HundredUnlived,"The Numberless Examiner",
     "An involuntary census became an armed question that nobody should be forced to answer.",
     "A numbered pulse narrows; evade or use the covered addressway.",
     "The safe homes no longer ask for a registered beginning.",
     "The public threshold requires permission before recognizing a name.",
     115,15,1.65,2.75,440},
    {Realm::OrchardOfKings,"The Root Bailiff",
     "A root court learned to enforce laws that nobody actually approved.",
     "A law-root points at your feet; cross to the unbound row before impact.",
     "The shared harvest never demands an oath to a crown.",
     "The empty court records every challenge to a stolen title.",
     160,19,2.0,2.9,385},
    {Realm::FirstAbsence,"The Unfinished Witness",
     "The first guardian remembers two mutually true beginnings and refuses to declare either false.",
     "Two overlapping silhouettes prepare a strike; retreat into the stable path.",
     "The harbor holds a gentle doorway open to people who need refuge.",
     "The hall records different beginnings without calling a witness a liar.",
     185,20,2.5,3.1,420}
}};
enum class GuardianOutcome { Unresolved=0, Pacified=1, Defeated=2 };
enum class GuardianResolution { Locked, Pacified, Defeated, AlreadyResolved, Invalid };
struct GuardianSnapshot {std::array<int,6> outcomes{};};
class GuardianChronicle final {
public:
    const GuardianSnapshot& Snapshot()const noexcept{return state_;}
    bool Restore(const GuardianSnapshot& s)noexcept {
        for(int outcome:s.outcomes)
            if(outcome<0||outcome>2)return false;
        state_=s;return true;
    }
    GuardianOutcome Outcome(Realm r)const noexcept {
        const int i=LaterIndex(r);
        return i<0?GuardianOutcome::Unresolved:
            static_cast<GuardianOutcome>(state_.outcomes[i]);
    }
    GuardianResolution Resolve(Realm r,bool witnessedEcho,bool mercy)noexcept {
        const int i=LaterIndex(r);
        if(i<0)return GuardianResolution::Invalid;
        if(!witnessedEcho)return GuardianResolution::Locked;
        if(state_.outcomes[i]!=0)return GuardianResolution::AlreadyResolved;
        state_.outcomes[i]=mercy?1:2;
        return mercy?GuardianResolution::Pacified:GuardianResolution::Defeated;
    }
private: GuardianSnapshot state_{};
};
enum class GuardianAction { Idle, Telegraph, Strike, Approach, Retreat, Stagger };
class GuardianBeat final {
public:
    explicit GuardianBeat(Realm realm)noexcept: index_(LaterIndex(realm)){}
    GuardianAction Advance(double now,double distanceCm,bool foldExposed,bool active) noexcept {
        if(index_<0||!active || !std::isfinite(now) || now<0 ||
           !std::isfinite(distanceCm) || distanceCm<0)return GuardianAction::Idle;
        const auto& g=RealmGuardians[index_];
        if(distanceCm>1800){ Reset();return GuardianAction::Idle; }
        if(foldExposed){ windupUntil_=0;nextReady_=now+1.5;winding_=false;return GuardianAction::Stagger;}
        if(winding_) {
            if(now<windupUntil_)return GuardianAction::Telegraph;
            winding_=false;nextReady_=now+g.recovery;
            return distanceCm<=g.rangeCm?GuardianAction::Strike:GuardianAction::Approach;
        }
        if(distanceCm>g.rangeCm)return GuardianAction::Approach;
        if(now<nextReady_)return GuardianAction::Idle;
        winding_=true;windupUntil_=now+g.windup;
        return GuardianAction::Telegraph;
    }
    void Reset()noexcept{winding_=false;windupUntil_=nextReady_=0;}
private:
    int index_=-1;
    bool winding_=false;
    double windupUntil_=0,nextReady_=0;
};
} // namespace UnmadeCore
