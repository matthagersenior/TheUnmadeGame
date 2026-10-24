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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeLocalDialogueMultiNpcTest,
    "Unmade.LocalDialogue.MultiNPCProvenance",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeLocalDialogueMultiNpcTest::RunTest(const FString& Parameters)
{
    const FGuid SharedEvent = FGuid::NewGuid();
    const FGuid SeparateEvent = FGuid::NewGuid();
    UUnmadeMemoryComponent* WitnessMemory = NewObject<UUnmadeMemoryComponent>();
    UUnmadeMemoryComponent* RumorMemory = NewObject<UUnmadeMemoryComponent>();
    UUnmadeMemoryComponent* CorrectedMemory = NewObject<UUnmadeMemoryComponent>();

    TestTrue(TEXT("one resident witnesses the event"), WitnessMemory->Witness(
        SharedEvent, FName("Player.Helped"), FName("npc.crossings.refugee.001")));
    TestTrue(TEXT("another resident hears the same event as a rumor"), RumorMemory->HearRumor(
        SharedEvent, FName("Player.Helped"), FName("npc.crossings.matron.001"),
        FName("npc.crossings.refugee.001")));
    TestTrue(TEXT("the second resident separately witnesses a conflicting choice"), RumorMemory->Witness(
        SeparateEvent, FName("World.ConflictResearch"), FName("npc.crossings.refugee.001")));

    const FString WitnessEvidence = FUnmadeDialoguePolicy::BuildEvidence(WitnessMemory);
    const FString RumorEvidence = FUnmadeDialoguePolicy::BuildEvidence(RumorMemory);
    TestTrue(TEXT("witness account retains its direct source"),
        WitnessEvidence.Contains(TEXT("Personally witnessed: Player.Helped")));
    TestFalse(TEXT("another NPC's knowledge does not leak into the witness"),
        WitnessEvidence.Contains(TEXT("World.ConflictResearch")));
    TestFalse(TEXT("a witnessed event cannot be downgraded by later rumor"),
        WitnessMemory->HearRumor(SharedEvent, FName("Player.Helped"),
            FName("npc.crossings.matron.001"), FName("npc.crossings.refugee.001")));
    TestTrue(TEXT("rumor account keeps its named speaker"),
        RumorEvidence.Contains(TEXT("Heard rumor from npc.crossings.matron.001")));
    TestTrue(TEXT("rumor remains uncertain even beside a contradictory witnessed choice"),
        RumorEvidence.Contains(TEXT("NOT witnessed")) &&
        RumorEvidence.Contains(TEXT("Personally witnessed: World.ConflictResearch")));

    TestTrue(TEXT("direct testimony corrects an earlier rumor"),
        CorrectedMemory->HearRumor(SharedEvent, FName("Player.Helped"),
            FName("npc.crossings.matron.001"), FName("npc.crossings.refugee.001")));
    TestTrue(TEXT("witness correction is accepted"), CorrectedMemory->Witness(
        SharedEvent, FName("Player.Helped"), FName("npc.crossings.refugee.001")));
    const FString CorrectedEvidence = FUnmadeDialoguePolicy::BuildEvidence(CorrectedMemory);
    TestTrue(TEXT("corrected account becomes firsthand"),
        CorrectedEvidence.Contains(TEXT("Personally witnessed: Player.Helped")));
    TestFalse(TEXT("corrected account cannot continue to repeat its old rumor"),
        CorrectedEvidence.Contains(TEXT("Heard rumor")));
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeLocalDialogueOfflineReplyTest,
    "Unmade.LocalDialogue.OfflineReplies",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeLocalDialogueOfflineReplyTest::RunTest(const FString& Parameters)
{
    UUnmadeMemoryComponent* Memory = NewObject<UUnmadeMemoryComponent>();
    const FGuid HelpEvent = FGuid::NewGuid();
    const FGuid RumorEvent = FGuid::NewGuid();
    Memory->Witness(HelpEvent, FName("Player.Helped"), FName("npc.merchant.001"));
    Memory->HearRumor(RumorEvent, FName("Reality.Anomaly"),
        FName("npc.guard.001"), FName("Hub.AnomalyMarker"));

    const FString WitnessReply = FUnmadeDialoguePolicy::BuildOfflineReply(
        Memory, TEXT("What have you personally witnessed?"), TEXT("routine voice"));
    TestTrue(TEXT("offline reply describes firsthand help"),
        WitnessReply.Contains(TEXT("you helped someone nearby")) &&
        WitnessReply.Contains(TEXT("I saw it myself")));

    const FString MemoryReply = FUnmadeDialoguePolicy::BuildOfflineReply(
        Memory, TEXT("What do you remember about me?"), TEXT("routine voice"));
    TestTrue(TEXT("offline memory reply labels rumor source and uncertainty"),
        MemoryReply.Contains(TEXT("npc.guard.001")) &&
        MemoryReply.Contains(TEXT("I did not see it myself")));

    const FString ConcernReply = FUnmadeDialoguePolicy::BuildOfflineReply(
        Memory, TEXT("What worries you today?"), TEXT("routine voice"));
    TestTrue(TEXT("concern reply labels the rumor as a report"),
        ConcernReply.Contains(TEXT("npc.guard.001")) &&
        ConcernReply.Contains(TEXT("did not witness it")));

    UUnmadeMemoryComponent* EmptyMemory = NewObject<UUnmadeMemoryComponent>();
    TestTrue(TEXT("empty witness question admits no firsthand account"),
        FUnmadeDialoguePolicy::BuildOfflineReply(EmptyMemory,
            TEXT("What have you personally witnessed?"), TEXT("routine voice"))
            .Contains(TEXT("no firsthand account")));
    return true;
}
#endif
