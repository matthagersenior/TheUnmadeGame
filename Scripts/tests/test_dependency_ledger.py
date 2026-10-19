"""Source-backed canon tests: demonstrate that CI rejects broken dependencies."""
import copy
import json
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from validate_dependency_ledger import PATH, validate


class DependencyLedgerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.canon = json.loads(PATH.read_text(encoding="utf-8"))

    def test_current_ledger_matches_game_sources(self):
        self.assertEqual([], validate(self.canon))

    def test_new_or_missing_item_fails_closed(self):
        modified = copy.deepcopy(self.canon)
        modified["items"].pop()
        self.assertTrue(any("ItemCatalog drift" in e for e in validate(modified)))

    def test_fake_loot_source_is_rejected(self):
        modified = copy.deepcopy(self.canon)
        modified["items"][0]["acquisition"] = [{"method":"drop","via":"unverified"}]
        self.assertTrue(any("item acquisition" in e for e in validate(modified)))

    def test_missing_boss_is_rejected(self):
        modified = copy.deepcopy(self.canon)
        modified["bosses"].pop()
        self.assertTrue(any("boss encounters drift" in e for e in validate(modified)))

    def test_missing_quest_is_rejected(self):
        modified = copy.deepcopy(self.canon)
        modified["quests"].pop()
        self.assertTrue(any("major quest registry drift" in e for e in validate(modified)))

    def test_quest_without_persistent_consequence_is_rejected(self):
        modified = copy.deepcopy(self.canon)
        modified["quests"][0]["aftermath"] = []
        self.assertTrue(any("missing or empty aftermath" in e for e in validate(modified)))

    def test_ability_without_learning_quest_is_rejected(self):
        modified = copy.deepcopy(self.canon)
        modified["skills"][0]["learning_quest"] = "quest:nonexistent"
        self.assertTrue(any("no concrete learning source/quest" in e for e in validate(modified)))

    def test_unearned_relic_cannot_be_misrepresented(self):
        modified = copy.deepcopy(self.canon)
        item = next(item for item in modified["items"] if item["id"] == "AtlasOfAbsence")
        item["availability"] = "earnable"
        self.assertTrue(any("item AtlasOfAbsence" in e for e in validate(modified)))

    def test_new_unexplained_reward_is_rejected(self):
        modified = copy.deepcopy(self.canon)
        modified["bosses"][0]["rewards"].append("item:invented_legendary")
        self.assertTrue(any("dangling reward" in e for e in validate(modified)))


if __name__ == "__main__":
    unittest.main()
