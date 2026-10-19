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
        checklist = row.get("requirements_checklist")
        if not isinstance(checklist, dict) or not checklist.get("items"):
            issues.append(f"{key}: missing prerequisites checklist")
        else:
            seen_requirements = set()
            for item in checklist["items"]:
                if not isinstance(item, dict) or not isinstance(item.get("text"), str) or not item["text"].strip():
                    issues.append(f"{key}: empty/invalid requirement")
                elif item.get("key") in seen_requirements:
                    issues.append(f"{key}: duplicate requirement ID")
                else:
                    seen_requirements.add(item.get("key"))
                if item.get("completion_status") != "evaluated_in_game_only":
                    issues.append(f"{key}: checklist cannot claim an unlocked state from static JSON")
            if key.startswith("study.") or key == "item.charm.atlas":
                if checklist.get("verification") != "playable_save_backed":
                    issues.append(f"{key}: playable requirements must derive from real save state")
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

    for recipe in re.findall(r'\{RecipeId::(\w+),\s*"([^"]+)"', source("Source/TheUnmadeGame/Public/Items/UnmadeCodexCraftingGenerated.inc")):
        check("recipe.workshop." + recipe[0], "name", recipe[1])
    fields = re.findall(r'\{\s*"([^"]+)",\s*Profession::', source(FIELD))
    _expect(issues, len(fields), 9, "field recipe count")
    for i, name in enumerate(fields):
        check(f"recipe.field.realm.{i}", "name", name)
    # The first actual learned abilities are distinct from raw enemy attacks.
    study_source = source("Source/TheUnmadeGame/Public/World/UnmadeAbilityStudyRules.h")
    study_ids = re.search(r'enum class StudyId : int \{([^}]+)\}', study_source)
    if study_ids is None:
        issues.append("missing stable study subject enum")
    else:
        subjects = [p.split("=")[0].strip() for p in study_ids[1].split(",")]
        _expect(issues, subjects, ["Cinderjaw", "PaleStrider", "RedactionEye", "EchoWell", "Count"],
                "study subject IDs must be append-only")
        for subject in subjects[:-1]:
            entry = by_id.get("study." + subject)
            if not entry:
                issues.append(f"missing learned ability for firsthand subject {subject}")
                continue
            check("study." + subject, "implementation", "prototype_handler")
            check("study." + subject, "cost.strain_per_activation", 9)
            check("study." + subject, "cost.activation_cooldown_seconds", 18)
            check("study." + subject, "observation.automatically_learnable", False)
            check("study." + subject, "acquisition.method", "observed_understood_experimented_mastered")
    atlas = by_id.get("item.charm.atlas")
    if atlas:
        check("item.charm.atlas", "implementation", "prototype_handler")
        check("item.charm.atlas", "acquisition.method", "mastered_cartography_verified_road_witnessed_first_absence_and_study")
        expected_effects = ["Equipped charm: Glimpse Strain discount 4",
                            "Equipped charm: Fold duration +2 seconds (even while using regional weapon)"]
        check("item.charm.atlas", "effect.distinct_effects", expected_effects)
    player = source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
    items = source(ITEMS)
    saves = source("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
    for token in ("AbilityStudy.CanClaimAtlas(CurrentAtlasRequirements())", "NearEchoStudySite()",
                  "Achievement::AtlasOfAbsenceMastery", "bool AUnmadeCharacter::PersistStudy()",
                  "UnmadeSave::SaveGameToSlot(Save)", "RecordStudyEvidence(Id,1)",
                  "AbilityStudy.Experiment(Id)", "AbilityStudy.Master(Id)",
                  "Enemy->IsActivelyTelegraphing(Now)", "Before>Target->GetCombat()->GetHealth()",
                  "Enemy->ExposeToFold"):
        if token not in player:
            issues.append(f"ability-study gameplay linkage missing: {token}")
    for token in ("case Achievement::AtlasOfAbsenceMastery:",
                  "charm==ItemId::AtlasOfAbsence ? 4",
                  "Equipped(GearSlot::Charm)==ItemId::AtlasOfAbsence ? 2"):
        if token not in items:
            issues.append(f"Atlas award/effect source missing: {token}")
    for token in ("CurrentAtlasRequirements()", "Save->VerifiedRoadBits>0",
                  "Save->RiteStages[9]==5", "Save->ConfluenceStages[5]==2",
                  "Save->DiscoveredLoreMask&1", "AbilityStudy.AtlasChecklist(Atlas)",
                  "AbilityStudy.Checklist(Id)", "!FindStudyEnemy(SelectedStudy,true)"):
        if token not in player:
            issues.append(f"missing lore-gated player requirement or checklist: {token}")
    for token in ("bHasAbilityStudy", "AbilityStudyStages", "AbilityStudyEvidence",
                  "AbilityStudyTrials", "AbilityStudySelection"):
        if token not in saves:
            issues.append(f"save-backed Ability Study source missing: {token}")
    # Unified canonical authoring: all top-level domains must have one stable ID
    # and every dependency must refer to a known authored record (not a rumor).
    required=("realm","journey.expedition","journey.wing","quest.personal",
              "quest.campaign","quest.aftermath")
    realms=("ThreefoldReach","WidowedRain","HearthBeneath","TidalLedger",
            "SkyBelow","CinderSpine","HundredUnlived","OrchardOfKings","FirstAbsence")
    if not isinstance(codex.get("domains"),dict) or not isinstance(codex.get("rules_of_authoring"),list):
        issues.append("missing whole-game Codex domains and authoring rules")
    else:
        indexed={kind for domain in codex["domains"].values()
                 for kind in domain.get("categories",[])}
        used={row.get("category") for row in records if isinstance(row,dict)}
        for missing in sorted(used-indexed):
            issues.append("Codex category not assigned to a domain: "+str(missing))
    for realm in realms:
        for prefix in required:
            if prefix+"."+realm not in by_id:
                issues.append("missing canonical progression/realm record "+prefix+"."+realm)
    for town in ("Crossings","Bellwold","Paperhaven"):
        if "settlement."+town not in by_id:
            issues.append("missing canonical settlement "+town)
    for row in records:
        if not isinstance(row,dict):continue
        key=row.get("id","")
        for dependency in row.get("depends_on",[]):
            if dependency not in by_id or dependency==key:
                issues.append(f"{key}: invalid Codex dependency {dependency}")
        if row.get("category") in ("realm","journey","experience","personal_quest",
                                    "campaign_quest","aftermath_quest","settlement"):
            if not isinstance(row.get("rule_contract"),dict):
                issues.append(f"{key}: missing realm/progression rule contract")
    # Direct source parity for authoritative names and numeric recipe costs.
    if '#include "World/UnmadeCodexAtlasGenerated.inc"' not in source("Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h"):
        issues.append("real world routing must consume Codex-generated realm and passage table")
    atlas=source("Source/TheUnmadeGame/Public/World/UnmadeCodexAtlasGenerated.inc")
    world_rows=re.findall(r'\{Realm::(\w+),\s*"([^"]+)"',atlas)
    for realm,name in world_rows[:9]:
        check("realm."+realm,"name",name)
    expedition_rows=re.findall(
        r'\{Realm::(\w+),"([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)"',
        source(EXPEDITION))
    for realm,city,frontier,refuge,dungeon,boss in expedition_rows:
        check("journey.expedition."+realm,"name",dungeon+" — "+city)
        check("journey.expedition."+realm,"rule_contract.boss",boss)
        check("journey.expedition."+realm,"rule_contract.frontier",frontier)
    wing_rows=re.findall(r'\{\s*"([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)","([^"]+)"',source(EXPERIENCE))
    _expect(issues,len(wing_rows),9,"wing registry count")
    for realm,row in zip(realms,wing_rows):
        city,wing,guardian=row[:3]
        check("journey.wing."+realm,"name",wing+" — "+city)
        check("journey.wing."+realm,"rule_contract.guardian",guardian)
    story_rows=re.findall(r'\{Realm::(\w+),\s*"([^"]+)"',source("Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h"))
    for realm,title in story_rows[:9]:
        check("quest.campaign."+realm,"name",title)
    aftermath_rows=re.findall(r'\{Realm::(\w+),\s*"([^"]+)"',source("Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h"))
    for realm,title in aftermath_rows[:9]:
        check("quest.aftermath."+realm,"name",title)
    personal_rows=re.findall(r'\{\s*"([^"]+)","([^"]+)"',source("Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h"))
    for realm,(title,goal) in zip(realms,personal_rows[:9]):
        check("quest.personal."+realm,"name",title)
        check("quest.personal."+realm,"rule_contract.goal",goal)
    settlement_rows=re.findall(r'\{SettlementId::(\w+),\s*"([^"]+)"',source("Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h"))
    for key,name in settlement_rows[:3]:
        check("settlement."+key,"name",name)
    recipe_rows=re.findall(
        r'\{RecipeId::(\w+),"([^"]+)",Profession::(\w+),SettlementId::(\w+),(\d+),\s*'
        r'ItemId::(\w+),(\d+),ItemId::(\w+),(\d+),ItemId::(\w+),(\d+)\}',
        source("Source/TheUnmadeGame/Public/Items/UnmadeCodexCraftingGenerated.inc"))
    for key,name,profession,town,skill,ing_a,n_a,ing_b,n_b,output,n_out in recipe_rows:
        ident="recipe.workshop."+key
        check(ident,"rule_contract.profession","profession."+profession.lower())
        check(ident,"rule_contract.workshop","settlement."+town)
        check(ident,"rule_contract.min_skill",int(skill))
        check(ident,"rule_contract.inputs",[{"item_enum":ing_a,"count":int(n_a)},
                                           {"item_enum":ing_b,"count":int(n_b)}])
        check(ident,"rule_contract.output",{"item_enum":output,"count":int(n_out)})
    if '#include "Items/UnmadeCodexCraftingGenerated.inc"' not in source(CRAFT):
        issues.append("live crafting model must include Codex-generated recipe table")
    graph_rows=re.findall(r'\{Realm::(\w+),Realm::(\w+),(\d+)\}',atlas)
    if len(graph_rows)!=13:
        issues.append("Codex route graph must describe exactly 13 original passages")
    for i,(origin,destination,level) in enumerate(graph_rows):
        path_id=f"passage.{i+1:02d}.{origin}.{destination}"
        check(path_id,"rule_contract.minimum_attunement",int(level))
        check(path_id,"rule_contract.from","realm."+origin)
        check(path_id,"rule_contract.to","realm."+destination)
        check(path_id,"rule_contract.bidirectional",True)
        if int(level)<0 or int(level)>4:
            issues.append(f"{path_id}: invalid attunement tier")
    study_source=source("Source/TheUnmadeGame/Public/World/UnmadeAbilityStudyRules.h")
    for token in ('#include "World/UnmadeCodexGenerated.h"',"CodexStudyNames",
                  "CodexStudyRequirements[static_cast<int>(id)][0]","CodexAtlasRequirements[0]"):
        if token not in study_source:
            issues.append("study game requirements not Codex-generated: "+token)
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
        print(f"GAPS: {len(missing)} definition-only; {len(observed)} enemy patterns not directly transferable; "
              f"{len(gear_only)} equipment definitions with only basic bonuses; "
              f"{counts.get('earned_study_ability', 0)} authored learnable variants")
        for row in missing:
            print("  FUTURE:", row["name"])
    print("NOTE: four gameplay study paths are wired in Unreal source; Unreal Editor compile and playtesting remain unverified.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
