#pragma once
// Discovery → named local testimony → deliberate civic choice → physical aftermath.
// Uses the existing nine-slot RealmAftermathSnapshot and expedition records;
// no new save field, global broadcast, morality meter or automatic boss victory.
#include "World/UnmadeRealmAftermathRules.h"
#include "World/UnmadeExpeditionRules.h"
#include <array>
#include <cstring>

namespace UnmadeCore {
struct DiscoveryCivicSpec {
 const char* initiator;
 const char* careWitness;
 const char* truthWitness;
 const char* title;
 const char* careWork;
 const char* truthWork;
 // Worksite coordinates are offsets from the already-built expedition frontier.
 int mechanismX,careX,truthX;
};
inline constexpr std::array<DiscoveryCivicSpec,9> DiscoveryCivicSites={{
 {"npc.expedition.0.0","npc.expedition.0.3","npc.expedition.0.4",
  "The Returned Charter","Shared traveler way and screened refuge","Redacted public survey dais",
  -1060,-1280,1360},
 {},{},
 {"npc.expedition.3.0","npc.expedition.3.3","npc.expedition.3.4",
  "Debt Without Consent","Protected ferry bond crossing","Public creditor-counterfoil archive",
  -1110,-1390,1220},
 {"npc.expedition.4.0","npc.expedition.4.3","npc.expedition.4.4",
  "The Choir's Missing Rest","Community counterweight span","Witnessed dissenting-score platform",
  -1180,-1460,1400},
 {"npc.expedition.5.0","npc.expedition.5.3","npc.expedition.5.4",
  "The Unmined Yesterday","Guarded household seam bridge","Redacted descendants' record hall",
  -980,-1270,1540},
 {"npc.expedition.6.0","npc.expedition.6.3","npc.expedition.6.4",
  "The Occupied Blank","Anonymous shelter entrance","Witness-approved census hearing",
  -1140,-1560,1250},
 {"npc.expedition.7.0","npc.expedition.7.3","npc.expedition.7.4",
  "The Crownless Orchard","Unowned orchard access path","Unclaimed title-review platform",
  -1030,-1340,1480},
 {"npc.expedition.8.0","npc.expedition.8.3","npc.expedition.8.4",
  "Two Honest Thresholds","Sheltered second passage","Dual-origin evidence gallery",
  -1200,-1460,1300}
}};
inline bool CivicUsesExpeditionAftermath(int arc) noexcept {
 return arc>=0&&arc<9&&arc!=1&&arc!=2;
}
inline const DiscoveryCivicSpec* CivicSite(int arc) noexcept {
 return CivicUsesExpeditionAftermath(arc)?&DiscoveryCivicSites[arc]:nullptr;
}
inline bool CivicEvidenceReady(int arc,const ExpeditionState* local) noexcept {
 return CivicUsesExpeditionAftermath(arc)&&local&&local->returned==1&&
        local->boss!=0&&(local->rooms&1)!=0&&local->clues==7;
}
inline bool CivicEvidenceValidOnRestore(const RealmAftermathSnapshot& aftermath,
 const Expeditions& expeditions,bool expeditionPresent) noexcept {
 for(int arc=0;arc<9;++arc)
  if(CivicUsesExpeditionAftermath(arc)&&aftermath.stage[arc]>0&&
     (!expeditionPresent||!CivicEvidenceReady(arc,expeditions.Arc(arc))))
   return false;
 return true;
}
inline bool CivicWorkAuthorized(int arc,int person,int stage,int ending,int choice) noexcept {
 return CivicUsesExpeditionAftermath(arc)&&stage==4&&
  (choice==1||choice==2)&&ending==choice&&
  (person==0||person==choice);
}
// Saltwake already has its own distinct, complete physical aftershock quest.
// Its original save gates are unchanged; learned expedition evidence can deepen
// witness dialogue and annotate the existing branch without inventing a new one.
inline bool CivicSaltwakeWitnessedDiscovery(int firstRoom,int records,int returned) noexcept {
 return firstRoom!=0&&records==7&&returned==1;
}
} // namespace UnmadeCore
