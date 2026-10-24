#pragma once
#include "World/UnmadeTimedProgressRules.h"
#include "World/UnmadeConfluenceRules.h"
namespace UnmadeCore {
// Earned facts and involuntary consequences remain live after write failure.
// They are captured by every combined writer, rather than replaying the action.
class ChronicleHistory final {
public:
 bool Pending()const{return dirty_;}
 bool RecordDeed(TenfoldChronicle& chronicle,Deed deed){
  const bool changed=chronicle.RecordDeed(deed);dirty_=dirty_||changed;return changed;
 }
 bool Threaten(TenfoldChronicle& chronicle){
  // Breaking and restitution are distinct intents. A threat never redeems.
  const bool changed=chronicle.BreakOath();dirty_=dirty_||changed;return changed;
 }
 bool VerifyRoute(TenfoldChronicle& chronicle,int realm){
  if(chronicle.Stage(RiteId::Cartography)<2)return false;
  const bool changed=chronicle.VerifyRoute(realm,2,true);dirty_=dirty_||changed;return changed;
 }
 template<class Writer> TimedProgress Attempt(bool ready,double now,Writer write){
  if(!dirty_)return TimedProgress::Unchanged;
  return retry_.Attempt(ready,now,[&]{if(!write())return false;dirty_=false;return true;});
 }
private:
 bool dirty_=false;TimedProgressRetry retry_;
};
enum class OathChoice { Unavailable, NeedsRestitution, Broken, Redeemed, SaveFailed };
// Explicit player intent remains a transaction: failure accepts no new choice.
template<class Writer> OathChoice CommitOathChoice(TenfoldChronicle& chronicle,
 const RiteContext& context,Writer write){
 const auto before=chronicle;
 const bool redeem=chronicle.Snapshot().brokenOaths==1&&!chronicle.Snapshot().oathRedeemed;
 if(redeem){if(!chronicle.RedeemOath(context))return OathChoice::NeedsRestitution;}
 else if(!chronicle.BreakOath())return OathChoice::Unavailable;
 if(!write()){chronicle=before;return OathChoice::SaveFailed;}
 return redeem?OathChoice::Redeemed:OathChoice::Broken;
}
// Remove both authored oath choices, while preserving another rite's buff.
inline bool ClearBrokenOathPower(RiteId& active,int& attack,int& armor,double& expires){
 if(active!=RiteId::Oathbinding)return false;
 attack=armor=0;expires=0;active=RiteId::Count;return true;
}
// One serializer for component writes and the central accepted-live-state capture.
// Copy adapts std::array to the destination's container (TArray in Unreal).
template<class Record,class Copy>
void CaptureChronicleRecord(const TenfoldChronicle& chronicle,
 const ConfluenceJourney& confluence,Record& save,Copy copy){
 const auto& s=chronicle.Snapshot();const auto& c=confluence.Snapshot();
 copy(save.RiteStages,s.stage);copy(save.RiteChoices,s.choice);copy(save.RiteReadyAt,s.readyAt);
 copy(save.DisciplineMastery,s.disciplineMastery);
 save.RiteTrialBits=s.trialUsed;save.LegacyDeedBits=s.deeds;save.VerifiedRoadBits=s.verifiedRoutes;
 save.TomorrowDebtDueDay=s.debtDueDay;save.TomorrowDebtExhaustedUntil=s.exhaustedUntilDay;
 save.BrokenOathCount=s.brokenOaths;save.bOathActive=s.oathActive;save.bOathRedeemed=s.oathRedeemed;
 save.bHasTenfoldChronicle=true;
 copy(save.ConfluenceStages,c.stage);copy(save.ConfluenceDecisions,c.decisions);
 copy(save.ConfluenceCastMasks,c.casts);copy(save.ConfluenceWindowEnds,c.windowEnds);
 save.bHasConfluenceSnapshot=true;
}
}
