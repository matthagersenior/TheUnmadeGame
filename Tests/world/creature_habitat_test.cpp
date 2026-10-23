#include "World/UnmadeCreatureHabitatRules.h"
#include <cassert>
#include <cmath>
#include <limits>
#include <string_view>
int main(){
 using namespace UnmadeCore;
 static_assert(HabitatSites.size()==6,"Six existing encounter sites");
 constexpr const char* ids[]={"Reach.AshRoad","Reach.GlassCut","Bellwold.Forgotten",
  "Paperhaven.Torn","Saltwake.Saltglass","Cinderhold.Slag"};
 for(unsigned i=0;i<HabitatSites.size();++i){
  const auto& s=HabitatSites[i];
  assert(std::string_view(s.id)==ids[i]);
  assert(s.denOffsetY>500&&s.denOffsetY<1000);
  assert(s.warningRadius<s.trackRadius);
  assert(s.inspectionRadius<s.warningRadius);
  assert(std::string_view(s.kind).size()>3);
  const auto unobserved=HabitatReaction(s,s.trackRadius-.5,false,false,false);
  assert(unobserved==HabitatCue::Tracks);
  assert(HabitatReaction(s,s.warningRadius-.5,false,false,false)==HabitatCue::Warning);
  assert(HabitatReaction(s,s.inspectionRadius-.5,false,false,false)==HabitatCue::Disturbed);
  assert(HabitatReaction(s,s.inspectionRadius-.5,true,false,true)==HabitatCue::Sheltered);
  assert(HabitatReaction(s,s.trackRadius+100,true,false,false)==HabitatCue::Quiet);
  assert(HabitatReaction(s,-1,false,false,false)==HabitatCue::Quiet);
  assert(HabitatReaction(s,std::numeric_limits<double>::quiet_NaN(),false,false,false)==HabitatCue::Quiet);
  assert(HabitatCanInspect(s,s.inspectionRadius-2,true));
  assert(!HabitatCanInspect(s,s.inspectionRadius-2,false));
  assert(!HabitatCanInspect(s,s.inspectionRadius+1,true));
  // Observation is readable counterplay, not an unlocked enemy power.
  assert(!HabitatGrantsAbility(HabitatCue::Sheltered));
  assert(!HabitatGrantsAbility(HabitatCue::Disturbed));
 }
 // The clock may affect the intensity but cannot force hostility through cover.
 assert(HabitatReaction(HabitatSites[2],250,true,true,true)==HabitatCue::Sheltered);
}