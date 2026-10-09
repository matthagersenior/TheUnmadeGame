# THE UNMADE — Engineering status

**Snapshot:** 2026-10-08. Source repository: `matthagersenior/TheUnmadeGame` (`main`).

## Repository source present
- Approved design v0.3 and eight-task first-playable implementation plan.
- Unreal C++ descriptor, game/editor targets, module, game mode, third-person camera, movement, temporary input mappings and temporary visible character mesh.
- Runtime-generated hub blockout with ground, buildings, anomalous marker and five placeholder NPCs (no authored `.umap` exists).
- Individual NPC memory component with witness vs attributed rumor, deduplication, reactions, bounded observations, and identity-bound snapshots.
- Line-of-sight/distance-limited witnessed actions (aid, anomaly test), and nearby one-hop rumor propagation on an eight-second timer; proximity-gated aid cannot repeatedly reward the same NPC.
- Narrow `USaveGame` container persisting NPC snapshots to one prototype slot on actions; restoration when hub constructs.
- C++ automation source testing event provenance, rumor upgrade, snapshot idempotence and distinct fear/trust.
- Python static checks and source-contract tests under GitHub Actions.

## Optional local LLM source (new, unverified)
- One shared Unreal GameInstance subsystem targeting only local Ollama /api/chat.
- NPC-scoped, bounded memory context; attributed rumors; strict dialogue-output decoding and deterministic fallback.
- NPC E interaction calls the subsystem; local AI disabled by default in config. No paid API use or externally hosted inference.
- Added Unreal automation tests for response parsing and memory provenance; these tests are **not** executed in GitHub static CI.\n- Added a local-only Ollama model readiness probe with four offline mock tests (GitHub static CI exercises the probe tests, but does not prove Ollama exists on the creator's machine).
- Unreal C++ build, actual Ollama response, CPU/GPU performance, and bundled llama.cpp packaging **remain unverified and incomplete**. See `docs/architecture/local-npc-llm.md`.

## Fracture prototype source
- Added a target with visible collision states and a clue mesh using built-in Unreal cubes.
- F=Glimpse, Q=Fold, R/T=two-confirmation permanent Rewrite (valid target required).
- Shared native C++ fracture logic has an executable C++17 GitHub CI test.
- World variant and Strain now coexist in the existing prototype save data alongside NPC memories.
- **Not verified** in Unreal Editor/Windows GPU host: compilation, geometry, collision, visual state timing, long-running save behavior.

## Still absent or unverified
- Unreal Engine compile, map spawn, player collision, actual NPC perception or snapshot save/load at runtime: **not tested without UE/GPU host**.
- Authored world visuals, character customization, realistic NPC routines, coherent combat abilities, actual Glimpse/Fold/Rewrite, rich social dialogue, lexicon, full save semantics and quests.
- Windows packages, real device/controller acceptance testing, cloud server, secure Pixel Streaming Android session.

**Static CI can confirm repository/source contracts only; it cannot confirm Unreal C++ compiles or that gameplay works.**

## Next verification gate
Use a licensed Windows Unreal Engine 5.8 installation with enough GPU/VRAM. Open `TheUnmadeGame.uproject`, generate Visual Studio project files, compile Editor, launch standalone/PIE, and verify the runtime hub loads with a visible character and walkable floor. Run `Scripts/run_ue_tests.ps1 -UnrealRoot <root> -TestFilter Unmade`; investigate failures, record raw logs and tests. In PIE, verify E/H/F near NPC/marker, recall after quitting and restarting, distant NPC non-omniscience, and obstruction line-of-sight.

An unexecuted Windows-host diagnostic and acceptance checklist are in `Scripts/check_unreal_host.ps1` and `docs/qa/windows-unreal-host.md`.

## Infrastructure
No paid hosting authorized or provisioned. The creator currently has Android but no PC. Interactive remote playtesting requires a separate GPU Windows host and explicit budget approval.
