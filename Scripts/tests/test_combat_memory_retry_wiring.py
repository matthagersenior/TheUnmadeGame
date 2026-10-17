from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
def read(p):return (ROOT/p).read_text()
class CombatMemoryRetryWiring(unittest.TestCase):
 def test_both_hit_sources_queue_existing_rewards(self):
  header=read('Public/Player/UnmadeCharacter.h')
  self.assertIn('NotifyProjectileImpact(class AUnmadeEnemyCharacter* Target,float Before){HandleMeleeImpact(Target,Before);}',header)
  source=read('Private/Player/UnmadeCharacter.cpp')
  impact=source.split('void AUnmadeCharacter::HandleMeleeImpact(',1)[1].split('void AUnmadeCharacter::StartGuard()',1)[0]
  self.assertIn('CombatRewards.Queue(Target->GetEnemyStyle())',impact)
  self.assertIn('RetryCombatRewards()',impact)
  tick=source.split('void AUnmadeCharacter::Tick(',1)[1]
  self.assertIn('RetryCombatRewards();',tick)
  hub=read('Private/World/UnmadePrototypeHub.cpp')
  pending=hub.split('bool AUnmadePrototypeHub::HasPendingDefeatReceipts()',1)[1].split('bool AUnmadePrototypeHub::SavePlayerSession(',1)[0]
  self.assertIn('HasPendingCombatRewards()',pending)
 def test_direct_memories_share_quiet_timer_retry(self):
  player=read('Private/Player/UnmadeCharacter.cpp')
  report=player.split('void AUnmadeCharacter::ReportLocalEvent(',1)[1].split('void AUnmadeCharacter::SaveNearbyNpcMemories()',1)[0]
  self.assertLess(report.index('IsWorldWritable(World)'),report.index('FGuid::NewGuid()'))
  self.assertIn('It->GetStableId().IsNone()',report)
  self.assertIn('if(Witnesses>0)SaveNearbyNpcMemories()',report)
  save=player.split('void AUnmadeCharacter::SaveNearbyNpcMemories()',1)[1].split('bool AUnmadeCharacter::IsRpgMenuOpen()',1)[0]
  self.assertIn('Hub->SaveNpcMemories()',save)
  self.assertNotIn('ReplaceNpcMemory(',save)
  hub=read('Private/World/UnmadePrototypeHub.cpp')
  direct=hub.split('bool AUnmadePrototypeHub::SaveNpcMemories()',1)[1].split('void AUnmadePrototypeHub::SpreadLocalRumors()',1)[0]
  self.assertIn('bNpcMemorySavePending=true',direct)
  self.assertIn('CommitPendingMemory(',direct)
  gossip=hub.split('void AUnmadePrototypeHub::SpreadLocalRumors()',1)[1].split('void AUnmadePrototypeHub::PrepareRealmGeometry(',1)[0]
  self.assertIn('CommitPendingMemory(',gossip)
  self.assertNotIn('ReportLocalEvent(',gossip)
