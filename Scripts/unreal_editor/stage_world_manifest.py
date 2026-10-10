#!/usr/bin/env python3
"""UNMADE: idempotent UE Editor scene staging from authored JSON.
Defaults to safe inspection; apply only inside dedicated staging map.
Offline --dry-run works on phone/CI with standard Python 3.11.
"""
import argparse,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
MANIFEST=ROOT/"Authoring/generated/scene_build_manifest.json"
def read_plan(path=MANIFEST):
    m=json.loads(path.read_text(encoding="utf-8"))
    if m.get("schema_version")!=1 or m.get("editor_level_name")!="Unmade_AuthoringStaging":
        raise ValueError("Incorrect staging schema or scene name")
    if len(m.get("actors",[]))!=52 or m.get("expected_actor_count")!=52:
        raise ValueError("Expected exactly 36 realm and 16 shot reference actors")
    ids=[x["id"] for x in m["actors"]]
    labels=[x["label"] for x in m["actors"]]
    if len(set(ids))!=52 or len(set(labels))!=52:
        raise ValueError("Duplicate actor identity")
    for actor in m["actors"]:
        if not actor["label"].startswith("UM_STG_") or actor["collision"] is not False:
            raise ValueError("Refusing non-staging or colliding actor")
        if len(actor.get("position_cm",[]))!=3 or not all(
            isinstance(v,(int,float)) and abs(v)<1e7
            for v in actor["position_cm"]):
            raise ValueError("Invalid staging position")
    return m

def apply_in_editor(m):
    import unreal
    world=unreal.EditorLevelLibrary.get_editor_world()
    if not world or "Unmade_AuthoringStaging" not in world.get_name():
        raise RuntimeError("Open dedicated Unmade_AuthoringStaging level first. No production-level writes allowed.")
    existing={a.get_actor_label():a for a in
              unreal.EditorLevelLibrary.get_all_level_actors()}
    default_mesh=unreal.load_asset("/Engine/BasicShapes/Cube.Cube")
    if not default_mesh: raise RuntimeError("BasicShapes.Cube is unavailable; no scene changes")
    created=0
    skipped=0
    for item in m["actors"]:
        label=item["label"]
        if label in existing:
            skipped+=1
            continue # Never alter an existing stage or gameplay actor.
        x,y,z=item["position_cm"]
        a=unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.StaticMeshActor,unreal.Vector(x,y,z))
        if not a:raise RuntimeError("Could not spawn "+label)
        a.set_actor_label(label)
        component=a.get_component_by_class(unreal.StaticMeshComponent)
        if component:
            component.set_static_mesh(default_mesh)
            component.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        a.set_actor_scale3d(unreal.Vector(.6,.6,1.4))
        # No SaveGame, quest-choice or authoritative actor tags changed.
        created+=1
    if created>0:
        unreal.EditorLevelLibrary.save_current_level()
    unreal.log("THE UNMADE scene staging: created %d, skipped %d stable labels"
               %(created,skipped))
    return created,skipped

def main():
    p=argparse.ArgumentParser(description="Safe repeatable Unreal level staging")
    p.add_argument("--apply",action="store_true",
        help="actually create noncolliding cubes in open Unmade_AuthoringStaging")
    p.add_argument("--manifest",type=Path,default=MANIFEST)
    a=p.parse_args()
    manifest=read_plan(a.manifest)
    if not a.apply:
        print("DRY RUN: %d staged reference actors, dedicated map %s. NO EDITOR WRITES."
             %(len(manifest["actors"]),manifest["editor_level_name"]))
        for group in ("RealmProductionReference","CinematicShotReference"):
            print(group,sum(x["kind"]==group for x in manifest["actors"]))
        return 0
    created,skipped=apply_in_editor(manifest)
    print("SUCCESS: staged %d new, skipped %d preexisting actors"%(created,skipped))
    return 0
if __name__=="__main__":sys.exit(main())
