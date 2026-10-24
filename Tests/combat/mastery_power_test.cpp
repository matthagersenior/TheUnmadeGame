#include "Combat/UnmadeMasteryRules.h"
#include "World/UnmadeAdventureDepthRules.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
using namespace UnmadeCore;
int main(){
 const double nan=std::numeric_limits<double>::quiet_NaN();
 const double inf=std::numeric_limits<double>::infinity();
 assert(MasteryTargetInRange(0,0,0,true));
 assert(MasteryTargetInRange(300,400,600,true));
 assert(!MasteryTargetInRange(0,0,800,true));
 assert(!MasteryTargetInRange(0,0,10000,true));
 assert(!MasteryTargetInRange(700,0,500,true));
 assert(!MasteryTargetInRange(1,1,1,false));
 for(int axis=0;axis<3;++axis)for(double invalid:{nan,inf,-inf}){
  double delta[]{1,1,1};delta[axis]=invalid;
  assert(!MasteryTargetInRange(delta[0],delta[1],delta[2],true));
 }
 // A rejected power neither spends stamina nor reserves its recovery window.
 for(int unavailable=0;unavailable<3;++unavailable){CombatTempo tempo;
  const bool alive=unavailable!=0,enabled=unavailable!=1,staggered=unavailable==2;
  assert(!TryMasteryPower(tempo,1,alive,enabled,staggered));
  assert(tempo.Stamina()==100);
  assert(TryMasteryPower(tempo,1,true,true,false));assert(tempo.Stamina()==70);
  assert(!TryMasteryPower(tempo,12.99,true,true,false));assert(tempo.Stamina()==70);
  assert(TryMasteryPower(tempo,13,true,true,false));assert(tempo.Stamina()==40);
 }
 for(double invalid:{nan,inf,-inf,-1.}){CombatTempo tempo;
  assert(!TryMasteryPower(tempo,invalid,true,true,false));assert(tempo.Stamina()==100);
  assert(TryMasteryPower(tempo,0,true,true,false));
 }
 CombatTempo tired;assert(tired.Spend(71));assert(!TryMasteryPower(tired,0,true,true,false));assert(tired.Stamina()==29);
 // Selection rollback preserves every realm and the previous active technique.
 for(int realm=0;realm<9;++realm)for(int choice=1;choice<=2;++choice){AdventureDepth depth;
  for(int a=0;a<9;++a){assert(depth.Offer(a,true));assert(depth.Witness(a));assert(depth.Evidence(a,true));assert(depth.Choose(a,choice,true));assert(depth.Buy(a,2,1,true));}
  assert(depth.Select((realm+1)%9));const auto before=depth.Pack();int writes=0;
  assert(CommitMasterySelection(depth,realm,[&]{++writes;return false;})==MasterySelection::SaveFailed);
  assert(depth.Pack()==before&&writes==1);
  assert(CommitMasterySelection(depth,realm,[&]{++writes;return true;})==MasterySelection::Selected);
  assert(depth.Active()==realm&&writes==2);
  const auto selected=depth.Pack();
  assert(CommitMasterySelection(depth,realm,[&]{++writes;return false;})==MasterySelection::Unchanged);
  assert(depth.Pack()==selected&&writes==2);
 }
 AdventureDepth fresh;int writes=0;
 for(int realm:{-1,0,8,9})assert(CommitMasterySelection(fresh,realm,[&]{++writes;return true;})==MasterySelection::Unavailable);
 assert(writes==0&&fresh.Active()==-1);
 // Older earned choices without training remain selectable, but confer no mastery.
 for(int a=0;a<2;++a){assert(fresh.Offer(a,true));assert(fresh.Witness(a));assert(fresh.Evidence(a,true));assert(fresh.Choose(a,1,true));}
 assert(CommitMasterySelection(fresh,0,[]{return true;})==MasterySelection::Selected);
 assert(fresh.Active()==0&&fresh.Arc(0)->training==0);
 std::cout<<"mastery: 18 failed selections, 18 successful selections, 18 unchanged selections; spatial and stagger gates\n";
}
