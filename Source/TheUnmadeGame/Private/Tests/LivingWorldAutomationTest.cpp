#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "World/UnmadeLivingWorldRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeLivingWorldTest, "Unmade.World.LivingCycle",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnmadeLivingWorldTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    LivingWorldClock Clock;
    TestEqual(TEXT("Start at 07:00"), Clock.MinuteOfDay(), 420);
    TestTrue(TEXT("Day begins"), Clock.Phase()==DayPhase::Day);
    TestTrue(TEXT("Dusk is distinct"), Clock.Restore(500.0) && Clock.Phase()==DayPhase::Dusk);
    TestFalse(TEXT("Invalid elapsed time rejected"), Clock.Restore(-1.0));
    DiscoveryLedger Exploration;
    TestTrue(TEXT("Discover one site"), Exploration.Discover(District::EchoWell));
    TestFalse(TEXT("Repeat discovery is idempotent"), Exploration.Discover(District::EchoWell));
    TestTrue(TEXT("Invalid mask fails closed"), !Exploration.Restore(1<<12));
    TestEqual(TEXT("Previous progress remains"), Exploration.Count(), 1);
    return true;
}
#endif
