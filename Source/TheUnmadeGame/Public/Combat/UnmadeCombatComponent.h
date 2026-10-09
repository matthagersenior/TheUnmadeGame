#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/UnmadeCombatRules.h"
#include "UnmadeCombatComponent.generated.h"

/** Authoritative native combat state. No LLM or paid service involved. */
UCLASS(ClassGroup=(Unmade), meta=(BlueprintSpawnableComponent))
class THEUNMADEGAME_API UUnmadeCombatComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UUnmadeCombatComponent();

    void Configure(double MaxHealth, double Damage, double Cooldown);
    bool TryStrikeTarget(UUnmadeCombatComponent* Target, double Now, bool bValidHit, bool bFractureExposed);
    void SetGuarding(bool bEnabled);

    UFUNCTION(BlueprintPure, Category="Unmade|Combat")
    bool IsDefeated() const { return !State.IsAlive(); }

    UFUNCTION(BlueprintPure, Category="Unmade|Combat")
    float GetHealth() const { return static_cast<float>(State.Health()); }

    UFUNCTION(BlueprintPure, Category="Unmade|Combat")
    float GetMaxHealth() const { return static_cast<float>(State.MaximumHealth()); }

    UFUNCTION(BlueprintPure, Category="Unmade|Combat")
    bool IsGuarding() const { return State.IsGuarding(); }

private:
    UnmadeCore::Combatant State;
};
