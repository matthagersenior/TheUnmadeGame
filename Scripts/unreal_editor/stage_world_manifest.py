#!/usr/bin/env python3
"""UNMADE: idempotent UE Editor scene staging from authored JSON.
Defaults to safe inspection; apply only inside dedicated staging map.
Offline --dry-run works on phone/CI with standard Python 3.11.
"""
import argparse,json,sys,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
MANIFEST=ROOT/"Authoring/generated/scene_build_manifest.json"
STATIC_MESH_ROOT="/Game/UnmadeProduction/Assets"

def static_mesh_asset_path(asset_hint):
    """Canonical Unreal object path for a staged static-mesh hint, if applicable."""
    if not isinstance(asset_hint,str) or not re.fullmatch(r"SM_[A-Za-z0-9_]+",asset_hint):
        return None
    return f"{STATIC_MESH_ROOT}/{asset_hint}.{asset_hint}"

def resolve_static_mesh(unreal,asset_hint,fallback):
    """Load a production mesh when imported; otherwise retain the visible cube proxy."""
    path=static_mesh_asset_path(asset_hint)
    if path:
        asset=unreal.load_asset(path)
        if asset is not None and isinstance(asset,unreal.StaticMesh):
            return asset,path
    return fallback,None
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
    updated=0
    for item in m["actors"]:
        label=item["label"]
        mesh,mesh_path=resolve_static_mesh(unreal,item.get("asset_hint"),default_mesh)
        if label in existing:
            actor=existing[label]
            component=actor.get_component_by_class(unreal.StaticMeshComponent)
            # Upgrade only an untouched engine-cube placeholder. Any custom
            # artist mesh remains intact when the staging plan is re-run.
            if mesh_path and component:
                current=component.get_editor_property("static_mesh")
                current_path=current.get_path_name() if current else ""
                if current_path==default_mesh.get_path_name():
                    component.set_static_mesh(mesh)
                    updated+=1
            skipped+=1
            continue
        x,y,z=item["position_cm"]
        a=unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.StaticMeshActor,unreal.Vector(x,y,z))
        if not a:raise RuntimeError("Could not spawn "+label)
        a.set_actor_label(label)
        component=a.get_component_by_class(unreal.StaticMeshComponent)
        if component:
            component.set_static_mesh(mesh)
            component.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        a.set_actor_scale3d(unreal.Vector(.6,.6,1.4))
        # No SaveGame, quest-choice or authoritative actor tags changed.
        created+=1
    if created>0 or updated>0:
        unreal.EditorLevelLibrary.save_current_level()
    unreal.log("THE UNMADE scene staging: created %d, mesh-upgraded %d, skipped %d stable labels"
               %(created,updated,skipped))
    return created,skipped

def main():
    p=argparse.ArgumentParser(description="Safe repeatable Unreal level staging")
    p.add_argument("--apply",action="store_true",
        help="actually create noncolliding cubes in open Unmade_AuthoringStaging")
    p.add_argument("--manifest",type=Path,default=MANIFEST)
    a=p.parse_args()
    manifest=read_plan(a.manifest)
    if not a.apply:
        mesh_hints={x["asset_hint"] for x in manifest["actors"]
                    if static_mesh_asset_path(x.get("asset_hint"))}
        print("DRY RUN: %d staged references / %d optional static-mesh bindings, dedicated map %s. NO EDITOR WRITES."
             %(len(manifest["actors"]),len(mesh_hints),manifest["editor_level_name"]))
        print("Mesh folder: %s/<SM_AssetName>.<SM_AssetName>; missing meshes use safe cube proxies."
             %STATIC_MESH_ROOT)
        for group in ("RealmProductionReference","CinematicShotReference"):
            print(group,sum(x["kind"]==group for x in manifest["actors"]))
        return 0
    created,skipped=apply_in_editor(manifest)
    print("SUCCESS: staged %d new, skipped %d preexisting actors"%(created,skipped))
    return 0
if __name__=="__main__":sys.exit(main())
