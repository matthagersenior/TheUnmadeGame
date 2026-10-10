#!/usr/bin/env python3
"""Deterministic, offline content pipeline for THE UNMADE.
JSON master -> tested portable NPC source, Unreal DataTable CSV, full print-ready
production dossiers. CI --check forbids source/content drift.
"""
import argparse
import csv
import io
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MASTER = ROOT / "Authoring/world_content_pack.json"
OUT = ROOT / "Authoring/generated"
HEADER = ROOT / "Source/TheUnmadeGame/Public/Authoring/UnmadeOuterPeopleData.h"
LATER = {"TidalLedger", "SkyBelow", "CinderSpine",
         "HundredUnlived", "OrchardOfKings", "FirstAbsence"}
NPC_FIELDS = ("id", "display_name", "realm", "day_line", "night_line",
              "desire", "fear", "knowledge_boundary", "voice_direction",
              "visual_direction", "gesture")
REALM_FIELDS = ("id", "culture", "architecture", "public_principle",
                "ritual", "sound_direction", "visual_direction",
                "optional_activity", "central_dispute")
ABILITY_FIELDS = ("id", "existing_site", "physical_verb", "telegraph",
                  "limitation", "synergy", "outer_realm_uses")

def verify(data):
    if data.get("schema_version") != 1:
        raise ValueError("Unsupported authoring schema: expected 1")
    realms, people, abilities = (data.get(key, []) for key in
                                  ("realms", "npcs", "abilities"))
    if (len(realms), len(people), len(abilities)) != (9,18,10):
        raise ValueError("Expected 9 realms, 18 local named people, 10 rites")
    def unique(rows, field):
        values = [r[field] for r in rows]
        if len(values) != len(set(values)): raise ValueError("Repeated " + field)
    for group, fields in ((realms, REALM_FIELDS),(people,NPC_FIELDS),
                          (abilities,ABILITY_FIELDS)):
        for entry in group:
            for field in fields:
                value = entry.get(field)
                if not isinstance(value, str) or len(value.strip()) < 4:
                    raise ValueError("Missing meaningful authoring field: " + field)
        unique(group, "id")
    realm_src = (ROOT / "Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h").read_text()
    rite_src = (ROOT / "Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h").read_text()
    witness_src = (ROOT / "Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h").read_text()
    actual_realm = realm_src.split("enum class Realm : int {",1)[1].split("};",1)[0]
    actual_rite = rite_src.split("enum class RiteId : int {",1)[1].split("};",1)[0]
    def enum_ids(text):
        return [x.strip().split("=")[0].strip() for x in text.split(",")
                if x.strip() and x.strip()!="Count"]
    if [r["id"] for r in realms] != enum_ids(actual_realm):
        raise ValueError("Realm enum and production pack differ")
    if [r["id"] for r in abilities] != enum_ids(actual_rite):
        raise ValueError("Rite enum and production pack differ")
    for r in realms:
        if r["id"] not in realm_src: raise ValueError("Unknown realm")
    for p in people:
        if p["id"] not in witness_src: raise ValueError("Unregistered NPC: "+p["id"])
        if p["realm"] not in LATER: raise ValueError("Unexpected person realm")
        if len(p["day_line"]) < 48 or len(p["night_line"]) < 48:
            raise ValueError("Generic dialogue cannot replace personal voice")
    for r in LATER:
        if sum(1 for p in people if p["realm"]==r)!=3:
            raise ValueError("Each later realm needs three authored witnesses")
    rite_names={a["id"] for a in abilities}
    for a in abilities:
        if a["synergy"] not in rite_names:
            raise ValueError("Missing mastered rite synergy "+a["synergy"])
    return data

def cpp(data):
    quote=lambda x:json.dumps(x,ensure_ascii=False)
    lines=[
        "#pragma once",
        "// GENERATED from Authoring/world_content_pack.json. Do not edit directly.",
        "// Regenerate with: python Scripts/build_world_content.py --write",
        "#include <array>",
        "#include <cstring>",
        "namespace UnmadeCore {",
        "struct OuterPersonSpec {",
        "    const char* id; const char* displayName; const char* realm;",
        "    const char* dayLine; const char* nightLine;",
        "};",
        f"inline constexpr std::array<OuterPersonSpec,{len(data['npcs'])}> OuterPeople = {{{{"
    ]
    for p in data["npcs"]:
        lines.append("    {"+",".join(quote(p[k]) for k in
                 ("id","display_name","realm","day_line","night_line"))+"},")
    lines += [
        "}};",
        "inline const OuterPersonSpec* FindOuterPerson(const char* id) noexcept {",
        "    if(!id)return nullptr;",
        "    for(const auto& p:OuterPeople)if(std::strcmp(id,p.id)==0)return &p;",
        "    return nullptr;",
        "}",
        "} // namespace UnmadeCore",
        ""
    ]
    return "\n".join(lines)

def csv_table(rows, columns):
    output=io.StringIO(newline="")
    writer=csv.writer(output,lineterminator="\n")
    writer.writerow(["Name",*columns.values()])
    for row in rows:
        writer.writerow([row["id"].replace(".","_"),
                         *(row[k] for k in columns)])
    return output.getvalue()

def dossiers(data):
    lines=[
        "# THE UNMADE — Nine-Realm Production Dossiers",
        "",
        "Generated from `Authoring/world_content_pack.json`. Source-authored concepts and",
        "dialogue; NOT UE-rendered maps, recorded performances or implemented rite combos.",
        "",
        "## Nine cultures and their production identities",
        ""
    ]
    fields=[("architecture","Built environment"),("public_principle","Ethic"),
            ("ritual","Everyday custom"),("sound_direction","Audio"),
            ("visual_direction","Art / accessibility"),("optional_activity","Noncombat activity"),
            ("central_dispute","Living conflict")]
    for r in data["realms"]:
        lines.extend(["### "+r["culture"]+" — "+r["id"],""])
        for k,label in fields: lines.append("**"+label+":** "+r[k])
        lines.append("")
    lines.extend(["## Eighteen local witnesses / individual actor dossiers",""])
    pf=[("realm","Home"),("day_line","Day dialogue"),("night_line","Night dialogue"),
        ("desire","Desire"),("fear","Fear"),("knowledge_boundary","Knows / cannot know"),
        ("voice_direction","Voice direction"),("visual_direction","Model / costume"),
        ("gesture","Signature animation")]
    for p in data["npcs"]:
        lines.extend(["### "+p["display_name"]+" — `"+p["id"]+"`",""])
        for k,label in pf:lines.append("**"+label+":** "+p[k])
        lines.append("")
    lines.extend(["## Ten canonical reality abilities / authored scene execution",""])
    af=[("existing_site","Existing training site"),("physical_verb","World-space verb"),
        ("telegraph","Player feedback"),("limitation","Fair limit"),
        ("synergy","Combination"),("outer_realm_uses","Authored later-realm use")]
    for a in data["abilities"]:
        lines.extend(["### "+a["id"],""])
        for k,label in af:lines.append("**"+label+":** "+a[k])
        lines.append("")
    lines.extend(["## Implementation honesty","",
      "NPC day/night lines are used by the source-spawned prototype actors. The",
      "full visual, sonic and mechanical brief is editor-production content,",
      "not automatically constructed maps or final animations.",
      ""])
    return "\n".join(lines)

def render(data):
    npc_columns={
      "id":"StableId","display_name":"DisplayName","realm":"Realm",
      "day_line":"DayLine","night_line":"NightLine","desire":"Desire",
      "fear":"Fear","knowledge_boundary":"KnowledgeBoundary",
      "voice_direction":"VoiceDirection","visual_direction":"VisualDirection",
      "gesture":"Gesture"
    }
    realm_columns={
      "id":"Realm","culture":"Culture","architecture":"Architecture",
      "public_principle":"PublicPrinciple","ritual":"Ritual",
      "sound_direction":"SoundDirection","visual_direction":"VisualDirection",
      "optional_activity":"OptionalActivity","central_dispute":"CentralDispute"
    }
    ability_columns={
      "id":"Rite","existing_site":"ExistingSite","physical_verb":"PhysicalVerb",
      "telegraph":"Telegraph","limitation":"Limitation","synergy":"Synergy",
      "outer_realm_uses":"OuterRealmUses"
    }
    return {
      HEADER:cpp(data),
      OUT / "npcs_unreal.csv":csv_table(data["npcs"],npc_columns),
      OUT / "realms_unreal.csv":csv_table(data["realms"],realm_columns),
      OUT / "abilities_unreal.csv":csv_table(data["abilities"],ability_columns),
      OUT / "WORLD_CONTENT_DOSSIERS.md":dossiers(data)
    }

def main():
    parser=argparse.ArgumentParser()
    mode=parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--write",action="store_true")
    mode.add_argument("--check",action="store_true")
    args=parser.parse_args()
    data=verify(json.loads(MASTER.read_text(encoding="utf-8")))
    files=render(data)
    drift=[]
    for path,content in files.items():
        if args.write:
            path.parent.mkdir(parents=True,exist_ok=True)
            path.write_text(content,encoding="utf-8",newline="")
        elif not path.is_file() or path.read_text(encoding="utf-8") != content:
            drift.append(str(path.relative_to(ROOT)))
    if drift:
        print("Authoring outputs stale (run --write and commit):", *drift, sep="\n  ")
        return 1
    print(f"PASS: 9 worlds, 18 NPCs, 10 canonical abilities, {len(files)} deterministic artifacts")
    return 0

if __name__=="__main__":sys.exit(main())
