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

## 2026-10-09 design addendum: AI-independent adaptive NPCs
- Player has explicitly confirmed that AI must be optional and unnecessary for gameplay.
- Added native C++ NPC decision policies (role, temperament, personal witness vs rumor, trust/fear) independently of all model or network libraries.
- Unreal NPC class exposes decision ID and trade-permission gate. Authored dialogue is the default. Five prototype residents have differentiated roles/traits.
- Prototype snapshot writing now clears its resident-snapshot collection before refreshing it, preventing accumulation from repeated saves.
- Pure C++17 decision tests run in GitHub CI; these are not Unreal Engine runtime tests. The existing Ollama adapter remains disabled by default.
- Still absent: trade transaction UI, actual NPC schedules/pathing, factions, and verified Unreal gameplay. See `docs/architecture/offline-npc-gameplay.md`.

## 2026-10-09 physical-combat prototype
- Added a pure offline deterministic combat policy: guarding, cooldown, one hit per swing/attacker, defeat, fracture exposure and two enemy behavior intents.
- New Unreal actor components and two graybox enemy actors use these rules; initial keyboard/gamepad attack and guard mappings plus Fold counterplay added.
- The Watcher currently applies a sight-gated ranged *placeholder* impact with no projectile animation. Navigation is straight-line swept movement, not pathfinding.
- These are source-level changes only; Unreal Editor compile, visuals, collision, controls, AI progression and actual gameplay have NOT been verified.

## 2026-10-09 first lexicon source
- Added an optional two-evidence language discovery rule tested without Unreal or LLM.
- Unreal Player Glimpse and archivist interaction supply distinct evidence and reveal a provisional inscription once both are present.
- New save evidence field merges with NPC and fracture snapshots; bad saved evidence is refused.
- Actual Unreal compilation, UI, gamepad interactions and persistence behavior still await an engine host.

## 2026-10-09 branching dispute and optional errand
- Added two mutually exclusive local settlement choices with explicit two-press warning and persistent separate choice state, without language or AI prerequisite.
- Runtime graybox opens shelter or archive passage according to saved choice; other route remains blocked.
- Optional pickup/delivery uses two saved stages; repeat delivery cannot award credit. Authored plain-text journal summarizes live story, clue, Strain state.
- New NPC-memory events grant trust or fear only to credible witnesses/rumor recipients through the existing non-omniscient event pipeline.
- Native C++17 tests cover choices, idempotence, optional activity, atomic restoration. Python CI checks basic Unreal source wiring.
- **Engine compilation, world collision, control input, and save/load gameplay remain unverified on a Windows Unreal host**.

## 2026-10-09 light NPC physical motion
- Five resident actors now have source for bounded swept movement driven by their authored offline decision policy: guard/scholar investigate, courier makes a small round trip, trusted residents approach, frightened residents withdraw.
- Deterministic pure C++17 steering (frame-time cap, radius stop, avoid invalid values) has a standalone CI test.
- Motion is a **graybox placeholder**: no NavMesh/StateTree smart-object paths, route scheduling, avoidance crowds, or animation blueprints yet. No Unreal runtime verification or paid AI.

## 2026-10-09 cross-system regression scenario
- A single native C++17 CI executable now exercises fracture discovery, authored language clues, both enemy tactics, guarding, fracture exposure, dispute preview/commit, supply errand, witnessed-vs-rumor NPC decisions, NPC steering, and snapshot restoration in sequence.
- This test is an **engine-independent simulation of deterministic rules**, not an Unreal Editor build, video, or proof that runtime actors interact correctly.

## 2026-10-09 immersive living-region source pass
- Expanded procedural graybox from 26 m to 52 m across and placed six original lore landmarks and outer ruin silhouettes in source.
- Implemented an offline twenty-minute accelerated day/night clock, NPC phase-based routine destinations, reactive atmosphere text and phase-sensitive point lights. NPC emergencies outrank routine targets.
- The player can inspect locations, track six unique discoveries and read the in-world clock in the existing journal. Landmarks respond to story outcomes.
- Added validated living-world save fields and a conservative periodic save interval. Old schema-v1 saves without these fields default safely.
- Native C++ world-cycle tests and Python source-contract tests; the cross-system offline scenario now includes world clock and exploration restoration.
- **Not verified in Unreal:** compilation, night lighting, walking routes, player controls, save behavior, animation, art quality, frame rate or sound. No remote Unreal machine exists.

## 2026-10-09 expanded district population and lighting
- Eleven stable resident IDs (six added in the outer districts), with individual home locations and retained identity-bound NPC memories.
- Authored NPC lines vary by personal identity, phase, and firsthand-versus-rumor evidence. Overheard gossip is proximity and line-of-sight gated.
- Added a movable engine directional light reacting to dawn/day/dusk/night; site lights and ambient cues transition with it.
- E interaction now selects the nearest NPC or landmark, not always NPCs over closer objects.
- Native C++ gameplay tests and GitHub source checks exist. No Unreal engine compile, soundscape, real animations, navigation meshes, weather system or in-engine demonstration yet.
