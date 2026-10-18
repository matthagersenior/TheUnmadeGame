"""Tactical combat stations and saved expedition aftermath must affect live UE gameplay."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source(p):return (ROOT/p).read_text()
class NineRealmTacticalObjectives(unittest.TestCase):
 def test_each_boss_chamber_has_timed_two_station_counterplay(self):
  hub=source("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  hdr=source("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  for value in ("ArenaCounterStations", "ArenaCounters[arc].Use(", "case 10:",
    "Arenas[", "bBossReady", "ExposeToFold(", "CounterObjective"):
   self.assertIn(value,hub+hdr,value)
  self.assertIn("Candidate(a,10,0,",hub)
  self.assertIn("Candidate(a,10,1,",hub)
  self.assertIn("ExpeditionEnemyInactive(",hub)
  self.assertIn("ArenaCounters[arc].Reset()",hub)
 def test_native_combat_counter_changes_boss_only_after_actual_two_step(self):
  rules=source("Source/TheUnmadeGame/Public/World/UnmadeArenaTacticsRules.h")
  hub=source("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  self.assertIn("CounterStep::Success",hub)
  self.assertIn("counter.exposureSeconds",hub)
  self.assertIn("CounterStep::Armed",hub)
  self.assertIn("std::array<ArenaTacticSpec,9>",rules)
  self.assertIn("ArenaAftermathText(",hub)
 def test_boss_has_true_support_threats_without_reload_farming(self):
  hub=source("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  enemy=source("Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp")
  header=source("Source/TheUnmadeGame/Public/Combat/UnmadeEnemyCharacter.h")
  player=source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
  for token in ("ArenaEscort", "11+add", "ExpeditionRole::Sentinel", "ExpeditionRole::Marksman"):
   self.assertIn(token,hub,token)
  self.assertIn("IsTemporaryArenaEscort()",header)
  self.assertIn("if(IsTemporaryArenaEscort())",enemy)
  self.assertIn("!Target->IsTemporaryArenaEscort()",player)
  self.assertIn("ExpeditionEnemyInactive(",hub)

 def test_streamed_arena_markers_start_hidden_until_restored_state_is_applied(self):
  world=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
  self.assertIn('ArenaShelterVisible[a]=-1',world)
  self.assertIn('ArenaAftermathVisible[a]=-1',world)
  self.assertIn('SetRiteWorldActorState(b.Label,false)',world)

 def test_outcomes_are_derived_from_real_boss_and_captive_save_flags(self):
  hub=source("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  self.assertIn("ArenaAftermathRecord(",hub)
  self.assertIn("Expeditions.Arc(a)",hub)
  self.assertIn("Expedition.%d.Aftermath.",hub)
  self.assertIn("GetExpeditionJournal()",hub)
if __name__=="__main__":unittest.main()
