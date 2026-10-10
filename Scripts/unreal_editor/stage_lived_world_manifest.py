#!/usr/bin/env python3
"""Offline deterministic 90-work-order visual staging contract.

A dedicated editor map holds *non-colliding reference cubes*, never quest
completion actors. Actor IDs and nine realm coordinates remain source-derived.
"""
from __future__ import annotations
import json
import re
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
MANIFEST=ROOT/"Authoring/generated/lived_universe_scene_manifest.json"
MAP="/Game/UnmadeProduction/Maps/Unmade_LivedWorldStaging"
COORDS={
 "ThreefoldReach": (0,0), "WidowedRain": (0,-50000),
 "HearthBeneath": (0,50000), "TidalLedger": (100000,-50000),
 "SkyBelow": (-100000,-50000), "CinderSpine": (100000,50000),
 "HundredUnlived": (-100000,50000), "OrchardOfKings": (200000,0),
 "FirstAbsence": (-200000,0)
}
KINDS={"physical_evidence":3,"resident_daily_scene":2,
       "optional_side_quest":1,"art_audio_work_package":4}
def read_plan(path=MANIFEST):
    obj=json.loads(path.read_text(encoding="utf-8"))
    if obj.get("schema_version")!=1 or obj.get("counts",{}).get("total")!=90:
        raise ValueError("Lived-universe work-order schema/count changed")
    records=obj.get("work_orders",[])
    if len(records)!=90 or len({r["id"] for r in records})!=90:
        raise ValueError("Lived-universe work-order identity drift")
    labels=[r["actor_label"] for r in records]
    if len(set(labels))!=90:
        raise ValueError("Lived-universe reference labels duplicated")
    for realm in COORDS:
        own=[r for r in records if r["realm"]==realm]
        if len(own)!=10:
            raise ValueError("Each realm must have exactly ten work orders")
        for kind,n in KINDS.items():
            if sum(r["kind"]==kind for r in own)!=n:
                raise ValueError("Lived-work-order kind distribution changed")
        for r in own:
            if (not re.fullmatch(r"GUIDE_[A-Za-z0-9_]+",r["actor_label"]) or
                r["collision"] is not False or
                r["gameplay_authority"] is not False or
                r["engine_status"]!="not_built"):
                raise ValueError("Invalid read-only work order "+r["id"])
            if not r["site"] or not r["action"]:
                raise ValueError("Missing visual job metadata")
    return obj

def places(obj):
    count={r:0 for r in COORDS}
    for job in obj["work_orders"]:
        realm=job["realm"]
        index=count[realm]
        count[realm]+=1
        base_x,base_y=COORDS[realm]
        # Two reference rows, five markers each, enough open working area.
        x=base_x + (index%5-2)*380
        y=base_y + (index//5-0.5)*680
        z=190
        yield job,(x,y,z)

def apply_in_editor(obj):
    import unreal
    if "Unmade_LivedWorldStaging" not in unreal.EditorLevelLibrary.get_editor_world().get_name():
        raise RuntimeError("Refusing live-world writes: use the isolated lived-world staging map")
    old={a.get_actor_label():a for a in unreal.EditorLevelLibrary.get_all_level_actors()}
    mesh=unreal.load_asset("/Engine/BasicShapes/Cube.Cube")
    if not mesh:raise RuntimeError("UE built-in cube unavailable")
    created=skipped=0
    for order,pos in places(obj):
        label=order["actor_label"]
        if label in old:
            skipped+=1
            continue
        a=unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor,
            unreal.Vector(*pos))
        if not a:raise RuntimeError("Unable to spawn staging-only marker: "+label)
        a.set_actor_label(label)
        comp=a.get_component_by_class(unreal.StaticMeshComponent)
        if comp:
            comp.set_static_mesh(mesh)
            comp.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        # Distinguish physical clues, social encounters, side stories, and
        # asset/film tickets by marker height, without relying on color alone.
        zscale={"physical_evidence":2.0,"resident_daily_scene":1.4,
                "optional_side_quest":2.7,"art_audio_work_package":0.8}[order["kind"]]
        a.set_actor_scale3d(unreal.Vector(.52,.52,zscale))
        created+=1
    if created:unreal.EditorLevelLibrary.save_current_level()
    return created,skipped

def main():
    obj=read_plan()
    entries=list(places(obj))
    if len(entries)!=90:raise ValueError("Marker coordinate plan malformed")
    print(f"DRY RUN ONLY: {len(entries)} lived-world reference markers across {len(COORDS)} realms.")
    return 0

if __name__=="__main__":
    raise SystemExit(main())
