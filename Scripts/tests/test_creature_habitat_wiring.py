"""Native encounter habitat contract: six distinct authored prototype habitats."""
from pathlib import Path
import re,unittest
ROOT=Path(__file__).resolve().parents[2]
ENC=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadePlayableEncounters.cpp").read_text()
HUB=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp").read_text()
PLAYER=(ROOT/"Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp").read_text()
HEAD=(ROOT/"Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h").read_text()
class HabitatWiring(unittest.TestCase):
 def test_six_distinct_collision_bearing_habitat_types(self):
  for tag in ("JawDen","GlassRoost","HoundUndercroft","ArchiveBurrow","TideCave","SlagMaw"):
   self.assertIn(tag,ENC)
  self.assertIn("BuildEncounterHabitat",ENC)
  self.assertIn("SetCollisionEnabled",ENC)
  self.assertIn("HabitatInspectSigns",ENC)
  self.assertIn("CreatureKind",ENC)
 def test_reactive_environment_and_physical_interaction_in_real_source(self):
  for name in ("TryHabitatInteraction","RefreshHabitatReactions"):
   self.assertIn(name,HEAD)
  self.assertIn("HabitatReaction(",HUB)
  self.assertIn("RefreshHabitatReactions(Position)",HUB)
  self.assertIn("TryHabitatInteraction(this,ResidentDistSq)",PLAYER)
  self.assertIn("SetIntensity(",HUB)
  self.assertIn("HabitatCanInspect(",ENC)
 def test_safety_and_existing_quests(self):
  self.assertIn("TryFieldSurveyInteraction(this,ResidentDistSq)",PLAYER)
  self.assertIn("HabitatInspectSigns",HEAD)
  self.assertNotIn("GiveReward(",ENC)
  self.assertNotIn("SaveGameToSlot(",ENC.split("bool AUnmadePrototypeHub::TryHabitatInteraction")[1])
