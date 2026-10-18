#pragma once
// A receiving chief can adjust only their *own* frontier sentry ward after
// personally receiving a witnessed report. No remote broadcasts, extra saves,
// instant enemy deaths, or unconditional moral verdicts.
#include "World/UnmadeCrossRealmCarryRules.h"
#include <array>
#include <algorithm>
namespace UnmadeCore {
enum class CrossRealmCombatKind:int {
 Unknown=0,BorderAlert=1,OpenWitnessPass=2,ReviewedAlert=3,ReviewedPass=4,CasualtyVigil=5,ReviewedVigil=6
};
struct CrossRealmCombatProfile {
 const char* frontier;
 const char* warning;
};
inline constexpr std::array<CrossRealmCombatProfile,9> CrossRealmCombatProfiles={{
 {"charter-road watch", "Charter wardkeepers brace the disputed travelers' path"},
 {"rainward pump cordon", "Pump sentries tighten or relax the communal waterline"},
 {"common-furnace watch", "The kiln sentries alter the refuge heat barricade"},
 {"oath-pier patrol", "Ferry guards adjust the contested debtor crossings"},
 {"choir-span sentries", "Load stewards retune the high bridge warning ward"},
 {"ancestral quarry watch", "Seam defenders brace the disputed rock crossing"},
 {"unregistered shelter watch", "Shelter wardkeepers defend an unnamed entry"},
 {"rootway guardians", "Orchard defenders respond at the shared graft path"},
 {"honest-threshold ward", "Threshold guards mark both contradictory approach maps"}
}};
struct CrossRealmCombatState {
 CrossRealmCombatKind kind=CrossRealmCombatKind::Unknown;
 int hazardDelta=0;
 int casualties=0;
 int rescued=0;
};
constexpr int CrossRealmCombatSource(int destination) noexcept {
 return destination>=0&&destination<9?(destination+8)%9:-1;
}
inline CrossRealmCombatState CrossRealmCombatResponse(
 int destination,int sourceBoss,int sourceKills,int sourceRescued,
 bool sourceReturned,bool received,bool reviewed,bool validSave)noexcept {
 if(CrossRealmCombatSource(destination)<0||
    sourceBoss<1||sourceBoss>2||sourceKills<0||sourceKills>255||
    sourceRescued<0||sourceRescued>3||
    !sourceReturned||!received||!validSave)return {};
 int casualties=0;
 for(int k=0;k<8;++k)casualties+=(sourceKills&(1<<k))?1:0;
 // Rescued is a 2-bit identity mask, never a head count.
 int personsFreed=0;
 for(int k=0;k<2;++k)personsFreed+=(sourceRescued&(1<<k))?1:0;
 if(sourceBoss==1){
  // Many casualties can increase defensive alarm, yet real rescues moderate
  // the reaction. This is civic practice, not a universal judgment of guilt.
  const int heightened=std::max(1,std::min(5,2+casualties/2)-
     (sourceRescued==3?1:0));
  return {reviewed?CrossRealmCombatKind::ReviewedAlert:
          CrossRealmCombatKind::BorderAlert,
          reviewed?std::max(0,heightened-2):heightened,
          casualties,personsFreed};
 }
 // A living surrender after many guard deaths is not a bloodless truce.
 // Those local deaths still change the next chief's safety calculus.
 if(casualties>=4){
  const int vigil=1+(casualties>=7?1:0);
  return {reviewed?CrossRealmCombatKind::ReviewedVigil:
          CrossRealmCombatKind::CasualtyVigil,
          reviewed?std::max(0,vigil-1):vigil,
          casualties,personsFreed};
 }
 const int open=std::min(-1,-3+casualties/2);
 return {reviewed?CrossRealmCombatKind::ReviewedPass:
         CrossRealmCombatKind::OpenWitnessPass,
         reviewed?std::min(0,open+2):open,
         casualties,personsFreed};
}
} // namespace UnmadeCore
