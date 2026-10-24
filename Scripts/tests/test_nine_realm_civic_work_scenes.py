"""Aftermath must be observable in the world, not just a moved idle NPC."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def code(path): return (ROOT/path).read_text(encoding="utf-8")

class NineRealmCivicWorkScenes(unittest.TestCase):
 def test_each_realm_builds_both_branch_scenes_and_hides_the_uncommitted_branch(self):
  hub=code("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  world=code("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  self.assertIn("DepthAccordWorkActors",hub)
  self.assertIn("DepthAccordWorkActors.SetNum(18)",world)
  self.assertIn("for(int choice=1;choice<=2;++choice)",world)
  self.assertIn("PublicAccordDuty(a,0,choice",world)
  self.assertIn("SetCollisionEnabled(ECollisionEnabled::NoCollision)",world)
  self.assertIn("DepthAccordWorkActors[a*2+choice-1]",world)
  self.assertIn("SetActorHiddenInGame(",world)
  self.assertIn("Expeditions.Arc(a)->returned",world)
  self.assertIn("s->choice==choice",world)
  self.assertIn("bDepthSaveRejected",world)
  self.assertIn("bExpeditionSaveRejected",world)

 def test_workstations_are_physically_inspectable_but_do_not_grant_rewards(self):
  world=code("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  self.assertIn("Add(a,7,choice",world)
  self.assertIn("if(kind==7)",world)
  self.assertIn("E  Inspect working accord station",world)
  section=world.split("if(kind==7)",1)[1].split("if(kind==5",1)[0]
  self.assertIn("public_return_a",section)
  self.assertIn("public_return_b",section)
  self.assertNotIn("WriteWorldSnapshot(",section)
  self.assertNotIn("Depth.Choose(",section)

 def test_at_worksite_actor_performs_existing_branch_specific_motion(self):
  hub=code("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  world=code("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  npc=code("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
  self.assertIn("GetPublicAccordWorkCue(",hub)
  self.assertIn("GetPublicAccordWorkCue(",world)
  self.assertIn("GetPublicAccordWorkCue(NpcId",npc)
  self.assertIn("bAccordDutyAssigned",npc)
  self.assertIn("FName(TEXT(\"Repair\"))",world)
  self.assertIn("FName(TEXT(\"Inspect\"))",world)
  self.assertIn("FVector::DistSquared2D(At,DutyTarget)",world)
  self.assertIn("ServiceWorkCue(NpcId,Current)",npc)
  self.assertIn("AnimateResident(",npc)
  self.assertIn("Phase==UnmadeCore::DayPhase::Night",npc)

if __name__=="__main__": unittest.main()
