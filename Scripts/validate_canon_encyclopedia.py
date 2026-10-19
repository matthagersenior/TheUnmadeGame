#!/usr/bin/env python3
"""Fail-closed coverage validator for THE UNMADE source-backed canon encyclopedia.

Canon content remains owned by the existing JSON and C++ registries.
This script checks that the readable compendium and the structured INDEX
cite every canonical stable ID with a correct name and authority.
It does not certify Unreal compilation, accessibility, play or lore quality.
"""
from __future__ import annotations

import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
ENCYCLOPEDIA = Path("docs/canon/encyclopedia")
INDEX = ENCYCLOPEDIA / "INDEX.json"


def read_json(root: Path, path: str) -> dict:
    return json.loads((root / path).read_text(encoding="utf-8"))


def expected_records(root: Path) -> dict[str, tuple[str, str, str]]:
    """key -> (human name, section filename, authoritative source)."""
    ability = "Authoring/ability_codex.json"
    ledger = "docs/canon/dependency-ledger.json"
    roads = "Authoring/remembered_roads_v13.json"
    world = "Authoring/lived_universe_atlas.json"
    literary = "Authoring/transmedia_storyworld_v1.json"
    ab, dep, npc, realms, lit = (
        read_json(root, path) for path in (ability, ledger, roads, world, literary)
    )
    out: dict[str, tuple[str, str, str]] = {}

    def add(group: str, rows: list, name_key: str, page: str,
            authority: str, id_key: str = "id") -> None:
        for row in rows:
            key = f"{group}:{row[id_key]}"
            if key in out:
                raise ValueError(f"canonical registry duplicates stable key {key}")
            out[key] = (str(row[name_key]), page, authority)

    add("codex", ab["records"], "name", "05_ABILITIES_SKILLS_AND_RULES.md", ability)
    add("item", dep["items"], "name", "04_ARMORY_ITEMS_AND_CRAFTING.md", ledger)
    add("regional_gear", dep["regional_gear"], "name",
        "04_ARMORY_ITEMS_AND_CRAFTING.md", ledger)
    add("boss", dep["bosses"], "name", "03_BESTIARY_BOSSES_AND_GUARDIANS.md", ledger)
    add("quest", dep["quests"], "name", "06_QUESTS_STORY_AND_ENDINGS.md", ledger)
    add("learned_skill", dep["skills"], "name", "06_QUESTS_STORY_AND_ENDINGS.md", ledger)
    add("npc", npc["residents"], "name",
        "02_CHARACTERS_NPCS_AND_RELATIONSHIPS.md", roads)
    add("realm", realms["worlds"], "title", "01_WORLDS_AND_PLACES.md", world)
    add("city", lit["cities"], "name", "01_WORLDS_AND_PLACES.md", literary)
    add("mystery", lit["mystery_register"], "name",
        "07_LORE_MYSTERIES_AND_CONTINUITY.md", literary)
    add("callback", lit["callback_register"], "artifact",
        "07_LORE_MYSTERIES_AND_CONTINUITY.md", literary)
    add("volume", lit["volume_spine"], "public_role",
        "07_LORE_MYSTERIES_AND_CONTINUITY.md", literary)

    # These additional current, source-executable systems are in C++ (not
    # separate JSON masters); compare against exact faction/route IDs there.
    faction_source = "Source/TheUnmadeGame/Public/World/UnmadeFactionChronicleRules.h"
    trade_source = "Source/TheUnmadeGame/Public/World/UnmadeLivingEconomyRules.h"
    faction_text = (root / faction_source).read_text(encoding="utf-8")
    trade_text = (root / trade_source).read_text(encoding="utf-8")
    factions = re.findall(
        r'\{Faction::(Refuge|Archive|Roadbound),"([^"]+)"', faction_text
    )
    if len(factions) != 3 or len({f[0] for f in factions}) != 3:
        raise ValueError("cannot reliably read 3 named faction arcs from C++")
    page = "08_FACTIONS_SOCIAL_SYSTEMS_AND_ECONOMIES.md"
    for faction_id, name in factions:
        out[f"faction:faction.{faction_id}"] = (name, page, faction_source)
    trade_ids = re.findall(r"\{TradeRoute::(\w+),Realm::", trade_text)
    if len(trade_ids) != 2 or len(set(trade_ids)) != 2:
        raise ValueError("cannot reliably read 2 trade routes from C++")
    for trade_id in trade_ids:
        out[f"trade_contract:trade.{trade_id}"] = ("", page, trade_source)

    # 88 source residents versus 82 authored return records: a covered
    # distinction, not an invitation to silently invent return dialogue.
    returned = read_json(root, "Authoring/resident_return_encounters.json")
    resident_ids = {x["id"] for x in npc["residents"]}
    return_ids = {x["id"] for x in returned["residents"]}
    if not return_ids.issubset(resident_ids):
        raise ValueError("resident-return register contains unknown stable NPC IDs")
    if len(return_ids) != returned["registered_count"]:
        raise ValueError("resident-return registered_count is stale")
    return out


def validate_content(index: dict, expected: dict, pages: dict[str, str],
                     root: Path | None = None) -> list[str]:
    errors: list[str] = []
    records = index.get("entries", [])
    if not isinstance(records, list):
        return ["INDEX.entries must be a list"]
    seen: set[str] = set()
    for row in records:
        key = row.get("key")
        if not isinstance(key, str) or not key:
            errors.append("INDEX entry lacks stable group:id key")
            continue
        if key in seen:
            errors.append(f"duplicate encyclopedia index key: {key}")
        seen.add(key)
        if key != f'{row.get("group")}:{row.get("id")}':
            errors.append(f"index key does not match group/id: {key}")
        if key not in expected:
            errors.append(f"noncanonical/orphan index key: {key}")
            continue
        name, page, authority = expected[key]
        if row.get("name") != name and name:
            errors.append(f"name drift: {key} expected {name!r}, got {row.get('name')!r}")
        if row.get("section") != page:
            errors.append(f"wrong chapter: {key} expected {page}")
        if row.get("source") != authority:
            errors.append(f"source provenance drift: {key}")
        if not row.get("canon_class") or not row.get("summary") and row.get("group") == "mystery":
            errors.append(f"missing source/canon classification: {key}")
        if page not in pages:
            errors.append(f"missing encyclopedia volume: {page}")
        elif f'\u0060{row["id"]}\u0060' not in pages[page]:
            errors.append(f"stable ID missing from canonical volume: {key} ({page})")
        if root is not None and not (root / authority).is_file():
            errors.append(f"source file does not exist: {authority}")
    missing = sorted(set(expected) - seen)
    errors += [f"unindexed canonical record: {key}" for key in missing]
    counts: dict[str, int] = {}
    for row in records:
        g = row.get("group")
        if isinstance(g, str):
            counts[g] = counts.get(g, 0) + 1
    if counts != index.get("counts"):
        errors.append(f"INDEX counts drift: records={counts}; declared={index.get('counts')}")
    if not (root / "Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h").is_file() if root is not None else False:
        errors.append("C++ world atlas authority is missing")
    return errors


def audit(root: Path = ROOT) -> list[str]:
    idx = read_json(root, str(INDEX))
    records = expected_records(root)
    pages: dict[str, str] = {}
    for _, page, _ in records.values():
        if page in pages:
            continue
        f = root / ENCYCLOPEDIA / page
        if f.is_file():
            pages[page] = f.read_text(encoding="utf-8")
    return validate_content(idx, records, pages, root)


def main() -> int:
    try:
        errors = audit()
    except (OSError, ValueError, KeyError, TypeError) as exc:
        print(f"CANON ENCYCLOPEDIA ERROR: {exc}", file=sys.stderr)
        return 1
    if errors:
        for line in errors[:100]:
            print(f"CANON ENCYCLOPEDIA DRIFT: {line}", file=sys.stderr)
        print(f"{len(errors)} encyclopedia integrity error(s)", file=sys.stderr)
        return 1
    idx = read_json(ROOT, str(INDEX))
    print(f"PASS: {len(idx['entries'])} indexed canonical records, "
          f"{len(idx['counts'])} families, all matching readable chapters")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
