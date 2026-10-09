#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Lexicon/UnmadeLexiconRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeLexiconTest, "Unmade.Lexicon.Evidence",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnmadeLexiconTest::RunTest(const FString& Parameters)
{
    UnmadeCore::LexiconModel Terms;
    TestTrue(TEXT("Unknown is unreadable"),
        Terms.Interpretation("term.veyl") == UnmadeCore::InterpretationState::Unknown);
    TestTrue(TEXT("Valid clue recorded"), Terms.RecordEvidence("term.veyl", "evidence.glimpse"));
    TestTrue(TEXT("One clue only partially understood"),
        Terms.Interpretation("term.veyl") == UnmadeCore::InterpretationState::Partial);
    TestFalse(TEXT("Duplicate clue cannot unlock interpretation"),
        Terms.RecordEvidence("term.veyl", "evidence.glimpse"));
    TestTrue(TEXT("Second independent clue recorded"),
        Terms.RecordEvidence("term.veyl", "evidence.archivist"));
    TestTrue(TEXT("Two clues reveal authored meaning"),
        Terms.Interpretation("term.veyl") == UnmadeCore::InterpretationState::Understood);
    UnmadeCore::LexiconModel Restored;
    TestTrue(TEXT("Save restoration is accepted"), Restored.Restore(Terms.Snapshot()));
    TestTrue(TEXT("Reconstructed interpretation is understood"),
        Restored.Interpretation("term.veyl") == UnmadeCore::InterpretationState::Understood);
    return true;
}
#endif
