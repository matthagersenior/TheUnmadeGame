# Six Cross-Realm Dual-Discipline Observatories — Win64 Unreal Acceptance

**Source implementation, 10 October 2026. These instructions do not assert the game was compiled or tested in an Unreal Editor.**

## Earned prerequisites
The existing Echo story is deliberately a hard narrative prerequisite, not a combat grind: three authored chapters in that specific realm are resolved before the observatory plinth appears. The character must then have earned each paired RiteId at mastery stage 5. Each cast is a **real successful execution** through `UUnmadeTenfoldComponent::InvokeRite`, with normal cooldown, cost, and restrictions. Neither a failed cast nor a fake dialogue string advances the site.

| Realm | Real pair | Extra archive / living-world significance |
|---|---|---|
| Drevlach | Tomorrow Debt + Oathbinding | Future loans and the binding rights of children who never consented |
| Orravane | Unwrite Law + Paradox Convergence | Two supporting harmonies that do not erase singers |
| Vathless | Legacy Forging + Understanding Bosses | A guardian's true name and the ethical provenance of stone |
| Eillun | Witnesscraft + Borrowed Lives | Testimony without forced birth identity |
| Tharniv | Living Roads + Oathbinding | Common roads and voluntary civic vows |
| Auvren | Unwrite Law + Cartography | Contradictory beginnings and a provably safe way home |

## Test every one of the six, both prior Echo outcomes
1. At a fresh realm save, the plinth and bridge must be absent. Completing only the first or second chapter does not reveal them. Complete the Echo Quest through two materially distinct clues, a matching witness, and a confirmed final civic choice.
2. The plinth must now appear on the **accessible upper civic terrace**. A second, currently hidden bridge must lead to an equally hidden island and archive. Old Echo saves (before the new optional save flag) must continue loading.
3. Approach within 370 cm, with a clear line of sight. Attempt an unrelated mastered rite: no resonance. Attempt an unmastered rite: no resonance. Cast the first listed rite successfully using the existing power control and verify a saved first-cast state, with a visible two-minute deadline in UI production.
4. Cast the **same** power again: still incomplete. Cast the other mastered power within 120 persisted world seconds: a traversable bridge and island appear; player can walk to the archive and back without teleportation or fall-through. Test opposite cast order.
5. If more than 120 seconds elapse, a new cast restarts the window. Reload mid-window: remaining interval is preserved by the saved world clock, rather than resetting to transient UE uptime. Rewind/corrupt the persisted clock: reject rather than granting an invalid instant second cast.
6. Read the archive with E; display both its authored insight **and the previously chosen Echo resolution**, with no repeatable item or economy grant. The nearby three residents can refer to the locally open archive. Residents of other realms must not suddenly know all its records.
7. Simulate `SaveGameToSlot` failure on first-cast and final-cast. The resonance result must roll back; a rite already successfully cast and persisted remains cast, without inventing a free extra use. Invalid six-element save array lengths, nonfinite deadlines, contradictory stages or a schema conflict block mutation without replacing unrelated progress.
8. Validate low-framerate transition and actual capsule collision, navmesh reachability, input/readability for keyboard and gamepad, source-actor tags, camera near bridges, and memory/scene budgets. Add authorable sound/visual cue and a player-facing journal countdown. Current source only uses prototype on-screen debug text.
9. Run the Editor automation test `Unmade.World.SixDualRiteObservatories` plus every other `Unmade` test. Capture full UHT/compiler and game logs. Existing Linux C++ tests do not substitute for any of this.

### Physics note
The observatory span runs from relative Y 3500 to 4200, connects the north floor (ending 3750), and reaches a genuinely separate island (beginning 4050). This is an offline geometry assertion in the shared layout contract, *not* proof that runtime collision behaves as intended. Keep the player's current save and checkpoint intact during failed traversals.

### Production remaining
Visual effects for each distinctive paired discipline, bespoke 3D archives and landmarks, full quest UI, NPC performance and dialogue state, ability/cooldown/readability feedback, ambient music, balanced cooldowns, accessibility, gamepad and Win64 Editor/PIE validation.
