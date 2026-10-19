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

    def test_codex_passages_have_no_free_shortcuts(self):
        routes=[x for x in self.codex["records"] if x["category"]=="realm_passage"]
        self.assertEqual(len(routes),13)
        for edge in routes:
            self.assertIn(edge["rule_contract"]["minimum_attunement"],range(5))
            self.assertEqual(len(edge["depends_on"]),2)
            self.assertTrue(edge["rule_contract"]["bidirectional"])
        changed=deepcopy(self.codex)
        ref=next(x for x in changed["records"] if x["id"].startswith("passage.11."))
        ref["rule_contract"]["minimum_attunement"]=0
        self.assertTrue(any("passage.11." in e for e in validate(ROOT,changed)))

    def test_expedition_reward_and_mercy_are_distinct_and_source_owned(self):
        journeys=[x for x in self.codex["records"] if x["id"].startswith("journey.expedition.")]
        self.assertEqual(len(journeys),9)
        for journey in journeys:
            contract=journey["rule_contract"]
            self.assertEqual(len(contract["people"]),5)
            self.assertEqual(len(contract["clues"]),3)
            self.assertNotEqual(contract["kill_return"],contract["mercy_return"])
        changed=deepcopy(self.codex)
        row=next(x for x in changed["records"] if x["id"]=="journey.expedition.ThreefoldReach")
        row["rule_contract"]["mercy_return"]=row["rule_contract"]["kill_return"]
        self.assertTrue(any("mercy and combat must differ" in issue for issue in validate(ROOT,changed)))

    def test_experience_wing_room_grid_and_scenes_are_source_owned(self):
        records=[x for x in self.codex["records"] if x["id"].startswith("journey.wing.")]
        self.assertEqual(len(records),9)
        for row in records:
            rule=row["rule_contract"]
            self.assertEqual(len(rule["scenes"]),4)
            self.assertEqual(len(rule["wing_grid"]),3)
            self.assertTrue(all(len(pair)==2 for pair in rule["wing_grid"]))
            self.assertNotEqual(rule["reserve_return"],rule["witness_return"])
        changed=deepcopy(self.codex)
        row=next(x for x in changed["records"] if x["id"]=="journey.wing.ThreefoldReach")
        row["rule_contract"]["scenes"]=[]
        self.assertTrue(any("four scenes" in issue for issue in validate(ROOT,changed)))

    def test_unanswered_road_chapters_are_codex_owned_and_ordered(self):
        rows=[r for r in self.codex["records"] if r["id"].startswith("quest.campaign.")]
        self.assertEqual(len(rows),9)
        self.assertEqual({r["rule_contract"]["source_index"] for r in rows},set(range(9)))
        for row in rows:
            self.assertTrue(row["rule_contract"]["witnessed_discovery"])
            self.assertTrue(row["rule_contract"]["next_direction"])
        changed=deepcopy(self.codex)
        row=next(r for r in changed["records"] if r["id"]=="quest.campaign.FirstAbsence")
        row["rule_contract"]["source_index"]=0
        self.assertTrue(any("changed the authored road order" in issue for issue in validate(ROOT,changed)))

    def test_personal_quests_have_two_witnessed_and_mutually_exclusive_outcomes(self):
        records=[x for x in self.codex["records"] if x["id"].startswith("quest.personal.")]
        self.assertEqual(len(records),9)
        for row in records:
            c=row["rule_contract"]
            self.assertNotEqual(c["option_a"],c["option_b"])
            self.assertNotEqual(c["return_a"],c["return_b"])
            self.assertNotEqual(c["witness_return_a"],c["witness_return_b"])
        changed=deepcopy(self.codex)
        row=next(x for x in changed["records"] if x["id"]=="quest.personal.ThreefoldReach")
        row["rule_contract"]["option_a"]=row["rule_contract"]["option_b"]
        self.assertTrue(any("distinct saved social outcomes" in issue for issue in validate(ROOT,changed)))

    def test_realm_aftermaths_preserve_two_physical_and_witnessed_consequences(self):
        rows=[x for x in self.codex["records"] if x["id"].startswith("quest.aftermath.")]
        self.assertEqual(len(rows),9)
        for row in rows:
            contract=row["rule_contract"]
            self.assertNotEqual(contract["care_evidence"],contract["truth_evidence"])
            self.assertNotEqual(contract["care_consequence"],contract["truth_consequence"])
            self.assertNotEqual(contract["care_witness"],contract["truth_witness"])
        changed=deepcopy(self.codex)
        row=next(x for x in changed["records"] if x["id"]=="quest.aftermath.WidowedRain")
        row["rule_contract"]["truth_consequence"]=row["rule_contract"]["care_consequence"]
        self.assertTrue(any("distinct source-backed outcomes required" in msg for msg in validate(ROOT,changed)))

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
