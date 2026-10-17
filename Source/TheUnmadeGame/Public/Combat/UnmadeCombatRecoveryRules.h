#pragma once
#include "Combat/UnmadeCombatRules.h"
#include "Combat/UnmadeCombatStyleRules.h"
#include "Combat/UnmadeContactRules.h"

namespace UnmadeCore {
// In-session recovery rollback must preserve receipts and active combat windows.
// This is a runtime copy, not a new save format or persisted combat cooldown.
struct CombatRecoveryState {
 Combatant body;
 CombatTempo tempo;
 ContactWindow contact;
 CombatStatus status=CombatStatus::None;
 double staggerUntil=0;
 double statusUntil=0;
};
}
