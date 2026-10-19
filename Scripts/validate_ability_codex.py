#!/usr/bin/env python3
"""Check the canonical Ability Codex against gameplay source. No Unreal required.

This is a catalog/provenance parity gate; it does NOT prove a playable skill,
Unreal compilation, animation, saved acquisition, or effective balancing.
"""
from __future__ import annotations

import argparse
import collections
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CODEX = "Authoring/ability_codex.json"
ITEMS = "Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h"
RITES = "Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h"
CONFLUENCES = "Source/TheUnmadeGame/Public/World/UnmadeConfluenceRules.h"
EXPEDITION = "Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h"
STYLES = "Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h"
CREATURES = "Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h"
BOSSES = "Source/TheUnmadeGame/Public/Combat/UnmadeBossRules.h"
GUARDIANS = "Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h"
EXPERIENCE = "Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h"
CRAFT = "Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h"
FIELD = "Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h"
FINAL = "Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h"


def _quotes(value: str) -> list[str]:
    return re.findall(r'"([^"]+)"', value)


def _expect(errors: list[str], actual: object, expected: object, label: str) -> None:
    if actual != expected:
        errors.append(f"{label}: codex={actual!r}, source={expected!r}")


def validate(root: Path = ROOT, codex: dict | None = None) -> list[str]:
    issues: list[str] = []
    if codex is None:
        try:
            codex = json.loads((root / CODEX).read_text(encoding="utf-8"))
        except (OSError, ValueError) as exc:
            return [f"Ability Codex missing or malformed: {exc}"]
    if not isinstance(codex, dict) or codex.get("schema_version") != 1:
        return ["Ability Codex requires schema_version 1"]
    records = codex.get("records")
    if not isinstance(records, list):
        return ["Ability Codex records must be a list"]
    by_id: dict[str, dict] = {}
    for number, row in enumerate(records):
        if not isinstance(row, dict):
            issues.append(f"records[{number}] is not an object")
            continue
        key = row.get("id")
        if not isinstance(key, str) or not re.fullmatch(r"[a-zA-Z0-9_.-]+", key):
            issues.append(f"records[{number}] invalid stable ID")
            continue
        if key in by_id:
            issues.append(f"duplicate stable ID: {key}")
        by_id[key] = row
        for field in ("name", "category", "summary", "acquisition", "effect", "cost",
                      "duration", "implementation", "observation", "sources"):
            if field not in row:
                issues.append(f"{key} missing {field}")
        if row.get("implementation") not in ("source_rule", "prototype_handler",
                                             "definition_only", "proposed"):
            issues.append(f"{key} has unsupported implementation status")
        for source in row.get("sources", []):
            if not isinstance(source, str) or not (root / source).is_file():
                issues.append(f"{key}: missing source file {source!r}")
        for target in row.get("combines_with", []):
            if target not in by_id and not any(other.get("id") == target for other in records if isinstance(other, dict)):
                issues.append(f"{key}: unknown combination {target}")
        obs = row.get("observation", {})
        if not isinstance(obs, dict) or obs.get("automatically_learnable") is True:
            issues.append(f"{key}: observation cannot automatically grant powers")
    def source(path: str) -> str:
        return (root / path).read_text(encoding="utf-8")
    def check(key: str, field: str, expected: object) -> None:
        row = by_id.get(key)
        if row is None:
            issues.append(f"missing registry entry: {key}")
            return
        current: object = row
        for segment in field.split("."):
            if not isinstance(current, dict) or segment not in current:
                issues.append(f"{key}: missing field {field}")
                return
            current = current[segment]
        _expect(issues, current, expected, f"{key}.{field}")

    # Every actual ItemCatalog row, including craft-only materials, must appear.
    item_rows = re.findall(
        r'\{ItemId::(\w+),\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*'
        r'ItemKind::(\w+),\s*Rarity::(\w+),\s*GearSlot::(\w+),\s*'
        r'(\d+),\s*(-?\d+),\s*(-?\d+),\s*(-?\d+),\s*(-?\d+)\s*\}', source(ITEMS))
    _expect(issues, len(item_rows), 60, "canonical item catalog size")
    for key, code, name, lore, kind, rarity, slot, maximum, attack, armor, heal, strain in item_rows:
        ident = "item." + code
        check(ident, "name", name)
        check(ident, "effect.attack", int(attack))
        check(ident, "effect.armor", int(armor))
        check(ident, "effect.healing", int(heal))
        check(ident, "effect.strain_recovery", int(strain))
        check(ident, "rarity", rarity)
        check(ident, "slot", slot)
        check(ident, "stack_cap", int(maximum))
        check(ident, "category", "consumable" if kind == "Consumable" else
              "craft_material" if kind == "Material" else "equipment")

    text = source(RITES)
    part = text.split("inline constexpr std::array<RiteSpec,10> RiteSpecs = {{")[1].split("}};")[0]
    found = list(re.finditer(
        r'\{RiteId::(\w+),Discipline::(\w+),"([^"]+)",\s*"([^"]+)",\s*'
        r'"([^"]+)",\s*"([^"]+)",\s*"([^"]+)"', part))
    _expect(issues, len(found), 10, "canonical rite catalog size")
    for i, match in enumerate(found):
        key, discipline, quest, site, witness, lore, reward = match.groups()
        chunk = part[match.start():found[i + 1].start() if i + 1 < len(found) else len(part)]
        numeric = re.search(r'\}\},\s*(\d+),\s*([\d.]+),\s*([\d.]+)\s*\}', chunk)
        if numeric is None:
            issues.append(f"cannot parse rite costs: {key}")
            continue
        ident = "rite." + key
        check(ident, "name", key)
        check(ident, "discipline", discipline)
        check(ident, "quest", quest)
        check(ident, "acquisition.site", site)
        check(ident, "acquisition.witness", witness)
        check(ident, "reward", reward)
        check(ident, "cost.strain", int(numeric[1]))
        check(ident, "duration.seconds", float(numeric[2]))
        check(ident, "cost.cooldown_seconds", float(numeric[3]))

    con = source(CONFLUENCES)
    conpart = con.split("inline constexpr std::array<ConfluenceSpec,6> Confluences = {{")[1].split("}};")[0]
    con_rows = re.findall(
        r'\{ConfluenceId::(\w+),\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",'
        r'\s*RiteId::(\w+),\s*RiteId::(\w+),\s*(\d+)', conpart)
    _expect(issues, len(con_rows), 6, "confluence catalog size")
    for key, name, lore, reward, a, b, region in con_rows:
        ident = "confluence." + key
        check(ident, "name", name)
        check(ident, "reward", reward)
        check(ident, "combines_with", ["rite." + a, "rite." + b])

    exp_rows = []
    for line in source(EXPEDITION).splitlines():
        match = re.search(r'\{Realm::(\w+),', line)
        if match:
            first = _quotes(line.split("{{")[0])
            if len(first) >= 11:
                exp_rows.append(first)
    _expect(issues, len(exp_rows), 9, "regional expedition count")
    for i, fields in enumerate(exp_rows):
        check(f"technique.realm.{i}", "name", fields[7])
        check(f"weapon_style.realm.{i}", "region", fields[0])
        check(f"enemy.expedition_signature.realm.{i}", "name", fields[4])
        check(f"service.realm.{i}", "region", fields[0])
        check(f"recipe.field.realm.{i}", "region", fields[0])

    style_part = source(STYLES).split("constexpr WeaponStyle s[]={")[1].split("};return")[0]
    styles = re.findall(
        r'\{\s*([\d.]+),([\d.]+),([\d.]+),([\d.]+),'
        r'CombatStatus::(\w+),"([^"]+)"\s*\}', style_part)
    _expect(issues, len(styles), 9, "weapon style count")
    for i, (reach, rhythm, stamina, modifier, status, name) in enumerate(styles):
        ident = f"weapon_style.realm.{i}"
        check(ident, "name", name)
        check(ident, "effect.reach_cm", float(reach))
        check(ident, "effect.rhythm_seconds", float(rhythm))
        check(ident, "effect.stamina", float(stamina))
        check(ident, "effect.status", status)
        check(ident, "effect.damage_multiplier", float(modifier))

    cr = source(CREATURES)
    names = [x.strip() for x in re.search(r'enum class CreatureKind \{([^}]+)\}', cr)[1].split(",") if x.strip()]
    _expect(issues, len(names), 12, "creature names")
    for name in names:
        check("enemy.creature." + name, "name", name)

    boss_rows = re.findall(r'\{BossId::(\w+),\s*"([^"]+)"', source(BOSSES))
    _expect(issues, len(boss_rows), 3, "named boss count")
    for key, name in boss_rows:
        check("enemy.boss." + key, "name", name)

    guardian_rows = re.findall(r'\{Realm::\w+,\s*"([^"]+)"', source(GUARDIANS))
    _expect(issues, len(guardian_rows), 6, "realm guardian count")
    for i, name in enumerate(guardian_rows, 1):
        check(f"enemy.guardian.realm.{i}", "name", name)

    wing_rows = []
    for line in source(EXPERIENCE).splitlines():
        if re.match(r'\s*\{"[^"]+","[^"]+","[^"]+"', line):
            wing_rows.append(_quotes(line)[2])
    _expect(issues, len(wing_rows), 9, "wing guardian count")
    for i, name in enumerate(wing_rows):
        check(f"enemy.wing_guardian.realm.{i}", "name", name)

    final = re.search(r'enum class FinalAttack:int \{([^}]+)\}', source(FINAL))[1]
    for name in (x.split("=")[0].strip() for x in final.split(",")):
        check("enemy.final." + name, "name", "Nhal-Vey: " + name)

    for recipe in re.findall(r'\{RecipeId::(\w+),\s*"([^"]+)"', source(CRAFT)):
        check("recipe.workshop." + recipe[0], "name", recipe[1])
    fields = re.findall(r'\{\s*"([^"]+)",\s*Profession::', source(FIELD))
    _expect(issues, len(fields), 9, "field recipe count")
    for i, name in enumerate(fields):
        check(f"recipe.field.realm.{i}", "name", name)
    return issues


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--report", action="store_true", help="Print counts and unimplemented gaps")
    args = parser.parse_args()
    issues = validate()
    if issues:
        for problem in issues:
            print("FAIL:", problem)
        return 1
    doc = json.loads((ROOT / CODEX).read_text(encoding="utf-8"))
    counts = collections.Counter(row["category"] for row in doc["records"])
    print(f"PASS: {len(doc['records'])} Ability Codex entries, source names, grants and costs checked")
    if args.report:
        for category, count in sorted(counts.items()):
            print(f"  {category}: {count}")
        missing = [r for r in doc["records"] if r["implementation"] == "definition_only"]
        observed = [r for r in doc["records"] if r["observation"].get("possible_to_study") and
                    not r["observation"].get("currently_usable_by_player", True)]
        gear_only = [r for r in doc["records"] if r["category"] == "equipment"
                     and not r["effect"].get("distinct_effects")]
        print(f"GAPS: {len(missing)} definition-only; {len(observed)} observable enemy patterns "
              f"not currently learnable; {len(gear_only)} equipment definitions with only basic bonuses")
        for row in missing:
            print("  FUTURE:", row["name"])
    print("NOTE: Unreal compilation, gameplay and observation-based power acquisition NOT proven.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
