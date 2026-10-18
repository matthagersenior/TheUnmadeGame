"""The physically reachable receiving-realm review must not be nested under collection."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]
class RealCrossRealmReviewReachability(unittest.TestCase):
 def test_review_and_collection_are_sibling_actions_not_unreachable_nested_handlers(self):
  src=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp").read_text(encoding="utf-8")
  pickup="if(kind==7&&action==0){"
  review="if(kind==8&&action==0){"
  self.assertEqual(src.count(pickup),1)
  self.assertEqual(src.count(review),1)
  prefix=src.split(pickup,1)[1].split(review,1)[0]
  # Count braces *between* the first handler's open and the second handler.
  # Balanced prefix means the first handler closed before the second begins.
  self.assertEqual(prefix.count("}")-prefix.count("{"),1,
     "The kind-8 review must not be physically unreachable inside kind-7.")
 def test_review_is_a_real_separate_in_world_candidate_with_save_commit(self):
  s=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp").read_text(encoding="utf-8")
  self.assertIn("Add(a,8,0,DepthCrossRealmReviewActors[a]->GetActorLocation()",s)
  self.assertIn("if(kind==8&&action==0){",s)
  self.assertIn("CrossRealmCarry.Review(source,arc,",s)
  self.assertIn("Person(GetWorld(),arc,0)",s)
  self.assertIn("CommitCrossRealmCarry(",s)
  self.assertIn("if(kind==8){",s)
  self.assertIn("ReviewState::ReadyToCompare",s)
  self.assertIn("WriteWorldSnapshot()",s)
if __name__=="__main__":unittest.main()
