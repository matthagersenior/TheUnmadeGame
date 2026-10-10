#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Combat/UnmadeFinalEncounterRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnmadeFinalAnswerTest,
    "Unmade.Combat.FinalAnswer",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeFinalAnswerTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    FinalJourney Story;
    TestTrue(TEXT("world transition unavailable before actual boss victory"),
        Story.ChooseWorld(Morning::Many)==FinalResult::WrongStage);
    TestTrue(TEXT("first death reveals second boss instead of ending game"),
        Story.BreakMask(true,3)==FinalResult::MaskBroken);
    TestTrue(TEXT("true victory only after second defeat"),
        Story.BreakCore()==FinalResult::TrueVictory);
    TestTrue(TEXT("two persistent world variants offered after victory"),
        Story.ChooseWorld(Morning::Anchor)==FinalResult::NewMorning);
    TestTrue(TEXT("post-victory state survives"),
        Story.World()==Morning::Anchor && Story.Act()==FinalAct::OtherMorning);
    FinalJourney Loaded;
    TestTrue(TEXT("saved alternate world restores"),Loaded.Restore(Story.Snapshot()));
    TestTrue(TEXT("noncompulsory new-form rematch can be awakened"),
        Loaded.WakeEcho()==FinalResult::EchoStarted);
    TestTrue(TEXT("echo does not undo world choice"),
        Loaded.World()==Morning::Anchor);
    FinalBattleRhythm Boss;
    const auto Cue=Boss.Advance(12,100,false,FinalAct::EchoAwake,3,Morning::Anchor);
    TestTrue(TEXT("new boss announces its strike"),Cue.move==FinalMove::Telegraph);
    TestTrue(TEXT("no invisible instant damage"),Cue.damage==0);
    const auto Hit=Boss.Advance(12+Cue.warningSeconds+.01,100,false,
        FinalAct::EchoAwake,3,Morning::Anchor);
    TestTrue(TEXT("strike only after the cue"),Hit.move==FinalMove::Strike);
    return true;
}
#endif
