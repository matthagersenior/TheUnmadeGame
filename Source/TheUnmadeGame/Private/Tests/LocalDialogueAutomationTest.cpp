#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Dialogue/UnmadeDialoguePolicy.h"
#include "NPC/UnmadeMemoryComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeLocalDialogueParseTest,
    "Unmade.LocalDialogue.Parse",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeLocalDialogueParseTest::RunTest(const FString& Parameters)
{
    FString Line;
    // Valid /api/chat message.content is itself a JSON object.
    const FString Valid = TEXT("{\"done\":true,\"message\":{\"content\":\"{\\\"line\\\":\\\"I remember your kindness.\\\"}\"}}");
    TestTrue(TEXT("valid response"), FUnmadeDialoguePolicy::TryExtractLine(Valid, Line));
    TestEqual(TEXT("dialogue is extracted"), Line, FString(TEXT("I remember your kindness.")));

    TestFalse(TEXT("malformed response"), FUnmadeDialoguePolicy::TryExtractLine(TEXT("{"), Line));
    TestFalse(TEXT("missing done marker"), FUnmadeDialoguePolicy::TryExtractLine(
        TEXT("{\"message\":{\"content\":\"{\\\"line\\\":\\\"Hello\\\"}\"}}"), Line));
    TestFalse(TEXT("empty dialogue"), FUnmadeDialoguePolicy::TryExtractLine(
        TEXT("{\"done\":true,\"message\":{\"content\":\"{\\\"line\\\":\\\"\\\"}\"}}"), Line));
    TestFalse(TEXT("overlong reply"), FUnmadeDialoguePolicy::TryExtractLine(
        FString::Printf(TEXT("{\"done\":true,\"message\":{\"content\":\"{\\\"line\\\":\\\"%s\\\"}\"}}"),
        *FString::ChrN(241, TEXT('X'))), Line));
    TestTrue(TEXT("failed parse clears prior text"), Line.IsEmpty());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeLocalDialogueProvenanceTest,
    "Unmade.LocalDialogue.Provenance",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeLocalDialogueProvenanceTest::RunTest(const FString& Parameters)
{
    UUnmadeMemoryComponent* Memory = NewObject<UUnmadeMemoryComponent>();
    Memory->Witness(FGuid::NewGuid(), FName("Player.Helped"), FName("npc.merchant.001"));
    Memory->HearRumor(FGuid::NewGuid(), FName("Reality.Anomaly"),
        FName("npc.guard.001"), FName("Hub.AnomalyMarker"));

    const FString Evidence = FUnmadeDialoguePolicy::BuildEvidence(Memory);
    TestTrue(TEXT("witnessed aid is labeled"), Evidence.Contains(TEXT("Personally witnessed")));
    TestTrue(TEXT("rumor remains explicitly uncertain"), Evidence.Contains(TEXT("Heard rumor")));
    TestTrue(TEXT("rumor provenance remains visible"), Evidence.Contains(TEXT("npc.guard.001")));
    TestTrue(TEXT("rumor is NOT witnessed"), Evidence.Contains(TEXT("NOT witnessed")));
    return true;
}
#endif
