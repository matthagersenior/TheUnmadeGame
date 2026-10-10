#!/usr/bin/env python3
"""Single-source, standard-library-only cinematic and nine-realm production validation.
Generates UE-importable CSVs; no engine, assets, network or paid tools necessary.
"""
from pathlib import Path
import argparse, csv, io, json, re, sys
ROOT=Path(__file__).resolve().parents[1]
SHOT=ROOT/"Authoring/cinematic_explainer_v1.json"
REALM=ROOT/"Authoring/cinematic_realm_playbook.json"
OUT=ROOT/"Authoring/generated"
SHOT_FIELDS={"title":"Title","subtitle":"Subtitle","narration":"Narration",
             "art_ref":"ArtSource","start_seconds":"StartSeconds",
             "duration_seconds":"DurationSeconds","end_seconds":"EndSeconds",
             "director_note":"DirectorNote"}
REALM_FIELDS={"id":"RealmId","display":"Display","signature":"Signature",
              "landscape":"Landscape","main_question":"MainQuestion",
              "mechanism":"Mechanism","danger":"Danger","care":"CareOutcome",
              "truth":"TruthOutcome","enemy":"Enemy","ability":"Ability",
              "foreshadow":"Foreshadow","return":"ReturnChapter","held":"HeldMorning",
              "many":"ManyMornings","test":"ManualTest"}

def verify(shots,world):
    names=["ThreefoldReach","WidowedRain","HearthBeneath","TidalLedger",
           "SkyBelow","CinderSpine","HundredUnlived","OrchardOfKings","FirstAbsence"]
    if shots.get("schema_version")!=1 or world.get("schema_version")!=1:
        raise ValueError("Schema expected 1")
    if len(shots.get("shots",[]))!=16 or len(world.get("realms",[]))!=9:
        raise ValueError("Expected 16 cinematic shots and 9 realm recipes")
    enum=(ROOT/"Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h").read_text()
    declared=enum.split("enum class Realm : int {",1)[1].split("};",1)[0]
    canon=[a.strip() for a in declared.split(",") if a.strip() and a.strip()!="Count"]
    if canon!=names or world["canonical_order"]!=names:
        raise ValueError("Atlas realm IDs have drifted from cinematic playbook")
    ids=[x["id"] for x in world["realms"]]
    if ids!=names or len(ids)!=len(set(ids)):
        raise ValueError("Realm recipe sequence incorrect")
    for r in world["realms"]:
        required=("display","signature","landscape","main_question","mechanism",
                  "danger","care","truth","enemy","ability","foreshadow","return",
                  "held","many","test")
        for k in required:
            if not isinstance(r.get(k),str) or len(r[k])<18:
                raise ValueError(f"Incomplete production note {r['id']}:{k}")
        if len(r.get("main_verbs",[]))!=3 or len(r.get("assets",[]))<4:
            raise ValueError("Need three physical actions and four asset IDs")
        if len(r.get("cast",[]))<3:
            raise ValueError("At least three people per region")
        if r["care"]==r["truth"] or r["held"]==r["many"]:
            raise ValueError("Distinct player consequences required")
    timeline=0.0
    for i,s in enumerate(shots["shots"]):
        if s["id"]!=f"S{i+1:02}" or s["duration_seconds"]<6:
            raise ValueError("Shot ID or pace inconsistent")
        for k in ("title","subtitle","art_ref","narration","director_note"):
            if not isinstance(s.get(k),str) or len(s[k])<8:
                raise ValueError(f"Incomplete cinematic shot {s['id']}:{k}")
        if abs(s["start_seconds"]-timeline)>0.001:
            raise ValueError("Gap or overlap in cinematic timeline")
        timeline+=s["duration_seconds"]
        if abs(s["end_seconds"]-timeline)>0.001:
            raise ValueError("Shot end doesn't match duration")
        if not re.fullmatch(r"legacy_art_\d{2}\.(jpg|png)",s["art_ref"]):
            raise ValueError("Concept art reference invalid")
        forbidden=("first victory was a lie","held morning","many mornings",
                   "optional echo rematch")
        if any(term in s["narration"].lower() for term in forbidden):
            raise ValueError("Public explainer leaks the behind-the-scenes ending")
    if abs(timeline-shots["film_duration_seconds"])>.001:
        raise ValueError("Cinematic expected duration drift")
    return world,shots

def make_csv(rows,columns,identity):
    f=io.StringIO(newline="")
    writer=csv.writer(f,lineterminator="\n")
    writer.writerow(["Name",*columns.values()])
    for row in rows:
        writer.writerow([row[identity],*[row[field] for field in columns]])
    return f.getvalue()

def generated(world,shots):
    return {
        OUT/"cinematic_shots_unreal.csv":make_csv(shots["shots"],SHOT_FIELDS,"id"),
        OUT/"cinematic_realms_unreal.csv":make_csv(world["realms"],REALM_FIELDS,"id")
    }

def main():
    p=argparse.ArgumentParser()
    group=p.add_mutually_exclusive_group(required=True)
    group.add_argument("--write",action="store_true")
    group.add_argument("--check",action="store_true")
    a=p.parse_args()
    shots=json.loads(SHOT.read_text(encoding="utf-8"))
    world=json.loads(REALM.read_text(encoding="utf-8"))
    verify(shots,world)
    drift=[]
    for name,text in generated(world,shots).items():
        if a.write:
            name.parent.mkdir(parents=True,exist_ok=True)
            name.write_text(text,encoding="utf-8",newline="")
        elif not name.is_file() or name.read_text(encoding="utf-8")!=text:
            drift.append(str(name.relative_to(ROOT)))
    if drift:
        print("Generated editor files missing or stale:",*drift,sep="\n  ")
        return 1
    print("PASS: 16 timed concept shots / 9 realm production recipes / exact atlas IDs / deterministic Unreal CSVs")
    return 0
if __name__=="__main__":sys.exit(main())
