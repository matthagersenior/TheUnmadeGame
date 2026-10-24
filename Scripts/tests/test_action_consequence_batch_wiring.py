"""Native boundary checks supplement production-rule action/fault scenarios."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
def read(p):return (ROOT/p).read_text()
def body(s,signature):return s.split(signature,1)[1].split('\n}',1)[0]
class ActionConsequenceBatchWiring(unittest.TestCase):
 def test_resolution_family_commits_events_before_refresh(self):
  hub=read('Private/World/UnmadePrototypeHub.cpp')
  for name in ('ResolveNearbyFaction','ResolveNearbyFrontier','ResolveNearbyAfterlight','ResolveNearbyRealmAftermath','ResolveNearbyWitnessBraid','ResolveNearbyWitnessReturn'):
   action=body(hub,'bool AUnmadePrototypeHub::'+name+'(')
   self.assertIn('CommitRealmAftermathEvent(' if name=='ResolveNearbyRealmAftermath' else 'CommitWorldEvent(',action,name)
   self.assertNotIn('if(!WriteWorldSnapshot())',action,name)
  player=read('Private/Player/UnmadeCharacter.cpp')
  commit=body(player,'void AUnmadeCharacter::CommitFaction(')
  self.assertNotIn('ReportLocalEvent(',commit)
  self.assertIn('ResolvePreviewedCommit(Scope,',commit)
  self.assertNotIn('!bResolvedFaction',commit)
 def test_aid_and_completed_tasks_do_not_leave_followup_receipts(self):
  player=read('Private/Player/UnmadeCharacter.cpp')
  aid=body(player,'void AUnmadeCharacter::OfferAid()')
  self.assertNotIn('ReportLocalEvent(',aid)
  self.assertIn('Observation.Evidence==EUnmadeEvidenceKind::Witnessed',aid)
  self.assertIn('AlreadyWitnessed',aid)
  resident=read('Private/World/UnmadeResidentContinuityWorld.cpp')
  self.assertIn('CommitWorldEvent(',body(resident,'bool AUnmadePrototypeHub::RecordResidentAid('))
  task=body(read('Private/World/UnmadePrototypeHub.cpp'),'bool AUnmadePrototypeHub::TryResidentVillageTask(')
  self.assertIn('CommitWorldEvent(',task)
 def test_player_rolls_back_observers_and_deeds_before_presentation(self):
  player=read('Private/Player/UnmadeCharacter.cpp')
  commit=body(player,'bool AUnmadeCharacter::CommitLocalEvents(')
  for token in ('CommitActionConsequences(', 'WriteSnapshot(', 'ReadSnapshot(', 'CommitActionDeeds(', 'StageLocalWitnesses('):self.assertIn(token,commit)
  self.assertIn('IsSessionWritable(this)',commit)
  rites=read('Private/World/UnmadeTenfoldComponent.cpp')
  self.assertNotIn('Player->ReportRiteChoiceEvent(',rites)
  self.assertNotIn('Player->ReportRedeemedOathEvent()',rites)
  self.assertIn('CommitLocalEvent(',body(rites,'void UUnmadeTenfoldComponent::DecideRite('))
  self.assertIn('CommitLocalEvent(',body(rites,'void UUnmadeTenfoldComponent::ChooseConfluence('))
  self.assertIn('CommitLocalEvent(',body(rites,'void UUnmadeTenfoldComponent::BreakChosenOath()'))
 def test_discovery_conflict_supply_and_reward_retry_are_connected(self):
  hub=read('Private/World/UnmadePrototypeHub.cpp')
  self.assertIn('CommitActionDeeds(',body(hub,'bool AUnmadePrototypeHub::InspectSite('))
  player=read('Private/Player/UnmadeCharacter.cpp')
  self.assertIn('CommitLocalEvent(',body(player,'void AUnmadeCharacter::ProgressSupplyActivity()'))
  rewards=body(player,'void AUnmadeCharacter::ReconcileEarnedRewards()')
  self.assertIn('bStartupRewardsReady=false',rewards)
  self.assertIn('InitializeStartupRewards()',rewards)
