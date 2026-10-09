#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Fracture/UnmadeFractureRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeFractureRulesTest, "Unmade.Fracture.CoreRules",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnmadeFractureRulesTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    FractureModel Model("region.prototype.hub", {"variant.open","variant.sealed"});
    TestTrue(TEXT("Invalid target rejected"), Model.Glimpse(false, 1.0) == Result::UnsupportedTarget);
    TestTrue(TEXT("Glimpse accepted"), Model.Glimpse(true, 1.0) == Result::Applied);
    TestTrue(TEXT("Glimpse expires"), !Model.IsGlimpsing(4.1));
    TestTrue(TEXT("Fold opens briefly"), Model.Fold(true, 5.0) == Result::Applied);
    TestTrue(TEXT("Fold expires"), !Model.IsFolded(12.0));
    TestTrue(TEXT("Rewrite preview only"), Model.Rewrite("region.prototype.hub","variant.open",false) == Result::NeedsConfirmation);
    TestTrue(TEXT("No preview mutation"), Model.WorldVariant().empty());
    TestTrue(TEXT("Confirmed rewrite"), Model.Rewrite("region.prototype.hub","variant.open",true) == Result::Applied);
    TestTrue(TEXT("Commit exactly once"), Model.Rewrite("region.prototype.hub","variant.sealed",true) == Result::AlreadyCommitted);
    return true;
}
#endif
