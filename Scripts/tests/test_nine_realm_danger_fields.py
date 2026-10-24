"""Ensure each realm's dungeon danger field has a real, readable safe lane."""
import subprocess
import tempfile
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
class NineRealmDangerFields(unittest.TestCase):
 def test_nine_geometry_patterns_have_tangible_safe_and_unsafe_samples(self):
  program=r"""
#include "World/UnmadeIdentityRules.h"
#include <cassert>
using namespace UnmadeCore;
int main(){
 const double r=280;
 struct Point {double x,y;};
 Point safe[9]={{0,0},{150,0},{0,-150},{160,160},{140,-140},{0,0},{0,0},{300,0},{0,0}};
 Point danger[9]={{200,0},{0,0},{0,150},{0,150},{140,140},{180,0},{0,180},{0,0},{250,0}};
 for(int a=0;a<9;++a){
  assert(!RealmHazardFootprint(a,safe[a].x,safe[a].y,r));
  assert(RealmHazardFootprint(a,danger[a].x,danger[a].y,r));
 }
 assert(!RealmHazardFootprint(-1,0,0,r));
 assert(!RealmHazardFootprint(9,0,0,r));
 assert(!RealmHazardFootprint(0,0,0,-1));
}
"""
  with tempfile.TemporaryDirectory() as tmp:
   src=Path(tmp)/"hazards.cpp";exe=Path(tmp)/"hazards"
   src.write_text(program)
   result=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",
     str(ROOT/"Source/TheUnmadeGame/Public"),str(src),"-o",str(exe)],capture_output=True,text=True)
   self.assertEqual(result.returncode,0,result.stderr)
   ran=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(ran.returncode,0,ran.stderr)
 def test_actor_applies_shape_only_inside_existing_ready_pulse_and_displays_tell(self):
  native=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp").read_text()
  self.assertIn("SampleDungeonHazard(",native)
  self.assertIn("EncounterHazardInCylinder(",native)
  self.assertIn("RealmHazardFootprint(a,hazardOffset.X,hazardOffset.Y,280)",native)
  self.assertIn("ExpeditionMove(UnmadeCore::ExpeditionRole::Boss,a,2,false).warning",native)
  self.assertIn("ReceiveHazardPulse(",native)
if __name__=="__main__":unittest.main()
