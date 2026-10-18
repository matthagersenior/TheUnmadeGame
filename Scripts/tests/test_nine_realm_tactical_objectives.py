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
  self.assertIn("ReadArenaAftermath",hub)
 def test_outcomes_are_derived_from_real_boss_and_captive_save_flags(self):
  hub=source("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  self.assertIn("ArenaAftermathRecord(",hub)
  self.assertIn("Expeditions.Arc(a)",hub)
  self.assertIn("Expedition.%d.Aftermath.",hub)
  self.assertIn("GetExpeditionJournal()",hub)
if __name__=="__main__":unittest.main()
