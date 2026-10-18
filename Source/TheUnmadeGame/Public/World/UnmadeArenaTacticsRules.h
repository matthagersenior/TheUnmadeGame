#pragma once
// Nine arena counters: real two-station positioning under active boss pressure.
// Transient combat state intentionally does not enter the durable save schema.
// Boss results and freed residents are read from the existing expedition ledger.
#include "World/UnmadeExpeditionRules.h"
#include <array>
#include <cmath>
namespace UnmadeCore {
struct ArenaTacticSpec {
 const char* name;
 const char* counterObjective;
 int first; // station 0 or 1, specific to each realm's environmental mechanism
 double openWindow; // seconds to reach the other station while still in combat
 double cooldown; // boss counter cannot be farmed on every interaction
 double exposureSeconds;
 double shelterSeconds;
};
inline constexpr std::array<ArenaTacticSpec,9> Arenas={{
 {"Split Charter Reversal","Turn the charter seal, then its opposing road-anchor before the Tollkeeper's warning.",0,7,27,5,0},
 {"Breakwater Backflow","Brace the floodgate before reversing the pressure lock; hide from the Bailiff's aim.",1,6,29,4,7},
 {"Kiln Exhaust Crossfeed","Open the ash return, then vent the common furnace against the Provost's heat.",0,8,31,6,0},
 {"Foreclosure Counterfoil","Release the debtor seal, then strike its matching ledger anchor across the room.",1,7,30,4,8},
 {"Choir Counterweight","Tune the high note, then ground the opposing weight before the cantor finishes a phrase.",0,6,32,5,0},
 {"Seam-Shear Brace","Brace the living seam, then break the claimant's fracture tension across the mine.",1,9,33,6,0},
 {"Unlisted Presence","Affirm the occupied bed, then invalidate the empty census before the Registrar's bell.",0,7,31,4,8},
 {"Rootcourt Refusal","Disarm the crown graft, then interrupt its other root without claiming the throne.",1,8,34,5,0},
 {"Paradox Aperture","Hold one truthful threshold, then open its opposite before the first witness is erased.",0,6,37,6,7}
}};
enum class CounterStep {Blocked,WrongOrder,Armed,AlreadyArmed,Expired,Success,Cooldown};
class ArenaCounter {
 double armedAt=-1,lockedUntil=0,last=-1;
public:
 void Reset()noexcept{armedAt=-1;lockedUntil=0;last=-1;}
 CounterStep Use(int arc,int station,double now,bool bBossReady)noexcept {
  if(arc<0||arc>=9||station<0||station>1||!std::isfinite(now)||
     now<0||(!bBossReady)||now<last)return CounterStep::Blocked;
  last=now;
  const auto& counter=Arenas[arc];
  if(now<lockedUntil)return CounterStep::Cooldown;
  if(armedAt>=0&&now>armedAt+counter.openWindow) {
   armedAt=-1;
   return CounterStep::Expired;
  }
  if(armedAt<0) {
   if(station!=counter.first)return CounterStep::WrongOrder;
   armedAt=now;
   return CounterStep::Armed;
  }
  if(station==counter.first)return CounterStep::AlreadyArmed;
  armedAt=-1;lockedUntil=now+counter.cooldown;
  return CounterStep::Success;
 }
};
enum class ArenaAftermath {None,UnfinishedRescue,Rescued,WitnessedYield};
inline ArenaAftermath ArenaAftermathRecord(const ExpeditionState& e)noexcept {
 if(!e.returned||!e.boss)return ArenaAftermath::None;
 if(e.boss==2)return ArenaAftermath::WitnessedYield;
 return e.rescued==3?ArenaAftermath::Rescued:ArenaAftermath::UnfinishedRescue;
}
inline const char* ArenaAftermathText(int arc,ArenaAftermath result)noexcept {
 if(arc<0||arc>=9||result==ArenaAftermath::None)return "";
 if(result==ArenaAftermath::UnfinishedRescue)
   return "The enemy fell, but not everyone came home. The unopened cells remain a responsibility.";
 static constexpr std::array<const char*,9> rescued={{
  "Rookstep's freed surveyors can testify that the road was used to imprison people, not merely collect tolls.",
  "Brinewatch's rescued divers return to water work; no paper claim can replace who survived the pressure locks.",
  "Emberwatch's furnace tenders are out of the galleries, but the city's common heat still needs accounting.",
  "Oathshoal's freed ferrymen can challenge being held as collateral; their own debts are not automatically void.",
  "Downsong's rescued riggers stand clear of the choir engine; rebuilding the public span still costs labor.",
  "Seamguard's living workers have their own names and claims; the seam register cannot speak for them.",
  "Elseward's caretakers have been released, and their existence cannot be reduced to a blank census line.",
  "Rootwatch's gardeners return alive; none is automatically bound to serve an unwritten castellan.",
  "The first threshold's captives return to their lives without being required to agree about one true origin."
 }};
 static constexpr std::array<const char*,9> yielded={{
  "The Tollkeeper yielded to the charter evidence. Its collectors stand down, but sealed rooms still need rescue.",
  "The Bailiff surrendered to the genuine levy register; the port must still examine the water accounts.",
  "The Provost yielded before the valve diagram. Cinderhold must still decide who controls the common furnace.",
  "The Interest Warden yielded to the counterfoil; witnesses still dispute what debts are valid.",
  "The Cantor ceased its attack before the tuning proof, but the choir still carries the cost of the span.",
  "The Last Claimant yielded before the seam record; Vathless must hear its descendants in person.",
  "The Registrar yielded to the presence ledger; Eillun's people still demand ordinary recognition.",
  "The Castellan yielded before the common root claim; no surrender appoints a new king.",
  "The threshold guardian yielded before contradictory testimony; the worlds remain free to disagree."
 }};
 return (result==ArenaAftermath::Rescued?rescued[arc]:yielded[arc]);
}
} // namespace UnmadeCore
