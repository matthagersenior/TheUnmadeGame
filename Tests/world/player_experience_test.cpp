#include "World/UnmadePlayerExperienceRules.h"
#include <cassert>
using namespace UnmadeCore;
int main(){
 Expeditions e; AdventureDepth d; PlayerExperience x;
 assert(!x.DiscoverCity(-1,true));assert(!x.DiscoverCity(8,false));assert(x.DiscoverCity(8,true));assert(!x.DiscoverCity(8,true));assert(x.Track(8,e));
 assert(!x.OfferWing(0,e)); assert(!x.Visit(0,0,0));
 e.Brief(0);e.Visit(0,0);e.Enter(0,0);
 assert(x.OfferWing(0,e)); assert(x.Visit(0,4,1));assert(!x.Visit(0,4,1));
 assert(!x.CompleteWing(0));assert(!x.Guardian(0));
 assert(x.WingStep(0,0,true));assert(!x.WingStep(0,0,true));
 assert(x.WingStep(0,1,true));assert(x.WingStep(0,2,true));
 assert(x.Guardian(0));assert(x.CompleteWing(0));assert(!x.CompleteWing(0));
 assert(x.Arc(0)->insight==5);assert(x.Upgrade(0,1));assert(x.Arc(0)->insight==3);assert(x.ActiveBuild()==0);
 assert(x.Upgrade(0,4));assert(x.Upgrade(0,5));assert(x.Upgrade(0,6));assert(x.Arc(0)->insight==0);
 assert(!x.Upgrade(0,2));assert(!x.Upgrade(0,4));assert(!x.Repair(0,d));
 auto p=x.Pack();PlayerExperience y;assert(y.Restore(p,e,d,1));assert(y.Pack()==p);
 auto bad=p;bad[6+11]=1;assert(!y.Restore(bad,e,d,1));assert(y.Pack()==p);
 bad=p;bad[6+8]=6;assert(!y.Restore(bad,e,d,1));
 assert(MapKnowsCity(*e.Arc(0)));assert(!MapKnowsCity(*e.Arc(1)));assert(!MapKnowsRoom(*e.Arc(0),1));assert(MapKnowsRoom(*e.Arc(0),0));
 assert(!MapKnowsChoice(*d.Arc(0)));assert(!ServiceOpen(0,2));assert(ServiceOpen(2,2));assert(ServiceOpen(0,12));
 for(int a=0;a<9;++a){auto anchor=WingAnchor(a);assert(anchor>=0&&anchor<6);auto q=WingRoom(a,0);auto c=DungeonRoom(a,anchor);assert(q.x==c.x+2600&&q.y==c.y);}
 assert(BuildEffects(1).attack>0&&BuildEffects(1).cost>1);assert(BuildEffects(2).armor>0&&BuildEffects(2).speed<1);assert(BuildEffects(3).speed>1);
 e.Brief(2);e.Visit(2,0);e.Enter(2,0);assert(x.OfferWing(2,e));assert(x.WingStep(2,0,true));assert(x.RestartCooling(2));assert(!x.WingStep(2,1,true));
 PlayerExperience z;assert(z.OfferWing(0,e));assert(z.Visit(0,4,1));assert(z.Scene(0,0,e,d));assert(!z.Scene(0,0,e,d));assert(!z.Scene(0,1,e,d));assert(z.Arc(0)->insight==1);for(int k=0;k<3;++k)assert(z.WingStep(0,k,true));assert(z.Guardian(0));assert(z.CompleteWing(0));assert(z.Upgrade(0,6));assert(z.Specialty(0,1));assert(!z.Specialty(0,1));assert(z.Specialty(0,2));assert(!z.Specialty(0,3));PlayerExperience restored;assert(restored.Restore(z.Pack(),e,d,2));auto corrupt=z.Pack();corrupt[6+13]=3;assert(!restored.Restore(corrupt,e,d,2));
 for(auto start:std::array<ServicePoint,4>{{{0,2200},{-1600,1100},{1800,1500},{0,-1500}}}){auto at=start;for(int i=0;i<2400&&std::hypot(at.x-400,at.y-800)>80;++i){auto q=ServiceApproach(at.x,at.y,400,800,0,0);double len=std::hypot(q.x-at.x,q.y-at.y);if(len>0){at.x+=(q.x-at.x)/len*6;at.y+=(q.y-at.y)/len*6;}assert(!(std::abs(at.y+600)<44&&std::abs(at.x)>240&&std::abs(at.x)<750));}assert(std::hypot(at.x-400,at.y-800)<85);}
 e.Brief(6);e.Visit(6,0);e.Enter(6,0);assert(!x.ClearObserver(6));assert(x.OfferWing(6,e));assert(x.ClearObserver(6));assert(!x.ClearObserver(6));
 auto unchanged=y.Pack();assert(!y.Restore(std::vector<int>{1},e,d,1));assert(y.Pack()==unchanged);
}
