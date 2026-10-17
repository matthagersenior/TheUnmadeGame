#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "NPC/UnmadeMemoryComponent.h"
#include "Save/UnmadeActionCommitRules.h"
#include "NPC/UnmadeNpcMotionRules.h"

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
    Original->Witness(Aid, FName("Player.Helped"), FName("npc.merchant.001"));
    Original->HearRumor(Anomaly, FName("Reality.Anomaly"), FName("npc.guard.001"), FName("Hub.AnomalyMarker"));
    const FUnmadeNpcSnapshot Snapshot = Original->WriteSnapshot(FName("npc.merchant.001"));

    UUnmadeMemoryComponent* Restored = NewObject<UUnmadeMemoryComponent>();
    TestFalse(TEXT("Wrong identity refuses snapshot"), Restored->ReadSnapshot(Snapshot, FName("npc.guard.001")));
    TestEqual(TEXT("Invalid restoration changes nothing"), Restored->GetObservationCount(), 0);
    TestTrue(TEXT("Identity matched restores"), Restored->ReadSnapshot(Snapshot, FName("npc.merchant.001")));
    TestEqual(TEXT("Round trip memory count"), Restored->GetObservationCount(), 2);
    TestEqual(TEXT("Trust restored"), Restored->GetTrust(), Original->GetTrust());
    TestEqual(TEXT("Aid target retained"), Restored->GetObservations()[0].SubjectId, FName("npc.merchant.001"));
    TestEqual(TEXT("Rumor subject retained"), Restored->GetObservations()[1].SubjectId, FName("Hub.AnomalyMarker"));
    TestEqual(TEXT("Fear restored"), Restored->GetFear(), Original->GetFear());
    TestEqual(TEXT("Rumor provenance retained"), Restored->GetObservations()[1].SpeakerId, FName("npc.guard.001"));
    TestTrue(TEXT("Repeat load is idempotent"), Restored->ReadSnapshot(Snapshot, FName("npc.merchant.001")));
    TestEqual(TEXT("Still only two observations"), Restored->GetObservationCount(), 2);
    return true;
}
#endif

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeNpcMotionTest, "Unmade.Npc.OfflineMotion",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeNpcMotionTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    const auto Step = SteerNpc({0,0}, {1000,0}, NpcMotion::Approach, 200, 1.0, 100);
    TestEqual(TEXT("Long frame cannot teleport"), Step.x, 50.0);
    TestEqual(TEXT("No sideways drift"), Step.y, 0.0);
    const auto Away = SteerNpc({0,0}, {100,0}, NpcMotion::Retreat, 200, 1.0, 300);
    TestEqual(TEXT("Avoidance increases separation"), Away.x, -50.0);
    return true;
}
#endif

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeNpcActionRollbackTest, "Unmade.Npc.ActionConsequenceRollback",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeNpcActionRollbackTest::RunTest(const FString& Parameters)
{
    auto* Memory=NewObject<UUnmadeMemoryComponent>();
    const FName Resident("npc.merchant.001");
    for(int32 i=0;i<64;++i)
        Memory->HearRumor(FGuid::NewGuid(),FName("Reality.Anomaly"),FName("npc.guard.001"),Resident);
    const auto Before=Memory->WriteSnapshot(Resident);
    const auto NewEvent=FGuid::NewGuid();
    UnmadeCore::TenfoldChronicle Chronicle;
    int32 Writes=0;
    const auto Commit=[&](bool SaveWorks){
        return UnmadeCore::CommitActionConsequences([&]{return Memory->WriteSnapshot(Resident);},
            [&]{Memory->Witness(NewEvent,FName("Player.Helped"),Resident);},
            [&]{return UnmadeCore::CommitActionDeeds(Chronicle,1,[&]{
                ++Writes;
                TestTrue(TEXT("Writer sees staged testimony"),Memory->KnowsEvent(NewEvent));
                TestEqual(TEXT("Writer sees the deed"),static_cast<int32>(Chronicle.Snapshot().deeds),1);
                return SaveWorks;
            });},[&](const auto& Previous){TestTrue(TEXT("Rollback restores snapshot"),Memory->ReadSnapshot(Previous,Resident));});
    };
    TestFalse(TEXT("Failed save rejects the action"),Commit(false));
    TestEqual(TEXT("No deed survives failure"),static_cast<int32>(Chronicle.Snapshot().deeds),0);
    TestFalse(TEXT("Failed testimony is removed"),Memory->KnowsEvent(NewEvent));
    TestTrue(TEXT("Evicted oldest event is restored"),Memory->KnowsEvent(Before.Observations[0].EventId));
    TestEqual(TEXT("Trust restored"),Memory->GetTrust(),Before.Trust);
    TestEqual(TEXT("Fear restored"),Memory->GetFear(),Before.Fear);
    TestTrue(TEXT("Retry succeeds"),Commit(true));
    TestEqual(TEXT("Capacity remains bounded"),Memory->GetObservationCount(),64);
    TestEqual(TEXT("One writer per attempt"),Writes,2);
    TestTrue(TEXT("Repeated snapshot restore"),Memory->ReadSnapshot(Memory->WriteSnapshot(Resident),Resident));
    return true;
}
#endif
