#pragma once
#include <cmath>
#include <algorithm>
#include <cstdint>
namespace UnmadeCore {
// One reserved swing, one applied target. Contact is bounded even when a notify repeats.
class ContactWindow {
 double contact=-1,end=-1,ready=0;std::uint64_t swing=0,target=0;
public:
 bool Begin(std::uint64_t id,double now,double rhythm){if(!id||!std::isfinite(now)||!std::isfinite(rhythm)||now<0||rhythm<=0||now<ready)return false;swing=id;target=0;contact=now+std::clamp(rhythm*.22,.10,.25);end=contact+std::clamp(rhythm*.22,.10,.22);ready=now+rhythm;return true;}
 bool Active(double now)const{return std::isfinite(now)&&swing&&now>=contact&&now<=end&&!target;}
 bool Claim(std::uint64_t id,double now){if(!id||!Active(now))return false;target=id;return true;}
 std::uint64_t Id()const{return swing;}
 void Cancel(){swing=0;target=0;contact=end=-1;}
};
inline bool ProjectileFlightValid(double speed,double life){return std::isfinite(speed)&&std::isfinite(life)&&speed>=100&&speed<=4000&&life>0&&life<=8;}
class StaggerResistance {
 double next=0;
public:bool Take(double now,double duration,bool heavy){if(!std::isfinite(now)||!std::isfinite(duration)||now<0||duration<=0||now<next)return false;next=now+(heavy?2.5:1.);return true;}
};
}
