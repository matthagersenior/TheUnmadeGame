# Gameplay parity and completion standard

**Milestone:** v21 source quality gate  
**Purpose:** keep every realm and every player-facing system to the same minimum quality bar while the game is developed without Unreal Editor access.

The maintained [nine-realm coverage ledger](realm-coverage-ledger.json) tracks story, locations, quests, NPC rosters, enemies, bosses, equipment, abilities, environment prototypes, consequences, and CI evidence against stable source records. Its validator runs in static CI. The source gate is a consistency check over authored material. It cannot verify a playable build, rendered interface, real timing, feel, accessibility in use, or save behavior in Unreal. Every engine-only item below stays open until tested in the Editor and a packaged build.

## Realm-by-realm authoring floor

Every one of the nine realms must have all of the following before its content can be called authored-complete:

- A distinct identity, dilemma, physical mechanism, and at least three player verbs.
- A readable hazard tell, counterplay, and a safe recovery route.
- Three inspectable pieces of evidence, a named local witness, and an interpretation that preserves uncertainty where intended.
- A main quest with two explicit choices, visible consequences, and distinct return reactions from affected residents.
- A distinct optional arc with multiple steps, meaningful care/truth resolutions, a return visit, and an isolated save contract.
- A boss or guardian whose identity and encounter purpose fit that realm; enemies need readable tells, counterplay, recovery, and role variety.
- Day/night, ordinary life, sensory cues, safe-access cues, economy, and local customs that can be understood without audio alone.
- Four production handoff identifiers and a concrete test case for both authored choices.
- Main progression must not depend on optional errands, grinding, external services, or AI.

The new CI test Scripts/tests/test_gameplay_parity.py checks that all nine realms appear in the same canonical order across authoring tracks, key identity/quest fields are distinct, optional and main arcs have branch/return material, and every atlas entry has evidence, access cues and sensory coverage. If a realm loses this baseline or data drifts out of order, Python CI fails.

## Standards shared across the whole game

| Area | Standard to carry through every feature | Evidence needed before calling it complete |
| --- | --- | --- |
| Menus and HUD | Every action has a visible label, useful feedback, consistent navigation, keyboard/controller access, focus visibility, readable text, and a safe cancel/back path. Important state is not conveyed by color or sound alone. | UE widget/controller pass at supported resolutions and input methods; all actions and settings exercised. |
| Cinematics and dialogue | Establish who, where, and why; subtitle all speech and meaningful sound cues; provide pause/skip and readable captions; keep choices explicit and consequences legible; never present a concept shot as gameplay. | Imported sequence, timing/caption review, muted-audio review, controller skip/pause pass, and narrative spoiler review. |
| Exploration and quests | Every objective has a discoverable clue, clear next action, useful journal state, recovery from a missed/failed interaction, and an optional route that stays optional. No progress by invisible trigger or empty checklist. | Walkthrough from clean save for each branch and return visit; verify landmarks, collision and journal behavior in-engine. |
| Abilities and puzzles | Explain what an ability changes, show target/range/cost, provide immediate feedback and a non-punitive retry, and support a readable alternative for timing/sensory barriers. | Each ability exercised on intended and unintended targets, with timing, collision, failure recovery and accessibility reviewed in-engine. |
| Enemies and bosses | Distinguish roles and silhouettes; telegraph before damage; provide counterplay and recovery; scale pressure without unavoidable damage or mandatory grind. Boss phases must change decisions, not only health. | Recorded combat passes at intended difficulty, including low-health recovery, repeat attempts and alternative builds. |
| Economy and crafting | Show costs, ingredients, outputs and limits before commitment; keep rewards bounded and non-repeatable where intended; avoid softlocks and currency/material traps. | Clean-save progression and economy simulations plus in-engine vendor/workstation, receipt, failure and reload checks. |
| Relationships and memory | Residents remember only events they saw, were told, or can infer; relationships change through understandable actions; consent and refusal remain meaningful. | Witness/rumor tests in source and in-engine multi-NPC scenario covering distance, time, reload and contradictory accounts. |
| Saves and consequences | Preview irreversible outcomes; confirm deliberately; save independent optional content independently; reject corrupt data safely; preserve prior good saves during recovery. | Repeated cold save/load, interruption, migration, corruption and branch-isolation tests in PIE and standalone. |
| Accessibility and performance | Provide text scale, contrast, remapping, reduced motion/flashes, captions, camera controls, stable routes and clear prompts. Maintain performance budgets without removing required feedback. | In-engine settings audit, keyboard/controller-only walkthrough, reduced-sensory pass, and measured target-hardware profiling. |

## Project status at v21

**Source/CI:** broad deterministic gameplay rules and authored content exist for nine realms. CI can validate their data consistency and source-level behavior.

**Still unverified:** Unreal Header Tool and Editor compilation, authored/imported maps and assets, collisions/navigation, rendered menu focus, combat feel, cinematics/audio, real save/load, accessibility in use, performance, and a complete playable campaign.

The repository has no packaged playable release. A green source CI run means the checked source contract passed; it does not mean the game has been built or played. See [PC-independent readiness](pre-pc-readiness.md), [first playable milestone tracker](vertical-slice-status.md), and [first-PC build process](first-pc-one-click-production.md).

## PC-independent next work

1. Keep the parity test green as authoring changes; fix any realm that falls below the floor instead of weakening the check.
2. Continue source-level interaction, save-isolation and economy edge-case audits, and write testable acceptance criteria beside each feature.
3. Prepare UI focus maps, subtitle/caption sheets, combat tell tables, and one full clean-save walkthrough for each realm's two decisions.
4. When an Unreal host becomes available, begin with compilation and one complete Bellwold return visit, then test controller/journal flow and combat/discovery before scaling to all nine realms.
