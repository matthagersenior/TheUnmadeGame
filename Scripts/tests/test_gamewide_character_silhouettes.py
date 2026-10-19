"""Person and creature temporary silhouettes must be recognizable without authored rigs."""
import subprocess,tempfile,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf8")
class CharacterGrayboxTest(unittest.TestCase):
 def test_twelve_creatures_five_roles_with_distinct_profiles(self):
  code=r'''
#include "World/UnmadeCharacterGrayboxRules.h"
#include <cassert>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
 assert(ResidentGrayboxVisuals.size()==5);
 assert(CreatureGrayboxVisuals.size()==12);
 std::set<std::string> names;
 for(const auto& e:CreatureGrayboxVisuals){
   assert(e.asset&&e.asset[0]&&e.signature&&e.signature[0]);
   assert(e.sx>0&&e.sy>0&&e.sz>0);
   assert(e.crestScale>0);
   assert(names.insert(e.asset).second);
 }
 names.clear();
 for(const auto& n:ResidentGrayboxVisuals){
   assert(n.asset&&n.asset[0]&&n.roleObject&&n.roleObject[0]);
   assert(names.insert(n.asset).second);
 }
 assert(CreatureGraybox(CreatureKind::Inkmaw).asset!=CreatureGraybox(CreatureKind::Ashmaw).asset);
 assert(ResidentGraybox(NpcRole::Courier).asset!=ResidentGraybox(NpcRole::Scholar).asset);
}
'''
  with tempfile.TemporaryDirectory() as d:
   source=Path(d)/"case.cpp";out=Path(d)/"case";source.write_text(code)
   b=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror",
     "-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(source),"-o",str(out)],capture_output=True,text=True)
   self.assertEqual(b.returncode,0,b.stderr)
   r=subprocess.run([str(out)],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
 def test_visible_role_and_enemy_shapes_not_collision(self):
  npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
  enemy=read("Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp")
  for x in ("ResidentGraybox(", "AccessoryVisual", "SetCollisionEnabled(ECollisionEnabled::NoCollision)"):
   self.assertIn(x,npc)
  for x in ("CreatureGraybox(", "EncounterCrest", "SetRelativeScale3D"):
   self.assertIn(x,enemy)
  self.assertIn("PlaceholderVisual->SetVisibility(false)",npc)
if __name__=="__main__":unittest.main()
