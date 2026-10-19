#!/usr/bin/env python3
"""Protect historical personal quests and physical layouts against canonical Codex drift."""
from pathlib import Path
import argparse,json,re,sys
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT))
from Scripts.generate_unmade_codex import REALMS,PERSONAL_FIELDS,generate_personal_quests
FIELDS=('quest','goal')+PERSONAL_FIELDS
TARGET=ROOT/'Source/TheUnmadeGame/Public/World/UnmadeCodexPersonalQuestsGenerated.inc'
WRAPPER=ROOT/'Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h'

def build():
    old=json.loads((ROOT/'Authoring/adventure_depth_v16.json').read_text(encoding='utf-8'))['realms']
    canon=json.loads((ROOT/'Authoring/ability_codex.json').read_text(encoding='utf-8'))
    by={row['id']:row for row in canon['records']}
    if len(old)!=9 or [x['realm_index'] for x in old]!=list(range(9)):
        raise ValueError('Personal quest saved indices drift')
    coords=list(map(int,re.findall(r'-?\\d+', (ROOT/'Source/TheUnmadeGame/Public/World/UnmadeAdventureLayout.h').read_text().split('constexpr int paths[9][6][2]=')[1].split(';')[0])))
    expected=[v for x in old for point in x['room_grid'] for v in point]
    if coords!=expected:
        raise ValueError('Personal quest physical dungeon layout drift')
    for index,(key,old_row) in enumerate(zip(REALMS,old)):
        row=by['quest.personal.'+key]
        rule=row['rule_contract']
        if row['name']!=old_row['quest'] or rule['source_order']!=index or rule['goal']!=old_row['goal']:
            raise ValueError('Personal quest name/source ordinal drift: '+key)
        for field in PERSONAL_FIELDS:
            if rule[field]!=old_row[field]:
                raise ValueError('Personal quest witness/choice continuity drift: '+key+'.'+field)
        if rule['room_grid']!=old_row['room_grid']:
            raise ValueError('Personal quest physical rooms changed: '+key)
    if '#include "World/UnmadeCodexPersonalQuestsGenerated.inc"' not in WRAPPER.read_text(encoding='utf-8'):
        raise ValueError('Personal quest gameplay no longer compiles Codex rules')
    return generate_personal_quests(canon)

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--check',action='store_true');args=parser.parse_args()
    generated=build()
    if args.check:
        if not TARGET.is_file() or TARGET.read_text(encoding='utf-8')!=generated:
            raise SystemExit('FAIL: Codex personal quest rules need regeneration')
    else:TARGET.write_text(generated,encoding='utf-8')
    print('PASS: nine personal quest choices, witnesses and physical rooms match Codex')
