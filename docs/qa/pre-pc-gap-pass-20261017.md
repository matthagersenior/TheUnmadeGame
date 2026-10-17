# The Unmade — pre-PC gap and upgrade pass (17 October 2026)

**Scope:** GitHub draft PR #74 and its source/portable tests, not an Unreal build.
The source is not a playable game; static CI does not substitute for native engine
compilation or a human playthrough.

## Confirmed, actionable gap addressed in this batch

- **Trigger rebinding (P1):** the preferences adapter rejected *every* analog
  input, including default left/right gamepad triggers. The shared portable
  predicate now accepts those two trigger buttons but still refuses analog
  movement/look axes. Native Unreal dispatch remains to be tested on PC.
- **Prompt consistency (P1):** the RPG widget calculated a contextual hint
  (including the chosen interaction button) but formatted the unadjusted original.
  The displayed prompt now consumes the contextual value. Native rendered focus,
  labels and input-mode behavior remain to be tested.
- Regression coverage: portable compiled policy examples plus static checks for
  the actual Unreal adapter and widget connection. Test-first CI failure was
  observed before the fix. No story choice, quest ID, save schema or lore changed.

## Prioritized next source work before Unreal

| Priority | Gameplay gap / upgrade | Pre-PC acceptance | Later Unreal acceptance |
| --- | --- | --- | --- |
| P0 | **First 15 minutes of player discovery:** character creation, initial objective, evidence and first consequence need a legible step-by-step path; broad simulation does not prove understandable onboarding. | One deterministic clean-profile walkthrough fixture, with deliberate wrong actions, refusal and recovery; complete captions/objective/tell sheet linked to actual source IDs. | Player can learn mechanics without developer help, traverse, converse, fight, choose, save and return. |
| P0 | **Input and modal-state integrity:** verify gamepad, keyboard, menu focus, paused-world, directed-scene and load/travel contexts together. Registry/list availability is not actual controller accessibility. | Matrix of all 28 Actions-page dispatch paths and 8 menu tabs; tests that modal actions do not leak into gameplay and that confirmation is cleared on scope change. | UI navigation, focus visibility, back/cancel, held-button release and rebinding with real gamepads. |
| P0 | **Persistence across mixed actions:** isolated save-fault tests exist, but mixed real-time player sequences may still reveal ordering defects. | Add scenario connecting potion/combat, NPC witness, gear, world travel and profile switch with interrupted saves and immediate reload. Preserve legacy schema 1 and partial reward receipt protection. | Crash/restart and physical travel using real platform file IO. |
| P1 | **Playable quest affordances in nine realms:** authored evidence and loops are source-verified, but objective discoverability and misleading empty feedback are not established. | Audit every objective for a reachable lead, physical target ID, condition, refusal/alternate action, journal result, and return reaction; test master-to-runtime key parity. | Geometry, routes, occlusion, navmesh, trigger activation and both return branches. |
| P1 | **Combat and boss readability:** distinct attacks exist as rules, but victory needs perceptible warnings, counterplay, hit recovery and adaptive pacing. | Verify warning-to-impact tables, status ownership, equipment styles, guard/dodge input budget, recovery and repeat attempts across all named encounters; produce actual cue/animation IDs, not generic placeholders. | Physical hitboxes, animation interrupts, gamepad timing, audible/visible attack tells and difficulty tuning. |
| P1 | **Resident identity and recurring society:** finite wages, households, memory and rumor rules exist; source tests cannot prove the dialogue feels natural. | Author distinct visit/return/absence/denial lines by stable resident ID, add subtitle reading-time and witness provenance checks; keep non-observers uninformed. | Performed, staged encounters and immersive audio/animation, schedules and crowd nav. |
| P1 | **Cinematic/world-production gap:** 56-scene scripts, city guides and reference mesh staging are not actual playable productions. | Ensure every shot and object has stable actor, camera, sound, subtitle, skip and state-handoff IDs; audit missing real assets and deferred substitutes in a generated manifest. | UE asset import, animation, scenes, lighting, audio mix, accessibility and packaging. |
| P2 | **Performance and accessibility readiness:** distance/cadence optimizations and settings exist; no measured target-platform costs or on-screen readability. | Set test budgets and telemetry hooks for actor counts, NPC ticks, AI latency opt-out, sound events, save retries, and large-text layout; add deterministic dense-city cases. | Profile frame time, memory, widget scaling, captions and reduced-motion on real hardware. |

## Evidence boundary and next gate

The latest PR #74 claims 380 Python tests and 93 strict portable C++17
executables with 76 dependency checks; those figures are source-only and may
change as the branch progresses. The first-PC work remains UE 5.8 UHT/Editor
compile, actor construction, live save IO, all nine realm traversal, imported
assets, controller usability, measured performance and a packaged demo.

**Next pre-PC focus:** the P0 input/modal-state and first-15-minutes acceptance
matrix, with explicit failure tests before changing more gameplay. Avoid new
systems or narrative sprawl until existing mechanics are understandable,
composable and independently testable.

## Input follow-up: actual analog-event capture

The initial trigger policy did not guarantee that an analog event reached the
rebinding menu. Source now overrides the Unreal UUserWidget analog input handler
and routes a high-value left/right trigger press to the existing control-capture
logic only while rebinding is armed. Portable threshold tests reject low press,
inactive capture and non-trigger axes. This is statically wired; platform input
routing and real controller feel remain an Unreal acceptance task.

## Living encounter ecology implementation — 17 October

A focused gameplay audit found authored creature routines that did not reach
navigation when the player was absent. The idle route now enters the normal
navmesh-backed approach/return path, preserving collision and obstructed
movement safety. Ordinary non-boss enemies also respect one active nearby
attack warning and circle toward a lateral opening instead of presenting
overlapping strikes; authored boss/elder patterns remain exempt.

This adds gameplay behavior in native C++ rather than another simulated
checklist entry. Portable behavior tests and source wiring checks are included
in CI, but UE actor ticking, actual world navigation, player perception and
performance remain first-PC acceptance work. Follow-up should test crowded
species mixes, LOS cancellation, Fold interruption and both equipped builds.
