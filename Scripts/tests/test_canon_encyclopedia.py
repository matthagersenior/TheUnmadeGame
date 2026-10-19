"""The canon encyclopedia must fail visibly when a future change omits lore."""
import copy
import json
import unittest

from Scripts.validate_canon_encyclopedia import (
    INDEX, ENCYCLOPEDIA, ROOT, expected_records, validate_content,
)


class CanonEncyclopediaTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.records = expected_records(ROOT)
        cls.index = json.loads((ROOT / INDEX).read_text(encoding="utf-8"))
        cls.pages = {
            name: (ROOT / ENCYCLOPEDIA / name).read_text(encoding="utf-8")
            for name in {entry[1] for entry in cls.records.values()}
        }

    def test_complete_current_source_canon(self):
        self.assertEqual([], validate_content(
            self.index, self.records, self.pages, ROOT
        ))

    def test_missing_boss_is_rejected(self):
        x = copy.deepcopy(self.index)
        x["entries"] = [
            e for e in x["entries"] if e["key"] != "boss:boss:original:HollowBell"
        ]
        errors = validate_content(x, self.records, self.pages)
        self.assertTrue(any("unindexed canonical record" in e
                            and "HollowBell" in e for e in errors))

    def test_unpublished_resident_is_rejected(self):
        x = copy.deepcopy(self.index)
        x["entries"] = [
            e for e in x["entries"]
            if e["key"] != "npc:npc.cinderhold.blackslag.scout.001"
        ]
        errors = validate_content(x, self.records, self.pages)
        self.assertTrue(any("unindexed canonical record" in e
                            and "blackslag.scout" in e for e in errors))

    def test_outdated_item_name_is_rejected(self):
        x = copy.deepcopy(self.index)
        item = next(e for e in x["entries"] if e["group"] == "item")
        item["name"] = "INVENTED REWARD"
        errors = validate_content(x, self.records, self.pages)
        self.assertTrue(any("name drift" in e for e in errors))

    def test_stable_id_removed_from_volume_is_rejected(self):
        x = copy.deepcopy(self.pages)
        key = "npc.cinderhold.blackslag.scout.001"
        path = "02_CHARACTERS_NPCS_AND_RELATIONSHIPS.md"
        tick = chr(96)
        x[path] = x[path].replace(f"{tick}{key}{tick}", "REDACTED-ID")
        errors = validate_content(self.index, self.records, x)
        self.assertTrue(any("stable ID missing" in e and key in e
                            for e in errors))

    def test_faction_and_trade_registers_are_indexed(self):
        self.assertEqual(3, self.index["counts"]["faction"])
        self.assertEqual(2, self.index["counts"]["trade_contract"])
        self.assertEqual(88, self.index["counts"]["npc"])
        self.assertEqual(256, self.index["counts"]["codex"])


if __name__ == "__main__":
    unittest.main()
