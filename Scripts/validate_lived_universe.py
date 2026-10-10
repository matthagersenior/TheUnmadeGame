"""Deterministic lived-universe authoring validator and editor work-order generator.

Preproduction content only. Never writes Unreal assets, gameplay state or
synthetic quest completions. Standard-library Python 3.11+.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MASTER = ROOT / "Authoring/lived_universe_atlas.json"
CANON = ROOT / "Authoring/cinematic_realm_playbook.json"
OUTPUT = ROOT / "Authoring/generated/lived_universe_scene_manifest.json"
REALMS = (
    "ThreefoldReach", "WidowedRain", "HearthBeneath", "TidalLedger",
    "SkyBelow", "CinderSpine", "HundredUnlived",
    "OrchardOfKings", "FirstAbsence",
)
REQUIRED_WORLD = (
    "id", "title", "settlements", "ordinary", "architecture", "custom",
    "taboo", "governance", "labor", "trade", "ecology", "weather",
    "sensory", "mystery", "routine", "side", "asset", "post",
)
REQUIRED_ROUTINE = (
    "id", "window", "place", "trigger", "action", "reaction", "line",
    "feedback", "failure", "recovery", "persistence",
)
REQUIRED_CLUE = ("id", "site", "verb", "discovery")
REQUIRED_SIDE = (
    "id", "name", "unlock", "objective", "steps", "branches",
    "failure", "recovery", "return_dialogue", "save",
)


def read(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def require_text(container: dict, fields: tuple[str, ...], where: str) -> None:
    for field in fields:
        value = container.get(field)
        if not isinstance(value, str) or not value.strip():
            raise ValueError(f"{where}.{field} requires meaningful text")


def verify(master: dict, canon: dict) -> bool:
    if master.get("schema_version") != 1:
        raise ValueError("Unsupported lived-universe schema")
    if master.get("canonical_order") != list(REALMS):
        raise ValueError("Lived-universe canonical realm order drift")
    if [r.get("id") for r in canon.get("realms", [])] != list(REALMS):
        raise ValueError("Cinematic production realm order drift")
    if len(master.get("worlds", [])) != len(REALMS):
        raise ValueError("Exactly nine world records required")
    if [w.get("id") for w in master["worlds"]] != list(REALMS):
        raise ValueError("Realm order differs from authoritative source")
    if not isinstance(master.get("authority"), list) or len(master["authority"]) < 3:
        raise ValueError("Missing canonical authority references")
    require_text(master, ("status", "design_intent", "presentation_budget")
                 if "presentation_budget" in master else ("status", "design_intent"), "master")
    if "not" not in master["status"].lower() or "unreal" not in master["status"].lower():
        raise ValueError("Content status must disclose lack of engine verification")
    rules = master.get("universal_rules")
    if not isinstance(rules, dict):
        raise ValueError("Missing universal player-first rules")
    require_text(rules, ("progression", "evidence", "personal_knowledge",
                         "choices", "rollback", "failure", "accessibility",
                         "ending", "content_status"), "universal_rules")
    all_ids = set()
    for realm in master["worlds"]:
        rid = realm["id"]
        require_text(realm, tuple(f for f in REQUIRED_WORLD if f not in
                     ("settlements", "sensory", "mystery", "routine", "side", "asset", "post")), rid)
        if not isinstance(realm.get("settlements"), list) or not realm["settlements"]:
            raise ValueError(f"{rid}: empty settlements")
        require_text(realm["sensory"], ("day", "night", "hazard_cue", "access"), f"{rid}.sensory")
        require_text(realm["post"], ("held", "many"), f"{rid}.post")
        mystery = realm.get("mystery")
        if not isinstance(mystery, dict):
            raise ValueError(f"{rid}: no mystery")
        require_text(mystery, ("question", "visible", "false_lead", "interpretation",
                               "link", "hint", "do_not_reveal"), f"{rid}.mystery")
        clues = mystery.get("evidence")
        if not isinstance(clues, list) or len(clues) != 3:
            raise ValueError(f"{rid}: exactly three physical evidence sites required")
        for clue in clues:
            require_text(clue, REQUIRED_CLUE, f"{rid}.evidence")
            if not clue["id"].startswith(f"LUE.{rid}."):
                raise ValueError(f"{rid}: noncanonical evidence id")
            if clue["id"] in all_ids:
                raise ValueError(f"Duplicate production id: {clue['id']}")
            all_ids.add(clue["id"])
        routines = realm.get("routine")
        if not isinstance(routines, list) or len(routines) != 2:
            raise ValueError(f"{rid}: exactly two distinct daily life scenes required")
        if routines[0].get("window") == routines[1].get("window"):
            raise ValueError(f"{rid}: routine must vary by time of day")
        for scene in routines:
            require_text(scene, REQUIRED_ROUTINE, f"{rid}.routine")
            if not scene["id"].startswith(f"LUR.{rid}."):
                raise ValueError(f"{rid}: noncanonical routine id")
            if scene["id"] in all_ids:
                raise ValueError(f"Duplicate production id: {scene['id']}")
            all_ids.add(scene["id"])
        side = realm.get("side")
        if not isinstance(side, dict):
            raise ValueError(f"{rid}: side story missing")
        require_text(side, tuple(x for x in REQUIRED_SIDE if x not in ("steps", "branches")), f"{rid}.side")
        if side["id"] in all_ids or not side["id"].startswith(f"LUQ.{rid}."):
            raise ValueError(f"{rid}: missing or duplicate side quest id")
        all_ids.add(side["id"])
        if not isinstance(side.get("steps"), list) or len(side["steps"]) != 5 or any(
            not isinstance(s, str) or not s.strip() for s in side["steps"]
        ):
            raise ValueError(f"{rid}: exactly five authored side-quest actions required")
        require_text(side.get("branches", {}), ("care", "truth"), f"{rid}.side.branches")
        if side["branches"]["care"] == side["branches"]["truth"]:
            raise ValueError(f"{rid}: care/truth must have distinct results")
        if not isinstance(realm.get("asset"), list) or len(realm["asset"]) != 4:
            raise ValueError(f"{rid}: exactly four named asset work packages")
        for asset in realm["asset"]:
            if not isinstance(asset, str) or "_" not in asset or asset in all_ids:
                raise ValueError(f"{rid}: invalid/duplicate asset id: {asset}")
            all_ids.add(asset)
    edges = master.get("cross_realm_mystery", {}).get("chains", [])
    if len(edges) != 9 or len({e.get("id") for e in edges}) != 9:
        raise ValueError("Nine unique cross-region comparative evidence links required")
    for edge in edges:
        require_text(edge, ("id", "from", "to", "player_delivery",
                            "disclosure", "claim"), "cross_realm_mystery.chains")
        if edge["from"] not in REALMS or edge["to"] not in REALMS or edge["from"] == edge["to"]:
            raise ValueError("Invalid inter-realm mystery link")
        if edge["id"] in all_ids:
            raise ValueError("Duplicate mystery link id")
        all_ids.add(edge["id"])
    hints = master.get("cross_realm_mystery", {}).get("puzzle_hints", [])
    if len(hints) != 3 or any(not isinstance(x, str) or not x.strip() for x in hints):
        raise ValueError("Three optional accessibility-respecting puzzle hints required")
    return True


def generate(master: dict) -> dict:
    orders = []
    for realm in master["worlds"]:
        rid = realm["id"]
        for clue in realm["mystery"]["evidence"]:
            orders.append({
                "id": clue["id"], "realm": rid, "kind": "physical_evidence",
                "site": clue["site"], "action": clue["verb"], "source": clue["discovery"],
                "actor_label": f"GUIDE_{clue['id'].replace('.', '_')}",
                "collision": False, "gameplay_authority": False,
                "engine_status": "not_built",
            })
        for scene in realm["routine"]:
            orders.append({
                "id": scene["id"], "realm": rid, "kind": "resident_daily_scene",
                "site": scene["place"], "action": scene["action"], "source": scene["trigger"],
                "actor_label": f"GUIDE_{scene['id'].replace('.', '_')}",
                "collision": False, "gameplay_authority": False,
                "engine_status": "not_built",
            })
        q = realm["side"]
        orders.append({
            "id": q["id"], "realm": rid, "kind": "optional_side_quest",
            "site": realm["settlements"][0], "action": q["objective"],
            "source": q["unlock"], "actor_label": f"GUIDE_{q['id'].replace('.', '_')}",
            "collision": False, "gameplay_authority": False,
            "engine_status": "not_built",
        })
        for asset in realm["asset"]:
            orders.append({
                "id": asset, "realm": rid, "kind": "art_audio_work_package",
                "site": realm["settlements"][0], "action": "Author distinct original or licensed asset",
                "source": realm["sensory"]["day"],
                "actor_label": f"GUIDE_{asset}",
                "collision": False, "gameplay_authority": False,
                "engine_status": "not_built",
            })
    return {
        "schema_version": 1,
        "source": "Authoring/lived_universe_atlas.json",
        "status": "reference-only; NOT importable Unreal gameplay actors or real assets",
        "counts": {
            "physical_evidence": 27,
            "resident_daily_scene": 18,
            "optional_side_quest": 9,
            "art_audio_work_package": 36,
            "total": 90,
        },
        "work_orders": orders,
    }


def render(manifest: dict) -> str:
    return json.dumps(manifest, indent=2, ensure_ascii=False) + "\n"


def main() -> int:
    p = argparse.ArgumentParser()
    group = p.add_mutually_exclusive_group(required=True)
    group.add_argument("--check", action="store_true")
    group.add_argument("--write", action="store_true")
    args = p.parse_args()
    master, canon = read(MASTER), read(CANON)
    verify(master, canon)
    generated = render(generate(master))
    if args.write:
        OUTPUT.parent.mkdir(parents=True, exist_ok=True)
        OUTPUT.write_text(generated, encoding="utf-8")
        print(f"Generated {OUTPUT.relative_to(ROOT)}: 90 non-authoritative work orders")
        return 0
    if not OUTPUT.exists() or OUTPUT.read_text(encoding="utf-8") != generated:
        print("ERROR: lived-universe work orders are missing or stale. Run --write.")
        return 1
    print("Lived-universe nine-realm authoring, 90 work orders and status contract verified.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
