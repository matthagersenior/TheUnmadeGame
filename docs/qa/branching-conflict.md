# THE UNMADE — First branching settlement dispute

**Status:** Prototype source implemented, not compiled or played inside Unreal Editor.

## Story choice: who gets the protected route?
Near the central fracture, the player is asked to favor either community shelter or anomaly researchers.
- **Z / D-pad Up:** shelter access, research route blocked.
- **X / D-pad Down:** research access, shelter route blocked.
- Press once to preview the mutually exclusive consequence; press the same key again within six seconds to confirm.
- No language clue, combat feat, LLM or network connection is required. Only the chosen route becomes passable. The choice is permanent for the save slot, separate from the earlier physical Rewrite ability.
- A committed decision is written to the existing prototype save; write failure rolls back the in-memory choice before revealing new access.
- NPCs near the actual decision may witness it and react. Distant NPCs may eventually hear rumors via the existing proximity conversation system, never by omniscient global assignment.

## Optional supply errand
- **V / controller face top:** collect a single load of supplies near the market, carry it to the shelter, and deliver with the same key.
- Pickup requires being near the market; delivery requires reaching the shelter. Delivery can happen regardless of the dispute outcome.
- The two stages save alongside the earlier fracture, lexicon, and NPC data; repeated pickup/delivery cannot award repeated credit.
- Delivery emits a witnessed event for NPC memory; local rumors may spread it. This is a deliberately modest graybox activity (no backpack prop, item inventory or trading UI yet).

## Prototype feedback
- **J / controller special right:** plain debug overlay of settlement choice, supply stage, current language clue count, and Strain.
- No finished quest journal widget, cinematics, event animations or soundtrack.
- The routes are primitive blocking cubes with matching collision state, not finished architecture.

## Windows Unreal QA gate — not yet run
1. Compile Editor and run `Unmade.Story.BranchingConflict` plus existing `Unmade.Npc` tests.
2. Approach fracture and press Z once: neither gate changes. Press again within six seconds: only shelter route opens.
3. Reload and verify outcome persists and X cannot reverse the committed choice.
4. New save: choose X instead; only archive route opens, then survives restart.
5. Repeated invalid presses, timeouts, missing save, and corrupt snapshot must not create inconsistent routes.
6. Verify V only collects near market, only delivers near shelter, and cannot duplicate rewards. Revisit and reload to confirm persistence.
7. Ensure prior Rewrite state, lexicon discoveries, and NPC memories survive both story choices and supply saves.
8. Switch off local Ollama and network. Narrative paths and optional activity should behave identically.
9. Verify physical collision/passability on controller and keyboard, and record screenshots.

Native C++17 and Python static tests execute in GitHub Actions, **not** Unreal runtime tests.
