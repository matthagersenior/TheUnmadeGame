#pragma once
#include "World/UnmadeSettlementRegistry.h"
#include "World/UnmadeBesHouseholdRules.h"
#include "World/UnmadeTenfoldChronicle.h"
#include <cmath>
namespace UnmadeCore {
enum class TimedProgress { Deferred, Unchanged, Saved, FirstFailure, FailedAgain };
// Session-only retry cadence; never consumes a day, arrival or reward on failure.
class TimedProgressRetry final {
public:
 template<class Commit> TimedProgress Attempt(bool ready,double now,Commit commit){
  if(!ready||!std::isfinite(now)||now<0||
     (failed_&&(now<lastAttempt_||now-lastAttempt_<5)))return TimedProgress::Deferred;
  const bool first=!failed_;lastAttempt_=now;
  if(commit()){failed_=false;return TimedProgress::Saved;}
  failed_=true;return first?TimedProgress::FirstFailure:TimedProgress::FailedAgain;
 }
private:
 bool failed_=false;double lastAttempt_=0;
};
inline bool AdvanceWorldClock(LivingWorldClock& clock,bool ready,double delta){
 return ready&&clock.Advance(delta);
}
template<class Writer> TimedProgress CommitSettlementVisit(SettlementVisits& visits,
 SettlementId village,bool ready,double now,TimedProgressRetry& retry,Writer write){
 auto next=visits;
 if(!next.Visit(village))return TimedProgress::Unchanged;
 return retry.Attempt(ready,now,[&]{
  const auto before=visits;visits=next;
  if(write())return true;
  visits=before;return false;
 });
}
template<class Writer> TimedProgress CommitRiteDay(TenfoldChronicle& chronicle,
 int day,bool ready,double now,TimedProgressRetry& retry,Writer write){
 auto next=chronicle;
 if(!next.PassDay(day))return TimedProgress::Unchanged;
 return retry.Attempt(ready,now,[&]{
  const auto before=chronicle;chronicle=next;
  if(write())return true;
  chronicle=before;return false;
 });
}
template<class Writer> TimedProgress CommitMaterialDay(MaterialWorld& materials,
 LivingEconomy& wages,BesHousehold& household,const std::array<RealityPublicResponse,9>& civic,
 int day,bool ready,double now,TimedProgressRetry& retry,Writer write){
 if(materials.Town(Realm::ThreefoldReach)->day>=day)return TimedProgress::Unchanged;
 return retry.Attempt(ready,now,[&]{
  const auto before=materials;const auto beforeWages=wages;const auto beforeHome=household;
  auto next=materials;auto nextWages=wages;auto nextHome=household;
  if(!next.AdvanceAllOneDay(civic,household.OnWalkout()))return false;
  // Payroll follows the single day actually simulated, not a lagging clock's future day.
  const int simulated=next.Town(Realm::ThreefoldReach)->day;
  if(!nextWages.AdvanceOneDay(simulated,household.OnWalkout())||
     !nextHome.AdvanceOneDay(simulated))return false;
  for(int i=0;i<9;++i){
   const auto realm=static_cast<Realm>(i);const auto* town=next.Town(realm);
   if(town->shortageDays>before.Town(realm)->shortageDays){
    const int support=nextWages.ApplyShelterSupport(realm,*town);
    if(support>0)next.ReinforceShelter(realm,support);
   }
  }
  materials=next;wages=nextWages;household=nextHome;
  if(write())return true;
  materials=before;wages=beforeWages;household=beforeHome;return false;
 });
}
}
