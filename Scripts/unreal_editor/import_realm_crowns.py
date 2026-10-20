#!/usr/bin/env python3
"""Import nine original landmark meshes into the isolated prototype preview map.

Always runs source validation before any write. Real Editor activity requires
the same explicit transient approval and receipt mechanism as 193-mesh importer.
"""
from __future__ import annotations
import hashlib
import json
import os
from pathlib import Path
import re
import sys

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/"Scripts"))
sys.path.insert(0,str(ROOT/"Scripts/unreal_editor"))
import build_realm_crowns as C
import build_realm_geometry as G
import import_prototype_geometry as P

CROWN_DIR=ROOT/"Build/UnmadePrototypeCrowns"
IMPORT_ROOT=P.IMPORT_ROOT+"/RealmCrowns"
PREVIEW_MAP=P.PREVIEW_MAP
SAFE_FILE=re.compile(r"^SM_UnmadeRealmCrown_[A-Za-z0-9_]+\.gltf$")


def prepare_crown_plan(directory:Path=CROWN_DIR)->dict:
    directory=Path(directory)
    manifest_path=directory/"manifest.json"
    if not manifest_path.is_file():
        raise ValueError("Missing realm crown manifest")
    manifest=json.loads(manifest_path.read_text(encoding="utf-8"))
    rows=manifest.get("assets")
    if manifest.get("schema_version")!=1 or manifest.get("asset_count")!=9 or not isinstance(rows,list) or len(rows)!=9:
        raise ValueError("Nine source-authored realm crowns required")
    if [r.get("realm") for r in rows] != list(G.REALMS):
        raise ValueError("Realm crown identity/order mismatch")
    expected_names=set()
    entries=[]; preview=[]
    for row in rows:
        name=row.get("file")
        if not isinstance(name,str) or not SAFE_FILE.fullmatch(name) or name in expected_names:
            raise ValueError("Unsafe/duplicate crown filename "+str(name))
        expected_names.add(name)
        file=directory/name
        if not file.is_file() or file.is_symlink():
            raise ValueError("Missing unique source landmark "+str(file))
        sha=hashlib.sha256(file.read_bytes()).hexdigest()
        if sha!=row.get("sha256"):
            raise ValueError("Crown source hash mismatch: "+name)
        if row.get("status")!="SOURCE_MESH_ENGINE_UNVERIFIED" or row.get("collision")!="none" or row.get("quest_authority") is not False:
            raise ValueError("Realm crown is not a source-only noncolliding prototype")
        if not isinstance(row.get("ue_position_cm"),list) or len(row["ue_position_cm"])!=3 or not all(
                isinstance(x,(int,float)) and abs(x)<1000000 for x in row["ue_position_cm"]):
            raise ValueError("Invalid prototype-only crown coordinates")
        model_name=name[:-5]
        key="crown:"+row["realm"]
        entries.append({
            "id":row["stable_id"],
            "asset_key":key,
            "kind":"world_crown",
            "source_file":file,
            "destination":IMPORT_ROOT+"/"+model_name,
            "asset_name":model_name,
            "sha256":sha,
            "triangles":row["triangles"],
        })
        preview.append({
            "asset_key":key,
            "label":"UM_PROTO_CROWN_"+row["realm"],
            "realm":row["realm"],
            "position_cm":row["ue_position_cm"],
        })
    existing={x.name for x in directory.iterdir() if x.is_file()}
    if existing!=expected_names|{"manifest.json"}:
        raise ValueError("Crown source inventory mismatch")
    if len({e["destination"] for e in entries})!=9:
        raise ValueError("Crown destination collision")
    return {"entries":entries,"preview":preview,"map":PREVIEW_MAP,
            "count":9,"status":"VALIDATED_SOURCE_ONLY"}


def import_and_stage_crowns(unreal,plan:dict)->dict:
    if os.environ.get("UNMADE_PROTO_IMPORT_APPROVED")!="1":
        raise RuntimeError("Editor crown staging requires explicit import approval")
    if len(plan["entries"])!=9 or len(plan["preview"])!=9:
        raise RuntimeError("Refusing incomplete nine-realm import plan")
    if any(not e["destination"].startswith(IMPORT_ROOT+"/") for e in plan["entries"]):
        raise RuntimeError("Crown importer attempted an unapproved folder")
    imported=P.import_assets(unreal,plan)
    staged=P.stage_preview(unreal,plan,imported["assets"])
    if len(imported["assets"])!=9 or staged["markers"]!=9:
        raise RuntimeError("Incomplete realm crown import receipt")
    return {"status":"REALM_CROWNS_STAGED_NOT_GAMEPLAY_VERIFIED",
            "imports":{"created":imported["created"],"existing":imported["existing"],
                       "total":len(imported["assets"])},
            "preview":staged,
            "non_authoritative":True}


def main()->int:
    plan=prepare_crown_plan()
    if os.environ.get("UNMADE_PROTO_IMPORT_APPROVED")!="1":
        print("VALIDATED SOURCE ONLY: nine distinct glTF realm crowns, no Editor edits")
        return 0
    receipt=os.environ.get("UNMADE_PROTO_IMPORT_RECEIPT","")
    if not receipt or not Path(receipt).parent.is_dir():
        raise RuntimeError("Refusing crown import without first-PC receipt path")
    import unreal
    result=import_and_stage_crowns(unreal,plan)
    Path(receipt).write_text(json.dumps(result,indent=2)+"\n",encoding="utf-8")
    unreal.log("UNMADE nine realm crown prototypes staged, not gameplay verified")
    print(result["status"])
    return 0


if __name__=="__main__":
    raise SystemExit(main())
