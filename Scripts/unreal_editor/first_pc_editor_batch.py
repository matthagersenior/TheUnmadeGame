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
LIVED=import_source("unmade_lived_workorders",ROOT/"Scripts/unreal_editor/stage_lived_world_manifest.py")
CITY=import_source("unmade_city_staging",ROOT/"Scripts/unreal_editor/stage_city_assembly.py")

def validate():
    plan=PLAN.authoring_plan()
    stage=STAGER.read_plan()
    if plan["staging_map"].rsplit("/",1)[-1] != stage["editor_level_name"]:
        raise ValueError("Authoring manifest and dedicated staging map differ")
    if len(stage["actors"])!=plan["expected_staging_actors"]:
        raise ValueError("Staging actor count drift")
    lived=LIVED.read_plan()
    if len(lived["work_orders"])!=90:
        raise ValueError("Nine-realm world work orders changed")
    city=CITY.read_plan()
    if city["actor_count"]!=402 or city["cities"]!=11:
        raise ValueError("City assembly actor plan incomplete")
    return plan,stage,lived,city

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

def put_lived_stage(unreal,lived):
    library=unreal.EditorAssetLibrary
    map_path=LIVED.MAP
    if library.does_asset_exist(map_path):
        if not unreal.EditorLevelLibrary.load_level(map_path):
            raise RuntimeError("Cannot load dedicated lived-world staging map")
    elif not unreal.EditorLevelLibrary.new_level(map_path):
        raise RuntimeError("Cannot create dedicated lived-world staging map")
    created,skipped=LIVED.apply_in_editor(lived)
    if created+skipped!=90:
        raise RuntimeError("Not all lived-universe reference markers placed")
    return {"map":map_path,"created":created,"skipped":skipped}

def put_city_assembly(unreal,city):
    library=unreal.EditorAssetLibrary
    map_path=CITY.MAP
    if library.does_asset_exist(map_path):
        if not unreal.EditorLevelLibrary.load_level(map_path):
            raise RuntimeError("Cannot open isolated city assembly map")
    elif not unreal.EditorLevelLibrary.new_level(map_path):
        raise RuntimeError("Cannot create isolated city assembly map")
    created,skipped=CITY.apply_in_editor(city)
    if created+skipped!=402:
        raise RuntimeError("City assembly scene incomplete")
    return {"map":map_path,"created":created,"skipped":skipped,
            "source_actor_total":402,"city_count":11}

def apply(plan,stage,lived,city):
    if os.environ.get("UNMADE_EDITOR_APPROVED")!="1":
        raise RuntimeError("Editor application requires explicit PowerShell approval")
    import unreal
    tables=put_tables(unreal,plan)
    staged=put_stage(unreal,stage)
    lived_stage=put_lived_stage(unreal,lived)
    city_stage=put_city_assembly(unreal,city)
    receipt={"status":"EDITOR_IMPORT_ATTEMPT_COMPLETE",
             "note":"Not a completed art or game playtest; only generated editor references",
             "tables":tables,"staging":staged,"lived_staging":lived_stage,
             "city_assembly":city_stage,
             "commit":os.environ.get("UNMADE_COMMIT","unknown")}
    report=os.environ.get("UNMADE_EDITOR_RECEIPT","")
    if not report:
        raise RuntimeError("Refusing to apply without receipt path")
    path=Path(report)
    if not path.parent.is_dir():
        raise RuntimeError("Receipt directory does not exist")
    path.write_text(json.dumps(receipt,indent=2)+"\n",encoding="utf-8")
    unreal.log("UNMADE EDITOR BATCH PASSED: 7 tables / 198 rows / 52+90+402 source markers")
    return receipt

def main():
    plan,stage,lived,city=validate()
    if os.environ.get("UNMADE_EDITOR_APPROVED")!="1":
        print("DRY RUN ONLY | tables %d | rows %d | staged refs %d+%d+%d | map %s"
              %(len(plan["tables"]),plan["total_rows"],len(stage["actors"]),
                len(lived["work_orders"]),city["actor_count"],MAP_PATH))
        return 0
    apply(plan,stage,lived,city)
    print("EDITOR REFERENCE IMPORT COMPLETED; no runtime content or SaveGame changed")
    return 0

if __name__=="__main__":
    raise SystemExit(main())
