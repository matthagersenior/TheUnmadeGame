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
