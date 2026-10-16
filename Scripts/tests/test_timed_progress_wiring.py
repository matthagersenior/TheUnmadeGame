"""Source adapter contracts supplement portable models; no engine is run."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text()
class TimedProgressWiring(unittest.TestCase):
 def test_all_combined_writers_name_their_source_owner(self):
  calls=[]
  for p in (ROOT/'Source/TheUnmadeGame/Private').rglob('*.cpp'):
   for line in p.read_text().splitlines():
    if 'UnmadeSave::SaveGameToSlot(' in line:calls.append(line)
  self.assertTrue(calls)
  for line in calls:self.assertIn('Save,this)',line)
  s=read('Source/TheUnmadeGame/Private/Save/UnmadeSaveSession.cpp')
  writer=s.split('bool SaveGameToSlot(',1)[1].split('bool SelectProfile(',1)[0]
  self.assertLess(writer.index('IsWorldWritable(Owner->GetWorld())'),writer.index('CaptureSession(s)'))
 def test_tick_and_timers_reject_wrong_world_before_changes(self):
  s=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')
  tick=s.split('void AUnmadePrototypeHub::Tick(',1)[1].split('\nvoid ',1)[0]
  self.assertLess(tick.index('IsWorldWritable(GetWorld())'),tick.index('AdvanceWorldClock('))
  self.assertLess(tick.index('CommitSettlementVisit('),tick.index('if (Village != LastVisitedVillage)'))
  for signature in ('bool AUnmadePrototypeHub::WriteWorldSnapshot(', 'void AUnmadePrototypeHub::SpreadLocalRumors()', 'void AUnmadePrototypeHub::SaveLivingWorld()'):
   body=s.split(signature,1)[1].split('\n}',1)[0]
   self.assertIn('IsWorldWritable(',body)
  self.assertIn('CommitMaterialDay(',s)
 def test_rite_day_and_memory_failures_keep_retrying(self):
  s=read('Source/TheUnmadeGame/Private/World/UnmadeTenfoldComponent.cpp')
  tick=s.split('void UUnmadeTenfoldComponent::TickComponent(',1)[1]
  self.assertLess(tick.index('IsWorldWritable(GetWorld())'),tick.index('CommitRiteDay('))
  self.assertLess(tick.index('CommitRiteDay('),tick.index('RefreshTemporaryPowers(Now,Day)'))
  hub=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')
  self.assertIn('if(bNewMemory)bNpcMemorySavePending=true;',hub)
  self.assertIn('if(bNpcMemorySavePending&&WriteWorldSnapshot())bNpcMemorySavePending=false;',hub)
  snapshot=hub.split('bool AUnmadePrototypeHub::CaptureWorldSnapshot(',1)[1].split('\nbool ',1)[0]
  self.assertIn('ReplaceNpcMemory(Save->NpcSnapshots,Snapshot)',snapshot)

 def test_both_memory_capture_paths_preserve_absent_residents(self):
  hub=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')
  player=read('Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp')
  world=hub.split('bool AUnmadePrototypeHub::CaptureWorldSnapshot(',1)[1].split('Save->bHasFieldSurveys',1)[0]
  nearby=player.split('void AUnmadeCharacter::SaveNearbyNpcMemories()',1)[1].split('void AUnmadeCharacter::RestoreSessionWhenReady()',1)[0]
  for capture in (world,nearby):
   self.assertNotIn('NpcSnapshots.Reset()',capture)
   self.assertIn('if(!UnmadeCore::ReplaceNpcMemory(Save->NpcSnapshots,Snapshot))Save->NpcSnapshots.Add(Snapshot);',capture)
