#!/usr/bin/env python3
"""Read-only GLB identity preflight; guarded Unreal Interchange into isolated art folders.

A GLB contains many individually named mesh nodes. Interchange may emit one or
many StaticMesh assets per original GLB: never claim a fixed node mapping
before a real Editor run. We never spawn these visuals in gameplay maps.
"""
from __future__ import annotations
import hashlib,json,os,re,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUTPUT=ROOT/"Build/UnmadeHeroModels"
IMPORT_ROOT="/Game/UnmadeProduction/PrototypeMeshes/HeroArt"
KIND_COUNTS={"Creature":12,"NPC":18,"Environment":9}
ALLOWED=re.compile(r"^SM_Unmade_[A-Za-z0-9_]+\.glb$")

def _inspect_glb(path:Path)->int:
    raw=path.read_bytes()
    if len(raw)<36:raise ValueError("Short or empty GLB: "+str(path))
    magic,version,length=struct.unpack_from("<4sII",raw)
    if magic!=b"glTF" or version!=2 or length!=len(raw):
        raise ValueError("Invalid binary glTF 2.0 header: "+str(path))
    chunk_len,chunk_kind=struct.unpack_from("<I4s",raw,12)
    if chunk_kind!=b"JSON" or chunk_len<=0 or 20+chunk_len>len(raw):
        raise ValueError("Missing glTF JSON chunk: "+str(path))
    doc=json.loads(raw[20:20+chunk_len].decode("utf-8").rstrip(" \x00"))
    if doc.get("asset",{}).get("version")!="2.0":
        raise ValueError("Wrong glTF version: "+str(path))
    meshes=doc.get("meshes",[])
    if not isinstance(meshes,list) or not meshes:
        raise ValueError("No actual mesh primitives: "+str(path))
    if not all(m.get("primitives") for m in meshes):
        raise ValueError("Empty mesh primitive in "+str(path))
    return len(meshes)

def prepare_plan(directory:Path=OUTPUT,root:Path=ROOT)->dict:
    directory=Path(directory);root=Path(root)
    manifest=json.loads((directory/"manifest.json").read_text(encoding="utf-8"))
    rows=manifest.get("assets")
    if manifest.get("schema")!=1 or manifest.get("asset_count")!=39 or manifest.get("categories")!=KIND_COUNTS:
        raise ValueError("Expected 39 source-authored, category-verified 3-D models")
    if not isinstance(rows,list) or len(rows)!=39:raise ValueError("Incomplete GLB list")
    source=json.loads((root/"Authoring/world_content_pack.json").read_text(encoding="utf-8"))
    npc_ids={r["id"] for r in source["npcs"]}
    realm_ids={r["id"] for r in source["realms"]}
    if len(npc_ids)!=18 or len(realm_ids)!=9:raise ValueError("Canonical source drift")
    species={"Cinderjaw","PaleStrider","Veilwatch","GleamMoth","HollowHound",
       "GriefLantern","Inkmaw","RedactionEye","SaltglassCrawler","DrownedWatch",
       "Ashmaw","KilnWisp"}
    expected={"NPC":npc_ids,"Creature":{"creature:"+s for s in species},
              "Environment":{"realm:"+r for r in realm_ids}}
    ids={k:set() for k in KIND_COUNTS};names=set();entries=[]
    for row in rows:
        kind=row.get("category")
        if kind not in KIND_COUNTS:raise ValueError("Unknown GLB category")
        asset_id=row.get("stable_id")
        if asset_id not in expected[kind] or asset_id in ids[kind]:
            raise ValueError("Duplicate, invented or noncanonical asset: "+str(asset_id))
        ids[kind].add(asset_id)
        filename=row.get("file")
        if not isinstance(filename,str) or not filename.startswith(kind+"/"):
            raise ValueError("Foreign or absolute GLB path")
        stem=filename[len(kind)+1:]
        if not ALLOWED.fullmatch(stem) or filename in names:
            raise ValueError("Unsafe/duplicate hero model filename")
        names.add(filename)
        path=directory/filename
        if not path.is_file() or path.is_symlink() or path.parent!=directory/kind:
            raise ValueError("Missing or linked source model: "+str(path))
        if hashlib.sha256(path.read_bytes()).hexdigest()!=row.get("sha256"):
            raise ValueError("Hero GLB source hash mismatch: "+stem)
        n=_inspect_glb(path)
        if not isinstance(row.get("parts"),int) or row["parts"]!=n:
            raise ValueError("Incorrect geometry node count for "+stem)
        if row.get("gameplay_authority") is not False or row.get("rigged") is not False:
            raise ValueError("Unexpected gameplay or skeletal authority for GLB")
        if not isinstance(row.get("triangles"),int) or row["triangles"]<=150:
            raise ValueError("Missing authored 3D geometry")
        destination=IMPORT_ROOT+"/"+kind+"/"+stem[:-4]
        entries.append({"source_file":path,"asset_name":stem[:-4],
           "asset_key":kind+":"+asset_id,"destination":destination,
           "expected_source_nodes":n,"category":kind})
    if ids!=expected:raise ValueError("Actual canonical roster differs from GLB manifest")
    if {str(p.relative_to(directory)) for p in directory.rglob("*.glb")}!=names:
        raise ValueError("Unexpected additional GLB in source directory")
    if len({x["destination"] for x in entries})!=39:raise ValueError("Unreal path collision")
    return {"status":"VALIDATED_SOURCE_ONLY","count":39,"entries":entries,
            "import_root":IMPORT_ROOT,"engine_verified":False}

def import_hero_models(unreal,plan:dict)->dict:
    if os.environ.get("UNMADE_HERO_IMPORT_APPROVED")!="1":
        raise RuntimeError("Hero art Interchange import requires explicit first-PC approval")
    if plan.get("count")!=39 or len(plan.get("entries",[]))!=39:
        raise RuntimeError("Refusing incomplete hero art import")
    library=unreal.EditorAssetLibrary;tools=unreal.AssetToolsHelpers.get_asset_tools()
    created=existing=parts_found=0
    for record in plan["entries"]:
        folder=record["destination"]
        if not folder.startswith(IMPORT_ROOT+"/") or ".." in folder:
            raise RuntimeError("Forbidden destination")
        prior=library.list_assets(folder,recursive=False) or []
        if not prior:
            if not library.does_directory_exist(folder) and not library.make_directory(folder):
                raise RuntimeError("Could not create isolated hero art folder")
            task=unreal.AssetImportTask()
            for key,value in (("filename",str(record["source_file"])),
                              ("destination_path",folder),("destination_name",record["asset_name"]),
                              ("automated",True),("save",True),("replace_existing",False)):
                task.set_editor_property(key,value)
            tools.import_asset_tasks([task])
            prior=library.list_assets(folder,recursive=False) or []
            found=[p for p in prior if isinstance(library.load_asset(p),unreal.StaticMesh)]
            if not found:raise RuntimeError("Interchange created no StaticMesh for "+folder)
            created+=1
        else:
            found=[p for p in prior if isinstance(library.load_asset(p),unreal.StaticMesh)]
            if not found:raise RuntimeError("Existing folder is incomplete or not static art: "+folder)
            existing+=1
        parts_found+=len(found)
    if created+existing!=39:raise RuntimeError("Incomplete model imports")
    return {"status":"HERO_ART_IMPORTED_ENGINE_ASSET_MAPPING_NEEDS_VISUAL_REVIEW",
      "source_glbs":39,"created_glbs":created,"preserved_glbs":existing,
      "editor_static_mesh_assets_found":parts_found,
      "gameplay_map_changed":False,"collision_tested":False,
      "node_transform_fidelity_tested":False,"skeletal_assets_claimed":False}

def main()->int:
    plan=prepare_plan()
    if os.environ.get("UNMADE_HERO_IMPORT_APPROVED")!="1":
        print("VALIDATED SOURCE ONLY: 39 actual GLB meshes; Unreal Editor untouched")
        return 0
    receipt=os.environ.get("UNMADE_HERO_IMPORT_RECEIPT","")
    if not receipt or not Path(receipt).parent.is_dir():
        raise RuntimeError("Refusing editor write without dated receipt path")
    import unreal
    result=import_hero_models(unreal,plan)
    Path(receipt).write_text(json.dumps(result,indent=2)+"\n",encoding="utf-8")
    unreal.log("The Unmade: imported 39 original hero-art source GLBs; validate node transforms and preview rendering")
    print(result["status"])
    return 0
if __name__=="__main__":
    raise SystemExit(main())
