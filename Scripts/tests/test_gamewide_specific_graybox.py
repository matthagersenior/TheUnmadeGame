"""Every major game-wide graybox family must have a concrete semantic art kit."""
import subprocess,tempfile,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def src(p):return (ROOT/p).read_text(encoding="utf8")
class GamewideHotbox(unittest.TestCase):
 def test_portable_lore_specific_art_taxonomy_and_safe_footprints(self):
  code=r'''
#include "World/UnmadeGamewideGrayboxRules.h"
#include <cassert>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
 struct Entry {const char* name;int realm;GrayboxKit kit;};
 const Entry cases[]={
 {"Village.Bellwold",0,GrayboxKit::Ground},
 {"Village.Commons",0,GrayboxKit::Commons},
 {"Bellwold.Belltower",0,GrayboxKit::BellTower},
 {"Bellwold.Refuge",0,GrayboxKit::Sanctuary},
 {"Paperhaven.ArchiveTower",0,GrayboxKit::ArchiveTower},
 {"Paperhaven.Registry",0,GrayboxKit::Records},
 {"Village.Trades",0,GrayboxKit::Market},
 {"Village.Watchpost",0,GrayboxKit::Watch},
 {"Road.4.YardFloor",2,GrayboxKit::Ground},
 {"Road.4.ClueStand",2,GrayboxKit::Evidence},
 {"Road.8.ReceivingShelf",6,GrayboxKit::Cargo},
 {"Road.Home.17.Bed",0,GrayboxKit::Bed},
 {"Road.Home.22.Hearth",0,GrayboxKit::Hearth},
 {"Road.Home.51.Table",static_cast<int>(RoadCities[RoadPeople[51].city].realm),GrayboxKit::Table},
 {"Expedition.8.CivicCareGate",8,GrayboxKit::Gate},
 {"Later.0.Aftermath.Gate.Truth",3,GrayboxKit::Gate},
 {"Expedition.4.ArenaShelter",4,GrayboxKit::Sanctuary},
 {"Expedition.4.Landmark.0",4,GrayboxKit::Landmark},
 {"Resonance.5.Plinth",8,GrayboxKit::Ritual},
 {"Echo.2.NeighborSignal",5,GrayboxKit::Signal},
 {"Atlas.Gateway.8.4",8,GrayboxKit::Portal},
 {"Final.Control.Anchor",8,GrayboxKit::FinalChoice},
 {"Final.World.0.Many",0,GrayboxKit::FinalChoice},
 {"Final.World.8.Anchor",8,GrayboxKit::FinalChoice},
 {"Cinderhold.Adventure.SharedKilnAccord",2,GrayboxKit::Mechanism},
 {"Saltwake.CivicWitnessBoard",1,GrayboxKit::Evidence},
 {"Depth.Cache.7",7,GrayboxKit::Cargo},
 {"Depth.CivicWork.3.1",3,GrayboxKit::Civic},
 {"Civic.PublicEvidencePlatform",0,GrayboxKit::Civic},
 {"Relay.CinderholdCopy",2,GrayboxKit::Records},
 {"Bes.WarmHearth",2,GrayboxKit::Hearth},
 {"WitnessBraid.RefugeCord",0,GrayboxKit::Civic},
 {"Questless.Seat",0,GrayboxKit::Generic}
 };
 for(const auto& entry:cases){
  const auto art=ClassifyGrayboxArt(entry.name,1,1,1);
  assert(art.realm==entry.realm);
  assert(art.kit==entry.kit);
  assert(art.footprintUnchanged);
  assert(art.detail&&art.replacementMesh&&art.detail[0]&&art.replacementMesh[0]);
 }
 assert(ClassifyGrayboxArt("Expedition.99.CivicCareGate",1,1,1).realm==-1);
 assert(ClassifyGrayboxArt("Road.11.Crate",1,1,1).realm==-1);
 assert(ClassifyGrayboxArt("Road.Home.999.Hearth",1,1,1).realm==-1);
 assert(ClassifyGrayboxArt("Later.6.Aftermath",1,1,1).realm==-1);
 assert(ClassifyGrayboxArt("Hub.Ground",52,52,1).kit==GrayboxKit::Ground);
 assert(!ClassifyGrayboxArt("Hub.Ground",52,52,1).decorate);
 assert(!ClassifyGrayboxArt("Road.Home.0.Floor",6,5,.35).decorate);
 assert(ClassifyGrayboxArt("Bellwold.Refuge",4,3,3).decorate);
 assert(ClassifyGrayboxArt("Road.Home.0.Hearth",.65,.65,.5).decorate);
 assert(GrayboxProfiles.size()>=20);
 std::set<std::string> replacements;
 for(const auto& family:GrayboxProfiles){
  assert(family.label&&family.surface&&family.material&&family.detail);
  assert(family.replacementMesh&&replacements.insert(family.replacementMesh).second);
 }
}
'''
  with tempfile.TemporaryDirectory() as d:
   p=Path(d)/"case.cpp";exe=Path(d)/"case"
   p.write_text(code)
   b=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(p),"-o",str(exe)],capture_output=True,text=True)
   self.assertEqual(b.returncode,0,b.stderr)
   r=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
 def test_common_gamewide_builder_styles_canonical_collision_objects(self):
  world=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
  rules=src("Source/TheUnmadeGame/Public/World/UnmadeGamewideGrayboxRules.h")
  roads=src("Source/TheUnmadeGame/Private/World/UnmadeRememberedRoads.cpp")
  encounters=src("Source/TheUnmadeGame/Private/World/UnmadePlayableEncounters.cpp")
  for x in ("ClassifyGrayboxArt(","GrayboxKit::Hearth","GrayboxKit::Bed","GrayboxKit::Market",
            "GrayboxKit::Sanctuary","GrayboxKit::Portal","GrayboxKit::Records",
            "GrayboxKit::FinalChoice","GrayboxKit::Ritual"):
   self.assertIn(x,world)
  self.assertIn("StyleWorldProp(Block,Label,Scale)",world)
  self.assertIn("SetCollisionEnabled(ECollisionEnabled::NoCollision)",world)
  self.assertIn("StyleWorldProp",roads)
  self.assertIn("StyleWorldProp",encounters)
  self.assertIn("GrayboxProfiles",rules)
 def test_art_choices_are_registered_before_content_replacement(self):
  qa=src("docs/qa/gamewide-specific-graybox-art.md")
  for word in ("88 homes","11 cargo","nine realms","Bellwold","Paperhaven",
               "Unanswered Road","Nhal-Vey","asset replacement","collision","first-person"):
   self.assertIn(word,qa)
if __name__=="__main__":unittest.main()
