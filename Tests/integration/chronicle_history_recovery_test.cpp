#include "rite_cast_fixture.h"
#include "World/UnmadeChronicleHistoryRules.h"
#include "Save/UnmadeCaptureRules.h"
#include "UI/UnmadePlayerFacingRules.h"
#include <iostream>
using namespace UnmadeCore;
using SaveFixture::Record;
static void Capture(const RiteFixture::State& live,Record& r){
 CaptureChronicleRecord(live.rites,live.trials,r,[](auto& target,const auto& source){target.assign(source.begin(),source.end());});
}
static void SameHistory(const Record& a,const Record& b){
 assert(a.RiteStages==b.RiteStages&&a.RiteChoices==b.RiteChoices&&a.RiteReadyAt==b.RiteReadyAt&&a.DisciplineMastery==b.DisciplineMastery);
 assert(a.RiteTrialBits==b.RiteTrialBits&&a.LegacyDeedBits==b.LegacyDeedBits&&a.VerifiedRoadBits==b.VerifiedRoadBits);
 assert(a.BrokenOathCount==b.BrokenOathCount&&a.bOathActive==b.bOathActive&&a.bOathRedeemed==b.bOathRedeemed);
 assert(a.TomorrowDebtDueDay==b.TomorrowDebtDueDay&&a.TomorrowDebtExhaustedUntil==b.TomorrowDebtExhaustedUntil);
 assert(a.ConfluenceStages==b.ConfluenceStages&&a.ConfluenceDecisions==b.ConfluenceDecisions&&a.ConfluenceCastMasks==b.ConfluenceCastMasks&&a.ConfluenceWindowEnds==b.ConfluenceWindowEnds);
}
int main(){int paths=0,reloads=0;
 for(int realm=0;realm<9;++realm)for(int profile=1;profile<=3;++profile)for(int event=0;event<5;++event)for(int fault=1;fault<=2;++fault){
  RiteFixture::State live;RiteFixture::MasterAll(live);auto t=live.rites.Snapshot();t.oathActive=true;for(int i=0;i<10;++i)t.readyAt[i]=100+i;for(int i=0;i<6;++i)t.disciplineMastery[i]=i;assert(live.rites.Restore(t));auto trials=live.trials.Snapshot();trials.stage[0]=1;assert(live.trials.Restore(trials));
  Record initial=live.Capture();initial.bHasCharacterIdentity=true;initial.CharacterIdentityChoices.assign(8,0);initial.CharacterChosenName="Traveler "+std::to_string(profile);
  initial.bHasInventorySnapshot=true;InventoryModel inventory;const auto inv=inventory.Snapshot();initial.ItemQuantities.assign(inv.quantities.begin(),inv.quantities.end());initial.EquippedItems.assign(inv.equipped.begin(),inv.equipped.end());
  SaveFixture::Storage store;assert(store.Commit(initial));assert(store.Commit(initial));
  // Other profile data is isolated from every current-world retry.
  SaveFixture::Storage other;auto foreign=initial;foreign.CharacterChosenName="Other";assert(other.Commit(foreign));const int otherWrites=other.writes;
  ChronicleHistory history;
  auto stage=[&]{if(event<3)return history.RecordDeed(live.rites,static_cast<Deed>(event));if(event==3)return history.Threaten(live.rites);return history.VerifyRoute(live.rites,realm%2);};
  assert(stage());assert(history.Pending());assert(!stage());
  SaveFixture::Npc witness;witness.NpcId="npc.realm."+std::to_string(realm);witness.Observations.push_back({{realm+1},"Player.Helped","subject","",0});
  int claims=0;
  auto write=[&]{++claims;Record r=initial;return PrepareCombinedCapture(r,SaveFixture::Valid,[&](auto& next){Capture(live,next);next.NpcSnapshots.push_back(witness);return true;})&&store.Commit(r);};
  assert(history.Attempt(false,0,write)==TimedProgress::Deferred);assert(claims==0);
  store.failAt=store.writes+fault;store.damageOnFailure=true;
  assert(history.Attempt(true,0,write)==TimedProgress::FirstFailure);assert(history.Pending());
  const auto earned=live.Capture();assert(store.Load());SameHistory(initial,*store.Load());
  assert(CommitSessionCheckpoint(history.Pending(),[]{assert(false);return true;})==CheckpointSave::PendingDefeat);
  for(int i=1;i<50;++i){assert(!stage());assert(history.Attempt(true,i/10.,write)==TimedProgress::Deferred);}assert(claims==1);
  assert(history.Attempt(true,5,write)==TimedProgress::Saved);assert(!history.Pending());assert(store.Load());SameHistory(earned,*store.Load());
  assert(store.Load()->NpcSnapshots.size()==1&&store.Load()->NpcSnapshots[0].Observations[0].EventId.value==realm+1);
  assert(store.Load()->ItemQuantities==initial.ItemQuantities&&store.Load()->CharacterChosenName==initial.CharacterChosenName);
  const int writes=store.writes;
  for(int i=0;i<12;++i){RiteFixture::State restored;restored.Load(*store.Load());SameHistory(earned,restored.Capture());assert(!stage());assert(history.Attempt(true,10+i,write)==TimedProgress::Unchanged);++reloads;}
  assert(store.writes==writes&&other.writes==otherWrites&&other.Load()->CharacterChosenName=="Other");++paths;
 }
 // Explicit oath choices roll back on both write targets; failed redemption
 // never consumes the scar or accepts an uncommitted choice.
 for(int redeem=0;redeem<=1;++redeem)for(int fault=1;fault<=2;++fault){
  RiteFixture::State chosen;RiteFixture::MasterAll(chosen);auto s=chosen.rites.Snapshot();s.oathActive=true;s.deeds=7;assert(chosen.rites.Restore(s));if(redeem)assert(chosen.rites.BreakOath());
  RiteContext evidence;evidence.factionEndings=1;evidence.bellQuestComplete=true;evidence.verifiedWitnesses=3;
  SaveFixture::Storage target;auto before=chosen.Capture();assert(target.Commit(before));assert(target.Commit(before));target.failAt=target.writes+fault;target.damageOnFailure=true;
  assert(CommitOathChoice(chosen.rites,evidence,[&]{return target.Commit(chosen.Capture());})==OathChoice::SaveFailed);SameHistory(before,chosen.Capture());assert(target.Load());SameHistory(before,*target.Load());
  assert(CommitOathChoice(chosen.rites,evidence,[&]{return target.Commit(chosen.Capture());})==(redeem?OathChoice::Redeemed:OathChoice::Broken));
  for(int i=0;i<12;++i){RiteFixture::State restored;restored.Load(*target.Load());assert(restored.rites.Snapshot().brokenOaths==1&&restored.rites.Snapshot().oathRedeemed==bool(redeem));}
 }
 // New facts arriving during an outage retain the original retry cadence.
 {TenfoldChronicle chronicle;auto t=chronicle.Snapshot();t.oathActive=true;assert(chronicle.Restore(t));ChronicleHistory pending;
  assert(!pending.RecordDeed(chronicle,static_cast<Deed>(99)));assert(!pending.VerifyRoute(chronicle,0));assert(!pending.Pending());
  assert(pending.RecordDeed(chronicle,Deed::Protected));assert(pending.Attempt(true,0,[]{return false;})==TimedProgress::FirstFailure);
  assert(pending.RecordDeed(chronicle,Deed::Discovered));assert(pending.Threaten(chronicle));assert(pending.Attempt(true,2,[]{assert(false);return true;})==TimedProgress::Deferred);
  assert(pending.Attempt(true,5,[&]{assert(chronicle.Snapshot().deeds==3&&chronicle.Snapshot().brokenOaths==1);return true;})==TimedProgress::Saved);
  assert(pending.RecordDeed(chronicle,Deed::Reconciled));assert(pending.Attempt(true,5,[]{return true;})==TimedProgress::Saved);
 }
 // Threat intent cannot redeem, even when every restitution prerequisite holds.
 RiteFixture::State live;RiteFixture::MasterAll(live);auto t=live.rites.Snapshot();t.oathActive=true;t.deeds=7;assert(live.rites.Restore(t));ChronicleHistory h;assert(h.Threaten(live.rites));
 RiteContext c;c.factionEndings=1;c.bellQuestComplete=true;c.verifiedWitnesses=3;
 for(int i=0;i<20;++i){assert(!h.Threaten(live.rites));assert(!live.rites.Snapshot().oathRedeemed&&live.rites.Snapshot().brokenOaths==1);}
 // Deliberate restitution still requires the authored evidence and keeps the scar.
 c.verifiedWitnesses=2;assert(!live.rites.RedeemOath(c));c.verifiedWitnesses=3;assert(live.rites.RedeemOath(c));assert(live.rites.Snapshot().oathRedeemed&&live.rites.Snapshot().brokenOaths==1);assert(!h.Threaten(live.rites));
 for(int choice=1;choice<=2;++choice){RiteId active=RiteId::Oathbinding;int attack=choice==2?14:0,armor=choice==1?14:0;double expires=40;assert(ClearBrokenOathPower(active,attack,armor,expires));assert(attack==0&&armor==0&&expires==0&&active==RiteId::Count);assert(!ClearBrokenOathPower(active,attack,armor,expires));}
 for(auto rite:{RiteId::BorrowedLives,RiteId::TomorrowDebt,RiteId::LegacyForging}){auto active=rite;int attack=9,armor=7;double expires=40;assert(!ClearBrokenOathPower(active,attack,armor,expires));assert(active==rite&&attack==9&&armor==7&&expires==40);}
 // A different subsystem writer captures failed-but-live history too.
 SaveFixture::Storage store;auto old=live.Capture();assert(store.Commit(old));assert(store.Commit(old));ChronicleHistory dirty;auto changed=live.rites.Snapshot();changed.deeds=0;assert(live.rites.Restore(changed));assert(dirty.RecordDeed(live.rites,Deed::Protected));
 assert(dirty.Attempt(true,0,[]{return false;})==TimedProgress::FirstFailure);
 Record candidate=old;assert(PrepareCombinedCapture(candidate,SaveFixture::Valid,[&](auto& r){Capture(live,r);return true;}));assert(store.Commit(candidate));assert(store.Load()->LegacyDeedBits==1);
 // Valid older schema-1 records may omit these sections. Capturing a fresh
 // owner adds valid default sections without changing unrelated authored data.
 {Record legacy;legacy.bHasCharacterIdentity=true;legacy.CharacterIdentityChoices.assign(8,0);legacy.CharacterChosenName="Older traveler";RiteFixture::State fresh;
  assert(!legacy.bHasTenfoldChronicle&&!legacy.bHasConfluenceSnapshot&&SaveFixture::Valid(legacy));
  assert(PrepareCombinedCapture(legacy,SaveFixture::Valid,[&](auto& r){Capture(fresh,r);return true;}));
  assert(legacy.bHasTenfoldChronicle&&legacy.bHasConfluenceSnapshot&&legacy.LegacyDeedBits==0&&legacy.BrokenOathCount==0&&legacy.CharacterChosenName=="Older traveler");
 }
 // Raw invalid caller data cannot be silently repaired; captured corruption is
 // revalidated, and neither case replaces a good backup.
 const int writes=store.writes;candidate=old;candidate.RiteStages.clear();
 assert(!PrepareCombinedCapture(candidate,SaveFixture::Valid,[&](auto& r){Capture(live,r);assert(false);return true;}));
 candidate=old;assert(!PrepareCombinedCapture(candidate,SaveFixture::Valid,[&](auto& r){Capture(live,r);r.BrokenOathCount=2;return true;}));assert(store.writes==writes&&store.Load()->LegacyDeedBits==1);
 assert(!dirty.RecordDeed(live.rites,static_cast<Deed>(99)));assert(!dirty.VerifyRoute(live.rites,99));
 std::cout<<"PASS: "<<paths<<" nine-realm/profile history fault paths, "<<reloads<<" reloads, retained deeds/roads/oath, deliberate restitution, combined witness capture and backup integrity\n";
}
