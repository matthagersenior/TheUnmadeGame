#include "realm_chapter_fixture.h"
#include "Story/UnmadeCommitmentRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
using RealmFixture::Chapters;
using RealmFixture::Same;
using SaveFixture::Record;
struct Endgame:Chapters {
 FinalJourney story;SaveFixture::Position position{ -200000,3350,155 };
 InventoryModel inventory;
 Record Capture()const{
  auto r=Chapters::Capture();r.bHasFinalEncounterSnapshot=true;const auto f=story.Snapshot();r.FinalEncounterStage=f.act;r.FinalMorningChoice=f.morning;r.FinalMemorySeed=f.memorySeed;
  r.bHasPlayerResume=true;r.ResumePosition=position;r.ResumeHealth=37;r.ResumeStamina=19;
  const auto items=inventory.Snapshot();r.bHasInventorySnapshot=true;r.ItemQuantities.assign(items.quantities.begin(),items.quantities.end());r.EquippedItems.assign(items.equipped.begin(),items.equipped.end());r.AwardedMilestoneBits=items.milestones;
  return r;
 }
 void Load(const Record& r){
  Chapters::Load(r);assert(story.Restore({r.FinalEncounterStage,r.FinalMorningChoice,r.FinalMemorySeed}));position=r.ResumePosition;
  InventorySnapshot items;for(int i=0;i<r.ItemQuantities.Num();++i)items.quantities[i]=r.ItemQuantities[i];for(int i=0;i<3;++i)items.equipped[i]=r.EquippedItems[i];items.milestones=r.AwardedMilestoneBits;assert(inventory.Restore(items));
 }
};
static void Identical(const Record& a,const Record& b){
 Same(a,b);assert(a.FinalEncounterStage==b.FinalEncounterStage&&a.FinalMorningChoice==b.FinalMorningChoice&&a.FinalMemorySeed==b.FinalMemorySeed);
 assert(a.ItemQuantities==b.ItemQuantities&&a.EquippedItems==b.EquippedItems&&a.AwardedMilestoneBits==b.AwardedMilestoneBits);
 assert(a.ResumeHealth==b.ResumeHealth&&a.ResumeStamina==b.ResumeStamina);
 assert(a.ResumePosition.X==b.ResumePosition.X&&a.ResumePosition.Y==b.ResumePosition.Y&&a.ResumePosition.Z==b.ResumePosition.Z);
}
int main(){
 int worlds=0,reloads=0,failures=0;
 for(int seed=0;seed<6;++seed)for(auto morning:{Morning::Anchor,Morning::Many}){
  Endgame live;assert(live.inventory.Claim(Achievement::Starter)==RewardResult::Awarded);
  for(int i=0;i<6;++i){const auto& f=LaterRealms[i];assert(live.first.Visit(f.realm)==LaterResult::Visited);assert(live.first.Begin(f.realm,f.initiator)==LaterResult::Started);const int choice=(seed+i)%2+1;assert(live.first.Study(f.realm,choice,choice==1?f.trialEvidenceA:f.trialEvidenceB)==LaterResult::EvidenceFound);assert(live.first.Operate(f.realm,f.mechanism)==LaterResult::Completed);}
  const auto* friendId=ResidentContinuityId(seed);const auto* stranger=ResidentContinuityId(seed+6);
  assert(live.residents.Talk(friendId,1)==EncounterEvent::FirstHello);assert(live.residents.Talk(friendId,2)==EncounterEvent::ReturnHello);
  SaveFixture::Storage store;assert(store.Commit(live.Capture()));
  auto checkpoint=[&]{const auto next=live.Capture();assert(store.Commit(next));for(int repeat=0;repeat<3;++repeat){assert(store.Load());live.Load(*store.Load());Identical(next,live.Capture());++reloads;}};
  assert(live.story.BreakMask(live.first.IsComplete(Realm::FirstAbsence),seed)==FinalResult::MaskBroken);checkpoint();assert(live.story.BreakCore()==FinalResult::TrueVictory);checkpoint();
  const auto pending=live.Capture();
  {int calls=0;assert(CommitFinalMorningAt(live.story,Morning::Unchosen,live.position,SaveFixture::Position{0,220,155},[&](const auto&){++calls;return true;},[&]{++calls;return true;})==FinalMorningCommit::Unavailable);assert(calls==0);Identical(pending,live.Capture());}

  CommitmentGate consent;assert(consent.Attempt("TheLastAnswer",static_cast<int>(morning),10)==CommitmentAttempt::Preview);assert(live.story.World()==Morning::Unchosen);assert(consent.Attempt("TheLastAnswer",static_cast<int>(morning),11)==CommitmentAttempt::Confirmed);
  for(int fault=1;fault<=2;++fault){
   Endgame failed=live;auto damaged=store;damaged.failAt=damaged.writes+fault;damaged.damageOnFailure=true;
   const auto result=CommitFinalMorningAt(failed.story,morning,failed.position,SaveFixture::Position{0,220,155},[&](const auto& at){failed.position=at;return true;},[&]{return damaged.Commit(failed.Capture());});
   assert(result==FinalMorningCommit::SaveFailed);Identical(pending,failed.Capture());assert(damaged.Load());Identical(pending,*damaged.Load());++failures;
   // A later independent save cannot accidentally persist the staged morning.
   assert(damaged.Commit(failed.Capture()));assert(damaged.Load());Identical(pending,*damaged.Load());
  }
  {Endgame failed=live;int writes=0;
   auto result=CommitFinalMorningAt(failed.story,morning,failed.position,SaveFixture::Position{0,220,155},[&](const auto& at){failed.position=at;return at.X!=0;},[&]{++writes;return true;});
   assert(result==FinalMorningCommit::RelocationFailed&&writes==0);Identical(pending,failed.Capture());++failures;
  }
  assert(CommitFinalMorningAt(live.story,morning,live.position,SaveFixture::Position{0,220,155},[&](const auto& at){live.position=at;return true;},[&]{return store.Commit(live.Capture());})==FinalMorningCommit::Applied);
  const auto saved=*store.Load();assert(saved.ResumePosition.X==0&&saved.ResumePosition.Y==220&&saved.ResumePosition.Z==155);assert(saved.FinalMorningChoice==static_cast<int>(morning));
  Same(pending,saved);assert(pending.ItemQuantities==saved.ItemQuantities&&pending.EquippedItems==saved.EquippedItems&&pending.AwardedMilestoneBits==saved.AwardedMilestoneBits);
  checkpoint();assert(live.story.MemorySeed()==seed);
  for(int realm=0;realm<9;++realm){const auto r=static_cast<Realm>(realm);assert(std::strlen(FinalLocalRecord(morning,r))>40);}
  assert(live.residents.Level(stranger)==Familiarity::Stranger);assert(std::strstr(ResidentContinuityLine(live.residents.Level(stranger),false,true,true),"begin"));assert(std::strstr(ResidentContinuityLine(live.residents.Level(friendId),false,true,true),"sky"));assert(!live.residents.Aided(friendId));
  assert(live.story.WakeEcho()==FinalResult::EchoStarted);checkpoint();assert(live.story.SettleEcho()==FinalResult::EchoDefeated);checkpoint();
  assert(live.story.ChooseWorld(morning==Morning::Anchor?Morning::Many:Morning::Anchor)==FinalResult::AlreadyDone);assert(live.story.SettleEcho()==FinalResult::AlreadyDone);
  {int calls=0;assert(CommitFinalMorningAt(live.story,morning,live.position,SaveFixture::Position{0,220,155},[&](const auto&){++calls;return true;},[&]{++calls;return true;})==FinalMorningCommit::Unavailable);assert(calls==0);}
  assert(live.story.World()==morning);assert(live.story.MemorySeed()==seed);assert(live.residents.Visits(friendId)==2);assert(live.residents.Visits(stranger)==0);checkpoint();++worlds;
 }
 std::cout<<"PASS: "<<worlds<<" final world/seed paths; "<<reloads<<" full reloads; "<<failures<<" failed writes/moves; return position, items and individual histories persist\n";
}
