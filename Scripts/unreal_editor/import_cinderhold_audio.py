#!/usr/bin/env python3
"""Dry-run by default. Import isolated prototype sounds inside an approved editor."""
import os,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from Scripts.build_cinderhold_audio import validate,DEST

def apply():
    folder=ROOT/'Build/CinderholdAudio';manifest=validate(folder)
    if os.environ.get('UNMADE_AUDIO_IMPORT_APPROVED')!='1':
        print('DRY RUN: 3 Cinderhold Foley cues validated; no editor writes');return
    import unreal
    tasks=[]
    for a in manifest['assets']:
        if unreal.EditorAssetLibrary.does_asset_exist(a['unreal_asset']):continue
        task=unreal.AssetImportTask()
        task.set_editor_property('filename',str(folder/a['asset']))
        task.set_editor_property('destination_path',DEST)
        task.set_editor_property('destination_name',Path(a['asset']).stem)
        task.set_editor_property('automated',True);task.set_editor_property('save',True)
        task.set_editor_property('replace_existing',False)
        tasks.append(task)
    if tasks:unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for a in manifest['assets']:
        if not unreal.EditorAssetLibrary.does_asset_exist(a['unreal_asset']):raise RuntimeError('Import failed: '+a['cue'])
    print('Imported or retained 3 Cinderhold prototype sounds')
if __name__=='__main__':apply()
