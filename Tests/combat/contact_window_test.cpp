#include "Combat/UnmadeContactRules.h"
#include <cassert>
using namespace UnmadeCore;
int main(){ContactWindow w;assert(w.Begin(1,1,1));assert(!w.Active(1.05));assert(w.Active(1.3));assert(w.Claim(8,1.3));assert(!w.Claim(8,1.35));assert(!w.Claim(9,1.4));assert(!w.Begin(2,1.5,1));w.Cancel();assert(!w.Active(1.3));assert(w.Begin(3,3,.5));assert(!w.Active(2));assert(!w.Begin(4,4,-1));
 assert(ProjectileFlightValid(100,1));assert(!ProjectileFlightValid(0,1));assert(!ProjectileFlightValid(100,-1));
 StaggerResistance s;assert(s.Take(1,.5,false));assert(!s.Take(1.1,.5,false));assert(s.Take(2,1,true));assert(!s.Take(2.1,1,true));
}
