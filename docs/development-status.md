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

## 2026-10-09 regional expansion

- Three separate villages, connected by two graybox causeways, with
  16 named residents each: 48 total, retaining the original eleven IDs.
- Each resident has independent memories, authored personal speech,
  role/temperament, village allegiance and local daily routes.
- Bellwold lantern and Paperhaven testimony errands are optional
  two-character interactions with durable quest state.
- Local completion can be witnessed, remembered and discussed;
  faction leaning influences personally known events, not omniscience.
- Source domain tests verify village identity, bounds, task ordering,
  persistence and cross-system simulation; GitHub source CI also runs
  Python contract tests.
- Unreal Engine compilation, physical road traversal, actual frame
  performance and visible gameplay remain **unverified**.

## 2026-10-09 earned gear and universe source milestone
- 28 uniquely named item definitions spanning equipment, potions and materials;
  each rarity, effect, cap and lore is authored without external inference.
- Finite claim markers link achievements to unique milestone rewards, once,
  in the existing schema-1 save. Corrupt snapshots or failed writes fail closed
  and potion Strain recovery shares the saved inventory transaction.
- Weapons/armor change combat damage, charms can reduce Glimpse cost,
  Waybreaker extends Fold, and the Unwritten Crown speeds normal Strain recovery.
- Starter kit, two village rewards, first enemy victories, six landmark finds,
  all-village progress, settlement choice and Veyl clues are wired to the
  Unreal prototype source. Bellwold's workshop permits gated mythic crafting.
- Nine authored future realms and their offline travel graph/attunement rules
  exist as data/tests. **They do not constitute playable continents.**
- Gear models, varied attack animations, gamepad equipment UI, inventory
  rendering, visible potion VFX, merchant economy, health checkpoint persistence,
  and full Unreal compilation are still absent or unverified.

## 2026-10-09 named bosses, regional economies, chronicles and realm footholds

- Three authored bosses with distinct native telegraphs, phase transitions,
  Hollow Bell shockwaves, Redacted Curator range control, Unfinished Pilgrim
  charging, interruption through Fold, finite unique trophies and victory
  persistence. Original enemy combat remains separate.
- Expanded catalog to 42 authored items. Appended stable IDs rather than
  renumbering previous equipment; source save logic accepts legacy 28+ slots.
- Six resource-consuming recipes and three finite professions; settlement
  discounts, safeguarded marks, one-time contract payments and transactional
  inventory/economy snapshots. Trade requires a merchant's proximity; crafting
  requires a specific workshop or scriptorium. UI is temporary keyboard text.
- Three named faction chronicles extend Bellwold, Paperhaven and the Crossings,
  with three witnesses each, evidence requirements, permanent outcomes, local
  price effects and one-time epic gear.
- Added Saltwake (Rain That Forgot the Sea) and Cinderhold (Hearth Beneath)
  graybox grounds, distinctive primitive architecture, returnable gate source,
  evidence objects and eight additional memory-bearing NPCs per outpost.
  Total named stable IDs in source: 64. Both have their own three-witness stories
  and uniquely earned relics.
- The initial nine-realm atlas remains; other six areas have no actor level
  source. Frontier simulation is still a small proof-of-concept and cannot
  be equated to finished continents or large open-world streaming.
- Combined native C++17 regression covers boss mechanics, crafting, faction
  stages and frontier journeys with restored snapshots. Python source
  contracts check Unreal hookup. **No UnrealEditor compile, in-engine
  checkpoint, actor traversal, full HUD, animation or verified live combat
  test was performed.**

## 2026-10-09 Tenfold Chronicles, testimony and Confluence saga

- Authored **ten** offline reality-discipline rules, each with five distinct
  chapter objectives, correct witness identity, firsthand evidence,
  proximity-limited ritual stones, finite Strain/cooldown, consequential
  protect/reveal decision, and nontrivial route/boss/quest mastery gates.
- Added physically spawned ritual stones in existing settlements and Saltwake,
  a persistent `UUnmadeTenfoldComponent`, keyboard actions and NPC
  interaction hooks. Mastery awards a unique named item once.
- Original gear catalog appended to **58** stable IDs including ten Rite
  rewards and six additional Confluence trophies. All append-only IDs;
  older inventory snapshots are accepted where structurally valid.
- Added six paired-discipline trials and world-space chambers. Both mastered
  skills must be cast in the same place within a timed window with actual
  witnesses. The first-realm capstone requires all ten and earlier trials.
- Actual first-pass physical effects: gravity launch, sound-based enemy
  interruption, knockback, timed witness bridge, common causeway, overlapping
  tower, temporary fighting/crafting/defense from alternate lives, temporary
  attack from tomorrow's strength with a delayed penalty, and researched
  nonlethal Hollow Keeper resolution. **All engine-unverified.**
- Saved irreversible oath breaking and a nontrivial reconciliation arc;
  NPC trust and rumors reflect actual witnessing of betrayal/restitution
  and a protective/revelatory choice. Unmake and debt choices modify future
  ability cost/duration, while the paradox historical choice affects the
  persistent layer.
- Added `TenfoldChronicle`, `ConfluenceJourney` native C++17 tests,
  extended the full offline cross-system scenario and Python integration
  contracts. GitHub CI validates code rules but does not compile Unreal.
- **Still missing:** Unreal engine build and runtime tests, visual abilities,
  input/menu polish, unique character animations, quest cinematics, actual
  rich geometry, complete environmental puzzles/alternate professions,
  production-quality balancing and length measurement.

## 2026-10-09 player-first consequence milestone

- Added engine-independent `UnmadeCommunityConsequences.h` with five
  non-farmable, authored settlement transitions calculated from the *existing*
  saved conflict, regional tasks, faction and frontier conclusions. Both
  shared institutions and revealed public evidence matter; neither is
  presented as an automatically evil moral choice.
- The Unreal prototype spawns 15 alternative graybox community-state markers
  (three per town), reveals only the state earned, and refreshes these after
  successfully persisted narrative decisions and on restored saves.
- Residents describe their own community's visible changes, not a magically
  known worldwide reputation. Existing merchant affordability reacts to
  the corresponding community trust; journals report local aftermath.
- Source unit/contract tests check the five towns, persistent fact
  derivation, invalid inputs, independence between towns and Unreal call
  sites. **This is source-level verification, not an Unreal playtest.**
- Established the player-first quality bar in
  `docs/design/2026-10-09-player-first-rpg-quality-contract.md`.

## 2026-10-09 Bellwold's Second Night

- Added a return-visit story, after Bellwold's earlier refugee task and
  completed Refuge Compact. Four stages: Hessa, physical evidence, Sorin
  (relief) or Ivera (census), then irreversible commitment at Hessa.
- Two mutually exclusive visible graybox structures and one-time epic
  items (the Ward of the Second Night or Lantern of Unredacted Names).
- Source NPCs react as firsthand witnesses or as rumor recipients. Saved
  chapter stages and choices restore atomically and guard corrupt snapshots.
- Appended items to existing indices: **60** named items. Added native
  model tests, item tests, integrated offline journey and source contracts.
- Unreal/UE editor build, actual cold/night hardship, animation, navigation,
  dialogue UI, accessible controls, campaign length and balancing unverified.

## 2026-10-10 Volume II story and canonical package expansion

- Codified the four user standards and ten signature mechanics in
  `docs/lore/living-world-continuity.md` with a dated release ledger.
- Added 9 uniquely authored Realm Aftermath arcs with care/truth evidence,
  different named witnesses, irreversible choices, atomic snapshots and
  persistent narrative effects in native C++17 source.
- Current runtime limit is **3 realms with footholds and only 5 populated
  communities**; the remaining six aftermath arcs are design data, not
  completed 3D destinations or actor conversations.
- The prior Bellwold Afterlight source episode remains the working template.
  The revised complete illustrated package is a separate downloadable
  dated artifact; GitHub docs are the canon source, not proof of in-engine art.

## 2026-10-10 Saltwake and Cinderhold source-integrated return adventures
- Shared nine-realm aftermath rules now require operating a distinct physical mechanism before either branch's evidence can advance.
- Unreal C++ actor source spawns Saltwake's Storm Sluice/barrier, Cinderhold's Heat Vent/seal, branch clues, exclusive downstream gates and selected post-choice traversable platforms.
- E uses nearest-interactable priority and line-of-sight. Previous frontier resolutions and named first/second witnesses gate the new stories; F7/F8 commitments are location-bound and irreversible.
- New nine-realm save arrays merge with existing version-1 slots; old missing optional fields initialize to zero. Invalid new arrays disable writes rather than silently resetting the previous outcome. Failed SaveGame writes restore the in-memory snapshot.
- Journal tracks returned chapters; frontier residents reference local visible consequences, while observed player choices remain witness/rumor events.
- GitHub Python source contracts and C++17 domain tests ran on CI; **the Unreal Editor has still not compiled this source** and no actual controller, navigation, flood/heat gameplay, geometry traversal or in-game save/load has been checked.
- Full illustrated lore package Volume III is tracked as a separately generated deliverable and should not be claimed from green source CI alone.
