#!/usr/bin/env python3
"""Generate gameplay-facing C++ display rules from the single canonical Codex."""
from __future__ import annotations
import argparse, json
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
CODEX=ROOT/"Authoring/ability_codex.json"
OUTPUT=ROOT/"Source/TheUnmadeGame/Public/World/UnmadeCodexGenerated.h"
STUDIES=("Cinderjaw","PaleStrider","RedactionEye","EchoWell")
HUMAN=("Cinderjaw","Pale Strider","Redaction Eye","Echo Well")
REALMS=("ThreefoldReach","WidowedRain","HearthBeneath","TidalLedger","SkyBelow","CinderSpine","HundredUnlived","OrchardOfKings","FirstAbsence")

def quote(value):
    return json.dumps(value,ensure_ascii=False,separators=(",",":"))

def generate(codex):
    by={row["id"]:row for row in codex["records"]}
    out=["#pragma once","// GENERATED FROM Authoring/ability_codex.json. Do not hand-edit.",
         "// Regenerate with: python Scripts/generate_unmade_codex.py",
         "#include <array>","namespace UnmadeCore {",
         "inline constexpr std::array<const char*,4> CodexStudyNames = {{"]
    for n,key in enumerate(STUDIES):
        out.append("    "+quote(by["study."+key]["name"]+" / "+HUMAN[n])+("" if n==3 else ","))
    out.extend(["}};","inline constexpr std::array<std::array<const char*,6>,4> CodexStudyRequirements = {{"])
    for n,key in enumerate(STUDIES):
        labels=by["study."+key]["requirements_checklist"]["items"]
        if len(labels)!=6:raise ValueError("Codex must specify six checkpoints for "+key)
        out.append("    {{"+",".join(quote(x["text"]) for x in labels)+"}}"+("" if n==3 else ","))
    out.extend(["}};","inline constexpr std::array<const char*,8> CodexAtlasRequirements = {{"])
    atlas=by["item.charm.atlas"]["requirements_checklist"]["items"][:8]
    if len(atlas)!=8:raise ValueError("Codex must specify eight Atlas gates")
    for n,item in enumerate(atlas):
        out.append("    "+quote(item["text"])+("" if n==7 else ","))
    out.extend(["}};","struct CodexRealmSummary {const char* id;const char* name;const char* hub;int stage;bool initialFootholdSource;};",
                "inline constexpr std::array<CodexRealmSummary,9> CodexRealmSummaries = {{"])
    for n,key in enumerate(REALMS):
        entry=by["realm."+key]
        contract=entry["rule_contract"]
        args=(quote(entry["id"]),quote(entry["name"]),quote(contract["hub"]),
              str(contract["stage"]),("true" if n<3 else "false"))
        out.append("    {"+",".join(args)+"}"+("" if n==8 else ","))
    out.extend(["}};","} // namespace UnmadeCore"])
    return "\n".join(out)+"\n"


CRAFT_OUTPUT=ROOT/"Source/TheUnmadeGame/Public/Items/UnmadeCodexCraftingGenerated.inc"
CRAFT_IDS=("TemperedEdge","HerbSalve","ScribedStillness","LanternMail","InkboundCharm","LifeAsh")

def generate_crafting(codex):
    by={row["id"]:row for row in codex["records"]}
    out=["// GENERATED FROM Authoring/ability_codex.json. Do not edit.",
         "// This table is included inside namespace UnmadeCore after RecipeDef.",
         "inline constexpr std::array<RecipeDef,6> Recipes = {{"]
    for i,key in enumerate(CRAFT_IDS):
        rec=by["recipe.workshop."+key]
        data=rec["rule_contract"]
        inputs=data["inputs"]
        product=data["output"]
        if len(inputs)!=2:raise ValueError("Codex craft contract requires two material kinds")
        profession=data["profession"].split(".")[1].capitalize()
        town=data["workshop"].split(".")[1]
        material=lambda value:"ItemId::"+value["item_enum"]+","+str(value["count"])
        fields=("RecipeId::"+key,quote(rec["name"]),"Profession::"+profession,
                "SettlementId::"+town,str(data["min_skill"]),
                material(inputs[0]),material(inputs[1]),material(product))
        out.append("    {"+",".join(fields)+"}"+("" if i==5 else ","))
    out.append("}};")
    return "\n".join(out)+"\n"


ATLAS_OUTPUT=ROOT/"Source/TheUnmadeGame/Public/World/UnmadeCodexAtlasGenerated.inc"
PASSAGE_IDS=(
 ("ThreefoldReach","WidowedRain"),("ThreefoldReach","HearthBeneath"),
 ("WidowedRain","TidalLedger"),("WidowedRain","SkyBelow"),
 ("HearthBeneath","SkyBelow"),("HearthBeneath","CinderSpine"),
 ("TidalLedger","HundredUnlived"),("SkyBelow","HundredUnlived"),
 ("SkyBelow","OrchardOfKings"),("CinderSpine","OrchardOfKings"),
 ("HundredUnlived","FirstAbsence"),("OrchardOfKings","FirstAbsence"),
 ("TidalLedger","CinderSpine")
)

def generate_atlas(codex):
    by={row["id"]:row for row in codex["records"]}
    out=["// GENERATED FROM Authoring/ability_codex.json. Do not hand-edit.",
         "// These tables live inside namespace UnmadeCore after RealmSpec and AtlasPassage.",
         "inline constexpr std::array<RealmSpec,static_cast<int>(Realm::Count)> WorldAtlas = {{"]
    for index,key in enumerate(REALMS):
        row=by["realm."+key]
        info=row["rule_contract"]
        values=("Realm::"+key,quote(row["name"]),quote(info["landscape"]),
                quote(info["civilization"]),quote(row["summary"]),
                quote(info["rare_reward_theme"]),str(info["stage"]))
        out.append("    {"+",".join(values)+"}"+("" if index==8 else ","))
    out.extend(["}};","inline constexpr std::array<AtlasPassage,13> AtlasPassages = {{"])
    for index,(origin,destination) in enumerate(PASSAGE_IDS):
        ref="passage."+str(index+1).zfill(2)+"."+origin+"."+destination
        record=by[ref]["rule_contract"]
        if record["from"]!="realm."+origin or record["to"]!="realm."+destination or not record["bidirectional"] or record["source_index"]!=index:
            raise ValueError("Codex route edge identity drift "+ref)
        out.append("    {Realm::"+origin+",Realm::"+destination+","+str(record["minimum_attunement"])+"}"+("" if index==12 else ","))
    out.append("}};")
    return "\n".join(out)+"\n"


EXPEDITION_OUTPUT=ROOT/"Source/TheUnmadeGame/Public/World/UnmadeCodexExpeditionGenerated.inc"

def generate_expeditions(codex):
    by={row["id"]:row for row in codex["records"]}
    out=["// GENERATED FROM Authoring/ability_codex.json. Do not hand-edit.",
         "// Included inside namespace UnmadeCore after ExpeditionSpec.",
         "inline constexpr std::array<ExpeditionSpec,9> ExpeditionSpecs={{"]
    for index,key in enumerate(REALMS):
        row=by["journey.expedition."+key]
        spec=row["rule_contract"]
        if spec["source_index"]!=index:
            raise ValueError("Expedition ordinal drift "+key)
        strings=(spec["city"],spec["frontier"],spec["refuge"],spec["dungeon"],
                 spec["boss"],spec["weapon"],spec["armor"],spec["technique"],
                 row["summary"],spec["kill_return"],spec["mercy_return"])
        if len(spec["people"])!=5 or len(spec["clues"])!=3 or spec["kill_return"]==spec["mercy_return"]:
            raise ValueError("Expedition needs five people, three clues and two different outcomes")
        origin=spec["world_origin"]
        parts=("Realm::"+key,)+tuple(quote(x) for x in strings)+(
              "{{"+",".join(quote(x) for x in spec["people"])+"}}",
              "{{"+",".join(quote(x) for x in spec["clues"])+"}}",
              str(origin["x"]),str(origin["y"]))
        out.append("    {"+",".join(parts)+"}"+("" if index==8 else ","))
    out.append("}};")
    return "\n".join(out)+"\n"


EXPERIENCE_OUTPUT=ROOT/"Source/TheUnmadeGame/Public/World/UnmadeCodexExperienceGenerated.inc"

def generate_experience(codex):
    by={row["id"]:row for row in codex["records"]}
    out=["// GENERATED FROM Authoring/ability_codex.json. Do not hand-edit.",
         "// Included inside namespace UnmadeCore after ExperienceSpec.",
         "inline constexpr ExperienceSpec ExperienceSpecs[9]={"]
    for index,realm in enumerate(REALMS):
        entry=by["journey.wing."+realm]
        c=entry["rule_contract"]
        if c["source_index"]!=index:
            raise ValueError("Wing source index drift "+realm)
        if len(c["scenes"])!=4 or len(c["wing_grid"])!=3 or any(len(pair)!=2 for pair in c["wing_grid"]):
            raise ValueError("Wing layout and four witness reactions required for "+realm)
        strings=(c["city"],c["wing"],c["guardian"],c["interior"],c["specialty"],
                 c["task"],c["hint"],c["tool"],c["recipe"],c["reserve_return"],c["witness_return"])
        scenes="{"+",".join(quote(s) for s in c["scenes"])+"}"
        grid="{"+",".join("{"+",".join(str(v) for v in pair)+"}" for pair in c["wing_grid"])+"}"
        parts=tuple(quote(s) for s in strings)+(scenes,grid)
        out.append("    {"+",".join(parts)+"}"+("" if index==8 else ","))
    out.append("};")
    return "\n".join(out)+"\n"


CAMPAIGN_OUTPUT=ROOT/"Source/TheUnmadeGame/Public/World/UnmadeCodexCampaignGenerated.inc"

def generate_campaign(codex):
    by={row["id"]:row for row in codex["records"]}
    out=["// GENERATED FROM Authoring/ability_codex.json. Do not hand-edit.",
         "// Included after RoadBeat inside namespace UnmadeCore.",
         "inline constexpr std::array<RoadBeat,9> UnansweredRoadBeats={{"]
    for index,realm in enumerate(REALMS):
        row=by["quest.campaign."+realm]
        c=row["rule_contract"]
        if c["source_index"]!=index:
            raise ValueError("Campaign source ordinal drift: "+realm)
        args=("Realm::"+realm,quote(row["name"]),quote(c["central_question"]),
              quote(c["witnessed_discovery"]),quote(c["next_direction"]),quote(c["public_event"]))
        out.append("    {"+",".join(args)+"}"+("" if index==8 else ","))
    out.append("}};")
    return "\n".join(out)+"\n"


DEPTH_OUTPUT=ROOT/"Source/TheUnmadeGame/Public/World/UnmadeCodexPersonalQuestsGenerated.inc"
PERSONAL_FIELDS=("offer","witness","evidence","option_a","option_b","return_a",
                 "return_b","challenge","secret_name","power","counterparty",
                 "witness_return_a","witness_return_b")

def generate_personal_quests(codex):
    by={row["id"]:row for row in codex["records"]}
    out=["// GENERATED FROM Authoring/ability_codex.json. Do not hand-edit.",
         "// Included inside namespace UnmadeCore after DepthSpec.",
         "inline constexpr DepthSpec DepthSpecs[]={"]
    for index,key in enumerate(REALMS):
        row=by["quest.personal."+key]
        contract=row["rule_contract"]
        if contract["source_order"]!=index or contract["option_a"]==contract["option_b"] or contract["return_a"]==contract["return_b"]:
            raise ValueError("Personal quest alternatives/ordinal drift: "+key)
        values=(row["name"],contract["goal"])+tuple(contract[k] for k in PERSONAL_FIELDS)
        out.append("    {"+",".join(quote(v) for v in values)+"}"+("" if index==8 else ","))
    out.append("};")
    return "\n".join(out)+"\n"

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--check",action="store_true")
    args=parser.parse_args()
    codex=json.loads(CODEX.read_text(encoding="utf-8"))
    generated=generate(codex)
    crafting=generate_crafting(codex)
    atlas=generate_atlas(codex)
    expeditions=generate_expeditions(codex)
    experience=generate_experience(codex)
    campaign=generate_campaign(codex)
    personal=generate_personal_quests(codex)
    if args.check:
        if not OUTPUT.is_file() or OUTPUT.read_text(encoding="utf-8")!=generated:
            print("FAIL: Codex-generated Unreal header is not identical to the canonical Codex.")
            return 1
        if not DEPTH_OUTPUT.is_file() or DEPTH_OUTPUT.read_text(encoding="utf-8")!=personal:
            print("FAIL: Codex-generated personal choice and witness quest data is stale.")
            return 1
        if not CAMPAIGN_OUTPUT.is_file() or CAMPAIGN_OUTPUT.read_text(encoding="utf-8")!=campaign:
            print("FAIL: Codex-generated campaign chapter data differs from authored Codex.")
            return 1
        if not EXPERIENCE_OUTPUT.is_file() or EXPERIENCE_OUTPUT.read_text(encoding="utf-8")!=experience:
            print("FAIL: Codex-generated experience wing / room graph is stale.")
            return 1
        if not EXPEDITION_OUTPUT.is_file() or EXPEDITION_OUTPUT.read_text(encoding="utf-8")!=expeditions:
            print("FAIL: Codex-generated expedition content differs from authoring.")
            return 1
        if not ATLAS_OUTPUT.is_file() or ATLAS_OUTPUT.read_text(encoding="utf-8")!=atlas:
            print("FAIL: Codex-generated world atlas or passage graph differs from authoring.")
            return 1
        if not CRAFT_OUTPUT.is_file() or CRAFT_OUTPUT.read_text(encoding="utf-8")!=crafting:
            print("FAIL: Codex-generated crafting rules differ from canonical recipe data.")
            return 1
        print("PASS: Codex-generated runtime and crafting rules match authoring JSON.")
        return 0
    OUTPUT.write_text(generated,encoding="utf-8")
    CRAFT_OUTPUT.write_text(crafting,encoding="utf-8")
    ATLAS_OUTPUT.write_text(atlas,encoding="utf-8")
    EXPEDITION_OUTPUT.write_text(expeditions,encoding="utf-8")
    EXPERIENCE_OUTPUT.write_text(experience,encoding="utf-8")
    CAMPAIGN_OUTPUT.write_text(campaign,encoding="utf-8")
    DEPTH_OUTPUT.write_text(personal,encoding="utf-8")
    print("Wrote",OUTPUT.relative_to(ROOT),CRAFT_OUTPUT.relative_to(ROOT),ATLAS_OUTPUT.relative_to(ROOT),EXPEDITION_OUTPUT.relative_to(ROOT),EXPERIENCE_OUTPUT.relative_to(ROOT),CAMPAIGN_OUTPUT.relative_to(ROOT),DEPTH_OUTPUT.relative_to(ROOT))
    return 0

if __name__=="__main__":raise SystemExit(main())
