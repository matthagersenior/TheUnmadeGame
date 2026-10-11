from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source(p): return (ROOT/p).read_text(encoding="utf-8")
class RuntimeWorldTest(unittest.TestCase):
    def test_six_fighting_exploration_sites(self):
        s=source("Source/TheUnmadeGame/Private/World/UnmadePlayableEncounters.cpp")
        for place in ("Reach.AshRoad","Reach.GlassCut","Bellwold.Forgotten",
                      "Paperhaven.Torn","Saltwake.Saltglass","Cinderhold.Slag"):
            self.assertIn(place,s)
        self.assertIn("for(int32 i=0;i<3;++i)",s)
        self.assertIn("ConfigureEncounter",s)
        self.assertIn("BuildPlayableEncounters();",
                      source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp"))
    def test_camera_can_be_skipped_and_restored(self):
        s=source("Source/TheUnmadeGame/Private/World/UnmadeUnpaidShiftPresentation.cpp")
        for item in ("FrameBesDirectedBeat","RestoreBesCamera","SetViewTargetWithBlend",
                     "SetFieldOfView","BesSubtitleOverlay->ClearBeat"):
            self.assertIn(item,s)
        self.assertIn("Hub->SkipBesDirectedScene()",
                      source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp"))
        self.assertIn('ActionName="SkipDirectedScene",Key=BackSpace',
                      source("Config/DefaultInput.ini"))
if __name__=="__main__": unittest.main()
