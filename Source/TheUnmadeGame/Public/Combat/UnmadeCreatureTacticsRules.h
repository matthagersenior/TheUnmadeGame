#pragma once
// Twelve deterministic species tactics. No AI service, random decisions,
// extra rewards, or saved encounter records.
#include "Combat/UnmadeCreatureRules.h"
#include <array>
#include <cmath>
namespace UnmadeCore {
struct CreatureTacticSpec {
 int signature;
 const char* name;
 const char* cue;
 double cycle, feint;
 double retreatHealth;
 int flankSide;
};
inline CreatureTacticSpec CreatureTactic(CreatureKind kind) noexcept {
 static const std::array<CreatureTacticSpec,12> tactics={{
  {0,"Jaw pressure","The jaw lowers before a direct rush.",0,0,0,0},
  {1,"Strider feint","The strider checks its stride before lunging.",6,2.5,0,1},
  {2,"Veil orbit","The veil drifts to one side of its target.",5,1.8,.35,1},
  {3,"Moth flicker","The gleam moth veers before its flash.",4,1.1,.45,-1},
  {4,"Pack pressure","The hound hunts for a second angle.",7,1.5,0,1},
  {5,"Grief resonance","The lantern lingers before its peal.",8,2.2,.50,-1},
  {6,"Inkmaw crush","The inkmaw commits to a heavy strike.",0,0,0,0},
  {7,"Erasure arc","The eye slips laterally before marking a line.",6.5,2.0,.4,-1},
  {8,"Saltglass brace","The crawler waits out a near strike.",7.5,1.6,0,0},
  {9,"Drowned drift","The drowned watch shifts back to gain range.",5.5,1.5,.5,1},
  {10,"Ashmaw anchor","The ashmaw holds its ground before a slam.",9.0,1.9,0,0},
  {11,"Kiln shimmer","The wisp flickers out of the attack lane.",3.8,1.2,.6,-1}
 }};
 const auto n=static_cast<unsigned>(kind);
 return tactics[n<tactics.size()?n:0];
}
inline EnemyIntent TacticalCreatureIntent(CreatureKind kind,const CreatureProfile& profile,
 double distanceMeters,bool sight,bool recovering,double now,double healthFraction) noexcept {
 if(!std::isfinite(now)||now<0||!std::isfinite(healthFraction)||
    healthFraction<0||healthFraction>1)return EnemyIntent::Hold;
 const auto baseline=CreatureIntent(profile,distanceMeters,sight,recovering);
 if(baseline!=EnemyIntent::Attack)return baseline;
 const auto tactic=CreatureTactic(kind);
 if(tactic.retreatHealth>0&&healthFraction<tactic.retreatHealth)
   return EnemyIntent::Retreat;
 if(tactic.cycle>0&&std::fmod(now,tactic.cycle)<tactic.feint)
   return tactic.flankSide==0?EnemyIntent::Hold:EnemyIntent::Approach;
 return baseline;
}
} // namespace UnmadeCore
