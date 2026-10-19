"""Native-mesh-based realm art direction must preserve the gameplay collision model."""
import subprocess
import tempfile
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def read(path):return (ROOT/path).read_text(encoding="utf-8")
class RealmVisualIdentityTest(unittest.TestCase):
 def test_nine_distinct_landmarks_and_safe_structural_tags(self):
  code=r'''
#include "World/UnmadeProceduralArtRules.h"
#include <cassert>
#include <cstring>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
 std::set<std::string> names, colors;
 for(int a=0;a<9;++a){
   const auto& d=RealmVisualDesigns[a];
   assert(d.name && d.landmark && d.storyMotif && d.color.R>=0);
   assert(names.insert(d.landmark).second);
   colors.insert(std::to_string(d.color.R)+":"+std::to_string(d.color.G)+":"+std::to_string(d.color.B));
   char label[80];
   std::snprintf(label,sizeof(label),"Expedition.%d.CivicCareGate",a);
   auto gate=ClassifyRealmProp(label,3,0.4,2.0);
   assert(gate.realm==a && gate.role==VisualRole::Gate);
   assert(gate.keepCubeCollision && gate.allowDecoration);
   std::snprintf(label,sizeof(label),"Expedition.%d.Structure",a);
   auto wall=ClassifyRealmProp(label,100,170,0.5);
   assert(wall.role==VisualRole::Foundation && wall.keepCubeCollision && !wall.allowDecoration);
   std::snprintf(label,sizeof(label),"Expedition.%d.Landmark.0",a);
   auto sig=ClassifyRealmProp(label,1,1,3);
   assert(sig.realm==a&&sig.role==VisualRole::Landmark&&sig.allowDecoration);
   for(int j=0;j<3;++j){
     const auto& piece=d.silhouette[j];
     assert(piece.sx>0&&piece.sy>0&&piece.sz>0);
     assert(piece.dx<-1200);
   }
 }
 assert(colors.size()==9);
 assert(ClassifyRealmProp("Saltwake.Aftermath.RainLedger",0.6,.6,1.5).realm==1);
 assert(ClassifyRealmProp("Cinderhold.Adventure.SharedKilnAccord",.8,.6,1.0).realm==2);
 assert(ClassifyRealmProp("Orchard.Aftermath.CommonRoots",1,1,1).realm==7);
 assert(ClassifyRealmProp("Absence.Aftermath.OriginDial",1,1,1).realm==8);
 assert(ClassifyRealmProp("Hub.Ground",52,52,1).role==VisualRole::Foundation);
 assert(ClassifyRealmProp("Expedition.8.CivicTruthRoute",7,8,.35).role==VisualRole::Foundation);
 assert(ClassifyRealmProp("Expedition.99.Structure",1,1,1).realm==-1);
 assert(ClassifyRealmProp("Expedition.1x.Structure",1,1,1).realm==-1);
 assert(ClassifyRealmProp("",1,1,1).realm==-1);
}
'''
  with tempfile.TemporaryDirectory() as temp:
   p=Path(temp)/"visual.cpp";q=Path(temp)/"visual"
   p.write_text("#include <cstdio>\n"+code,encoding="utf-8")
   cmd=["g++","-std=c++17","-Wall","-Wextra","-Werror",
       "-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(p),"-o",str(q)]
   b=subprocess.run(cmd, text=True,capture_output=True)
   self.assertEqual(b.returncode,0,b.stderr)
   r=subprocess.run([str(q)],text=True,capture_output=True)
   self.assertEqual(r.returncode,0,r.stderr)
 def test_native_styling_preserves_collision_gate_toggle_and_spawn_owner(self):
  hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
  h=read("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  expedition=read("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  for token in ("ClassifyRealmProp(","StyleWorldProp(","MaterialInstanceDynamic",
                "SetCollisionEnabled(ECollisionEnabled::NoCollision)",
                "SetupAttachment(","RegisterComponent("):
   self.assertIn(token,hub)
  self.assertIn("void StyleWorldProp(",h)
  self.assertIn("StyleWorldProp(Block,Label,Scale)",hub)
  self.assertIn("RealmVisualDesigns[a]",expedition)
  self.assertIn("Expedition.%d.Landmark.%d",expedition)
  self.assertIn("SpawnBlock(",expedition)
  self.assertIn("SetRiteWorldActorState",hub)
  self.assertIn("Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics)",hub)
 def test_firsthand_fractures_still_nonblocking_and_realm_styled(self):
  expedition=read("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  self.assertIn("StyleWorldProp(relic",expedition)
  self.assertIn("SetCollisionEnabled(ECollisionEnabled::NoCollision)",expedition)
  self.assertIn("FirstFracturePhysicals[a]",expedition)
if __name__=="__main__":unittest.main()
