"""Source contracts for optional Echo quests, integrity and Unreal handoff."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source(path): return (ROOT/path).read_text()
class EchoQuestWiring(unittest.TestCase):
    def test_runtime_actors_and_confirmation(self):
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        echo=source("Source/TheUnmadeGame/Private/World/UnmadeEchoQuestWorld.cpp")
        header=source("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        for item in ["BuildEchoQuests()", "RefreshEchoQuestWorld()", "TryEchoQuestConversation(",
                     "InspectEchoQuestSite(", "EchoCommitGate.Attempt(",
                     "EchoQuest.Commit(", "EchoQuest.Restore(Before)",
                     "EchoTag(i,TEXT(\"NeighborSignal\"))",
                     "RealmAftermath.Stage(Q.realm)==4",
                     "LaterRealm.IsComplete(Q.realm)", "FVector::DistSquared("]:
            self.assertIn(item,echo+hub+header)
        self.assertIn("BuildEchoQuests();",hub)
        self.assertIn("RefreshEchoQuestWorld();",hub)
        self.assertIn("SetRiteWorldActorState(",echo)
        self.assertIn("GetWorld()->LineTraceTestByChannel(",echo)
        self.assertIn("UGameplayStatics::SaveGameToSlot(",hub)
    def test_atomic_saves_and_controller(self):
        save=source("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for term in ["bHasEchoQuestSnapshot","EchoQuestStages","EchoQuestEvidence",
                     "EchoQuestTestimonies","EchoQuestEndings"]:
            self.assertIn(term,save)
            self.assertIn(term,hub)
        self.assertIn("bEchoSaveRejected=true;",hub)
        self.assertIn("bLaterRealmSaveRejected || bEchoSaveRejected ||",hub)
        self.assertIn("bResonanceSaveRejected || bGuardianSaveRejected ||",hub)
        self.assertIn("InspectEchoQuestSite(",player)
        self.assertIn("TryEchoQuestConversation(",player)
        self.assertIn("GetEchoQuestStage(",player)
    def test_floor_and_gates_have_usable_dimensions(self):
        world=source("Source/TheUnmadeGame/Private/World/UnmadeLaterRealmWorld.cpp")
        rules=source("Source/TheUnmadeGame/Public/World/UnmadeRealmGeometryRules.h")
        for term in ["LaterArenaLayout::SouthCenterY","LaterArenaLayout::SouthHalfY",
                     "LaterArenaLayout::NorthCenterY","LaterArenaLayout::NorthHalfY",
                     "LaterArenaLayout::BridgeCenterY","LaterArenaLayout::BridgeHalfY",
                     "FVector(55,.55,3)"]:
            self.assertIn(term,world)
        self.assertIn("static_assert(LaterArenaLayout::Sound()",rules)
        self.assertIn("WallMiddle()",rules)
if __name__=="__main__":unittest.main()
