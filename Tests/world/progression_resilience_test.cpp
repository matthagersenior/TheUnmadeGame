#include "World/UnmadeAdventureDepthRules.h"
#include <cassert>
using namespace UnmadeCore;
int main(){for(int a=0;a<9;++a)for(int choice=1;choice<=2;++choice){Expeditions e;e.Brief(a);e.Visit(a,0);for(int r=0;r<5;++r){e.Enter(a,r);if(r>0&&r<4)e.Inspect(a,r-1);}e.Choose(a,2);e.Enter(a,5);e.Boss(a,false);AdventureDepth d;d.Offer(a,true);d.Witness(a);d.Evidence(a,true);d.Choose(a,choice,true);for(int day=1;day<=4;++day)d.Buy(a,0,day,true);assert(d.Buy(a,2,4,true));assert(!d.Buy(a,2,4,true));assert(d.Route(a,true));while(d.Buy(a,1,4,true))d.SpendKit(a);assert(d.Buy(a,2,4,true));assert(!d.Buy(a,2,4,true));assert(d.Secret(a,true));while(d.Buy(a,1,4,true))d.SpendKit(a);assert(d.Buy(a,2,4,true));assert(!d.Buy(a,2,4,true));assert(d.Arc(a)->training==3);AdventureDepth restored;assert(restored.Restore(d.Pack(),e,4));assert(restored.Arc(a)->training==3);auto old=d.Pack();old[2+a*12+6]=0;assert(restored.Restore(old,e,4)); // Legacy training spending remains valid and is not refunded.
 }
}
