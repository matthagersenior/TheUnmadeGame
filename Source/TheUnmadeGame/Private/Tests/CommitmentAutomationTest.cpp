#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Story/UnmadeCommitmentRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeCommitmentTest,"Unmade.Story.CommitmentConfirmation",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeCommitmentTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    CommitmentGate Gate;
    TestTrue(TEXT("initial selection is preview"),
        Gate.Attempt("Bellwold.Afterlight",1,10)==CommitmentAttempt::Preview);
    TestTrue(TEXT("changed route forces new preview"),
        Gate.Attempt("Bellwold.Afterlight",2,12)==CommitmentAttempt::Preview);
    TestTrue(TEXT("changing witness/arc forces new preview"),
        Gate.Attempt("Cinderhold.Return",2,13)==CommitmentAttempt::Preview);
    TestTrue(TEXT("same choice, same arc commits"),
        Gate.Attempt("Cinderhold.Return",2,15)==CommitmentAttempt::Confirmed);
    TestTrue(TEXT("confirmation cannot replay without new preview"),
        Gate.Attempt("Cinderhold.Return",2,16)==CommitmentAttempt::Preview);
    TestTrue(TEXT("expired previews do not commit"),
        Gate.Attempt("Cinderhold.Return",2,24)==CommitmentAttempt::Preview);
    Gate.Cancel();
    TestTrue(TEXT("aborted choice requires fresh preview"),
        Gate.Attempt("Cinderhold.Return",2,25)==CommitmentAttempt::Preview);
    return true;
}
#endif
