"""Every realm guardian must be actor backed with safe nonviolent resolution."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source(p):return (ROOT/p).read_text()
class RealmGuardiansWiring(unittest.TestCase):
    def test_no_untelegraphed_base_enemy_tick(self):
        guardian=source("Source/TheUnmadeGame/Private/Combat/UnmadeRealmGuardian.cpp")
        world=source("Source/TheUnmadeGame/Private/World/UnmadeRealmGuardianWorld.cpp")
        for term in ("ACharacter::Tick(DeltaSeconds)","GuardianAction::Telegraph",
                     "GuardianAction::Strike","TryStrikeTarget(",
                     "UnmadeGuardianSight","IsFractureExposed"):
            self.assertIn(term,guardian)
        self.assertNotIn("AUnmadeEnemyCharacter::Tick(",guardian)
        self.assertIn("BuildRealmGuardians()",world)
        self.assertIn("RefreshRealmGuardians()",world)
        self.assertIn("TryCalmNearbyGuardian(",world)
        self.assertIn("ResolveRealmGuardian(",world)
        self.assertIn("Guardians.Restore(Before)",world)
        self.assertIn("GuardianCommitGate.Attempt(",world)
        self.assertIn("CommitmentAttempt::Confirmed",world)
        self.assertIn("EchoQuest.Ending(Realm)!=0",world)
        self.assertIn("WriteWorldSnapshot()",world)
    def test_gameplay_and_memories(self):
        pc=source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        npc=source("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        echo=source("Source/TheUnmadeGame/Private/World/UnmadeEchoQuestWorld.cpp")
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertIn("TryCalmNearbyGuardian(this,ResidentDistSq)",pc)
        self.assertIn("TActorIterator<AUnmadeRealmGuardian>",pc)
        self.assertIn("Guardian->ExposeToFold(Now,5.0)",pc)
        self.assertIn("GetRealmGuardianOutcome(LaterHome)",npc)
        self.assertIn("RefreshRealmGuardians()",echo)
        self.assertIn("BuildRealmGuardians();",hub)
        self.assertIn("RefreshRealmGuardians();",hub)
    def test_persistent_outcomes_are_optional(self):
        save=source("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for field in ("bHasGuardianSnapshot","RealmGuardianOutcomes"):
            self.assertIn(field,save)
            self.assertIn(field,hub)
        self.assertIn("bGuardianSaveRejected=true;",hub)
        self.assertIn("Guardians=RestoredGuardians;",hub)
        self.assertIn("bGuardianSaveRejected ||",hub)
        self.assertIn("bFinalSaveRejected || bResidentContinuityRejected ||",hub)
        self.assertIn("bWitnessEchoRejected)return false;",hub)
        self.assertIn("Save->RealmGuardianOutcomes.Num()!=6",hub)
    def test_ci(self):
        self.assertIn("Tests/combat/realm_guardian_test.cpp",
            source(".github/workflows/static-checks.yml"))
if __name__=="__main__":unittest.main()
