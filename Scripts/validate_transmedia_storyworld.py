"""Continuity/identity gate for the multi-medium Unmade storyworld.

Editorial preproduction only. Game quest/save rules and actual runtime scenes
remain authoritative; this is not a UE content importer.
"""
from __future__ import annotations
import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ATLAS = ROOT / "Authoring/transmedia_storyworld_v1.json"
WORLDS = ROOT / "Authoring/lived_universe_atlas.json"
RESIDENTS = ROOT / "Authoring/resident_return_encounters.json"
REALM_PACK = ROOT / "Authoring/world_content_pack.json"
REALM_IDS = (
    "ThreefoldReach", "WidowedRain", "HearthBeneath", "TidalLedger",
    "SkyBelow", "CinderSpine", "HundredUnlived", "OrchardOfKings",
    "FirstAbsence",
)
MINIMUM = {"cities": 11, "character_dossiers": 13, "mystery_register": 9,
           "callback_register": 13, "volume_spine": 10}


def load(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def filled(value) -> bool:
    return isinstance(value, str) and bool(value.strip())


def require(record: dict, keys: tuple[str, ...], owner: str) -> None:
    for key in keys:
        if not filled(record.get(key)):
            raise ValueError(f"{owner}.{key}: missing authored specific text")


def unique(records: list[dict], key: str, owner: str) -> set[str]:
    ids = [r.get(key) for r in records]
    if not all(filled(x) for x in ids) or len(set(ids)) != len(ids):
        raise ValueError(f"{owner}: duplicate or missing {key}")
    return set(ids)


def verify(a: dict, lived: dict, people: dict, world: dict) -> bool:
    if a.get("schema_version") != 1:
        raise ValueError("Unsupported transmedia storyworld schema")
    if "not" not in a.get("status", "").lower() or "unreal" not in a["status"].lower():
        raise ValueError("Production status must not claim completed Unreal scenes")
    if not all(a.get("historical_protection", {}).get(flag) for flag in
               ("stable_realm_ids", "stable_npc_ids", "preserve_source_quest_state",
                "preserve_two_postboss_mornings")):
        raise ValueError("Historical game identity/save protection removed")
    cities, cast, mysteries, callbacks, volumes = [
        a.get(k, []) for k in MINIMUM
    ]
    for key, count in MINIMUM.items():
        if len(a.get(key, [])) < count:
            raise ValueError(f"Missing {key} records: expected >= {count}")
    authoritative_realms = [x["id"] for x in lived["worlds"]]
    if authoritative_realms != list(REALM_IDS) or [x["id"] for x in world["realms"]] != list(REALM_IDS):
        raise ValueError("Canonical nine realm order changed")
    allowed_city_realm = {
        (w["id"], place) for w in lived["worlds"]
        for place in w["settlements"]
    }
    ids = unique(cities, "id", "cities")
    if len({x["name"] for x in cities}) != len(cities):
        raise ValueError("Two distinct cities share a public name")
    for city in cities:
        require(city, ("id", "realm", "name", "recognition", "skyline", "sound",
                       "social_rule", "civic_power", "personal_stakes", "anomaly",
                       "visual_icon", "adaption_rule"), "city")
        if (city["realm"], city["name"]) not in allowed_city_realm:
            raise ValueError(f"Invented city or wrong region: {city['name']}")
        if not city["id"].startswith("CITY."):
            raise ValueError("City stable key lost")
    authoritative_people = {p["id"]: p for p in people["residents"]}
    person_ids = unique(cast, "id", "character_dossiers")
    for person in cast:
        require(person, ("id", "display", "anchor", "face", "public_aim", "private_need",
                         "fault", "wound", "voice", "entrance", "lie", "contradiction",
                         "reappearance", "open_hook", "continuity_class"), "person")
        if person["id"] not in authoritative_people:
            raise ValueError(f"Cast member has no authoritative actor ID: {person['id']}")
        if person["display"] != authoritative_people[person["id"]]["name"]:
            raise ValueError(f"Renamed existing resident: {person['id']}")
        if not person.get("relationships") or len(person["relationships"]) < 2:
            raise ValueError(f"Not enough distinct relationships: {person['id']}")
        for rel in person["relationships"]:
            if len(rel) != 2 or rel[0] not in authoritative_people or not filled(rel[1]):
                raise ValueError(f"Invalid relationship/provenance: {person['id']}")
        if not set(person.get("media", [])) >= {"game", "novel", "television", "film"}:
            raise ValueError("Main cast missing adaptation-facing medium coverage")
    myth_ids = unique(mysteries, "id", "mysteries")
    for m in mysteries:
        require(m, ("id", "name", "public_premise", "recurrences",
                    "grounded_known", "question_stays_open", "forbidden_shortcut",
                    "canon_guard", "continuity_truth"), "mystery")
        if m.get("answers_status") != "OPEN":
            raise ValueError(f"A story mystery was resolved without explicit authority: {m['id']}")
        if not m["id"].startswith("MYST."):
            raise ValueError("Unknown mystery key")
    cb_ids = unique(callbacks, "id", "callbacks")
    for cb in callbacks:
        require(cb, ("id", "artifact", "origin_city_id", "mystery_id",
                     "seed", "payoff", "expansion_bridge", "boundary"), "callback")
        if cb["origin_city_id"] not in ids or cb["mystery_id"] not in myth_ids:
            raise ValueError("Callback link points to an unknown city or mystery")
        if cb["seed"] == cb["payoff"] or cb["seed"] == cb["expansion_bridge"]:
            raise ValueError("Callback needs developed meaning, not duplicate exposition")
    vol_ids = unique(volumes, "id", "volumes")
    if vol_ids != {f"VOL.{n}" for n in
                  ("I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X")}:
        raise ValueError("Volume lineage lost")
    for v in volumes:
        require(v, ("id", "public_role", "in_world_anchor", "retained", "future_use"), "volume")
    pack_ids = unique(a.get("transmedia_packages", []), "id", "transmedia")
    for work in a["transmedia_packages"]:
        require(work, ("id", "medium", "name", "lead",
                       "premise", "story_engine", "ending_promise"), "transmedia")
        if work["lead"] not in person_ids or not set(work["setting_ids"]).issubset(ids):
            raise ValueError("Unlicensed lead/city in future work")
        if not set(work["required_callbacks"]).issubset(cb_ids):
            raise ValueError("Expansion requires nonexistent callback")
    if len(pack_ids) != 4 or len(a.get("rules_for_adaptation", [])) < 5:
        raise ValueError("Transmedia continuity rules incomplete")
    return True


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    try:
        a = load(ATLAS)
        verify(a, load(WORLDS), load(RESIDENTS), load(REALM_PACK))
    except (ValueError, KeyError, OSError, json.JSONDecodeError) as exc:
        print(f"FAIL transmedia storyworld: {exc}")
        return 1
    print("PASS: 10-volume lineage / 11 existing cities / 13 established actor IDs / 9 open mysteries / 13 callbacks / 4 expansions")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
