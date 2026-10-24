#pragma once
#include "World/UnmadeExpeditionRules.h"

namespace UnmadeCore {
// Runtime floods retain their existing twelve-second cycle. They do not add a
// saved suppression state or replay missed pulses while an owner is restoring.
inline DungeonHazard SampleWingFlood(double now,bool disabled){
 if(disabled||!std::isfinite(now)||now<0||now>1e12)return {};
 return {false,std::fmod(now,12.)>=9,static_cast<unsigned long long>(now/12)+1};
}
inline bool EncounterHazardEligible(bool ownerReady,bool alive,const DungeonHazard& hazard,
                                    double x,double y,double z){
 return ownerReady&&alive&&hazard.danger&&hazard.pulse!=0&&
        std::isfinite(x)&&std::isfinite(y)&&std::isfinite(z);
}
// Keep the chamber's strict horizontal/height edges and the wing's inclusive
// spherical reach. Sight and registered world ownership remain native checks.
inline bool EncounterHazardInCylinder(bool ownerReady,bool alive,const DungeonHazard& hazard,
                                      double x,double y,double z,double radius,double height){
 return EncounterHazardEligible(ownerReady,alive,hazard,x,y,z)&&
        std::isfinite(radius)&&radius>0&&std::isfinite(height)&&height>0&&
        std::hypot(x,y)<radius&&std::abs(z)<height;
}
inline bool EncounterHazardInSphere(bool ownerReady,bool alive,const DungeonHazard& hazard,
                                    double x,double y,double z,double radius){
 return EncounterHazardEligible(ownerReady,alive,hazard,x,y,z)&&
        std::isfinite(radius)&&radius>0&&std::hypot(std::hypot(x,y),z)<=radius;
}
}
