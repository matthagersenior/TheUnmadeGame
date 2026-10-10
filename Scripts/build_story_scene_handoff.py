#!/usr/bin/env python3
"""Offline deterministic nine-realm quest/return/scene handoff generator.

No Unreal, pip dependencies, remote API or Windows computer needed.
--check is a CI/first-PC build gate; --write is an explicit source edit.
"""
import argparse
import csv
import io
import json
import re
import sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
AUTHOR=ROOT/"Authoring"
OUTPUT=AUTHOR/"generated"
QUESTS=AUTHOR/"main_quest_interaction_contracts.json"
RESIDENTS=AUTHOR/"resident_return_encounters.json"
REALMS=AUTHOR/"cinematic_realm_playbook.json"
SHOTS=AUTHOR/"cinematic_explainer_v1.json"
COORDS={
 "ThreefoldReach":[0,0],"WidowedRain":[0,-50000],
 "HearthBeneath":[0,50000],"TidalLedger":[100000,-50000],
 "SkyBelow":[-100000,-50000],"CinderSpine":[100000,50000],
 "HundredUnlived":[-100000,50000],"OrchardOfKings":[200000,0],
 "FirstAbsence":[-200000,0]
}
OFFSETS=[[-1400,-1100,160],[-750,-1000,160],[750,-1000,160],[1400,-1100,160]]
QUEST_COLS={"realm":"Realm","chapter":"Chapter","step":"StepId",
 "trigger":"Trigger","interaction":"Interaction","objective":"Objective",
 "speaker":"SpeakerId","line":"ExactDialogue","failure":"Failure",
 "recovery":"Recovery","saved_as":"SavedState",
 "outcome":"Outcome","physical_tag":"PhysicalTag"}
RETURN_COLS={"id":"StableId","name":"DisplayName","realm":"Realm",
 "recognize_line":"ReturnLine","aid_line":"AidLine",
 "public_line":"LocalPublicLine","post_morning":"NewMorningLine",
 "personal_concern":"PersonalConcern","next_visit_hook":"NextVisitHook"}
def load():
    return [json.loads(p.read_text(encoding="utf-8")) for p in
            (QUESTS,RESIDENTS,REALMS,SHOTS)]
def verify(q,p,w,c):
    if any(x.get("schema_version")!=1 for x in (q,p,w,c)):
        raise ValueError("All master schema versions must be 1")
    ids=[r["id"] for r in w["realms"]]
    if ids!=list(COORDS) or [r["id"] for r in q["quests"]]!=ids:
        raise ValueError("Quest/realm enum order drift")
    if len(p["residents"])!=82 or p["registered_count"]!=82:
        raise ValueError("82 registered person profiles required")
    if len(c["shots"])!=16 or len(q["act_gates"])!=7:
        raise ValueError("Expected 16 shots and 7 act gates")
    hdr=(ROOT/"Source/TheUnmadeGame/Public/World/UnmadeLaterRealmRules.h").read_text()
    for idx,quest in enumerate(q["quests"]):
        if len(quest["steps"])!=6:
            raise ValueError("Every realm must have six first/return beats")
        steps=[s["id"] for s in quest["steps"]]
        if steps!=["arrival","investigation","mechanism","care_commit",
                    "truth_commit","return"]:
            raise ValueError("Realm step structure changed")
        if quest["chapter"]!="QUEST_MAIN_"+ids[idx]:
            raise ValueError("Unstable canonical chapter ID")
        if quest["steps"][3]["line"]==quest["steps"][4]["line"]:
            raise ValueError("Care and truth cannot share an answer")
        for s in quest["steps"]:
            for field in ("trigger","interaction","objective","speaker",
                          "line","failure","recovery","saved_as"):
                if not isinstance(s.get(field),str) or len(s[field])<12:
                    raise ValueError(f"Missing {ids[idx]}/{s['id']}/{field}")
        if idx>=3:
            tags=quest["real_source_tags"]
            if not tags["contract_status"].startswith("authoritative"):
                raise ValueError("Later Realm actual site authority not marked")
            for tag in tags["evidence"]+[tags["mechanism"]]:
                if tag not in hdr:
                    raise ValueError("Invented gameplay source tag: "+tag)
        if any(n not in [r["id"] for r in p["residents"]]
               for n in quest["npc_ids"]):
            raise ValueError("Unknown speaker in "+ids[idx])
    ids_person=[r["id"] for r in p["residents"]]
    if len(set(ids_person))!=82:raise ValueError("Duplicate stable resident")
    for resident in p["residents"]:
        if resident["realm"] not in COORDS:raise ValueError("Resident lacks home")
        for k in ("recognize_line","aid_line","public_line","post_morning",
                  "personal_concern","next_visit_hook"):
            if len(resident[k])<24:raise ValueError("Unwritten resident "+k)
    # Order follows existing gameplay compiled arrays: 48 settlement, 16 frontier,
    # 18 later person IDs (exactly the same native ResidentContinuity index).
    native=[
        ROOT/"Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h",
        ROOT/"Source/TheUnmadeGame/Public/World/UnmadeFrontierRealmRules.h",
        ROOT/"Source/TheUnmadeGame/Public/Authoring/UnmadeOuterPeopleData.h"]
    expected=[]
    import re
    for path in native:
        text=path.read_text()
        if "SettlementRegistry" in path.name:
            content=text.split("inline constexpr std::array<ResidentSpec, 48> Residents",1)[1].split("}};",1)[0]
        elif "FrontierRealmRules" in path.name:
            content=text.split("inline constexpr std::array<FrontierResident,16> FrontierResidents",1)[1].split("}};",1)[0]
        else:
            content=text.split("inline constexpr std::array<OuterPersonSpec,18> OuterPeople",1)[1].split("}};",1)[0]
        expected += re.findall(r'\{\s*"(npc\.[^"]+)"',content)
    if ids_person!=expected:
        raise ValueError("Resident indices could corrupt existing saves: stable order differs")
    return True

def quest_rows(q):
    rows=[]
    for quest in q["quests"]:
        for s in quest["steps"]:
            rows.append({"id":f'{quest["id"]}_{s["id"]}',"realm":quest["id"],
                         "chapter":quest["chapter"],"step":s["id"],
                         "trigger":s["trigger"],"interaction":s["interaction"],
                         "objective":s["objective"],"speaker":s["speaker"],
                         "line":s["line"],"failure":s["failure"],
                         "recovery":s["recovery"],"saved_as":s["saved_as"],
                         "outcome":s.get("outcome",""),
                         "physical_tag":s.get("physical_tag",
                              ";".join(quest["real_source_tags"]["evidence"])
                               if s["id"]=="investigation" else "")})
    return rows

def csv_rows(records, columns, ident):
    out=io.StringIO(newline="")
    writer=csv.writer(out,lineterminator="\n")
    writer.writerow(["Name",*columns.values()])
    for r in records:
        writer.writerow([r[ident].replace(".","_"),
                         *(r[k] for k in columns)])
    return out.getvalue()

def scene_manifest(w,c):
    actors=[]
    for r in w["realms"]:
        rx,ry=COORDS[r["id"]]
        for i,hint in enumerate(r["assets"][:4]):
            dx,dy,z=OFFSETS[i]
            actors.append({"id":f'STG_{r["id"]}_{i+1:02}',
              "kind":"RealmProductionReference","realm":r["id"],
              "label":f'UM_STG_{r["id"]}_{i+1:02}',
              "position_cm":[rx+dx,ry+dy,z],
              "asset_hint":hint,"source_ref":f'QUEST_MAIN_{r["id"]}',
              "collision":False,"note":"Authoring-only stage. Does NOT replace runtime quest tagged actor."})
    for i,s in enumerate(c["shots"]):
        actors.append({"id":f'STG_SHOT_{s["id"]}',
          "kind":"CinematicShotReference","realm":"StagingOnly",
          "label":f'UM_STG_SHOT_{s["id"]}',
          "position_cm":[300000+(i%8)*900,100000+(i//8)*1500,180],
          "asset_hint":s["art_ref"],"source_ref":s["id"],
          "collision":False,"note":s["director_note"]})
    return {"schema_version":1,"editor_level_name":"Unmade_AuthoringStaging",
      "save_policy":"authoring-only, never overwrite runtime actors",
      "actors":actors,"expected_actor_count":len(actors)}

def outputs(q,p,w,c):
    return {
     OUTPUT/"main_quest_steps_unreal.csv":
       csv_rows(quest_rows(q),QUEST_COLS,"id"),
     OUTPUT/"resident_returns_unreal.csv":
       csv_rows(p["residents"],RETURN_COLS,"id"),
     OUTPUT/"scene_build_manifest.json":
       json.dumps(scene_manifest(w,c),ensure_ascii=False,indent=2)+"\n"
    }

def main():
    parser=argparse.ArgumentParser()
    modes=parser.add_mutually_exclusive_group(required=True)
    modes.add_argument("--write",action="store_true")
    modes.add_argument("--check",action="store_true")
    args=parser.parse_args()
    q,p,w,c=load()
    verify(q,p,w,c)
    expected=outputs(q,p,w,c)
    bad=[]
    for path,text in expected.items():
        if args.write:
            path.parent.mkdir(parents=True,exist_ok=True)
            path.write_text(text,encoding="utf-8",newline="")
        elif not path.is_file():
            bad.append(str(path.relative_to(ROOT)))
        elif path.suffix==".json":
            if json.loads(path.read_text(encoding="utf-8"))!=json.loads(text):
                bad.append(str(path.relative_to(ROOT)))
        elif path.read_text(encoding="utf-8")!=text:
            bad.append(str(path.relative_to(ROOT)))
    if bad:
        print("Quest/scene data out of sync; run --write:",*bad,sep="\n  ")
        return 1
    print("PASS: 9 quests / 54 authored beats / 82 returning residents / 52 idempotent staging guides")
    return 0
if __name__=="__main__":sys.exit(main())
