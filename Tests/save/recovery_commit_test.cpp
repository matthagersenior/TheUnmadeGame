#include "Save/UnmadeRecoveryRules.h"
#include "Combat/UnmadeCombatRules.h"
#include "Items/UnmadeItemRules.h"
#include "Fracture/UnmadeFractureRules.h"
#include <cassert>
#include <limits>

using namespace UnmadeCore;
struct Player {
    Combatant body;
    double stamina = 19;
    struct RecoveryState {Combatant body;double stamina;};
    double GetHealth() const { return body.Health(); }
    double GetStamina() const { return stamina; }
    bool Heal(double amount) { return body.Heal(amount); }
    bool RestoreSession(double health, double energy) {
        stamina = energy;
        return body.RestoreHealth(health);
    }
    RecoveryState CaptureRecoveryState()const{return {body,stamina};}
    void RestoreRecoveryState(const RecoveryState& before){body=before.body;stamina=before.stamina;}
};
int main() {
    Player player;
    assert(player.body.RestoreHealth(37));
    InventoryModel bag;
    assert(bag.Add(ItemId::HearthSalve, 2));
    auto before = bag.Snapshot();
    int heal = 0, strain = 0, writes = 0;
    assert(bag.Consume(ItemId::HearthSalve, 63, 0, heal, strain) == ConsumeResult::Used);
    double savedHealth = 0;
    InventorySnapshot savedBag;
    assert(CommitHealing(player, heal, [&] {
        ++writes;
        savedHealth = player.GetHealth();
        savedBag = bag.Snapshot();
        return true;
    }));
    assert(savedHealth == 37 + heal && writes == 1);
    Player reloaded;
    InventoryModel reloadedBag;
    assert(reloaded.body.RestoreHealth(savedHealth));
    assert(reloadedBag.Restore(savedBag));
    assert(reloaded.GetHealth() == player.GetHealth());
    assert(reloadedBag.Quantity(ItemId::HearthSalve) == 1);

    // Failed storage must restore both benefit and cost; retry consumes once.
    assert(bag.Restore(before));
    assert(player.body.RestoreHealth(37));
    assert(bag.Consume(ItemId::HearthSalve, 63, 0, heal, strain) == ConsumeResult::Used);
    assert(!CommitHealing(player, heal, [&] {
        assert(player.GetHealth() == 37 + heal);
        return false;
    }));
    assert(bag.Restore(before));
    assert(player.GetHealth() == 37 && player.GetStamina() == 19);
    assert(bag.Quantity(ItemId::HearthSalve) == 2);

    // The same transaction couples a finite service receipt to clamped healing.
    assert(player.body.RestoreHealth(90));
    int serviceUses = 0;
    assert(CommitHealing(player, 25, [&] {
        assert(player.GetHealth() == 100);
        ++serviceUses;
        savedHealth = player.GetHealth();
        return true;
    }));
    assert(serviceUses == 1 && savedHealth == 100);
    assert(player.body.RestoreHealth(90));
    assert(!CommitHealing(player, 25, [] { return false; }));
    assert(player.GetHealth() == 90);
    // Strain-only consumables still commit without requiring health benefit.
    assert(CommitHealing(player, 0, [&] { ++writes; return true; }));
    int calls = 0;
    for (double invalid : {-1.0, std::numeric_limits<double>::quiet_NaN(),
                           std::numeric_limits<double>::infinity()})
        assert(!CommitHealing(player, invalid, [&] { ++calls; return true; }));
    assert(calls == 0 && player.GetHealth() == 90);
    FractureModel fracture("region", {"open"});
    assert(fracture.Fold(true, 10) == Result::Applied);
    const double oldStrain = fracture.CurrentStrain();
    assert(!CommitStrainRecovery(fracture, 20, [&] {
        assert(fracture.CurrentStrain() == oldStrain - 20);
        return false;
    }));
    assert(fracture.CurrentStrain() == oldStrain && fracture.IsFolded(11));
    double savedStrain = -1;
    assert(CommitStrainRecovery(fracture, 20, [&] {
        savedStrain = fracture.CurrentStrain();
        return true;
    }));
    FractureModel restoredFracture("region", {"open"});
    assert(restoredFracture.Restore({"", savedStrain}));
    assert(restoredFracture.CurrentStrain() == oldStrain - 20);
    assert(CommitStrainRecovery(fracture, 20, [] { return true; }));
    assert(fracture.CurrentStrain() == 0); // saturates without charging twice
    assert(!CommitStrainRecovery(fracture, 20, [&] { ++calls; return true; }));
    assert(calls == 0);
}
