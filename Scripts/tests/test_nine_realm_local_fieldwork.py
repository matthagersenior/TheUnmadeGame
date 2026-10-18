"""All nine side-quest notices must describe their playable local mechanics."""
import json
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]

class RealmFieldworkContent(unittest.TestCase):
    def test_nine_unique_local_hunt_survey_and_repair_contracts(self):
        rows=json.loads((ROOT/"Authoring/world_identity_v15.json").read_text())["realms"]
        self.assertEqual([r["realm_index"] for r in rows],list(range(9)))
        for index in range(3):
            tasks=[r["side_quests"][index] for r in rows]
            self.assertEqual(len(set(tasks)),9, f"fieldwork index {index} is still generic")
            self.assertTrue(all(len(task)>45 for task in tasks))
        source=(ROOT/"Scripts/build_world_identity.py").read_text()
        self.assertIn("side_quests",source)
        header=(ROOT/"Source/TheUnmadeGame/Public/World/UnmadeIdentityRegistry.h").read_text()
        for label in ("hunt_task","survey_task","repair_task"):
            self.assertIn(f"const char* {label}",header)

    def test_authored_objectives_reach_actual_graybox_interactions_and_journal(self):
        native=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeWorldIdentity.cpp").read_text()
        personal=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp").read_text()
        for name in ("hunt_task","survey_task","repair_task"):
            self.assertIn(f"IdentitySpecs[arc].{name}",native)
            self.assertIn(f"IdentitySpecs[a].{name}",personal)
        self.assertIn("Identity.Arc(a)->chart",personal)
        self.assertIn("Identity.Arc(a)->repair",personal)
        self.assertIn("Identity.Arc(a)->hunt",personal)
        # Full authored stakes are in the physical E/F response and J journal.
        # Tiny world-space signs must not print three unreadable paragraphs.
        self.assertIn('TEXT("LOCAL FIELDWORK\\nE - CLAIM EARNED HUNT',native)
        self.assertIn('J - PURPOSE AND LOCAL CONSEQUENCES',native)
        self.assertNotIn('UTF8_TO_TCHAR(UnmadeCore::IdentitySpecs[a].hunt_task)',native)
        self.assertNotIn('UTF8_TO_TCHAR(UnmadeCore::IdentitySpecs[a].survey_task)',native)
        self.assertIn('IdentitySpecs[arc].hunt_task',native)
        self.assertIn('IdentitySpecs[arc].survey_task',native)
        self.assertIn('IdentitySpecs[arc].repair_task',native)

if __name__=="__main__":
    unittest.main()
