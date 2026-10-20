#!/usr/bin/env python3
"""Fail-closed Unreal skeletal asset handoff for three original glTF prototypes.

Generates no animation or gameplay events. Exact 3 skeleton-bearing mesh assets
and 57 AN_* motion clips must exist at the paths the actual C++ actors load.
An unverified or partially imported Interchange result fails with a receipt
and explanation, never a false "gameplay ready" claim.
"""
from __future__ import annotations
import hashlib
import json
import os
from pathlib import Path
import sys

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/"Scripts"))
import build_skeletal_performance_kit as K

SOURCE_DIR=ROOT/"Build/UnmadePrototypeRigs"
CONTENT_ROOT="/Game/UnmadeProduction/Characters"
KINDS=("Resident","Beast","Wraith")
EXPECTED_ANIMATIONS=19


def prepare_plan(directory:Path=SOURCE_DIR)->dict:
    directory=Path(directory)
    if not directory.is_dir():
        raise ValueError("Missing original skinned performance kit")
    expected_files={"SK_Unmade"+kind+".gltf" for kind in KINDS}
    expected_files.add("UnmadeSixRegionPBR.gltf")
    actual={f.name for f in directory.iterdir() if f.is_file()}
    if actual!=expected_files:
        raise ValueError("Skeletal source inventory drift; require three rigs and one PBR atlas")
    rigs=[];digests={}
    for kind in KINDS:
        path=directory/("SK_Unmade"+kind+".gltf")
        if not path.is_file() or path.is_symlink():
            raise ValueError("Missing or linked skeletal source")
        data=path.read_bytes()
        expected=json.dumps(K.make_actor(kind),separators=(",",":")).encode("utf-8")
        if data!=expected:
            raise ValueError("Skeletal source SHA/deterministic byte mismatch: "+path.name)
        K.validate(path)
        doc=json.loads(data)
        if len(doc.get("skins",[]))!=1 or len(doc["skins"][0].get("joints",[]))<4:
            raise ValueError("Source skeletal mesh has no usable skin: "+kind)
        clips=[x.get("name") for x in doc.get("animations",[])]
        if len(clips)!=EXPECTED_ANIMATIONS or len(set(clips))!=EXPECTED_ANIMATIONS or any(
                not isinstance(name,str) or not name.startswith("AN_Unmade"+kind+"_")
                for name in clips):
            raise ValueError("Missing authored rig motion clips: "+kind)
        folder=CONTENT_ROOT+"/"+kind
        mesh="SK_Unmade"+kind
        rigs.append({
            "kind":kind, "file":path, "sha256":hashlib.sha256(data).hexdigest(),
            "destination":folder, "mesh":mesh,
            "mesh_asset":folder+"/"+mesh,
            "animation_names":clips,
            "animation_assets":[folder+"/"+clip for clip in clips],
        })
        digests[path.name]=hashlib.sha256(data).hexdigest()
    atlas_file=directory/"UnmadeSixRegionPBR.gltf"
    if atlas_file.is_symlink() or not atlas_file.is_file():
        raise ValueError("Missing source PBR swatch atlas")
    atlas=atlas_file.read_bytes()
    if atlas!=json.dumps(K.make_material_atlas(),separators=(",",":")).encode("utf-8"):
        raise ValueError("PBR swatch source byte mismatch")
    K.validate(atlas_file)
    digests[atlas_file.name]=hashlib.sha256(atlas).hexdigest()
    if len(set(x["destination"] for x in rigs))!=3 or sum(
            len(x["animation_assets"]) for x in rigs)!=57:
        raise ValueError("Skeletal target path collision or clip-count drift")
    return {"status":"VALIDATED_SOURCE_ONLY","rigs":rigs,
            "mesh_count":3,"animation_count":57,
            "material_atlas_imported":False,
            "source_sha256":digests}


def _approved():
    if os.environ.get("UNMADE_SKIN_IMPORT_APPROVED")!="1":
        raise RuntimeError("Real character rig import requires explicit first-PC approval")


def _typed_library_assets(unreal,rig):
    library=unreal.EditorAssetLibrary
    mesh_obj=library.load_asset(rig["mesh_asset"])
    clips=[library.load_asset(p) for p in rig["animation_assets"]]
    has_any=bool(mesh_obj or any(clips))
    has_all=bool(mesh_obj is not None and all(clip is not None for clip in clips))
    if has_any and not has_all:
        raise RuntimeError("Partial skeletal import in "+rig["destination"]+
                           "; preserve existing work and inspect Editor log")
    if not has_all:
        return False
    if not isinstance(mesh_obj,unreal.SkeletalMesh):
        raise RuntimeError("Wrong type for original skeletal mesh: "+rig["mesh_asset"])
    if any(not isinstance(clip,unreal.AnimSequence) for clip in clips):
        raise RuntimeError("One or more expected motion clips are not AnimSequence assets")
    # Check every clip targets this rig's actual Skeleton. No mismatched
    # animation may be reported playable even when the asset names exist.
    skeleton=mesh_obj.get_editor_property("skeleton")
    if skeleton is None:
        raise RuntimeError("Imported rig lacks an Unreal Skeleton")
    for clip in clips:
        if clip.get_editor_property("skeleton")!=skeleton:
            raise RuntimeError("Animation skeleton mismatch for "+rig["kind"])
    return True


def import_skeletal_assets(unreal,plan:dict)->dict:
    _approved()
    if plan["mesh_count"]!=3 or plan["animation_count"]!=57 or len(plan["rigs"])!=3:
        raise RuntimeError("Refusing incomplete skinned animation import plan")
    library=unreal.EditorAssetLibrary
    tools=unreal.AssetToolsHelpers.get_asset_tools()
    new=existing=0
    clips=0
    names=[]
    for rig in plan["rigs"]:
        if rig["destination"]!=CONTENT_ROOT+"/"+rig["kind"]:
            raise RuntimeError("Refusing a foreign skeletal asset folder")
        if _typed_library_assets(unreal,rig):
            existing+=1
            clips+=len(rig["animation_assets"])
            names.append(rig["kind"])
            continue
        # A folder with other assets might be an artist's work or an
        # incomplete previous Interchange attempt. Never overwrite/rename it.
        if library.list_assets(rig["destination"],recursive=False):
            raise RuntimeError("Unexpected existing content in "+rig["destination"]+
                               "; manual reconciliation required")
        if not library.does_directory_exist(rig["destination"]):
            if not library.make_directory(rig["destination"]):
                raise RuntimeError("Unable to create approved character folder")
        task=unreal.AssetImportTask()
        for field,value in (
            ("filename",str(rig["file"])),
            ("destination_path",rig["destination"]),
            ("destination_name",rig["mesh"]),
            ("automated",True),("save",True),("replace_existing",False),
        ):
            task.set_editor_property(field,value)
        tools.import_asset_tasks([task])
        if not _typed_library_assets(unreal,rig):
            raise RuntimeError("UE Interchange did not create SK_* and all 19 AN_* assets "
                               "at runtime paths for "+rig["kind"]+
                               ". Verify glTF skeletal/animation pipeline and engine importer")
        new+=1
        clips+=len(rig["animation_assets"])
        names.append(rig["kind"])
    if new+existing!=3 or clips!=57 or names!=list(KINDS):
        raise RuntimeError("Incomplete rig/animation import acceptance")
    return {"status":"SKINNED_PROTOTYPE_ASSETS_PRESENT_NOT_GAMEPLAY_VERIFIED",
            "meshes":{"created":new,"existing":existing,"total":3},
            "animations":{"total":clips,"per_rig":EXPECTED_ANIMATIONS},
            "archetypes":names,"material_atlas_imported":False,
            "note":"Asset identities and skeleton compatibility checked; no UE playback/retargeting certified"}


def main():
    plan=prepare_plan()
    if os.environ.get("UNMADE_SKIN_IMPORT_APPROVED")!="1":
        print("VALIDATED SOURCE ONLY: 3 skinned original glTF rigs and 57 motion clips; "
              "Unreal, retargeting and playback untouched")
        return 0
    receipt=os.environ.get("UNMADE_SKIN_IMPORT_RECEIPT","")
    if not receipt or not Path(receipt).parent.is_dir():
        raise RuntimeError("Refusing skinned import without dated first-PC receipt path")
    import unreal
    proof=import_skeletal_assets(unreal,plan)
    proof["source_sha256"]=plan["source_sha256"]
    Path(receipt).write_text(json.dumps(proof,indent=2)+"\n",encoding="utf-8")
    unreal.log("UNMADE: imported three skeletal source prototypes and verified 57 AnimationSequences")
    print(proof["status"])
    return 0

if __name__=="__main__":
    raise SystemExit(main())
