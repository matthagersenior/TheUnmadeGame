"""Ability Codex must match source IDs, measured effects and acquisition boundaries."""
from copy import deepcopy
import json
from pathlib import Path
import unittest

from Scripts.validate_ability_codex import validate

ROOT = Path(__file__).resolve().parents[2]


class AbilityCodexContract(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.codex = json.loads((ROOT / "Authoring/ability_codex.json").read_text(encoding="utf-8"))

    def test_all_authored_abilities_match_source(self):
        errors = validate(ROOT, self.codex)
        self.assertEqual(errors, [], "\n".join(errors))
        self.assertGreaterEqual(len(self.codex["records"]), 182)

    def test_every_codex_record_has_a_non_fabricated_requirements_checklist(self):
        for row in self.codex["records"]:
            checklist = row["requirements_checklist"]
            self.assertGreater(len(checklist["items"]), 0)
            for item in checklist["items"]:
                self.assertEqual(item["completion_status"], "evaluated_in_game_only")
                self.assertTrue(item["text"].strip())
        self.assertEqual(self.codex["records"][0]["requirements_checklist"]["verification"],
                         "source_authored_not_runtime_verified")

    def test_fake_checked_checklist_is_rejected(self):
        data = deepcopy(self.codex)
        data["records"][0]["requirements_checklist"]["items"][0]["completion_status"] = "complete"
        self.assertTrue(any("checklist cannot claim an unlocked state" in e
                            for e in validate(ROOT,data)))

    def test_unified_codex_realm_and_progression_registry(self):
        kinds={row["category"] for row in self.codex["records"]}
        for kind in ("realm","journey","experience","personal_quest","campaign_quest",
                     "aftermath_quest","settlement"):
            self.assertIn(kind,kinds)
        self.assertEqual(len([x for x in self.codex["records"] if x["category"]=="realm"]),9)
        self.assertEqual(len([x for x in self.codex["records"] if x["category"]=="settlement"]),3)
        self.assertGreaterEqual(len(self.codex["records"]),243)

    def test_invalid_dependency_rejected(self):
        changed=deepcopy(self.codex)
        row=next(x for x in changed["records"] if x["id"]=="journey.wing.FirstAbsence")
        row["depends_on"].append("realm.nonexistent")
        self.assertTrue(any("invalid Codex dependency" in e for e in validate(ROOT,changed)))

    def test_crafting_material_count_drift_rejected(self):
        changed=deepcopy(self.codex)
        row=next(x for x in changed["records"] if x["id"]=="recipe.workshop.HerbSalve")
        row["rule_contract"]["inputs"][0]["count"]+=1
        self.assertTrue(any("recipe.workshop.HerbSalve" in e for e in validate(ROOT,changed)))

    def test_duplicate_ids_are_rejected(self):
        data = deepcopy(self.codex)
        data["records"].append(deepcopy(data["records"][0]))
        self.assertTrue(any("duplicate stable ID" in e for e in validate(ROOT, data)))

    def test_power_grants_cannot_be_faked_by_observation(self):
        data = deepcopy(self.codex)
        match = next(row for row in data["records"] if row["id"] == "enemy.creature.GriefLantern")
        match["observation"]["automatically_learnable"] = True
        self.assertTrue(any("observation cannot automatically grant" in e
                            for e in validate(ROOT, data)))

    def test_equipment_effect_drift_is_rejected(self):
        data = deepcopy(self.codex)
        row = next(row for row in data["records"] if row["id"] == "item.blade.waybreaker")
        row["effect"]["attack"] += 50
        self.assertTrue(any("item.blade.waybreaker.effect.attack" in e
                            for e in validate(ROOT, data)))

    def test_rite_cost_drift_is_rejected(self):
        data = deepcopy(self.codex)
        row = next(row for row in data["records"] if row["id"] == "rite.UnwriteLaw")
        row["cost"]["strain"] += 1
        self.assertTrue(any("rite.UnwriteLaw.cost.strain" in e
                            for e in validate(ROOT, data)))


if __name__ == "__main__":
    unittest.main()
