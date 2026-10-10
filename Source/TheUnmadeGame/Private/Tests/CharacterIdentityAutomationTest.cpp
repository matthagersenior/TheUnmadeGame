#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Player/UnmadeCharacterIdentityRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeCharacterIdentityTest,"Unmade.Player.CharacterIdentity",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeCharacterIdentityTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    CharacterIdentity Person;
    TestEqual(TEXT("origin fixed"), FString(UTF8_TO_TCHAR(Person.SharedOrigin().c_str())),
        FString(TEXT("Unmade.Origin.Impossible")));
    TestTrue(TEXT("new frame"),Person.Select(IdentityAspect::BodyFrame,2));
    TestFalse(TEXT("invalid voice refuses change"),Person.Select(IdentityAspect::Voice,8));
    TestTrue(TEXT("unconventional name"),Person.Rename(u8"Vēyl-No One"));
    const auto Save=Person.Snapshot();
    CharacterIdentity Reloaded;
    TestTrue(TEXT("identity round trip"),Reloaded.Restore(Save));
    TestEqual(TEXT("face choice preserved"),Reloaded.Snapshot().options[0],2);
    auto Bad=Save;
    Bad.options[4]=33;
    TestFalse(TEXT("invalid save rejected"),Reloaded.Restore(Bad));
    TestEqual(TEXT("invalid save does not wipe profile"),Reloaded.Snapshot().chosenName,Save.chosenName);
    return true;
}
#endif
