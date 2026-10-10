"""Afterlight integration boundaries. Native tests are the behavioral oracle;
these checks only cover Unreal source connections, not a UE build."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(path): return (ROOT/path).read_text(encoding="utf-8")
class BellwoldAfterlightWiring(unittest.TestCase):
    def test_physical_sites_and_mutually_exclusive_world_state(self):
        world=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for word in ("Bellwold.Afterlight.Relief","Bellwold.Afterlight.Census",
                     "Bellwold.Afterlight.SafeWard","Bellwold.Afterlight.OpenCensus"):
            self.assertIn(word,world)
        self.assertIn("RefreshAfterlightWorld();",world)
        self.assertIn("UnmadeCore::AfterlightChoice::Relief",world)
        self.assertIn("UnmadeCore::AfterlightChoice::Revelation",world)
        self.assertIn("Afterlight.Resolve(",world)
        self.assertIn("Afterlight.Inspect(",world)
        self.assertIn("Afterlight.Begin(",world)
    def test_story_reacts_to_saved_action_not_proximity_only(self):
        world=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for fn in ("TryAfterlightConversation","InspectAfterlightClue","ResolveNearbyAfterlight"):
            self.assertIn("Hub->"+fn+"(",player)
        self.assertIn("static_cast<int>(Chronicle.Ending(UnmadeCore::Faction::Refuge))",world)
        self.assertIn("static_cast<int>(RegionalTasks.Progress(UnmadeCore::SettlementId::Bellwold))",world)
        self.assertIn("if(!WriteWorldSnapshot())",world)
        self.assertIn("Afterlight.Restore(Before);",world)
        self.assertIn("GetAfterlightStage()",player)
    def test_old_save_support_and_distinct_rewards(self):
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        world=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        equipment=read("Source/TheUnmadeGame/Private/Items/UnmadeEquipmentComponent.cpp")
        for k in ("bHasAfterlightSnapshot","BellwoldAfterlightStage",
                  "BellwoldAfterlightApproach","BellwoldAfterlightOutcome"):
            self.assertIn(k,save)
            self.assertIn(k,world)
        self.assertIn("bAfterlightSaveRejected=true;",world)
        self.assertIn("UPROPERTY(SaveGame)\n    bool bHasInventorySnapshot = false;",save)
        self.assertNotIn("UPROPERTY(SaveGame)\n    /** Bellwold return-visit story",save)
        self.assertIn("if(bAfterlightSaveRejected || bRealmAftermathSaveRejected ||",world)
        self.assertIn("bLaterRealmSaveRejected)return false;",world)
        self.assertIn("Achievement::AfterlightShelter",equipment)
        self.assertIn("Achievement::AfterlightNames",equipment)
    def test_npc_firsthand_testimony_and_offline_scenario(self):
        npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        memory=read("Source/TheUnmadeGame/Private/NPC/UnmadeMemoryComponent.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        scenario=read("Tests/integration/offline_slice_scenario.cpp")
        for event in ("World.AfterlightShelter","World.AfterlightNames"):
            self.assertIn(event,npc)
            self.assertIn(event,memory)
            self.assertIn(event,player)
        self.assertIn("BellwoldAfterlight secondNight;",scenario)
        self.assertIn("BellwoldAfterlight alternateSecondNight;",scenario)
if __name__=="__main__":unittest.main()
