#pragma once
#include "Combat/UnmadeCombatStyleRules.h"
#include <cmath>

namespace UnmadeCore {
// Powers share their authored eight-metre reach in every spatial direction.
inline bool MasteryTargetInRange(double dx,double dy,double dz,bool targetable){
 return targetable&&std::isfinite(dx)&&std::isfinite(dy)&&std::isfinite(dz)&&
        dx*dx+dy*dy+dz*dz<800.*800.;
}
// Rejection must precede both the stamina expense and the twelve-second window.
inline bool TryMasteryPower(CombatTempo& tempo,double now,bool alive,bool enabled,bool staggered){
 return alive&&enabled&&!staggered&&tempo.Power(now);
}
enum class MasterySelection { Unavailable, Unchanged, Selected, SaveFailed };
template<class Depth,class Writer>
MasterySelection CommitMasterySelection(Depth& depth,int realm,Writer write){
 const auto* arc=depth.Arc(realm);
 if(!arc||!arc->choice)return MasterySelection::Unavailable;
 if(depth.Active()==realm)return MasterySelection::Unchanged;
 const auto before=depth;
 if(!depth.Select(realm))return MasterySelection::Unavailable;
 if(!write()){depth=before;return MasterySelection::SaveFailed;}
 return MasterySelection::Selected;
}
}
