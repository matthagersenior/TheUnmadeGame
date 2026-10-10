#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "World/UnmadeEchoQuestRules.h"
#include "World/UnmadeRealmGeometryRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnmadeEchoQuestTest,
    "Unmade.World.SixThirdChapterEchoQuests",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeEchoQuestTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    TestTrue(TEXT("floors separated and both civic gates clear"),
             LaterArenaLayout::Sound());
    EchoChronicle Chronicle;
    for(const auto& Quest:EchoQuests)
    {
        TestTrue(TEXT("third chapter demands first two outcomes"),
                 Chronicle.Begin(Quest.realm,false,true,Quest.openingWitness)==EchoResult::Locked);
        TestTrue(TEXT("starts with recorded civic keeper"),
                 Chronicle.Begin(Quest.realm,true,true,Quest.openingWitness)==EchoResult::Started);
        TestTrue(TEXT("first discovery"),
                 Chronicle.Inspect(Quest.realm,Quest.evidenceA)==EchoResult::Discovered);
        TestTrue(TEXT("testimony blocked before both clues"),
                 Chronicle.Testify(Quest.realm,Quest.witnessCare)==EchoResult::NeedEvidence);
        TestTrue(TEXT("second independent discovery"),
                 Chronicle.Inspect(Quest.realm,Quest.evidenceB)==EchoResult::Discovered);
        TestTrue(TEXT("care witness qualifies"),
                 Chronicle.Testify(Quest.realm,Quest.witnessCare)==EchoResult::Testified);
        TestTrue(TEXT("wrong control refused"),
                 Chronicle.Commit(Quest.realm,2,Quest.ritualTruth)==EchoResult::WrongChoice);
        TestTrue(TEXT("one-time matched resolution"),
                 Chronicle.Commit(Quest.realm,1,Quest.ritualCare)==EchoResult::Committed);
    }
    EchoChronicle Load;
    TestTrue(TEXT("six independent state groups restore"),Load.Restore(Chronicle.Snapshot()));
    auto Invalid=Chronicle.Snapshot();
    Invalid.evidence[3]=0;
    TestFalse(TEXT("corrupt proof cannot erase a resolution"),Load.Restore(Invalid));
    TestEqual(TEXT("previous ending intact"),Load.Ending(Realm::HundredUnlived),1);
    return true;
}
#endif
