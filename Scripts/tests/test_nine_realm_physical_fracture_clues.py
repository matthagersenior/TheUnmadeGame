"""Reality-fracture clues are physical, distinct and earned, never magic progress."""
import subprocess,tempfile,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def source(path):return (ROOT/path).read_text(encoding="utf-8")
class PhysicalFirstFractures(unittest.TestCase):
 def test_nine_different_lore_appropriate_clues_without_spoilers(self):
  c=r'''
#include "World/UnmadeFirstFractureRules.h"
#include <cassert>
#include <set>
#include <string>
#include <utility>
using namespace UnmadeCore;
int main(){
 std::set<std::pair<int,int>> positions;
 std::set<std::string> hints;
 for(int a=0;a<9;++a){
  const auto& cue=FirstFracturePhysicals[a];
  positions.insert({cue.dx,cue.dy});
  hints.insert(cue.investigation);
  assert(cue.scaleX>0&&cue.scaleY>0&&cue.scaleZ>0);
  assert(cue.scaleX*cue.scaleY*cue.scaleZ>0.05);
  assert(cue.spin>=.2&&cue.spin<=2.5);
  assert(cue.investigation&&std::string(cue.investigation).size()>60);
 }
 assert(positions.size()==9&&hints.size()==9);
 assert(FirstFracturePhysical(-1)==nullptr);
 assert(FirstFracturePhysical(9)==nullptr);
}
'''
  with tempfile.TemporaryDirectory() as temp:
   p=Path(temp)/"test.cpp";exe=Path(temp)/"test";p.write_text(c)
   build=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(p),"-o",str(exe)],capture_output=True,text=True)
   self.assertEqual(build.returncode,0,build.stderr)
   run=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(run.returncode,0,run.stderr)
 def test_physical_world_clue_is_hidden_until_actual_recorded_discovery(self):
  cpp=source("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  header=source("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  self.assertIn("ExpeditionFractureActors.SetNum(9)",cpp)
  self.assertIn("ExpeditionFractureActors[a]=relic",cpp)
  self.assertIn("FirstFracturePhysicals[a]",cpp)
  self.assertIn("relic->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision)",cpp)
  self.assertIn("relic->SetActorHiddenInGame(true)",cpp)
  self.assertIn("ExpeditionFractureActors[a]->SetActorHiddenInGame(!observed)",cpp)
  self.assertIn("(t->rooms&1)!=0",cpp)
  self.assertIn("bExpeditionSaveRejected",cpp)
  self.assertIn("ExpeditionFractureActors",header)
  self.assertIn("ExpeditionFractureActors[a]->SetActorRotation(",cpp)
 def test_physical_reinspection_provides_actionable_bounded_lead(self):
  cpp=source("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  self.assertIn("Candidate(a,14,0,",cpp)
  self.assertIn("case 14:",cpp)
  self.assertIn("FirstFracturePhysical(arc)",cpp)
  self.assertIn("FIRSTHAND FRACTURE",cpp)
  self.assertIn("E - EXAMINE",cpp)
  segment=cpp.split("case 14:",1)[1].split("case 13:",1)[0]
  self.assertIn("investigation",segment)
  for forbidden in ("Expeditions.Boss(","Expeditions.Claim(","Expeditions.Rescue(","WriteWorldSnapshot(","Depth.Choose(","CrossRealmCarry."):
   self.assertNotIn(forbidden,segment)
 def test_no_public_lore_secret_or_unearned_map_location(self):
  rules=source("Source/TheUnmadeGame/Public/World/UnmadeFirstFractureRules.h")
  for secret in ("Nhal-Vey","Held Morning","Many Mornings"):
   self.assertNotIn(secret,rules)
  cpp=source("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  self.assertIn("t->rooms&1",cpp)
  self.assertNotIn("FractureProgressState",source("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h"))
if __name__=="__main__":unittest.main()
