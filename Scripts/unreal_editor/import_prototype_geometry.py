#!/usr/bin/env python3
"""Guarded Unreal 5.8 prototype-mesh importer and noncolliding city preview.

Source-only validation is safe without Unreal. Actual import requires
UNMADE_PROTO_IMPORT_APPROVED=1 and a writable receipt path from the
first-PC quickstart. Source assets never grant evidence, gear, quest flags
or saved memories, and no gameplay map is edited.
"""
from __future__ import annotations

import hashlib
import json
import os
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PREVIEW_MAP = "/Game/UnmadeProduction/Maps/Unmade_PrototypeGeometryStaging"
IMPORT_ROOT = "/Game/UnmadeProduction/PrototypeMeshes"
WORLD_DIR = ROOT/"Build/UnmadePrototypeGeometry"
EQUIPMENT_DIR = ROOT/"Build/UnmadePrototypeEquipment"
SAFE_FILE = re.compile(r"^SM_Unmade[A-Za-z0-9_]+\.gltf$")


def prepare_plan(world_dir: Path = WORLD_DIR, equipment_dir: Path = EQUIPMENT_DIR) -> dict:
    """Validate full, unmixed manifests and every source byte BEFORE any writes."""
    entries = []
    preview = []
    for kind, directory, expected, group in (
        ("world", Path(world_dir), 115, "World"),
        ("equipment", Path(equipment_dir), 78, "Equipment"),
    ):
        if not directory.is_dir():
            raise ValueError("Missing generated prototype directory: " + str(directory))
        manifest_path = directory/"manifest.json"
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
        records = manifest.get("assets") if kind=="world" else manifest.get("records")
        if manifest.get("asset_count") != expected or not isinstance(records,list) or len(records)!=expected:
            raise ValueError("Prototype manifest count changed: " + kind)
        if kind=="world" and (
                manifest.get("city_module_count") != 88 or manifest.get("clue_count") != 27):
            raise ValueError("World city/clue inventory drift")
        if kind=="equipment" and (
                manifest.get("main_catalog")!=60 or manifest.get("regional_gear")!=18):
            raise ValueError("Equipment catalog/expedition inventory drift")
        declared=set()
        for record in records:
            name=record.get("file")
            key=record.get("stable_id") if kind=="world" else record.get("id")
            if not isinstance(key,str) or not key.strip():
                raise ValueError("Empty canonical asset identifier")
            if not isinstance(name,str) or not SAFE_FILE.fullmatch(name) or name in declared:
                raise ValueError("Duplicate or unsafe prototype source filename: "+str(name))
            declared.add(name)
            asset_file=directory/name
            if not asset_file.is_file() or asset_file.is_symlink():
                raise ValueError("Missing or linked generated geometry file: "+str(asset_file))
            digest=hashlib.sha256(asset_file.read_bytes()).hexdigest()
            if digest!=record.get("sha256"):
                raise ValueError("SHA-256 hash mismatch for "+name)
            stem=name[:-5]
            destination=IMPORT_ROOT+"/"+group+"/"+stem
            entry={
                "id":key,
                "asset_key":kind+":"+key,
                "kind":kind,
                "source_file":asset_file,
                "destination":destination,
                "asset_name":stem,
                "sha256":digest,
                "triangles":record.get("triangles"),
            }
            entries.append(entry)
            if kind=="world" and record.get("kind")!="physical_clue":
                position=record.get("ue_position_cm")
                if not isinstance(position,list) or len(position)!=3 or not all(
                    isinstance(x,(int,float)) and abs(x)<1000000 for x in position):
                    raise ValueError("Invalid city module source placement "+key)
                label="UM_PROTO_"+re.sub(r"[^A-Za-z0-9_]+","_",key).strip("_")
                preview.append({"asset_key":entry["asset_key"],"label":label,
                                "realm":record["realm"],"position_cm":position})
        actual={f.name for f in directory.iterdir() if f.is_file()}
        if actual != declared | {"manifest.json"}:
            raise ValueError("Prototype input inventory differs from manifest: "+kind)
    if len(entries)!=193 or len(preview)!=88:
        raise ValueError("Unexpected import or preview scope")
    if len({e["asset_key"] for e in entries})!=193 or len({
            e["destination"] for e in entries})!=193:
        raise ValueError("Prototype import ID/destination collision")
    if len({p["label"] for p in preview})!=88:
        raise ValueError("City preview actor label collision")
    return {"status":"VALIDATED_SOURCE_ONLY","entries":entries,
            "preview":preview,"source_counts":{"world":115,"equipment":78},
            "destination_root":IMPORT_ROOT,"map":PREVIEW_MAP}


def _approved():
    if os.environ.get("UNMADE_PROTO_IMPORT_APPROVED")!="1":
        raise RuntimeError("Unreal prototype asset import needs first-PC approval")


def _existing_mesh(unreal, folder: str):
    library=unreal.EditorAssetLibrary
    paths=library.list_assets(folder,recursive=False) or []
    candidates=[]
    for path in paths:
        asset=library.load_asset(path)
        if isinstance(asset,unreal.StaticMesh):
            candidates.append((path,asset))
    if len(candidates)>1:
        raise RuntimeError("Multiple unexpected StaticMesh assets in isolated folder "+folder)
    if paths and not candidates:
        raise RuntimeError("Existing prototype folder has no StaticMesh: "+folder)
    if candidates:
        return candidates[0][0]
    return None


def import_assets(unreal, plan: dict) -> dict:
    """Import only into dedicated prototype folders; never replace existing assets."""
    _approved()
    library=unreal.EditorAssetLibrary
    tools=unreal.AssetToolsHelpers.get_asset_tools()
    created=existing=0
    paths={}
    for item in plan["entries"]:
        folder=item["destination"]
        if not folder.startswith(IMPORT_ROOT+"/") or (
                item["asset_name"] not in folder or ".." in folder):
            raise RuntimeError("Refusing non-prototype import target")
        # Some editor providers expose assets before folder metadata is
        # refreshed. Always inspect asset paths FIRST; never rely on the
        # directory-exists bit to decide it is safe to import.
        prior=_existing_mesh(unreal,folder)
        if prior:
            paths[item["asset_key"]]=prior
            existing+=1
            continue
        if library.list_assets(folder,recursive=False):
            raise RuntimeError("Incomplete existing prototype import in "+folder)
        if not library.does_directory_exist(folder) and not library.make_directory(folder):
            raise RuntimeError("Cannot create isolated asset folder "+folder)
        task=unreal.AssetImportTask()
        for field,value in (
            ("filename",str(item["source_file"])),
            ("destination_path",folder),
            ("destination_name",item["asset_name"]),
            ("automated",True),
            ("save",True),
            ("replace_existing",False),
        ):
            task.set_editor_property(field,value)
        tools.import_asset_tasks([task])
        imported=_existing_mesh(unreal,folder)
        if not imported:
            raise RuntimeError("glTF import did not produce StaticMesh in "+folder+
                               "; check Interchange plugin and pipeline settings")
        paths[item["asset_key"]]=imported
        created+=1
    if created+existing!=len(plan["entries"]):
        raise RuntimeError("Incomplete prototype import receipt")
    return {"created":created,"existing":existing,"assets":paths}


def stage_preview(unreal, plan: dict, asset_paths: dict) -> dict:
    """Never stage in Entry or another gameplay map; skip artist-edited actors."""
    _approved()
    library=unreal.EditorAssetLibrary
    subsystem=unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    if library.does_asset_exist(PREVIEW_MAP):
        if not subsystem.load_level(PREVIEW_MAP):
            raise RuntimeError("Cannot load isolated prototype preview map")
    elif not subsystem.new_level(PREVIEW_MAP):
        raise RuntimeError("Cannot create isolated prototype preview map")
    levels=unreal.EditorLevelLibrary
    world=levels.get_editor_world()
    if not world or world.get_name() != PREVIEW_MAP.rsplit("/",1)[-1]:
        raise RuntimeError("Refusing to mutate any non-prototype map")
    existing={actor.get_actor_label():actor
              for actor in levels.get_all_level_actors()}
    created=skipped=0
    for item in plan["preview"]:
        if item["label"] in existing:
            # An artist may have moved/replaced a marker. Never change it.
            skipped+=1
            continue
        ref=asset_paths.get(item["asset_key"])
        mesh=library.load_asset(ref) if isinstance(ref,str) else ref
        if not isinstance(mesh,unreal.StaticMesh):
            raise RuntimeError("Unimported preview mesh: "+item["asset_key"])
        actor=levels.spawn_actor_from_class(
            unreal.StaticMeshActor,unreal.Vector(*item["position_cm"]))
        if actor is None:
            raise RuntimeError("Failed to stage city asset "+item["asset_key"])
        comp=actor.get_component_by_class(unreal.StaticMeshComponent)
        if comp is None:
            raise RuntimeError("City preview actor lacks mesh component")
        comp.set_static_mesh(mesh)
        comp.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        actor.set_actor_label(item["label"])
        created+=1
    if created and levels.save_current_level() is False:
        raise RuntimeError("Prototype preview level save failed")
    if created+skipped!=len(plan["preview"]):
        raise RuntimeError("Prototype map staging count drift")
    return {"map":PREVIEW_MAP,"created":created,"existing":skipped,
            "markers":len(plan["preview"]),"collision":False,
            "gameplay_authority":False}


def main() -> int:
    plan=prepare_plan()
    if os.environ.get("UNMADE_PROTO_IMPORT_APPROVED")!="1":
        print("VALIDATED SOURCE ONLY: 193 glTF files; 88 city preview actors "+
              "(dry-run, Unreal untouched)")
        return 0
    receipt_path=os.environ.get("UNMADE_PROTO_IMPORT_RECEIPT","")
    if not receipt_path or not Path(receipt_path).parent.is_dir():
        raise RuntimeError("Refusing Unreal import without writable receipt location")
    import unreal
    imported=import_assets(unreal,plan)
    preview=stage_preview(unreal,plan,imported["assets"])
    receipt={
        "status":"PROTOTYPE_ASSETS_IMPORTED_STAGING_COMPLETE",
        "note":"Engine-imported prototypes only; not playable levels or granted quests",
        "source_commit":os.environ.get("UNMADE_COMMIT","unknown"),
        "source_counts":plan["source_counts"],
        "imports":{"created":imported["created"],"existing":imported["existing"],
                   "total":len(imported["assets"])},
        "preview":preview,
    }
    Path(receipt_path).write_text(json.dumps(receipt,indent=2)+"\n",encoding="utf-8")
    unreal.log("UNMADE prototype import: 193 source meshes; 88 staged noncolliding guides")
    print("PROTOTYPE_IMPORT_STAGED, NOT GAMEPLAY CERTIFIED")
    return 0


if __name__=="__main__":
    raise SystemExit(main())
