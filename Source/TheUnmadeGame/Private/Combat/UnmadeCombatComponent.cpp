#include "Combat/UnmadeCombatComponent.h"
#include "GameFramework/Actor.h"

UUnmadeCombatComponent::UUnmadeCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UUnmadeCombatComponent::Configure(double MaxHealth, double Damage, double Cooldown)
{
    State = UnmadeCore::Combatant(MaxHealth, Damage, Cooldown);
}

void UUnmadeCombatComponent::SetGearBonuses(int32 Attack, int32 Armor)
{
    GearAttack = FMath::Clamp(Attack, 0, 200);
    GearArmor = FMath::Clamp(Armor, 0, 200);
}

bool UUnmadeCombatComponent::Heal(float Amount)
{
    return State.Heal(static_cast<double>(Amount));
}

void UUnmadeCombatComponent::SetGuarding(bool bEnabled)
{
    State.SetGuarding(bEnabled);
}

bool UUnmadeCombatComponent::TryStrikeTarget(UUnmadeCombatComponent* Target, double Now,
                                              bool bValidHit, bool bFractureExposed)
{
    if (!IsValid(Target) || !bValidHit || Target == this || Target->IsDefeated())
    {
        return false;
    }
    AActor* OwnerActor = GetOwner();
    if (!IsValid(OwnerActor) || !IsValid(Target->GetOwner()))
    {
        return false;
    }
    const UnmadeCore::AttackAttempt Swing = State.TryAttack(Now);
    if (Swing.outcome != UnmadeCore::AttackOutcome::Started)
    {
        return false;
    }
    const auto Hit = Target->State.ReceiveHit(
        static_cast<std::uint64_t>(OwnerActor->GetUniqueID()),
        Swing.swingId, Swing.baseDamage + GearAttack, bFractureExposed, Target->GearArmor);
    return Hit == UnmadeCore::HitOutcome::Applied;
}
