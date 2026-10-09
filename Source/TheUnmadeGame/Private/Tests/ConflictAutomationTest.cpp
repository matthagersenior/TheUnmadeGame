#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Story/UnmadeConflictRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeStoryConflictTest, "Unmade.Story.BranchingConflict",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeStoryConflictTest::RunTest(const FString& Parameters)
{
    using namespace UnmadeCore;
    ConflictModel Model;

    TestTrue(TEXT("Choice needs preview"), Model.Preview(ConflictChoice::Shelter)
        == ConflictResult::NeedsConfirmation);
    TestTrue(TEXT("Preview does not open route"), !Model.ShelterOpen());
    TestTrue(TEXT("Confirmation commits shelter"), Model.Commit(ConflictChoice::Shelter,true)
        == ConflictResult::Committed);
    TestTrue(TEXT("Shelter passage opens"), Model.ShelterOpen());
    TestTrue(TEXT("Archive passage remains closed"), !Model.ArchiveOpen());
    TestTrue(TEXT("Second choice cannot override first"), Model.Commit(ConflictChoice::Research,true)
        == ConflictResult::AlreadyCommitted);
    TestFalse(TEXT("Deliver requires supplies"), Model.DeliverSupplies(true));
    TestTrue(TEXT("Collect at market"), Model.CollectSupplies(true));
    TestTrue(TEXT("Deliver at shelter"), Model.DeliverSupplies(true));

    const ConflictSnapshot Snapshot = Model.Snapshot();
    ConflictModel Restored;
    TestTrue(TEXT("Restore choice and supply status"), Restored.Restore(Snapshot));
    TestTrue(TEXT("Restored route remains open"), Restored.ShelterOpen());
    TestFalse(TEXT("Reject invalid choices"), Restored.Restore({99, 2}));
    TestTrue(TEXT("Rejected restore preserves prior state"), Restored.ShelterOpen());
    return true;
}
#endif
