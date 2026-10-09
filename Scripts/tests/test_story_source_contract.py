"""Static wiring checks. The C++17 domain test is executable; Unreal is not run here."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(path):
    return (ROOT/path).read_text(encoding="utf-8")

class StoryWiringTests(unittest.TestCase):
    def test_choices_are_not_dependent_on_ai_or_language(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        rules=read("Source/TheUnmadeGame/Public/Story/UnmadeConflictRules.h")
        self.assertIn("LocalConflict.Commit(Choice, true)",player)
        self.assertIn("PendingStoryExpiresAt = Now + 6.0",player)
        self.assertIn("LocalConflict.Restore(Previous)",player)
        self.assertIn("LocalConflict.Preview(Choice)",player)
        self.assertNotIn("UnmadeLocalDialogue",rules)
        self.assertNotIn("Lexicon",rules)
        self.assertNotIn("HTTP",rules)

    def test_two_physical_routes_follow_outcome(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        gate=read("Source/TheUnmadeGame/Private/Story/UnmadeConflictGate.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertEqual(hub.count("SpawnActor<AUnmadeConflictGate>("),2)
        self.assertIn('FName("gate.prototype.shelter")',hub)
        self.assertIn('FName("gate.prototype.archive")',hub)
        self.assertIn("SetCollisionEnabled",gate)
        self.assertIn("It->SetAccess(LocalConflict.ShelterOpen())",player)
        self.assertIn("It->SetAccess(LocalConflict.ArchiveOpen())",player)

    def test_existing_save_data_is_preserved(self):
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for field in ("NpcSnapshots", "WorldVariantId", "LexiconEvidence",
                      "bHasConflictSnapshot", "LocalConflictChoice", "SupplyActivityStage"):
            self.assertIn(field,save)
        self.assertIn("UUnmadePrototypeSave::LoadOrCreate()",player)
        self.assertIn("ReportLocalEvent",player)

    def test_supply_activity_and_journal_are_wired(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        config=read("Config/DefaultInput.ini")
        self.assertIn("CollectSupplies(bNearMarket)",player)
        self.assertIn("DeliverSupplies(bNearShelter)",player)
        self.assertIn('FName("Player.DeliveredSupplies")',player)
        self.assertIn("Lexicon->GetClueCount()",player)
        for action in ("StoryShelter", "StoryResearch", "SupplyActivity", "StoryJournal"):
            self.assertIn('BindAction("'+action+'"',player)
            self.assertIn('ActionName="'+action+'"',config)

    def test_real_native_tests_are_on_ci(self):
        workflow=read(".github/workflows/static-checks.yml")
        self.assertIn("Tests/story/conflict_rules_test.cpp",workflow)
        self.assertIn("/tmp/unmade-conflict-test",workflow)
        self.assertIn("Unmade.Story.BranchingConflict",
            read("Source/TheUnmadeGame/Private/Tests/ConflictAutomationTest.cpp"))
if __name__=="__main__":
    unittest.main()
