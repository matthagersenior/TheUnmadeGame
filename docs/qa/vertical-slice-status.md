# THE UNMADE — First playable milestone tracker

**Tracked revision:** 2026-10-09. Source of truth: GitHub `main`. This file records **source readiness**, not an engine-tested playable game.

| Milestone | Source | Evidence / missing verification |
|---|---|---|
| Unreal C++ project, game mode, player camera | Present | No Unreal Editor compile or launch yet |
| Three road-linked villages, six lore sites, two story gates | Present | No verified Unreal screenshot, lighting, collision, or traversal |
| 48 named NPCs in three villages, personal memory and regional routines | Present | Native rules CI; Unreal actor movement and save/load unverified |
| Optional locally hosted language model | Adapter present, off by default | Game fully functions without model by design; model inference untested |
| Three-tier fracture with Strain and persistent Rewrite | Source present | Native rules CI; Unreal timers/visuals/collision unverified |
| Melee, guard, Stalker and Watcher | Source present | Native rules CI; animations, projectiles and navigation absent |
| Optional lexicon clue chain | Source present | Native rules CI; Unreal UI/interaction unverified |
| Branching dispute + two route outcomes | Source present | Native rules CI; physical access and story presentation unverified |
| Optional market-to-shelter supply errand | Source present | Native rules CI; no pickup prop or inventory UI yet |
| Coherent complete journey | Native C++ integration regression test | No full Unreal level traversal test or packaged executable |
| Original world art, animations, voice, dialogue UI | Not implemented | Unreal-equipped creator/dev host required |
| Authored maps, checkpoint/respawn, save migration | Not implemented | Windows Unreal toolchain/asset work required |
| Android remote playtest | Not provisioned | Approved, cost-capped GPU host and authenticated Pixel Streaming required |

## Verification procedure

GitHub Actions compiles native C++17 tests covering fracture rules, NPC decisions and movement, combat, language discovery, the branching story, and a combined no-AI scenario. It also runs Python source-contract tests. Those are valid source-level results, **not an Unreal build pass**.

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
5. Complete user-facing story dialogue, genuine trade and world reactions, durable versioned saves, accessibility and controller testing.

## Source artifacts

- [Combat QA](combat-prototype.md)
- [Language discovery QA](lexicon-prototype.md)
- [Branching story QA](branching-conflict.md)
- [Fracture QA](fracture-prototype.md)
- [Windows host acceptance](windows-unreal-host.md)
