"""Static native adapter contracts; executable rules/storage tests cover behavior."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
def read(path):return (ROOT/path).read_text()
class ServiceMasteryWiring(unittest.TestCase):
 def test_personal_accord_stages_lesson_and_witness_in_original_write(self):
  s=read('Private/World/UnmadeAdventureDepth.cpp').split('bool AUnmadePrototypeHub::TryDepthInteraction(',1)[1].split('FString AUnmadePrototypeHub::GetDepthJournal',1)[0]
  self.assertIn('DepthConfirmation.Confirm(arc,action,',s)
  self.assertIn('StagePersonalAccord(Depth,Experience,arc,action,true)',s)
  self.assertIn('CommitLocalEvent(',s)
  self.assertIn('Experience=beforeExperience',s)
  self.assertNotIn('LearnExperienceTutorial(',s)
  self.assertNotIn('ReportIntegrationEvent(',s)
 def test_rest_is_scoped_to_selected_physical_bed(self):
  s=read('Private/World/UnmadePlayerExperience.cpp').split('bool AUnmadePrototypeHub::ExperienceAction(',1)[1].split('FString AUnmadePrototypeHub::GetTrackedObjective',1)[0]
  self.assertIn('selectedSite=site',s)
  self.assertIn('ExperienceRestConfirmation.Confirm(a*5+selectedSite,11,',s)
  self.assertLess(s.index('ExperienceRestConfirmation.Confirm('),s.index('Clock.Advance(1)'))
  self.assertIn('Clock=oldClock',s)
  self.assertIn('Preview and confirm rest',s)
 def test_services_do_not_write_followup_tracking_or_witness_receipts(self):
  s=read('Private/World/UnmadePlayerExperience.cpp').split('bool AUnmadePrototypeHub::ExperienceAction(',1)[1].split('FString AUnmadePrototypeHub::GetTrackedObjective',1)[0]
  self.assertIn('StageGuideService(Experience,a,GetGameDay(),Expeditions)',s)
  self.assertIn('CommitLocalEvent(',s)
  self.assertIn('action==7||action==8||(action==9&&a==3)',s)
  self.assertNotIn('TrackExperienceRealm(',s)
  self.assertNotIn('ReportIntegrationEvent(',s)
 def test_commit_defers_discovery_refresh_until_after_transaction(self):
  s=read('Private/World/UnmadePlayerExperience.cpp').split('bool AUnmadePrototypeHub::CommitExperience(',1)[1].split('\n',1)[0]
  self.assertIn('NextExperienceRefresh=0',s)
  self.assertNotIn('RefreshPlayerExperience()',s)
