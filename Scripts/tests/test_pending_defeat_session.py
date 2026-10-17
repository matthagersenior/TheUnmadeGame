from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source(path):return (ROOT/path).read_text()
class PendingDefeatSession(unittest.TestCase):
    def test_session_checkpoint_checks_hidden_defeated_actors_before_writing(self):
        world=source('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')
        self.assertIn('AUnmadePrototypeHub::HasPendingDefeatReceipts()const',world)
        query=world.split('AUnmadePrototypeHub::HasPendingDefeatReceipts()const',1)[1].split('bool AUnmadePrototypeHub::SavePlayerSession',1)[0]
        self.assertIn('TActorIterator<AUnmadeEnemyCharacter>',query)
        self.assertIn('HasPendingDefeatSave(*this)',query)
        self.assertNotIn('IsHidden()',query)
        save=world.split('bool AUnmadePrototypeHub::SavePlayerSession',1)[1].split('bool AUnmadePrototypeHub::WriteWorldSnapshot',1)[0]
        self.assertIn('CommitSessionCheckpoint(HasPendingDefeatReceipts()',save)
        self.assertIn('CheckpointSave::PendingDefeat',save)
        self.assertIn('WriteWorldSnapshot(Player)',save)
    def test_final_phase_identity_and_all_receipt_owners_are_checked(self):
        final=source('Source/TheUnmadeGame/Private/Combat/UnmadeFinalBoss.cpp')
        self.assertIn('PendingFinalDefeat(',final)
        self.assertIn('ConfiguredAct,Hub.GetFinalAct()',final)
        enemy=source('Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp')
        for token in ('HasPendingDefeatSave(', 'WingObserverInactive()', 'WingGuardianInactive(ExpeditionArc)', 'ExpeditionEnemyInactive(ExpeditionArc,ExpeditionSlot)', 'GetRoadElderResolution(RoadElderCity)', 'GetCinderholdElderResolution()'):
            self.assertIn(token,enemy)
        guardian=source('Source/TheUnmadeGame/Private/Combat/UnmadeRealmGuardian.cpp')
        self.assertIn('HasPendingDefeatSave(',guardian)
        self.assertIn('Hub.GetRealmGuardianOutcome(HomeRealm)',guardian)
    def test_manual_feedback_and_existing_discard_confirmation_are_retained(self):
        widget=source('Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp')
        save=widget.split('void UUnmadeRpgWidget::SaveNow()',1)[1].split('void UUnmadeRpgWidget::LoadProfile()',1)[0]
        self.assertIn('HasPendingDefeatReceipts()',save)
        self.assertIn('Continue the game',save)
        for name,next_name in [('LoadProfile','LoadBackup'),('NewProfile','FindPlayer')]:
            if name=='NewProfile':handler=widget.split('void UUnmadeRpgWidget::NewProfile()',1)[1].split('\n',1)[0]
            else:handler=widget.split(f'void UUnmadeRpgWidget::{name}()',1)[1].split(f'void UUnmadeRpgWidget::{next_name}()',1)[0]
            self.assertIn('SessionConfirm.Request',handler)
            self.assertIn('p->SaveSessionNow()',handler)
        recovery=widget.split('void UUnmadeRpgWidget::LoadBackup()',1)[1].split('void UUnmadeRpgWidget::NewProfile()',1)[0]
        self.assertNotIn('SaveSessionNow',recovery)
