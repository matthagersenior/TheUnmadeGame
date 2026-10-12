#pragma once
#include <array>
namespace UnmadeCore {
struct PlayerActionDef {const char* action;const char* title;const char* requirement;};
inline constexpr std::array<PlayerActionDef,28> PlayerActions={{
 {"AnomalyPulse","Inspect or Glimpse","Stand beside the real evidence, council or anomaly; existing branch confirmations apply."},
 {"FoldReality","Fold","Earn the power and approach its physical anchor or encounter."},
 {"RewriteOpen","Rewrite open","Approach the real anomaly; review the warning before confirming."},
 {"RewriteSealed","Rewrite sealed","Approach the real anomaly; review the warning before confirming."},
 {"FrontierTravel","Cross realm gateway","Stand beside an earned crossing; required attunement still applies."},
 {"RiteNext","Next discipline","Changes the selected discipline; grants no mastery."},
 {"RitePrevious","Previous discipline","Changes the selected discipline; grants no mastery."},
 {"RiteStudy","Study selected discipline","Read the current chapter and physical objective."},
 {"RiteUse","Invoke selected discipline","Earn the chapter and satisfy its real target, strain and cooldown requirements."},
 {"RiteChoice1","Discipline solidarity","Meet the actual chapter witness and confirmation requirements."},
 {"RiteChoice2","Discipline truth","Meet the actual chapter witness and confirmation requirements."},
 {"RiteBreak","Break selected oath","Existing authored oath and consequence conditions remain required."},
 {"RiteLaw","Cycle world law","Selects an earned law; its normal application conditions remain."},
 {"ConfluenceCycle","Next confluence","Selects a chamber experiment; grants no progress."},
 {"ConfluenceStudy","Study confluence","Read the two-power chamber requirement."},
 {"ConfluenceOption1","Confluence option one","Stand beside the real chamber and satisfy both power proofs."},
 {"ConfluenceOption2","Confluence option two","Stand beside the real chamber and satisfy both power proofs."},
 {"FactionSolidarity","Faction solidarity","Find the faction's actual concluding witness."},
 {"FactionTruth","Faction truth","Find the faction's actual concluding witness."},
 {"StoryShelter","Story shelter","Use the real story decision and its repeated confirmation."},
 {"StoryResearch","Story research","Use the real story decision and its repeated confirmation."},
 {"OfferAid","Offer aid","Approach a real person; their consent and saved eligibility remain authoritative."},
 {"SupplyActivity","Local supply activity","Use the existing physical supply objective."},
 {"FieldKit","Use field kit","Stand in an eligible danger chamber with an owned kit."},
 {"FieldTechnique","Use learned technique","Earn mastery; stamina, target and recovery requirements apply."},
 {"ItemStrain","Use strain vial","Requires an owned vial and strain that it can relieve."},
 {"ItemForge","Forge Waybreaker","Approach the existing forge with the required earned materials."},
 {"Sneak","Toggle sneak","Uses actual crouch movement and stealth checks; it grants no invisibility."}
}};
}
