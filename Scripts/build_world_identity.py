#!/usr/bin/env python3
from pathlib import Path
import argparse,json
ROOT=Path(__file__).resolve().parents[1]
def build():
 rows=json.loads((ROOT/'Authoring/world_identity_v15.json').read_text())['realms'];assert len(rows)==9 and [r['realm_index'] for r in rows]==list(range(9));assert len({r['mechanism'] for r in rows})==9
 assert all(len(r['side_quests'])==3 for r in rows)
 assert all(len({r['side_quests'][index] for r in rows})==9 for index in range(3))
 return '#pragma once\nnamespace UnmadeCore {\nstruct IdentitySpec{const char* mechanism;const char* hint;const char* ambience;const char* hunt_task;const char* survey_task;const char* repair_task;};\ninline constexpr IdentitySpec IdentitySpecs[]={\n'+''.join(' {'+','.join(json.dumps(c[k]) for k in ['mechanism','hint','ambience'])+','+','.join(json.dumps(task) for task in c['side_quests'])+'},\n' for c in rows)+'};\n}\n'
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--check',action='store_true');a=p.parse_args();out=ROOT/'Source/TheUnmadeGame/Public/World/UnmadeIdentityRegistry.h';s=build()
 if a.check:assert out.read_text()==s
 else:out.write_text(s)
 print('PASS: nine authored world identity mechanisms')
