"""Each public personal accord is discoverable in the returning settlement world."""
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
def source(path):
    return (ROOT/path).read_text(encoding="utf-8")

class NineRealmPublicAccordMarkers(unittest.TestCase):
    def test_nine_real_world_notice_actors_are_spawned_and_refreshed(self):
        header=source("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        world=source("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
        self.assertIn("DepthAccordSigns",header)
        self.assertIn("DepthAccordSigns.SetNum(9)",world)
        self.assertIn("DepthAccordSigns[a]=Sign(",world)
        self.assertIn("DepthAccordSigns[a].IsValid()",world)
        self.assertIn("DepthAccordSigns[a]->SetActorHiddenInGame(",world)

    def test_notice_requires_witnessed_return_and_committed_branch(self):
        world=source("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
        self.assertIn("Expeditions.Arc(a)->returned",world)
        self.assertIn("s->choice",world)
        self.assertIn("DepthSpecs[a].public_return_a",world)
        self.assertIn("DepthSpecs[a].public_return_b",world)
        self.assertIn("bDepthSaveRejected",world)
        self.assertIn("bExpeditionSaveRejected",world)
        marker=world.split("if(DepthAccordSigns[a].IsValid())",1)[1].split("if(!bDepthSaveRejected&&s->choice==2",1)[0]
        self.assertIn("SetText(",marker)
        self.assertNotIn("WriteWorldSnapshot(",marker)
        self.assertNotIn("Depth.Choose(",marker)
        self.assertNotIn("SaveGameToSlot(",marker)
if __name__=="__main__":unittest.main()
