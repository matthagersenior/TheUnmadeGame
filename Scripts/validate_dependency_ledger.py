#!/usr/bin/env python3
"""Check the canonical relationship ledger against the authored C++ registries.

Pure stdlib, deliberately fail-closed when a known registry changes syntax.
This is a source contract; UE Editor/PIE and audiovisual content remain unverified.
"""
from __future__ import annotations

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PATH = ROOT / "docs/canon/dependency-ledger.json"
PUBLIC = "Source/TheUnmadeGame/Public/"

def read(path: str) -> str:
    return (ROOT / path).read_text(encoding="utf-8")

def rows(path: str, pattern: str):
    return re.findall(pattern, read(PUBLIC + path), flags=re.DOTALL)

def index(data, key, errors):
    records = data.get(key)
    if not isinstance(records, list):
        errors.append(f"{key} must be an array")
        return {}
    found = {}
    for record in records:
        if not isinstance(record, dict) or not isinstance(record.get("id"), str):
            errors.append(f"{key} has an entry without a string id")
            continue
        if record["id"] in found:
            errors.append(f"{key}: duplicate id {record['id']}")
        found[record["id"]] = record
    return found

def equal_ids(actual, expected, label, errors):
    a, e = set(actual), set(expected)
    if a != e:
        errors.append(f"{label} drift: missing {sorted(e-a)}; orphaned {sorted(a-e)}")

def error_if(condition, message, errors):
    if condition:
        errors.append(message)

def validate(data: dict) -> list[str]:
    errors = []
    if data.get("schema") != 1:
        errors.append("schema must be 1")
    all_items = index(data, "items", errors)
    all_gear = index(data, "regional_gear", errors)
    all_skills = index(data, "skills", errors)
    all_bosses = index(data, "bosses", errors)
    all_quests = index(data, "quests", errors)
    if len(errors):
        return errors

    cat = "Items/UnmadeItemRules.h"
    item_rows = rows(cat, r'\{ItemId::(\w+),\s*"([^"]+)",\s*"([^"]+)",\s*"[^"]+",\s*ItemKind::(\w+),Rarity::(\w+),GearSlot::(\w+),(\d+),(\d+),(\d+),(\d+),(\d+)\}')
    error_if(len(item_rows) < 60, "ItemCatalog parser found fewer than original 60 entries", errors)
    equal_ids(all_items, [r[0] for r in item_rows], "ItemCatalog", errors)
    for id, code, name, kind, rarity, slot, _, atk, armor, heal, strain in item_rows:
        item = all_items.get(id)
        if item is None:
            continue
        actual = (item.get("code"), item.get("name"), item.get("kind"), item.get("rarity"),
                  item.get("slot"), item.get("attack"), item.get("armor"), item.get("heal"),
                  item.get("strain_recovery"))
        expected = (code, name, kind, rarity, slot, int(atk), int(armor), int(heal), int(strain))
        error_if(actual != expected, f"item {id}: identity or stat drift", errors)

    # Acquisition is derived from actual executable milestone, recipe, vendor and forge code.
    grants = {}
    def add(item, method, via):
        grants.setdefault(item, set()).add((method, via))
    cases = rows(cat, r'case Achievement::(\w+): okay=(.*?);break;')
    error_if(len(cases) < 42, "Achievement::Claim reward parser missed original cases", errors)
    for milestone, expr in cases:
        for item in re.findall(r'add\(ItemId::(\w+),\d+\)', expr):
            add(item, "milestone", milestone)
    craft = "Items/UnmadeCraftEconomyRules.h"
    if '#include "Items/UnmadeCodexCraftingGenerated.inc"' not in read(PUBLIC + craft):
        errors.append("workshop crafting table is not wired to Codex-generated source")
    recipes = rows("Items/UnmadeCodexCraftingGenerated.inc", r'\{RecipeId::(\w+),"([^"]+)",Profession::(\w+),SettlementId::(\w+),(\d+),\s*ItemId::(\w+),(\d+),ItemId::(\w+),(\d+),ItemId::(\w+),(\d+)\}')
    field = rows("Items/UnmadeFieldCraftRules.h", r'\{"([^"]+)",Profession::(\w+),ItemId::(\w+),(\d+),ItemId::(\w+),(\d+),ItemId::(\w+),(\d+)\}')
    error_if(len(recipes) < 6 or len(field) < 9, "craft recipe source parser is incomplete", errors)
    for recipe in recipes:
        add(recipe[9], "workshop", recipe[0])
    for recipe in field:
        add(recipe[6], "field_recipe", recipe[0])
    for item in re.findall(r'case ItemId::(\w+):\s*base=\d+;break;', read(PUBLIC + craft)):
        add(item, "shop", "RegionalEconomy::Price")
    if "bool ForgeWaybreaker()" in read(PUBLIC + cat):
        add("Waybreaker", "unique_forge", "ForgeWaybreaker")
    for id, item in all_items.items():
        actual = {(g.get("method"), g.get("via")) for g in item.get("acquisition", [])}
        equal_ids(actual, grants.get(id, set()), "item acquisition " + id, errors)
        expect = "earnable" if grants.get(id) else "reserved"
        # A Claim() switch branch alone is not an executable acquisition route.
        if id == "AtlasOfAbsence" and ("milestone", "AtlasOfAbsenceMastery") in grants.get(id, set()):
            wiring = any("Achievement::AtlasOfAbsenceMastery" in file.read_text(encoding="utf-8")
                         for file in (ROOT / "Source/TheUnmadeGame/Private").rglob("*.cpp")
                         if "/Tests/" not in file.as_posix())
            if not wiring:
                expect = "declared_unwired"
        error_if(item.get("availability") != expect, f"item {id}: incorrect {expect} availability", errors)
    pending = [id for id, item in all_items.items() if item.get("availability") in ("reserved", "declared_unwired")]
    recorded = [r.get("id") for r in data.get("exceptions", [])
                if r.get("type") in ("no_acquisition", "unwired_milestone")]
    equal_ids(recorded, pending, "unobtainable/unwired exception", errors)
    error_if(any(r.get("type") == "reward_name_conflict" for r in data.get("exceptions", [])),
             "obsolete rite reward exception; correct the gameplay/ledger names", errors)

    if '#include "World/UnmadeCodexExpeditionGenerated.inc"' not in read(PUBLIC + "World/UnmadeExpeditionRegistry.h"):
        errors.append("expedition runtime not using Codex-generated rules")
    expedition = rows("World/UnmadeCodexExpeditionGenerated.inc", r'\{Realm::(\w+),"([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)"')
    error_if(len(expedition) != 9, "ExpeditionSpecs parse expected nine realm records", errors)
    realm_ids = [x[0] for x in expedition]
    city_to_realm = {x[1]: x[0] for x in expedition}
    equal_ids(all_gear, [f"gear:{r}:{kind}" for r in realm_ids for kind in ("weapon","armor")], "regional gear", errors)
    study = re.search(
        r'enum class StudyId\s*:\s*int\s*\{([^}]+)\}', read(PUBLIC + "World/UnmadeAbilityStudyRules.h")).group(1)
    study_ids = [id for id in re.findall(r'(\w+)\s*=\s*\d+', study) if id != "Count"]
    source_study = read(PUBLIC + "World/UnmadeAbilityStudyRules.h")
    if "StudyNames=CodexStudyNames" not in source_study:
        errors.append("Study names are not wired to Codex-generated source")
    generated_study = read(PUBLIC + "World/UnmadeCodexGenerated.h")
    match = re.search(r'CodexStudyNames\s*=\s*\{\{(.*?)\}\};', generated_study, flags=re.DOTALL)
    study_names = re.findall(r'"([^"]+)"', match.group(1)) if match else []
    if len(study_ids) != len(study_names) or len(study_ids) < 4:
        errors.append("new firsthand Study registry or title parsing changed")
    for id, title in zip(study_ids, study_names):
        skill = all_skills.get("skill:study:"+id, {})
        quest = all_quests.get("quest:study:"+id, {})
        if skill.get("name") != title or quest.get("name") != "Study: "+title:
            errors.append("firsthand study source identity drift: "+id)
        if skill.get("learning_quest") != "quest:study:"+id:
            errors.append("firsthand study has no earned mastery quest: "+id)
    rites = rows("World/UnmadeTenfoldChronicle.h", r'\{RiteId::(\w+),Discipline::(\w+),"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)"')
    error_if(len(rites) < 10, "RiteSpecs parse expected original ten rites", errors)
    equal_ids(all_skills, ["skill:rite:" + x[0] for x in rites] + ["skill:technique:" + x[0] for x in expedition] + ["skill:study:" + x for x in study_ids], "skills", errors)
    for id, disc, title, site, teacher, _, advertised in rites:
        skill = all_skills.get("skill:rite:" + id, {})
        quest = all_quests.get("quest:rite:" + id, {})
        error_if((skill.get("name"), skill.get("discipline"), skill.get("teacher_or_source")) !=
                 (title, disc, teacher), "rite " + id + ": learned ability source changed", errors)
        error_if(quest.get("name") != title or quest.get("rite_reward_label") != advertised,
                 "rite " + id + ": quest identity or advertised reward changed", errors)
        granted = next((re.findall(r'ItemId::(\w+)', body)[0] for achievement, body in cases
                        if achievement == "Rite" + id), None)
        error_if(skill.get("mastery_reward") != "item:" + str(granted),
                 "rite " + id + ": mastery reward no longer linked to source grant", errors)
        error_if(advertised != all_items.get(granted, {}).get("name"),
                 "rite " + id + ": named reward differs from actual ItemCatalog grant", errors)

    for i, e in enumerate(expedition):
        realm, city, _, _, dungeon, boss_name, weapon, armor, technique = e
        for kind, name, bonus in (("weapon", weapon, 7+2*i), ("armor", armor, 5+i)):
            gear = all_gear.get(f"gear:{realm}:{kind}", {})
            error_if((gear.get("name"), gear.get("bonus"), gear.get("acquisition")) !=
                     (name, bonus, "quest:expedition:" + realm), f"regional gear {realm}/{kind} drift", errors)
        skill = all_skills.get("skill:technique:" + realm, {})
        error_if(skill.get("name") != technique or skill.get("learning_quest") != "quest:expedition:" + realm,
                 "expedition technique drift: " + realm, errors)
        boss = all_bosses.get("boss:expedition:" + realm, {})
        error_if((boss.get("name"), boss.get("location"), boss.get("hp")) !=
                 (boss_name, dungeon, 240+25*i), "expedition boss name/location/HP drift: " + realm, errors)
        error_if(all_quests.get("quest:expedition:" + realm, {}).get("boss") != "boss:expedition:" + realm,
                 "expedition boss quest relation broken: " + realm, errors)

    original = rows("Combat/UnmadeBossRules.h",
                    r'\{BossId::(\w+),"([^"]+)","([^"]+)",\s*"([^"]+)",(\d+),(\d+),(\d+),([\d.]+),([\d.]+)\}')
    guards = rows("Combat/UnmadeRealmGuardianRules.h",
                  r'\{Realm::(\w+),"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*(\d+),(\d+),([\d.]+),([\d.]+),(\d+)\}')
    roads = rows("World/UnmadeRememberedRoadRegistry.h",
                 r'\{"([^"]+)",Realm::(\w+),[-\d.]+,[-\d.]+,\d+,"([^"]+)","([^"]+)","([^"]+)","([^"]+)",\{[^}]+\},"([^"]+)","([^"]+)",CreatureKind::(\w+),"([^"]+)"')
    if '#include "World/UnmadeCodexExperienceGenerated.inc"' not in read(PUBLIC + "World/UnmadePlayerExperienceRegistry.h"):
        errors.append("wing registry is not Codex-generated")
    wings = rows("World/UnmadeCodexExperienceGenerated.inc", r'\{"([^"]+)","([^"]+)","([^"]+)","([^"]+)"')
    wings = [w for w in wings if w[0] in city_to_realm]
    error_if(len(original) != 3 or len(guards) != 6 or len(roads) != 11 or len(wings) != 9,
             "boss/guardian/wing/road source parse changed: inspect the registry", errors)
    expected_bosses = (["boss:original:"+x[0] for x in original] +
                       ["boss:expedition:"+x for x in realm_ids] +
                       ["boss:guardian:"+x[0] for x in guards] +
                       ["boss:wing:"+x for x in realm_ids] +
                       ["boss:road:"+x[0] for x in roads] + ["boss:final:NhalVey"])
    equal_ids(all_bosses, expected_bosses, "boss encounters", errors)
    for id, name, place, _, health, damage, *_ in original:
        b = all_bosses.get("boss:original:" + id, {})
        error_if((b.get("name"), b.get("location"), b.get("hp"), b.get("damage")) !=
                 (name, place, int(health), int(damage)), "original boss changed " + id, errors)
    for row in guards:
        b = all_bosses.get("boss:guardian:" + row[0], {})
        error_if((b.get("name"), b.get("hp"), b.get("damage")) !=
                 (row[1], int(row[6]), int(row[7])), "realm guardian changed: " + row[0], errors)
        error_if(b.get("quest") != "quest:echo:" + row[0],
                 "guardian Echo prerequisite drift: " + row[0], errors)
    for row in roads:
        b = all_bosses.get("boss:road:" + row[0], {})
        error_if(b.get("name") != row[9] or b.get("creature") != row[8],
                 "road elder identity changed: " + row[0], errors)
    for city, wing_name, guardian_name, _ in wings:
        realm = city_to_realm[city]
        error_if(all_bosses.get("boss:wing:" + realm, {}).get("name") != guardian_name or
                 all_quests.get("quest:wing:" + realm, {}).get("name") != wing_name,
                 "wing encounter changed: " + realm, errors)

    confluences = rows("World/UnmadeConfluenceRules.h",
                       r'\{ConfluenceId::(\w+),\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*RiteId::(\w+),RiteId::(\w+),(\d+)')
    aftermath = rows("World/UnmadeRealmAftermathRules.h",
                     r'\{Realm::(\w+),"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)"')
    echoes = rows("World/UnmadeEchoQuestRules.h", r'\{Realm::(\w+),"([^"]+)","([^"]+)",\s*"([^"]+)"')
    faction = rows("World/UnmadeFactionChronicleRules.h",
                   r'\{Faction::(\w+),"([^"]+)",\s*"([^"]+)","([^"]+)","([^"]+)",\s*"([^"]+)"\}')
    depth = rows("World/UnmadeAdventureDepthRegistry.h",
                 r'\{"([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)"')
    if '#include "World/UnmadeCodexCampaignGenerated.inc"' not in read(PUBLIC + "World/UnmadeCampaignSpineRules.h"):
        errors.append("main story source not wired to Codex-owned chapters")
    campaign = rows("World/UnmadeCodexCampaignGenerated.inc",
                    r'\{Realm::(\w+),"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)"\}')
    error_if((len(confluences),len(aftermath),len(echoes),len(faction),len(depth),len(campaign)) !=
             (6,9,6,3,9,9), "quest registry parser shape changed", errors)
    expected_quests = (["quest:campaign:"+e[0] for e in campaign] +
                       ["quest:expedition:"+e for e in realm_ids] +
                       ["quest:aftermath:"+e[0] for e in aftermath] +
                       ["quest:echo:"+e[0] for e in echoes] +
                       ["quest:rite:"+e[0] for e in rites] +
                       ["quest:confluence:"+e[0] for e in confluences] +
                       ["quest:wing:"+e for e in realm_ids] +
                       ["quest:road:"+e[0] for e in roads] +
                       ["quest:faction:"+e[0] for e in faction] +
                       ["quest:depth:"+e for e in realm_ids] +
                       ["quest:identity:"+e for e in realm_ids] +
                       ["quest:study:"+x for x in study_ids] +
                       ["quest:local:BellwoldLanterns","quest:local:PaperhavenTestimony","quest:local:Afterlight",
                        "quest:frontier:WidowedRain","quest:frontier:HearthBeneath"])
    equal_ids(all_quests, expected_quests, "major quest registry", errors)
    for row in aftermath:
        realm, title, _, control = row[:4]
        q = all_quests.get("quest:aftermath:" + realm, {})
        error_if(q.get("name") != title or q.get("mechanism") != control,
                 "aftermath source identity drift: " + realm, errors)
        # These must be actual distinct source-defined effects, not generic promises.
        error_if(q.get("aftermath") != [row[8], row[9]],
                 "aftermath care/truth world effects drift: " + realm, errors)
    for row in echoes:
        error_if(all_quests.get("quest:echo:"+row[0], {}).get("name") != row[1],
                 "Echo chapter title drift: " + row[0], errors)
    for row in campaign:
        error_if(all_quests.get("quest:campaign:"+row[0], {}).get("name") != row[1],
                 "main plot chapter changed: " + row[0], errors)
    for row in confluences:
        q = all_quests.get("quest:confluence:"+row[0], {})
        error_if(q.get("name") != row[1] or q.get("skills") != ["skill:rite:"+row[4],"skill:rite:"+row[5]],
                 "Confluence mastery pair changed: " + row[0], errors)
        error_if(row[3] not in [item["name"] for item in all_items.values()],
                 "Confluence reward has no real ItemCatalog item: " + row[0], errors)
    for row in roads:
        error_if(all_quests.get("quest:road:"+row[0], {}).get("name") != row[3],
                 "road quest name changed: " + row[0], errors)
    for i, row in enumerate(depth):
        error_if(all_quests.get("quest:depth:"+realm_ids[i], {}).get("name") != row[0],
                 "community accord changed: " + realm_ids[i], errors)
    for row in faction:
        error_if(all_quests.get("quest:faction:"+row[0], {}).get("name") != row[1],
                 "faction arc title changed: " + row[0], errors)

    item_refs = {"item:"+id for id in all_items}
    valid_rewards = item_refs | set(all_gear) | set(all_skills)
    for category, entries in (("boss", all_bosses), ("quest", all_quests)):
        for id, obj in entries.items():
            for key in (("access","attacks","resolutions","aftermath","rewards") if category=="boss"
                        else ("entry","choices","aftermath","rewards")):
                val = obj.get(key)
                if not val or isinstance(val,list) and not all(isinstance(s,str) and s.strip() for s in val):
                    errors.append(f"{id}: missing or empty {key}")
            for r in obj.get("rewards", []):
                if r not in valid_rewards and not r.startswith(("world:", "resource:")):
                    errors.append(f"{id}: dangling reward {r}")
            for link in (("quest", all_quests),) if category=="boss" else (("boss", all_bosses), ("skill", all_skills), ("guardian", all_bosses)):
                prop, targets = link
                if obj.get(prop) and obj[prop] not in targets:
                    errors.append(f"{id}: missing linked {prop} {obj[prop]}")
    for id, skill in all_skills.items():
        if skill.get("learning_quest") not in all_quests or not skill.get("teacher_or_source"):
            errors.append(id + ": no concrete learning source/quest")
        if skill.get("mastery_reward") and skill["mastery_reward"] not in item_refs and not skill["mastery_reward"].startswith(("world:", "resource:")):
            errors.append(id + ": mastery item not catalogued")
    # The larger Ability Codex indexes inherent, learned, observed, passive
    # and equipment-mediated capabilities. Do not clone its full 182 records.
    codex = data.get("codex", {})
    codex_path = codex.get("path")
    if codex_path != "Authoring/ability_codex.json":
        errors.append("the canonical Ability Codex source must remain linked")
    else:
        ability = json.loads(read(codex_path))
        records = ability.get("records", [])
        if len(records) < 182:
            errors.append("Ability Codex unexpectedly lost registered capabilities")
        ability_ids = {r.get("id") for r in records}
        for r in records:
            if not r.get("id") or not r.get("name") or not r.get("category") or not r.get("sources") or not r.get("acquisition", {}).get("method"):
                errors.append("Ability Codex capability has missing identity, acquisition, or source: "+str(r.get("id")))
        for id, item in all_items.items():
            if "item."+item["code"] not in ability_ids:
                errors.append("Item missing from canonical Ability Codex: "+id)
        for id in [x[0] for x in rites]:
            if "rite."+id not in ability_ids:
                errors.append("Learned rite missing from canonical Ability Codex: "+id)
    codex_by_id = {row.get("id"): row for row in
                   json.loads(read("Authoring/ability_codex.json")).get("records", [])}
    for id, _, _, _, _, _, reward_name in rites:
        if codex_by_id.get("rite."+id, {}).get("reward") != reward_name:
            errors.append("Ability Codex reward mismatch: " + id)
    for id, gear in all_gear.items():
        if gear.get("acquisition") not in all_quests:
            errors.append(id + ": no available regional quest")
        if gear.get("exclusive_with") not in all_gear:
            errors.append(id + ": no reciprocal exclusive alternative")
    return errors

def main() -> int:
    try:
        data = json.loads(PATH.read_text(encoding="utf-8"))
        problems = validate(data)
    except (OSError, ValueError, KeyError, TypeError) as exc:
        print("Dependency ledger could not be validated:", exc)
        return 1
    if problems:
        for problem in problems:
            print("CANON DRIFT:", problem)
        return 1
    print("Source-backed canon dependency ledger valid: " +
          ", ".join(f"{group}={len(data[group])}" for group in
                    ("items", "regional_gear", "skills", "bosses", "quests")))
    return 0

if __name__ == "__main__":
    sys.exit(main())
