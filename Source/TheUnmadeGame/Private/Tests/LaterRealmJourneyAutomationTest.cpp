#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "World/UnmadeLaterRealmRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnmadeLaterRealmJourneyTest,
    "Unmade.World.SixLaterRealmJourneys",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnmadeLaterRealmJourneyTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    LaterRealmJourney Adventure;
    for(const auto& Spec:LaterRealms)
    {
        TestTrue(TEXT("first arrival persisted"),Adventure.Visit(Spec.realm)==LaterResult::Visited);
        TestTrue(TEXT("first witness opens the investigation"),
            Adventure.Begin(Spec.realm,Spec.initiator)==LaterResult::Started);
        TestTrue(TEXT("operating before evidence refuses"),
            Adventure.Operate(Spec.realm,Spec.mechanism)==LaterResult::NeedEvidence);
        TestTrue(TEXT("care clue inspected"),
            Adventure.Study(Spec.realm,1,Spec.trialEvidenceA)==LaterResult::EvidenceFound);
        TestTrue(TEXT("truth clue may replace evidence before decision"),
            Adventure.Study(Spec.realm,2,Spec.trialEvidenceB)==LaterResult::EvidenceFound);
        TestTrue(TEXT("mechanism commits real route"),
            Adventure.Operate(Spec.realm,Spec.mechanism)==LaterResult::Completed);
        TestEqual(TEXT("outcome independently retained"),Adventure.Outcome(Spec.realm),2);
    }
    LaterRealmJourney Loaded;
    TestTrue(TEXT("all six realm snapshots reload"),Loaded.Restore(Adventure.Snapshot()));
    auto Corrupt=Adventure.Snapshot();
    Corrupt.stage[1]=4;
    TestFalse(TEXT("invalid stage cannot erase progress"),Loaded.Restore(Corrupt));
    TestEqual(TEXT("old progress preserved"),Loaded.Outcome(Realm::SkyBelow),2);
    return true;
}
#endif
