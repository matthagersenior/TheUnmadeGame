"""NPC actor movement wiring; compiled native tests separately assert the steering outcomes."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(path): return (ROOT/path).read_text(encoding="utf-8")

class NpcMotionContract(unittest.TestCase):
    def test_unreal_actor_moves_without_local_llm(self):
        npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        for token in ("AUnmadeNpcCharacter::Tick(", "DecideForPlayer(bPlayerNearby)",
                      "UnmadeCore::SteerNpc(", "AddActorWorldOffset(Offset, true)",
                      "NpcAction::InvestigateAnomaly", "NpcAction::ResearchAnomaly",
                      "NpcAction::VerifyRumor", "NpcAction::DeliverMessage"):
            self.assertIn(token,npc)
        self.assertNotIn("Ollama",npc)
        self.assertNotIn("RequestDialogue",npc)

    def test_bounded_displacement_has_native_tests(self):
        workflow=read(".github/workflows/static-checks.yml")
        self.assertIn("Tests/npc/npc_motion_test.cpp",workflow)
        self.assertIn("/tmp/unmade-motion-test",workflow)
        motion=read("Source/TheUnmadeGame/Public/NPC/UnmadeNpcMotionRules.h")
        self.assertIn("std::min(deltaSeconds, 0.25)",motion)
        self.assertIn("NpcMotion::Retreat",motion)
        self.assertIn("std::isfinite",motion)

if __name__=="__main__":
    unittest.main()
