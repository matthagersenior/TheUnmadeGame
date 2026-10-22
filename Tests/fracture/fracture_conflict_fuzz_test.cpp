#include "Fracture/UnmadeFractureRules.h"
#include "Story/UnmadeConflictRules.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>

using namespace UnmadeCore;

namespace {
uint32_t Next(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

void CheckFractureInvariants(const FractureModel& model) {
    assert(std::isfinite(model.CurrentStrain()));
    assert(model.CurrentStrain() >= 0.0 && model.CurrentStrain() <= FractureModel::MaxStrain);
    assert(model.WorldVariant().empty() || model.WorldVariant() == "variant.open" ||
           model.WorldVariant() == "variant.sealed");
}
}

int main() {
    constexpr uint32_t Seed = 0x5EED1234;
    uint32_t random = Seed;
    FractureModel fracture("region.prototype.hub", {"variant.open", "variant.sealed"});
    ConflictModel conflict;

    for (int i = 0; i < 100000; ++i) {
        const uint32_t value = Next(random);
        const double now = static_cast<double>(value % 100000) / 100.0;
        const auto before = fracture.TakeSnapshot();
        switch ((value >> 8) % 8) {
            case 0: fracture.Glimpse((value & 1) != 0, now, static_cast<double>((value >> 16) % 9)); break;
            case 1: fracture.Fold((value & 1) != 0, now, static_cast<double>((value >> 16) % 7)); break;
            case 2: {
                const std::string choice = (value & 1) ? "variant.open" : "variant.sealed";
                const Result result = fracture.Rewrite("region.prototype.hub", choice, true);
                if (result == Result::Applied) {
                    // Model a durable-write failure after the in-memory commit.
                    assert(fracture.Restore(before));
                    assert(fracture.TakeSnapshot().variant == before.variant);
                    assert(fracture.TakeSnapshot().strain == before.strain);
                }
                break;
            }
            case 3: fracture.Recover(static_cast<double>((value >> 16) % 1000) / 10.0); break;
            case 4: fracture.SpendStrain(static_cast<double>((value >> 16) % 130)); break;
            case 5: fracture.ApplyEnvironmentalStrain(static_cast<double>((value >> 16) % 130)); break;
            case 6: {
                FractureModel restored("region.prototype.hub", {"variant.open", "variant.sealed"});
                assert(restored.Restore(fracture.TakeSnapshot()));
                assert(restored.TakeSnapshot().variant == fracture.TakeSnapshot().variant);
                assert(restored.TakeSnapshot().strain == fracture.TakeSnapshot().strain);
                break;
            }
            case 7: {
                const FractureSnapshot invalid{"variant.injected", std::numeric_limits<double>::quiet_NaN()};
                assert(!fracture.Restore(invalid));
                assert(fracture.TakeSnapshot().variant == before.variant);
                assert(fracture.TakeSnapshot().strain == before.strain);
                break;
            }
        }
        CheckFractureInvariants(fracture);

        const auto conflictBefore = conflict.Snapshot();
        switch ((value >> 12) % 6) {
            case 0: conflict.Commit(ConflictChoice::Shelter, true); break;
            case 1: conflict.Commit(ConflictChoice::Research, true); break;
            case 2: conflict.Commit(ConflictChoice::Shelter, false); break;
            case 3: conflict.CollectSupplies((value & 2) != 0); break;
            case 4: conflict.DeliverSupplies((value & 4) != 0); break;
            case 5: {
                ConflictModel restored;
                assert(restored.Restore(conflictBefore));
                const auto roundTrip = restored.Snapshot();
                assert(roundTrip.choice == conflictBefore.choice);
                assert(roundTrip.supplies == conflictBefore.supplies);
                break;
            }
        }
        const auto committed = conflict.Snapshot();
        assert(committed.choice >= 0 && committed.choice <= 2);
        assert(committed.supplies >= 0 && committed.supplies <= 2);
        if (conflictBefore.choice != 0) assert(committed.choice == conflictBefore.choice);
        if (conflictBefore.supplies == 2) assert(committed.supplies == 2);
    }

    std::cout << "PASS deterministic fracture/conflict stress: seed 0x5EED1234, 100000 transitions, rollback and persistence invariants\n";
}
