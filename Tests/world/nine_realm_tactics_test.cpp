#include "World/UnmadeArenaTacticsRules.h"
#include <cassert>
#include <cstring>
#include <set>
using namespace UnmadeCore;
int main(){
 std::set<int> orders;
 for(int a=0;a<9;++a){
  const auto& cfg=Arenas[a];
  assert(cfg.name&&cfg.name[0]&&cfg.counterObjective&&cfg.counterObjective[0]);
  assert(cfg.openWindow>=5&&cfg.openWindow<=10);
  assert(cfg.cooldown>=16&&cfg.exposureSeconds>=3);
  orders.insert(cfg.first);
  ArenaCounter counter;
  assert(counter.Use(a,cfg.first,1,false)==CounterStep::Blocked); // quest locked
  assert(counter.Use(a,1-cfg.first,1,true)==CounterStep::WrongOrder);
  assert(counter.Use(a,cfg.first,2,true)==CounterStep::Armed);
  assert(counter.Use(a,1-cfg.first,2+cfg.openWindow+0.1,true)==CounterStep::Expired);
  assert(counter.Use(a,cfg.first,15,true)==CounterStep::Armed);
  assert(counter.Use(a,1-cfg.first,15+cfg.openWindow-0.1,true)==CounterStep::Success);
  assert(counter.Use(a,cfg.first,16,true)==CounterStep::Cooldown);
  assert(counter.Use(a,cfg.first,16+cfg.cooldown+cfg.openWindow,true)==CounterStep::Armed);
  counter.Reset();
  assert(counter.Use(a,cfg.first,1,true)==CounterStep::Armed);
  ExpeditionState none{};
  assert(ArenaAftermathRecord(none)==ArenaAftermath::None);
  ExpeditionState defeated{};defeated.boss=1;defeated.returned=1;
  assert(ArenaAftermathRecord(defeated)==ArenaAftermath::UnfinishedRescue);
  defeated.rescued=3;
  assert(ArenaAftermathRecord(defeated)==ArenaAftermath::Rescued);
  defeated.boss=2;
  assert(ArenaAftermathRecord(defeated)==ArenaAftermath::WitnessedYield);
  assert(std::strlen(ArenaAftermathText(a,ArenaAftermath::Rescued))>15);
  assert(std::strlen(ArenaAftermathText(a,ArenaAftermath::WitnessedYield))>15);
  assert(std::strcmp(ArenaAftermathText(a,ArenaAftermath::Rescued),
    ArenaAftermathText(a,ArenaAftermath::WitnessedYield))!=0);
 }
 assert(orders.size()==2);
 ArenaCounter c;
 assert(c.Use(0,Arenas[0].first,-1,true)==CounterStep::Blocked);
 assert(c.Use(-1,0,2,true)==CounterStep::Blocked);
 assert(c.Use(9,0,2,true)==CounterStep::Blocked);
}
