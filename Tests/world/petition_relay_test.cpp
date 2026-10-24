#include "World/UnmadePetitionRelayRules.h"
#include <cassert>
#include <string>
using namespace UnmadeCore;
int main() {
 BesHouseholdSnapshot home;
 home.phase=static_cast<int>(BesPhase::Documented);
 home.firstReportDay=2;home.homeVisited=true;home.wardenInterviewed=true;
 HearthPetition petition;
 assert(petition.ReadBook(2,home)==PetitionResult::Recorded);
 assert(petition.WitnessLedger(2,home)==PetitionResult::Recorded);
 assert(petition.Decide(1,2)==PetitionResult::Recorded);
 PetitionRelay relay;
 assert(relay.Collect(2,petition)==RelayEvent::NotReady); // Bes has not received it
 assert(petition.ReturnWitness(3)==PetitionResult::Finished);
 assert(relay.Collect(3,petition)==RelayEvent::Collected);
 assert(relay.Collect(3,petition)==RelayEvent::AlreadyDone);
 assert(relay.GiveToMera(2)==RelayEvent::InvalidClock);
 assert(relay.GiveToOrrel(3)==RelayEvent::NotReady);
 assert(relay.GiveToMera(3)==RelayEvent::Delivered);
 assert(relay.Stage()==2 && relay.Route()==1);
 assert(relay.TakeMeraReply(3)==RelayEvent::NotReady); // actual later morning
 assert(relay.TakeMeraReply(4)==RelayEvent::ReplyCollected);
 assert(relay.GiveToOrrel(3)==RelayEvent::InvalidClock);
 assert(relay.GiveToOrrel(4)==RelayEvent::Finished);
 assert(relay.GiveToOrrel(5)==RelayEvent::AlreadyDone);
 assert(relay.Stage()==4);
 auto saved=relay.Save();
 PetitionRelay loaded;
 assert(loaded.Restore(saved,petition,4));
 assert(loaded.Stage()==4&&loaded.Route()==1);
 assert(!loaded.Restore({4,2,3,3,4,4},petition,4)); // route cannot rewrite
 assert(!loaded.Restore({4,1,3,3,3,4},petition,4)); // no same-day response
 assert(!loaded.Restore({4,1,3,3,4,5},petition,4)); // impossible future delivery
 assert(!loaded.Restore({1,1,3,0,0,0},petition,4)); // active result not downgraded
 assert(loaded.Stage()==4&&loaded.Route()==1);
 PetitionRelay rolledBack;
 assert(CommitPetitionRelay(rolledBack,[&]{return rolledBack.Collect(3,petition);},[]{return false;})==RelayCommit::SaveFailed);
 assert(rolledBack.Stage()==0);
 assert(CommitPetitionRelay(rolledBack,[&]{return rolledBack.Collect(3,petition);},[]{return true;})==RelayCommit::Committed);
 assert(rolledBack.Stage()==1);
 // Public route retains its distinct risk without inventing repayment.
 HearthPetition publicPetition;
 assert(publicPetition.ReadBook(2,home)==PetitionResult::Recorded);
 assert(publicPetition.WitnessLedger(2,home)==PetitionResult::Recorded);
 assert(publicPetition.Decide(2,2)==PetitionResult::Recorded);
 assert(publicPetition.ReturnWitness(3)==PetitionResult::Finished);
 PetitionRelay publicRelay;
 assert(publicRelay.Collect(3,publicPetition)==RelayEvent::Collected);
 assert(publicRelay.Route()==2);
 assert(publicRelay.GiveToMera(4)==RelayEvent::Delivered);
 assert(publicRelay.TakeMeraReply(5)==RelayEvent::ReplyCollected);
 assert(publicRelay.GiveToOrrel(5)==RelayEvent::Finished);
 assert(std::string(publicRelay.MeraLine()).find("public")!=std::string::npos);
 assert(std::string(relay.MeraLine()).find("private")!=std::string::npos);
 assert(std::string(publicRelay.OrrelLine()).find("redacted")!=std::string::npos);
 assert(std::string(relay.OrrelLine()).find("sealed")!=std::string::npos);
 assert(std::string(PetitionRelay{}.MeraLine()).empty()); // no remote omniscience
 assert(std::string(PetitionRelay{}.OrrelLine()).empty());
}
