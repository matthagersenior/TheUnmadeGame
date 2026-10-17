"""Portable stealth detection must be invoked by actual ordinary enemy movement/attack source."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]
SOURCE=ROOT/'Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp'
HEADER=ROOT/'Source/TheUnmadeGame/Public/Combat/UnmadeEnemyCharacter.h'

class CreatureAwarenessIntegration(unittest.TestCase):
 def test_crouch_rear_approach_changes_enemy_awareness(self):
  s=SOURCE.read_text()
  body=s.split('void AUnmadeEnemyCharacter::Tick(float DeltaSeconds)',1)[1].split('void AUnmadeEnemyCharacter::ConfigureExpedition',1)[0]
  self.assertIn('Player->GetCharacterMovement()->IsCrouching()',body)
  self.assertIn('UnmadeCore::CreatureCanSpot(',body)
  self.assertIn('Alertness.Update(',body)
  self.assertIn('bEngaged',body)
  self.assertIn('if(bEngaged)SetActorRotation(',body)
  self.assertIn('UnmadeCore::CreatureRoutine(Style,Now,',body)
  self.assertIn('!bEngaged&&!bReturnHome',body)
  self.assertIn('!bCinderholdElder&&!bElderEncounter',body)
  self.assertLess(body.index('CreatureCanSpot('),body.index('CreatureRoutine('))
 def test_unready_player_clears_transient_awareness_and_bosses_keep_their_rhythm(self):
  s=SOURCE.read_text()
  body=s.split('void AUnmadeEnemyCharacter::Tick(float DeltaSeconds)',1)[1].split('void AUnmadeEnemyCharacter::ConfigureExpedition',1)[0]
  self.assertIn('Alertness.Reset();',body)
  self.assertIn('if(bCinderholdElder)',body)
  self.assertIn('ElderStep=ElderPattern.Step(',body)
  self.assertIn('other->bElderEncounter',body)
  self.assertIn('!bElderEncounter&&bNearby',body)
  h=HEADER.read_text()
  self.assertIn('UnmadeCore::CreatureAwareness Alertness;',h)

if __name__=="__main__":
 unittest.main()
