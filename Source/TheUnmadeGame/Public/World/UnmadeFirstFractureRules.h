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
inline const char* FirstFractureCue(int realm)noexcept{
 return realm>=0&&realm<9?FirstFractureSpecs[realm].cue:nullptr;
}
inline const char* FirstFractureLine(int realm)noexcept{
 return realm>=0&&realm<9?FirstFractureSpecs[realm].sensory:nullptr;
}
} // namespace UnmadeCore
