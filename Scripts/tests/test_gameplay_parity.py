"""Cross-realm authored gameplay parity gate.

This verifies data consistency and the minimum authored player-facing contract.
It does not claim that Unreal assets, input, saves, or playability are verified.
"""
import json
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
AUTHORING = ROOT / "Authoring"
CANONICAL_REALMS = [
    "ThreefoldReach", "WidowedRain", "HearthBeneath", "TidalLedger",
    "SkyBelow", "CinderSpine", "HundredUnlived", "OrchardOfKings",
    "FirstAbsence",
]
CITIES = [
    "Crossings", "Saltwake", "Cinderhold", "Drevlach", "Orravane",
    "Vathless", "Eillun", "Tharniv", "Auvren",
]


def load_json(name):
    return json.loads((AUTHORING / name).read_text(encoding="utf-8"))


def normalized_unique(values):
    values = [str(value).strip().casefold() for value in values]
    return all(values) and len(values) == len(set(values))


class GameplayParityTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.playbook = load_json("cinematic_realm_playbook.json")
        cls.experience = load_json("player_experience_v17.json")
        cls.depth = load_json("adventure_depth_v16.json")
        cls.atlas = load_json("lived_universe_atlas.json")
        cls.identity = load_json("world_identity_v15.json")

    def test_all_authoring_tracks_cover_the_same_nine_realms_in_order(self):
        self.assertEqual(self.playbook["canonical_order"], CANONICAL_REALMS)
        self.assertEqual([r["id"] for r in self.playbook["realms"]], CANONICAL_REALMS)
        self.assertEqual([r["id"] for r in self.atlas["worlds"]], CANONICAL_REALMS)
        self.assertEqual([r["arc"] for r in self.experience["realms"]], list(range(9)))
        self.assertEqual([r["realm_index"] for r in self.depth["realms"]], list(range(9)))
        self.assertEqual([r["realm_index"] for r in self.identity["realms"]], list(range(9)))
        for records in (self.experience["realms"], self.depth["realms"], self.identity["realms"]):
            self.assertEqual([r["city"] for r in records], CITIES)

    def test_each_realm_has_distinct_identity_and_production_handoff(self):
        realms = self.playbook["realms"]
        for field in ("signature", "mechanism"):
            with self.subTest(field=field):
                self.assertTrue(normalized_unique(r[field] for r in realms))
        for realm in realms:
            with self.subTest(realm=realm["id"]):
                self.assertGreaterEqual(len(realm["main_verbs"]), 3)
                self.assertTrue(all(v.strip() for v in realm["main_verbs"]))
                self.assertEqual(len(realm["assets"]), 4)
                self.assertTrue(all(a.strip() for a in realm["assets"]))
                self.assertTrue(realm["test"].strip())

    def test_each_realm_has_unique_optional_and_main_quest_content(self):
        experiences = self.experience["realms"]
        depths = self.depth["realms"]
        for field in ("wing", "guardian", "task", "tool", "recipe"):
            with self.subTest(field=field):
                self.assertTrue(normalized_unique(r[field] for r in experiences))
        self.assertTrue(normalized_unique(r["quest"] for r in depths))
        self.assertTrue(normalized_unique(r["boss"] for r in depths))
        for exp, quest in zip(experiences, depths):
            with self.subTest(realm=exp["city"]):
                self.assertEqual(len(exp["scenes"]), 4)
                self.assertTrue(all(s.strip() for s in exp["scenes"]))
                self.assertTrue(exp["reserve_return"].strip())
                self.assertTrue(exp["witness_return"].strip())
                self.assertTrue(quest["option_a"].strip())
                self.assertTrue(quest["option_b"].strip())
                self.assertNotEqual(quest["option_a"], quest["option_b"])
                self.assertTrue(quest["return_a"].strip())
                self.assertTrue(quest["return_b"].strip())
                self.assertTrue(quest["witness_return_a"].strip())
                self.assertTrue(quest["witness_return_b"].strip())

    def test_each_realm_has_discoverable_accessible_local_worldbuilding(self):
        for world in self.atlas["worlds"]:
            with self.subTest(realm=world["id"]):
                for field in ("ordinary", "architecture", "custom", "taboo", "governance",
                              "labor", "trade", "ecology", "weather"):
                    self.assertTrue(world[field].strip(), field)
                sensory = world["sensory"]
                for field in ("day", "night", "hazard_cue", "access"):
                    self.assertTrue(sensory[field].strip(), field)
                mystery = world["mystery"]
                evidence = mystery["evidence"]
                self.assertEqual(len(evidence), 3)
                self.assertEqual(len({e["id"] for e in evidence}), 3)
                self.assertTrue(all(e["site"] and e["verb"] and e["discovery"] for e in evidence))
                side = world["side"]
                self.assertTrue(side["name"].strip())
                self.assertTrue(side["objective"].strip())
                self.assertGreaterEqual(len(side["steps"]), 3)
                self.assertTrue(side["branches"]["care"].strip())
                self.assertTrue(side["branches"]["truth"].strip())
                self.assertTrue(side["return_dialogue"].strip())
                self.assertTrue(side["save"].strip())

    def test_shared_world_identity_defaults_do_not_claim_unique_quest_content(self):
        # The index-level identity table deliberately shares service/reward scaffolding.
        # Realm-specific content is supplied by the quest and world authoring tracks above.
        for record in self.identity["realms"]:
            self.assertTrue(record["mechanism"].strip())
            self.assertTrue(record["hint"].strip())
            self.assertEqual(len(record["side_quests"]), 3)
            self.assertTrue(record["service"].strip())


if __name__ == "__main__":
    unittest.main()
