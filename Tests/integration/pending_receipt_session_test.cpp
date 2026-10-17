// Portable checkpoint/profile policy with real progression models and combined
// fault storage. UE actor scanning, menus, travel and platform IO stay pending.
#include "realm_chapter_fixture.h"
#include "World/UnmadeRealmJourneyRules.h"
#include "UI/UnmadePlayerFacingRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
using RealmFixture::Chapters;
using RealmFixture::Same;
using SaveFixture::Record;
static void First(Chapters& s,int i){const auto& f=LaterRealms[i];assert(s.first.Visit(f.realm)==LaterResult::Visited);assert(s.first.Begin(f.realm,f.initiator)==LaterResult::Started);assert(s.first.Study(f.realm,1,f.trialEvidenceA)==LaterResult::EvidenceFound);assert(s.first.Operate(f.realm,f.mechanism)==LaterResult::Completed);}
static void Echo(Chapters& s,int i){First(s,i);const auto& a=RealmAftermathSpecs[i+3];const auto& e=EchoQuests[i];assert(s.aftermath.Begin(e.realm,true,true,a.initiatingWitness)==RealmAftermathResult::Started);assert(s.aftermath.Prepare(e.realm,a.mechanismSite)==RealmAftermathResult::Prepared);assert(s.aftermath.Inspect(e.realm,1,a.evidenceForCare)==RealmAftermathResult::EvidenceFound);assert(s.aftermath.Testify(e.realm,a.careWitness)==RealmAftermathResult::Witnessed);assert(s.aftermath.Decide(e.realm,1)==RealmAftermathResult::Resolved);assert(s.echo.Begin(e.realm,true,true,e.openingWitness)==EchoResult::Started);assert(s.echo.Inspect(e.realm,e.evidenceA)==EchoResult::Discovered);assert(s.echo.Inspect(e.realm,e.evidenceB)==EchoResult::Discovered);assert(s.echo.Testify(e.realm,e.witnessCare)==EchoResult::Testified);assert(s.echo.Commit(e.realm,1,e.ritualCare)==EchoResult::Committed);}
static void Identical(const Record& a,const Record& b){Same(a,b);assert(a.ExpeditionState==b.ExpeditionState);assert(a.FinalEncounterStage==b.FinalEncounterStage&&a.FinalMorningChoice==b.FinalMorningChoice&&a.FinalMemorySeed==b.FinalMemorySeed);}
template<class Pending,class Capture,class Commit>
static void Exercise(int fault,SaveFixture::Storage& current,Pending pending,Capture capture,Commit commit){
 const auto before=capture();assert(current.Commit(before));assert(pending());
 current.failAt=current.writes+fault;current.damageOnFailure=true;
 assert(!commit());assert(pending());Identical(before,capture());assert(current.Load());Identical(before,*current.Load());
 // The target profile/backup have their own history and are never written by
 // saving the current profile or by confirmation prompts.
 SaveFixture::Storage target;auto other=before;other.bHasCharacterIdentity=true;other.CharacterIdentityChoices.assign(8,0);other.CharacterChosenName="Another traveler";assert(target.Commit(other));assert(target.Commit(other));const int targetWrites=target.writes;
 const int failedWrites=current.writes;int checkpointWrites=0;
 auto save=[&]{return CommitSessionCheckpoint(pending(),[&]{++checkpointWrites;return current.Commit(capture());});};
 for(int repeat=0;repeat<20;++repeat)assert(save()==CheckpointSave::PendingDefeat);
 assert(checkpointWrites==0&&current.writes==failedWrites);
 SessionConfirmation choice;
 assert(!choice.Request(SessionIntent::Load,1,save()==CheckpointSave::Saved));
 assert(!choice.Request(SessionIntent::Load,2,save()==CheckpointSave::Saved)); // New target requires its own prompt.
 assert(choice.Request(SessionIntent::Load,2,save()==CheckpointSave::Saved)); // Explicit discard remains allowed.
 assert(target.Load()&&target.Load()->CharacterChosenName=="Another traveler");
 choice.Clear();assert(!choice.Request(SessionIntent::New,2,save()==CheckpointSave::Saved));assert(choice.Request(SessionIntent::New,2,save()==CheckpointSave::Saved));
 choice.Clear();assert(!choice.Request(SessionIntent::Recover,1,false));assert(choice.Request(SessionIntent::Recover,1,false));
 target.recovery=true;assert(target.Load()==&*target.backup); // No pre-recovery current-session save.
 assert(target.writes==targetWrites&&checkpointWrites==0&&current.writes==failedWrites);
 Identical(before,*current.Load());
 // Once the actual receipt commits, ordinary saves and a safe profile switch
 // can succeed without an unsaved-discard prompt.
 assert(commit());assert(!pending());assert(save()==CheckpointSave::Saved);assert(checkpointWrites==1);
 assert(current.Load());Identical(capture(),*current.Load());choice.Clear();assert(choice.Request(SessionIntent::Load,1,true));
 const auto complete=*current.Load();
 for(int repeat=0;repeat<3;++repeat){assert(current.Load());Identical(complete,*current.Load());}
 current.failAt=current.writes+1;assert(save()==CheckpointSave::Failed);
 choice.Clear();assert(!choice.Request(SessionIntent::Load,1,false));assert(current.Load());Identical(complete,*current.Load());
}
int main(){
 int guardians=0,phases=0,expeditions=0;
 for(int i=0;i<6;++i)for(int fault=1;fault<=2;++fault){
  Chapters s;Echo(s,i);SaveFixture::Storage store;const auto r=EchoQuests[i].realm;
  Exercise(fault,store,[&]{return s.guardians.Outcome(r)==GuardianOutcome::Unresolved;},[&]{return s.Capture();},[&]{const auto before=s.guardians.Snapshot();assert(s.guardians.Resolve(r,true,false)==GuardianResolution::Defeated);if(store.Commit(s.Capture()))return true;assert(s.guardians.Restore(before));return false;});++guardians;
 }
 for(int phase:{0,1,4})for(auto morning:{Morning::Anchor,Morning::Many})for(int fault=1;fault<=2;++fault){
  Chapters s;First(s,5);FinalJourney final;SaveFixture::Storage store;
  if(phase>=1)assert(final.BreakMask(true,3)==FinalResult::MaskBroken);
  if(phase==4){assert(final.BreakCore()==FinalResult::TrueVictory);assert(final.ChooseWorld(morning)==FinalResult::NewMorning);assert(final.WakeEcho()==FinalResult::EchoStarted);}
  auto capture=[&]{auto r=s.Capture();const auto f=final.Snapshot();r.bHasFinalEncounterSnapshot=true;r.FinalEncounterStage=f.act;r.FinalMorningChoice=f.morning;r.FinalMemorySeed=f.memorySeed;return r;};
  Exercise(fault,store,[&]{return PendingFinalDefeat(true,phase,final.Act());},capture,[&]{const auto before=final.Snapshot();const auto result=phase==0?final.BreakMask(true,3):phase==1?final.BreakCore():final.SettleEcho();assert(result==(phase==0?FinalResult::MaskBroken:phase==1?FinalResult::TrueVictory:FinalResult::EchoDefeated));if(store.Commit(capture()))return true;assert(final.Restore(before));return false;});
  // A dead actor left over from the committed phase must not block a checkpoint
  // or count as a defeat of a newly configured, still-unfought phase.
  assert(!PendingFinalDefeat(true,phase,final.Act()));assert(!PendingFinalDefeat(false,static_cast<int>(final.Act()),final.Act()));++phases;
 }
 for(int a=0;a<9;++a)for(int slot:{0,1,2,3,8})for(int fault=1;fault<=2;++fault){
  Expeditions e;assert(e.Brief(a));assert(e.Visit(a,0));for(int room=0;room<5;++room){assert(e.Enter(a,room));if(room>0&&room<4)assert(e.Inspect(a,room-1));}assert(e.Choose(a,2));assert(e.Enter(a,5));
  SaveFixture::Storage store;auto capture=[&]{Record r;r.bHasLivingWorldSnapshot=true;r.bHasExpeditions=true;r.ExpeditionState=e.Pack();return r;};
  Exercise(fault,store,[&]{return ExpeditionEncounterActive(*e.Arc(a),slot);},capture,[&]{const auto before=e.Pack();assert(slot==8?e.Boss(a,false):e.Kill(a,slot));if(store.Commit(capture()))return true;assert(e.Restore(before));return false;});++expeditions;
 }
 assert(!PendingFinalDefeat(true,-1,FinalAct::Veil));assert(!PendingFinalDefeat(true,2,FinalAct::AnswerPending));assert(!PendingFinalDefeat(true,3,FinalAct::OtherMorning));assert(!PendingFinalDefeat(true,5,FinalAct::EchoSettled));
 std::cout<<"PASS: "<<guardians<<" guardian, "<<phases<<" final-phase and "<<expeditions<<" expedition pending-receipt sessions; blocked writes, discard confirmation, profile/recovery isolation and post-commit saves\n";
}
