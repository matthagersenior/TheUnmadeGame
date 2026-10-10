#pragma once
// Volume X: three physical firsthand objects support two human responses,
// but neither resolves the larger OPEN mystery.
#include "World/UnmadeWitnessEchoRules.h"
namespace UnmadeCore {

enum class WitnessBraidOutcome : int { Unresolved=0, ShelteredThread=1, PublicDocket=2 };
enum class WitnessBraidEvent : int { NotReady=0, Committed=1, AlreadyCommitted=2, InvalidChoice=3 };
struct WitnessBraidSnapshot { int outcome=0; };

class WitnessBraidChronicle final {
public:
    WitnessBraidSnapshot Snapshot() const noexcept { return state_; }
    WitnessBraidOutcome Outcome() const noexcept {
        return static_cast<WitnessBraidOutcome>(state_.outcome);
    }
    static bool Ready(const WitnessEchoLedger& evidence) noexcept {
        return evidence.HasFirst("CALL.01") && evidence.HasFirst("CALL.03") &&
               evidence.HasFirst("CALL.04") && evidence.ReturnCount()>=1;
    }
    bool Restore(WitnessBraidSnapshot saved,const WitnessEchoLedger& evidence) noexcept {
        if(saved.outcome<0 || saved.outcome>2 ||
           (saved.outcome!=0 && !Ready(evidence)))return false;
        state_=saved;
        return true;
    }
    WitnessBraidEvent Commit(int choice,const WitnessEchoLedger& evidence) noexcept {
        if(choice!=1 && choice!=2)return WitnessBraidEvent::InvalidChoice;
        if(state_.outcome!=0)return WitnessBraidEvent::AlreadyCommitted;
        if(!Ready(evidence))return WitnessBraidEvent::NotReady;
        state_.outcome=choice;
        return WitnessBraidEvent::Committed;
    }
    static const char* Warning(int choice) noexcept {
        if(choice==1)return "SHELTERED THREAD: place a private refuge guide. Travelers gain a safe marker, while the archive loses a public lead.";
        if(choice==2)return "PUBLIC DOCKET: display contradictory evidence without protected names. More people can challenge the record, but unwanted scrutiny may follow.";
        return "No valid public response selected.";
    }
    static const char* Investigation() noexcept {
        return "The uncounted refuge cup, the unmarked seal and the inkless nail contradict each other. The objects are real; none proves who erased the interval.";
    }
    const char* OutcomeText() const noexcept {
        switch(Outcome()) {
        case WitnessBraidOutcome::ShelteredThread:
            return "A refuge cord now guides unnamed visitors. The archive lacks a public lead; the original omission is unexplained.";
        case WitnessBraidOutcome::PublicDocket:
            return "A public docket compares three objects but names no protected guests. Orrel worries how it might be used.";
        default:
            return "The gathered clues can be compared at Orrel's nail. No cosmic verdict can be filed.";
        }
    }
private:
    WitnessBraidSnapshot state_{};
};
} // namespace UnmadeCore
