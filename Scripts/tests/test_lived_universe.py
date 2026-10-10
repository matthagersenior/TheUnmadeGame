"""Contract checks for the pre-engine living-universe expansion."""
from __future__ import annotations

import copy
import importlib.util
import json
import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
MODULE_PATH = ROOT / "Scripts/validate_lived_universe.py"
SPEC = importlib.util.spec_from_file_location("lived_universe", MODULE_PATH)
L = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(L)


class LivedUniverseAuthoring(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.master = json.loads((ROOT / "Authoring/lived_universe_atlas.json").read_text(encoding="utf-8"))
        cls.canon = json.loads((ROOT / "Authoring/cinematic_realm_playbook.json").read_text(encoding="utf-8"))

    def test_every_realm_has_life_mystery_and_nonmandatory_challenge(self):
        self.assertTrue(L.verify(self.master, self.canon))
        self.assertEqual(len(self.master["worlds"]), 9)
        self.assertEqual(sum(len(x["mystery"]["evidence"]) for x in self.master["worlds"]), 27)
        self.assertEqual(sum(len(x["routine"]) for x in self.master["worlds"]), 18)
        self.assertEqual(len({x["side"]["id"] for x in self.master["worlds"]}), 9)
        self.assertIn("optional", self.master["universal_rules"]["progression"].lower())
        self.assertIn("not", self.master["status"].lower())
        self.assertIn("unreal", self.master["status"].lower())

    def test_90_editor_orders_are_deterministic_non_authoritative(self):
        expected = L.generate(self.master)
        actual = json.loads((ROOT / "Authoring/generated/lived_universe_scene_manifest.json").read_text(encoding="utf-8"))
        self.assertEqual(actual, expected)
        self.assertEqual(len(expected["work_orders"]), 90)
        self.assertEqual(expected["counts"], {
            "physical_evidence": 27,
            "resident_daily_scene": 18,
            "optional_side_quest": 9,
            "art_audio_work_package": 36,
            "total": 90,
        })
        self.assertTrue(all(not o["collision"] and not o["gameplay_authority"]
                            and o["engine_status"] == "not_built"
                            for o in expected["work_orders"]))
        self.assertEqual(len({o["actor_label"] for o in expected["work_orders"]}), 90)

    def test_duplicates_and_missing_actions_are_rejected(self):
        data = copy.deepcopy(self.master)
        data["worlds"][3]["mystery"]["evidence"][1]["id"] = data["worlds"][3]["mystery"]["evidence"][0]["id"]
        with self.assertRaises(ValueError):
            L.verify(data, self.canon)
        data = copy.deepcopy(self.master)
        del data["worlds"][5]["routine"][0]["recovery"]
        with self.assertRaises(ValueError):
            L.verify(data, self.canon)
        data = copy.deepcopy(self.master)
        data["worlds"][8]["side"]["steps"].pop()
        with self.assertRaises(ValueError):
            L.verify(data, self.canon)
        data = copy.deepcopy(self.master)
        data["worlds"][1]["side"]["branches"]["truth"] = data["worlds"][1]["side"]["branches"]["care"]
        with self.assertRaises(ValueError):
            L.verify(data, self.canon)

    def test_canon_drift_and_false_engine_credits_are_rejected(self):
        data = copy.deepcopy(self.master)
        data["canonical_order"][1] = "InventedRealm"
        with self.assertRaises(ValueError):
            L.verify(data, self.canon)
        data = copy.deepcopy(self.master)
        data["status"] = "fully compiled and launched in the Unreal Editor"
        with self.assertRaises(ValueError):
            L.verify(data, self.canon)

    def test_cli_and_first_pc_preflight_run_the_gate(self):
        call = subprocess.run([sys.executable, str(MODULE_PATH), "--check"],
                              capture_output=True, text=True)
        self.assertEqual(call.returncode, 0, call.stdout + call.stderr)
        self.assertIn("90 work orders", call.stdout)
        pc = (ROOT / "Scripts/first_pc_build_and_test.ps1").read_text(encoding="utf-8")
        ci = (ROOT / ".github/workflows/static-checks.yml").read_text(encoding="utf-8")
        self.assertIn("validate_lived_universe.py", pc)
        self.assertIn("validate_lived_universe.py", ci)


if __name__ == "__main__":
    unittest.main()
