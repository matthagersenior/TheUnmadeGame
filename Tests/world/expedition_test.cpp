#include "World/UnmadeExpeditionRules.h"
#include "World/UnmadeExpeditionRegistry.h"
#include "Combat/UnmadeCombatRules.h"
#include <cassert>
#include <limits>
using namespace UnmadeCore;
int main(){
 for(int arc=0;arc<9;++arc)for(int branch=1;branch<=2;++branch)for(int type=1;type<=2;++type)for(int kill=0;kill<=1;++kill){
  if(branch==1&&!kill)continue;
  Expeditions m;assert(!m.Boss(arc,true));assert(!m.Claim(arc,type));assert(m.Brief(arc));assert(!m.Brief(arc));assert(m.Visit(arc,0));assert(m.Visit(arc,1));
  for(int r=0;r<5;++r){assert(m.Enter(arc,r));if(r>0&&r<4)assert(m.Inspect(arc,r-1));if(r<4)assert(m.Kill(arc,r));}
  assert(!m.Enter(arc,5));assert(!m.Rescue(arc,0));assert(m.Kill(arc,4));assert(m.Kill(arc,5));assert(m.Rescue(arc,0));assert(m.Rescue(arc,1));assert(m.Choose(arc,branch));assert(!m.Choose(arc,3-branch));assert(m.Enter(arc,5));assert(m.Boss(arc,kill));assert(!m.Boss(arc,true));assert(m.Return(arc));assert(m.Claim(arc,type));assert(!m.Claim(arc,type));assert(m.Equip(arc));assert(m.Benefit().attack>0||m.Benefit().armor>0);
  auto p=m.Pack();assert(p.size()==93);Expeditions restored;assert(restored.Restore(p));assert(restored.Pack()==p);for(unsigned i=0;i<p.size();++i){auto bad=p;bad[i]=-1;assert(!restored.Restore(bad));assert(restored.Pack()==p);}for(int i=0;i<9;++i)assert(ExpeditionSpecs[i].people.size()==5);
 }
 Expeditions stealth;assert(stealth.Brief(0));assert(stealth.Visit(0,0));for(int r=0;r<5;++r){assert(stealth.Enter(0,r));if(r>0&&r<4)assert(stealth.Inspect(0,r-1));}assert(stealth.Choose(0,2));assert(stealth.Enter(0,5));assert(stealth.Boss(0,false));assert(stealth.Save().arcs[0].kills==0);assert(stealth.Return(0));assert(stealth.Claim(0,2));assert(stealth.Save().arcs[0].rescued==0);
 auto good=stealth.Pack();for(int index:{3,4,5,6,7,8,9,10,91,92}){auto bad=good;bad[index]=999;Expeditions r;assert(!r.Restore(bad));}
 Expeditions all;for(int a=0;a<9;++a){assert(all.Brief(a));assert(all.Visit(a,0));for(int r=0;r<5;++r){assert(all.Enter(a,r));if(r>0&&r<4)all.Inspect(a,r-1);}assert(all.Choose(a,2));assert(all.Enter(a,5));assert(all.Boss(a,false));assert(all.Return(a));assert(all.Claim(a,a%2+1));}for(int a=0;a<9;++a){assert(all.Equip(a));Expeditions reload;assert(reload.Restore(all.Pack()));assert(reload.Save().technique==a+1);}auto before=all;all=before;assert(all.Pack()==before.Pack());
 for(int a=0;a<9;++a)for(int role=0;role<5;++role){ExpeditionPattern p;auto move=ExpeditionMove(static_cast<ExpeditionRole>(role),a,0,false);assert(move.windup>=.8&&move.recovery>=1.2);auto start=p.Step(0,static_cast<ExpeditionRole>(role),a,false,true,100,false);assert(start.started&&!start.strike);assert(!p.Step(move.windup-.01,static_cast<ExpeditionRole>(role),a,false,true,100,false).strike);assert(p.Step(move.windup,static_cast<ExpeditionRole>(role),a,false,true,100,false).strike);assert(p.Step(move.windup+.1,static_cast<ExpeditionRole>(role),a,false,true,100,false).recovering);
  ExpeditionPattern cover;assert(cover.Step(0,static_cast<ExpeditionRole>(role),a,false,true,100,false).started);assert(!cover.Step(move.windup,static_cast<ExpeditionRole>(role),a,false,false,100,false).strike);
  ExpeditionPattern fold;fold.Step(0,static_cast<ExpeditionRole>(role),a,false,true,100,false);assert(!fold.Step(move.windup,static_cast<ExpeditionRole>(role),a,false,true,100,true).strike);
 }
 for(int i=0;i<1000;++i){auto h=SampleDungeonHazard(i*.1,false);assert(!(h.warning&&h.danger));assert(!SampleDungeonHazard(i,true).danger);}assert(SampleDungeonHazard(0,false).warning);assert(SampleDungeonHazard(2,false).danger);assert(!SampleDungeonHazard(3,false).danger);assert(!SampleDungeonHazard(-1,false).danger);
 assert(stealth.Rescue(0,0));assert(stealth.Rescue(0,1));Expeditions rescueReload;assert(rescueReload.Restore(stealth.Pack()));
 Combatant c(100,20,.5);c.ReceiveHit(1,1,30,false);double hp=c.Health();assert(c.SetDamage(25));assert(c.Health()==hp);assert(c.TryAttack(0).baseDamage==25);assert(!c.SetDamage(std::numeric_limits<double>::infinity()));assert(!c.SetDamage(-1));
}
