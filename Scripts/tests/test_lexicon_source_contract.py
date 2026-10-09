"""Ensure authored language clues are wired into Unreal without an LLM dependency."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]
def src(name):
    return (ROOT/name).read_text(encoding="utf-8")
class LexiconSourceContract(unittest.TestCase):
    def test_two_clues_reach_player_without_model(self):
        p=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertIn('Lexicon->RecordEvidence(FName("evidence.glimpse"))',p)
        self.assertIn('Lexicon->RecordEvidence(FName("evidence.archivist"))',p)
        self.assertIn('Lexicon->UnderstandsVeyl()',p)
        self.assertNotIn("GetSubsystem<UUnmadeLocalDialogueSubsystem>()->RecordEvidence",p)

    def test_fails_closed_on_invalid_saves(self):
        component=src("Source/TheUnmadeGame/Private/Lexicon/UnmadeLexiconComponent.cpp")
        save=src("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for key in ("LexiconEvidence", "LoadOrCreate()", "Lexicon.Restore(Previous)", "SaveGameToSlot"):
            self.assertIn(key,component if key!="LexiconEvidence" else save)
        self.assertIn("SchemaVersion != 1", component)
        self.assertIn("LexiconEvidence",save)

    def test_native_rules_are_exercised_in_ci(self):
        workflow=src(".github/workflows/static-checks.yml")
        self.assertIn("Tests/lexicon/lexicon_rules_test.cpp",workflow)
        self.assertIn("/tmp/unmade-lexicon-test",workflow)
        self.assertIn("Unmade.Lexicon.Evidence",
            src("Source/TheUnmadeGame/Private/Tests/LexiconAutomationTest.cpp"))
if __name__=="__main__":
    unittest.main()
