#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "World/UnmadeFrontierHazardRules.h"
#include "Combat/UnmadeCombatRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeFrontierHazardTest, "Unmade.World.FrontierHazardAndCheckpoint",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeFrontierHazardTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    const auto Storm=SampleFrontierHazard(Realm::WidowedRain,11,0,-49300,true,false);
    const auto Warning=SampleFrontierHazard(Realm::WidowedRain,8,0,-49300,true,false);
    TestTrue(TEXT("warning before storm strike"),
        Warning.phase==FrontierHazardPhase::Warning && Warning.damage==0);
    TestTrue(TEXT("storm damages only during impact"),
        Storm.phase==FrontierHazardPhase::Impact && Storm.damage==16);
    TestTrue(TEXT("mechanism stops pulses"),
        SampleFrontierHazard(Realm::WidowedRain,11,0,-49300,true,true).damage==0);
    TestTrue(TEXT("safe edge is not damaged"),
        SampleFrontierHazard(Realm::HearthBeneath,11,3000,50700,true,false).damage==0);
    Combatant Player(100,24,0.75);
    Player.SetGuarding(true);
    TestTrue(TEXT("guard reduces environment impact"),
        Player.ReceiveHit(0xF0100,1,Storm.damage,false)==HitOutcome::Applied);
    TestEqual(TEXT("guarded health"), Player.Health(),96.0);
    Player.SetGuarding(false);
    TestTrue(TEXT("same pulse cannot repeat"),
        Player.ReceiveHit(0xF0100,1,Storm.damage,false)==HitOutcome::Duplicate);
    TestTrue(TEXT("first lethal source applied"),
        Player.ReceiveHit(0xF0101,1,150,false)==HitOutcome::Applied);
    TestFalse(TEXT("player defeated"),Player.IsAlive());
    TestTrue(TEXT("checkpoint recovery"),
        Player.ReviveAtCheckpoint(.75));
    TestEqual(TEXT("checkpoint health"),Player.Health(),75.0);
    return true;
}
#endif
