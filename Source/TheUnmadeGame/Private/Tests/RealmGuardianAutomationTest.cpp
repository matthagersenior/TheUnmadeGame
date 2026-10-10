#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Combat/UnmadeRealmGuardianRules.h"
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUnmadeSixRealmGuardians,
    "Unmade.Combat.SixOptionalRealmGuardians",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FUnmadeSixRealmGuardians::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    GuardianChronicle Resolutions;
    for(const auto& Profile:RealmGuardians)
    {
        GuardianBeat Beat(Profile.realm);
        TestTrue(TEXT("warning precedes impact"),
            Beat.Advance(12,Profile.rangeCm-25,false,true)==GuardianAction::Telegraph);
        TestTrue(TEXT("cannot hit before telegraph expires"),
            Beat.Advance(12+.5,Profile.rangeCm-25,false,true)==GuardianAction::Telegraph);
        TestTrue(TEXT("can hit after warning interval"),
            Beat.Advance(12+Profile.windup+.01,Profile.rangeCm-25,false,true)==GuardianAction::Strike);
        TestTrue(TEXT("unwitnessed mercy remains unavailable"),
            Resolutions.Resolve(Profile.realm,false,true)==GuardianResolution::Locked);
        TestTrue(TEXT("witnessed mercy resolves permanently"),
            Resolutions.Resolve(Profile.realm,true,true)==GuardianResolution::Pacified);
        TestEqual(TEXT("saved mercy code"),
            static_cast<int>(Resolutions.Outcome(Profile.realm)),1);
    }
    GuardianChronicle Loaded;
    TestTrue(TEXT("all six outcomes restore safely"),Loaded.Restore(Resolutions.Snapshot()));
    auto Invalid=Resolutions.Snapshot();
    Invalid.outcomes[0]=3;
    TestFalse(TEXT("bad saved guardian code rejected"),Loaded.Restore(Invalid));
    TestEqual(TEXT("previous resolution survives invalid save"),
        static_cast<int>(Loaded.Outcome(Realm::TidalLedger)),1);
    return true;
}
#endif
