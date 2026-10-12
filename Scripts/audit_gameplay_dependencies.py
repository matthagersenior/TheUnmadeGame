"""Repeatable source audit; exercises production rules, never certifies Unreal."""
import argparse,json,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def main():
 p=argparse.ArgumentParser();p.add_argument('--check',action='store_true');p.add_argument('--report',type=Path);args=p.parse_args();checks=[]
 def run(name,cmd):
  result=subprocess.run(cmd,cwd=ROOT,text=True,capture_output=True)
  checks.append({'name':name,'passed':result.returncode==0,'evidence':(result.stdout+result.stderr)[-2000:]})
  return result.returncode==0
 with tempfile.TemporaryDirectory(prefix='unmade-audit-') as tmp:
  for name,test in [('progression after spending','Tests/world/progression_resilience_test.cpp'),('atomic field recipes','Tests/items/field_craft_test.cpp'),('save recovery and cadence','Tests/save/session_policy_test.cpp'),('player-facing failure behavior','Tests/player/player_facing_resilience_test.cpp')]:
   exe=str(Path(tmp)/Path(test).stem)
   if run(name+' compile',['g++','-std=c++17','-Wall','-Wextra','-pedantic','-I','Source/TheUnmadeGame/Public',test,'-o',exe]):run(name,[exe])
  run('profile UI world dependencies',['python','-m','unittest','discover','-s','Scripts/tests','-p','test_rpg_loop_dependencies.py','-v'])
  run('player-facing adapter dependencies',['python','-m','unittest','discover','-s','Scripts/tests','-p','test_player_facing_resilience.py','-v'])
 report={'scope':'portable rules and source wiring','checks':checks,'passed':all(c['passed'] for c in checks),'deferred':['Unreal build and asset import','controller focus and scroll navigation','save IO failure on platform','collision navigation and nine realm walkthrough','animation and sound acceptance','measured frame time and memory']}
 if args.report:args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(report,indent=2));return 0 if report['passed'] else 1
if __name__=='__main__':raise SystemExit(main())
