#pragma once
// GENERATED FROM Authoring/ability_codex.json. Do not hand-edit.
// Regenerate with: python Scripts/generate_unmade_codex.py
#include <array>
namespace UnmadeCore {
inline constexpr std::array<const char*,4> CodexStudyNames = {{
    "Kiln Ward / Cinderjaw",
    "Seam Step / Pale Strider",
    "Redaction Read / Redaction Eye",
    "Memory Lens / Echo Well"
}};
inline constexpr std::array<std::array<const char*,6>,4> CodexStudyRequirements = {{
    {{"Personally witness a real attack warning by this living enemy species","Corroborate observation by dealing actual damage to the same living enemy species","Interpret both firsthand observations to reach Understanding","Complete experiment one at the actual subject with required Strain and time limits","Complete experiment two at the actual subject with a fresh attack warning when applicable","Master the skill physically, pay its cost, and commit the result to the character's save"}},
    {{"Personally witness a real attack warning by this living enemy species","Corroborate observation by dealing actual damage to the same living enemy species","Interpret both firsthand observations to reach Understanding","Complete experiment one at the actual subject with required Strain and time limits","Complete experiment two at the actual subject with a fresh attack warning when applicable","Master the skill physically, pay its cost, and commit the result to the character's save"}},
    {{"Personally witness a real attack warning by this living enemy species","Corroborate observation by dealing actual damage to the same living enemy species","Interpret both firsthand observations to reach Understanding","Complete experiment one at the actual subject with required Strain and time limits","Complete experiment two at the actual subject with a fresh attack warning when applicable","Master the skill physically, pay its cost, and commit the result to the character's save"}},
    {{"Personally inspect the physical Echo Well","Corroborate the Well through a real Strain-priced measurement","Interpret both firsthand observations to reach Understanding","Complete experiment one at the actual subject with required Strain and time limits","Complete experiment two at the actual subject with a fresh attack warning when applicable","Master the skill physically, pay its cost, and commit the result to the character's save"}}
}};
inline constexpr std::array<const char*,8> CodexAtlasRequirements = {{
    "Personally inspect the Well of Returned Voices",
    "Master the Memory Lens ability through firsthand experience",
    "Master two distinct hostile principles in physical combat",
    "Complete the Cartography rite through its existing staged quest",
    "Verify at least one route through physical frontier exploration",
    "Resolve The Place Before a First Story with two credible witnesses",
    "Return physically to the Echo Well to claim the artifact",
    "Keep a valid, writable character study and inventory save"
}};
struct CodexRealmSummary {const char* id;const char* name;const char* hub;int stage;bool initialFootholdSource;};
inline constexpr std::array<CodexRealmSummary,9> CodexRealmSummaries = {{
    {"realm.ThreefoldReach","The Threefold Reach","Crossings",0,true},
    {"realm.WidowedRain","The Rain That Forgot the Sea","Saltwake",1,true},
    {"realm.HearthBeneath","The Hearth Beneath","Cinderhold",1,true},
    {"realm.TidalLedger","The Sea of Written Debts","Drevlach",2,false},
    {"realm.SkyBelow","The Upside-Down Choir","Orravane",2,false},
    {"realm.CinderSpine","The Bones of Yesterdays","Vathless",3,false},
    {"realm.HundredUnlived","The Hundred Unlived","Eillun",3,false},
    {"realm.OrchardOfKings","The Orchard of Unwritten Kings","Tharniv",4,false},
    {"realm.FirstAbsence","The Place Before Place","Auvren",5,false}
}};
} // namespace UnmadeCore
