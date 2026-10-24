"""Static actor contracts; portable production rules are exercised separately."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
def read(p):return (ROOT/p).read_text()
class RiteCommitWiring(unittest.TestCase):
 def test_cost_and_world_consequences_share_one_writer(self):
  s=read('Private/World/UnmadeTenfoldComponent.cpp')
  invoke=s.split('void UUnmadeTenfoldComponent::InvokeRite()',1)[1].split('\nvoid ',1)[0]
  self.assertIn('Hub->CommitRiteCast(',invoke)
  self.assertLess(invoke.index('IsSessionWritable(Player)'),invoke.index('Chronicle.Invoke('))
  self.assertIn('return Persist(true);',invoke)
  self.assertNotIn('RecordNearbyRealmRite(',invoke)
  self.assertLess(invoke.index('Hub->CommitRiteCast('),invoke.index('Player->ApplyRiteAbility('))
  self.assertIn('Chronicle.Restore(Previous)',invoke)
  self.assertIn('Confluence.Restore(PriorConfluence)',invoke)
 def test_world_stages_both_ledgers_before_the_single_callback(self):
  hub=read('Private/World/UnmadePrototypeHub.cpp')
  commit=hub.split('bool AUnmadePrototypeHub::CommitRiteCast(',1)[1].split('\nvoid ',1)[0]
  self.assertIn('CommitRiteWorld(',commit)
  self.assertIn('StageNearbyRealmRite(',commit)
  self.assertIn('SaveCast',commit)
  self.assertNotIn('WriteWorldSnapshot()',commit)
  resonance=read('Private/World/UnmadeRealmResonanceWorld.cpp')
  stage=resonance.split('bool AUnmadePrototypeHub::StageNearbyRealmRite(',1)[1].split('\nvoid ',1)[0]
  self.assertNotIn('WriteWorldSnapshot()',stage)
  self.assertNotIn('RefreshRealmResonanceWorld()',stage)
 def test_boss_receipt_survives_failed_reward_and_mercy(self):
  boss=read('Private/Combat/UnmadeBossCharacter.cpp')
  self.assertIn('CommitDefeatReceipt()',boss)
  self.assertIn('DefeatSave.Attempt(',boss)
  self.assertIn('HasPendingDefeatSave(',boss)
  self.assertIn('ResolveByMercy()',boss)
  player=read('Private/Player/UnmadeCharacter.cpp')
  mercy=player.split('case UnmadeCore::RiteId::UnderstandingBosses:',1)[1].split('case UnmadeCore::RiteId::BorrowedLives:',1)[0]
  self.assertIn('ResolveByMercy()',mercy)
  self.assertNotIn('SetActorTickEnabled(false)',mercy)
  self.assertNotIn('Equipment->Claim(',mercy)

 def test_mercy_and_receipts_use_physical_and_durable_gates(self):
  player=read('Private/Player/UnmadeCharacter.cpp')
  mercy=player.split('case UnmadeCore::RiteId::UnderstandingBosses:',1)[1].split('case UnmadeCore::RiteId::BorrowedLives:',1)[0]
  self.assertIn('!It->CanBeTargeted()',mercy)
  self.assertIn('LineTraceTestByChannel(',mercy)
  boss=read('Private/Combat/UnmadeBossCharacter.cpp')
  receipt=boss.split('void AUnmadeBossCharacter::CommitDefeatReceipt()',1)[1].split('bool AUnmadeBossCharacter::HasPendingDefeatSave(',1)[0]
  self.assertLess(receipt.index('DefeatSaveResult::Saved'),receipt.index('SetActorTickEnabled(false)'))
  self.assertIn('!UnmadeSave::IsWorldWritable(GetWorld())',receipt)
  world=read('Private/World/UnmadeRealmResonanceWorld.cpp')
  publish=world.split('void AUnmadePrototypeHub::PublishRealmResonance(',1)[1].split('bool AUnmadePrototypeHub::InspectResonanceArchive(',1)[0]
  for notice in ('OBSERVATORY OPEN:', 'Cast the OTHER mastered rite', 'The observatory remains permanently open'):
   self.assertIn(notice,publish)
