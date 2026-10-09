"""Structural integration checks; the real C++ rules run in Tests/fracture/.
These tests cannot replace Unreal Editor compilation or gameplay verification.
"""
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

def read(path):
    return (ROOT / path).read_text(encoding="utf-8")

class FractureSourceContractTests(unittest.TestCase):
    def test_hub_spawns_explicit_physical_target(self):
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        anchor = read("Source/TheUnmadeGame/Private/Fracture/UnmadeFractureAnchor.cpp")
        self.assertIn("SpawnActor<AUnmadeFractureAnchor>", hub)
        self.assertIn("ApplyFractureState", anchor)
        self.assertIn("SetCollisionEnabled", anchor)
        self.assertIn("SetVisibility", anchor)

    def test_actions_use_tested_core_and_input(self):
        player = read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        keys = read("Config/DefaultInput.ini")
        self.assertIn("FractureModel.Glimpse(", player)
        self.assertIn("FractureModel.Fold(", player)
        self.assertIn("FractureModel.Rewrite(", player)
        self.assertIn("PendingRewriteExpiresAt = Now + 6.0", player)
        self.assertIn("FractureModel.Restore(Previous);", player)
        for name in ("AnomalyPulse", "FoldReality", "RewriteOpen", "RewriteSealed"):
            self.assertIn("ActionName=\"" + name + "\"", keys)
            self.assertIn('BindAction("' + name + '"', player)

    def test_prototype_saves_do_not_overwrite_other_components(self):
        save = read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player = read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for name in ("WorldVariantId", "PlayerStrain", "NpcSnapshots",
                     "bHasFractureSnapshot", "DoesSaveGameExist"):
            self.assertIn(name, save)
        self.assertIn("UUnmadePrototypeSave::LoadOrCreate()", hub)
        self.assertIn("UUnmadePrototypeSave::LoadOrCreate()", player)
        self.assertIn("SaveGameToSlot", player)

    def test_executable_cpp_test_exists_in_ci(self):
        workflow = read(".github/workflows/static-checks.yml")
        self.assertIn("g++ -std=c++17", workflow)
        self.assertIn("Tests/fracture/fracture_core_test.cpp", workflow)
        cpp = read("Tests/fracture/fracture_core_test.cpp")
        for requirement in ("NeedsConfirmation", "AlreadyCommitted",
                            "NotEnoughStability", "Restore(", "UnsupportedTarget"):
            self.assertIn(requirement, cpp)

if __name__ == "__main__":
    unittest.main()
