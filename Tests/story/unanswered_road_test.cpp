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
static void Golden(const RoadEvidence& evidence,RoadAct act,const char* title,
                   const char* question,const char* objective,
                   std::array<Realm,3> alternatives,int available,bool boss) {
    const auto actual=EvaluateUnansweredRoad(evidence);
    assert(actual.act==act);
    assert(std::strcmp(actual.title,title)==0);
    assert(std::strcmp(actual.question,question)==0);
    assert(std::strcmp(actual.objective,objective)==0);
    if(actual.alternatives!=alternatives) {
        std::cerr<<"journal alternative snapshot mismatch: "<<title<<" actual="
                 <<static_cast<int>(actual.alternatives[0])<<","<<
                   static_cast<int>(actual.alternatives[1])<<","<<
                   static_cast<int>(actual.alternatives[2])<<" expected="
                 <<static_cast<int>(alternatives[0])<<","<<
                   static_cast<int>(alternatives[1])<<","<<
                   static_cast<int>(alternatives[2])<<"\n";
        assert(actual.alternatives==alternatives);
    }
    assert(actual.available==available);
    assert(actual.bossAccessible==boss);
}
static void TestJournalGoldens() {
    using R=Realm;
    const auto none=std::array<R,3>{{R::Count,R::Count,R::Count}};
    Golden(With(false,{}),RoadAct::Opening,"A Bell That Had No Last Note",
        "Why does the Crossings bell ring from a road that no one has walked?",
        "Listen to Bellwold or Paperhaven, investigate two landmarks, or take a stand in the Crossings.",
        {{R::ThreefoldReach,R::Count,R::Count}},1,false);
    Golden(With(true,{}),RoadAct::MissingWorld,"What the World Withheld",
        "Why does the bell's missing note answer from both rain and flame?",
        "Resolve one FIRST local chapter: Saltwake's lost sea OR Cinderhold's borrowed ember.",
        {{R::WidowedRain,R::HearthBeneath,R::Count}},2,false);
    Golden(With(true,{R::WidowedRain}),RoadAct::CostOfAbsence,"What the Absences Cost",
        "Who pays whenever the world forgets something that was real?",
        "Resolve the FIRST chapter in Drevlach, Orravane or Vathless; their evidence speaks to the same missing burden.",
        {{R::TidalLedger,R::SkyBelow,R::Count}},2,false);
    Golden(With(true,{R::HearthBeneath}),RoadAct::CostOfAbsence,"What the Absences Cost",
        "Who pays whenever the world forgets something that was real?",
        "Resolve the FIRST chapter in Drevlach, Orravane or Vathless; their evidence speaks to the same missing burden.",
        {{R::SkyBelow,R::CinderSpine,R::Count}},2,false);
    Golden(With(true,{R::HearthBeneath,R::CinderSpine}),RoadAct::RightToExist,"Who Deserves To Be Real",
        "Are the erased inhabitants the cost of keeping every city standing?",
        "Resolve Eillun's or Tharniv's FIRST chapter; learn what a world may ask of its people.",
        {{R::OrchardOfKings,R::Count,R::Count}},1,false);
    Golden(With(true,{R::WidowedRain,R::TidalLedger}),RoadAct::RightToExist,"Who Deserves To Be Real",
        "Are the erased inhabitants the cost of keeping every city standing?",
        "Resolve Eillun's or Tharniv's FIRST chapter; learn what a world may ask of its people.",
        {{R::HundredUnlived,R::Count,R::Count}},1,false);
    Golden(With(true,{R::WidowedRain,R::TidalLedger,R::SkyBelow}),RoadAct::RightToExist,"Who Deserves To Be Real",
        "Are the erased inhabitants the cost of keeping every city standing?",
        "Resolve Eillun's or Tharniv's FIRST chapter; learn what a world may ask of its people.",
        {{R::HundredUnlived,R::OrchardOfKings,R::Count}},2,false);
    Golden(With(true,{R::WidowedRain,R::TidalLedger,R::HundredUnlived}),RoadAct::BeforePlace,"The Witness Before Place",
        "Who created an answer to protect a world from contradictory truths?",
        "Find Auvren via the linked upper realms and resolve its FIRST trial. Echo and guardian quests remain optional.",
        {{R::FirstAbsence,R::Count,R::Count}},1,false);
    const auto beforeBoss=With(true,{R::HearthBeneath,R::CinderSpine,
                                     R::OrchardOfKings,R::FirstAbsence});
    Golden(beforeBoss,RoadAct::ConfrontTheAnswer,"The Answer That Ate Its Question",
        "Did Nhal-Vey save the world, or silence everybody who disagreed?",
        "Enter the northern Auvren arena and face the first form. Its defeat may not be the ending.",
        {{R::FirstAbsence,R::Count,R::Count}},1,true);
    Golden(With(false,{},1),RoadAct::ConfrontTheAnswer,"Nhal-Vey, The Answer That Ate Its Question",
        "Why did its first death leave a second enemy behind?",
        "Survive the unveiled form, then choose a new morning deliberately.",none,0,true);
    Golden(With(false,{},3),RoadAct::LivingAfterward,"The World After Its Answer",
        "A victory is not a disappearance; how will the surviving people live now?",
        "Return to familiar people and public records. The Auvren echo is optional.",none,0,true);
}
static void TestSchemaOneJournalRestoration() {
    // Schema-one saves predate this journal. Reconstruct it from the old
    // witnessed-opening, realm-completion and final-journey authorities.
    const auto oldCrossings=RestoreRoadEvidence(true,0,0);
    assert(oldCrossings.openingWitnessed&&oldCrossings.firstStories==0);
    assert(EvaluateUnansweredRoad(oldCrossings).act==RoadAct::MissingWorld);

    const auto oldRealmProgress=RestoreRoadEvidence(false,
        RoadMask(Realm::WidowedRain)|RoadMask(Realm::TidalLedger),0);
    assert(oldRealmProgress.openingWitnessed);
    assert(EvaluateUnansweredRoad(oldRealmProgress).act==RoadAct::RightToExist);
    assert(RoadTravelAttunement(oldRealmProgress,0)==3);

    const auto oldFinalSave=RestoreRoadEvidence(false,0,1);
    assert(oldFinalSave.openingWitnessed&&oldFinalSave.finalAct==1);
    assert(EvaluateUnansweredRoad(oldFinalSave).bossAccessible);
    assert(EvaluateUnansweredRoad(oldFinalSave).act==RoadAct::ConfrontTheAnswer);

    const auto oldEnding=RestoreRoadEvidence(false,0,3);
    assert(EvaluateUnansweredRoad(oldEnding).act==RoadAct::LivingAfterward);
    assert(EvaluateUnansweredRoad(oldEnding).bossAccessible);

    // A corrupt future bit cannot masquerade as a canonical realm receipt.
    constexpr auto futureBit=static_cast<std::uint16_t>(1u<<15);
    const auto bounded=RestoreRoadEvidence(false,futureBit,0);
    assert(!bounded.openingWitnessed&&bounded.firstStories==0);
    assert(EvaluateUnansweredRoad(bounded).act==RoadAct::Opening);
}
int main(){
    TestJournalGoldens();
    TestSchemaOneJournalRestoration();
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
           RoadAct::Opening); // pure rules reject fabricated opening evidence
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
    // Main-story crossing attunement must be earnable WITHOUT any optional
    // relic, landmark checklist, forge or side-quest milestone.
    // Fresh InventoryModel has no items, confirming zero relic attunement.
    InventoryModel noRelics;
    assert(AttunementFromRewards(noRelics)==0);
    assert(RoadTravelAttunement(With(false,{}),0)==0);
    assert(RoadTravelAttunement(With(true,{}),0)==1);
    assert(RoadTravelAttunement(With(true,{Realm::WidowedRain}),0)==2);
    assert(RoadTravelAttunement(With(true,{Realm::WidowedRain,Realm::TidalLedger}),0)==3);
    const auto debtRoute=With(true,{Realm::WidowedRain,Realm::TidalLedger,
                                    Realm::HundredUnlived});
    assert(RoadTravelAttunement(debtRoute,0)==4);
    assert(EvaluateUnansweredRoad(debtRoute).act==RoadAct::BeforePlace);
    assert(!RoadRoute(Realm::ThreefoldReach,Realm::FirstAbsence,
                      RoadTravelAttunement(debtRoute,0)).empty());
    const auto choirRoute=With(true,{Realm::HearthBeneath,Realm::SkyBelow,
                                     Realm::HundredUnlived});
    assert(RoadTravelAttunement(choirRoute,0)==4);
    const auto stoneRoute=With(true,{Realm::HearthBeneath,Realm::CinderSpine,
                                     Realm::OrchardOfKings});
    assert(RoadTravelAttunement(stoneRoute,0)==4);
    assert(!RoadRoute(Realm::HearthBeneath,Realm::FirstAbsence,
                      RoadTravelAttunement(stoneRoute,0)).empty());
    // No spoofed late-realm completion or final access without preceding acts.
    assert(RoadTravelAttunement(With(false,{Realm::HundredUnlived}),0)==0);
    assert(RoadTravelAttunement(With(true,{Realm::HundredUnlived}),0)==1);
    assert(RoadTravelAttunement(With(true,{Realm::WidowedRain,Realm::HundredUnlived}),0)==2);
    assert(RoadTravelAttunement(With(true,{Realm::WidowedRain,Realm::TidalLedger}),0)<4);
    assert(!EvaluateUnansweredRoad(debtRoute).bossAccessible);
    // Legacy relic progression still assists exploration but never fakes story.
    assert(RoadTravelAttunement(With(false,{}),4)==4);
    assert(!EvaluateUnansweredRoad(With(false,{})).bossAccessible);
    // Every realm can be explored and completed without mandatory Echo or
    // guardian status; no new grind currency enters the proof.
    auto whole=With(true,{Realm::WidowedRain,Realm::HearthBeneath,Realm::TidalLedger,
        Realm::SkyBelow,Realm::CinderSpine,Realm::HundredUnlived,
        Realm::OrchardOfKings,Realm::FirstAbsence});
    assert(EvaluateUnansweredRoad(whole).bossAccessible);
    std::cout<<"PASS: coherent five-act narrative, nine authored hooks, both playable routes, no side-quest grind, post-game continuity\n";
}
