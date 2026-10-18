"""A witnessed local discovery can earn one physical route-puzzle assist, not a free gate."""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")

class WitnessGuidedRoutes(unittest.TestCase):
 def test_nine_manual_puzzles_have_one_earned_control_seed_without_free_victory(self):
  code=r'''
#include "World/UnmadeAdventureDepthRules.h"
#include <cassert>
using namespace UnmadeCore;
int main(){
 const int remaining[9][2]={{2,1},{2,-1},{1,2},{1,-1},{1,2},{1,2},{1,2},{0,1},{2,-1}};
 for(int a=0;a<9;++a){
  RealmChallenge original;
  assert(original.Progress()==0&&original.Mask()==0);
  assert(original.Prime(a,4));
  assert(original.Progress()!=0||original.Mask()!=0);
  assert(!original.Prime(a,4));
  // Pre-aligning one witnessed control never triggers a saved route or victory.
  if(a==1){auto r=original.Attempt(a,2,6,true,true);assert(r.outcome==ChallengeOutcome::Complete);}
  else if(a==3){auto r=original.Attempt(a,1,6,true,true);assert(r.outcome==ChallengeOutcome::Complete);}
  else if(a==8){auto r=original.Attempt(a,2,6,true,true);assert(r.outcome==ChallengeOutcome::Complete);}
  else {
   auto first=original.Attempt(a,remaining[a][0],6,true,true);
   assert(first.outcome!=ChallengeOutcome::Complete);
   auto second=original.Attempt(a,remaining[a][1],7,true,true);
   assert(second.outcome==ChallengeOutcome::Complete);
  }
 }
 RealmChallenge bad;
 assert(!bad.Prime(-1,1));
 assert(!bad.Prime(9,1));
 assert(!bad.Prime(0,-1));
 assert(!bad.Prime(0,1e309)); // nonfinite timestamps forbidden
 assert(bad.Progress()==0&&bad.Mask()==0);
 RealmChallenge eillun;
 assert(eillun.Prime(6,4));
 assert(eillun.Attempt(6,1,6,true,false).outcome==ChallengeOutcome::Observed);
 RealmChallenge orravane;
 assert(orravane.Prime(4,4));
 assert(orravane.Attempt(4,1,6,false,true).outcome==ChallengeOutcome::NeedElevation);
}
'''
  code=code.replace('1e309','__builtin_huge_val()')
  with tempfile.TemporaryDirectory() as temp:
   src=Path(temp)/"try.cpp";executable=Path(temp)/"try"
   src.write_text(code,encoding="utf-8")
   compilation=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(src),"-o",str(executable)],capture_output=True,text=True)
   self.assertEqual(compilation.returncode,0,compilation.stderr)
   process=subprocess.run([str(executable)],capture_output=True,text=True)
   self.assertEqual(process.returncode,0,process.stderr)

 def test_requires_physically_returning_to_named_witness_then_operating_route_plan(self):
  world=read("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  header=read("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  self.assertIn("DepthWitnessBriefUntil",header)
  self.assertIn("DepthWitnessPlanSigns",header)
  self.assertIn("DepthWitnessPlanSigns.SetNum(9)",world)
  self.assertIn("if(kind==1)",world)
  self.assertIn("DepthWitnessBriefUntil[arc]=GetWorld()->GetTimeSeconds()+",world)
  self.assertIn("Depth.Arc(arc)->evidence",world)
  self.assertIn("Expeditions.Arc(arc)->visited",world)
  self.assertIn("Add(a,9,0,",world)
  self.assertIn("if(kind==9)",world)
  self.assertIn("DepthChallenges[arc].Prime(arc,",world)
  self.assertIn("E  Apply witness-guided route plan",world)
  self.assertIn("A witness can guide one control",world)
  segment=world.split("if(kind==9)",1)[1].split("if(kind==3)",1)[0]
  self.assertNotIn("Depth.Route(",segment)
  self.assertNotIn("WriteWorldSnapshot(",segment)
  self.assertIn("DepthWitnessBriefUntil[arc]=0",segment)
  self.assertIn("REALM",world)

if __name__=="__main__":unittest.main()
