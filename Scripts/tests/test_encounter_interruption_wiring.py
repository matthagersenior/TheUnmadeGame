"""Static actor contracts supplement portable warnings, damage and receipt tests."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame/Private/Combat'

class EncounterInterruptionWiring(unittest.TestCase):
 def test_named_receipt_owner_runs_before_player_guard(self):
  source=(ROOT/'UnmadeBossCharacter.cpp').read_text()
  tick=source.split('void AUnmadeBossCharacter::Tick(',1)[1].split('bool AUnmadeBossCharacter::ResolveByMercy',1)[0]
  self.assertLess(tick.index('CommitDefeatReceipt();'),tick.index('RequireEncounterPlayer('))
  for token in ['IsSessionWritable(Player)','Player->GetWorld()==GetWorld()',
                'InterruptEncounter(Encounter);bTelegraphShown=false','StopMovementImmediately()']:
   self.assertIn(token,tick)
  self.assertNotIn('DefeatSave.Reset()',tick)
  self.assertNotIn('Configure(',tick)

 def test_final_and_guardian_receipts_precede_readiness(self):
  for name,pattern in [('UnmadeFinalBoss.cpp','Rhythm'),('UnmadeRealmGuardian.cpp','Encounter')]:
   source=(ROOT/name).read_text()
   tick=source.split('::Tick(',1)[1].split('bool AUnmade',1)[0]
   self.assertLess(tick.index('DefeatSave.Attempt('),tick.index('RequireEncounterPlayer('))
   self.assertIn('IsSessionWritable(Player)',tick)
   self.assertIn('Player->GetWorld()==World',tick)
   self.assertIn(f'InterruptEncounter({pattern});bWarningShown=false',tick)
   self.assertIn('if(!IsValid(Hub)){Interrupt();return;}',tick)
   self.assertNotIn('DefeatSave.Reset()',tick)

 def test_generic_elder_retry_and_warning_cleanup(self):
  source=(ROOT/'UnmadeEnemyCharacter.cpp').read_text()
  tick=source.split('void AUnmadeEnemyCharacter::Tick(float',1)[1].split('void AUnmadeEnemyCharacter::ConfigureExpedition',1)[0]
  self.assertLess(tick.index('RecordCinderholdElderDefeat(this)'),tick.index('RequireEncounterPlayer('))
  for token in ['IsSessionWritable(Player)','InterruptAttackWindup(WindupEndsAt,NextWindupAllowedAt,RecoveryEndsAt)',
                'InterruptEncounter(ElderPattern)','NormalLockedAim=ElderLockedAim=FVector::ZeroVector',
                'ElderLastWarning=UnmadeCore::SlagAttack::None','ElderAimMarker->SetVisibility(false)',
                'TelegraphLight->SetIntensity(0)','CombatSignal->SetText(FText::GetEmpty())']:
   self.assertIn(token,tick)

 def test_expedition_and_observer_keep_receipts_then_cancel_aim(self):
  source=(ROOT/'UnmadeEnemyCharacter.cpp').read_text()
  expedition=source.split('void AUnmadeEnemyCharacter::TickExpedition(',1)[1].split('FString AUnmadeEnemyCharacter::GetCombatName',1)[0]
  observer=source.split('void AUnmadeEnemyCharacter::TickWingObserver(',1)[1].split('bool AUnmadeEnemyCharacter::HasPendingDefeatSave',1)[0]
  self.assertLess(expedition.index('RecordExpeditionDefeat(this,'),expedition.index('RequireEncounterPlayer('))
  self.assertLess(observer.index('RecordWingObserver(this)'),observer.index('RequireEncounterPlayer('))
  self.assertIn('IsSessionWritable(P)',expedition)
  self.assertIn('IsSessionWritable(p)',observer)
  for token in ['InterruptEncounter(ExpeditionPattern)','bExpeditionWarning=false',
                'ExpeditionAim=FVector::ZeroVector','Combat->SetGuarding(false)',
                'tile->SetVisibility(false)']:
   self.assertIn(token,expedition)
  self.assertIn('NormalLockedAim=FVector::ZeroVector',observer)
  self.assertIn('InterruptAttackWindup(WindupEndsAt,NextWindupAllowedAt,RecoveryEndsAt)',observer)
  for body in [expedition,observer]:
   self.assertIn('CombatSignal->SetText(FText::GetEmpty())',body)
   self.assertNotIn('NextElderSaveAttemptAt=0',body)

 def test_final_damage_uses_authored_beat_without_phase_bonus(self):
  source=(ROOT/'UnmadeFinalBoss.cpp').read_text()
  self.assertIn('ConfigureAuthoredFinalStrike(*GetCombat(),Beat)',source)
  self.assertNotIn('Beat.damage-17',source)
  self.assertNotIn('SetGearBonuses(',source)
  self.assertLess(source.index('ConfigureAuthoredFinalStrike(*GetCombat(),Beat)'),source.index('TryStrikeTarget(Player->GetCombat(),Now'))
