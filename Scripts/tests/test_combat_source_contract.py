"""Lightweight source wiring checks; must not be called Unreal runtime tests."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]
def src(path):
    return (ROOT/path).read_text(encoding="utf-8")

class OfflineCombatContract(unittest.TestCase):
    def test_native_combat_state_and_two_enemy_styles(self):
        core=src("Source/TheUnmadeGame/Public/Combat/UnmadeCombatRules.h")
        for token in ("class Combatant", "ReceiveHit(", "SetGuarding(",
                      "ChooseEnemyIntent(", "EnemyStyle::Stalker", "EnemyStyle::Watcher"):
            self.assertIn(token, core)
        self.assertNotIn("Ollama", core)

    def test_player_buttons_and_hub_enemies(self):
        p=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        ini=src("Config/DefaultInput.ini")
        for token in ('BindAction("Attack"', 'BindAction("Guard"', "TryStrikeTarget(",
                      "LineTraceTestByChannel", "ExposeToFold("):
            self.assertIn(token,p)
        self.assertEqual(hub.count("SpawnActor<AUnmadeEnemyCharacter>("),2)
        for token in ('ActionName="Attack"', 'ActionName="Guard"',
                      'Key=LeftMouseButton','Key=Gamepad_RightTrigger'):
            self.assertIn(token,ini)

    def test_engine_tests_present_but_not_falsely_run_by_ci(self):
        cpp=src("Source/TheUnmadeGame/Private/Tests/CombatAutomationTest.cpp")
        self.assertIn("Unmade.Combat.OfflineRules",cpp)
        workflow=src(".github/workflows/static-checks.yml")
        self.assertIn("/tmp/unmade-combat-test",workflow)
        self.assertNotIn("UnrealEditor",workflow)

if __name__=="__main__":
    unittest.main()
