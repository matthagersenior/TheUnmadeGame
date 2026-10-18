#pragma once
// Hand-carried cross-realm consequences of a finished Hearth Petition.
// No automatic courier, borrowed testimony, debt mutation, or new rewards.
#include "World/UnmadeHearthPetitionRules.h"
namespace UnmadeCore {
enum class RelayEvent { NotReady, Collected, Delivered, ReplyCollected, Finished, AlreadyDone, InvalidClock };
enum class RelayCommit { Unavailable, Committed, SaveFailed };
struct PetitionRelaySnapshot {
 int stage=0; // 0 absent; 1 carrying copy; 2 Mera received; 3 carrying reply; 4 Orrel received
 int route=0; // original private(1) or redacted public(2) petition
 int collectionDay=0,meraDay=0,replyDay=0,orrelDay=0;
};
class PetitionRelay {
 PetitionRelaySnapshot state_{};
public:
 PetitionRelaySnapshot Save()const noexcept{return state_;}
 int Stage()const noexcept{return state_.stage;}
 int Route()const noexcept{return state_.route;}
 bool Restore(PetitionRelaySnapshot s,const HearthPetition& petition,int day)noexcept{
  if(day<1||day>1000000||s.stage<0||s.stage>4||s.route<0||s.route>2||
     s.collectionDay<0||s.meraDay<0||s.replyDay<0||s.orrelDay<0||
     s.collectionDay>day||s.meraDay>day||s.replyDay>day||s.orrelDay>day)return false;
  const auto source=petition.Save();
  if(s.stage==0){
   if(s.route||s.collectionDay||s.meraDay||s.replyDay||s.orrelDay)return false;
  }else {
   if(petition.Stage()!=4||s.route!=petition.Ending()||
      s.collectionDay<source.returnDay||s.collectionDay==0)return false;
   if(s.stage<2){
    if(s.meraDay||s.replyDay||s.orrelDay)return false;
   } else {
    if(s.meraDay<s.collectionDay||s.meraDay==0)return false;
    if(s.stage<3){
     if(s.replyDay||s.orrelDay)return false;
    } else {
     if(s.replyDay<=s.meraDay)return false;
     if(s.stage==3 ? s.orrelDay!=0 : s.orrelDay<s.replyDay)return false;
    }
   }
  }
  if(state_.stage>s.stage)return false;
  if(state_.route && state_.route!=s.route)return false;
  if(state_.stage==4 && (s.stage!=4||s.orrelDay!=state_.orrelDay))return false;
  state_=s;return true;
 }
 RelayEvent Collect(int day,const HearthPetition& petition)noexcept{
  if(day<1||day>1000000)return RelayEvent::InvalidClock;
  if(state_.stage)return RelayEvent::AlreadyDone;
  if(petition.Stage()!=4||petition.Ending()==0||
     day<petition.Save().returnDay)return RelayEvent::NotReady;
  state_={1,petition.Ending(),day,0,0,0};
  return RelayEvent::Collected;
 }
 RelayEvent GiveToMera(int day)noexcept{
  if(day<1||day>1000000|| (state_.collectionDay&&day<state_.collectionDay))
   return RelayEvent::InvalidClock;
  if(state_.stage>=2)return RelayEvent::AlreadyDone;
  if(state_.stage!=1)return RelayEvent::NotReady;
  state_.stage=2;state_.meraDay=day;return RelayEvent::Delivered;
 }
 RelayEvent TakeMeraReply(int day)noexcept{
  if(day<1||day>1000000|| (state_.meraDay&&day<state_.meraDay))
   return RelayEvent::InvalidClock;
  if(state_.stage>=3)return RelayEvent::AlreadyDone;
  if(state_.stage!=2||day<=state_.meraDay)return RelayEvent::NotReady;
  state_.stage=3;state_.replyDay=day;return RelayEvent::ReplyCollected;
 }
 RelayEvent GiveToOrrel(int day)noexcept{
  if(day<1||day>1000000|| (state_.replyDay&&day<state_.replyDay))
   return RelayEvent::InvalidClock;
  if(state_.stage==4)return RelayEvent::AlreadyDone;
  if(state_.stage!=3)return RelayEvent::NotReady;
  state_.stage=4;state_.orrelDay=day;return RelayEvent::Finished;
 }
 const char* Journal()const noexcept{
  switch(state_.stage){
   case 0:return "The Hearth Petition has not traveled. Complete Bes's personal acknowledgement in Cinderhold, then take the physical copy from beside his return station.";
   case 1:return "You carry the actual Cinderhold petition. Travel to Saltwake and hand it to Mera in person. No one there knows its contents yet.";
   case 2:return "Mera has read the petition in Saltwake. Speak to her again after the next in-world morning for her considered, signed reply.";
   case 3:return "You carry Mera's reply. Bring it to Orrel the bridge keeper at the Crossings and speak to him in person.";
   case 4:return state_.route==1
     ?"Orrel has received the confidential reply. A sealed crossing archive now acknowledges the wage question but cannot pay it."
     :"Orrel has received Mera's redacted public reply. A civic crossing docket now requests an accounting without naming guests.";
  }
  return "";
 }
 const char* MeraLine()const noexcept{
  if(state_.stage<2)return "";
  if(state_.route==1)return state_.stage==2
    ?"Mera: I received the private Cinderhold petition from your hands. Privacy shields workers, but how will Rheva prevent a concealed ledger from being ignored? Return tomorrow."
    :"Mera: This sealed reply questions the private process. I have not volunteered another guarantee and your journey has not repaid any wages.";
  return state_.stage==2
    ?"Mera: You carried a public redacted petition. I support naming the unpaid amount, not the refuge guests; public pressure may still endanger the household. Return tomorrow."
    :"Mera: The reply is redacted. I disagree with any public accusation without an actual hearing. No guest names and no free loan.";
 }
 const char* OrrelLine()const noexcept{
  if(state_.stage!=4)return "";
  return state_.route==1
   ?"Orrel: This sealed Saltwake reply is in my hands. I will preserve the private passage; later Cinderhold payroll must be verified separately."
   :"Orrel: I have the redacted public reply. The Crossings may request an accounting, not convict anyone or expose the people who took shelter.";
 }
};
template<class Mutate,class Save>
RelayCommit CommitPetitionRelay(PetitionRelay& relay,Mutate action,Save save){
 const auto prior=relay;
 const auto result=action();
 if(result!=RelayEvent::Collected&&result!=RelayEvent::Delivered&&
    result!=RelayEvent::ReplyCollected&&result!=RelayEvent::Finished)
  return RelayCommit::Unavailable;
 if(!save()){relay=prior;return RelayCommit::SaveFailed;}
 return RelayCommit::Committed;
}
} // namespace UnmadeCore
