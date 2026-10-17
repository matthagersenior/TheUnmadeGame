#pragma once
// Optional, spoiler-aware discovery and opening guidance derived exclusively
// from persisted expedition/depth records. Does not grant progress.
#include "World/UnmadeAdventureDepthRules.h"
#include <array>
namespace UnmadeCore {
struct OpeningBeat {int stage;const char* title;const char* objective;const char* cinematicCue;};
inline OpeningBeat NextOpeningBeat(const ExpeditionState& e,const DepthArc& personal,bool hometownKnown) noexcept {
 if(!e.brief)return {0,"The Bell That Had No Last Note",
   "Begin in the Crossings. Listen to Bellwold or Paperhaven, or examine a landmark before taking a side.",
   "A bell is heard where the road has no matching witness. No narrator tells you which account is true."};
 if(!(e.visited&1))return {1,"The First Wrong Road",
   "Follow the frontier directions from the recorded adventure board. Do not abandon the settlement's unanswered question.",
   "The original witness leaves. The road's silhouette contradicts the town record."};
 if(!(e.rooms&1))return {2,"What the Door Remembers",
   "Find the physical entrance. Inspect it, then enter the first chamber; the route can be tried again.",
   "A close sound from the passage replies to a distant settlement bell."};
 if(!e.branch)return {3,"The Cost of Evidence",
   "In the holding chambers, rescue the two captives OR examine all three records; these are different but valid resolutions.",
   "Hold on the people and evidence. Explain the choice without declaring either witness a liar."};
 if(!e.boss)return {4,"An Answer That Can Be Refused",
   "Resolve the arena confrontation. On the evidence path, a living surrender may be possible.",
   "Show the attack warning, the chance to retreat, and the enemy's response to the chosen resolution."};
 if(!e.returned)return {5,"The Return Has Witnesses",
   "Follow the real return route and speak to its leader; the world must acknowledge what you actually did.",
   "Return to the same street; show what changed, not a new generic victory shot."};
 if(!e.reward)return {6,"The Earned Tool",
   "Claim the earned weapon or armor at the frontier bench; inspect the difference before equipping.",
   "The same hands accept a tool built from a witnessed consequence."};
 if(!personal.offered)return {7,"The Person Beyond the Quest",
   "Find the local leader again for their personal account. Optional stories must not block the main road.",
   "A named person continues living after the hero's reward."};
 if(!personal.witness||!personal.evidence)return {8,"Someone Disagrees",
   "Hear the named witness and inspect the physical personal evidence before deciding.",
   "Let the witness tell their own version; no voiceover speaks for them."};
 if(!personal.choice)return {9,"An Accord That Remains",
   "Preview the protected reserve or public witness and deliberately confirm the choice. Both have later costs.",
   "The choice returns to a local person's face and the road they must live with."};
 (void)hometownKnown;
 return {10,"The Road Is Not Finished",
   "Return on another day, inspect how people and services changed, or pursue the next main-story road.",
   "The same people remember the choice; the camera does not call the matter solved."};
}
inline const char* NextInvestigationLead(int realm,const ExpeditionState& e,const DepthArc& p) noexcept {
 static constexpr std::array<const char*,9> routeQuestions={{
  "Crossings: compare the shelter bell and the civic record, then follow the marked control order.",
  "Saltwake: find the exposed tide gauge; test the three controls during the quiet flood interval.",
  "Cinderhold: trace the borrowed ember through the furnace; stabilize all three controls before cooling ends.",
  "Drevlach: compare the impossible debt entries; a lawful-looking tally is not a true witness.",
  "Orravane: read the suspended weight score from above, then reach all three elevated controls.",
  "Vathless: follow the seam's counted footsteps; an interrupted sequence needs a fresh attempt.",
  "Eillun: find a truthful quiet interval before using the three census stations.",
  "Tharniv: read the root court's reverse order; the third control is not always the last.",
  "Auvren: compare the two absent signatures before opening the contradiction gate."
 }};
 if(realm<0||realm>=9)return "Unknown realm: follow a documented route before seeking a local investigation.";
 if(!e.brief)return "Find this city's adventure board before interpreting its hidden roads.";
 if(!(e.visited&1))return "Reach the recorded frontier first. Its local witnesses and route mechanism are physical.";
 if(!p.route)return routeQuestions[realm];
 if(!p.secret)return "The side passage is open. Inspect its real cache beyond the route controls; rewards are finite.";
 if(!p.offered)return "The cache is documented. Return to the leader for the optional local person's account.";
 if(!p.witness)return "Listen to the local witness in person; a rumor alone is not the needed evidence.";
 if(!p.evidence)return "Use the physical evidence marker in chamber four. Do not mistake a journal rumor for proof.";
 if(!e.boss)return "The evidence is complete, but the existing arena resolution is still needed.";
 if(!p.choice)return "Return to the leader and explicitly confirm reserve or public witness. Both are permanent.";
 return "The accord is recorded. Return later and ask residents what actually changed.";
}
} // namespace UnmadeCore
