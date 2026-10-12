"""Install bounded player contact notifies after real clip import and C++ compilation.
Dry-run by default. Never claims editor execution when run with ordinary Python.
"""
import argparse,json
PLAN=[{'path':'/Game/UnmadeProduction/Characters/Resident/AN_UnmadeResident_Attack','track':'UnmadeContact','fractions':[.23,.32,.42],'class':'/Script/TheUnmadeGame.UnmadeWeaponContactNotify'}]
def main():
 p=argparse.ArgumentParser();p.add_argument('--execute',action='store_true');a=p.parse_args()
 if not a.execute:print(json.dumps({'mode':'dry_run','notifies':3,'plan':PLAN}));return
 try:import unreal
 except ImportError:raise SystemExit('UNREAL_EDITOR_REQUIRED: no asset changed')
 library=unreal.AnimationLibrary
 for item in PLAN:
  asset=unreal.load_asset(item['path']);notify=unreal.load_class(None,item['class'])
  if asset is None or notify is None:raise SystemExit('IMPORTED_CLIP_AND_COMPILED_NOTIFY_REQUIRED')
  # Only the dedicated owned track is replaced; other production notifies remain.
  names=[str(n) for n in library.get_animation_notify_track_names(asset)]
  if item['track'] in names:library.remove_animation_notify_track(asset,item['track'])
  library.add_animation_notify_track(asset,item['track'])
  duration=asset.get_editor_property('sequence_length')
  for fraction in item['fractions']:library.add_animation_notify_event(asset,item['track'],duration*fraction,notify)
  if not unreal.EditorAssetLibrary.save_loaded_asset(asset):raise RuntimeError('CONTACT_NOTIFY_SAVE_FAILED')
 print('EDITOR EXECUTION: three contact notifies installed; verify animation timing in PIE')
if __name__=='__main__':main()
