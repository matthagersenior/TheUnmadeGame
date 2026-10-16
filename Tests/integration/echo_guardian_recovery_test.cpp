// Production models + combined-record store. Actor targeting, serialization and
// presentation still require Unreal; faults here exercise portable record rules.
#include "../save/combined_record_store.h"
#include "Story/UnmadeCommitmentRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
using SaveFixture::Record;
struct Chapters {
 LaterRealmJourney first;RealmAftermathChronicle aftermath;EchoChronicle echo;
 GuardianChronicle guardians;ResidentContinuity residents;RealmResonanceJourney resonance;
 Record Capture()const{
  Record r;r.bHasLivingWorldSnapshot=true;r.LivingWorldSeconds=1200;
  const auto f=first.Snapshot();r.bHasLaterRealmSnapshot=true;r.LaterVisitedMask=f.visits;
  r.LaterRealmStages.assign(f.stage.begin(),f.stage.end());r.LaterRealmChoices.assign(f.choice.begin(),f.choice.end());
  const auto a=aftermath.Snapshot();r.bHasRealmAftermathSnapshot=true;
  r.RealmAftermathStages.assign(a.stage.begin(),a.stage.end());r.RealmAftermathPrepared.assign(a.prepared.begin(),a.prepared.end());r.RealmAftermathApproaches.assign(a.approach.begin(),a.approach.end());r.RealmAftermathEndings.assign(a.ending.begin(),a.ending.end());
  const auto e=echo.Snapshot();r.bHasEchoQuestSnapshot=true;r.EchoQuestStages.assign(e.stage.begin(),e.stage.end());r.EchoQuestEvidence.assign(e.evidence.begin(),e.evidence.end());r.EchoQuestTestimonies.assign(e.testimony.begin(),e.testimony.end());r.EchoQuestEndings.assign(e.ending.begin(),e.ending.end());
  r.bHasGuardianSnapshot=true;const auto g=guardians.Snapshot();r.RealmGuardianOutcomes.assign(g.outcomes.begin(),g.outcomes.end());
  r.bHasResidentContinuitySnapshot=true;const auto n=residents.Snapshot();r.ResidentEncounterVisits.assign(n.visits.begin(),n.visits.end());r.ResidentEncounterLastDays.assign(n.lastDay.begin(),n.lastDay.end());r.ResidentEncounterAidFlags.assign(n.aids.begin(),n.aids.end());
  r.bHasRealmResonanceSnapshot=true;const auto v=resonance.Snapshot();r.RealmResonanceStages.assign(v.stage.begin(),v.stage.end());r.RealmResonanceFirstCasts.assign(v.firstCast.begin(),v.firstCast.end());r.RealmResonanceDeadlines.assign(v.deadline.begin(),v.deadline.end());
  return r;
 }
 void Load(const Record& r){
  assert(SaveFixture::Valid(r));LaterRealmSnapshot f;f.visits=r.LaterVisitedMask;
  RealmAftermathSnapshot a;EchoSnapshot e;GuardianSnapshot g;ResidentContinuitySnapshot n;ResonanceSnapshot v;
  for(int i=0;i<9;++i){a.stage[i]=r.RealmAftermathStages[i];a.prepared[i]=r.RealmAftermathPrepared[i];a.approach[i]=r.RealmAftermathApproaches[i];a.ending[i]=r.RealmAftermathEndings[i];}
  for(int i=0;i<6;++i){f.stage[i]=r.LaterRealmStages[i];f.choice[i]=r.LaterRealmChoices[i];e.stage[i]=r.EchoQuestStages[i];e.evidence[i]=r.EchoQuestEvidence[i];e.testimony[i]=r.EchoQuestTestimonies[i];e.ending[i]=r.EchoQuestEndings[i];g.outcomes[i]=r.RealmGuardianOutcomes[i];v.stage[i]=r.RealmResonanceStages[i];v.firstCast[i]=r.RealmResonanceFirstCasts[i];v.deadline[i]=r.RealmResonanceDeadlines[i];}
  for(int i=0;i<ResidentContinuityCount;++i){n.visits[i]=r.ResidentEncounterVisits[i];n.lastDay[i]=r.ResidentEncounterLastDays[i];n.aids[i]=r.ResidentEncounterAidFlags[i];}
  assert(first.Restore(f)&&aftermath.Restore(a)&&echo.Restore(e)&&guardians.Restore(g)&&residents.Restore(n)&&resonance.Restore(v));
 }
};
static void Same(const Record& a,const Record& b){
 assert(a.LaterRealmStages==b.LaterRealmStages&&a.LaterRealmChoices==b.LaterRealmChoices&&a.LaterVisitedMask==b.LaterVisitedMask);
 assert(a.RealmAftermathStages==b.RealmAftermathStages&&a.RealmAftermathPrepared==b.RealmAftermathPrepared&&a.RealmAftermathApproaches==b.RealmAftermathApproaches&&a.RealmAftermathEndings==b.RealmAftermathEndings);
 assert(a.EchoQuestStages==b.EchoQuestStages&&a.EchoQuestEvidence==b.EchoQuestEvidence&&a.EchoQuestTestimonies==b.EchoQuestTestimonies&&a.EchoQuestEndings==b.EchoQuestEndings&&a.RealmGuardianOutcomes==b.RealmGuardianOutcomes);
 assert(a.ResidentEncounterVisits==b.ResidentEncounterVisits&&a.ResidentEncounterLastDays==b.ResidentEncounterLastDays&&a.ResidentEncounterAidFlags==b.ResidentEncounterAidFlags);
 assert(a.RealmResonanceStages==b.RealmResonanceStages&&a.RealmResonanceFirstCasts==b.RealmResonanceFirstCasts&&a.RealmResonanceDeadlines==b.RealmResonanceDeadlines);
}
int main(){
 // Internally well-formed subrecords must also obey present chapter prerequisites.
 auto forged=Chapters{}.Capture();forged.RealmGuardianOutcomes[0]=1;
 assert(!SaveFixture::Valid(forged));
 // Omitted optional sections retain the existing legacy policy.
 forged.bHasEchoQuestSnapshot=false;assert(SaveFixture::Valid(forged));
 assert(std::strstr(ResidentContinuityLine(Familiarity::Stranger,false,true,true),"begin"));
 assert(std::strstr(ResidentContinuityLine(Familiarity::Recognized,false,true,true),"first meeting"));
 int paths=0,checks=0,faults=0;
 for(int i=0;i<6;++i)for(int initial=1;initial<=2;++initial)for(int followup=1;followup<=2;++followup)for(int answer=1;answer<=2;++answer)for(int force=0;force<2;++force){
  Chapters s;SaveFixture::Storage store;const auto& q=EchoQuests[i];const auto& f=LaterRealms[i];const auto& a=RealmAftermathSpecs[i+3];const auto realm=q.realm;
  const char* neighbor=OuterPeople[i*3].id;
  auto checkpoint=[&]{
   const auto next=s.Capture();assert(SaveFixture::Valid(next));
   if(store.primary)for(int fail=1;fail<=2;++fail){
    auto damaged=store;const auto before=*damaged.Load();damaged.failAt=damaged.writes+fail;damaged.damageOnFailure=true;
    assert(!damaged.Commit(next));assert(damaged.Load());Same(before,*damaged.Load());Chapters recovered;recovered.Load(*damaged.Load());Same(before,recovered.Capture());++faults;
   }
   assert(store.Commit(next));
   for(int repeat=0;repeat<3;++repeat){assert(store.Load());s.Load(*store.Load());Same(next,s.Capture());++checks;}
  };
  checkpoint();assert(s.echo.Begin(realm,false,false,q.openingWitness)==EchoResult::Locked);
  assert(s.residents.Talk(neighbor,1)==EncounterEvent::FirstHello);checkpoint();
  assert(s.first.Visit(realm)==LaterResult::Visited);checkpoint();assert(s.first.Begin(realm,f.initiator)==LaterResult::Started);checkpoint();assert(s.first.Study(realm,initial,initial==1?f.trialEvidenceA:f.trialEvidenceB)==LaterResult::EvidenceFound);checkpoint();assert(s.first.Operate(realm,f.mechanism)==LaterResult::Completed);checkpoint();
  assert(s.aftermath.Begin(realm,s.first.IsComplete(realm),true,a.initiatingWitness)==RealmAftermathResult::Started);checkpoint();assert(s.aftermath.Prepare(realm,a.mechanismSite)==RealmAftermathResult::Prepared);checkpoint();assert(s.aftermath.Inspect(realm,followup,followup==1?a.evidenceForCare:a.evidenceForTruth)==RealmAftermathResult::EvidenceFound);checkpoint();assert(s.aftermath.Testify(realm,followup==1?a.careWitness:a.truthWitness)==RealmAftermathResult::Witnessed);checkpoint();assert(s.aftermath.Decide(realm,followup)==RealmAftermathResult::Resolved);checkpoint();
  assert(s.echo.Begin(realm,s.first.IsComplete(realm),s.aftermath.Stage(realm)==4,q.openingWitness)==EchoResult::Started);checkpoint();assert(s.echo.Inspect(realm,q.evidenceB)==EchoResult::Discovered);checkpoint();assert(s.echo.Testify(realm,answer==1?q.witnessCare:q.witnessTruth)==EchoResult::NeedEvidence);assert(s.echo.Inspect(realm,q.evidenceA)==EchoResult::Discovered);checkpoint();assert(s.echo.Testify(realm,answer==1?q.witnessCare:q.witnessTruth)==EchoResult::Testified);checkpoint();
  assert(s.echo.Commit(realm,3-answer,answer==1?q.ritualTruth:q.ritualCare)==EchoResult::WrongChoice);
  CommitmentGate gate;assert(gate.Attempt(q.title,answer,10)==CommitmentAttempt::Preview);assert(s.echo.Ending(realm)==0);assert(gate.Attempt(q.title,answer,11)==CommitmentAttempt::Confirmed);
  assert(s.echo.Commit(realm,answer,answer==1?q.ritualCare:q.ritualTruth)==EchoResult::Committed);checkpoint();
  assert(s.guardians.Resolve(realm,s.echo.Ending(realm)!=0,!force)==(force?GuardianResolution::Defeated:GuardianResolution::Pacified));checkpoint();
  const auto& v=RealmResonances[i];assert(s.resonance.Record(realm,v.first,1000,true,true)==ResonanceResult::Started);checkpoint();assert(s.resonance.Record(realm,v.second,1010,true,true)==ResonanceResult::Opened);checkpoint();
  assert(s.residents.Talk(neighbor,2)==EncounterEvent::ReturnHello);checkpoint();assert(!s.residents.Aided(neighbor));assert(s.residents.Talk(neighbor,2)==EncounterEvent::SameDay);assert(s.echo.Commit(realm,3-answer,q.ritualCare)==EchoResult::NoChange);assert(s.guardians.Resolve(realm,true,!force)==GuardianResolution::AlreadyResolved);
  const auto good=s.Capture();
  // Every combination of omitted optional chapter sections remains loadable.
  // Stored bytes in an absent section are ignored, as in the existing schema.
  for(int mask=0;mask<16;++mask){auto legacy=good;
   legacy.bHasLaterRealmSnapshot=(mask&1)!=0;legacy.bHasRealmAftermathSnapshot=(mask&2)!=0;
   legacy.bHasEchoQuestSnapshot=(mask&4)!=0;legacy.bHasGuardianSnapshot=(mask&8)!=0;
   assert(SaveFixture::Valid(legacy));
  }
  auto reject=[&](Record bad){assert(!SaveFixture::Valid(bad));const int writes=store.writes;assert(!store.Commit(bad)&&store.writes==writes);Same(good,*store.Load());auto recovery=store;recovery.primary=bad;assert(recovery.Load());Same(*store.backup,*recovery.Load());};
  auto bad=good;bad.EchoQuestStages[i]=0;bad.EchoQuestEvidence[i]=0;bad.EchoQuestTestimonies[i]=0;bad.EchoQuestEndings[i]=0;reject(bad);
  bad=good;bad.LaterRealmStages[i]=0;bad.LaterRealmChoices[i]=0;reject(bad);
  bad=good;bad.RealmAftermathStages[i+3]=0;bad.RealmAftermathPrepared[i+3]=0;bad.RealmAftermathApproaches[i+3]=0;bad.RealmAftermathEndings[i+3]=0;reject(bad);
  bad=good;bad.ResidentEncounterVisits.back()=6;reject(bad);
  ++paths;
 }
 std::cout<<"PASS: "<<paths<<" later-realm chapter/guardian paths; "<<checks<<" full reloads; "<<faults<<" failed-write recoveries; contradictory chapters refused before writes\n";
}
