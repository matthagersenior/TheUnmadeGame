#pragma once
#include "World/UnmadeExpeditionRules.h"
namespace UnmadeCore {
struct UnifiedLoadout {
 int weapon=-1,armor=-1,technique=-1;
 bool Valid(const Expeditions& e)const{return weapon>=-1&&weapon<9&&armor>=-1&&armor<9&&technique>=-1&&technique<9&&(weapon<0||e.Arc(weapon)->reward==1)&&(armor<0||e.Arc(armor)->reward==2)&&(technique<0||e.Arc(technique)->reward!=0);}
 static UnifiedLoadout Migrate(const Expeditions& e){UnifiedLoadout n;auto& s=e.Save();if(s.gear){int a=(s.gear-1)/2;if(s.gear%2)n.weapon=a;else n.armor=a;n.technique=a;}return n;}
 ExpeditionBenefit Benefit()const{ExpeditionBenefit b;if(weapon>=0)b.attack=7+weapon*2;if(armor>=0)b.armor=5+armor;if(technique>=0){constexpr int A[]={3,0,2,1,0,2,0,1,0},D[]={0,2,2,1,1,0,2,3,3},G[]={3,2,0,1,0,2,3,0,1},F[]={0,0,1,1,2,0,0,1,2};b.attack+=A[technique];b.armor+=D[technique];b.glimpseDiscount=G[technique];b.foldSeconds=F[technique];}return b;}
};
}
