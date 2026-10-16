"""Native adapter contracts supplement portable transaction tests."""
from pathlib import Path
import unittest
import re
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text()
class CompleteCaptureWiring(unittest.TestCase):
 def test_every_save_preflights_then_captures_world_before_commit(self):
  s=read('Source/TheUnmadeGame/Private/Save/UnmadeSaveSession.cpp')
  writer=s.split('bool SaveGameToSlot(',1)[1].split('bool SelectProfile(',1)[0]
  self.assertLess(writer.index('PrepareCombinedCapture('),writer.index('CommitCombinedRecord('))
  self.assertIn('!IsValid(Owner)',writer)
  self.assertIn('h->CaptureWorldSnapshot(s)',writer)
  self.assertIn('SessionPlayer->CaptureSession(s)',writer)
 def test_world_capture_never_writes_or_loads_a_second_record(self):
  s=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')
  body=s.split('bool AUnmadePrototypeHub::CaptureWorldSnapshot(',1)[1].split('\nbool ',1)[0]
  for field in ('LivingWorldSeconds','FieldSurveyStages','NpcSnapshots','RealmAftermathStages','EchoQuestStages','RealmGuardianOutcomes','MaterialDays','ResidentEncounterVisits'):
   self.assertIn('Save->'+field,body)
  self.assertNotIn('LoadOrCreate()',body)
  self.assertNotIn('SaveGameToSlot(',body)
  self.assertNotIn('PlayerStrain=',body)
 def test_survey_stages_live_state_and_only_opens_after_commit(self):
  s=read('Source/TheUnmadeGame/Private/World/UnmadePlayableEncounters.cpp')
  action=s.split('bool AUnmadePrototypeHub::TryFieldSurveyInteraction(',1)[1].split('\nvoid ',1)[0]
  self.assertIn('CommitFieldSurveyStage(FieldSurveyStages,i',action)
  self.assertIn('return WriteWorldSnapshot()',action)
  self.assertLess(action.index('CommitFieldSurveyStage('),action.index('SetActorEnableCollision(false)'))
  self.assertNotIn('Save->FieldSurveyStages[i]=',action)

 def test_portable_world_field_fixture_tracks_native_ownership(self):
  hub=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')
  body=hub.split('bool AUnmadePrototypeHub::CaptureWorldSnapshot(',1)[1].split('\nbool ',1)[0]
  native=set(re.findall(r'Save->(\w+)',body))
  fixture=read('Tests/integration/world_capture_fixture.h')
  self.assertEqual(native,set(re.findall(r'out\.(\w+)=world\.',fixture)))
  for field in ('ItemQuantities','LexiconEvidence','RiteStages','PlayerStrain','CharacterIdentityChoices','LocalConflictChoice'):
   self.assertNotIn(field,native)
