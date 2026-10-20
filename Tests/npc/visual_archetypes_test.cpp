#include "World/UnmadeCharacterGrayboxRules.h"
#include <cassert>
#include <iostream>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
  const NpcRole roles[5]={NpcRole::Merchant,NpcRole::Guard,NpcRole::Scholar,
                          NpcRole::Courier,NpcRole::Wanderer};
  std::set<std::string> names,descriptions;
  for(auto role:roles){
    const auto& visual=ResidentGraybox(role);
    assert(visual.role==role && visual.sx>.2f && visual.sy>.2f &&
           visual.sz>1.0f && visual.accessoryScale>0);
    assert(names.insert(visual.asset).second);
    assert(descriptions.insert(visual.silhouetteDescription).second);
  }
  assert(names.size()==5);
  const CreatureKind species[12]={CreatureKind::Cinderjaw,CreatureKind::PaleStrider,
    CreatureKind::Veilwatch,CreatureKind::GleamMoth,CreatureKind::HollowHound,
    CreatureKind::GriefLantern,CreatureKind::Inkmaw,CreatureKind::RedactionEye,
    CreatureKind::SaltglassCrawler,CreatureKind::DrownedWatch,
    CreatureKind::Ashmaw,CreatureKind::KilnWisp};
  std::set<std::string> creatureNames;
  for(auto kind:species){
    const auto& visual=CreatureGraybox(kind);
    assert(visual.species==kind && visual.sx>0 && visual.sy>0 &&
           visual.sz>0 && visual.crestScale>0 && visual.crestHeight>0);
    assert(creatureNames.insert(visual.asset).second);
  }
  assert(creatureNames.size()==12);
  std::cout<<"PASS: five established resident silhouettes and twelve distinctive creature profiles\\n";
}
