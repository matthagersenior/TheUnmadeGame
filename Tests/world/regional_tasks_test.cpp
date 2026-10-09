#include "World/UnmadeRegionalTaskRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main() {
    RegionalTaskModel tasks;
    assert(tasks.Progress(SettlementId::Bellwold) == TaskProgress::None);
    assert(tasks.Converse(SettlementId::Bellwold, "npc.bellwold.matron.001") == TaskResult::NoChange);
    assert(tasks.Converse(SettlementId::Bellwold, "npc.bellwold.lamplighter.001") == TaskResult::Advanced);
    assert(tasks.Converse(SettlementId::Bellwold, "npc.bellwold.lamplighter.001") == TaskResult::NoChange);
    assert(tasks.Converse(SettlementId::Bellwold, "npc.paperhaven.registrar.001") == TaskResult::NoChange);
    assert(tasks.Converse(SettlementId::Bellwold, "npc.bellwold.matron.001") == TaskResult::Completed);
    assert(tasks.Converse(SettlementId::Bellwold, "npc.bellwold.matron.001") == TaskResult::NoChange);
    assert(tasks.Progress(SettlementId::Bellwold) == TaskProgress::Completed);
    assert(tasks.Converse(SettlementId::Paperhaven, "npc.paperhaven.registrar.001") == TaskResult::NoChange);
    assert(tasks.Converse(SettlementId::Paperhaven, "npc.paperhaven.scribe.001") == TaskResult::Advanced);
    assert(tasks.Converse(SettlementId::Paperhaven, "npc.paperhaven.registrar.001") == TaskResult::Completed);
    assert(tasks.Converse(SettlementId::Crossings, "npc.bellwold.lamplighter.001") == TaskResult::NoChange);
    assert(tasks.Converse(SettlementId::Bellwold, nullptr) == TaskResult::NoChange);
    const auto saved = tasks.Snapshot();
    RegionalTaskModel restored;
    assert(restored.Restore(saved));
    assert(restored.Progress(SettlementId::Bellwold) == TaskProgress::Completed);
    assert(restored.Progress(SettlementId::Paperhaven) == TaskProgress::Completed);
    assert(!restored.Restore({2,99}));
    assert(restored.Progress(SettlementId::Bellwold) == TaskProgress::Completed);
    assert(restored.Progress(SettlementId::Paperhaven) == TaskProgress::Completed);
    assert(restored.Restore({0,1}));
    assert(restored.Progress(SettlementId::Bellwold) == TaskProgress::None);
    assert(restored.Progress(SettlementId::Paperhaven) == TaskProgress::Started);
    std::cout << "PASS: two authored local village stories, gating, idempotence, snapshots\n";
}
