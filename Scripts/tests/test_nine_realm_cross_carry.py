"""Real hand-carried nine-realm witnesses, not world-state teleportation or duplicate rewards."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
def file(path):return (ROOT/path).read_text(encoding="utf-8")
class CrossRealmCarry(unittest.TestCase):
 def test_nine_real_deliveries_and_corrupt_restore_guards(self):
  c=r'''
#include "World/UnmadeCrossRealmCarryRules.h"
#include <cassert>
#include <array>
#include <vector>
using namespace UnmadeCore;
int main(){
 CrossRealmCarry chain;
 std::array<bool,9> yes{},no{};
 yes.fill(true);no.fill(false);
 for(int a=0;a<9;++a){
  assert(CrossRealmDestination(a)==(a+1)%9);
  assert(chain.Collect(a,2,false)==CarryEvent::NotReady);
  assert(chain.Collect(a,2,true)==CarryEvent::Collected);
  assert(chain.Collect(a,2,true)==CarryEvent::AlreadyDone);
  assert(chain.Deliver(a,a,2,true)==CarryEvent::WrongRecipient);
  assert(chain.Deliver(a,(a+1)%9,2,false)==CarryEvent::NotReady);
  assert(chain.Deliver(a,(a+1)%9,3,true)==CarryEvent::Delivered);
  assert(chain.Deliver(a,(a+1)%9,4,true)==CarryEvent::AlreadyDone);
 }
 auto saved=chain.Pack();
 assert(saved.size()==28&&saved[0]==1);
 CrossRealmCarry restored;
 assert(restored.Restore(saved,4,yes,yes));
 assert(restored.Stage(8)==CarryStage::Delivered);
 auto bad=saved;bad[1+8*3+2]=1;
 assert(!restored.Restore(bad,4,yes,yes));
 assert(restored.Stage(8)==CarryStage::Delivered);
 assert(!CrossRealmCarry{}.Restore(saved,4,no,yes));
 assert(!CrossRealmCarry{}.Restore(saved,4,yes,no));
 assert(!CrossRealmCarry{}.Restore(saved,2,yes,yes));
 bad=saved;bad.pop_back();
 assert(!CrossRealmCarry{}.Restore(bad,4,yes,yes));
 assert(!CrossRealmCarry{}.Collect(9,1,true)==false); // invalid index must never grant custody
 CrossRealmCarry fresh;
 assert(fresh.Deliver(0,1,2,true)==CarryEvent::NotReady);
 assert(fresh.Collect(0,0,true)==CarryEvent::InvalidClock);
 assert(fresh.Collect(0,1,true)==CarryEvent::Collected);
 assert(fresh.Deliver(0,1,0,true)==CarryEvent::InvalidClock);
 assert(fresh.Deliver(0,1,1,true)==CarryEvent::Delivered);
}
'''
  # Check the real API with a host-free C++ compile, not a Python model copy.
  c=c.replace("assert(!CrossRealmCarry{}.Collect(9,1,true)==false); // invalid index must never grant custody","assert(CrossRealmCarry{}.Collect(9,1,true)==CarryEvent::InvalidRealm);")
  with tempfile.TemporaryDirectory() as temp:
   p=Path(temp)/"main.cpp";exe=Path(temp)/"main";p.write_text(c)
   b=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(p),"-o",str(exe)],capture_output=True,text=True)
   self.assertEqual(b.returncode,0,b.stderr)
   x=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(x.returncode,0,x.stderr)
 def test_capture_and_restore_are_additive_and_protect_malformed_prior_save(self):
  h=file("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  save=file("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
  game=file("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
  self.assertIn("UnmadeCore::CrossRealmCarry CrossRealmCarry;",h)
  self.assertIn("bCrossRealmCarryRejected",h)
  self.assertIn("bHasCrossRealmCarry",save)
  self.assertIn("CrossRealmCarryState",save)
  self.assertIn("Save->CrossRealmCarryState",game)
  self.assertIn("RestoredCarry.Restore(",game)
  self.assertIn("bCrossRealmCarryRejected)return false",game)
  self.assertIn("CrossRealmCarry=RestoredCarry",game)
 def test_collect_and_deliver_require_physical_source_and_recipient(self):
  world=file("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  self.assertIn("CrossRealmCarry.Collect(arc",world)
  self.assertIn("CrossRealmCarry.Deliver(source,arc",world)
  self.assertIn("CommitCrossRealmCarry(",world)
  self.assertIn("kind==7",world)
  self.assertIn("kind==0",world)
  self.assertIn("Expeditions.Arc(source)->visited",world)
  self.assertIn("Identity.Arc(arc)->repair==7",world)
  self.assertIn("WriteWorldSnapshot(",world)
  self.assertIn("CROSS-REALM CUSTODY",world)
  self.assertIn("GetCrossRealmCarryJournal",world)
if __name__=="__main__":unittest.main()
