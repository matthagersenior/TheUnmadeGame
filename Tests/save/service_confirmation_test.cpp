#include "Save/UnmadeServiceCommitRules.h"
#include <cassert>
#include <limits>
using namespace UnmadeCore;
int main(){
 ScopedServiceConfirmation c;
 assert(!c.Confirm(0,1,0));assert(c.Confirm(0,1,20));
 // A confirmed attempt is consumed even when its writer subsequently fails.
 assert(!c.Confirm(0,1,21));assert(c.Confirm(0,1,22));
 assert(!c.Confirm(0,1,23));assert(!c.Confirm(0,2,24));
 assert(!c.Confirm(1,2,25));assert(c.Confirm(1,2,26));
 assert(!c.Confirm(44,11,30));assert(!c.Confirm(43,11,31));
 assert(c.Confirm(43,11,32));assert(!c.Confirm(43,11,33));
 assert(!c.Confirm(43,11,54));assert(!c.Confirm(43,11,53));
 assert(c.Confirm(43,11,54));
 assert(!c.Confirm(0,1,60));assert(!c.Confirm(-1,1,61));
 assert(!c.Confirm(0,1,62));assert(!c.Confirm(0,1,std::numeric_limits<double>::quiet_NaN()));
 assert(!c.Confirm(0,1,63));assert(!c.Confirm(0,1,std::numeric_limits<double>::infinity()));
 assert(!c.Confirm(0,1,64));assert(c.Confirm(0,1,65));
}
