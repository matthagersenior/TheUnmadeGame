#include "Story/UnmadeConflictRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main() {
    ConflictModel game;
    assert(game.Choice() == ConflictChoice::None);
    assert(!game.ShelterOpen() && !game.ArchiveOpen());
    assert(game.Preview(ConflictChoice::Shelter) == ConflictResult::NeedsConfirmation);
    assert(game.Choice() == ConflictChoice::None);
    assert(game.Commit(ConflictChoice::None,true) == ConflictResult::InvalidChoice);
    assert(game.Commit(ConflictChoice::Shelter,false) == ConflictResult::NeedsConfirmation);
    assert(game.Commit(ConflictChoice::Shelter,true) == ConflictResult::Committed);
    assert(game.ShelterOpen() && !game.ArchiveOpen());
    assert(game.Commit(ConflictChoice::Research,true) == ConflictResult::AlreadyCommitted);
    assert(game.Choice() == ConflictChoice::Shelter);
    assert(!game.DeliverSupplies(true));
    assert(!game.CollectSupplies(false));
    assert(game.CollectSupplies(true));
    assert(!game.CollectSupplies(true));
    assert(!game.DeliverSupplies(false));
    assert(game.DeliverSupplies(true));
    assert(!game.DeliverSupplies(true));
    const auto snapshot = game.Snapshot();
    ConflictModel restored;
    assert(restored.Restore(snapshot));
    assert(restored.ShelterOpen() && restored.Supplies() == SupplyStage::Delivered);
    assert(!restored.Restore({88,2}) && !restored.Restore({1,99}));
    assert(restored.Snapshot().choice == snapshot.choice);
    assert(restored.Snapshot().supplies == snapshot.supplies);
    ConflictModel alternate;
    assert(alternate.Commit(ConflictChoice::Research,true) == ConflictResult::Committed);
    assert(alternate.ArchiveOpen() && !alternate.ShelterOpen());
    assert(alternate.CollectSupplies(true));
    assert(alternate.Supplies() == SupplyStage::Carrying);
    std::cout << "PASS: AI-independent conflict, one-time choices, supply activity, atomic save restoration\n";
}
