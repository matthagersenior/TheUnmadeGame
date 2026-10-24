"""Native capture/receipt seams supplement portable storage and combat checks."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
class EncounterRecoveryWiring(unittest.TestCase):
 def test_in_session_recovery_uses_complete_models(self):
  header=(ROOT/'Public/Combat/UnmadeCombatComponent.h').read_text()
  self.assertIn('CaptureRecoveryState()',header)
  self.assertIn('RestoreRecoveryState(',header)
  self.assertIn('StatusSource=Before.StatusSource',header)
  self.assertIn('StatusUntil=Before.Combat.statusUntil',header)
  source=(ROOT/'Private/Player/UnmadeCharacter.cpp').read_text()
  recovery=source.split('void AUnmadeCharacter::RecoverAtSafeCheckpoint()',1)[1].split('void AUnmadeCharacter::PlayIdentityMotion',1)[0]
  self.assertIn('CommitCombatRecovery(*Combat',recovery)
  self.assertNotIn('Combat->RestoreSession(',recovery)
  actual_load=source.split('void AUnmadeCharacter::RestoreSessionWhenReady()',1)[1].split('void AUnmadeCharacter::CaptureSession',1)[0]
  self.assertIn('Combat->RestoreSession(',actual_load)
 def test_counter_threat_pending_query_includes_actual_defeated_actor(self):
  source=(ROOT/'Private/World/UnmadePrototypeHub.cpp').read_text()
  pending=source.split('bool AUnmadePrototypeHub::HasPendingDefeatReceipts()const',1)[1].split('bool AUnmadePrototypeHub::SaveRecoveryState',1)[0]
  self.assertIn('DepthRetaliators',pending)
  self.assertIn('PendingCounterThreat(Depth,a,',pending)
  self.assertIn('Threat->GetCombat()->IsDefeated()',pending)
  self.assertNotIn('IsHidden',pending)
  native=(ROOT/'Private/World/UnmadeAdventureDepth.cpp').read_text()
  self.assertIn('CommitCounterThreatReceipt(Depth,a,true,',native)
 def test_delayed_status_reward_has_no_fabricated_fight_witness(self):
  source=(ROOT/'Private/Player/UnmadeCharacter.cpp').read_text()
  delayed=source.split('void AUnmadeCharacter::NotifyStatusDefeat(',1)[1].split('void AUnmadeCharacter::StartGuard()',1)[0]
  self.assertIn('Target->GetWorld()!=GetWorld()',delayed)
  self.assertIn('CombatRewards.Queue(Target->GetEnemyStyle())',delayed)
  self.assertIn('RetryCombatRewards()',delayed)
  self.assertIn('CommitDefeatReceipt()',delayed)
  self.assertNotIn('ReportLocalEvent(',delayed)
 def test_fieldkit_never_spends_in_an_unaccepted_world(self):
  source=(ROOT/'Private/World/UnmadeAdventureDepth.cpp').read_text()
  kit=source.split('UnmadeCore::FieldKitResult AUnmadePrototypeHub::UseDepthFieldKit(',1)[1]
  self.assertIn('p->GetWorld()!=GetWorld()',kit)
  self.assertLess(kit.index('IsSessionWritable(p)'),kit.index('CommitFieldKit('))
