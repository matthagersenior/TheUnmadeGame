#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "World/UnmadeLaterHazardRules.h"
#include "Fracture/UnmadeFractureRules.h"
#include "Combat/UnmadeCombatRules.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeLaterRealmHazardTest,"Unmade.World.SixLaterRealmHazards",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnmadeLaterRealmHazardTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    for(int Index=0;Index<6;++Index)
    {
        const auto& Zone=LaterHazards[Index];
        const auto& Home=LaterRealms[Index];
        const double X=Home.centerX+Zone.laneX;
        const double Y=Home.centerY+(Zone.minY+Zone.maxY)*.5;
        const auto Warning=SampleLaterHazard(Zone.realm,Zone.warningAt+.5,X,Y,1);
        const auto Impact=SampleLaterHazard(Zone.realm,Zone.impactAt+.5,X,Y,1);
        TestTrue(TEXT("warning is noninjurious"),Warning.phase==FrontierHazardPhase::Warning);
        TestEqual(TEXT("warning severity"),Warning.severity,0.0);
        TestTrue(TEXT("impact changes domain"),Impact.phase==FrontierHazardPhase::Impact);
        TestEqual(TEXT("impact strength"),Impact.severity,Zone.severity);
        TestTrue(TEXT("safe edge avoids exposure"),
             SampleLaterHazard(Zone.realm,Zone.impactAt+.5,
                 LaterHazardShelterX(Zone.realm,X),Y,1).pulseId==0);
        TestTrue(TEXT("first-choice completion stops danger"),
             SampleLaterHazard(Zone.realm,Zone.impactAt+.5,X,Y,3).pulseId==0);
    }
    FractureModel Reality("region.prototype.hub",{"variant.open","variant.sealed"});
    TestTrue(TEXT("nonlethal unreality accumulates"),
        Reality.ApplyEnvironmentalStrain(18));
    TestTrue(TEXT("exposure never exceeds cap"),Reality.ApplyEnvironmentalStrain(1000));
    TestEqual(TEXT("capped strain"),Reality.CurrentStrain(),100.0);
    Combatant Guard(100,24,.75);
    TestTrue(TEXT("player can guard"),Guard.SetGuarding(true));
    TestTrue(TEXT("one physical hit"),Guard.ReceiveHit(0xF1000,1,14,false)==HitOutcome::Applied);
    TestEqual(TEXT("guard mitigates harm"),Guard.Health(),96.5);
    TestTrue(TEXT("duplicate pulse refuses damage"),
        Guard.ReceiveHit(0xF1000,1,14,false)==HitOutcome::Duplicate);
    return true;
}
#endif
