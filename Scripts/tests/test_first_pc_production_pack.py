"""Production-pack regression tests: source coverage, ID integrity and output stability.

No Unreal build or playable game is implied by these checks.
"""
from __future__ import annotations

import copy
import importlib.util
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location(
    "unmade_first_pc_production_pack", ROOT / "Scripts/build_first_pc_production_pack.py")
PACK = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PACK)


class FirstPCProductionPackTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.sources = PACK.read_sources(ROOT)

    def test_every_source_linked_record_has_a_staging_work_order(self):
        receipt, files = PACK.build(ROOT)
        self.assertEqual(receipt["realm_count"], 9)
        self.assertEqual(
            receipt["summary"],
            {"REALM": 9, "CITY": 11, "MAIN_STEP": 54, "CLUE": 27,
             "SIDE_STORY": 9, "RESIDENT": 82, "QUEST": 99,
             "BOSS": 39, "ITEM": 60, "SKILL": 23, "CODEX": 256},
        )
        self.assertEqual(receipt["total_work_orders"], 669)
        self.assertEqual(len(files), 12)
        self.assertIn("UNREAL_NOT_VERIFIED", files["work-orders.csv"])
        self.assertIn("no game assets, Unreal compilation", files["README.md"])

    def test_nine_realm_dossiers_have_recoveries_and_saved_aftermath(self):
        _, files = PACK.build(ROOT)
        for realm in PACK.REALMS:
            with self.subTest(realm=realm):
                dossier = files["realm-" + realm + ".md"]
                self.assertIn("## Main chapter: six physically staged beats", dossier)
                main_chapter = dossier.split("## Main chapter: six physically staged beats", 1)[1].split("## Saved outcomes", 1)[0]
                self.assertEqual(main_chapter.count("**Recovery:**"), 6)
                self.assertIn("## Saved outcomes", dossier)
                self.assertIn("## Required Unreal acceptance", dossier)

    def test_auvren_first_trial_does_not_leak_finale_or_endings(self):
        first_absence = next(
            quest for quest in self.sources["campaign"]["quests"]
            if quest["id"] == "FirstAbsence"
        )
        steps = {step["id"]: step for step in first_absence["steps"]}
        mechanism = steps["mechanism"]
        return_step = steps["return"]
        canonical = (ROOT / "docs/canon/encyclopedia/06_QUESTS_STORY_AND_ENDINGS.md").read_text()

        self.assertEqual(mechanism["physical_tag"], "Absence.Trial.OriginAnchor")
        texts = [*first_absence["player_verbs"], mechanism["objective"],
                 return_step["objective"], return_step["line"]]
        for text in texts:
            lowered = text.lower()
            self.assertNotIn("nhal-vey", lowered)
            self.assertNotIn("held morning", lowered)
            self.assertNotIn("many mornings", lowered)
        self.assertIn("threshold", mechanism["objective"].lower())
        self.assertIn("threshold", return_step["objective"].lower())
        self.assertIn("**objective:** Open one real threshold with stable fallback travel", canonical)
        self.assertIn("The Held Morning", canonical)
        self.assertIn("The Many Mornings", canonical)

    def test_source_only_determinism(self):
        a, files_a = PACK.build(ROOT)
        b, files_b = PACK.build(ROOT)
        self.assertEqual(a, b)
        self.assertEqual(files_a, files_b)

    def test_missing_quest_boss_reference_fails_closed(self):
        data = copy.deepcopy(self.sources)
        data["ledger"]["quests"][0]["boss"] = "boss:unknown"
        with self.assertRaisesRegex(ValueError, "references missing boss"):
            PACK.verify(data)

    def test_duplicate_canonical_id_fails_closed(self):
        data = copy.deepcopy(self.sources)
        data["ledger"]["items"][1]["id"] = data["ledger"]["items"][0]["id"]
        with self.assertRaisesRegex(ValueError, "Duplicate or empty stable ID"):
            PACK.verify(data)

    def test_missing_manual_retry_contract_fails_closed(self):
        data = copy.deepcopy(self.sources)
        data["campaign"]["quests"][0]["steps"][0]["recovery"] = ""
        with self.assertRaisesRegex(ValueError, "Unstaged main quest field"):
            PACK.verify(data)

    def test_no_duplicate_work_order_identities(self):
        orders = PACK.make_orders(self.sources)
        identities = {(row["type"], row["id"]) for row in orders}
        self.assertEqual(len(identities), len(orders))


if __name__ == "__main__":
    unittest.main()
