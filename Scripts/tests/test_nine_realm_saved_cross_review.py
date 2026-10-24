"""Only physically reviewed local proof may become a receiving chief's settled comparison."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
class SavedCrossReview(unittest.TestCase):
 def test_nine_physical_review_receipts_survive_load_and_fail_closed(self):
  source=r"""
#include "World/UnmadeCrossRealmCarryRules.h"
#include <array>
#include <cassert>
using namespace UnmadeCore;
int main(){
 std::array<bool,9> yes{},no{};yes.fill(true);no.fill(false);
 CrossRealmCarry state;
 for(int from=0;from<9;++from){
  const int dest=CrossRealmDestination(from);
  assert(state.Collect(from,1,true)==CarryEvent::Collected);
  assert(state.Review(from,dest,2,true,true)==CarryEvent::NotReady);
  assert(state.Deliver(from,dest,2,true)==CarryEvent::Delivered);
  assert(state.Review(from,dest,2,false,true)==CarryEvent::NotReady);
  assert(state.Review(from,from,2,true,true)==CarryEvent::WrongRecipient);
  assert(state.Review(from,dest,2,true,false)==CarryEvent::NotReady);
  assert(state.Review(from,dest,2,true,true)==CarryEvent::Reviewed);
  assert(state.Stage(from)==CarryStage::Reviewed);
  assert(state.Review(from,dest,3,true,true)==CarryEvent::AlreadyDone);
 }
 auto packed=state.Pack();
 assert(packed.size()==28 && packed[0]==1);
 CrossRealmCarry restored;
 assert(restored.Restore(packed,3,yes,yes,yes));
 assert(!CrossRealmCarry{}.Restore(packed,3,yes,yes,no));
 auto original=packed;
 CrossRealmCarry failed;
 assert(failed.Collect(0,1,true)==CarryEvent::Collected);
 assert(failed.Deliver(0,1,2,true)==CarryEvent::Delivered);
 bool committed=CommitCrossRealmCarry(failed,[&]{
  return failed.Review(0,1,2,true,true);
 },[]{return false;});
 assert(!committed && failed.Stage(0)==CarryStage::Delivered);
 assert(failed.Pack()!=original);
}
"""
  with tempfile.TemporaryDirectory() as temp:
   path=Path(temp)/"review.cpp";exe=Path(temp)/"review";path.write_text(source)
   b=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(path),"-o",str(exe)],capture_output=True,text=True)
   self.assertEqual(b.returncode,0,b.stderr)
   run=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(run.returncode,0,run.stderr)
 def test_new_review_is_in_existing_snapshot_validator_without_extra_save_fields(self):
  save=(ROOT/"Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h").read_text()
  world=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp").read_text()
  combined=(ROOT/"Source/TheUnmadeGame/Public/Save/UnmadeCombinedSaveRules.h").read_text()
  native=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp").read_text()
  self.assertNotIn("CrossRealmReviewState",save)
  self.assertIn("RestoredCarry.Restore(data,RestoredClock.DayIndex()+1,sourceReady,destVisited,localEvidence)",world)
  self.assertIn("proof.Restore(data,RestoredClock.DayIndex()+1,",combined)
  self.assertIn("CrossRealmCarry.Review(source,arc,",native)
  self.assertIn("CommitCrossRealmCarry(",native)
  self.assertIn("Person(GetWorld(),arc,0)",native)
  self.assertIn("WriteWorldSnapshot()",native)
if __name__=="__main__":unittest.main()
