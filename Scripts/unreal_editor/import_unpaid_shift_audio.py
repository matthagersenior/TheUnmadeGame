#!/usr/bin/env python3
"""Validate and optionally import The Unpaid Shift's 35 ORIGINAL prototype
WAV Foley files into UE 5.8. This never installs software or overwrites real
voice recordings. Import only with UNMADE_AUDIO_IMPORT_APPROVED=1 inside UE.
"""
import hashlib
import json
import os
from pathlib import Path
import sys
import wave

ROOT=Path(__file__).resolve().parents[2]
ASSETS=ROOT/"Build/UnpaidShiftAudio"
DEST="/Game/UnmadeProduction/Audio/UnpaidShift"

def validate(folder=ASSETS):
    manifest_path=folder/"unpaid_shift_audio_manifest.json"
    if not manifest_path.is_file():
        raise RuntimeError("Missing generated audio manifest; first run Scripts/build_unpaid_shift_audio.py")
    manifest=json.loads(manifest_path.read_text(encoding="utf-8"))
    assets=manifest["assets"]
    if manifest["schema"]!=1 or manifest["beat_count"]!=39 or len(assets)!=35:
        raise RuntimeError("Unpaid Shift audio cue contract has changed")
    if manifest["production_status"]!="ORIGINAL_SYNTHETIC_PLACEHOLDER_FOLEY_NOT_FINAL_VO":
        raise RuntimeError("Audio sources must not pretend to be final voiced performances")
    names=set()
    for item in assets:
        name=item["asset"]
        if name in names or not name.startswith("SFX_US_") or not name.endswith(".wav"):
            raise RuntimeError("Duplicate or invalid cue file")
        names.add(name)
        file=folder/name
        if not file.is_file():
            raise RuntimeError("Missing sound file "+name)
        if hashlib.sha256(file.read_bytes()).hexdigest()!=item["sha256"]:
            raise RuntimeError("Modified or corrupted sound "+name)
        with wave.open(str(file),"rb") as snd:
            if snd.getnchannels()!=2 or snd.getframerate()!=32000 or snd.getsampwidth()!=2:
                raise RuntimeError("Incorrect stereo audio format "+name)
        if not (0<float(item["peak"])<0.85 and float(item["rms"])>0):
            raise RuntimeError("Silent or clipping audio "+name)
        if item["unreal_asset"]!=DEST+"/"+name[:-4]:
            raise RuntimeError("Unexpected Unreal import path "+name)
    return assets

def apply(folder=ASSETS):
    assets=validate(folder)
    if os.environ.get("UNMADE_AUDIO_IMPORT_APPROVED")!="1":
        print(f"DRY RUN: {len(assets)} real stereo prototype Foley cues; no editor writes")
        return {"mode":"dry_run","count":len(assets)}
    import unreal
    library=unreal.EditorAssetLibrary
    if not library.does_directory_exist(DEST):
        if not library.make_directory(DEST):
            raise RuntimeError("Cannot create isolated Unpaid Shift audio folder")
    tasks=[]
    existing=[]
    for item in assets:
        dest=item["unreal_asset"]
        # Never overwrite edited/bespoke SoundWaves. User may remove obsolete
        # prototypes intentionally and reimport after approval.
        if library.does_asset_exist(dest):
            existing.append(dest)
            continue
        task=unreal.AssetImportTask()
        task.set_editor_property("filename",str(folder/item["asset"]))
        task.set_editor_property("destination_path",DEST)
        task.set_editor_property("destination_name",item["asset"][:-4])
        task.set_editor_property("automated",True)
        task.set_editor_property("save",True)
        task.set_editor_property("replace_existing",False)
        task.set_editor_property("replace_existing_settings",False)
        tasks.append(task)
    if tasks:
        unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for item in assets:
        path=item["unreal_asset"]
        if not library.does_asset_exist(path):
            raise RuntimeError("Import incomplete: "+path)
        asset=library.load_asset(path)
        if not asset or not isinstance(asset,unreal.SoundWave):
            raise RuntimeError("Existing asset is not SoundWave: "+path)
        if not library.save_loaded_asset(asset):
            raise RuntimeError("Failed saving imported audio: "+path)
    receipt={"mode":"unreal_import_complete","count":len(assets),
             "existing_untouched":len(existing),
             "new_imports":len(tasks),
             "production_status":"PROTOTYPE_FOLEY_NOT_RECORDED_VO"}
    report=os.environ.get("UNMADE_AUDIO_IMPORT_RECEIPT")
    if report:
        Path(report).write_text(json.dumps(receipt,indent=2)+"\n",encoding="utf-8")
    return receipt

if __name__=="__main__":
    try:
        result=apply()
        print(json.dumps(result,sort_keys=True))
    except Exception as error:
        print("UNPAID_SHIFT_AUDIO_FAILED:",error,file=sys.stderr)
        raise SystemExit(2)
