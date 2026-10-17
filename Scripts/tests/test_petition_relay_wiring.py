"""Three-realm hand-carry must use actual interactions, preflight and atomic world save."""
import unittest
from pathlib import Path
R=Path(__file__).resolve().parents[2]
def read(p):return (R/p).read_text()
class PetitionRelayWiring(unittest.TestCase):
 def test_world_save_protects_orphan_or_malformed_relay(self):
  s=read('Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h')
  h=read('Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h')
  c=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')+read('Source/TheUnmadeGame/Private/World/UnmadePetitionRelay.cpp')
  pre=read('Source/TheUnmadeGame/Public/Save/UnmadeCombinedSaveRules.h')
  for token in ('bHasPetitionRelay','PetitionRelayState'):self.assertIn(token,s)
  for token in ('UnmadeCore::PetitionRelay PetitionRelay;','bPetitionRelayRejected'):self.assertIn(token,h)
  for token in ('RestoredRelay.Restore(', 'Save->bHasPetitionRelay=true',
      'Save->PetitionRelayState=', 'bPetitionRelayRejected', 'CommitPetitionRelay('):self.assertIn(token,c)
  for token in ('Save->bHasPetitionRelay','RestoredRelay.Restore('):self.assertIn(token,pre)
 def test_real_physical_quest_and_conversations(self):
  p=read('Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp')
  c=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')+read('Source/TheUnmadeGame/Private/World/UnmadePetitionRelay.cpp')
  npc=read('Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp')
  self.assertIn('Hub->TryPetitionRelayInteraction(this,ResidentDistSq)',p)
  self.assertIn('Hub->RecordPetitionRelayConversation(Target->GetStableId())',p)
  self.assertIn('GetPetitionRelayResidentLine(NpcId)',npc)
  for token in ('Relay.CinderholdCopy','Relay.SaltwakePrivate','Relay.SaltwakePublic',
                'Relay.CrossingsPrivate','Relay.CrossingsPublic','LineTraceTestByChannel',
                'CompetingNpcDistanceSq', 'GetPetitionRelayJournal()', 'WriteWorldSnapshot()'):
   self.assertIn(token,c,token)
  for token in ('PetitionRelay.Collect(', 'PetitionRelay.GiveToMera(',
                'PetitionRelay.TakeMeraReply(', 'PetitionRelay.GiveToOrrel('):self.assertIn(token,c)
  self.assertIn('npc.saltwake.courier.001',c)
  self.assertIn('npc.bridgekeeper.001',c)
 def test_fixture_and_ui(self):
  a=read('Tests/save/combined_record_fixture.h')
  b=read('Tests/integration/world_capture_fixture.h')
  w=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')
  for s in ('bHasPetitionRelay','PetitionRelayState'):
   self.assertIn(s,a)
   self.assertIn('out.'+s+'=world.'+s,b)
  self.assertIn('GetPetitionRelayJournal()',w.split('GetBesHouseholdJournal() const',1)[1])
if __name__=='__main__':unittest.main()
