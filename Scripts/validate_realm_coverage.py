#!/usr/bin/env python3
"""Validate the nine-realm source coverage ledger against canonical inputs.

This is a data consistency gate. It does not promote source evidence to an
Unreal, PIE, or packaged-game claim.
"""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LEDGER = ROOT / "docs/qa/realm-coverage-ledger.json"
INDEX = ROOT / "docs/canon/encyclopedia/INDEX.json"
FRONTIER = ROOT / "Authoring/frontier_adventures_v14.json"
DEPTH = ROOT / "Authoring/adventure_depth_v16.json"
WORLD = ROOT / "Authoring/world_content_pack.json"
DIMENSIONS = {
    "story", "locations", "quests", "npcs", "enemies", "bosses",
    "equipment", "abilities", "environment", "consequences", "tests",
}
EXPECTED_TESTS = {
    "Scripts/tests/test_gameplay_parity.py",
    "Scripts/tests/test_frontier_aftermath_wiring.py",
    "Tests/world/expedition_test.cpp",
    ".github/workflows/static-checks.yml",
}


def read_json(path):
    return json.loads(path.read_text(encoding="utf-8"))


def records(index, realm, group):
    return [entry for entry in index["entries"]
            if entry.get("realm") == realm and entry.get("group") == group]


def stable_keys(entries):
    return [entry["key"] for entry in entries]


def validate():
    ledger = read_json(LEDGER)
    index = read_json(INDEX)
    frontier = read_json(FRONTIER)
    depth = read_json(DEPTH)
    world = read_json(WORLD)
    arcs = frontier["arcs"]
    order = [arc["realm"] for arc in arcs]

    assert ledger["schema_version"] == 1
    assert ledger["realm_order"] == order, "realm order must match canonical frontier source"
    assert len(order) == 9 and len(set(order)) == 9, "all nine stable realm IDs are required"
    assert set(ledger["dimensions"]) == DIMENSIONS, "coverage dimensions changed"
    assert [realm["id"] for realm in ledger["realms"]] == order
    assert "do not imply" in ledger["scope_note"]
    assert "Unreal" in ledger["scope_note"]

    world_by_id = {realm["id"]: realm for realm in world["realms"]}
    optional_by_index = {realm["realm_index"]: realm for realm in depth["realms"]}
    total = {"quests": 0, "bosses": 0, "regional_gear": 0, "learned_skill": 0}
    evidence_paths = set()

    for realm_index, (realm, arc) in enumerate(zip(ledger["realms"], arcs)):
        assert realm["id"] == arc["realm"]
        assert realm["culture"] == world_by_id[arc["realm"]]["culture"]
        coverage = realm["coverage"]
        assert set(coverage) == DIMENSIONS, f'{arc["realm"]}: missing or extra dimension'

        story = coverage["story"]
        optional = optional_by_index[realm_index]
        assert story["main_arc"] == arc["quest"]
        assert story["optional_arc"] == optional["quest"]
        assert story["status"] == "source-authored"

        locations = coverage["locations"]
        for field in ("city", "frontier", "refuge", "dungeon"):
            assert locations[field] == arc[field], f'{arc["realm"]}: {field} drift'
        assert locations["status"] == "source-authored"

        expected_groups = {
            "quests": "quest",
            "bosses": "boss",
            "equipment": "regional_gear",
            "abilities": "learned_skill",
        }
        for dimension, group in expected_groups.items():
            source_entries = records(index, arc["realm"], group)
            block = coverage[dimension]
            total[group] += len(source_entries)
            if dimension == "equipment":
                actual = [entry["key"] for entry in block["entries"]]
            elif dimension == "bosses" or dimension == "abilities":
                actual = [entry["key"] for entry in block["stable_ids"]]
            else:
                actual = block["stable_ids"]
            assert actual == stable_keys(source_entries), f'{arc["realm"]}: {dimension} IDs drift'
            assert block["count"] == len(source_entries), f'{arc["realm"]}: {dimension} count drift'

        people = coverage["npcs"]
        assert people["names"] == arc["people"] and people["count"] == len(arc["people"])
        assert people["scope"].startswith("Five named frontier-roster people")

        consequences = coverage["consequences"]
        assert consequences["kill_return"] == arc["kill_return"]
        assert consequences["mercy_return"] == arc["mercy_return"]
        assert consequences["optional_arc"] == optional["quest"]
        for key in ("enemies", "environment", "consequences"):
            assert coverage[key].get("engine_verified") is False, (
                f'{arc["realm"]}: source evidence cannot claim engine verification'
            )

        tests = set(coverage["tests"]["evidence"])
        assert tests == EXPECTED_TESTS, f'{arc["realm"]}: test evidence changed'
        assert "no Unreal Editor" in coverage["tests"]["scope"]
        for block in coverage.values():
            evidence_paths.update(block.get("evidence", []))

    for path in evidence_paths:
        assert (ROOT / path).is_file(), f"missing evidence path: {path}"
    assert total == {
        "quests": 86,
        "bosses": 39,
        "regional_gear": 18,
        "learned_skill": 10,
    }, f"realm-attributed index totals changed: {total}"

    print(
        "Realm coverage ledger passed: 9/9 realms; "
        "86 realm-attributed quest groups, 39 bosses, 18 regional gear records, "
        "10 realm-attributed learned skills. Evidence remains source-level; "
        "Unreal Editor/PIE/package verification is unclaimed."
    )


if __name__ == "__main__":
    validate()
