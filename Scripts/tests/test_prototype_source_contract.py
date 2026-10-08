"""Static evidence that the prototype is actually wired, never a runtime/compile claim."""
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

def source(path):
    return (ROOT / path).read_text(encoding="utf-8")

class PrototypeSourceContractTests(unittest.TestCase):
    def test_memory_has_separate_witness_and_rumor_with_explicit_speaker(self):
        header = source("Source/TheUnmadeGame/Public/NPC/UnmadeMemoryComponent.h")
        impl = source("Source/TheUnmadeGame/Private/NPC/UnmadeMemoryComponent.cpp")
        self.assertIn("bool Witness(", header)
        self.assertIn("bool HearRumor(", header)
        self.assertIn("if (SpeakerId.IsNone()) return false;", impl)
        self.assertIn("Existing.Evidence == EUnmadeEvidenceKind::Rumor", impl)
        self.assertIn("if (Observations.Num() >= 64)", impl)
        self.assertIn("ReadSnapshot", impl)

    def test_runtime_hub_has_5_distinct_citizen_ids_and_ground(self):
        hub = source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertEqual(hub.count("SpawnCitizen(FName("), 5)
        for token in ("npc.merchant.001", "npc.guard.001", "npc.courier.001",
                      "npc.wanderer.001", "npc.archivist.001", "Hub.Ground"):
            self.assertIn(token, hub)
        self.assertIn("RestoreCitizens();", hub)

    def test_player_interactions_are_proximity_and_witness_gated(self):
        p = source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertIn('FindNearbyCitizen(GetWorld(), GetActorLocation(), 260.f)', p)
        self.assertIn("LineTraceTestByChannel", p)
        self.assertIn('TEXT("You have already helped this resident.")', p)
        self.assertIn("UGameplayStatics::SaveGameToSlot", p)

    def test_test_suite_has_provenance_and_repeated_restore(self):
        tests = source("Source/TheUnmadeGame/Private/Tests/NpcAutomationTest.cpp")
        self.assertIn("Unmade.Npc.WitnessVsRumor", tests)
        self.assertIn("Unmade.Npc.Snapshot", tests)
        self.assertIn("Anonymous rumor is refused", tests)
        self.assertIn("Repeat load is idempotent", tests)

if __name__ == "__main__":
    unittest.main()
