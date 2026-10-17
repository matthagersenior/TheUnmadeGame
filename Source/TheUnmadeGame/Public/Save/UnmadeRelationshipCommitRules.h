#pragma once
#include "World/UnmadePlayerExperienceRules.h"
namespace UnmadeCore {
enum class ConversationState { Unavailable, Unchanged, Saved, SaveFailed };
struct ConversationCommit {ConversationState state=ConversationState::Unavailable;int scene=-1;};
inline int SceneForResident(int person){
 constexpr int scenes[]={2,3,-1,1,0};return person>=0&&person<5?scenes[person]:-1;
}
// Actual contact, its first-step lesson and this speaker's eligible scene are
// one reversible receipt. A failed scene never dispatches another speaker.
template<class Writer>
ConversationCommit CommitResidentConversation(PlayerExperience& experience,int realm,int person,int day,
 const Expeditions& expeditions,const AdventureDepth& depth,bool acceptsScene,Writer write){
 if(!experience.Arc(realm)||person<0||person>4||day<1||day>1000000)return {};
 const auto before=experience;
 bool changed=experience.Visit(realm,person,day);
 changed=experience.Learn(0)||changed;
 int scene=SceneForResident(person);
 if(acceptsScene&&scene>=0&&experience.Scene(realm,scene,expeditions,depth))changed=true;
 else scene=-1;
 if(!changed)return {ConversationState::Unchanged,-1};
 if(!write()){experience=before;return {ConversationState::SaveFailed,-1};}
 return {ConversationState::Saved,scene};
}
}
