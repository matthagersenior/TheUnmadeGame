#pragma once
// A real alternate battlefield affordance from reading local physical evidence.
// No permanent reward, quest resolution, NPC telepathy or automatic victory.
// The existing free lever and timed two-station counter remain available.
#include <array>
#include <cmath>
namespace UnmadeCore {
struct EvidenceCounterplaySpec {
 const char* name;
 const char* intervention;
 double exposureSeconds;
 double coverSeconds;
 double cooldownSeconds;
 int offsetX,offsetY;
};
inline constexpr std::array<EvidenceCounterplaySpec,9> EvidenceCounterplaySpecs={{
 {"Charter Counterseal","Anchor the denied road in the Tollkeeper's charter ring.",5,0,30,-890,360},
 {"Drywater Pressure Proof","Reverse the pressure mark and raise a brief breakwater shield.",4,5,32,910,460},
 {"Common Kiln Diagram","Use the witnessed ash circuit to interrupt the Provost's heat.",6,0,34,-940,470},
 {"Foreclosure Counterfoil","Set the debtor's real date against the ledger; take temporary shelter.",5,6,33,930,510},
 {"Choir Missing Rest","Ground the absent note and hide behind its load-bearing stave.",5,3,35,-840,510},
 {"Yesterday Seam Evidence","Stabilize contradictory ancestry seams during the Claimant's fracture.",7,0,36,870,380},
 {"Occupied Census Testimony","Mark the occupied bed as present and take brief census cover.",4,8,37,-930,420},
 {"Unsigned Graft Refusal","Route the living root around the crown graft for an opening.",6,3,38,900,560},
 {"Honest Threshold Pair","Hold both conflicting threshold records; cover the next paradox echo.",6,5,40,-780,480}
}};
enum class EvidenceCounterplayResult {Invalid,NoLiveBoss,MissingProof,Recovering,Active};
class EvidenceCounterplay {
 double readyAt=0,last=-1;
public:
 double ReadyAt()const noexcept{return readyAt;}
 EvidenceCounterplayResult Use(int arc,double now,bool personalEvidence,
  bool dungeonRecords,bool bossResolved,bool livingBoss) noexcept{
  if(arc<0||arc>=9||!std::isfinite(now)||now<0||now<last)
   return EvidenceCounterplayResult::Invalid;
  last=now;
  if(!livingBoss||bossResolved)return EvidenceCounterplayResult::NoLiveBoss;
  if(!personalEvidence&&!dungeonRecords)return EvidenceCounterplayResult::MissingProof;
  if(now<readyAt)return EvidenceCounterplayResult::Recovering;
  readyAt=now+EvidenceCounterplaySpecs[arc].cooldownSeconds;
  return EvidenceCounterplayResult::Active;
 }
};
} // namespace UnmadeCore
