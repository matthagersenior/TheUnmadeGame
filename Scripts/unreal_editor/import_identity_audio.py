"""Run in Unreal Editor Python with UNMADE_IDENTITY_AUDIO pointing to generated WAV folder."""
import json,os
from pathlib import Path
import unreal
source=Path(os.environ['UNMADE_IDENTITY_AUDIO']);m=json.loads((source/'identity_audio_manifest.json').read_text());tasks=[]
for row in m['assets']:
 t=unreal.AssetImportTask();t.filename=str(source/row['file']);t.destination_path='/Game/UnmadeProduction/Audio/Identity';t.automated=True;t.save=True;t.replace_existing=False;tasks.append(t)
unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
for row in m['assets']:
 sound=unreal.load_asset(row['unreal_asset'])
 if not sound:raise RuntimeError('Missing import '+row['cue'])
 sound.set_editor_property('looping',row['loop']);unreal.EditorAssetLibrary.save_loaded_asset(sound)
