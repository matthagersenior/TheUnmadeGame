#pragma once

// Offline deterministic combat. No Unreal dependency and no LLM/network integration.
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>
#include <vector>

namespace UnmadeCore {
enum class AttackOutcome { Started, Guarding, CoolingDown, Defeated, InvalidTime };
enum class HitOutcome { Applied, Duplicate, AlreadyDefeated, InvalidSource, InvalidDamage };
enum class EnemyStyle { Stalker, Watcher };
enum class EnemyIntent { Approach, Retreat, Attack, Hold };
struct AttackAttempt {
    AttackOutcome outcome = AttackOutcome::CoolingDown;
    std::uint64_t swingId = 0;
    double baseDamage = 0;
};
class Combatant final {
public:
    Combatant(double maximumHealth = 100.0, double damage = 24.0, double cooldown = 0.75)
        : maximumHealth_(Positive(maximumHealth) ? maximumHealth : 100.0),
          health_(maximumHealth_), damage_(Positive(damage) ? damage : 24.0),
          cooldown_(Positive(cooldown) ? cooldown : 0.75) {}

    double Health() const noexcept { return health_; }
    double MaximumHealth() const noexcept { return maximumHealth_; }
    bool IsAlive() const noexcept { return health_ > 0; }
    bool IsGuarding() const noexcept { return guarding_; }

    bool SetGuarding(bool enabled) noexcept {
        if (!IsAlive() || guarding_ == enabled) return false;
        guarding_ = enabled;
        return true;
    }
    AttackAttempt TryAttack(double now) noexcept {
        if (!std::isfinite(now) || now < 0) return {AttackOutcome::InvalidTime, 0, 0};
        if (!IsAlive()) return {AttackOutcome::Defeated, 0, 0};
        if (guarding_) return {AttackOutcome::Guarding, 0, 0};
        if (now < nextAttackAt_) return {AttackOutcome::CoolingDown, 0, 0};
        nextAttackAt_ = now + cooldown_;
        return {AttackOutcome::Started, ++swingCounter_, damage_};
    }
    HitOutcome ReceiveHit(std::uint64_t attackerId, std::uint64_t swingId,
                          double baseDamage, bool fractureExposed) {
        if (!IsAlive()) return HitOutcome::AlreadyDefeated;
        if (!attackerId || !swingId) return HitOutcome::InvalidSource;
        if (!Positive(baseDamage)) return HitOutcome::InvalidDamage;
        for (const auto& recent : seen_) {
            if (recent.first == attackerId && swingId <= recent.second)
                return HitOutcome::Duplicate;
        }
        auto old = std::find_if(seen_.begin(), seen_.end(),
            [attackerId](const auto& entry) { return entry.first == attackerId; });
        if (old != seen_.end()) old->second = swingId;
        else {
            if (seen_.size() == 32) seen_.erase(seen_.begin());
            seen_.emplace_back(attackerId, swingId);
        }
        const double multiplier = (guarding_ ? 0.25 : 1.0) * (fractureExposed ? 1.5 : 1.0);
        health_ = std::max(0.0, health_ - baseDamage * multiplier);
        if (!IsAlive()) guarding_ = false;
        return HitOutcome::Applied;
    }
private:
    static bool Positive(double n) { return std::isfinite(n) && n > 0; }
    double maximumHealth_, health_, damage_, cooldown_, nextAttackAt_ = 0;
    bool guarding_ = false;
    std::uint64_t swingCounter_ = 0;
    std::vector<std::pair<std::uint64_t, std::uint64_t>> seen_;
};

inline EnemyIntent ChooseEnemyIntent(EnemyStyle style, double distanceMeters,
                                     bool clearSight) noexcept {
    if (!std::isfinite(distanceMeters) || distanceMeters < 0) return EnemyIntent::Hold;
    if (style == EnemyStyle::Stalker) {
        return distanceMeters <= 2.2 && clearSight ? EnemyIntent::Attack : EnemyIntent::Approach;
    }
    if (style == EnemyStyle::Watcher) {
        if (distanceMeters < 3.5) return EnemyIntent::Retreat;
        if (distanceMeters <= 7.0 && clearSight) return EnemyIntent::Attack;
        return EnemyIntent::Approach;
    }
    return EnemyIntent::Hold; // Unknown styles must not inherit an enemy attack policy.
}
}
