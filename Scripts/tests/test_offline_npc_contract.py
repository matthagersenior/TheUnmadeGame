"""Checks Unreal wiring; the core NPC decision policy executes as a native C++ CI test."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
def read(path):
    return (ROOT / path).read_text(encoding="utf-8")

class OfflineNpcContract(unittest.TestCase):
    def test_ai_is_disabled_by_default(self):
        config = read("Config/DefaultGame.ini")
        self.assertIn("bEnableLocalModel=false", config)

    def test_npc_actions_ignore_llm_and_still_gate_trade(self):
        cpp = read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        self.assertIn("ChooseNpcAction(Input)", cpp)
        self.assertIn("CanTradeWithPlayer()", cpp)
        self.assertIn("NpcAction::RefuseTrade", cpp)
        self.assertIn("NpcAction::InvestigateAnomaly", cpp)
        self.assertIn("Observation.Evidence == EUnmadeEvidenceKind::Witnessed", cpp)
        self.assertNotIn("Ollama", cpp)
        self.assertNotIn("UUnmadeLocalDialogueSubsystem", cpp)

    def test_all_five_npc_roles_are_configured(self):
        registry = read("Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h")
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for role in ("Merchant", "Guard", "Scholar", "Courier", "Wanderer"):
            self.assertIn("NpcRole::" + role, registry)
        self.assertIn("UnmadeCore::Residents", hub)
        self.assertIn("SpawnCitizen(Resident)", hub)

    def test_repeated_saves_replace_snapshots_not_duplicate(self):
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player = read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertIn("Save->NpcSnapshots.Reset()", hub)
        self.assertIn("Save->NpcSnapshots.Reset()", player)

    def test_native_cpp_policy_runs_in_ci(self):
        workflow = read(".github/workflows/static-checks.yml")
        self.assertIn("Tests/npc/adaptive_npc_test.cpp", workflow)
        self.assertIn("/tmp/unmade-npc-test", workflow)

if __name__ == "__main__":
    unittest.main()
