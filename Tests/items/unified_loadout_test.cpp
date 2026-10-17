#include "Items/UnmadeUnifiedLoadoutRules.h"
#include <cassert>
using namespace UnmadeCore;
int main(){Expeditions e;UnifiedLoadout n;assert(n.Valid(e));n.weapon=0;assert(!n.Valid(e));n={};for(int a:{0,1}){e.Brief(a);e.Visit(a,0);for(int r=0;r<5;++r){e.Enter(a,r);if(r>0&&r<4)e.Inspect(a,r-1);}e.Choose(a,2);e.Enter(a,5);e.Boss(a,false);e.Return(a);e.Claim(a,a==0?1:2);}n=UnifiedLoadout::Migrate(e);assert(n.armor==1&&n.weapon==-1);n.weapon=0;assert(n.Valid(e));assert(n.Benefit().attack==7);assert(n.Benefit().armor==8);n.weapon=-1;assert(n.Benefit().attack==0);n.armor=0;assert(!n.Valid(e));
 // A committed owned reward survives a failed selection without replacing slots.
 n={};int writes=0;
 assert(!CommitRealmSelection(n,e,0,[&]{++writes;assert(n.weapon==0&&n.technique==0);return false;}));
 assert(writes==1&&n.weapon==-1&&n.armor==-1&&n.technique==-1&&e.Arc(0)->reward==1);
 assert(CommitRealmSelection(n,e,0,[&]{++writes;return true;}));
 assert(n.weapon==0&&n.technique==0);
 assert(!CommitRealmSelection(n,e,1,[&]{++writes;assert(n.armor==1);return false;}));
 assert(n.weapon==0&&n.armor==-1&&n.technique==0&&e.Arc(1)->reward==2);
 assert(CommitRealmSelection(n,e,1,[]{return true;}));
 assert(n.weapon==0&&n.armor==1&&n.technique==1&&n.Valid(e));
 const auto packed=e.Pack();
 for(int reload=0;reload<3;++reload){Expeditions restored;assert(restored.Restore(packed));
     assert(CommitRealmSelection(n,restored,1,[]{return true;}));assert(restored.Pack()==packed);}
 writes=0;assert(!CommitRealmSelection(n,e,8,[&]{++writes;return true;}));assert(writes==0);
}
