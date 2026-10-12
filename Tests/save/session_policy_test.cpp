#include "Save/UnmadeSessionRules.h"
#include "Combat/UnmadeCombatRules.h"
#include "Combat/UnmadeCombatStyleRules.h"
#include <cassert>
#include <limits>
using namespace UnmadeCore;
int main(){for(int n=0;n<=3;++n)assert(ValidProfile(n));assert(!ValidProfile(-1)&&!ValidProfile(4));assert(ValidResume(0,0,135,37,19));assert(!ValidResume(0,0,135,101,0));assert(!ValidResume(0,0,135,10,-1));assert(!ValidResume(0,0,135,std::numeric_limits<double>::quiet_NaN(),10));assert(!ValidResume(900000,0,135,100,100));
 assert(ChooseStoredRecord(true,true,true,true,false)==StoredRecord::Primary);assert(ChooseStoredRecord(true,true,false,true,false)==StoredRecord::Backup);assert(ChooseStoredRecord(true,false,false,true,false)==StoredRecord::Incompatible);assert(ChooseStoredRecord(true,false,false,true,true)==StoredRecord::Incompatible);assert(ChooseStoredRecord(false,true,false,false,false)==StoredRecord::Missing);
 int old=1,next=2,primary=1,backup=0,calls=0;auto writer=[&](int n,bool b){++calls;if(b)backup=n;else primary=n;return true;};assert(CommitWithPrevious(next,&old,writer));assert(primary==2&&backup==1&&calls==2);
 primary=1;calls=0;assert(!CommitWithPrevious(next,&old,[&](int,bool){++calls;return false;}));assert(primary==1&&calls==1);
 backup=0;assert(!CommitWithPrevious(next,&old,[&](int n,bool b){if(b){backup=n;return true;}return false;}));assert(primary==1&&backup==1);
 calls=0;assert(CommitWithPrevious(next,static_cast<const int*>(nullptr),writer));assert(calls==1);
 Combatant health;assert(health.RestoreHealth(37));assert(health.Health()==37);assert(!health.RestoreHealth(101));assert(health.Health()==37);CombatTempo tempo;assert(tempo.RestoreEnergy(19));assert(tempo.Stamina()==19);assert(!tempo.RestoreEnergy(-1));assert(tempo.Stamina()==19);
 assert(SimulationInterval(0,true)==0&&SimulationInterval(19000,true)==2);assert(SimulationInterval(4000,false)==.25);for(int a=0;a<9;++a)for(int n=0;n<5;++n)assert(WorkMotion(a,n)>=0&&WorkMotion(a,n)<3);
}
