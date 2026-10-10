#pragma once
// Volume X: what the PLAYER has physically learned vs what a RESIDENT heard.
// Text is editorially attributed; a private theory must never become game truth.
#include "World/UnmadeWitnessBraidRules.h"
#include <array>
#include <cstring>

namespace UnmadeCore {
enum class EvidenceStage : int { Hidden=0, PersonallySeen=1, SeenAgain=2 };
struct EvidenceNotebookNode {
    const char* id;
    const char* city;
    const char* witnessId;
    const char* namedSpeaker;
    const char* theory;
    const char* challenge;
};
inline constexpr std::array<EvidenceNotebookNode,3> EvidenceNotebook = {{
    {"CALL.01","Bellwold","npc.bellwold.matron.001","Hessa",
     "Hessa suspects someone removed from the census chipped the refuge cup.",
     "Hessa cannot date the chip, and her guests have not consented to be named."},
    {"CALL.03","Paperhaven","npc.paperhaven.registrar.001","Sevrin",
     "Sevrin thinks the empty seal face records a deliberate limit on publication.",
     "The date stamped on the other side might be older than the press."},
    {"CALL.04","The Crossings","npc.bridgekeeper.001","Orrel",
     "Orrel suspects the road was taxed before builders agreed it existed.",
     "Three incompatible surveys do not reveal who or what changed the road."}
}};
inline EvidenceStage NotebookStage(const WitnessEchoLedger& ledger,const char* id) noexcept {
    if (!ledger.HasFirst(id)) return EvidenceStage::Hidden;
    return ledger.HasReturn(id) ? EvidenceStage::SeenAgain : EvidenceStage::PersonallySeen;
}
inline const EvidenceNotebookNode* KnownNotebookNode(const WitnessEchoLedger& ledger,
                                                     const char* id) noexcept {
    if(NotebookStage(ledger,id)==EvidenceStage::Hidden)return nullptr;
    for(const auto& node:EvidenceNotebook)
        if(id && std::strcmp(node.id,id)==0)return &node;
    return nullptr;
}
inline bool NotebookComparisonUnlocked(const WitnessEchoLedger& ledger) noexcept {
    return WitnessBraidChronicle::Ready(ledger);
}
enum class WitnessProvenance : int { NoEvidence=0, PersonallyWitnessed=1, HeardFromPerson=2 };
enum class WitnessBraidAccount : int { None=0, Shelter=1, Docket=2 };
inline const char* BraidAccountLine(WitnessBraidAccount outcome,
                                   WitnessProvenance source,
                                   const char* speakerId=nullptr) noexcept {
    if(source==WitnessProvenance::NoEvidence ||
       outcome==WitnessBraidAccount::None)return "";
    if(source==WitnessProvenance::HeardFromPerson && (!speakerId||!*speakerId))
        return ""; // Hearsay must identify an actual speaker.
    if(source==WitnessProvenance::PersonallyWitnessed)
        return outcome==WitnessBraidAccount::Shelter
            ? "I saw the refuge cord hung at the nail. It might keep travelers safe, but I cannot call the case closed."
            : "I saw the redacted docket posted. It names no guest; I worry who will search for the people it does not name.";
    return outcome==WitnessBraidAccount::Shelter
        ? "I heard from another resident that a refuge cord was hung. I have not visited the nail myself."
        : "Someone told me a redacted docket was posted. I have not seen it and cannot confirm their explanation.";
}
inline bool IsBraidEvent(const char* kind) noexcept {
    return kind && (std::strcmp(kind,"World.WitnessBraidShelter")==0 ||
                    std::strcmp(kind,"World.WitnessBraidDocket")==0);
}
inline WitnessBraidAccount BraidAccountFromEvent(const char* kind) noexcept {
    if(!kind)return WitnessBraidAccount::None;
    if(std::strcmp(kind,"World.WitnessBraidShelter")==0)return WitnessBraidAccount::Shelter;
    if(std::strcmp(kind,"World.WitnessBraidDocket")==0)return WitnessBraidAccount::Docket;
    return WitnessBraidAccount::None;
}
} // namespace UnmadeCore
