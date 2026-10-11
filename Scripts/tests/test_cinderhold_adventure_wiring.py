"""Runtime connection guards, distinct from portable behavioral tests.
These cannot establish UHT, navigation, camera or Unreal playtest success.
"""
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def read(path):return (ROOT/path).read_text()
class CinderholdAdventureWiring(unittest.TestCase):
    def test_locality_save_failure_and_no_fabricated_resources(self):
        s=read('Source/TheUnmadeGame/Private/World/UnmadeCinderholdAdventure.cpp')
        for term in ('LineTraceTestByChannel','CompetingNpcDistanceSq','260.f',
                     'if(WriteWorldSnapshot(StrainOwner))','Cinderhold=Before;',
                     'Player->RefundRealityStrain(Cost)','Now>CinderholdConfirmUntil',
                     'CinderholdPendingRoute!=Choice','CinderholdConfirmUntil=Now+6',
                     'House.heatBaskets,House.daysCold,Worker->owedMarks'):
            self.assertIn(term,s)
        for forbidden in ('Equipment->Claim','LivingTrades.Restore','Materials.Restore',
                          'SaveFractureState','ResolveFinal','employerMarks='):
            self.assertNotIn(forbidden,s)
        # One world slot persists route AND paid strain; no second-save gap.
        hub=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')
        save=hub[hub.index('bool AUnmadePrototypeHub::WriteWorldSnapshot('):hub.index('bool AUnmadePrototypeHub::TryResidentVillageTask(')]
        self.assertIn('Save->PlayerStrain=Reality.strain;',save)
        self.assertIn('Save->CinderholdAdventureState=',save)
        self.assertEqual(save.count('SaveGameToSlot'),1)
        self.assertIn('S.Num()!=10||!Cinderhold.Restore',hub)
        self.assertIn('if(bCinderholdSaveRejected)return false;',save)
    def test_elder_counterplay_and_resolved_target_filter(self):
        s=read('Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp')
        for term in ('ElderLockedAim=Player->GetActorLocation()',
                     'ElderPattern.Step','ElderPattern.Interrupt',
                     'RecordCinderholdElderDefeat','GetCinderholdElderResolution',
                     'NextElderSaveAttemptAt=Now+3','ELDER QUIETED - ALIVE',
                     'bOutsideTerritory','ElderAimMarker->SetWorldLocation',
                     'FVector::DistSquared2D(Player->GetActorLocation(),ElderLockedAim)',
                     'ELDER RECOVERY - OPENING'):
            self.assertIn(term,s)
        player=read('Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp')
        self.assertIn('if (!It->CanBeTargeted()) continue;',player)
        for action in ('TryCinderholdAbility(this,0)','TryCinderholdAbility(this,1)',
                       'TryCinderholdAbility(this,2,','TryCinderholdInteraction(this,ResidentDistSq)',
                       'RecordCinderholdConversation(Target->GetStableId())','GetCinderholdJournal()'):
            self.assertIn(action,player)
    def test_presentation_and_old_optional_survey_coexist(self):
        s=read('Source/TheUnmadeGame/Private/World/UnmadeUnpaidShiftPresentation.cpp')
        for term in ('CinderholdBeatAt','HOME_RETURN_DYNAMIC','Return.returnedHeat',
                     'RestoreBesCamera','ClearBeat','FIELD_REPORT_DYNAMIC'):
            self.assertIn(term,s)
        survey=read('Source/TheUnmadeGame/Private/World/UnmadePlayableEncounters.cpp')
        self.assertIn('if(i==5&&FieldSurveyStages[i]==2)continue;',survey)
        self.assertIn('ConfigureCinderholdElder()',survey)
        self.assertIn('Tests/world/cinderhold_adventure_test.cpp',read('.github/workflows/static-checks.yml'))
        self.assertIn('test_cinderhold_adventure_wiring.py',read('Scripts/first_pc_build_and_test.ps1'))
