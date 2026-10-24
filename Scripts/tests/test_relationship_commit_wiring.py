"""Native contracts supplement portable conversation fault/storage scenarios."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
class RelationshipCommitWiring(unittest.TestCase):
 def test_conversation_has_one_scoped_writer_and_no_scene_fallback(self):
  source=(ROOT/'Private/World/UnmadePlayerExperience.cpp').read_text()
  body=source.split('bool AUnmadePrototypeHub::RecordExperienceConversation(',1)[1].split('FString AUnmadePrototypeHub::ExperienceReaction',1)[0]
  self.assertIn('CommitResidentConversation(Experience,a,n,',body)
  self.assertIn('IsSessionWritable(p)',body)
  self.assertNotIn('for(int scene=',body)
  self.assertNotIn('ExperienceScene(',source)
  self.assertEqual(body.count('CommitExperience(before,p)'),1)
  self.assertLess(body.index('ConversationState::SaveFailed'),body.index('BeginDirectedBlocking'))
  self.assertIn('if(result.scene>=0)',body)
 def test_original_guide_refusal_is_checked_before_commit(self):
  body=(ROOT/'Private/World/UnmadePlayerExperience.cpp').read_text().split('bool AUnmadePrototypeHub::RecordExperienceConversation(',1)[1].split('FString AUnmadePrototypeHub::ExperienceReaction',1)[0]
  self.assertIn('if(a==0&&n==4)',body)
  for action in ['AvoidPlayer','RefuseTrade']:self.assertIn(action,body)
  self.assertLess(body.index('DecideForPlayer(true)'),body.index('CommitResidentConversation('))
  self.assertIn('if(!acceptsScene)',body)
 def test_failed_handled_conversation_does_not_start_tutorial_writer(self):
  body=(ROOT/'Private/Player/UnmadeCharacter.cpp').read_text()
  self.assertIn('if(!Hub->RecordExperienceConversation(Target->GetStableId(),this))\n                Hub->LearnExperienceTutorial(0,this);',body)
