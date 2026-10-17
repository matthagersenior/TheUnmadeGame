#pragma once
#include "Combat/UnmadeCreatureRules.h"
#include <cmath>
namespace UnmadeCore {
enum class WildActivity { Rest,Forage,Patrol };
struct WildRoutine { WildActivity activity=WildActivity::Rest; double offsetX=0,offsetY=0; };
// Resting remains still; nearby combat, return-to-territory and peaceful
// forage/patrol travel all deserve a real navigation update.
inline bool WantsCreatureNavigation(bool playerNearby,bool returningHome,bool routineTravel) noexcept {
 return playerNearby||returningHome||routineTravel;
}
// A normal creature waits for a readable opening instead of stacking attacks.
inline bool MayBeginOrdinaryWindup(bool clearSight,int otherActiveWarnings) noexcept {
 return clearSight&&otherActiveWarnings==0;
}
inline bool ShouldCircleForOpening(bool canAttack,bool alreadyWarning,int otherActiveWarnings) noexcept {
 return canAttack&&!alreadyWarning&&otherActiveWarnings>0;
}
inline WildRoutine CreatureRoutine(EnemyStyle style,double seconds,bool night,bool threatened)noexcept{
 if(!std::isfinite(seconds)||seconds<0||threatened)return {};
 const int slot=static_cast<int>(std::fmod(seconds,36.0)/12.0);
 if((night&&style==EnemyStyle::Stalker)||(!night&&style==EnemyStyle::Watcher))return {};
 if(slot==0)return {WildActivity::Forage,-280,180};
 if(slot==1)return {WildActivity::Patrol,300,-200};
 return {};
}
}
