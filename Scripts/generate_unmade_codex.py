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

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument("--check",action="store_true")
    args=parser.parse_args()
    codex=json.loads(CODEX.read_text(encoding="utf-8"))
    generated=generate(codex)
    crafting=generate_crafting(codex)
    atlas=generate_atlas(codex)
    if args.check:
        if not OUTPUT.is_file() or OUTPUT.read_text(encoding="utf-8")!=generated:
            print("FAIL: Codex-generated Unreal header is not identical to the canonical Codex.")
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
    print("Wrote",OUTPUT.relative_to(ROOT),CRAFT_OUTPUT.relative_to(ROOT),ATLAS_OUTPUT.relative_to(ROOT))
    return 0

if __name__=="__main__":raise SystemExit(main())
