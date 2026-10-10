"""Six later destinations must be actor-backed; tests never imply UE was compiled."""
import pathlib
import unittest
ROOT=pathlib.Path(__file__).resolve().parents[2]
def src(path):return (ROOT/path).read_text()
class SixLaterRealmSource(unittest.TestCase):
    def test_world_builders_and_real_travel(self):
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        header=src("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        for symbol in ("BuildLaterRealms()", "InspectLaterRealmSite(", "TryLaterRealmConversation(",
                       "RefreshLaterRealmWorld()", "LaterRealmJourney",
                       "AtlasPassages", "Attunement", "GetCurrentRealmName(",
                       "SetRiteWorldActorState(", "FVector::DistSquared("):
            self.assertIn(symbol,hub+header)
        self.assertIn("LaterRealms",hub)
        self.assertIn("RealmAftermath.Prepare(",hub)
        self.assertIn("RealmAftermath.Testify(",hub)
    def test_player_and_save(self):
        player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        save=src("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        world=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for value in ("LaterVisitedMask","LaterRealmStages","LaterRealmChoices","bHasLaterRealmSnapshot"):
            self.assertIn(value,save+world)
        self.assertIn("LaterRealm.Restore(Before)",world)
        self.assertIn("InspectLaterRealmSite(",player)
        self.assertIn("TryLaterRealmConversation(",player)
        self.assertIn("ResolveNearbyRealmAftermath(",player)
    def test_npc_identity_and_local_reactions(self):
        npc=src("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        self.assertIn("ConfigureLaterRealm(",npc)
        self.assertIn("GetRealmAftermathEnding(",npc)
        self.assertIn("World.LaterRealmResolved",npc)
if __name__=="__main__": unittest.main()
