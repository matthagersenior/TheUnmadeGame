"""Native adapters only; executable history tests cover production rules."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
def read(p):return (ROOT/p).read_text()
class ChronicleHistoryWiring(unittest.TestCase):
 def test_threats_never_use_redemption_control(self):
  player=read('Private/Player/UnmadeCharacter.cpp')
  event=player.split('void AUnmadeCharacter::ReportLocalEvent(',1)[1].split('void AUnmadeCharacter::SaveNearbyNpcMemories()',1)[0]
  self.assertIn('Tenfold->RecordThreatenedOath()',event)
  self.assertNotIn('Tenfold->BreakChosenOath()',event)
  self.assertLess(event.index('RecordThreatenedOath()'),event.index('SaveNearbyNpcMemories()'))
 def test_all_writers_capture_live_history_without_recursive_save(self):
  writer=read('Private/Save/UnmadeSaveSession.cpp').split('bool SaveGameToSlot(',1)[1].split('bool SelectProfile(',1)[0]
  self.assertIn('rite->CaptureChronicle(s)',writer)
  component=read('Private/World/UnmadeTenfoldComponent.cpp')
  capture=component.split('bool UUnmadeTenfoldComponent::CaptureChronicle(',1)[1].split('bool UUnmadeTenfoldComponent::Persist(',1)[0]
  self.assertIn('CaptureChronicleRecord(',capture)
  self.assertNotIn('LoadOrCreate',capture);self.assertNotIn('SaveGameToSlot',capture)
  tick=component.split('void UUnmadeTenfoldComponent::TickComponent(',1)[1]
  self.assertIn('RetryHistorySave()',tick)
  self.assertIn('HasPendingHistory()',read('Private/World/UnmadePrototypeHub.cpp'))
 def test_manual_oath_failure_explains_rollback_and_removes_only_oath_power(self):
  component=read('Private/World/UnmadeTenfoldComponent.cpp')
  action=component.split('void UUnmadeTenfoldComponent::BreakChosenOath()',1)[1].split('void UUnmadeTenfoldComponent::RecordLegacyDeed(',1)[0]
  self.assertIn('OATH CHANGE NOT SAVED',action)
  self.assertIn('RemoveOathPowers()',action)
  remove=component.split('void UUnmadeTenfoldComponent::RemoveOathPowers()',1)[1].split('void UUnmadeTenfoldComponent::RecordThreatenedOath()',1)[0]
  self.assertIn('ClearBrokenOathPower(',remove)
  self.assertIn('SetTemporaryBonuses(LastAppliedAttack,LastAppliedArmor)',remove)
