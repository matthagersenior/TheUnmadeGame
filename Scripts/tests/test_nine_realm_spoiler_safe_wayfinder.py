"""Tracked objectives have honest, physically useful map bearings without spoilers."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
def file(p):return (ROOT/p).read_text(encoding="utf-8")

class SpoilerSafeWayfinding(unittest.TestCase):
 def test_all_nine_realms_use_earned_steps_not_omniscient_undiscovered_rooms(self):
  code=r"""
#include "World/UnmadeRealmJourneyRules.h"
#include <cassert>
#include <cmath>
using namespace UnmadeCore;
int main(){
 for(int a=0;a<9;++a){
  ExpeditionState e{};DepthArc d{};ExperienceArc x{};
  auto board=NextRealmWaypoint(a,e,d,x);
  assert(board.available&&board.precise&&!board.optional&&board.focus==JourneyFocus::Main);
  assert(std::abs(board.x-(ExpeditionSpecs[a].x+700))<.01);
  e.brief=1;
  auto frontier=NextRealmWaypoint(a,e,d,x);
  assert(frontier.available&&frontier.precise);
  assert(std::abs(frontier.y-(ExpeditionSpecs[a].y+8500))<.01);
  e.visited=1;
  auto first=NextRealmWaypoint(a,e,d,x);
  assert(first.available&&first.precise&&first.step==JourneyStep::Chamber);
  auto room0=DungeonRoom(a,0);
  assert(std::abs(first.x-(ExpeditionSpecs[a].x+room0.x))<.01);
  e.rooms=1;
  auto next=NextRealmWaypoint(a,e,d,x);
  assert(next.available&&!next.precise&&next.step==JourneyStep::Chamber);
  assert(std::abs(next.x-(ExpeditionSpecs[a].x+room0.x))<.01);
  assert(std::abs(next.y-(ExpeditionSpecs[a].y+15000+room0.y))<.01);
  e.rooms=31;e.branch=2;e.clues=7;
  auto bossEntry=NextRealmWaypoint(a,e,d,x);
  assert(bossEntry.step==JourneyStep::Chamber&&bossEntry.index==5);
  assert(!bossEntry.precise); // cannot reveal the unknown chamber six
  e.rooms=63;
  auto combat=NextRealmWaypoint(a,e,d,x);
  assert(combat.step==JourneyStep::Boss&&combat.precise);
  e.boss=2;
  auto home=NextRealmWaypoint(a,e,d,x);
  assert(home.step==JourneyStep::Return&&home.precise);
  e.returned=1;auto gear=NextRealmWaypoint(a,e,d,x);
  assert(gear.step==JourneyStep::Reward&&gear.precise);
  e.reward=1;
  auto personal=NextRealmWaypoint(a,e,d,x);
  assert(personal.focus==JourneyFocus::Personal&&personal.optional&&!personal.precise);
  d.offered=1;d.witness=1;
  auto evidence=NextRealmWaypoint(a,e,d,x);
  assert(evidence.step==JourneyStep::PersonalEvidence&&evidence.precise);
  d.evidence=1;d.choice=1;
  auto wing=NextRealmWaypoint(a,e,d,x);
  assert(wing.focus==JourneyFocus::Optional&&wing.optional&&!wing.precise);
  x.offered=1;
  auto wingStation=NextRealmWaypoint(a,e,d,x);
  assert(wingStation.step==JourneyStep::WingFieldwork&&wingStation.optional&&!wingStation.precise);
  x.steps=7;
  auto guardian=NextRealmWaypoint(a,e,d,x);
  assert(guardian.step==JourneyStep::WingGuardian&&guardian.optional&&!guardian.precise);
  x.guardian=1;
  auto reward=NextRealmWaypoint(a,e,d,x);
  assert(reward.step==JourneyStep::WingReward&&reward.optional&&reward.precise);
  x.complete=1;
  auto finished=NextRealmWaypoint(a,e,d,x);
  assert(!finished.available&&finished.focus==JourneyFocus::FreeRoam);
 }
 ExpeditionState e{};DepthArc d{};ExperienceArc x{};
 assert(!NextRealmWaypoint(-1,e,d,x).available);
 assert(!NextRealmWaypoint(9,e,d,x).available);
}
"""
  with tempfile.TemporaryDirectory() as d:
   src=Path(d)/"test.cpp";out=Path(d)/"test";src.write_text(code)
   b=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(src),"-o",str(out)],capture_output=True,text=True)
   self.assertEqual(b.returncode,0,b.stderr)
   x=subprocess.run([str(out)],capture_output=True,text=True)
   self.assertEqual(x.returncode,0,x.stderr)
 def test_journal_and_map_share_same_real_goal(self):
  src=file("Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp")
  self.assertIn("NextRealmWaypoint(a,*Expeditions.Arc(a),*Depth.Arc(a),*Experience.Arc(a))",src)
  self.assertIn("MAIN QUEST",src)
  self.assertIn("OPTIONAL PERSONAL STORY",src)
  self.assertIn("OPTIONAL WING",src)
  self.assertIn("WAYFINDER",src)
  self.assertIn("LAST RECORDED POSITION",src)
  self.assertIn("NEXT KNOWN POSITION",src)
  self.assertIn("waypoint.x",src)
  self.assertIn("waypoint.y",src)
  self.assertIn("IsRealmKnown(a)",src)
  self.assertNotIn("UnmadeCore::DungeonRoom(a,goal.index)",src)
if __name__=="__main__":unittest.main()
