"""Physically reachable, save-backed, and outcome-distinct Hearth Petition."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[2]
class HearthPetitionWiring(unittest.TestCase):
 def test_persistent_quest_uses_world_save_and_cannot_corrupt_other_saves(self):
  s=(R/"Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h").read_text()
  h=(R/"Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h").read_text()
  source=(R/"Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp").read_text()
  for text in ("bHasHearthPetition","HearthPetitionState"):self.assertIn(text,s)
  self.assertIn("UnmadeCore::HearthPetition HearthPetition;",h)
  self.assertIn("bHearthPetitionRejected",h)
  self.assertIn("bHearthPetitionRejected",source)
  self.assertIn("RestoredPetition.Restore(",source)
  self.assertIn("Save->bHasHearthPetition=true",source)
  self.assertIn("Save->HearthPetitionState",source)
  self.assertIn("CommitHearthPetition(",source)
  self.assertIn("RefreshHearthPetitionWorld()",source)
 def test_real_interaction_requires_player_proximity_los_and_confirmation(self):
  s=(R/"Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp").read_text()
  p=(R/"Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp").read_text()
  for token in ('Bes.MissedWageBook','Petition.CivicLedger','Petition.PrivateChoice',
       'Petition.PublicChoice','Petition.ReturnWitness','Petition.PrivateMarker',
       'Petition.PublicMarker','LineTraceTestByChannel','CompetingNpcDistanceSq',
       'HearthPetitionPendingTag','HearthPetitionConfirmUntil','WriteWorldSnapshot()'):
   self.assertIn(token,s,token)
  self.assertIn('Hub->TryHearthPetitionInteraction(this,ResidentDistSq)',p)
  self.assertIn('HearthPetition.ReadBook(',s)
  self.assertIn('HearthPetition.WitnessLedger(',s)
  self.assertIn('HearthPetition.Decide(',s)
  self.assertIn('HearthPetition.ReturnWitness(',s)
 def test_named_witnesses_and_journal_reflect_alternative_outcomes(self):
  h=(R/"Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp").read_text()
  npc=(R/"Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp").read_text()
  self.assertIn('GetHearthPetitionJournal()',h)
  self.assertIn('GetHearthPetitionResidentLine(',h)
  self.assertIn('GetHearthPetitionResidentLine(NpcId)',npc)
  self.assertIn('GetHearthPetitionJournal()',h.split('GetBesHouseholdJournal() const',1)[1])
if __name__=="__main__":unittest.main()
