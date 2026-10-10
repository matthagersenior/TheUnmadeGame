#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "World/UnmadeRealmResonanceRules.h"
#include "World/UnmadeRealmGeometryRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeRealmResonanceTest,"Unmade.World.SixDualRiteObservatories",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnmadeRealmResonanceTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    TestTrue(TEXT("physical upper bridge reaches separate observatory island"),
             LaterArenaLayout::Sound());
    RealmResonanceJourney Journey;
    for(const auto& Spec:RealmResonances)
    {
        TestTrue(TEXT("unresolved story cannot open a site"),
                 Journey.Record(Spec.realm,Spec.first,240,false,true)==ResonanceResult::Locked);
        TestTrue(TEXT("first earned cast records the discipline"),
                 Journey.Record(Spec.realm,Spec.first,240,true,true)==ResonanceResult::Started);
        TestTrue(TEXT("same cast cannot satisfy second requirement"),
                 Journey.Record(Spec.realm,Spec.first,280,true,true)==ResonanceResult::AlreadyRecorded);
        TestTrue(TEXT("other cast opens distinct physical site"),
                 Journey.Record(Spec.realm,Spec.second,290,true,true)==ResonanceResult::Opened);
        TestEqual(TEXT("realm remembers opened state"),Journey.Stage(Spec.realm),2);
    }
    RealmResonanceJourney Reloaded;
    TestTrue(TEXT("six completed observatories reload atomically"),
             Reloaded.Restore(Journey.Snapshot()));
    auto Corrupt=Journey.Snapshot();
    Corrupt.stage[0]=3;
    TestFalse(TEXT("corrupt state cannot overwrite good progress"),
              Reloaded.Restore(Corrupt));
    TestEqual(TEXT("saved first realm remains open"),Reloaded.Stage(Realm::TidalLedger),2);
    return true;
}
#endif
