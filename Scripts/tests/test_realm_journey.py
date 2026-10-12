import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
class RealmJourneyWiring(unittest.TestCase):
 def test_journal_uses_real_equipment_owner(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp').read_text().split('GetExpeditionJournal()const',1)[1].split('GetExpeditionReaction',1)[0]
  self.assertIn('GetPlayerCharacter',s);self.assertIn('equipment->SelectedBenefit()',s);self.assertNotIn('p->',s);self.assertNotIn('Expeditions.Save().gear',s)
 def test_physical_arrival_commits_before_encounters(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp').read_text()
  self.assertIn('InsideRegisteredChamber(a,r,at.X,at.Y,at.Z)',s);self.assertIn('if(changed)CommitExpeditions(before,p)',s);self.assertIn('!UnmadeCore::ExpeditionEncounterActive(*t,slot)',s)
 def test_all_irreversible_branch_and_reward_inputs_confirm(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp').read_text()
  for x in ['ConfirmExpeditionDecision(arc,1,1)','ConfirmExpeditionDecision(a,1,2)','ConfirmExpeditionDecision(arc,2,1)','ConfirmExpeditionDecision(a,2,2)']:self.assertIn(x,s)
  self.assertIn('ExpeditionReturnWitness',s);self.assertIn('sight.AddIgnoredActor(*N)',s)
 def test_tracker_covers_all_terminal_transitions(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp').read_text().split('GetTrackedObjective()const',1)[1].split('GetExperienceJournal()',1)[0]
  for x in ['Board','Frontier','Chamber','Resolution','Boss','Return','Reward','PersonalOffer','Witness','PersonalEvidence','Accord','WingOffer','WingFieldwork','WingGuardian','WingReward','Home']:self.assertIn('case S::'+x+':',s)
 def test_composed_route_is_part_of_ci(self):
  s=(ROOT/'.github/workflows/static-checks.yml').read_text();self.assertIn('Tests/world/realm_journey_test.cpp',s)
if __name__=='__main__':unittest.main()
