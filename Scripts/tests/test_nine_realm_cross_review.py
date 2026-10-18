"""Destination realms must challenge physically received testimony with their own evidence."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class CrossRealmReview(unittest.TestCase):
 def test_nine_custody_and_local_evidence_proofs(self):
  code=r"""
#include "World/UnmadeCrossRealmReviewRules.h"
#include <cassert>
#include <set>
using namespace UnmadeCore;
int main(){
 std::set<int> sources;
 for(int target=0;target<9;++target){
  int origin=CrossRealmSource(target);
  sources.insert(origin);
  assert(origin>=0&&origin<9);
  assert(CrossRealmDestination(origin)==target);
  assert(CrossRealmReview(target,CarryStage::Waiting,true,true,true)==ReviewState::Absent);
  assert(CrossRealmReview(target,CarryStage::Carrying,true,true,true)==ReviewState::Absent);
  assert(CrossRealmReview(target,CarryStage::Delivered,false,true,true)==ReviewState::Unavailable);
  assert(CrossRealmReview(target,CarryStage::Delivered,true,true,false)==ReviewState::Unavailable);
  assert(CrossRealmReview(target,CarryStage::Delivered,true,false,true)==ReviewState::NeedEvidence);
  assert(CrossRealmReview(target,CarryStage::Delivered,true,true,true)==ReviewState::ReadyToCompare);
  assert(CrossRealmReview(target,CarryStage::Reviewed,true,true,true)==ReviewState::Compared);
 }
 assert(sources.size()==9);
 assert(CrossRealmSource(-1)==-1);
 assert(CrossRealmSource(9)==-1);
 assert(CrossRealmReview(-1,CarryStage::Delivered,true,true,true)==ReviewState::Unavailable);
}
"""
  with tempfile.TemporaryDirectory() as d:
   source=Path(d)/"review.cpp";exe=Path(d)/"review"
   source.write_text(code)
   built=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(source),"-o",str(exe)],capture_output=True,text=True)
   self.assertEqual(built.returncode,0,built.stderr)
   run=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(run.returncode,0,run.stderr)
 def test_physical_workstation_changes_only_on_verified_destination_evidence(self):
  world=read("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  hub=read("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  self.assertIn("DepthCrossRealmReviewActors",hub)
  self.assertIn("DepthCrossRealmReviewActors.SetNum(9)",world)
  self.assertIn("DepthCrossRealmReviewActors[a]->SetActorHiddenInGame(",world)
  self.assertIn("DepthCrossRealmReviewActors[a]->SetActorScale3D(",world)
  self.assertIn("ReviewState::Compared",world)
  self.assertIn("Depth.Arc(a)->evidence",world)
  self.assertIn("CrossRealmCarry.Stage(incoming)",world)
  self.assertIn("SetCollisionEnabled(ECollisionEnabled::NoCollision)",world)
 def test_review_is_onsite_and_never_grants_progress(self):
  world=read("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  self.assertIn("Add(a,8,0,",world)
  self.assertIn("if(kind==8)",world)
  self.assertIn("GetCrossRealmReviewJournal",world)
  self.assertIn("E  Compare incoming report with local evidence",world)
  review=world.split("if(kind==8)",1)[1].split("if(kind==5",1)[0]
  self.assertIn("ReviewState::NeedEvidence",review)
  self.assertIn("ReviewState::ReadyToCompare",review)
  self.assertIn("ReviewState::Compared",review)
  self.assertIn("UnmadePresentation::Sound(p,TEXT(\"inspect\"))",review)
  self.assertNotIn("WriteWorldSnapshot(",review)
  self.assertNotIn("Depth.Choose(",review)
 def test_recipient_behavior_advances_to_comparison_desk_only_with_local_evidence(self):
  world=read("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  f=world.split("bool AUnmadePrototypeHub::GetPublicAccordRoutineTarget(",1)[1].split("FName AUnmadePrototypeHub::GetPublicAccordWorkCue(",1)[0]
  self.assertIn("Depth.Arc(a)->evidence",f)
  self.assertIn("FVector(550,800,0)",f)
  self.assertIn("CrossRealmCarry.Stage((a+8)%9)",f)
  receipt=world.split("FString AUnmadePrototypeHub::GetCrossRealmCarryRecipientLine(",1)[1].split("FString AUnmadePrototypeHub::GetDepthReaction(",1)[0]
  self.assertIn("I compared",receipt)
  self.assertIn("Depth.Arc(a)->evidence",receipt)
if __name__=="__main__":unittest.main()
