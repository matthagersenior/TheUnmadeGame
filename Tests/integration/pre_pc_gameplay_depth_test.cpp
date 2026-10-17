#include "Combat/UnmadeCreatureTacticsRules.h"
#include "World/UnmadeJourneyGuidanceRules.h"
#include "World/UnmadeConfluenceGuidanceRules.h"
#include "World/UnmadeBesHouseholdRules.h"
#include <cassert>
#include <set>
#include <string>
int main() {
 using namespace UnmadeCore;
 std::set<int> signatures;
 for(int i=0;i<12;++i){
   const auto kind=static_cast<CreatureKind>(i);
   const auto tactic=CreatureTactic(kind);
   assert(tactic.name && tactic.name[0]);
   assert(tactic.cue && tactic.cue[0]);
   signatures.insert(tactic.signature);
   const auto profile=CreatureStats(kind);
   assert(TacticalCreatureIntent(kind,profile,100,true,false,2.0,1.0)==EnemyIntent::Approach);
   assert(TacticalCreatureIntent(kind,profile,1,true,true,2.0,1.0)==EnemyIntent::Hold);
 }
 assert(signatures.size()==12);
 assert(TacticalCreatureIntent(CreatureKind::PaleStrider,CreatureStats(CreatureKind::PaleStrider),2.1,true,false,2.0,1.0)!=
        TacticalCreatureIntent(CreatureKind::Inkmaw,CreatureStats(CreatureKind::Inkmaw),2.1,true,false,2.0,1.0));
 ExpeditionState expedition;
 DepthArc personal;
 auto opening=NextOpeningBeat(expedition,personal,false);
 assert(opening.stage==0 && std::string(opening.objective).size()>15);
 expedition.brief=1; expedition.visited=1;
 assert(NextOpeningBeat(expedition,personal,false).stage>=1);
 expedition.rooms=63;expedition.branch=2;expedition.clues=7;expedition.boss=2;
 expedition.returned=1;expedition.reward=1;
 assert(NextOpeningBeat(expedition,personal,true).stage>4);
 for(int i=0;i<9;++i){
   const auto route=NextInvestigationLead(i,expedition,personal);
   assert(route && route[0]);
 }
 personal.offered=1;personal.witness=1;personal.evidence=1;
 assert(std::string(NextInvestigationLead(1,expedition,personal))!=std::string(NextInvestigationLead(7,expedition,personal)));
 assert(std::string(DescribeConfluenceNeed(0,1,3,0,125.0,2,true,120.0)).find("expired")!=std::string::npos);
 auto missing=DescribeConfluenceNeed(0,0,0,0,0.0,0,false);
 assert(missing && missing[0]);
 assert(std::string(DescribeConfluenceNeed(0,1,1,0,10.0,2,true))!=
        std::string(DescribeConfluenceNeed(0,1,3,0,10.0,2,true)));
 BesHouseholdSnapshot home;
 assert(CommunityPressure(home,3)==HouseholdPressure::Unreported);
 home.phase=static_cast<int>(BesPhase::Walkout);home.firstReportDay=1;
 home.wardenInterviewed=true;home.homeVisited=true;home.daysWalkedOut=2;
 assert(CommunityPressure(home,5)==HouseholdPressure::Stoppage);
 home.phase=static_cast<int>(BesPhase::Settled);home.guaranteeOwed=0;
 assert(CommunityPressure(home,5)==HouseholdPressure::Restitution);
}
