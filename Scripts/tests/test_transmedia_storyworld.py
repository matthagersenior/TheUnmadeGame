"""Protect recognisable people, place continuity and genuine unsolved secrets."""
from __future__ import annotations
import copy
import importlib.util
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location("storyworld_gate", ROOT / "Scripts/validate_transmedia_storyworld.py")
S = importlib.util.module_from_spec(spec)
spec.loader.exec_module(S)


class MultiMediaContinuity(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.a = S.load(S.ATLAS)
        cls.l = S.load(S.WORLDS)
        cls.r = S.load(S.RESIDENTS)
        cls.w = S.load(S.REALM_PACK)

    def test_full_ten_volume_universe(self):
        self.assertTrue(S.verify(self.a, self.l, self.r, self.w))
        self.assertEqual(len(self.a["cities"]), 11)
        self.assertEqual(len(self.a["character_dossiers"]), 13)
        self.assertEqual(len(self.a["callback_register"]), 13)
        self.assertEqual(len(self.a["mystery_register"]), 9)

    def test_does_not_rename_existing_acting_resident(self):
        modified = copy.deepcopy(self.a)
        modified["character_dossiers"][0]["display"] = "Random Generic Matron"
        with self.assertRaises(ValueError):
            S.verify(modified, self.l, self.r, self.w)

    def test_cannot_invent_geo_or_secretly_move_people(self):
        modified = copy.deepcopy(self.a)
        modified["cities"][3]["name"] = "New Atlantis"
        with self.assertRaises(ValueError):
            S.verify(modified, self.l, self.r, self.w)
        modified = copy.deepcopy(self.a)
        modified["character_dossiers"][0]["relationships"][1][0] = "npc.missing.999"
        with self.assertRaises(ValueError):
            S.verify(modified, self.l, self.r, self.w)

    def test_mystery_resolution_requires_a_separate_authorized_commit(self):
        modified = copy.deepcopy(self.a)
        modified["mystery_register"][7]["answers_status"] = "SOLVED"
        with self.assertRaises(ValueError):
            S.verify(modified, self.l, self.r, self.w)
        modified = copy.deepcopy(self.a)
        modified["historical_protection"]["preserve_two_postboss_mornings"] = False
        with self.assertRaises(ValueError):
            S.verify(modified, self.l, self.r, self.w)

    def test_callbacks_cannot_teleport_provenance(self):
        modified = copy.deepcopy(self.a)
        modified["callback_register"][0]["mystery_id"] = "MYST.NOT_REGISTERED"
        with self.assertRaises(ValueError):
            S.verify(modified, self.l, self.r, self.w)
        modified = copy.deepcopy(self.a)
        modified["transmedia_packages"][0]["required_callbacks"][0] = "CALL.UNKNOWN"
        with self.assertRaises(ValueError):
            S.verify(modified, self.l, self.r, self.w)

    def test_lore_files_and_ci(self):
        ci = (ROOT / ".github/workflows/static-checks.yml").read_text()
        pc = (ROOT / "Scripts/first_pc_build_and_test.ps1").read_text()
        self.assertIn("validate_transmedia_storyworld.py", ci)
        self.assertIn("validate_transmedia_storyworld.py", pc)
        canon = (ROOT / "docs/lore/living-world-continuity.md").read_text()
        self.assertIn("The Unanswered Atlas", canon)
        self.assertIn("MYST.01", canon)


if __name__ == "__main__":
    unittest.main()
