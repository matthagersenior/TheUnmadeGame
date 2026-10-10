#!/usr/bin/env python3
"""Repeatable source-authored eleven-city 3D assembly and production guide plan.

Outputs are noncolliding, isolated Unreal Editor rehearsal geometry. Nothing
here changes SaveGame, gameplay actor tags, quest completion or the player's
choice. Run --check offline / in GitHub CI; --json exports an inspectable plan.
"""
from __future__ import annotations
import argparse
from collections import Counter
import json
import math
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
KITS=ROOT/"Authoring/city_assembly_kits_v1.json"
CANON=ROOT/"Authoring/transmedia_storyworld_v1.json"
PEOPLE=ROOT/"Authoring/resident_return_encounters.json"
QUESTS=ROOT/"Authoring/main_quest_interaction_contracts.json"
RITES=ROOT/"Authoring/world_content_pack.json"
LIVED=ROOT/"Authoring/lived_universe_atlas.json"
STAGING_MAP="/Game/UnmadeProduction/Maps/Unmade_CityAssemblyStaging"
KINDS={"landmark","bridge","civic","social","architecture","evidence","path"}
CITY_REALMS=["ThreefoldReach","WidowedRain","HearthBeneath","TidalLedger",
             "SkyBelow","CinderSpine","HundredUnlived","OrchardOfKings","FirstAbsence"]
# The 10 rites keep their original native C++ IDs and actual authored sites.
RITE_CITY={
    "UnwriteLaw":"Bellwold", "Witnesscraft":"The Crossings",
    "BorrowedLives":"Paperhaven", "LegacyForging":"Bellwold",
    "LivingRoads":"The Crossings","TomorrowDebt":"The Crossings",
    "UnderstandingBosses":"Bellwold","ParadoxConvergence":"Paperhaven",
    "Oathbinding":"Bellwold","Cartography":"Saltwake"
}
CENTRE_CITY={"ThreefoldReach":"The Crossings","WidowedRain":"Saltwake",
    "HearthBeneath":"Cinderhold","TidalLedger":"Drevlach","SkyBelow":"Orravane",
    "CinderSpine":"Vathless","HundredUnlived":"Eillun",
    "OrchardOfKings":"Tharniv","FirstAbsence":"Auvren"}
REACH_QUEST_CITY=["The Crossings","Bellwold","Paperhaven","Bellwold",
                 "Paperhaven","The Crossings"]

def get(path):
    return json.loads(path.read_text(encoding="utf-8"))

def city_of_person(p):
    if p["realm"]=="ThreefoldReach":
        match={"Crossings":"The Crossings","Bellwold":"Bellwold",
               "Paperhaven":"Paperhaven"}.get(p.get("settlement"))
        if match is None: raise ValueError("Unknown Threefold settlement: "+str(p))
        return match
    return CENTRE_CITY[p["realm"]]

def add(actors, ids, city, kind, slug, pos, scale, source, brief):
    if len(pos)!=3 or len(scale)!=3 or not all(
            isinstance(n,(int,float)) and math.isfinite(n) for n in pos+scale):
        raise ValueError("Invalid city marker transform")
    if any(s<=0 or s>30 for s in scale) or max(abs(v) for v in pos)>400000:
        raise ValueError("Unsafe city staging bounds")
    key=f"UM_CITY_{city['id'].replace('.','_')}_{kind}_{slug}"
    if key in ids:raise ValueError("Actor label duplicate: "+key)
    ids.add(key)
    ox,oy,oz=city["origin_cm"]
    actors.append({"label":key,"city_id":city["id"],"city":city["name"],
      "realm":city["realm"],"kind":kind,"source_id":source,
      "position_cm":[ox+pos[0],oy+pos[1],oz+pos[2]],"scale":scale,
      "collision":False,"gameplay_authority":False,"source_brief":str(brief)[:450]})

def build():
    kits=get(KITS);story=get(CANON);people=get(PEOPLE)
    quests=get(QUESTS);world=get(RITES);lived=get(LIVED)
    if kits.get("schema_version")!=1 or kits.get("city_count")!=11:
        raise ValueError("City kit version/count drift")
    cities=kits["cities"]
    canon=story["cities"]
    if [(x["id"],x["name"],x["realm"]) for x in cities]!=[
        (x["id"],x["name"],x["realm"]) for x in canon]:
        raise ValueError("City names/order/realm diverged from Volume X")
    if [x["realm"] for x in cities[:3]]!=["ThreefoldReach"]*3 or [
        c["realm"] for c in cities[3:]]!=CITY_REALMS[1:]:
        raise ValueError("Realm-city mapping drift")
    if len({c["profile"] for c in cities})!=11:raise ValueError("City silhouettes reused")
    if len({tuple(c["origin_cm"]) for c in cities})!=11:
        raise ValueError("City layout origins overlap")
    if len(people["residents"])!=82 or len(quests["quests"])!=9 or len(world["abilities"])!=10:
        raise ValueError("Resident, quest, rite scope drift")
    if [x["id"] for x in quests["quests"]]!=CITY_REALMS:
        raise ValueError("Canonical nine-realm quest order drift")
    if set(RITE_CITY)!={x["id"] for x in world["abilities"]}:
        raise ValueError("Native rite name mapped incorrectly")
    if {c["name"] for c in cities}!=set(CENTRE_CITY.values())|{
            "Bellwold","Paperhaven"}:
        raise ValueError("Not all canonical settlements present")
    by_name={c["name"]:c for c in cities}
    city_by_realm={x["id"]:x for x in lived["worlds"]}
    actors=[]; ids=set()
    for city in cities:
        if len(city["modules"])!=8:
            raise ValueError("Each recognizable city needs eight unique silhouette modules")
        if len({x["slug"] for x in city["modules"]})!=8:
            raise ValueError("Duplicate landmark in "+city["name"])
        for module in city["modules"]:
            if module["kind"] not in KINDS:
                raise ValueError("Unsupported city kit shape")
            add(actors,ids,city,"SIGNATURE",module["slug"],
                module["local_cm"],module["scale"],
                f"{city['id']}/{module['slug']}",
                city_by_realm[city["realm"]]["architecture"])
        # Source-built street topology, non-colliding cubes; measured site lines,
        # not magically navigable terrain or a final open-world landscape.
        for index,(p,s) in enumerate((
            ([0,0,15],[27,1.4,.2]),
            ([0,0,15],[1.4,27,.2]),
            ([0,1200,15],[21,1.2,.2]),
            ([-1200,0,15],[1.2,21,.2])
        )):
            add(actors,ids,city,"STREET",f"{index+1:02}",p,s,
                city["id"],"Noncolliding route rehearsal; test real walkability in UE")
        for index in range(8):
            a=index*math.tau/8
            px=round(math.cos(a)*1540)
            py=round(math.sin(a)*1540)
            add(actors,ids,city,"SHELL",f"{index+1:02}",
                [px,py,150],[2.5+(index%3)*.7,2.3+(index%2)*.7,3+index%4],
                city["id"],"Replace housing placeholder with lived-in modular exterior")
    person_labels=[]
    for p in people["residents"]:
        town=city_of_person(p)
        city=by_name[town]
        count=sum(1 for a in person_labels if a[0]==town)
        # Four residents per courtyard row; Threefold cities fit 16 each.
        pos=[-680+(count%4)*430,-960+(count//4)*370,95]
        add(actors,ids,city,"RESIDENT",f"{count+1:02}",pos,[.6,.6,1.9],
            p["id"],p["name"]+" — "+p["personal_concern"])
        person_labels.append((town,p["id"]))
    quest_steps=0
    for quest in quests["quests"]:
        steps=quest["steps"]
        if len(steps)!=6:raise ValueError("Quest step count drift")
        for index,step in enumerate(steps):
            town=REACH_QUEST_CITY[index] if quest["id"]=="ThreefoldReach" else CENTRE_CITY[quest["id"]]
            city=by_name[town]
            pos=[-970+(index%3)*960,990+(index//3)*370,125]
            add(actors,ids,city,"QUEST",f"{quest['id']}_{index+1:02}",pos,
                [.75,.75,2.6],f"{quest['chapter']}/{step['id']}",
                step["objective"]+" | "+step["failure"]+" | "+step["recovery"])
            quest_steps+=1
    for i,ability in enumerate(world["abilities"]):
        city=by_name[RITE_CITY[ability["id"]]]
        pos=[-900+(i%4)*560,-1090+(i//4)*260,105]
        add(actors,ids,city,"RITE",ability["id"],pos,[.85,.85,2.6],
            ability["id"],ability["physical_verb"]+"; limitation: "+ability["limitation"])
    for r in lived["worlds"]:
        city=by_name[CENTRE_CITY[r["id"]]]
        for i,e in enumerate(r["mystery"]["evidence"]):
            add(actors,ids,city,"CLUE",f"{i+1:02}",
                [-820+i*800,1170,75],[.8,.8,1.5],e["id"],
                e["site"]+" | "+e["verb"])
        add(actors,ids,city,"SIDEQUEST","01",[980,980,110],[.9,.9,2.2],
            r["side"]["id"],r["side"]["name"]+" | "+r["side"]["objective"])
    tally=Counter(a["kind"] for a in actors)
    expected={"SIGNATURE":88,"STREET":44,"SHELL":88,"RESIDENT":82,
              "QUEST":54,"RITE":10,"CLUE":27,"SIDEQUEST":9}
    if dict(tally)!=expected:
        raise ValueError(f"Unexpected nine-realm assembly counts {tally}")
    if len(actors)!=402 or len(ids)!=402:
        raise ValueError("City assembly lost source actors")
    return {"schema_version":1,"mode":"NONAUTHORITATIVE_CITY_REHEARSAL",
      "editor_map":STAGING_MAP,
      "source":["Authoring/city_assembly_kits_v1.json",
        "Authoring/transmedia_storyworld_v1.json",
        "Authoring/resident_return_encounters.json",
        "Authoring/main_quest_interaction_contracts.json",
        "Authoring/world_content_pack.json",
        "Authoring/lived_universe_atlas.json"],
      "actor_count":len(actors),"cities":len(cities),
      "counts":dict(tally),
      "status":"Noncolliding modular proxy geometry, character/quest/rite anchors; NOT UE-playtested, final collision, dialogue UI, navigation, animation or production assets",
      "actors":actors}

def main():
    p=argparse.ArgumentParser()
    p.add_argument("--check",action="store_true")
    p.add_argument("--json",action="store_true")
    p.add_argument("--output",type=Path,help="Export preview JSON to explicit local path")
    args=p.parse_args()
    result=build()
    if args.output:
        args.output.write_text(json.dumps(result,ensure_ascii=False,indent=2)+"\n",encoding="utf-8")
    if args.json:
        print(json.dumps(result,ensure_ascii=False,indent=2))
    else:
        print(f"PASS: {result['cities']} distinct cities, {result['actor_count']} source-bound assembly markers")
        print("Counts: "+", ".join(f"{k}={v}" for k,v in result["counts"].items()))
        print("No Unreal assets or gameplay state created by this offline check")
    return 0
if __name__=="__main__":
    raise SystemExit(main())
