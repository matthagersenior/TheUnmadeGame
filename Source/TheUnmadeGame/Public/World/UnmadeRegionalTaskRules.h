#pragma once
// Small, independently playable village stories. No model or network dependency.
#include "World/UnmadeSettlementRegistry.h"
#include <cstring>

namespace UnmadeCore {
enum class TaskProgress { None, Started, Completed };
enum class TaskResult { NoChange, Advanced, Completed };
struct RegionalTaskSnapshot { int bellwold = 0; int paperhaven = 0; };
class RegionalTaskModel final {
public:
    TaskProgress Progress(SettlementId village) const noexcept {
        if (village == SettlementId::Bellwold) return static_cast<TaskProgress>(bellwold_);
        if (village == SettlementId::Paperhaven) return static_cast<TaskProgress>(paperhaven_);
        return TaskProgress::None;
    }
    TaskResult Converse(SettlementId village, const char* residentId) noexcept {
        if (!residentId) return TaskResult::NoChange;
        int* stage = nullptr;
        const char* starter = nullptr;
        const char* finisher = nullptr;
        if (village == SettlementId::Bellwold) {
            stage = &bellwold_;
            starter = "npc.bellwold.lamplighter.001";
            finisher = "npc.bellwold.matron.001";
        } else if (village == SettlementId::Paperhaven) {
            stage = &paperhaven_;
            starter = "npc.paperhaven.scribe.001";
            finisher = "npc.paperhaven.registrar.001";
        } else return TaskResult::NoChange;

        if (*stage == 0 && std::strcmp(starter, residentId) == 0) {
            *stage = 1; return TaskResult::Advanced;
        }
        if (*stage == 1 && std::strcmp(finisher, residentId) == 0) {
            *stage = 2; return TaskResult::Completed;
        }
        return TaskResult::NoChange;
    }
    RegionalTaskSnapshot Snapshot() const noexcept { return {bellwold_, paperhaven_}; }
    bool Restore(const RegionalTaskSnapshot& value) noexcept {
        if (value.bellwold < 0 || value.bellwold > 2 ||
            value.paperhaven < 0 || value.paperhaven > 2) return false;
        bellwold_ = value.bellwold;
        paperhaven_ = value.paperhaven;
        return true;
    }
private:
    int bellwold_ = 0;
    int paperhaven_ = 0;
};
}
