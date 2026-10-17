#pragma once
#include "realm_chapter_fixture.h"
#include "Save/UnmadeRiteCommitRules.h"
#include "World/UnmadeConfluenceRules.h"
namespace RiteFixture {
using namespace UnmadeCore;
using SaveFixture::Record;
struct State {
 RealmFixture::Chapters chapters;TenfoldChronicle rites;ConfluenceJourney trials;
 FractureModel fracture{"region.prototype.hub",{"variant.open","variant.sealed"}};
 RealityConsequenceLedger civic;OuterRealmReactions outer;double now=1200;
 Record Capture()const{
  auto r=chapters.Capture();r.LivingWorldSeconds=now;
  r.bHasFractureSnapshot=true;const auto f=fracture.TakeSnapshot();r.WorldVariantId=f.variant;r.PlayerStrain=f.strain;
  const auto t=rites.Snapshot();r.bHasTenfoldChronicle=true;
  r.RiteStages.assign(t.stage.begin(),t.stage.end());r.RiteChoices.assign(t.choice.begin(),t.choice.end());r.RiteReadyAt.assign(t.readyAt.begin(),t.readyAt.end());
  r.DisciplineMastery.assign(t.disciplineMastery.begin(),t.disciplineMastery.end());r.RiteTrialBits=t.trialUsed;r.LegacyDeedBits=t.deeds;r.VerifiedRoadBits=t.verifiedRoutes;
  r.TomorrowDebtDueDay=t.debtDueDay;r.TomorrowDebtExhaustedUntil=t.exhaustedUntilDay;r.BrokenOathCount=t.brokenOaths;r.bOathActive=t.oathActive;r.bOathRedeemed=t.oathRedeemed;
  r.bHasConfluenceSnapshot=true;const auto c=trials.Snapshot();r.ConfluenceStages.assign(c.stage.begin(),c.stage.end());r.ConfluenceDecisions.assign(c.decisions.begin(),c.decisions.end());r.ConfluenceCastMasks.assign(c.casts.begin(),c.casts.end());r.ConfluenceWindowEnds.assign(c.windowEnds.begin(),c.windowEnds.end());
  r.bHasRealityConsequenceSnapshot=true;const auto local=civic.Snapshot();r.RealityCareWitnessMask=local.careWitnessMask;r.RealityInquiryWitnessMask=local.inquiryWitnessMask;r.RealityLastRite=local.activeRite;r.RealityActiveUntil=local.activeUntil;
  r.bHasOuterRealmReactions=true;for(const auto& o:outer.Save()){r.OuterCareMasks.push_back(o.careWitnessMask);r.OuterInquiryMasks.push_back(o.inquiryWitnessMask);r.OuterActiveRites.push_back(o.activeRite);r.OuterActiveUntil.push_back(o.activeUntil);}
  assert(SaveFixture::Valid(r));return r;
 }
 void Load(const Record& r){
  chapters.Load(r);now=r.LivingWorldSeconds;assert(fracture.Restore({r.WorldVariantId,r.PlayerStrain}));
  TenfoldSnapshot t;for(int i=0;i<10;++i){t.stage[i]=r.RiteStages[i];t.choice[i]=r.RiteChoices[i];t.readyAt[i]=r.RiteReadyAt[i];}for(int i=0;i<6;++i)t.disciplineMastery[i]=r.DisciplineMastery[i];
  t.trialUsed=r.RiteTrialBits;t.deeds=r.LegacyDeedBits;t.verifiedRoutes=r.VerifiedRoadBits;t.debtDueDay=r.TomorrowDebtDueDay;t.exhaustedUntilDay=r.TomorrowDebtExhaustedUntil;t.brokenOaths=r.BrokenOathCount;t.oathActive=r.bOathActive;t.oathRedeemed=r.bOathRedeemed;assert(rites.Restore(t));
  ConfluenceSnapshot c;for(int i=0;i<6;++i){c.stage[i]=r.ConfluenceStages[i];c.decisions[i]=r.ConfluenceDecisions[i];c.casts[i]=r.ConfluenceCastMasks[i];c.windowEnds[i]=r.ConfluenceWindowEnds[i];}assert(trials.Restore(c));
  assert(civic.Restore({static_cast<std::uint16_t>(r.RealityCareWitnessMask),static_cast<std::uint16_t>(r.RealityInquiryWitnessMask),r.RealityLastRite,r.RealityActiveUntil},now));
  OuterRealmReactions::Snapshot o;for(int i=0;i<8;++i)o[i]={static_cast<std::uint16_t>(r.OuterCareMasks[i]),static_cast<std::uint16_t>(r.OuterInquiryMasks[i]),r.OuterActiveRites[i],r.OuterActiveUntil[i]};assert(outer.Restore(o,now));
 }
};
inline void MasterAll(State& s){auto t=s.rites.Snapshot();t.stage.fill(5);t.choice.fill(1);t.trialUsed=1023;assert(s.rites.Restore(t));}
inline void CompleteEcho(State& s,int i){
 const auto& f=LaterRealms[i];const auto& a=RealmAftermathSpecs[i+3];const auto& e=EchoQuests[i];const auto realm=f.realm;
 assert(s.chapters.first.Visit(realm)==LaterResult::Visited);assert(s.chapters.first.Begin(realm,f.initiator)==LaterResult::Started);assert(s.chapters.first.Study(realm,1,f.trialEvidenceA)==LaterResult::EvidenceFound);assert(s.chapters.first.Operate(realm,f.mechanism)==LaterResult::Completed);
 assert(s.chapters.aftermath.Begin(realm,true,true,a.initiatingWitness)==RealmAftermathResult::Started);assert(s.chapters.aftermath.Prepare(realm,a.mechanismSite)==RealmAftermathResult::Prepared);assert(s.chapters.aftermath.Inspect(realm,1,a.evidenceForCare)==RealmAftermathResult::EvidenceFound);assert(s.chapters.aftermath.Testify(realm,a.careWitness)==RealmAftermathResult::Witnessed);assert(s.chapters.aftermath.Decide(realm,1)==RealmAftermathResult::Resolved);
 assert(s.chapters.echo.Begin(realm,true,true,e.openingWitness)==EchoResult::Started);assert(s.chapters.echo.Inspect(realm,e.evidenceA)==EchoResult::Discovered);assert(s.chapters.echo.Inspect(realm,e.evidenceB)==EchoResult::Discovered);assert(s.chapters.echo.Testify(realm,e.witnessCare)==EchoResult::Testified);assert(s.chapters.echo.Commit(realm,1,e.ritualCare)==EchoResult::Committed);
}
inline void Same(const Record& a,const Record& b){
 RealmFixture::Same(a,b);
 assert(a.PlayerStrain==b.PlayerStrain&&a.WorldVariantId==b.WorldVariantId&&a.LivingWorldSeconds==b.LivingWorldSeconds);
 assert(a.RiteStages==b.RiteStages&&a.RiteChoices==b.RiteChoices&&a.RiteReadyAt==b.RiteReadyAt&&a.RiteTrialBits==b.RiteTrialBits&&a.DisciplineMastery==b.DisciplineMastery);
 assert(a.TomorrowDebtDueDay==b.TomorrowDebtDueDay&&a.TomorrowDebtExhaustedUntil==b.TomorrowDebtExhaustedUntil&&a.bOathActive==b.bOathActive);
 assert(a.ConfluenceStages==b.ConfluenceStages&&a.ConfluenceCastMasks==b.ConfluenceCastMasks&&a.ConfluenceWindowEnds==b.ConfluenceWindowEnds);
 assert(a.RealityCareWitnessMask==b.RealityCareWitnessMask&&a.RealityInquiryWitnessMask==b.RealityInquiryWitnessMask&&a.RealityLastRite==b.RealityLastRite&&a.RealityActiveUntil==b.RealityActiveUntil);
 assert(a.OuterCareMasks==b.OuterCareMasks&&a.OuterInquiryMasks==b.OuterInquiryMasks&&a.OuterActiveRites==b.OuterActiveRites&&a.OuterActiveUntil==b.OuterActiveUntil);
}
// Same ownership split as InvokeRite: the component rolls back its models; the
// actor-used helper owns civic/outer/resonance rollback around the single writer.
template<class Writer> bool Cast(State& s,Realm realm,RiteId id,bool witnessed,bool focus,bool atPlinth,Writer write){
 const auto beforeRite=s.rites;const auto beforeTrials=s.trials;
 RiteContext c;c.site=static_cast<int>(id);c.day=2;c.now=s.now;c.strain=s.fracture.CurrentStrain();c.verifiedWitnesses=3;
 const auto power=s.rites.Invoke(id,c);if(power.result!=RiteResult::Applied)return false;
 assert(s.fracture.SpendStrain(power.cost));
 if(s.rites.Stage(id)==2)assert(s.rites.Advance(id,RiteAction::Trial,c)==RiteResult::Advanced);
 if(s.trials.Stage(ConfluenceId::SilentAlarm)==1)s.trials.RecordCast(ConfluenceId::SilentAlarm,id,s.now,0);
 if(CommitRiteWorld(s.civic,s.outer,s.chapters.resonance,[&]{
  const bool mastered=s.rites.IsMastered(id);
  if(focus){if(realm==Realm::ThreefoldReach)assert(s.civic.Cast(id,s.now,power.duration,mastered,witnessed,true));else assert(s.outer.Cast(realm,id,s.now,power.duration,mastered,witnessed));}
  if(mastered&&atPlinth)s.chapters.resonance.Record(realm,id,s.now,s.chapters.echo.Ending(realm)!=0,true);
 },write))return true;
 s.rites=beforeRite;s.trials=beforeTrials;assert(s.fracture.Recover(power.cost/FractureModel::RecoveryPerSecond));return false;
}
}
