#!/usr/bin/env python3
from pathlib import Path
import argparse,json,re
ROOT=Path(__file__).resolve().parents[1]
FIELDS=['quest','goal','offer','witness','evidence','option_a','option_b','return_a','return_b','challenge','secret_name','power','counterparty','witness_return_a','witness_return_b']
def build():
 rows=json.loads((ROOT/'Authoring/adventure_depth_v16.json').read_text())['realms'];assert len(rows)==9 and [x['realm_index'] for x in rows]==list(range(9));assert len({x['quest'] for x in rows})==9
 layout=(ROOT/'Source/TheUnmadeGame/Public/World/UnmadeAdventureLayout.h').read_text();coords=list(map(int,re.findall(r'-?\d+',layout.split('constexpr int paths[9][6][2]=')[1].split(';')[0])));assert coords==[v for row in rows for point in row['room_grid'] for v in point]
 return '#pragma once\nnamespace UnmadeCore {\nstruct DepthSpec{'+''.join('const char* '+k+';' for k in FIELDS)+'};\ninline constexpr DepthSpec DepthSpecs[]={\n'+''.join('{'+','.join(json.dumps(x[k]) for k in FIELDS)+'},\n' for x in rows)+'};\n}\n'
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--check',action='store_true');a=p.parse_args();out=ROOT/'Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h';s=build()
 if a.check:assert out.read_text()==s
 else:out.write_text(s)
 print('PASS: nine personal quest and exploration specifications')
