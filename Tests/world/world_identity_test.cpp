#include "World/UnmadeIdentityRules.h"
#include <cassert>
#include <limits>
using namespace UnmadeCore;
int main(){WorldIdentity w;Expeditions e;assert(!w.Restore({},e,1));assert(!w.Repair(0,0,false));assert(!w.Hunt(0,3));assert(!w.Rest(0,1,true));
 for(int a=0;a<9;++a){assert(e.Brief(a));assert(e.Visit(a,0));for(int room=0;room<5;++room)assert(e.Enter(a,room));for(int k=0;k<8;++k)assert(e.Kill(a,k));for(int c=0;c<3;++c)assert(e.Inspect(a,c));assert(e.Choose(a,2));assert(e.Enter(a,5));assert(e.Boss(a,false));assert(e.Return(a));
 // Exhaustive search proves every realm target reachable, without assuming operator order.
 bool solved=false;for(int bits=0;bits<8&&!solved;++bits){WorldIdentity trial;for(int k=0;k<3;++k)if(bits&(1<<k))trial.Operate(a,k);if(trial.Arc(a)->puzzle==WorldIdentity::Target(a)){for(int k=0;k<3;++k)if(bits&(1<<k))w.Operate(a,k);solved=true;}}
 assert(solved);assert(w.Chart(a,7));assert(!w.Chart(a,7));assert(!w.Operate(a,0));assert(w.Hunt(a,4));assert(!w.Hunt(a,8));for(int n=0;n<3;++n)assert(w.Repair(a,n,true));assert(w.Rest(a,1,true));assert(!w.Rest(a,1,true));assert(w.Rest(a,2,true));
 auto move=ExpeditionMove(ExpeditionRole::Boss,a,2,false);assert(move.shape==a+1&&move.windup>=2);assert(!SignatureHit(move.shape,1000,1000,420));}
 auto p=w.Pack();WorldIdentity n;assert(n.Restore(p,e,2));auto old=n.Pack();p[5]=3;assert(!n.Restore(p,e,2));assert(n.Pack()==old);assert(!n.Restore(old,Expeditions{},2));assert(!SignatureHit(1,std::numeric_limits<double>::quiet_NaN(),0,420));assert(SignatureHit(1,300,0,420));assert(!SignatureHit(1,0,0,420));assert(SignatureHit(8,400,400,420));assert(!SignatureHit(6,0,0,420));}
