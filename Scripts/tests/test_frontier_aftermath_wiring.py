"""Frontier physical adventures must use real Unreal world actors and durable saves.
Native aftermath tests own behavior; this checks the engine integration boundary.
"""
import pathlib
import unittest

ROOT=pathlib.Path(__file__).resolve().parents[2]
def source(p): return (ROOT/p).read_text(encoding="utf-8")

class FrontierAftermathWiring(unittest.TestCase):
    def test_two_physical_mechanisms_and_two_evidence_routes_per_realm(self):
        world=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        rules=source("Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h")
        for tag in ("Saltwake.Aftermath.StormSluice",
                    "Saltwake.Aftermath.Cistern","Saltwake.Aftermath.RainLedger",
                    "Cinderhold.Aftermath.HeatVent",
                    "Cinderhold.Aftermath.CommonKiln","Cinderhold.Aftermath.EmberDeed",
                    "Saltwake.Aftermath.StormBarrier","Cinderhold.Aftermath.HeatSeal"):
            self.assertIn(tag,world+rules)
        self.assertIn("RealmAftermath.Prepare(",world)
        self.assertIn("RealmAftermath.Inspect(",world)
        self.assertIn("RealmAftermath.Testify(",world)
        self.assertIn("RealmAftermath.Decide(",world)
        self.assertIn("RefreshRealmAftermathWorld()",world)
        self.assertIn("LineTraceTestByChannel(",world)

    def test_player_interaction_and_choice_are_wired(self):
        hub=source("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        player=source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for name in ("InspectRealmAftermathSite","TryRealmAftermathConversation",
                     "ResolveNearbyRealmAftermath","GetRealmAftermathStage",
                     "GetRealmAftermathEnding"):
            self.assertIn(name,hub)
            self.assertIn(name,player)
        self.assertIn("ReportLocalEvent(",player)

    def test_atomic_save_and_restore_guards(self):
        save=source("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for field in ("bHasRealmAftermathSnapshot","RealmAftermathStages",
                      "RealmAftermathPrepared","RealmAftermathApproaches",
                      "RealmAftermathEndings"):
            self.assertIn(field,save)
            self.assertIn(field,hub)
        self.assertIn("bRealmAftermathSaveRejected",hub)
        self.assertIn("RestoredRealmAftermath.Restore(",hub)
        self.assertIn("RealmAftermath.Restore(Before)",hub)
        self.assertIn("if(bAfterlightSaveRejected || bRealmAftermathSaveRejected ||",hub)
        self.assertIn("bLaterRealmSaveRejected || bEchoSaveRejected)return false;",hub)

if __name__=="__main__":
    unittest.main()
