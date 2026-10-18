"""Nine-realm return dialogue must retain provenances and actual NPC relationship history.

Actor-side source contracts: Unreal is not available in portable CI.
"""
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
NPC = ROOT / "Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp"


class WitnessReturnMemory(unittest.TestCase):
    def test_expedition_context_does_not_bypass_real_witness_memories(self):
        src = NPC.read_text(encoding="utf-8")
        dialogue = src.split("FString AUnmadeNpcCharacter::GetReactionText() const", 1)[1].split(
            "void AUnmadeNpcCharacter::ConfigureExpeditionResident", 1
        )[0]
        self.assertNotRegex(dialogue, r"if\(!Expedition\.IsEmpty\(\)\)return Expedition")
        self.assertIn("CachedHub->GetExpeditionReaction(NpcId)", dialogue)
        self.assertIn("const TArray<FUnmadeNpcObservation>& Observations", dialogue)
        self.assertIn("GetWitnessDispatchRecipientLine(NpcId)", dialogue)
        self.assertIn("GetWitnessReturnLine(NpcId)", dialogue)
        self.assertIn("GetResidentReturnLine(NpcId,CurrentHome)", dialogue)

    def test_later_realm_day_night_lines_are_chosen_before_saved_return_append(self):
        src = NPC.read_text(encoding="utf-8")
        dialogue = src.split("FString AUnmadeNpcCharacter::GetReactionText() const", 1)[1].split(
            "void AUnmadeNpcCharacter::ConfigureExpeditionResident", 1
        )[0]
        night = dialogue.index("Phase==UnmadeCore::DayPhase::Night")
        memories = dialogue.index("GetResidentReturnLine(NpcId,CurrentHome)")
        self.assertLess(night, memories, "Later-realm day/night lines erase remembered visits")
        self.assertNotIn("CharacterLine=FString(UTF8_TO_TCHAR(", dialogue[memories:])
        self.assertIn("UnmadeCore::ResidentContinuityIndex(", dialogue,
                      "Unregistered expedition actors must not claim recorded personal meetings")
        self.assertIn("CharacterLine+=ReturnReply", dialogue)
        self.assertIn("CharacterLine+=PhysicalReceipt", dialogue)

    def test_no_interactions_are_granted_by_dialogue_composition(self):
        src=NPC.read_text(encoding="utf-8")
        dialogue=src.split("FString AUnmadeNpcCharacter::GetReactionText() const",1)[1].split(
            "void AUnmadeNpcCharacter::ConfigureExpeditionResident",1)[0]
        for forbidden in ("WriteWorldSnapshot(", "SaveGameToSlot(", "ResidentRelationships.Talk("):
            self.assertNotIn(forbidden,dialogue)
        self.assertIn("Event.Evidence == EUnmadeEvidenceKind::Witnessed", dialogue)


if __name__ == "__main__":
    unittest.main()
