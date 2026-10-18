"""A hand-delivered report must physically change recipient behavior, not just a UI counter."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class RecipientScene(unittest.TestCase):
 def test_only_named_recipient_gets_local_delivery_dialogue(self):
  h=read("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  world=read("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
  self.assertIn("GetCrossRealmCarryRecipientLine(FName",h)
  self.assertIn("GetCrossRealmCarryRecipientLine(NpcId)",npc)
  section=world.split("FString AUnmadePrototypeHub::GetCrossRealmCarryRecipientLine(",1)[1].split("FString AUnmadePrototypeHub::GetDepthReaction(",1)[0]
  self.assertIn('npc.expedition.%d.0',section)
  self.assertIn("CarryStage::Delivered",section)
  self.assertIn("I personally received",section)
  self.assertNotIn("WriteWorldSnapshot(",section)
 def test_delivery_remaps_actual_recipient_routine_to_physical_receipt(self):
  world=read("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  start=world.index("bool AUnmadePrototypeHub::GetPublicAccordRoutineTarget(")
  end=world.index("FName AUnmadePrototypeHub::GetPublicAccordWorkCue(",start)
  routine=world[start:end]
  self.assertIn("CrossRealmCarry.Stage((a+8)%9)",routine)
  self.assertIn("n==0",routine)
  self.assertIn("FVector(950,600,0)",routine)
  self.assertIn("Expeditions.Arc(a)->visited",routine)
  cue=world.split("FName AUnmadePrototypeHub::GetPublicAccordWorkCue(",1)[1].split("bool AUnmadePrototypeHub::TryDepthInteraction",1)[0]
  self.assertIn("CrossRealmCarry.Stage((a+8)%9)",cue)
  self.assertIn('TEXT("Inspect")',cue)
 def test_physical_receipt_is_visible_only_after_saved_handoff(self):
  world=read("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  self.assertIn("DepthCrossRealmReceiptSigns.SetNum(9)",world)
  self.assertIn("CrossRealmCarry.Stage(incoming)>=UnmadeCore::CarryStage::Delivered",world)
  self.assertIn("DepthCrossRealmReceiptSigns[a]->SetActorHiddenInGame(!received)",world)
if __name__=="__main__":unittest.main()
