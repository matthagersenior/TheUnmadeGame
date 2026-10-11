"""Build the exact authored 11-city / 88-person portable registry. No runtime JSON parsing."""
from pathlib import Path
import argparse,json
ROOT=Path(__file__).resolve().parents[1]
def build():
 d=json.loads((ROOT/'Authoring/remembered_roads_v13.json').read_text())
 q=lambda s:json.dumps(s,ensure_ascii=True)
 lines=['#pragma once','#include "World/UnmadeWorldAtlas.h"','#include "Combat/UnmadeCreatureRules.h"','#include <array>','#include <cstring>','namespace UnmadeCore {','inline constexpr int RoadCityCount=11,RoadPersonCount=88;',
 'struct RoadCitySpec { const char* id; Realm realm; double x,y; int destination; const char* sender; const char* title; const char* cargo; const char* mechanism; std::array<const char*,3> clues; const char* openLine; const char* sealedLine; CreatureKind creature; const char* elder; const char* brief; const char* returned; const char* voice; };','inline constexpr std::array<RoadCitySpec,RoadCityCount> RoadCities={{']
 for c in d['cities']:
  vals=[q(c['id']),'Realm::'+c['realm'],str(c['x']),str(c['y']),str(c['destination']),q(c['sender']),q(c['title']),q(c['cargo']),q(c['mechanism']),'{'+','.join(q(x) for x in c['clues'])+'}',q(c['route_open']),q(c['route_sealed']),'CreatureKind::'+c['creature'],q(c['elder']),q(c['brief_line']),q(c['return_line']),q(c['performance']['voice'])]
  lines.append(' {'+','.join(vals)+'},')
 lines+=['}};','struct RoadPersonSpec { const char* id; const char* name; int city,preference; double homeX,homeY; const char* concern; const char* workLine; const char* agrees; const char* disagrees; const char* homeLine; };','inline constexpr std::array<RoadPersonSpec,RoadPersonCount> RoadPeople={{']
 for p in d['residents']:
  vals=[q(p['id']),q(p['name']),str(p['city']),str(p['preference']),str(p['home_x']),str(p['home_y'])]+[q(p[k]) for k in ['concern','work_line','agrees','disagrees','home_line']]
  lines.append(' {'+','.join(vals)+'},')
 lines+=['}};','inline int RoadPersonIndex(const char* id) noexcept { if(!id)return -1; for(int i=0;i<RoadPersonCount;++i)if(std::strcmp(id,RoadPeople[i].id)==0)return i;return -1; }','}']
 return '\n'.join(lines)+'\n'
def main():
 a=argparse.ArgumentParser();a.add_argument('--check',action='store_true');args=a.parse_args();p=ROOT/'Source/TheUnmadeGame/Public/World/UnmadeRememberedRoadRegistry.h';s=build()
 if args.check:
  if not p.exists() or p.read_text()!=s:raise SystemExit('Remembered Road registry drift')
 else:p.write_text(s)
 print('11 authored city adventures and 88 residents: registry current')
if __name__=='__main__':main()
