"""Six resonance bridge mechanics must be tied to persisted, mastered rite casts."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def src(path):return (ROOT/path).read_text()
class ResonanceWiring(unittest.TestCase):
    def test_real_cast_and_npc_awareness(self):
        ten=src("Source/TheUnmadeGame/Private/World/UnmadeTenfoldComponent.cpp")
        world=src("Source/TheUnmadeGame/Private/World/UnmadeRealmResonanceWorld.cpp")
        npc=src("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        for term in ("if(!Persist(true))", "Hub->RecordNearbyRealmRite(Player,Id)",
                     "Chronicle.IsMastered(Id)"):
            self.assertIn(term,ten)
        self.assertIn("Resonance.Record(",world)
        self.assertIn("EchoQuest.Ending(Spec.realm)!=0",world)
        self.assertIn("Clock.ElapsedSeconds()",world)
        self.assertIn("Resonance.Restore(Before)",world)
        self.assertIn("RefreshRealmResonanceWorld()",world)
        self.assertIn("SetRiteWorldActorState(",world)
        self.assertIn("GetRealmResonanceStage(LaterHome)",npc)
    def test_physical_nonfarmable_observatory(self):
        world=src("Source/TheUnmadeGame/Private/World/UnmadeRealmResonanceWorld.cpp")
        player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        echo=src("Source/TheUnmadeGame/Private/World/UnmadeEchoQuestWorld.cpp")
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for tag in ('TEXT("Plinth")','TEXT("Bridge")','TEXT("Island")','TEXT("Archive")'):
            self.assertIn(tag,world)
        self.assertIn("FVector::DistSquared(",world)
        self.assertIn("LineTraceTestByChannel(",world)
        self.assertIn("InspectResonanceArchive(this,ResidentDistSq)",player)
        self.assertIn("RefreshRealmResonanceWorld()",echo)
        self.assertIn("BuildRealmResonance();",hub)
        self.assertIn("RefreshRealmResonanceWorld();",hub)
        self.assertIn("return true; // One-time story state",world)
    def test_save_backward_compatibility(self):
        save=src("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for field in ("bHasRealmResonanceSnapshot","RealmResonanceStages",
                      "RealmResonanceFirstCasts","RealmResonanceDeadlines"):
            self.assertIn(field,save)
            self.assertIn(field,hub)
        self.assertIn("bResonanceSaveRejected=true;",hub)
        self.assertIn("bResonanceSaveRejected || bGuardianSaveRejected)return false;",hub)
        self.assertIn("Resonance=RestoredResonance;",hub)
        self.assertIn("Save->RealmResonanceStages.Num()!=6",hub)
    def test_native_test_in_ci(self):
        workflow=src(".github/workflows/static-checks.yml")
        self.assertIn("Tests/world/realm_resonance_test.cpp",workflow)
if __name__=="__main__":unittest.main()
