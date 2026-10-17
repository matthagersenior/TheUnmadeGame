#include "Save/UnmadeRelationshipCommitRules.h"
#include "../save/combined_record_store.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
struct LocalRelationship {
 Expeditions expeditions;AdventureDepth depth;PlayerExperience experience;
 int day=2;
 void Prepare(int a){
  auto& e=expeditions;assert(e.Brief(a));assert(e.Visit(a,0));
  for(int room=0;room<5;++room){assert(e.Enter(a,room));if(room<4)assert(e.Kill(a,room));if(room>0&&room<4)assert(e.Inspect(a,room-1));}
  for(int k=4;k<8;++k)assert(e.Kill(a,k));
  assert(e.Rescue(a,0));assert(e.Rescue(a,1));assert(e.Choose(a,1));assert(e.Enter(a,5));assert(e.Boss(a,true));assert(e.Return(a));
  assert(depth.Offer(a,true));assert(depth.Witness(a));assert(depth.Evidence(a,true));assert(depth.Choose(a,2,true));
  assert(experience.OfferWing(a,e));for(int k=0;k<3;++k)assert(experience.WingStep(a,k,true));
  assert(experience.Guardian(a));assert(experience.CompleteWing(a));
  for(int p=0;p<5;++p)assert(experience.Visit(a,p,1));
  assert(experience.Visit(a,0,2));assert(experience.Visit(a,3,2));
 }
 SaveFixture::Record Capture()const{
  SaveFixture::Record r;r.bHasLivingWorldSnapshot=true;r.LivingWorldSeconds=(day-1)*1200.;
  r.bHasExpeditions=true;r.ExpeditionState=expeditions.Pack();r.bHasAdventureDepth=true;r.AdventureDepthState=depth.Pack();
  r.bHasPlayerExperience=true;r.PlayerExperienceState=experience.Pack();assert(SaveFixture::Valid(r));return r;
 }
};
int main(){
 int paths=0,reloads=0;
 const int expectedScenes[]={2,3,-1,1,0};
 for(int a=0;a<9;++a)for(int person=0;person<5;++person)for(int fail=0;fail<=2;++fail){
  LocalRelationship state;state.Prepare(a);const auto before=state.experience.Pack();SaveFixture::Storage disk;disk.primary=state.Capture();disk.backup=disk.primary;
  disk.failAt=fail;disk.damageOnFailure=true;int attempts=0;
  const auto result=CommitResidentConversation(state.experience,a,person,state.day,state.expeditions,state.depth,true,[&]{++attempts;return disk.Commit(state.Capture());});
  assert(attempts==1);assert(result.state==(fail?ConversationState::SaveFailed:ConversationState::Saved));
  if(fail){assert(result.scene==-1);assert(state.experience.Pack()==before);}
  else {assert(result.scene==expectedScenes[person]);assert(state.experience.Tutorial()&1);assert(state.experience.Arc(a)->scenes==(result.scene>=0?1<<result.scene:0));}
  for(int n=0;n<12;++n){const auto* saved=disk.Load();assert(saved);PlayerExperience restored;
   assert(restored.Restore(saved->PlayerExperienceState,state.expeditions,state.depth,state.day));assert(restored.Pack()==state.experience.Pack());++reloads;}
  disk.failAt=0;const auto retry=CommitResidentConversation(state.experience,a,person,state.day,state.expeditions,state.depth,true,[&]{++attempts;return disk.Commit(state.Capture());});
  assert(retry.state==(fail?ConversationState::Saved:ConversationState::Unchanged));
  assert(state.experience.Arc(a)->scenes==(expectedScenes[person]>=0?1<<expectedScenes[person]:0));
  const auto after=state.experience.Pack();int duplicateWrites=0;
  assert(CommitResidentConversation(state.experience,a,person,state.day,state.expeditions,state.depth,true,[&]{++duplicateWrites;return true;}).state==ConversationState::Unchanged);
  assert(duplicateWrites==0&&state.experience.Pack()==after);++paths;
 }
 // Declining a guide scene still records actual contact, but no scene or insight.
 LocalRelationship guide;guide.Prepare(0);const int insight=guide.experience.Arc(0)->insight;int writes=0;
 auto denied=CommitResidentConversation(guide.experience,0,4,2,guide.expeditions,guide.depth,false,[&]{++writes;return true;});
 assert(denied.state==ConversationState::Saved&&denied.scene==-1&&writes==1);assert(guide.experience.Arc(0)->scenes==0&&guide.experience.Arc(0)->insight==insight);
 assert(CommitResidentConversation(guide.experience,0,4,2,guide.expeditions,guide.depth,true,[&]{++writes;return true;}).scene==0);
 assert(guide.experience.Arc(0)->insight==insight+1);
 // A previously counted same-day conversation can gain a newly eligible scene.
 LocalRelationship newlyEligible;assert(newlyEligible.expeditions.Brief(1));assert(newlyEligible.expeditions.Visit(1,0));assert(newlyEligible.expeditions.Enter(1,0));
 assert(newlyEligible.experience.OfferWing(1,newlyEligible.expeditions));
 for(int k=0;k<3;++k)assert(newlyEligible.experience.WingStep(1,k,true));
 assert(newlyEligible.experience.Visit(1,3,1));
 int newWrites=0;
 auto newWriter=[&]{++newWrites;return SaveFixture::Valid(newlyEligible.Capture());};
 assert(CommitResidentConversation(newlyEligible.experience,1,3,2,newlyEligible.expeditions,newlyEligible.depth,true,newWriter).scene==-1);
 assert(newlyEligible.experience.Guardian(1));
 assert(CommitResidentConversation(newlyEligible.experience,1,3,2,newlyEligible.expeditions,newlyEligible.depth,true,newWriter).scene==1);
 assert(newWrites==2&&newlyEligible.experience.Arc(1)->visits[3]==2&&newlyEligible.experience.Arc(1)->insight==3);
 // Speaking to the second captive must not consume the first captive's scene.
 LocalRelationship captives;captives.Prepare(4);
 assert(CommitResidentConversation(captives.experience,4,2,2,captives.expeditions,captives.depth,true,[]{return true;}).scene==-1);
 assert(!(captives.experience.Arc(4)->scenes&8));
 assert(CommitResidentConversation(captives.experience,4,1,2,captives.expeditions,captives.depth,true,[]{return true;}).scene==3);
 // Refused guidance still rolls back contact/lesson when its writer fails.
 for(int fail=1;fail<=2;++fail){LocalRelationship refused;refused.Prepare(0);const auto before=refused.experience.Pack();
  SaveFixture::Storage disk;disk.primary=refused.Capture();disk.backup=disk.primary;disk.failAt=fail;disk.damageOnFailure=true;
  auto result=CommitResidentConversation(refused.experience,0,4,2,refused.expeditions,refused.depth,false,[&]{return disk.Commit(refused.Capture());});
  assert(result.state==ConversationState::SaveFailed&&result.scene==-1&&refused.experience.Pack()==before);
  assert(disk.Load()->PlayerExperienceState==before);
 }
 // Invalid external inputs cannot partially stage a visit or tutorial.
 const auto old=guide.experience.Pack();int invalidWrites=0;
 for(auto input:std::array<std::array<int,3>,4>{{{{-1,0,2}},{{0,5,2}},{{0,0,0}},{{0,0,1000001}}}}){
  assert(CommitResidentConversation(guide.experience,input[0],input[1],input[2],guide.expeditions,guide.depth,true,[&]{++invalidWrites;return true;}).state==ConversationState::Unavailable);
  assert(guide.experience.Pack()==old);
 }
 assert(invalidWrites==0);
 std::cout<<"relationship: "<<paths<<" scoped conversation/fault paths, "<<reloads<<" reloads\n";
}
