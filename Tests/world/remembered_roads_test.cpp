#include "World/UnmadeRememberedRoadRules.h"
#include "World/UnmadeRememberedRoadScenes.h"
#include "Combat/UnmadeCreatureRoutineRules.h"
#include <cassert>
#include <limits>
using namespace UnmadeCore;
int main(){
 for(int city=0;city<11;++city){const auto& c=RoadCities[city];assert(!PlanRealmRoute(c.realm,RoadCities[c.destination].realm,4).empty());}
 for(int city=0;city<11;++city)for(int scene=0;scene<2;++scene)for(int beat=0;beat<3;++beat){auto b=RoadBeatAt(city,scene!=0,beat);assert(b.duration>=6&&b.speakerId&&b.subtitle&&b.voice);}
 for(int style=0;style<2;++style)for(int sec=0;sec<1000;++sec)for(int night=0;night<2;++night){auto r=CreatureRoutine(static_cast<EnemyStyle>(style),sec,night!=0,false);assert(std::hypot(r.offsetX,r.offsetY)<400);assert(CreatureRoutine(static_cast<EnemyStyle>(style),sec,night!=0,true).activity==WildActivity::Rest);}
 RememberedRoads a;assert(a.TotalUnits()==44);
 for(int p=0;p<88;++p)for(int item=0;item<4;++item){assert(a.InspectFurniture(p,item));assert(!a.InspectFurniture(p,item));}
 RememberedRoads furnishings;assert(furnishings.Restore(a.Pack(),1));
 assert(!a.Pickup(0,1)&&!a.Deliver(0,1)&&!a.Resolve(0,1,false,true,0));
 for(int city=0;city<11;++city)for(int route=1;route<=3;++route)for(int outcome=1;outcome<=2;++outcome){
  RememberedRoads m;assert(m.Brief(city,2));assert(!m.Brief(city,2));
  for(int c=0;c<3;++c)assert(m.Inspect(city,c,2));
  assert(m.Cool(city,1));
  assert(!m.Resolve(city,2,true,true,1));assert(m.Resolve(city,outcome,true,outcome==1,1));
  assert(m.Choose(city,route));assert(!m.Choose(city,2));assert(m.LaneOpen(city,true)==(route!=3));assert(m.RecessOpen(city)==(route!=2));
  assert(m.Pickup(city,3));assert(m.TotalUnits()==44);assert(!m.Deliver(RoadCities[city].destination,3));
  RememberedRoads loaded;assert(loaded.Restore(m.Pack(),3));assert(!loaded.Cooling(city,1));
  assert(loaded.Checkpoint(city,3));assert(!loaded.Deliver(city,3));assert(loaded.Deliver(RoadCities[city].destination,4));assert(loaded.TotalUnits()==44);
  assert(loaded.Report(city,5));assert(loaded.Return(city,6));assert(!loaded.Return(city,7));
  for(int p=0;p<88;++p){bool local=RoadPeople[p].city==city||RoadPeople[p].city==RoadCities[city].destination;assert(loaded.Tell(p,city,6)==local);if(local){assert(loaded.VisitHome(p,7));assert(!loaded.VisitHome(p,8));}}
  auto packed=loaded.Pack();RememberedRoads round;assert(round.Restore(packed,7));assert(round.Pack()==packed);
  // Every integer is independently corrupted; failed restore leaves prior bytes intact.
  for(std::size_t k=0;k<packed.size();++k){auto bad=packed;bad[k]=-900;assert(!round.Restore(bad,7));assert(round.Pack()==packed);}
  assert(!round.Restore(packed,3));
 }
 RememberedRoads m;assert(m.Brief(0,1));for(int i=0;i<3;++i)assert(m.Inspect(0,i,1));assert(m.Resolve(0,1,true,true,0));
 assert(!m.Maintain(0,0));assert(m.Turns(0)==1);assert(!m.Maintain(0,1));assert(m.Turns(0)==1);assert(!m.Maintain(0,2));assert(m.Maintain(0,4));
 assert(m.Fold(0,10)&&!m.Fold(0,11));assert(m.Cool(0,11));assert(m.Cooling(0,28)&&!m.Cooling(0,29));
 assert(m.Glimpse(0,10)&&!m.Glimpse(0,11));assert(!m.Fold(0,std::numeric_limits<double>::quiet_NaN()));
 // All eleven deliveries conserve supply, including shared receiving cities.
 RememberedRoads all;for(int i=0;i<11;++i){assert(all.Brief(i,1));for(int c=0;c<3;++c)all.Inspect(i,c,1);assert(all.Resolve(i,1,true,true,0));assert(all.Choose(i,1));assert(all.Pickup(i,2));assert(all.Checkpoint(i,2));assert(all.Deliver(RoadCities[i].destination,3));assert(all.TotalUnits()==44);}
 assert(m.Restore(all.Pack(),3));
}
