#pragma once
#include <cmath>
namespace UnmadeCore {
// Crouching behind ordinary wildlife avoids detection outside near reach.
// World geometry must independently confirm an unobstructed view.
inline bool CreatureCanSpot(bool clearSight,bool crouched,double distanceMeters,double facingDot) noexcept {
 if(!clearSight||!std::isfinite(distanceMeters)||distanceMeters<0||
    !std::isfinite(facingDot)||facingDot<-1||facingDot>1)return false;
 if(!crouched)return true;
 return distanceMeters<=2.5||(distanceMeters<=6.5&&facingDot>=0.35);
}
// Transient alertness, never saved or used as public testimony.
// Time is in active-world seconds, so pause and story UI do not decay it.
class CreatureAwareness {
 double awareUntil=0;
public:
 void Reset() noexcept {awareUntil=0;}
 bool Update(double now,bool sawPlayer) noexcept {
  if(!std::isfinite(now)||now<0)return false;
  if(sawPlayer){awareUntil=now+4.;return true;}
  return now<awareUntil;
 }
};
}
