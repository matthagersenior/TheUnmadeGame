#include "Combat/UnmadeCreatureAwarenessRules.h"
#include <cassert>
#include <limits>

int main() {
 using namespace UnmadeCore;
 assert(CreatureCanSpot(true,false,16.0,-1.0)); // visible standing target
 assert(!CreatureCanSpot(false,false,1.0,1.0)); // cover blocks vision
 assert(!CreatureCanSpot(true,true,5.0,-0.8)); // crouch behind the creature
 assert(CreatureCanSpot(true,true,5.0,0.8));  // crouch in forward sight cone
 assert(CreatureCanSpot(true,true,2.0,-0.8)); // too close to hide
 assert(!CreatureCanSpot(true,true,9.0,0.8)); // crouching at long range
 assert(!CreatureCanSpot(true,true,std::numeric_limits<double>::quiet_NaN(),1));
 assert(!CreatureCanSpot(true,true,3.0,std::numeric_limits<double>::quiet_NaN()));

 CreatureAwareness aware;
 assert(!aware.Update(0.0,false));
 assert(aware.Update(1.0,true));
 assert(aware.Update(4.9,false)); // spotted enemies do not instantly forget
 assert(!aware.Update(5.1,false));
 assert(aware.Update(7.0,true));
 aware.Reset();
 assert(!aware.Update(7.1,false));
 assert(!aware.Update(-1.0,true));
}
