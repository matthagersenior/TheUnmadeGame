"""Project nine authored optional wings and return scenes without authoring a mystery answer."""
from pathlib import Path
import json,argparse
ROOT=Path(__file__).resolve().parents[1]
FIELDS=['city','wing','guardian','interior','specialty','task','hint','tool','recipe','reserve_return','witness_return']
def build():
 rows=json.loads((ROOT/'Authoring/player_experience_v17.json').read_text())['realms'];assert len(rows)==9
 for i,r in enumerate(rows):
  assert r['arc']==i and len(r['scenes'])==4 and r['canonical_id']==f'quest.wing.{i}'
  assert len(r['wing_grid'])==3 and all(len(p)==2 and all(isinstance(v,int) for v in p) for p in r['wing_grid'])
  assert all(abs(a[0]-b[0])+abs(a[1]-b[1])==1 for a,b in zip(r['wing_grid'],r['wing_grid'][1:]))
  assert all(isinstance(r[f],str) and r[f] for f in FIELDS)
 for f in ['wing','guardian','task','tool','recipe']:assert len({r[f] for r in rows})==9
 q=lambda s:json.dumps(s,ensure_ascii=True)
 s='#pragma once\nnamespace UnmadeCore {\nstruct ExperienceSpec {'+''.join('const char* '+f+';' for f in FIELDS)+'const char* scenes[4];int wing_grid[3][2];};\ninline constexpr ExperienceSpec ExperienceSpecs[9]={\n'
 for r in rows:s+=' {'+','.join(q(r[f]) for f in FIELDS)+',{'+','.join(q(t) for t in r['scenes'])+'},{'+','.join('{'+str(p[0])+','+str(p[1])+'}' for p in r['wing_grid'])+'}},\n'
 return s+'};\n}\n'
def main():
 p=argparse.ArgumentParser();p.add_argument('--check',action='store_true');a=p.parse_args();target=ROOT/'Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h';s=build()
 if a.check:assert target.read_text()==s,'experience registry drift'
 else:target.write_text(s)
 print('PASS: nine authored wings, specialties and 36 return scenes')
if __name__=='__main__':main()
