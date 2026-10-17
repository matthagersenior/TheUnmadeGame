#include "World/UnmadeHearthPetitionRules.h"
#include <cassert>
#include <string>
#include <vector>
using namespace UnmadeCore;
int main() {
 BesHouseholdSnapshot household;
 HearthPetition petition;
 assert(petition.ReadBook(2,household)==PetitionResult::Blocked);
 household.phase=static_cast<int>(BesPhase::WagesReported);
 household.firstReportDay=2;
 household.homeVisited=true;
 assert(petition.ReadBook(2,household)==PetitionResult::Recorded);
 assert(petition.ReadBook(2,household)==PetitionResult::AlreadyDone);
 assert(petition.WitnessLedger(2,household)==PetitionResult::Blocked);
 household.wardenInterviewed=true;
 household.phase=static_cast<int>(BesPhase::Documented);
 assert(petition.WitnessLedger(2,household)==PetitionResult::Recorded);
 assert(petition.Decide(3,2)==PetitionResult::Blocked);
 assert(petition.Decide(1,2)==PetitionResult::Recorded);
 assert(petition.Decide(2,2)==PetitionResult::AlreadyDone);
 assert(petition.ReturnWitness(2)==PetitionResult::Blocked);
 assert(petition.ReturnWitness(3)==PetitionResult::Finished);
 assert(petition.ReturnWitness(4)==PetitionResult::AlreadyDone);
 assert(petition.Ending()==1);
 auto record=petition.Save();
 assert(record.stage==4 && record.ending==1);
 HearthPetition loaded;
 assert(loaded.Restore(record,household,4));
 assert(loaded.Save().ending==1);
 assert(!loaded.Restore({4,2,2,2,2,3},household,4)); // no ending rewrite
 assert(loaded.Save().ending==1);
 assert(!loaded.Restore({4,1,2,2,2,2},household,4)); // return next day required
 assert(!loaded.Restore({4,1,2,2,2,6},household,4)); // time travel
 auto privateEnd=petition.Save();
 HearthPetition publicPetition;
 assert(publicPetition.ReadBook(2,household)==PetitionResult::Recorded);
 assert(publicPetition.WitnessLedger(3,household)==PetitionResult::Recorded);
 assert(publicPetition.Decide(2,3)==PetitionResult::Recorded);
 assert(publicPetition.ReturnWitness(4)==PetitionResult::Finished);
 assert(publicPetition.Ending()==2);
 assert(privateEnd.ending != publicPetition.Save().ending);
 HearthPetition rollback;
 assert(CommitHearthPetition(rollback,
   [&]{return rollback.ReadBook(2,household);},
   [&]{return false;})==PetitionCommitResult::SaveFailed);
 assert(rollback.Stage()==0);
 assert(CommitHearthPetition(rollback,
   [&]{return rollback.ReadBook(2,household);},
   [&]{return true;})==PetitionCommitResult::Committed);
 assert(rollback.Stage()==1);
 assert(CommitHearthPetition(rollback,
   [&]{return rollback.ReadBook(3,household);},
   [&]{return true;})==PetitionCommitResult::Unavailable);
 assert(rollback.Stage()==1);
 // A decision is testimony, never a payment or invented inventory item.
 assert(std::string(HearthPetitionLine(1)).find("confidential")!=std::string::npos);
 assert(std::string(HearthPetitionLine(2)).find("redacted")!=std::string::npos);
}
