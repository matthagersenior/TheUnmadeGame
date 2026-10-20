#pragma once
// The Threefold Reach objective guide reads canonical firsthand evidence,
// public choice, physically carried correspondence, and next-day reply.
// It NEVER changes quest states or reveals unseen evidence.
#include "World/UnmadeEvidenceProvenanceRules.h"
#include "World/UnmadeWitnessReturnRules.h"
namespace UnmadeCore {
enum class ThreefoldBeat : int {
    InspectBellwold, InspectPaperhaven, InspectCrossings, ReturnAfterCivicChoice,
    DecideAtOrrel, CollectOrrelRecord, DeliverToHessa, WaitAMorning,
    DecideHessaReply, CollectHessaReply, ReturnToOrrel, Completed, InvalidState
};
struct ThreefoldGuide {
    ThreefoldBeat beat;
    const char* city;
    const char* actor;
    const char* control;
    const char* prompt;
};
inline ThreefoldGuide NextThreefoldGuide(const WitnessEchoLedger&,
    const WitnessBraidChronicle&,const WitnessDispatch&,
    const WitnessReturn&,int) noexcept {
    return {ThreefoldBeat::InvalidState,"","","",
            "Prototype guidance not yet implemented."};
}
} // namespace UnmadeCore
