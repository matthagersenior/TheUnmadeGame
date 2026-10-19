import copy
import unittest
from Scripts.check_canon_change import (
    check_changes, requires_registration, validate_manifest,
)

IDX = {"entries": [{"key": "codex:rite.X"}, {"key": "npc:npc.sample"}]}
VALID = {"schema": 1, "entries": [{
    "path": "Source/TheUnmadeGame/Public/World/NewMechanic.h",
    "classification": "implementation",
    "codex_keys": ["codex:rite.X"]
}]}


class CanonChangeGateTests(unittest.TestCase):
    def test_new_source_without_canon_mapping_is_rejected(self):
        errors = check_changes(
            [("A", "Source/TheUnmadeGame/Public/World/UnregisteredPower.h")],
            VALID, IDX,
        )
        self.assertTrue(any("UNREGISTERED GAME ADDITION" in e for e in errors))

    def test_new_art_asset_is_not_ignored(self):
        errors = check_changes(
            [("A", "Content/Characters/NewBoss.uasset")], VALID, IDX)
        self.assertTrue(any("UNREGISTERED GAME ADDITION" in e for e in errors))

    def test_new_source_bound_to_real_codex_id_passes(self):
        self.assertEqual([], check_changes(
            [("A", "Source/TheUnmadeGame/Public/World/NewMechanic.h")],
            VALID, IDX))

    def test_noncanonical_infrastructure_needs_a_real_reason(self):
        entry = copy.deepcopy(VALID)
        entry["entries"][0]["classification"] = "noncanonical_infrastructure"
        entry["entries"][0]["codex_keys"] = []
        entry["entries"][0]["rationale"] = "minor"
        self.assertTrue(validate_manifest(entry, IDX))
        entry["entries"][0]["rationale"] = (
            "Build-only compiler adapter: no new player-facing game behavior."
        )
        self.assertEqual([], validate_manifest(entry, IDX))

    def test_unknown_canon_key_is_rejected(self):
        x = copy.deepcopy(VALID)
        x["entries"][0]["codex_keys"] = ["codex:rite.Nonexistent"]
        self.assertTrue(any("unknown codex key" in e
                            for e in validate_manifest(x, IDX)))

    def test_modifying_a_registry_requires_readable_volume_sync(self):
        path = "Authoring/ability_codex.json"
        errors = check_changes([("M", path)], VALID, IDX)
        self.assertTrue(any("STALE CANON" in e for e in errors))
        self.assertEqual([], check_changes([
            ("M", path), ("M", "docs/canon/encyclopedia/05_ABILITIES_SKILLS_AND_RULES.md")
        ], VALID, IDX))

    def test_faction_and_economy_authority_require_codex_sync(self):
        path = "Source/TheUnmadeGame/Public/World/UnmadeLivingEconomyRules.h"
        self.assertTrue(check_changes([("M", path)], VALID, IDX))
        self.assertEqual([], check_changes([
            ("M", path),
            ("M", "docs/canon/encyclopedia/08_FACTIONS_SOCIAL_SYSTEMS_AND_ECONOMIES.md"),
        ], VALID, IDX))

    def test_literary_new_entries_are_covered(self):
        self.assertTrue(requires_registration("docs/lore/NEW_CANON.md"))
        self.assertFalse(requires_registration("docs/lore/README.txt"))

    def test_generated_files_and_tests_are_exempt(self):
        self.assertFalse(requires_registration(
            "Authoring/generated/npcs_unreal.csv"))
        self.assertFalse(requires_registration("Tests/world/new_rule_test.cpp"))

    def test_duplicate_registration_rejected(self):
        m = copy.deepcopy(VALID)
        m["entries"].append(m["entries"][0])
        self.assertTrue(any("duplicate covered source" in e
                            for e in validate_manifest(m, IDX)))


if __name__ == "__main__":
    unittest.main()
