"""Check historical nine optional wing definitions against Codex-owned gameplay."""
from pathlib import Path
import json,argparse,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT))
from Scripts.generate_unmade_codex import REALMS,generate_experience

FIELDS=('city','wing','guardian','interior','specialty','task','hint',
        'tool','recipe','reserve_return','witness_return')
TARGET=ROOT/'Source/TheUnmadeGame/Public/World/UnmadeCodexExperienceGenerated.inc'
WRAPPER=ROOT/'Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h'

def build():
    historical=json.loads((ROOT/'Authoring/player_experience_v17.json').read_text(encoding='utf-8'))['realms']
    codex=json.loads((ROOT/'Authoring/ability_codex.json').read_text(encoding='utf-8'))
    records={row['id']:row for row in codex['records']}
    if len(historical)!=9:
        raise ValueError('Exactly nine optional experience wings must remain')
    for i,old in enumerate(historical):
        key=REALMS[i]
        c=records['journey.wing.'+key]['rule_contract']
        if old['arc']!=i or old['canonical_id']!=f'quest.wing.{i}' or c['source_index']!=i:
            raise ValueError('Wing source/save identity drift: '+key)
        for field in FIELDS:
            if old[field]!=c[field]:
                raise ValueError('Wing dialogue, challenge or reward drift: '+key+'.'+field)
        if old['scenes']!=c['scenes'] or old['wing_grid']!=c['wing_grid']:
            raise ValueError('Wing physical room grid or witness reaction drift: '+key)
        if len(old['scenes'])!=4 or len(old['wing_grid'])!=3:
            raise ValueError('Wing needs four return scenes and three rooms: '+key)
        if any(len(pair)!=2 for pair in old['wing_grid']):
            raise ValueError('Wing room coordinates malformed: '+key)
        if any(abs(a[0]-b[0])+abs(a[1]-b[1])!=1 for a,b in zip(old['wing_grid'],old['wing_grid'][1:])):
            raise ValueError('Wing fieldwork rooms lost physical adjacency: '+key)
    if '#include "World/UnmadeCodexExperienceGenerated.inc"' not in WRAPPER.read_text(encoding='utf-8'):
        raise ValueError('Wing gameplay source no longer includes Codex table')
    return generate_experience(codex)

def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--check',action='store_true')
    args=parser.parse_args()
    generated=build()
    if args.check:
        if not TARGET.is_file() or TARGET.read_text(encoding='utf-8')!=generated:
            raise SystemExit('FAIL: Codex-generated optional wing definitions are stale')
    else:
        TARGET.write_text(generated,encoding='utf-8')
    print('PASS: nine authored optional wings and 36 return scenes match Codex')

if __name__=='__main__':main()
