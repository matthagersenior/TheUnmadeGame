from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def src(p):return (ROOT/p).read_text()
class FinalBossContracts(unittest.TestCase):
    def test_persistent_story_integrated(self):
        h=src("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        save=src("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        world=src("Source/TheUnmadeGame/Private/World/UnmadeFinalWorld.cpp")
        for term in ("FinalJourney", "GetFinalAct(", "GetNewMorning("):
            self.assertIn(term,h)
        for term in ("FinalEncounterStage","FinalMorningChoice","FinalMemorySeed","bHasFinalEncounterSnapshot"):
            self.assertIn(term,save)
            self.assertIn(term,hub)
        self.assertIn("bFinalSaveRejected",hub)
        self.assertIn("Restore(",world)
        self.assertIn("SaveGame",hub)
    def test_boss_must_telegraph_and_false_victory(self):
        boss=src("Source/TheUnmadeGame/Private/Combat/UnmadeFinalBoss.cpp")
        rule=src("Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h")
        self.assertIn("FinalBattleRhythm",rule)
        for token in ("FinalMove::Telegraph","FinalMove::Strike",
                      "BreakFinalMask(", "BreakFinalCore(", "ACharacter::Tick(",
                      "TryStrikeTarget(", "IsFractureExposed("):
            self.assertIn(token,boss)
        self.assertNotIn("AUnmadeEnemyCharacter::Tick(",boss)
        self.assertIn("FinalStrikeInFootprint(",boss)
        self.assertIn("GetActorRightVector()",boss)
        self.assertIn("Beat.move==UnmadeCore::FinalMove::Still",boss)
    def test_world_after_boss_and_safe_player_restart(self):
        world=src("Source/TheUnmadeGame/Private/World/UnmadeFinalWorld.cpp")
        player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        npc=src("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        self.assertIn("FinalLocalRecord(",npc)
        self.assertIn("RefreshFinalWorld()",world)
        self.assertIn("BuildFinalWorld()",world)
        self.assertIn("SetActorLocation(",world)
        self.assertIn("CommitFinalMorning(",world)
        self.assertIn("WakeFinalEcho(",world)
        self.assertIn("TryFinalInteraction(this,",player)
        self.assertIn("Final.World.",world)
    def test_ci(self):
        self.assertIn("final_encounter_test.cpp",src(".github/workflows/static-checks.yml"))
if __name__=="__main__":unittest.main()
