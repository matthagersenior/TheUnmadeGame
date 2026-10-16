#pragma once
#include "../save/combined_record_store.h"
#include <cassert>
namespace RealmFixture {
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
}
