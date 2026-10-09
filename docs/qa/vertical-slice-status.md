# THE UNMADE — First playable milestone tracker

**Tracked revision:** 2026-10-09. Source of truth: GitHub `main`. This file records **source readiness**, not an engine-tested playable game.

| Milestone | Source | Evidence / missing verification |
|---|---|---|
| Unreal C++ project, game mode, player camera | Present | No Unreal Editor compile or launch yet |
| Three original graybox villages + two new realm footholds | Source present | Realm gates and outpost geometry are not editor-compiled or traversed |
| Three named bosses with different actions and phase changes | Source present | Native C++17 tests pass; hit timing, visuals and combat are Unreal-unverified |
| Six crafting recipes, three professions and finite village economies | Source present | Native test coverage; shop proximity, workstation use and UI are engine-unverified |
| Three faction chronicles and two frontier arcs | Source present | Native tests; NPC conversation and save persistence unverified inside Unreal |
| Inventory and equipment (60 authored items) | Source present | Native tests; models, combat effects, trading and inventory UI unverified |
| 64 named NPCs across three villages and two frontier outposts, memory and routines | Present | Native rules CI; Unreal actor movement and save/load unverified |
| Optional locally hosted language model | Adapter present, off by default | Game fully functions without model by design; model inference untested |
| Three-tier fracture with Strain and persistent Rewrite | Source present | Native rules CI; Unreal timers/visuals/collision unverified |
| Melee, guard, Stalker and Watcher | Source present | Native rules CI; animations, projectiles and navigation absent |
| Optional lexicon clue chain | Source present | Native rules CI; Unreal UI/interaction unverified |
| Branching dispute + two route outcomes | Source present | Native rules CI; physical access and story presentation unverified |
| Optional market-to-shelter supply errand | Source present | Native rules CI; no pickup prop or inventory UI yet |
| Coherent complete journey | Native C++ integration regression test | No full Unreal level traversal test or packaged executable |
| Original world art, animations, voice, dialogue UI | Not implemented | Unreal-equipped creator/dev host required |
| Authored maps and checkpoint/respawn | Not implemented | Windows Unreal toolchain/asset work required |
| Append-only inventory catalog save migration | Source implementation | Previous save files must still be tested in Unreal |
| Android remote playtest | Not provisioned | Approved, cost-capped GPU host and authenticated Pixel Streaming required |

## Verification procedure

GitHub Actions compiles native C++17 tests covering fracture rules, NPC decisions and movement, combat/bosses, crafting, economies, factions, frontier stories, language discovery, the branching story, and a combined no-AI scenario. It also runs Python source-contract tests. Those are valid source-level results, **not an Unreal build pass**.

On a provisioned Windows Unreal GPU host, run `Scripts/check_unreal_host.ps1`, build the Unreal Editor module, run `Scripts/run_ue_tests.ps1 -TestFilter Unmade`, enter Play-in-Editor, test all interactions and alternative saved choices, record screenshots and logs, then package the Windows build. The Unreal gate stays **unverified** until those steps actually succeed.

## Design guardrails

- Game progression, NPC choices, combat, clues, saves and quest branching must not require AI or external services.
- NPC memory distinguishes firsthand evidence from hearsay. World events do not broadcast magically to all residents.
- Permanent changes must preview consequences, require confirmation, validate saves and persist through reload.
- Do not invent compiled gameplay, passed Unreal tests, screenshots, purchased hosting, or shipped builds.

## Immediate next implementation tasks

1. Obtain an Unreal-equipped Windows host (no paid provision without explicit provider and cost-cap approval) and fix compiler/toolchain/engine issues revealed by the first build.
2. Replace the generic Entry world with an authored map containing connected explorable spaces and usable movement/collision.
3. Add actual health HUD, attack/guard animations, projectiles and StateTree/NavMesh enemy behaviors.
4. Build character appearance/profile selection, contextual quest log and player checkpoint/respawn.
5. Replace basic source trade/profession/faction interactions with robust user-facing dialogue, menus, balanced inventories, durable versioned saves, accessibility and controller testing.

## Source artifacts

- [Epic world and expansion QA](epic-world-systems.md)
- [Combat QA](combat-prototype.md)
- [Language discovery QA](lexicon-prototype.md)
- [Branching story QA](branching-conflict.md)
- [Fracture QA](fracture-prototype.md)
- [Windows host acceptance](windows-unreal-host.md)

## Tenfold source status — October 9, 2026

| Feature | State | Engine verification |
| --- | --- | --- |
| Ten deterministic signature powers and distinct effects | Native + Unreal integration source | Unreal runtime uncompiled; no ability/VFX demo |
| Ten authored five-chapter questlines and named witnesses | Native model + E/F-key hook | Quest logic not tested in-game; no finished journal |
| Six Confluence chambers and 120s dual-power puzzles | Native model + physical marker source | Chamber collision, casting and witness playtest required |
| 60 named items and sixteen earned rite/confluence relics | Native item tests + saved achievement integration | Inventory/equipment UI and save migration unverified in Unreal |
| Oath betrayal/restitution, debt exhaustion, branching law/history consequences | C++ source and tests | Live NPC/world reactions, difficulty and pacing unverified |
| Full nine-realm RPG, audio, cinematics, authored levels | Not completed | Six other realms remain concepts; three have source footholds |

This is a **meaningfully expanded code foundation**, not a finished game.
See [Tenfold QA](tenfold-prototype-qa.md).

## Bellwold — The Second Night
A saved four-stage return-visit story with physical evidence, Sorin/Ivera
testimony, alternate public structures and exclusive rewards.
See [specific acceptance tests](bellwold-afterlight.md). Not yet playtested.
