#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Game/UnmadeGameMode.h"
#include "Player/UnmadeCharacter.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeBootstrapClassesTest,
    "Unmade.Bootstrap.ModuleClasses",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeBootstrapClassesTest::RunTest(const FString& Parameters)
{
    const AUnmadeGameMode* GameModeDefaults = GetDefault<AUnmadeGameMode>();
    TestNotNull(TEXT("The game mode has a default object"), GameModeDefaults);
    if (!GameModeDefaults)
    {
        return false;
    }

    TestEqual(TEXT("The player pawn is the Unmade character"),
        GameModeDefaults->DefaultPawnClass.Get(), AUnmadeCharacter::StaticClass());

    const AUnmadeCharacter* CharacterDefaults = GetDefault<AUnmadeCharacter>();
    TestNotNull(TEXT("The player class has a default object"), CharacterDefaults);
    return true;
}
#endif
