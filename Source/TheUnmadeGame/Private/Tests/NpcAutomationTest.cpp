#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "NPC/UnmadeMemoryComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeNpcEvidenceTest, "Unmade.Npc.WitnessVsRumor",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeNpcEvidenceTest::RunTest(const FString& Parameters)
{
    UUnmadeMemoryComponent* Witness = NewObject<UUnmadeMemoryComponent>();
    UUnmadeMemoryComponent* Hearer = NewObject<UUnmadeMemoryComponent>();
    const FGuid EventId = FGuid::NewGuid();
    TestTrue(TEXT("Witness registers observation"), Witness->Witness(EventId, FName("Player.Helped")));
    TestFalse(TEXT("Duplicate event is ignored"), Witness->Witness(EventId, FName("Player.Helped")));
    TestEqual(TEXT("No duplicate memories"), Witness->GetObservationCount(), 1);
    TestFalse(TEXT("Anonymous rumor is refused"), Hearer->HearRumor(EventId, FName("Player.Helped"), NAME_None));
    TestEqual(TEXT("No magical knowledge"), Hearer->GetObservationCount(), 0);
    TestTrue(TEXT("Attributed rumor is admitted"), Hearer->HearRumor(EventId, FName("Player.Helped"), FName("npc.merchant.001")));
    TestTrue(TEXT("Directly witnessed aid has a stronger trust effect"), Witness->GetTrust() > Hearer->GetTrust());
    TestTrue(TEXT("Witness upgrades the rumor"), Hearer->Witness(EventId, FName("Player.Helped")));
    TestEqual(TEXT("Upgrading doesn't duplicate"), Hearer->GetObservationCount(), 1);
    TestEqual(TEXT("Upgrading corrects trust score"), Hearer->GetTrust(), Witness->GetTrust());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeNpcSnapshotTest, "Unmade.Npc.Snapshot",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeNpcSnapshotTest::RunTest(const FString& Parameters)
{
    UUnmadeMemoryComponent* Original = NewObject<UUnmadeMemoryComponent>();
    const FGuid Aid = FGuid::NewGuid();
    const FGuid Anomaly = FGuid::NewGuid();
    Original->Witness(Aid, FName("Player.Helped"));
    Original->HearRumor(Anomaly, FName("Reality.Anomaly"), FName("npc.guard.001"));
    const FUnmadeNpcSnapshot Snapshot = Original->WriteSnapshot(FName("npc.merchant.001"));

    UUnmadeMemoryComponent* Restored = NewObject<UUnmadeMemoryComponent>();
    TestFalse(TEXT("Wrong identity refuses snapshot"), Restored->ReadSnapshot(Snapshot, FName("npc.guard.001")));
    TestEqual(TEXT("Invalid restoration changes nothing"), Restored->GetObservationCount(), 0);
    TestTrue(TEXT("Identity matched restores"), Restored->ReadSnapshot(Snapshot, FName("npc.merchant.001")));
    TestEqual(TEXT("Round trip memory count"), Restored->GetObservationCount(), 2);
    TestEqual(TEXT("Trust restored"), Restored->GetTrust(), Original->GetTrust());
    TestEqual(TEXT("Fear restored"), Restored->GetFear(), Original->GetFear());
    TestEqual(TEXT("Rumor provenance retained"), Restored->GetObservations()[1].SpeakerId, FName("npc.guard.001"));
    TestTrue(TEXT("Repeat load is idempotent"), Restored->ReadSnapshot(Snapshot, FName("npc.merchant.001")));
    TestEqual(TEXT("Still only two observations"), Restored->GetObservationCount(), 2);
    return true;
}
#endif
