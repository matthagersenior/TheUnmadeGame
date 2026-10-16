// Exercises the actor-used retry scheduler with production chapter models and
// combined-record fault selection. Adapter rollback is statically checked;
// Unreal ticks, real SaveGame IO and presentation are not executed here.
#include "realm_chapter_fixture.h"
#include "Save/UnmadeDefeatSaveRetryRules.h"
#include "Story/UnmadeCommitmentRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
using RealmFixture::Chapters;
using RealmFixture::Same;
using SaveFixture::Record;
static void Identical(const Record& a,const Record& b){
 Same(a,b);assert(a.FinalEncounterStage==b.FinalEncounterStage&&a.FinalMorningChoice==b.FinalMorningChoice&&a.FinalMemorySeed==b.FinalMemorySeed);
}
static void First(Chapters& s,int i){
 const auto& f=LaterRealms[i];assert(s.first.Visit(f.realm)==LaterResult::Visited);assert(s.first.Begin(f.realm,f.initiator)==LaterResult::Started);assert(s.first.Study(f.realm,1,f.trialEvidenceA)==LaterResult::EvidenceFound);assert(s.first.Operate(f.realm,f.mechanism)==LaterResult::Completed);
}
static void Echo(Chapters& s,int i,int answer){
 First(s,i);const auto& a=RealmAftermathSpecs[i+3];const auto& e=EchoQuests[i];
 assert(s.aftermath.Begin(e.realm,true,true,a.initiatingWitness)==RealmAftermathResult::Started);assert(s.aftermath.Prepare(e.realm,a.mechanismSite)==RealmAftermathResult::Prepared);assert(s.aftermath.Inspect(e.realm,answer,answer==1?a.evidenceForCare:a.evidenceForTruth)==RealmAftermathResult::EvidenceFound);assert(s.aftermath.Testify(e.realm,answer==1?a.careWitness:a.truthWitness)==RealmAftermathResult::Witnessed);assert(s.aftermath.Decide(e.realm,answer)==RealmAftermathResult::Resolved);
 assert(s.echo.Begin(e.realm,true,true,e.openingWitness)==EchoResult::Started);assert(s.echo.Inspect(e.realm,e.evidenceA)==EchoResult::Discovered);assert(s.echo.Inspect(e.realm,e.evidenceB)==EchoResult::Discovered);assert(s.echo.Testify(e.realm,answer==1?e.witnessCare:e.witnessTruth)==EchoResult::Testified);assert(s.echo.Commit(e.realm,answer,answer==1?e.ritualCare:e.ritualTruth)==EchoResult::Committed);
}
template<class Capture,class Commit>
static void Retry(int scope,int fault,SaveFixture::Storage& store,Capture capture,Commit commit){
 const auto before=capture();assert(store.Load());Identical(before,*store.Load());
 store.failAt=store.writes+fault;store.damageOnFailure=true;
 DefeatSaveRetry retry;int attempts=0;
 auto tryCommit=[&]{++attempts;return commit();};
 assert(retry.Attempt(scope,100,tryCommit)==DefeatSaveResult::FirstFailure);assert(attempts==1);
 Identical(before,capture());assert(store.Load());Identical(before,*store.Load());
 const int writes=store.writes;
 for(int tick=0;tick<50;++tick)assert(retry.Attempt(scope,100+tick/10.,tryCommit)==DefeatSaveResult::Waiting);
 assert(attempts==1&&store.writes==writes);
 assert(retry.Attempt(scope,105,tryCommit)==DefeatSaveResult::Saved);assert(attempts==2);
 const auto after=capture();assert(store.Load());Identical(after,*store.Load());
 for(int repeat=0;repeat<3;++repeat){Chapters loaded;loaded.Load(*store.Load());Same(after,loaded.Capture());}
 const int savedWrites=store.writes;
 for(int tick=0;tick<=600;++tick)assert(retry.Attempt(scope,105+tick/10.,tryCommit)==DefeatSaveResult::Waiting);
 assert(attempts==2&&store.writes==savedWrites);Identical(after,*store.Load());
}
int main(){
 int guardians=0,phases=0;
 for(int i=0;i<6;++i)for(int answer=1;answer<=2;++answer)for(int fault=1;fault<=2;++fault){
  Chapters live;Echo(live,i,answer);SaveFixture::Storage store;assert(store.Commit(live.Capture()));
  Retry(i,fault,store,[&]{return live.Capture();},[&]{
   const auto before=live.guardians.Snapshot();const auto result=live.guardians.Resolve(EchoQuests[i].realm,live.echo.Ending(EchoQuests[i].realm)!=0,false);assert(result==GuardianResolution::Defeated);
   if(store.Commit(live.Capture()))return true;
   assert(live.guardians.Restore(before));return false;
  });
  assert(live.guardians.Resolve(EchoQuests[i].realm,true,true)==GuardianResolution::AlreadyResolved);
  assert(live.guardians.Outcome(EchoQuests[i].realm)==GuardianOutcome::Defeated);++guardians;
 }
 for(int seed=0;seed<6;++seed)for(auto morning:{Morning::Anchor,Morning::Many})for(int fault=1;fault<=2;++fault){
  Chapters live;First(live,5);FinalJourney final;SaveFixture::Storage store;
  auto capture=[&]{auto r=live.Capture();const auto f=final.Snapshot();r.bHasFinalEncounterSnapshot=true;r.FinalEncounterStage=f.act;r.FinalMorningChoice=f.morning;r.FinalMemorySeed=f.memorySeed;return r;};
  assert(store.Commit(capture()));
  for(int phase:{0,1,4}){
   if(phase==4){assert(final.ChooseWorld(morning)==FinalResult::NewMorning);assert(store.Commit(capture()));assert(final.WakeEcho()==FinalResult::EchoStarted);assert(store.Commit(capture()));}
   Retry(phase,fault,store,capture,[&]{
    const auto before=final.Snapshot();const auto result=phase==0?final.BreakMask(live.first.IsComplete(Realm::FirstAbsence),seed):phase==1?final.BreakCore():final.SettleEcho();
    assert(result==(phase==0?FinalResult::MaskBroken:phase==1?FinalResult::TrueVictory:FinalResult::EchoDefeated));
    if(store.Commit(capture()))return true;
    assert(final.Restore(before));return false;
   });++phases;
  }
  assert(final.MemorySeed()==seed&&final.World()==morning&&final.Act()==FinalAct::EchoSettled);
  assert(final.SettleEcho()==FinalResult::AlreadyDone);assert(final.BreakCore()==FinalResult::WrongStage);
 }
 // Confirmation is consumed by a failed commit. Retrying a permanent choice
 // requires a new preview and matching confirmation, not replay of old consent.
 for(const char* scope:{"Guardian.0","Echo.0","TheLastAnswer"}){
  CommitmentGate gate;int commits=0;
  auto press=[&](int choice,double time){if(gate.Attempt(scope,choice,time)==CommitmentAttempt::Confirmed){++commits;return false;}return true;};
  assert(press(1,10));assert(press(1,17));assert(commits==0); // expiry gives a fresh preview
  assert(press(2,18));assert(commits==0); // changing control gives a fresh preview
  gate.Cancel();assert(press(2,19));assert(commits==0);
  assert(!press(2,20));assert(commits==1); // simulated failed write after confirmation
  assert(press(2,21));assert(commits==1);assert(!press(2,22));assert(commits==2);
  CommitmentGate reloaded;assert(reloaded.Attempt(scope,2,22)==CommitmentAttempt::Preview);
 }
 std::cout<<"PASS: "<<guardians<<" guardian and "<<phases<<" final-phase failed-write/retry paths; no tick writes or repeated progression; consumed/expired/cancelled consent cannot replay\n";
}
