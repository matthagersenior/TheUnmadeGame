from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
def read(p):return (ROOT/p).read_text()
def body(s,sig):return s.split(sig,1)[1].split('\n}',1)[0]
class RealityTravelWiring(unittest.TestCase):
 def test_reality_effects_follow_one_checked_commit(self):
  s=read('Private/Player/UnmadeCharacter.cpp')
  for name in ('DemonstrateAnomaly','FoldReality','RewriteChoice'):
   b=body(s,'void AUnmadeCharacter::'+name+'(')
   self.assertIn('CommitFractureAction(',b)
   self.assertIn('CommitLocalEvent(',b)
   self.assertNotIn('ReportLocalEvent(',b)
   self.assertLess(b.index('CommitFractureAction('),b.index('ApplyFractureVisuals()'))
  self.assertIn('CommitEvidence(',body(s,'void AUnmadeCharacter::DemonstrateAnomaly('))
 def test_travel_moves_before_its_original_writer(self):
  hub=read('Private/World/UnmadePrototypeHub.cpp');atlas=read('Private/World/UnmadeLaterRealmWorld.cpp')
  for source,name in ((hub,'TryTravelFrontier'),(atlas,'TryTravelAtlas')):
   b=body(source,'bool AUnmadePrototypeHub::'+name+'(')
   self.assertIn('CommitCheckpointMove(',b)
   self.assertNotIn('SetActorLocation(',b)
   self.assertLess(b.index('CanCommitCheckpoint()'),b.index('.Visit('))
  p=read('Private/Player/UnmadeCharacter.cpp')
  self.assertNotIn('VerifyExploredFrontier(',body(p,'void AUnmadeCharacter::CrossFrontierGateway('))
 def test_death_recovery_is_saved_and_rollback_is_protected(self):
  p=read('Private/Player/UnmadeCharacter.cpp')
  b=body(p,'void AUnmadeCharacter::RecoverAtSafeCheckpoint()')
  self.assertIn('DeathRecoveryRetry.Attempt(',b)
  self.assertIn('CommitCheckpointMove(',b)
  self.assertIn('RestoreSession(',b)
  header=read('Public/Player/UnmadeCharacter.h')
  self.assertIn('HasRejectedSave()const{return bCheckpointRollbackPending',header)
  move=body(p,'bool AUnmadeCharacter::CommitCheckpointMove(')
  self.assertIn('bCheckpointRollbackPending=true',move)
  guard=body(p,'bool AUnmadeCharacter::CanCommitCheckpoint()')
  self.assertIn('!InitializeStartupRewards()',guard)
  self.assertIn('HasPendingDefeatReceipts()',guard)
  self.assertIn('!Recovery&&!CanCommitCheckpoint()',move)
  self.assertIn('SaveRecoveryState(this)',b)
  self.assertIn('},true)',b)
  self.assertIn('PrepareRealmGeometry(At,100000)',move)
 def test_every_writer_captures_lexicon_and_saved_arrival_is_retained(self):
  s=read('Private/Save/UnmadeSaveSession.cpp')
  self.assertIn('lex->CaptureLexicon(s)',s)
  p=read('Private/Player/UnmadeCharacter.cpp')
  capture=body(p,'void AUnmadeCharacter::CaptureSession(')
  self.assertIn('bCaptureCheckpointPosition',capture)
  self.assertIn('ResolveSafeCheckpoint(',capture)
  restore=body(p,'void AUnmadeCharacter::RestoreSessionWhenReady()')
  self.assertIn('Save->ResumePosition',restore)
  self.assertNotIn('ExperienceCheckpoint(',restore)
