#include "World/UnmadeAdventureDepthRules.h"
#include "World/UnmadeAdventureLayout.h"
#include <cassert>
using namespace UnmadeCore;
int main(){Expeditions e;AdventureDepth d;assert(!d.Offer(0,false));assert(!d.Buy(0,0,1,true));
 for(int a=0;a<9;++a){assert(e.Brief(a));assert(e.Visit(a,0));for(int n=0;n<5;++n)assert(e.Enter(a,n));for(int c=0;c<3;++c)e.Inspect(a,c);e.Choose(a,2);e.Enter(a,5);e.Boss(a,false);
 assert(d.Offer(a,true));assert(d.Witness(a));assert(d.Evidence(a,true));assert(d.Choose(a,1,true));assert(!d.Choose(a,2,true));assert(d.Route(a,true));assert(d.Secret(a,true));assert(d.Buy(a,2,1,true));assert(d.Buy(a,2,1,true));assert(d.Buy(a,2,1,true));assert(!d.Buy(a,2,1,true));assert(d.Buy(a,0,1,true));assert(!d.Buy(a,0,1,true));
 for(int n=0;n<5;++n){auto p=DungeonRoom(a,n),q=DungeonRoom(a,n+1);assert(std::abs(p.x-q.x)+std::abs(p.y-q.y)==2600);assert(DungeonDoor(a,n,(q.x-p.x)/2600,(q.y-p.y)/2600));}assert(DungeonDoor(a,5,0,1));}
 auto p=d.Pack();AdventureDepth restored;assert(restored.Restore(p,e,1));p[8]=99;auto old=restored.Pack();assert(!restored.Restore(p,e,1));assert(restored.Pack()==old);assert(!restored.Restore(old,Expeditions{},1));
 RealmChallenge c;assert(!c.Step(0,0,0,false,true));assert(!c.Step(0,2,1,false,true));assert(c.Step(0,1,2,false,true));RealmChallenge wet;assert(!wet.Step(1,0,0,false,true));assert(wet.Step(1,2,5,false,true));RealmChallenge heat;heat.Step(2,0,0,false,true);heat.Step(2,1,1,false,true);assert(!heat.Step(2,2,20,false,true));assert(!heat.Step(2,0,21,false,true));assert(heat.Step(2,1,22,false,true));for(int a=0;a<9;++a){RealmChallenge test;int seq[9][3]={{0,2,1},{0,2,2},{0,1,2},{0,1,1},{0,1,2},{0,1,2},{0,1,2},{2,0,1},{1,2,2}};bool done=false;for(int n=0;n<3&&!done;++n)done=test.Step(a,seq[a][n],5+n,true,true);assert(done);}
 AdventureDepth publicStory;publicStory.Offer(0,true);publicStory.Witness(0);publicStory.Evidence(0,true);assert(publicStory.Choose(0,2,true));assert(publicStory.Buy(0,2,1,true));assert(publicStory.Price(0,0)==2&&publicStory.Price(0,2)==0);assert(publicStory.CounterThreat(0));assert(!publicStory.CounterThreat(0));assert(publicStory.Restore(publicStory.Pack(),e,1));
 RealmChallenge sky;assert(!sky.Step(4,0,1,false,true));sky.Step(4,0,1,true,true);sky.Step(4,1,1,true,true);assert(sky.Step(4,2,1,true,true));}
