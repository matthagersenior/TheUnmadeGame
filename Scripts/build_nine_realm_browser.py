#!/usr/bin/env python3
"""Deterministically build the browser campaign's source-authored scenario data.

This transfers source fields, not canon authority: Unreal quest and SaveGame rules
remain authoritative, and browser runtime never writes an Unreal save.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCES = [
    "Authoring/main_quest_interaction_contracts.json",
    "Authoring/lived_universe_atlas.json",
    "Authoring/frontier_adventures_v14.json",
    "Authoring/world_content_pack.json",
]
OUTPUT = ROOT / "site/nine-realms-data.json"


def read(relative: str) -> dict:
    with (ROOT / relative).open(encoding="utf-8") as stream:
        return json.load(stream)


def build() -> dict:
    contracts, atlas, arcs, content = (read(name) for name in SOURCES)
    worlds = {w["id"]: w for w in atlas["worlds"]}
    frontier = {a["realm"]: a for a in arcs["arcs"]}
    cultures = {c["id"]: c for c in content["realms"]}
    ids = atlas["canonical_order"]
    if len(ids) != 9 or [q["id"] for q in contracts["quests"]] != ids:
        raise ValueError("Canonical quest order has drifted")
    realms = []
    for quest in contracts["quests"]:
        ident = quest["id"]
        w, a, c = worlds[ident], frontier[ident], cultures[ident]
        steps = {s["id"]: s for s in quest["steps"]}
        evidence = w["mystery"]["evidence"]
        side = w["side"]
        if len(evidence) != 3 or len(side["steps"]) != 5:
            raise ValueError(f"Expected three physical clues and five side steps in {ident}")
        realm = {
            "id": ident, "title": w["title"], "city": a["city"],
            "refuge": a["refuge"], "dungeon": a["dungeon"],
            "brief": a["brief"], "boss": a["boss"], "technique": a["technique"],
            "weapon": a["weapon"], "armor": a["armor"],
            "culture": c["culture"], "architecture": w["architecture"],
            "world": w["ordinary"], "hazard": w["sensory"]["hazard_cue"],
            "access": w["sensory"]["access"], "public_principle": c["public_principle"],
            "scene": w["sensory"]["day"],
            "evidence": [
                {"id": clue["id"], "site": clue["site"],
                 "verb": clue["verb"], "discovery": clue["discovery"]}
                for clue in evidence
            ],
            "witness": {
                "id": steps["investigation"]["speaker"],
                "line": steps["investigation"]["line"],
                "arrival": steps["arrival"]["line"],
                "return": steps["return"]["line"],
            },
            "mechanism": {
                "id": steps["mechanism"]["physical_tag"],
                "line": steps["mechanism"]["line"],
            },
            "choices": {
                name: {
                    "objective": steps[name + "_commit"]["objective"],
                    "line": steps[name + "_commit"]["line"],
                    "after": quest["world_after_save"][name],
                }
                for name in ("care", "truth")
            },
            "side": {
                "id": side["id"], "name": side["name"],
                "objective": side["objective"], "steps": side["steps"],
                "branches": side["branches"], "return": side["return_dialogue"],
            },
            "guardian": {
                "clues": a["clues"], "mercy": a["mercy_return"],
                "defeat": a["kill_return"],
            },
            "return_after": {
                "held": quest["world_after_save"]["held"],
                "many": quest["world_after_save"]["many"],
            },
            "architecture_style": c["visual_direction"],
        }
        realms.append(realm)
    return {
        "schema": 1,
        "prototype": "Independent browser gameplay proof. Does not write Unreal saves, modify canon, or assert engine parity.",
        "sources": SOURCES,
        "realms": realms,
    }


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="Fail if generated JSON is stale")
    args = parser.parse_args()
    expected = json.dumps(build(), ensure_ascii=False, indent=2) + "\n"
    if args.check:
        if not OUTPUT.exists() or OUTPUT.read_text(encoding="utf-8") != expected:
            raise SystemExit("Browser realm data drifted; run python Scripts/build_nine_realm_browser.py and commit result")
        print("Nine realm browser data is exact and current")
    else:
        OUTPUT.write_text(expected, encoding="utf-8")
        print(f"Wrote {OUTPUT.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
