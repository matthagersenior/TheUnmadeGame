#pragma once
#include <cmath>

namespace UnmadeCore {
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

// The synchronous writer captures live health. Apply recovery before capturing
// the record, and undo it if the write fails. The caller owns cost rollback.
// Combat exposes GetHealth/GetStamina/Heal/RestoreSession in the UE adapter.
template<class Combat, class Writer>
bool CommitHealing(Combat& combat, double amount, Writer write) {
    if (!std::isfinite(amount) || amount < 0) return false;
    const auto health = combat.GetHealth();
    const auto stamina = combat.GetStamina();
    if (amount > 0 && !combat.Heal(amount)) return false;
    if (write()) return true;
    combat.RestoreSession(health, stamina);
    return false;
}
}
