#include "Items/UnmadeUnifiedLoadoutRules.h"
#include <cassert>
using namespace UnmadeCore;
int main(){Expeditions e;UnifiedLoadout n;assert(n.Valid(e));n.weapon=0;assert(!n.Valid(e));n={};for(int a:{0,1}){e.Brief(a);e.Visit(a,0);for(int r=0;r<5;++r){e.Enter(a,r);if(r>0&&r<4)e.Inspect(a,r-1);}e.Choose(a,2);e.Enter(a,5);e.Boss(a,false);e.Return(a);e.Claim(a,a==0?1:2);}n=UnifiedLoadout::Migrate(e);assert(n.armor==1&&n.weapon==-1);n.weapon=0;assert(n.Valid(e));assert(n.Benefit().attack==7);assert(n.Benefit().armor==8);n.weapon=-1;assert(n.Benefit().attack==0);n.armor=0;assert(!n.Valid(e));}
