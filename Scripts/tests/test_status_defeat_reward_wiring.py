"""Native attribution contracts only; portable tests cover rule/storage behavior."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2] / "Source/TheUnmadeGame"


def read(path):
    return (ROOT / path).read_text(encoding="utf-8")


class StatusDefeatRewardWiring(unittest.TestCase):
    def test_combo_and_projectile_retain_actual_source(self):
        component = read("Private/Combat/UnmadeCombatComponent.cpp")
        projectile = read("Private/Combat/UnmadeTravelingProjectile.cpp")
        self.assertIn("Target->ApplyStatus(Style.status,4,OwnerActor)", component)
        self.assertIn("combat->ApplyStatus(ShotStatus,4,Shooter.Get())", projectile)

    def test_status_clearing_drops_previous_attribution(self):
        header = read("Public/Combat/UnmadeCombatComponent.h")
        component = read("Private/Combat/UnmadeCombatComponent.cpp")
        self.assertIn("TWeakObjectPtr<AActor> StatusSource", header)
        clear = header.split("void ClearTemporaryStatus()", 1)[1].split("\n", 1)[0]
        self.assertIn("StatusSource.Reset()", clear)
        apply = component.split("void UUnmadeCombatComponent::ApplyStatus(", 1)[1].split(
            "void UUnmadeCombatComponent::TickComponent(", 1
        )[0]
        self.assertIn("Source->GetWorld()==GetWorld()?Source:nullptr", apply)

    def test_lethal_tick_uses_retained_source_without_new_witness_event(self):
        component = read("Private/Combat/UnmadeCombatComponent.cpp")
        tick = component.split("void UUnmadeCombatComponent::TickComponent(", 1)[1].split(
            "bool UUnmadeCombatComponent::HasTemporaryStatus()", 1
        )[0]
        self.assertIn("Cast<AUnmadeCharacter>(StatusSource.Get())", tick)
        self.assertIn("ApplyStatusDamagePulse", tick)
        self.assertIn("PlayerSource->NotifyStatusDefeat(Enemy)", tick)
        self.assertNotIn("GetPlayerCharacter", tick)
        self.assertNotIn("NotifyProjectileImpact", tick)
        self.assertNotIn("ReportLocalEvent", tick)


if __name__ == "__main__":
    unittest.main()
