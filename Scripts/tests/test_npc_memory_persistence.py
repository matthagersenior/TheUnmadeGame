"""NPC memory cannot disappear because an actor was not spawned on save."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class NpcMemoryPersistence(unittest.TestCase):
 def test_npc_memory_preflight_at_actual_saved_profile_boundary(self):
  core=read("Source/TheUnmadeGame/Public/Save/UnmadeCriticalSavePreflight.h")
  self.assertIn("ValidNpcSnapshots(save.NpcSnapshots)",core)
  self.assertIn("Save/UnmadeNpcMemorySaveRules.h",core)
  rules=read("Source/TheUnmadeGame/Public/Save/UnmadeNpcMemorySaveRules.h")
  self.assertIn("SpeakerId.IsNone()",rules)
  self.assertIn("EventId.IsValid()",rules)
 def test_current_world_does_not_erase_absent_residents(self):
  hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
  section=hub.split("bool AUnmadePrototypeHub::SaveNpcMemories()",1)[1].split("void AUnmadePrototypeHub::SaveCitizens()",1)[0]
  self.assertNotIn("Save->NpcSnapshots.Reset()",section)
  self.assertIn("CommitPendingMemory(bNpcMemorySavePending",section)
  self.assertIn("WriteWorldSnapshot()",section)
  self.assertIn("bNpcMemorySavePending=true",section)
  player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
  self.assertNotIn("Save->NpcSnapshots.Reset()",player)
  self.assertIn("Hub->SaveNpcMemories()",player)
if __name__=="__main__":unittest.main()
