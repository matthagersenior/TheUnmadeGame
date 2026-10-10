#include "World/UnmadeCampaignSpineRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
using namespace UnmadeCore;
static RoadEvidence With(bool opened,std::initializer_list<Realm> realms,int finalStage=0) {
    RoadEvidence out{};out.openingWitnessed=opened;out.finalAct=finalStage;
    for(const auto r:realms)out.firstStories|=RoadMask(r);
    return out;
}
int main(){
    static_assert(UnansweredRoadBeats.size()==9,"nine distinct first encounters");
    std::set<std::string> tags;
    for(int i=0;i<9;++i){
        const auto& beat=UnansweredRoadBeats[i];
        assert(static_cast<int>(beat.realm)==i);
        assert(std::strlen(beat.localQuestion)>45);
        assert(std::strlen(beat.witnessedDiscovery)>105);
        assert(std::strlen(beat.nextDirection)>45);
        assert(tags.insert(beat.publicTag).second);
        assert(RoadMask(beat.realm)==(1u<<i));
    }
    assert(RoadMask(Realm::Count)==0);
    auto start=EvaluateUnansweredRoad(With(false,{}));
    assert(start.act==RoadAct::Opening&&start.available==1&&!start.bossAccessible);
    assert(EvaluateUnansweredRoad(With(true,{})).act==RoadAct::MissingWorld);
    auto frontier=EvaluateUnansweredRoad(With(true,{Realm::WidowedRain}));
    assert(frontier.act==RoadAct::CostOfAbsence&&frontier.available==2);
    assert(frontier.alternatives[0]==Realm::TidalLedger);
    assert(frontier.alternatives[1]==Realm::SkyBelow);
    frontier=EvaluateUnansweredRoad(With(true,{Realm::HearthBeneath}));
    assert(frontier.alternatives[0]==Realm::SkyBelow);
    assert(frontier.alternatives[1]==Realm::CinderSpine);
    auto mid=EvaluateUnansweredRoad(With(true,{Realm::HearthBeneath,Realm::CinderSpine}));
    assert(mid.act==RoadAct::RightToExist&&mid.available==1);
    assert(mid.alternatives[0]==Realm::OrchardOfKings);
    mid=EvaluateUnansweredRoad(With(true,{Realm::WidowedRain,Realm::TidalLedger}));
    assert(mid.act==RoadAct::RightToExist&&mid.available==1);
    assert(mid.alternatives[0]==Realm::HundredUnlived);
    auto all=With(true,{Realm::HearthBeneath,Realm::CinderSpine,
                         Realm::OrchardOfKings,Realm::FirstAbsence});
    assert(EvaluateUnansweredRoad(all).bossAccessible);
    assert(EvaluateUnansweredRoad(all).act==RoadAct::ConfrontTheAnswer);
    // Auvren alone is NO LONGER a substitute for learning why the
    // world was erasing people. Extra Echo/guardian jobs are never required.
    assert(!EvaluateUnansweredRoad(With(true,{Realm::FirstAbsence})).bossAccessible);
    assert(EvaluateUnansweredRoad(With(true,{Realm::WidowedRain,Realm::TidalLedger,
        Realm::HundredUnlived})).act==RoadAct::BeforePlace);
    // Nine first arcs might be completed out of sequence in a migrated save:
    // the story derives the highest coherent act; no new SaveGame fields.
    const auto alternate=With(true,{Realm::WidowedRain,Realm::SkyBelow,
                                     Realm::HundredUnlived,Realm::FirstAbsence});
    assert(EvaluateUnansweredRoad(alternate).bossAccessible);
    assert(EvaluateUnansweredRoad(With(false,{Realm::HearthBeneath})).act==
           RoadAct::MissingWorld); // normalized real opening still needed
    assert(EvaluateUnansweredRoad(With(false,{},1)).act==RoadAct::ConfrontTheAnswer);
    assert(EvaluateUnansweredRoad(With(false,{},2)).bossAccessible);
    assert(EvaluateUnansweredRoad(With(false,{},3)).act==RoadAct::LivingAfterward);
    assert(EvaluateUnansweredRoad(With(false,{},5)).act==RoadAct::LivingAfterward);
    // Atlas is the only authority on attunement and physical connections.
    assert(RoadRoute(Realm::ThreefoldReach,Realm::HearthBeneath,0).size()==2);
    assert(RoadRoute(Realm::ThreefoldReach,Realm::TidalLedger,0).empty());
    assert(RoadRoute(Realm::ThreefoldReach,Realm::TidalLedger,1).size()==3);
    assert(RoadRoute(Realm::ThreefoldReach,Realm::FirstAbsence,3).empty());
    assert(!RoadRoute(Realm::ThreefoldReach,Realm::FirstAbsence,4).empty());
    // Every realm can be explored and completed without mandatory Echo or
    // guardian status; no new grind currency enters the proof.
    auto whole=With(true,{Realm::WidowedRain,Realm::HearthBeneath,Realm::TidalLedger,
        Realm::SkyBelow,Realm::CinderSpine,Realm::HundredUnlived,
        Realm::OrchardOfKings,Realm::FirstAbsence});
    assert(EvaluateUnansweredRoad(whole).bossAccessible);
    std::cout<<"PASS: coherent five-act narrative, nine authored hooks, both playable routes, no side-quest grind, post-game continuity\n";
}
