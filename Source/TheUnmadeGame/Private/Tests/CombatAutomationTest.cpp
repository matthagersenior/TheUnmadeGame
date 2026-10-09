#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Combat/UnmadeCombatRules.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FUnmadeCombatCoreTest, "Unmade.Combat.OfflineRules",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FUnmadeCombatCoreTest::RunTest(const FString& Parameters)
{
    UnmadeCore::Combatant Player(100.0, 24.0, 0.75);
    UnmadeCore::Combatant Enemy(60.0, 12.0, 1.3);
    TestTrue(TEXT("guard enables"), Player.SetGuarding(true));
    TestTrue(TEXT("attack blocked while guarding"),
        Player.TryAttack(0.0).outcome == UnmadeCore::AttackOutcome::Guarding);
    TestTrue(TEXT("guard can be released"), Player.SetGuarding(false));
    const auto Hit = Player.TryAttack(0.0);
    TestTrue(TEXT("swing begins"), Hit.outcome == UnmadeCore::AttackOutcome::Started);
    TestTrue(TEXT("hit lands"), Enemy.ReceiveHit(1, Hit.swingId, Hit.baseDamage, false)
        == UnmadeCore::HitOutcome::Applied);
    TestTrue(TEXT("same swing never damages twice"),
        Enemy.ReceiveHit(1, Hit.swingId, Hit.baseDamage, false)
        == UnmadeCore::HitOutcome::Duplicate);
    TestEqual(TEXT("health reflects one impact"), Enemy.Health(), 36.0);
    TestTrue(TEXT("watcher retreats when cornered"),
        UnmadeCore::ChooseEnemyIntent(UnmadeCore::EnemyStyle::Watcher, 2.0, true)
        == UnmadeCore::EnemyIntent::Retreat);
    TestTrue(TEXT("stalker advances"),
        UnmadeCore::ChooseEnemyIntent(UnmadeCore::EnemyStyle::Stalker, 6.0, true)
        == UnmadeCore::EnemyIntent::Approach);
    return true;
}
#endif
