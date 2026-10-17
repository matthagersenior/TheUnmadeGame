"""Player-facing pre-PC development contracts against native adapters."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]/"Source/TheUnmadeGame"
class GameplayDepthWiring(unittest.TestCase):
 def test_real_enemy_actor_consumes_twelve_tactics(self):
  actor=(ROOT/"Private/Combat/UnmadeEnemyCharacter.cpp").read_text()
  header=(ROOT/"Public/Combat/UnmadeEnemyCharacter.h").read_text()
  self.assertIn("UnmadeCore::TacticalCreatureIntent(",actor)
  self.assertIn("CreatureKind Species",header)
  self.assertIn("Species=Kind;",actor)
  self.assertIn("bTacticalFlank",actor)
  self.assertIn("UnmadeCore::CreatureTactic(",actor)
 def test_guidance_connects_to_live_saved_quest_flags(self):
  experience=(ROOT/"Private/World/UnmadePlayerExperience.cpp").read_text()
  depth=(ROOT/"Private/World/UnmadeAdventureDepth.cpp").read_text()
  self.assertIn("UnmadeCore::NextOpeningBeat(",experience)
  self.assertIn("UnmadeCore::NextInvestigationLead(",depth)
  self.assertIn("UnmadeCore::NextInvestigationLead(",experience)
  self.assertIn("TEXT(\"FIRST JOURNEY",experience)
 def test_confluence_and_household_feedback_reflect_real_state(self):
  rites=(ROOT/"Private/World/UnmadeTenfoldComponent.cpp").read_text()
  hub=(ROOT/"Private/World/UnmadePrototypeHub.cpp").read_text()
  self.assertIn("UnmadeCore::DescribeConfluenceNeed(",rites)
  self.assertIn("UnmadeCore::CommunityPressure(",hub)
  self.assertIn("BesHousehold.Save()",hub)
 def test_first_return_cinematic_cues_match_real_story_beats(self):
  import json
  manifest=json.loads((ROOT.parents[1]/"Authoring/first_journey_performance_map_v1.json").read_text())
  self.assertEqual(manifest["schema_version"],1)
  self.assertEqual(len(manifest["beats"]),11)
  self.assertEqual([b["stage"] for b in manifest["beats"]],list(range(11)))
  source=(ROOT/"Public/World/UnmadeJourneyGuidanceRules.h").read_text()
  for beat in manifest["beats"]:
   self.assertIn(beat["title"],source)
   self.assertTrue(beat["skippable"])
   self.assertTrue(beat["requires_saved_story_state"])
   self.assertTrue(beat["shot_instruction"] and beat["sound_cue"] and beat["subtitle_prompt"])
 def test_species_feint_does_not_cancel_existing_warning(self):
  actor=(ROOT/"Private/Combat/UnmadeEnemyCharacter.cpp").read_text()
  self.assertIn("bExistingWindup=WindupEndsAt>=0.0",actor)
  self.assertIn("bElderEncounter||bExistingWindup",actor)

if __name__=="__main__":unittest.main()
