"""Source-level actor wiring checks for real creature routines and spaced group windups."""
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]

class LivingEncounterEcology(unittest.TestCase):
    def test_idle_creatures_can_actually_navigate_routines(self):
        s=(ROOT/'Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp').read_text()
        tick=s.split('void AUnmadeEnemyCharacter::Tick(float DeltaSeconds)',1)[1].split('void AUnmadeEnemyCharacter::ConfigureExpedition',1)[0]
        self.assertIn('UnmadeCore::WantsCreatureNavigation(bNearby,bReturnHome,bLocalRoutine)',tick)
        self.assertIn('bLocalRoutine?RoutineTarget',tick)
        self.assertIn('UNavigationSystemV1::FindPathToLocationSynchronously',tick)
        self.assertIn('bLocalPathClear',tick)
        self.assertLess(tick.index('UnmadeCore::CreatureRoutine('),tick.index('WantsCreatureNavigation('))

    def test_group_pressure_has_spatial_scope_and_does_not_override_bosses(self):
        s=(ROOT/'Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp').read_text()
        tick=s.split('void AUnmadeEnemyCharacter::Tick(float DeltaSeconds)',1)[1].split('void AUnmadeEnemyCharacter::ConfigureExpedition',1)[0]
        self.assertIn('TActorIterator<AUnmadeEnemyCharacter>',tick)
        self.assertIn('UnmadeCore::MayBeginOrdinaryWindup',tick)
        self.assertIn('UnmadeCore::ShouldCircleForOpening',tick)
        self.assertIn('other->ExpeditionArc>=0',tick)
        self.assertIn('other->WindupEndsAt',tick)
        self.assertIn('bCircleForOpening?',tick)
        self.assertIn('NextWindupAllowedAt',tick)
        self.assertIn('UnmadeCore::RequireEncounterPlayer',tick)
        self.assertIn('if(bCinderholdElder)',tick)

if __name__=='__main__':
    unittest.main()
