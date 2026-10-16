"""Adapter checks; Unreal startup ordering still requires an engine host."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
class StartupRewardWiring(unittest.TestCase):
 def test_component_startup_is_read_only(self):
  s=(ROOT/'Private/Items/UnmadeEquipmentComponent.cpp').read_text()
  begin=s.split('void UUnmadeEquipmentComponent::BeginPlay()',1)[1].split('\nbool ',1)[0]
  self.assertNotIn('Persist(',begin)
  self.assertNotIn('Inventory.Claim(',begin)
  reconcile=s.split('bool UUnmadeEquipmentComponent::ReconcileEarnedMilestones()',1)[1].split('\nbool ',1)[0]
  self.assertLess(reconcile.index('IsSessionWritable('),reconcile.index('CommitStarterKit('))
  self.assertIn('!Inventory.HasClaimed(reward)&&!Claim(reward)',reconcile)
  self.assertIn('return ReconcileCivicContracts(Save)&&complete;',reconcile)
 def test_writer_checks_registered_restored_world_before_rotation(self):
  s=(ROOT/'Private/Save/UnmadeSaveSession.cpp').read_text()
  ready=s.split('bool IsSessionWritable(',1)[1].split('bool SaveGameToSlot(',1)[0]
  for check in ('!SessionPlayer.IsValid()', 'Player!=p', '!p->HasActorBegunPlay()', '!p->IsSessionRestored()', '!h->IsWorldReady()', 'h->HasRejectedWorld()', 'return found;'):
   self.assertIn(check,ready)
  writer=s.split('bool SaveGameToSlot(',1)[1].split('bool SelectProfile(',1)[0]
  self.assertLess(writer.index('!IsSessionWritable()'),writer.index('CaptureSession(s)'))
  self.assertLess(writer.index('!IsSessionWritable()'),writer.index('CommitCombinedRecord'))
 def test_failed_startup_retries_and_checkpoint_reports_pending(self):
  s=(ROOT/'Private/Player/UnmadeCharacter.cpp').read_text()
  begin=s.split('void AUnmadeCharacter::BeginPlay()',1)[1].split('void AUnmadeCharacter::Tick(',1)[0]
  self.assertNotIn('ReconcileEarnedRewards();',begin)
  tick=s.split('void AUnmadeCharacter::Tick(',1)[1]
  self.assertLess(tick.index('RestoreSessionWhenReady();'),tick.index('InitializeStartupRewards();'))
  self.assertIn('NextStartupRewardsAttempt=GetWorld()->GetTimeSeconds()+5',tick)
  save=s.split('bool AUnmadeCharacter::SaveSessionNow()',1)[1]
  self.assertLess(save.index('!InitializeStartupRewards()'),save.index('SavePlayerSession(this)'))
  self.assertIn('STARTUP REWARDS PENDING',s)
