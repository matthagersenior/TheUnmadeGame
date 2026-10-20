#pragma once
// Player-facing, read-only objective from established Threefold Reach state.
// Never guesses hidden evidence, changes quest truth, grants items or teleports.
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
    const char* actor;   // Existing stable ID or physical-site tag; no new actor authority.
    const char* control;
    const char* prompt;  // Player-readable instruction, not private world truth.
};

// Re-validate the combined dependency graph before displaying guidance.
// Each Restore is performed on a temporary; neither live state nor save changes.
inline ThreefoldGuide NextThreefoldGuide(
        const WitnessEchoLedger& seen,
        const WitnessBraidChronicle& braid,
        const WitnessDispatch& dispatch,
        const WitnessReturn& reply,
        int currentDay) noexcept {
    if(currentDay<1 || currentDay>10000000)
        return {ThreefoldBeat::InvalidState,"","","NONE",
            "Progress needs a valid world clock. Preserve the existing save and inspect its record."};
    WitnessEchoLedger evidence;
    WitnessBraidChronicle decision;
    WitnessDispatch carried;
    WitnessReturn returned;
    if(!evidence.Restore(seen.Snapshot()) ||
       !decision.Restore(braid.Snapshot(),evidence) ||
       !carried.Restore(dispatch.Snapshot(),decision.Outcome()) ||
       !returned.Restore(reply.Snapshot(),carried,decision.Outcome(),currentDay))
        return {ThreefoldBeat::InvalidState,"","","NONE",
            "The witness record is inconsistent. Preserve it; do not overwrite or assume progress."};

    // Guide names towns and public institutions, not unseen objects or conclusions.
    if(!evidence.HasFirst("CALL.01"))
        return {ThreefoldBeat::InspectBellwold,"Bellwold",
            "region.bellwold.refuge","E",
            "Visit Bellwold's refuge entrance. Look closely at a local trace and inspect it in person."};
    if(!evidence.HasFirst("CALL.03"))
        return {ThreefoldBeat::InspectPaperhaven,"Paperhaven",
            "region.paperhaven.archive","E",
            "Travel to Paperhaven's Index Hall. Examine what the building keeps in public view."};
    if(!evidence.HasFirst("CALL.04"))
        return {ThreefoldBeat::InspectCrossings,"The Crossings",
            "npc.bridgekeeper.001","E",
            "Meet Orrel at the Crossings. Inspect an original trace near the bridge instead of trusting a rumor."};
    if(evidence.ReturnCount()==0)
        return {ThreefoldBeat::ReturnAfterCivicChoice,"Previously visited site",
            "site.callback.*","E",
            "After a genuine local civic outcome, revisit one of the three places you personally inspected. A repeated look without changed circumstances does not count."};

    if(decision.Outcome()==WitnessBraidOutcome::Unresolved)
        return {ThreefoldBeat::DecideAtOrrel,"The Crossings",
            "site.callback.inkless_nail","F7/F8",
            "Return to Orrel's nail. Choose private shelter or a redacted public record; press the same choice twice to confirm. The origin of the contradiction remains unknown."};

    if(carried.Stage()==WitnessDispatchStage::Waiting)
        return {ThreefoldBeat::CollectOrrelRecord,"The Crossings",
            "WitnessDispatch.FoldedRecord","E",
            "A folded, unnamed record waits beside Orrel's nail. Take it yourself, then carry it to Bellwold."};
    if(carried.Stage()==WitnessDispatchStage::PlayerCarrying)
        return {ThreefoldBeat::DeliverToHessa,"Bellwold",
            "npc.bellwold.matron.001","E",
            "Bring the original folded record to Hessa at Bellwold's refuge. Deliver it in person; she cannot know what was never brought."};

    if(returned.Stage()==WitnessReturnStage::AwaitingMorning) {
        if(!WitnessReturn::CanChoose(carried,decision.Outcome(),currentDay))
            return {ThreefoldBeat::WaitAMorning,"Bellwold",
                "npc.bellwold.matron.001","WAIT",
                "Hessa has received the record. Give her one complete in-world morning to consider it; then return to her at Bellwold."};
        return {ThreefoldBeat::DecideHessaReply,"Bellwold",
            "npc.bellwold.matron.001","F7/F8",
            "Speak to Hessa after the new morning. Choose private counsel or a redacted public hearing; press the same choice twice before she prepares the reply."};
    }
    if(returned.Stage()==WitnessReturnStage::ReplyPrepared)
        return {ThreefoldBeat::CollectHessaReply,"Bellwold",
            "WitnessReturn.ReplyNote","E",
            "Hessa has prepared her answer. Stand beside the physical folded reply and press E to take it."};
    if(returned.Stage()==WitnessReturnStage::PlayerCarrying)
        return {ThreefoldBeat::ReturnToOrrel,"The Crossings",
            "npc.bridgekeeper.001","E",
            "Carry Hessa's physical reply back to Orrel at the Crossings. Speak to him in person; neither settlement is automatically informed."};
    return {ThreefoldBeat::Completed,"The Crossings",
        "npc.bridgekeeper.001","JOURNAL",
        "Orrel received Hessa's response. The third road remains disputed; the original mystery is not resolved. Visit the witnesses again to see what they personally know."};
}
} // namespace UnmadeCore
