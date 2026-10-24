#pragma once
// Only firsthand sensory contradictions from physically crossed thresholds.
// These are observations, not evidence that someone else consented or that
// the player already knows the world-ending answer.
#include <array>
namespace UnmadeCore {
struct FirstFractureSpec {const char* cue;const char* sensory;};
inline constexpr std::array<FirstFractureSpec,9> FirstFractureSpecs={{
 {"first_fracture_crossings","A bell shivers behind the stone, but its final note finds no road."},
 {"first_fracture_saltwake","A wave hits a dry wall. The salt on your hand remains wet."},
 {"first_fracture_cinderhold","An ember draws breath backward; another family feels the warmth leave."},
 {"first_fracture_drevlach","A receipt ticks forward one day though the debtor has not lived it."},
 {"first_fracture_orravane","A missing choir note carries weight. The overhead span tilts but does not fall."},
 {"first_fracture_vathless","Two footprints occupy one ancestral seam; both sound like real feet."},
 {"first_fracture_eillun","A narrow bed is warm where the census records an empty room."},
 {"first_fracture_tharniv","An unclaimed root moves at the command of a crown no one wears."},
 {"first_fracture_auvren","Your shadow crosses a threshold before your body; both routes remain open."}
}};
// Graybox source hooks for first-hand, revisitable physical anomalies.
// Positions are relative to the actual first room, never unseen later rooms.
struct FirstFracturePhysicalSpec {
 int dx,dy;
 double scaleX,scaleY,scaleZ,spin;
 const char* investigation;
};
inline constexpr std::array<FirstFracturePhysicalSpec,9> FirstFracturePhysicals={{
 {-540,330, .6,1.8,2.8,.65,
  "Count the missing interval against the charters etched into the nearest stones. Reach the next real chamber and question the local shelter witness before naming the denied road."},
 {450,480, 2.2,.35,.7,.9,
  "Examine how salt clings to the drywater pressure gauge. Follow the actual pump passage, then ask whose water claim was measured rather than believing the first invoice."},
 {-470,-430, 1.8,1.4,.35,1.15,
  "Compare the backward ember's warmth with the other side of the common vent. Look for a physical furnace record and ask which home loses heat when this channel opens."},
 {660,190, 1.5,.25,2.3,.55,
  "Observe the stamped date on the hanging debt strip. Follow the pier ledger and its counterfoil in person before treating a tomorrow not yet lived as an enforceable oath."},
 {-400,560, 2.1,.25,1.5,1.3,
  "Match the unsung rest to the load-bearing choir stave. Climb toward the real counterweight path and compare the dissenting score before touching its anchors."},
 {580,-250, .45,2.2,1.9,1.45,
  "Trace the incompatible footsteps across the same ancestral seam. Enter the next quarry passage and recover both physical dates before accepting a single family's claim."},
 {-580,410, 1.7,.7,.4,.7,
  "Check the warmth of the occupied census bed rather than copying its erased name. Search the actual shelter record and speak to the resident without exposing their identity."},
 {400,-570, 1.6,.8,1.8,1.0,
  "Follow the moving root back toward its unsigned graft. Inspect the public passage and listen to households before letting an empty crown govern their harvest."},
 {-620,-480, 1.2,1.2,2.1,2.0,
  "Watch where shadow and body cross different thresholds. Explore the physically reachable passage and compare the two maps without declaring either one the beginning."}
}};
inline const FirstFracturePhysicalSpec* FirstFracturePhysical(int realm)noexcept{
 return realm>=0&&realm<9?&FirstFracturePhysicals[realm]:nullptr;
}
inline const char* FirstFractureCue(int realm)noexcept{
 return realm>=0&&realm<9?FirstFractureSpecs[realm].cue:nullptr;
}
inline const char* FirstFractureLine(int realm)noexcept{
 return realm>=0&&realm<9?FirstFractureSpecs[realm].sensory:nullptr;
}
} // namespace UnmadeCore
