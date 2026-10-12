#pragma once
#include <array>
namespace UnmadeCore {
struct EncounterPerformance {const char* warning;double propX,propY,propZ;int shape;};
inline constexpr std::array<EncounterPerformance,9> EncounterPerformances={{
 {"The charter is raised before the marked road is struck.",.25,1.8,1.0,0},
 {"The bailiff draws back its pressure lantern; leave the wet mark.",.7,.7,1.2,1},
 {"The provost braces its furnace weight; the heated lane is next.",1.5,.5,.5,0},
 {"The warden stretches its oath hook across the warned arc.",.3,.3,1.7,2},
 {"The choir arm rises; step beyond the falling measure.",1.8,.2,.7,0},
 {"The buried counterweight tips toward the paired footprint.",.8,1.1,.6,0},
 {"The census lamp opens; break its locked line of sight.",.8,.8,.8,1},
 {"The root crown bends; leave the branching pressure mark.",1.0,1.0,1.3,2},
 {"The threshold frame aligns before the near-side strike.",.2,1.4,1.8,0}
}};
inline double MotionHold(int kind){return kind==3?.75:kind==2?.55:kind==1?.4:0;}
}
