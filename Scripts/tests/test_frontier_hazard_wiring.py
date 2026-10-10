"""Source contracts for fair frontier hazards and non-softlocking player recovery."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def get(p):return (ROOT/p).read_text(encoding="utf-8")
class RecoveryWiring(unittest.TestCase):
    def test_runtime_hazard_and_guard_counterplay(self):
        hub=get("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=get("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        component=get("Source/TheUnmadeGame/Private/Combat/UnmadeCombatComponent.cpp")
        self.assertIn("SampleFrontierHazard(",hub)
        self.assertIn("GetNearbyFrontierHazard(",player)
        self.assertIn("ReceiveHazardPulse(",player)
        self.assertIn("State.ReceiveHit(",component)
        self.assertIn("GetWorldClockSeconds()",hub+player)
        self.assertIn("bHasRealmAftermathSnapshot",get("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h"))
    def test_defeat_is_no_longer_permanent_stuck_state(self):
        player=get("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        combat=get("Source/TheUnmadeGame/Private/Combat/UnmadeCombatComponent.cpp")
        self.assertIn("ReviveAtCheckpoint()",player)
        self.assertIn("SetMovementMode(MOVE_Walking)",player)
        self.assertIn("ReviveAtCheckpoint(",combat)
        self.assertIn("SetActorLocation(",player)
if __name__=="__main__":unittest.main()
