#pragma once
#include "Combat/UnmadeCreatureRules.h"
#include <cmath>
namespace UnmadeCore {
enum class WildActivity { Rest,Forage,Patrol };
struct WildRoutine { WildActivity activity=WildActivity::Rest; double offsetX=0,offsetY=0; };
inline WildRoutine CreatureRoutine(EnemyStyle style,double seconds,bool night,bool threatened)noexcept{
 if(!std::isfinite(seconds)||seconds<0||threatened)return {};
 const int slot=static_cast<int>(std::fmod(seconds,36.0)/12.0);
 if((night&&style==EnemyStyle::Stalker)||(!night&&style==EnemyStyle::Watcher))return {};
 if(slot==0)return {WildActivity::Forage,-280,180};
 if(slot==1)return {WildActivity::Patrol,300,-200};
 return {};
}
}
