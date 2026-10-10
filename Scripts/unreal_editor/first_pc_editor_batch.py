#!/usr/bin/env python3
"""Guarded first-PC Unreal Python Editor batch.

With no flags: standard-library-only dry run, safe in GitHub CI.
Only env UNMADE_EDITOR_APPROVED=1 in the Unreal Python commandlet can
create seven *production-reference* DataTables and one separate staging map.
No production map, SaveGame, or C++ gameplay state is modified.
"""
from __future__ import annotations
import importlib.util
import json
import os
from pathlib import Path
import sys

ROOT=Path(__file__).resolve().parents[2]
MAP_PATH="/Game/UnmadeProduction/Maps/Unmade_AuthoringStaging"

def import_source(name: str, source: Path):
    spec=importlib.util.spec_from_file_location(name,source)
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module

PLAN=import_source("unmade_authoring_contract",ROOT/"Scripts/prepare_unreal_authoring.py")
STAGER=import_source("unmade_scene_stager",ROOT/"Scripts/unreal_editor/stage_world_manifest.py")

def validate():
    plan=PLAN.authoring_plan()
    stage=STAGER.read_plan()
    if plan["staging_map"].rsplit("/",1)[-1] != stage["editor_level_name"]:
        raise ValueError("Authoring manifest and dedicated staging map differ")
    if len(stage["actors"])!=plan["expected_staging_actors"]:
        raise ValueError("Staging actor count drift")
    return plan,stage

def put_tables(unreal, plan):
    tools=unreal.AssetToolsHelpers.get_asset_tools()
    library=unreal.EditorAssetLibrary
    folder=plan["data_folder"]
    if not library.does_directory_exist(folder) and not library.make_directory(folder):
        raise RuntimeError("Cannot create isolated authoring table folder")
    result=[]
    for entry in plan["tables"]:
        asset_path=entry["asset"]
        struct=unreal.load_object(None,entry["row_struct"])
        if struct is None:
            raise RuntimeError("Missing UHT row USTRUCT "+entry["row_struct"]+
                               "; compile TheUnmadeGameEditor first")
        asset=library.load_asset(asset_path) if library.does_asset_exist(asset_path) else None
        if asset:
            if not isinstance(asset,unreal.DataTable):
                raise RuntimeError("Refusing to overwrite non-DataTable asset at "+asset_path)
            current=asset.get_editor_property("row_struct")
            if not current or current.get_path_name()!=struct.get_path_name():
                raise RuntimeError("Row struct mismatch; refusing to replace "+asset_path)
        else:
            factory=unreal.DataTableFactory()
            factory.set_editor_property("struct",struct)
            asset=tools.create_asset(asset_path.rsplit("/",1)[-1],folder,
                                     unreal.DataTable,factory)
            if not asset:
                raise RuntimeError("Could not create authoring-only DataTable "+asset_path)
        csv_path=ROOT/entry["source"]
        if not unreal.DataTableFunctionLibrary.fill_data_table_from_csv_file(
                asset,str(csv_path)):
            raise RuntimeError("CSV import failed; previous asset must be inspected: "+asset_path)
        if not library.save_loaded_asset(asset):
            raise RuntimeError("Could not save imported table "+asset_path)
        result.append({"asset":asset_path,"rows":entry["rows"],
                       "source_sha256":entry["sha256"]})
        unreal.log("THE UNMADE imported "+asset_path)
    return result

def put_stage(unreal,stage):
    library=unreal.EditorAssetLibrary
    if library.does_asset_exist(MAP_PATH):
        if not unreal.EditorLevelLibrary.load_level(MAP_PATH):
            raise RuntimeError("Could not load existing dedicated staging level")
    else:
        if not unreal.EditorLevelLibrary.new_level(MAP_PATH):
            raise RuntimeError("Could not create dedicated staging level")
    if "Unmade_AuthoringStaging" not in unreal.EditorLevelLibrary.get_editor_world().get_name():
        raise RuntimeError("Map isolation check failed; no staging writes allowed")
    created,skipped=STAGER.apply_in_editor(stage)
    if created+skipped!=52:
        raise RuntimeError("Incomplete staging actor result")
    return {"map":MAP_PATH,"created":created,"skipped":skipped}

def apply(plan,stage):
    if os.environ.get("UNMADE_EDITOR_APPROVED")!="1":
        raise RuntimeError("Editor application requires explicit PowerShell approval")
    import unreal
    tables=put_tables(unreal,plan)
    staged=put_stage(unreal,stage)
    receipt={"status":"EDITOR_IMPORT_ATTEMPT_COMPLETE",
             "note":"Not a completed art or game playtest; only generated editor references",
             "tables":tables,"staging":staged,"commit":os.environ.get("UNMADE_COMMIT","unknown")}
    report=os.environ.get("UNMADE_EDITOR_RECEIPT","")
    if not report:
        raise RuntimeError("Refusing to apply without receipt path")
    path=Path(report)
    if not path.parent.is_dir():
        raise RuntimeError("Receipt directory does not exist")
    path.write_text(json.dumps(receipt,indent=2)+"\n",encoding="utf-8")
    unreal.log("UNMADE EDITOR BATCH PASSED: 7 tables / 198 content rows / staging 52")
    return receipt

def main():
    plan,stage=validate()
    if os.environ.get("UNMADE_EDITOR_APPROVED")!="1":
        print("DRY RUN ONLY | tables %d | rows %d | noncolliding stage refs %d | map %s"
              %(len(plan["tables"]),plan["total_rows"],len(stage["actors"]),MAP_PATH))
        return 0
    apply(plan,stage)
    print("EDITOR REFERENCE IMPORT COMPLETED; no runtime content or SaveGame changed")
    return 0

if __name__=="__main__":
    raise SystemExit(main())
