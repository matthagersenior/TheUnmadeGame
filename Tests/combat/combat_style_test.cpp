#include "Combat/UnmadeCombatStyleRules.h"
#include <cassert>
#include <limits>
using namespace UnmadeCore;
int main(){CombatTempo c;c.Tick(0,false,0);auto a=c.Attack(0,RealmWeapon(6),false);assert(a.started&&a.combo==1);assert(!c.Attack(.1,RealmWeapon(6),false).started);assert(c.Attack(.45,RealmWeapon(6),false).combo==2);assert(c.Attack(.9,RealmWeapon(6),false).combo==3);assert(c.Stamina()==67);assert(!c.Attack(2,RealmWeapon(0),true).started);assert(c.Guard(1.1));assert(c.Defense(1.2,true)==2);assert(c.Defense(1.21,true)==0);c.Tick(2,false,3);assert(c.Dodge(2));assert(c.Defense(2.1,false)==1);assert(c.Defense(2.5,false)==0);assert(c.Power(3));assert(!c.Power(4));double before=c.Stamina();assert(!c.Spend(std::numeric_limits<double>::quiet_NaN()));assert(c.Stamina()==before);for(int i=0;i<9;++i)assert(RealmWeapon(i).reach>200);}
