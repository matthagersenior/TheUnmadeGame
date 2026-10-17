#pragma once
#include <cmath>

namespace UnmadeCore {
enum class ConsumableCommit { Unavailable, Used, SaveFailed };
// A failed commit must not fall through to a more valuable potion.
template<class Items, class Use>
ConsumableCommit UseFirstConsumable(const Items& items, Use use) {
    for(const auto item:items){
        const auto result=use(item);
        if(result!=ConsumableCommit::Unavailable)return result;
    }
    return ConsumableCommit::Unavailable;
}

// Copying the model also preserves active Fold/Glimpse windows on failure;
// restoring a persistent-only snapshot would incorrectly cancel those effects.
template<class Fracture, class Writer>
bool CommitStrainRecovery(Fracture& fracture, double amount, Writer write) {
    if (!std::isfinite(amount) || amount < 0) return false;
    const auto before = fracture;
    if (amount > 0 && !fracture.Recover(amount / Fracture::RecoveryPerSecond)) return false;
    if (write()) return true;
    fracture = before;
    return false;
}

// A failed in-session recovery must not use profile restoration: that resets
// combat windows and can erase a cooldown or an already acknowledged hit.
template<class Combat, class Stage, class Writer>
bool CommitCombatRecovery(Combat& combat, Stage stage, Writer write) {
    const auto before = combat.CaptureRecoveryState();
    if (stage() && write()) return true;
    combat.RestoreRecoveryState(before);
    return false;
}

// The synchronous writer captures live health. Apply recovery before capturing
// the record, and undo it if the write fails. The caller owns cost rollback.
template<class Combat, class Writer>
bool CommitHealing(Combat& combat, double amount, Writer write) {
    if (!std::isfinite(amount) || amount < 0) return false;
    return CommitCombatRecovery(combat,
        [&] { return amount == 0 || combat.Heal(amount); }, write);
}
}
