"""Cross-realm world changes are physical, local and tied to witnessed records."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def text(path):return (ROOT/path).read_text()
class CivicReverberationWiring(unittest.TestCase):
 def test_world_has_exclusive_collidable_crossing_routes_and_saltwake_state(self):
  s=text("Source/TheUnmadeGame/Private/World/UnmadePetitionRelay.cpp")
  h=text("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
  for token in ("UnmadeCore::EvaluateCivicReverberation(",
     "Civic.PrivateShelterPassage","Civic.PublicEvidencePlatform",
     "Civic.SaltwakeQuestion","SetRiteWorldActorState(",
     "GetGameDay()","HearthPetition.Ending()"):
   self.assertIn(token,s+h,token)
  self.assertIn("RefreshPetitionRelayWorld()",h)
  self.assertIn("SpawnBlock(",h)
 def test_disagreements_and_nhal_story_are_grounded_and_nonmagical(self):
  s=text("Source/TheUnmadeGame/Private/World/UnmadePetitionRelay.cpp")
  campaign=text("Source/TheUnmadeGame/Private/World/UnmadeCampaignWorld.cpp")
  self.assertIn("EvaluateCivicReverberation(",s)
  self.assertIn("GetPetitionRelayResidentLine(",s)
  self.assertIn("BesHousehold.Save()",s)
  resident=s.split("FString AUnmadePrototypeHub::GetPetitionRelayResidentLine(",1)[1].split("bool AUnmadePrototypeHub::RecordPetitionRelayConversation",1)[0]
  self.assertNotIn("Effects.wageLine",resident)  # residents cannot magically see late Cinderhold payroll
  self.assertIn("Effects.wageLine",s.split("FString AUnmadePrototypeHub::GetPetitionRelayJournal(",1)[1].split("FString AUnmadePrototypeHub::GetPetitionRelayResidentLine(",1)[0])
  for actor in ("SaltDebate","ProtectedRoute","PublicPlatform"):
   self.assertIn(actor,s)
  self.assertIn("ECC_Visibility",s)
  self.assertIn("EvaluateCivicReverberation(",campaign)
  self.assertIn("DescribeUnansweredRoad",campaign)
  self.assertIn("The contradiction is witnessed, not a free victory",campaign)
if __name__=="__main__":unittest.main()
