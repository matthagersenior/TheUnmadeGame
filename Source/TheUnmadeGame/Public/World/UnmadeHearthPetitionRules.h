#pragma once
// The Hearth Petition: optional, genuinely physical Cinderhold investigation.
// Personal evidence is not a wage payment. A confidential record and a
// redacted public claim are mutually exclusive, saved historical outcomes.
#include "World/UnmadeBesHouseholdRules.h"
namespace UnmadeCore {
enum class PetitionResult { Blocked,Recorded,Finished,AlreadyDone };
enum class PetitionCommitResult { Unavailable,Committed,SaveFailed };
struct HearthPetitionSnapshot {
 int stage=0; // 0 unvisited,1 home book,2 civic ledger,3 petition,4 next-day response
 int ending=0; // 1 confidential; 2 redacted public; never reversible
 int bookDay=0,ledgerDay=0,choiceDay=0,returnDay=0;
};
class HearthPetition {
 HearthPetitionSnapshot record_{};
public:
 HearthPetitionSnapshot Save()const noexcept{return record_;}
 int Stage()const noexcept{return record_.stage;}
 int Ending()const noexcept{return record_.ending;}
 bool Restore(HearthPetitionSnapshot s,const BesHouseholdSnapshot& home,int day) noexcept {
  if(day<1||day>1000000 || s.stage<0||s.stage>4 || s.ending<0||s.ending>2 ||
     s.bookDay<0||s.ledgerDay<0||s.choiceDay<0||s.returnDay<0 ||
     s.bookDay>day||s.ledgerDay>day||s.choiceDay>day||s.returnDay>day)return false;
  if(s.stage==0 ? (s.ending||s.bookDay||s.ledgerDay||s.choiceDay||s.returnDay) :
    (!home.homeVisited||home.firstReportDay<1||home.firstReportDay>s.bookDay||
     s.bookDay==0))return false;
  if(s.stage<2 ? (s.ledgerDay||s.choiceDay||s.returnDay||s.ending) :
    (!home.wardenInterviewed||s.ledgerDay<s.bookDay))return false;
  if(s.stage<3 ? (s.choiceDay||s.returnDay||s.ending) :
    (s.ending==0||s.choiceDay<s.ledgerDay))return false;
  if(s.stage==4 ? s.returnDay<=s.choiceDay : s.returnDay!=0)return false;
  // An accepted result is irreversible within the active session.
  if(record_.ending && record_.ending!=s.ending)return false;
  if(record_.stage==4 && (s.stage!=4||s.returnDay!=record_.returnDay))return false;
  record_=s;
  return true;
 }
 PetitionResult ReadBook(int day,const BesHouseholdSnapshot& home) noexcept {
  if(record_.stage!=0)return PetitionResult::AlreadyDone;
  if(day<1||day>1000000||!home.homeVisited||home.firstReportDay<1||
     home.firstReportDay>day)return PetitionResult::Blocked;
  record_.stage=1;record_.bookDay=day;
  return PetitionResult::Recorded;
 }
 PetitionResult WitnessLedger(int day,const BesHouseholdSnapshot& home) noexcept {
  if(record_.stage>1)return PetitionResult::AlreadyDone;
  if(record_.stage!=1||!home.wardenInterviewed||day<record_.bookDay||
     day>1000000)return PetitionResult::Blocked;
  record_.stage=2;record_.ledgerDay=day;
  return PetitionResult::Recorded;
 }
 PetitionResult Decide(int ending,int day) noexcept {
  if(record_.stage>=3)return PetitionResult::AlreadyDone;
  if(record_.stage!=2 || (ending!=1&&ending!=2)||day<record_.ledgerDay||
     day>1000000)return PetitionResult::Blocked;
  record_.stage=3;record_.ending=ending;record_.choiceDay=day;
  return PetitionResult::Recorded;
 }
 PetitionResult ReturnWitness(int day) noexcept {
  if(record_.stage==4)return PetitionResult::AlreadyDone;
  if(record_.stage!=3||day<=record_.choiceDay||day>1000000)
   return PetitionResult::Blocked;
  record_.stage=4;record_.returnDay=day;
  return PetitionResult::Finished;
 }
 const char* Journal() const noexcept {
  switch(record_.stage){
   case 0:return "THE HEARTH PETITION: hear Bes, inspect his real home, then read the missed-wage book beside his pay desk.";
   case 1:return "The home book is read. Ask Rheva to corroborate the claim, then inspect the civic ledger outside Bes's house.";
   case 2:return "The two records agree. At the Cinderhold petition tables choose CONFIDENTIAL protection or a REDACTED PUBLIC hearing; E twice confirms one permanent route. Neither repays wages.";
   case 3:return record_.ending==1
     ? "The confidential petition is carried privately. Return to the shelter witness marker after the NEXT in-world morning."
     : "The redacted public hearing is recorded. Return to its witness marker after the NEXT in-world morning.";
   case 4:return record_.ending==1
     ? "The private petition has been personally acknowledged. Household identities remain protected; the wage debt continues until genuinely paid."
     : "The public redacted petition has been acknowledged. A hearing is requested, not a verdict; the wage debt continues until genuinely paid.";
  }
  return "";
 }
};
inline const char* HearthPetitionLine(int ending) noexcept {
 return ending==1?"The confidential petition shields uninvolved neighbors without forgiving any wages."
   :ending==2?"The redacted public petition records the claim without naming refuge guests."
   :"There is no chosen petition.";
}
template<class Mutate,class Write>
PetitionCommitResult CommitHearthPetition(HearthPetition& p,Mutate action,Write write){
 const auto before=p;
 const auto change=action();
 if(change!=PetitionResult::Recorded&&change!=PetitionResult::Finished)
   return PetitionCommitResult::Unavailable;
 if(!write()){p=before;return PetitionCommitResult::SaveFailed;}
 return PetitionCommitResult::Committed;
}
} // namespace UnmadeCore
