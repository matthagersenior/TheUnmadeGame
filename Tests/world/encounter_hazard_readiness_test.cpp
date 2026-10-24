#include "World/UnmadeEncounterHazardRules.h"
#include "Combat/UnmadeCombatRules.h"
#include <cassert>
#include <iostream>
#include <limits>
using namespace UnmadeCore;
int main(){
 int exposures=0;
 for(int realm=0;realm<9;++realm){
  Combatant player(100,20,.5);
  const auto sample=SampleDungeonHazard(11+realm*9,false);assert(sample.danger&&sample.pulse);
  auto apply=[&](bool ready,bool alive,const DungeonHazard& pulse,double x,double y,double z){
   if(!EncounterHazardInCylinder(ready,alive,pulse,x,y,z,280,150))return false;
   return player.ReceiveHit(100+realm,pulse.pulse,12+realm,false)==HitOutcome::Applied;
  };
  for(int construction=0;construction<32;++construction){assert(!apply(false,true,sample,0,0,0));assert(!apply(true,false,sample,0,0,0));}
  assert(player.Health()==100);assert(apply(true,true,sample,0,0,0));++exposures;
  const double after=player.Health();for(int repeat=0;repeat<32;++repeat)assert(!apply(true,true,sample,0,0,0));assert(player.Health()==after);
  assert(!apply(true,true,sample,280,0,0));assert(!apply(true,true,sample,0,0,150));assert(!apply(true,true,sample,0,0,-150));
  assert(EncounterHazardInCylinder(true,true,sample,279,0,149,280,150));
  assert(!EncounterHazardInCylinder(true,true,sample,280,0,0,280,150));assert(!EncounterHazardInCylinder(true,true,sample,0,0,150,280,150));assert(!EncounterHazardInCylinder(true,true,sample,0,0,-150,280,150));
  for(double invalid:{std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()}){
   assert(!apply(true,true,sample,invalid,0,0));assert(!apply(true,true,sample,0,invalid,0));assert(!apply(true,true,sample,0,0,invalid));
   assert(!EncounterHazardInCylinder(true,true,sample,0,0,0,invalid,150));assert(!EncounterHazardInSphere(true,true,sample,0,0,0,invalid));
  }
  assert(!EncounterHazardInCylinder(true,true,sample,0,0,0,0,150));assert(!EncounterHazardInSphere(true,true,sample,0,0,0,-1));
  for(int tick=0;tick<1200;++tick){const double now=tick*.025;const auto flood=SampleWingFlood(now,false);const double phase=std::fmod(now,12.);
   assert(flood.danger==(phase>=9));assert(flood.pulse==static_cast<unsigned long long>(now/12)+1);
   assert(!SampleWingFlood(now,true).danger);
   assert(EncounterHazardInSphere(true,true,flood,0,0,0,750)==flood.danger);
   assert(!EncounterHazardInSphere(false,true,flood,0,0,0,750));assert(!EncounterHazardInSphere(true,false,flood,0,0,0,750));
   assert(!EncounterHazardInSphere(true,true,flood,751,0,0,750));assert(!EncounterHazardInSphere(true,true,flood,0,0,751,750));
  }
  const auto flood=SampleWingFlood(9,false);assert(EncounterHazardInSphere(true,true,flood,450,600,0,750));assert(!EncounterHazardInSphere(true,true,flood,450,600,1,750));
  assert(!EncounterHazardInSphere(true,true,{},0,0,0,750));
  const DungeonHazard noReceipt{false,true,0};
  assert(!EncounterHazardInSphere(true,true,noReceipt,0,0,0,750));assert(!EncounterHazardInCylinder(true,true,noReceipt,0,0,0,280,150));
  for(double invalid:{-1.,1e13,std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()})assert(!SampleWingFlood(invalid,false).danger);
  // A free vent and an earned kit disable the same runtime field; expiration
  // restores the original clock/pulse, without inventing persistent suppression.
  for(double duration:{8.,20.,30.})for(double now:{2.,3.,9.,11.,20.,29.,32.}){
   const double quietUntil=2+duration;auto h=SampleDungeonHazard(now,now<quietUntil);
   assert(h.danger==(now>=quietUntil&&std::fmod(now,9.)>=2&&std::fmod(now,9.)<3));
  }
 }
 std::cout<<exposures<<" resumed realm pulses and 10800 wing readiness/timing samples passed\n";
}
