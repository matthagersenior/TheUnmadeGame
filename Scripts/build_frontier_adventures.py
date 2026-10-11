#!/usr/bin/env python3
from pathlib import Path
import json,argparse
ROOT=Path(__file__).resolve().parents[1]
def build():
 data=json.loads((ROOT/'Authoring/frontier_adventures_v14.json').read_text());a=data['arcs'];assert len(a)==9
 assert [x['realm'] for x in a]==['ThreefoldReach','WidowedRain','HearthBeneath','TidalLedger','SkyBelow','CinderSpine','HundredUnlived','OrchardOfKings','FirstAbsence'], 'saved realm indices must remain stable'
 fields=['city','frontier','refuge','dungeon','boss','weapon','armor','technique','brief','kill_return','mercy_return']
 out=['#pragma once','#include "World/UnmadeWorldAtlas.h"','#include <array>','namespace UnmadeCore {','struct ExpeditionSpec { Realm realm;const char* '+',*'.join(fields)+'; std::array<const char*,5> people; std::array<const char*,3> clues;double x,y; };','inline constexpr std::array<ExpeditionSpec,9> ExpeditionSpecs={{']
 for r in a:
  q=lambda s:json.dumps(s,ensure_ascii=False)
  out.append('{Realm::'+r['realm']+','+','.join(q(r[f]) for f in fields)+',{{'+','.join(q(s) for s in r['people'])+'}},{{'+','.join(q(s) for s in r['clues'])+'}},'+str(r['x'])+','+str(r['y'])+'},')
 out+=['}};','}'];return '\n'.join(out)+'\n'
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--check',action='store_true');a=p.parse_args();target=ROOT/'Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h';s=build()
 if a.check:assert target.read_text()==s,'expedition registry drift'
 else:target.write_text(s)
 print('9 dungeon arcs, 18 new settlements, 45 new residents: registry current')
