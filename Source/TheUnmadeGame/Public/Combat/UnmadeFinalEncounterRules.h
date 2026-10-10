#pragma once
// THE UNMADE: the final enemy is a question, not a loot gate.
// Offline, deterministic portable C++17. The first "kill" is a mask breaking;
// the real victory changes the saved world, without resetting a character.
// A later rematch in the answered world is OPTIONAL.
#include "World/UnmadeLaterRealmRules.h"
#include <array>
#include <cmath>
#include <cstdint>
namespace UnmadeCore {
enum class FinalAct:int { Veil=0, Unmasked=1, AnswerPending=2,
    OtherMorning=3, EchoAwake=4, EchoSettled=5 };
enum class Morning:int { Unchosen=0, Anchor=1, Many=2 };
enum class FinalResult { Locked, MaskBroken, TrueVictory, NewMorning,
    EchoStarted, EchoDefeated, AlreadyDone, WrongStage, Invalid };
enum class FinalMove:int { Still=0, Approach=1, Telegraph=2, Strike=3,
    Stagger=4, Disengage=5 };
enum class FinalAttack:int { Bell=0, Horizon=1, Names=2, Counterfactual=3 };
struct FinalSnapshot {
    int act=0;
    int morning=0;
    int memorySeed=0; // stable 0-5 from recorded local choices, not random
};
struct FinalProfile {
    const char* name;
    const char* visibleLie;
    const char* maskReveal;
    const char* trueReveal;
    const char* returnTruth;
};
inline constexpr FinalProfile LastAdversary={
    "Nhal-Vey, The Answer That Ate Its Question",
    "I am the wound that keeps your world alive. You may kill me and keep everything you believe.",
    "That was only the version of me your world was willing to fight.",
    "Each town you saved is real. Each town you condemned is real. You are holding all of them together.",
    "I wasn't guarding the world's ending. I was guarding the door to its other beginning."
};
struct FinalWorldVariant {
    const char* name;
    const char* changedWorld;
    const char* bossReturn;
    const char* soundAndColor;
    const char* guarantee;
};
inline constexpr std::array<FinalWorldVariant,3> FinalWorlds={{
    {"The Unanswered Road","The original world with its unresolved public histories.",
     "The boss has not been defeated.","Established music and landmarks.","No saved state is rewritten."},
    {"The Held Morning",
     "Every realm returns changed, with visible seams preserving what its people decided; pathways remember each promise.",
     "Nhal-Vey returns as the Keeper of Unpaid Promises, using delayed oath and guard-breaking rhythm.",
     "Stone dawn, held bells, deliberate stable bass notes.","All earlier quests, items and people persist."},
    {"The Many Mornings",
     "The same nine lands return with coexisting outlines and contradictory public records; visitors can compare the versions without erasing either.",
     "Nhal-Vey returns as the Choir of Counterfactuals, alternating mirrored patterns with stable safe intervals.",
     "Double dawn, two-note music, accessible stable horizon.","All earlier quests, items and people persist."}
}};
inline constexpr std::array<const char*,9> PostMorningRecordsAnchor={{
    "Bellwold remembers every shelter built and every public truth, with its old streets unchanged beneath a new seam.",
    "The widowed coast keeps the missing sea's tide marks and the promises made to its ferrymen.",
    "The hearth's common flame casts two shadows but each household still owns its chosen obligations.",
    "Drevlach's estuary holds its debts in stone, so nobody may silently rewrite a child's tomorrow.",
    "Orravane's missing choir has a visible line in every public score.",
    "Vathless's quarry keeps the names of those its walls once consumed.",
    "Eillun's citizens can keep private doors without losing their right to shelter.",
    "Tharniv's untitled harvest endures without an imposed crown.",
    "Auvren's unbroken threshold keeps one truthful way back."
}};
inline constexpr std::array<const char*,9> PostMorningRecordsMany={{
    "Bellwold and Paperhaven share two readable versions of the same street; neither replaces a witnessed act.",
    "The rain falls toward both the sea and the missing sea, and its shore keeps the old ferry records.",
    "Every common hearth reflects two flames, each tended by real neighbors.",
    "The estuary runs with future and past receipts, while the protected borrower remains a person.",
    "The inverted choir's unheard parts are finally scored as possible music, without conscription.",
    "The stone of Vathless shows erased family memories next to the living mortar.",
    "Every unclaimed home keeps both a private invitation and a voluntary civic threshold.",
    "The orchard displays crowns that were refused and laws that never acquired rulers.",
    "Auvren presents two true beginnings with a stable exit marked for those who need it."
}};
inline const char* FinalLocalRecord(Morning variant,Realm realm) noexcept {
    const int i=static_cast<int>(realm);
    if(i<0||i>=9)return "";
    if(variant==Morning::Anchor)return PostMorningRecordsAnchor[i];
    if(variant==Morning::Many)return PostMorningRecordsMany[i];
    return "";
}
inline int FinalMemorySignature(int protectedChoices,int publishedChoices,
                               int sparedGuardians)noexcept {
    if(protectedChoices<0||publishedChoices<0||sparedGuardians<0)return 0;
    // Deliberately repeatable, bounded: recorded acts tune moves; no secret ML.
    return (protectedChoices+2*publishedChoices+3*sparedGuardians)%6;
}
class FinalJourney final {
public:
    const FinalSnapshot& Snapshot()const noexcept{return state_;}
    bool Restore(const FinalSnapshot& value)noexcept {
        if(value.act<0||value.act>5 ||value.morning<0||value.morning>2||
           value.memorySeed<0||value.memorySeed>5||
           (value.act<=2&&value.morning!=0)||
           (value.act==0&&value.memorySeed!=0)||
           (value.act>=3&&value.morning==0))return false;
        state_=value;return true;
    }
    FinalAct Act()const noexcept {return static_cast<FinalAct>(state_.act);}
    Morning World()const noexcept {return static_cast<Morning>(state_.morning);}
    int MemorySeed()const noexcept{return state_.memorySeed;}
    FinalResult BreakMask(bool firstRealmConcluded,int memorySeed)noexcept {
        if(!firstRealmConcluded)return FinalResult::Locked;
        if(memorySeed<0||memorySeed>5)return FinalResult::Invalid;
        if(state_.act!=0)return FinalResult::WrongStage;
        state_.act=1;state_.memorySeed=memorySeed;
        return FinalResult::MaskBroken;
    }
    FinalResult BreakCore()noexcept {
        if(state_.act!=1)return FinalResult::WrongStage;
        state_.act=2;return FinalResult::TrueVictory;
    }
    FinalResult ChooseWorld(Morning outcome)noexcept {
        if(outcome!=Morning::Anchor&&outcome!=Morning::Many)return FinalResult::Invalid;
        if(state_.act>=3)return FinalResult::AlreadyDone;
        if(state_.act!=2)return FinalResult::WrongStage;
        state_.morning=static_cast<int>(outcome);
        state_.act=3;return FinalResult::NewMorning;
    }
    FinalResult WakeEcho()noexcept {
        if(state_.act==4||state_.act==5)return FinalResult::AlreadyDone;
        if(state_.act!=3)return FinalResult::WrongStage;
        state_.act=4;return FinalResult::EchoStarted;
    }
    FinalResult SettleEcho()noexcept {
        if(state_.act==5)return FinalResult::AlreadyDone;
        if(state_.act!=4)return FinalResult::WrongStage;
        state_.act=5;return FinalResult::EchoDefeated;
    }
private:FinalSnapshot state_{};
};
struct FinalBeat {
    FinalMove move=FinalMove::Still;
    FinalAttack attack=FinalAttack::Bell;
    double damage=0;
    double warningSeconds=0;
    const char* warning="";
};
inline constexpr std::array<const char*,4> FinalWarnings={{
    "The last bell inhales twice. Its second ring is the attack; guard or leave the arc.",
    "The horizon splits from the middle. Step sideways before the two silhouettes meet.",
    "Your old name is spoken from behind you. The actual strike remains in front; follow the visible mark.",
    "A borrowed history folds inward. Leave its marked line until the counterfactual settles."
}};
// Stationary, directional attack footprints. The boss holds its facing once
// a tell starts, so stepping OUT of the marked area is legitimate counterplay.
// Forward/right coordinates are boss-local centimeters; no radial auto-hit.
inline bool FinalStrikeInFootprint(FinalAttack attack,double forwardCm,
                                  double rightCm)noexcept {
    if(!std::isfinite(forwardCm)||!std::isfinite(rightCm) ||
       forwardCm<0)return false;
    const double side=std::fabs(rightCm);
    switch(attack){
    case FinalAttack::Bell:
        return forwardCm<=520 && side<=370; // wide guarded toll
    case FinalAttack::Horizon:
        return forwardCm<=740 && side<=160; // long, thin horizon seam
    case FinalAttack::Names:
        return forwardCm<=440 && side<=240; // close single-name thrust
    case FinalAttack::Counterfactual:
        return forwardCm<=625 && rightCm>=80 && rightCm<=400; // right strip
    }
    return false;
}
class FinalBattleRhythm final {
public:
    void Reset()noexcept {armed_=false;warnUntil_=nextReady_=0;sequence_=0;}
    FinalBeat Advance(double now,double distanceCm,bool foldExposed,
                      FinalAct act,int memorySeed,Morning variant)noexcept {
        if(!std::isfinite(now)||now<0||!std::isfinite(distanceCm)||distanceCm<0||
           memorySeed<0||memorySeed>5)return {};
        if(act!=FinalAct::Veil&&act!=FinalAct::Unmasked&&act!=FinalAct::EchoAwake)
            return {};
        if(distanceCm>2200){armed_=false;return {FinalMove::Disengage,FinalAttack::Bell,0,0,""};}
        const int style=act==FinalAct::EchoAwake
            ? (variant==Morning::Anchor
                ? ((sequence_+memorySeed)%2)*2 // only Bell / Names: oath echoes
                : 1+((sequence_+memorySeed)%2)*2) // Horizon / Counterfactual
            : (sequence_+memorySeed+
               (act==FinalAct::Unmasked?1:0))%4;
        const auto attack=static_cast<FinalAttack>(style);
        const double range=attack==FinalAttack::Horizon?700:
                           attack==FinalAttack::Counterfactual?625:500;
        if(foldExposed) {
            armed_=false;nextReady_=now+2;
            return {FinalMove::Stagger,attack,0,0,""};
        }
        if(armed_) {
            if(now<warnUntil_)return {FinalMove::Telegraph,locked_,0,
                    warnDuration_,FinalWarnings[static_cast<int>(locked_)]};
            armed_=false;
            nextReady_=now+(act==FinalAct::EchoAwake?2.3:3.0);
            ++sequence_;
            if(distanceCm>lockedRange_)return {FinalMove::Approach,locked_,0,0,""};
            const double base=act==FinalAct::Veil?17:act==FinalAct::Unmasked?21:24;
            return {FinalMove::Strike,locked_,base+((locked_==FinalAttack::Names)?3:0),0,""};
        }
        if(distanceCm>range)return {FinalMove::Approach,attack,0,0,""};
        if(now<nextReady_)return {FinalMove::Still,attack,0,0,""};
        locked_=attack;lockedRange_=range;
        // Each act is visually different, but none hides an instant new input
        // mapping or a zero-warning, unavoidable strike.
        warnDuration_=act==FinalAct::EchoAwake
            ? (variant==Morning::Anchor?2.8:1.95)
            : (act==FinalAct::Unmasked?1.85:2.5);
        warnUntil_=now+warnDuration_;armed_=true;
        return {FinalMove::Telegraph,locked_,0,warnDuration_,
                FinalWarnings[static_cast<int>(locked_)]};
    }
private:
    bool armed_=false;double warnUntil_=0,nextReady_=0,warnDuration_=0,lockedRange_=0;
    int sequence_=0;FinalAttack locked_=FinalAttack::Bell;
};
} // namespace UnmadeCore
