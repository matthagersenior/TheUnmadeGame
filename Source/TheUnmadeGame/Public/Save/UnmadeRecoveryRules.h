#pragma once
// Source-only, save-backed recovery rules. The combatant copy contains hit
// dedup receipts, cooldown, current health and guard state (not merely health).
#include "Combat/UnmadeCombatRules.h"
#include <cmath>

namespace UnmadeCore {
enum class ConsumableAttempt { Unavailable, Used, SaveFailed };

template<class Range,class Try>
ConsumableAttempt UseFirstConsumable(const Range& items,Try attempt) {
    for(const auto& item:items) {
        const auto outcome=attempt(item);
        if(outcome!=ConsumableAttempt::Unavailable)return outcome;
    }
    return ConsumableAttempt::Unavailable;
}

template<class Writer>
bool CommitCombatHealing(Combatant& state,double amount,Writer writer) {
    if(!std::isfinite(amount)||amount<0)return false;
    const Combatant before=state;
    if(amount>0&&!state.Heal(amount))return false;
    if(writer())return true;
    state=before;
    return false;
}
}
