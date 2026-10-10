"""Six later realms have independent fair danger zones and engine source wiring."""
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def src(p):return (ROOT/p).read_text()
class LaterHazardWiring(unittest.TestCase):
    def test_clock_and_collision_cues(self):
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        later=src("Source/TheUnmadeGame/Private/World/UnmadeLaterRealmWorld.cpp")
        h=src("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        self.assertIn("GetNearbyLaterRealmHazard(",h+later)
        self.assertIn("RefreshLaterHazardCues()",hub+later)
        self.assertIn("SampleLaterHazard(",later)
        self.assertIn("HazardCue",later)
        self.assertIn("SetActorEnableCollision(false)",later)
        self.assertIn("Clock.ElapsedSeconds()",later)
        self.assertIn("RefreshLaterHazardCues()",hub)
        self.assertIn("RefreshLaterHazardCues()",later)
        self.assertIn("LaterRealm.Stage(",later)
    def test_damage_strain_and_pulse_identity(self):
        player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        header=src("Source/TheUnmadeGame/Public/Player/UnmadeCharacter.h")
        self.assertIn("GetNearbyLaterRealmHazard(",player)
        self.assertIn("LastLaterHazardPulse[6]",header)
        self.assertIn("ReceiveHazardPulse(",player)
        self.assertIn("ApplyEnvironmentalStrain(",player)
        self.assertIn("SaveFractureState()",player)
        self.assertIn("LaterHazardEffect::Strain",player)
        self.assertIn("LaterHazardEffect::Injury",player)
    def test_native_test_registered(self):
        wf=src(".github/workflows/static-checks.yml")
        self.assertIn("Tests/world/later_hazard_test.cpp",wf)
if __name__=="__main__":unittest.main()
