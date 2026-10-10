#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "World/UnmadeCampaignSpineRules.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnmadeUnansweredRoadStory,
    "Unmade.Story.UnansweredRoad",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnmadeUnansweredRoadStory::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    RoadEvidence E{};
    auto G=EvaluateUnansweredRoad(E);
    TestTrue(TEXT("opening mystery already has an objective"),
        G.act==RoadAct::Opening && G.available>0);
    E.openingWitnessed=true;
    G=EvaluateUnansweredRoad(E);
    TestEqual(TEXT("two plausible frontier leads"),G.available,2);
    E.firstStories|=RoadMask(Realm::HearthBeneath);
    G=EvaluateUnansweredRoad(E);
    TestTrue(TEXT("frontier evidence reveals cost"),
        G.act==RoadAct::CostOfAbsence);
    E.firstStories|=RoadMask(Realm::CinderSpine);
    G=EvaluateUnansweredRoad(E);
    TestTrue(TEXT("the quarry route leads to the untitled court"),
        G.act==RoadAct::RightToExist && G.alternatives[0]==Realm::OrchardOfKings);
    E.firstStories|=RoadMask(Realm::OrchardOfKings);
    E.firstStories|=RoadMask(Realm::FirstAbsence);
    G=EvaluateUnansweredRoad(E);
    TestTrue(TEXT("first Auvren resolved; optional sidequests not gated"),
        G.bossAccessible && G.act==RoadAct::ConfrontTheAnswer);
    TestFalse(TEXT("unattuned route cannot be invented"),
        !RoadRoute(Realm::ThreefoldReach,Realm::FirstAbsence,0).empty());
    E.finalAct=3;
    TestTrue(TEXT("same saved quest progress survives new morning"),
        EvaluateUnansweredRoad(E).act==RoadAct::LivingAfterward);
    return true;
}
#endif
