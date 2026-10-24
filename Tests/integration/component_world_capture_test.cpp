#include "realm_chapter_fixture.h"
#include "world_capture_fixture.h"
#include "Save/UnmadeCaptureRules.h"
#include "NPC/UnmadeMemorySaveRules.h"
#include "Items/UnmadeItemRules.h"
#include "Lexicon/UnmadeLexiconRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
using SaveFixture::Record;
static Record Component(Record record,int writer){
 if(writer==0){
  InventoryModel items;assert(items.Claim(Achievement::Starter)==RewardResult::Awarded);
  const auto s=items.Snapshot();record.bHasInventorySnapshot=true;
  record.ItemQuantities.assign(s.quantities.begin(),s.quantities.end());
  record.EquippedItems.assign(s.equipped.begin(),s.equipped.end());record.AwardedMilestoneBits=s.milestones;
  // Equipment's explicit staged strain override must not be overwritten by world capture.
  record.bHasFractureSnapshot=true;record.WorldVariantId="variant.open";record.PlayerStrain=17;
 }else if(writer==1){
  LexiconModel words;assert(words.RecordEvidence("term.veyl","evidence.glimpse"));
  record.LexiconEvidence.assign(words.Snapshot().begin(),words.Snapshot().end());
 }else{
  TenfoldChronicle rites;auto s=rites.Snapshot();s.stage[0]=1;assert(rites.Restore(s));
  record.bHasTenfoldChronicle=true;record.RiteStages.assign(s.stage.begin(),s.stage.end());
  record.RiteChoices.assign(s.choice.begin(),s.choice.end());record.RiteReadyAt.assign(s.readyAt.begin(),s.readyAt.end());
  record.DisciplineMastery.assign(s.disciplineMastery.begin(),s.disciplineMastery.end());
 }
 return record;
}
static void SameComponent(const Record& a,const Record& b){
 assert(a.ItemQuantities==b.ItemQuantities&&a.EquippedItems==b.EquippedItems&&a.AwardedMilestoneBits==b.AwardedMilestoneBits);
 assert(a.LexiconEvidence==b.LexiconEvidence&&a.RiteStages==b.RiteStages&&a.RiteChoices==b.RiteChoices&&a.DisciplineMastery==b.DisciplineMastery);
 assert(a.WorldVariantId==b.WorldVariantId&&a.PlayerStrain==b.PlayerStrain);
}
int main(){
 struct NoncopyableRecord:Record {NoncopyableRecord()=default;NoncopyableRecord(const NoncopyableRecord&)=delete;};
 NoncopyableRecord nativeShape;
 assert(PrepareCombinedCapture(nativeShape,SaveFixture::Valid,[](auto& record){record.bHasPlayerResume=true;record.ResumeHealth=75;return true;}));
 int paths=0,reloads=0;
 for(int realm=0;realm<9;++realm)for(int ending=1;ending<=2;++ending)
 for(int recovery=0;recovery<2;++recovery)
 for(int fault=1;fault<=2;++fault)for(int writer=0;writer<3;++writer){
  ++paths;RealmFixture::Chapters live;
  auto before=live.Capture();before.LivingWorldSeconds=0;
  SaveFixture::Npc absent;absent.NpcId="npc.saved.absent";
  absent.Observations.push_back({{1},"Player.Helped","npc.saved.absent","",0});
  before.NpcSnapshots.push_back(absent);
  const auto r=static_cast<Realm>(realm);const auto& a=RealmAftermathSpecs[realm];
  if(realm>=3){
   const auto& f=LaterRealms[realm-3];
   assert(live.first.Visit(r)==LaterResult::Visited);
   assert(live.first.Begin(r,f.initiator)==LaterResult::Started);
   assert(live.first.Study(r,ending,ending==1?f.trialEvidenceA:f.trialEvidenceB)==LaterResult::EvidenceFound);
   assert(live.first.Operate(r,f.mechanism)==LaterResult::Completed);
  }
  // Earlier first-chapter sections remain optional in this legacy-shaped fixture.
  assert(live.aftermath.Begin(r,true,true,a.initiatingWitness)==RealmAftermathResult::Started);
  assert(live.aftermath.Prepare(r,a.mechanismSite)==RealmAftermathResult::Prepared);
  assert(live.aftermath.Inspect(r,ending,ending==1?a.evidenceForCare:a.evidenceForTruth)==RealmAftermathResult::EvidenceFound);
  assert(live.aftermath.Testify(r,ending==1?a.careWitness:a.truthWitness)==RealmAftermathResult::Witnessed);
  assert(live.aftermath.Decide(r,ending)==RealmAftermathResult::Resolved);
  const char* person=ResidentContinuityId(realm);
  assert(live.residents.Talk(person,1)==EncounterEvent::FirstHello);
  assert(live.residents.Talk(person,2)==EncounterEvent::ReturnHello);
  auto world=live.Capture();world.LivingWorldSeconds=1230+realm;
  world.NpcSnapshots=before.NpcSnapshots;
  SaveFixture::Npc current;current.NpcId=person;
  current.Observations.push_back({{2},"Player.Helped",person,"",0});
  if(!ReplaceNpcMemory(world.NpcSnapshots,current))world.NpcSnapshots.push_back(current);
  assert(SaveFixture::Valid(before)&&SaveFixture::Valid(world));
  SaveFixture::Storage store;store.primary=before;store.backup=before;store.recovery=recovery;
  auto staged=Component(*store.Load(),writer);const auto own=staged;
  int captures=0;
  auto prepare=[&](Record& candidate,bool rejectedOwner=false,bool malformedCapture=false){
   return PrepareCombinedCapture(candidate,SaveFixture::Valid,[&](Record& target){
    ++captures;if(rejectedOwner)return false;
    CaptureWorldFields(target,world);target.bHasPlayerResume=true;target.ResumeHealth=75;
    if(malformedCapture)target.ResidentEncounterVisits.back()=6;
    return true;
   });
  };
  // A malformed caller-owned subrecord may not be repaired by a live capture.
  auto malformed=staged;malformed.NpcSnapshots[0].Observations[0].EventId.value=0;
  assert(!prepare(malformed));assert(captures==0&&store.writes==0);
  auto refused=staged;assert(!prepare(refused,true));assert(store.writes==0);
  auto late=staged;assert(!prepare(late,false,true));assert(store.writes==0);
  assert(prepare(staged));SameComponent(own,staged);RealmFixture::Same(world,staged);
  store.failAt=recovery?1:fault;store.damageOnFailure=true;
  assert(!store.Commit(staged));assert(store.Load());RealmFixture::Same(before,*store.Load());
  assert(store.Load()->LivingWorldSeconds==0);
  store.failAt=0;staged=Component(*store.Load(),writer);
  assert(prepare(staged)&&store.Commit(staged));
  for(int reload=0;reload<5;++reload){
   ++reloads;const auto* saved=store.Load();assert(saved&&SaveFixture::Valid(*saved));
   assert(saved->LivingWorldSeconds==world.LivingWorldSeconds&&saved->ResumeHealth==75);
   SameComponent(own,*saved);RealmFixture::Same(world,*saved);
   assert(saved->NpcSnapshots.size()==2&&saved->NpcSnapshots[0].Observations[0].EventId.value==1);
   RealmFixture::Chapters restored;restored.Load(*saved);RealmFixture::Same(world,restored.Capture());
   assert(restored.aftermath.Decide(r,3-ending)==RealmAftermathResult::NoChange);
   assert(restored.residents.Talk(person,2)==EncounterEvent::SameDay);
  }
 }
 int surveys=0;
 for(int index=0;index<6;++index)for(int stage=0;stage<2;++stage)for(int recovery=0;recovery<2;++recovery)
 for(int fault=1;fault<=2;++fault){
  ++surveys;SaveFixture::Array<int> stages(6,0);stages[index]=stage;
  Record initial;initial.bHasFieldSurveys=true;initial.FieldSurveyStages=stages;
  SaveFixture::Storage store;store.primary=initial;store.backup=initial;store.recovery=recovery;
  auto write=[&]{auto next=*store.Load();return PrepareCombinedCapture(next,SaveFixture::Valid,[&](Record& capture){capture.FieldSurveyStages=stages;return true;})&&store.Commit(next);};
  store.failAt=recovery?1:fault;store.damageOnFailure=true;
  assert(!CommitFieldSurveyStage(stages,index,write));assert(stages[index]==stage);
  assert(store.Load()->FieldSurveyStages[index]==stage);
  store.failAt=0;assert(CommitFieldSurveyStage(stages,index,write));assert(stages[index]==stage+1);
  assert(store.Load()->FieldSurveyStages[index]==stage+1);
  if(stage==1){const int writes=store.writes;assert(!CommitFieldSurveyStage(stages,index,write));assert(store.writes==writes);}
 }
 SaveFixture::Array<int> malformed(6,0);malformed[5]=3;
 assert(!CommitFieldSurveyStage(malformed,0,[]{assert(false);return true;}));
 SaveFixture::Array<int> complete(6,2),shortRecord(5,0);
 assert(!CommitFieldSurveyStage(complete,0,[]{assert(false);return true;}));
 assert(!CommitFieldSurveyStage(shortRecord,0,[]{assert(false);return true;}));
 for(int invalid:{-1,6})assert(!CommitFieldSurveyStage(complete,invalid,[]{assert(false);return true;}));
 std::cout<<"PASS: "<<paths<<" realm/component recovery paths; "<<reloads<<" reloads; "<<surveys<<" survey fault paths; caller fields, absent histories and authored endings retained\n";
}
