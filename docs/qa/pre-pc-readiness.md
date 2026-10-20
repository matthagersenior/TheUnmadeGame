# THE UNMADE — PC-independent readiness ledger
**Updated 2026-10-20 | Live engineering backlog; this is not a release-complete certification. Historical entries below retain their original dates.**

## Current October 20 supplemental source acceptance

[The pre-PC closure matrix](pre-pc-closure-matrix.md) now tracks each system's canonical source, native test, Unreal handler, physical test requirement and remaining work. The new standalone native campaign test enumerates 1,024 story state combinations, legal no-relic paths, both morning endings and recovery of saved states. Nine distinct realm crowns and 57 prototype motion clips have guarded source-only import plans. None of this proves Unreal engine compilation or a walkable, render-tested game.

## Current source handoff status (20 October 2026)

Main at the start of this status refresh was `8611e5d8da04c0e3ae99e16f16500b447ae7161f` (first-PC production package, PR #117). Its source CI passed; the Unreal Editor job was skipped, so there is no engine compilation or playable-game claim. The handoff generator produces nine realm dossiers and 669 source-linked work orders for main steps, clues, optional side stories, return residents and cataloged content. These rows identify production work; they do not certify runtime handlers, Unreal assets or playable scenes. The first-PC implementation plan tracks the remaining source, engine and content gates.

## Evidence codes
- **NATIVE TESTED** — deterministic C++17 tests run under GitHub Actions (not Unreal).
- **SOURCE WIRED** — UE C++ and static contracts exist; compiler/gameplay unverified.
- **ENGINE REQUIRED** — this cannot be certified without Unreal Engine Editor/Windows test host.
- **CONTENT MISSING** — authored levels/performances/visual UX are still required; a PC alone does not create them.

## Already available before acquiring a PC
| System | Source preparation | Remaining approval gate |
|---|---|---|
| Third-person pawn, camera, travel | Runtime-generated prototype hub and gates (SOURCE WIRED) | ENGINE REQUIRED: actual map/boot/collision |
| Adaptive NPCs and relationships | 82 persistent resident identities; witness vs rumor, decisions, continuity and limited movement (NATIVE TESTED + SOURCE WIRED) | ENGINE REQUIRED: final actor placement, crowds, NavMesh, animation and save/load |
| Player identity | Eight feature values, validated name, immutable origin, backward-compatible save, blueprint access (NATIVE TESTED + SOURCE WIRED) | CONTENT MISSING: UMG creator, rig, hairstyles, face/hair/skin materials |
| Reality and signature disciplines | Native Glimpse/Fold/Rewrite, ten rites and six confluences (NATIVE TESTED + SOURCE WIRED) | CONTENT MISSING: authored hazards/VFX, controller affordance, in-world puzzle play |
| Combat and recovery | Two enemy styles, three boss patterns, guarding, deduplication and 75%-health safe respawn (NATIVE TESTED + SOURCE WIRED) | ENGINE REQUIRED: hit VFX, stamina/difficulty tuning and real play |
| Bellwold, Paperhaven, Crossings | Three populated graybox settlements and returning Bellwold Afterlight (SOURCE WIRED) | CONTENT MISSING: authored map, quests, menus, ambience |
| Saltwake and Cinderhold | Distinct frontier story, post-choice paths, timed flood/heat and localized NPC memory (SOURCE WIRED) | ENGINE REQUIRED: collision/traversal and realistic hazard feel |
| Persistent rewards/economy | 60 named items, finite rewards, professions, receipts and multiple save subsystems (NATIVE TESTED + SOURCE WIRED) | ENGINE REQUIRED: entire cold-save/reload/failed-write acceptance |
| Nine-realm atlas | Distinct future scripts, witness names, controls, paths and ethical choices (NATIVE TESTED) | CONTENT MISSING: six graybox realm chunks are now source-spawned, but they are **not authored or engine-tested levels** |
| Windows host bootstrap | `Scripts/first_pc_build_and_test.ps1` with preflight, build and automation, per-attempt logs | ENGINE REQUIRED: actually running it on a supported host |

## Current source milestone: cross-realm parity (12 October 2026)\n\nA new Python CI gate verifies the nine realm authoring tracks stay aligned and that each realm retains distinct identity, main and optional quest choices, return reactions, evidence, sensory cues, accessibility cues and production handoffs. The shared acceptance standard now names consistent requirements for menus/HUD, cinematics/dialogue, exploration, abilities, enemies/bosses, economy/crafting, relationships, saves, accessibility and performance. See [gameplay parity and completion standard](gameplay-parity-v21.md). This verifies authored source only; it does not close the Unreal build or playtest gates.\n\nThe current menu source includes eight pages and 28 bound actions, with text scaling, high contrast, reduced flashes, controller prompts and remapping represented in settings. Controller focus, navigation, rendered layouts and actual effect of every preference remain engine acceptance work. Cinematic handoff plans and realm production references are source assets; rendered sequences, captured footage, final audio and imported world assets are not claimed complete.\n\n## Pre-PC backlog (can continue now, not yet complete)
1. **SOURCE WIRED / NATIVE TESTED:** irreversible F7/F8 previews now require a second same-arc, same-ending press within six seconds. The pending remaining gate is UE controller, UI and save-failure playtesting.
2. **SOURCE WIRED / NATIVE TESTED / ENGINE UNVERIFIED:** Six different world-timed dangers apply health injury or nonlethal reality strain. Six optional third-chapter Echo investigations add two proofs and a verified local witness per realm, plus source-world consequences. Now prioritize distinct enemies, advanced ability interaction, professional dialogue/cinematic/audio content and true navigation. No engine playtest yet.
3. Improve save transaction integrity across combat health/checkpoints, quests, NPC memory, equipment and worlds; test bad arrays, failed writes and repeated saves.
4. Author navigation and controller-accessible UI interaction specifications, first-party feedback, item/quest/inventory states, audio cue sheets and localization text independent of a PC.
5. Create source-verifiable quests and world consequences per realm, not merely names, stat counters or 3D placeholders; validate performance budgets and progression without infinite grinding.
6. Extend source gates to catch feature wiring gaps and run player-first acceptance scenarios across all realm choices; keep the new parity gate strict as content changes.
7. Maintain canonical lore and dated illustrated assets when the project crosses a real content milestone. Source tests cannot prove the visual package has been reissued.

## Work that requires a real Unreal-capable environment to finish
- Unreal Engine C++ compilation fixes, map setup, skeletal character assets, animations, terrain/lighting/weather/audio, navigation meshes, UMG accessibility and controller testing.
- Actual Glimpse/Fold/Rewrite collision and frame timing, enemy attack readability, world hazard intensity and physically accessible alternative routes.
- True save/load, inventory and NPC memory acceptance in PIE and standalone; long-duration memory/resource audits.
- Win64 game packaging, FPS/camera/stability tests and creator gameplay acceptance. Android phone testing requires optional, separately approved GPU streaming (no paid host authorized).

## First-PC execution contract
1. Install an appropriate UE version, Visual Studio C++ tools and Git LFS on a supported Windows PC **under the creator's control**.
2. Clone clean `main`, then execute `Scripts/first_pc_build_and_test.ps1 -UnrealRoot "<actual engine path>"`.
3. Upload the exact commit SHA and timestamped `TestReports/first-pc-*` logs if compilation/automation fails.
4. Fix all compile errors; do not call a green GitHub check a working game. Verify an actual actor spawn, movement, camera, hazards and two branching consequence paths.
5. Author level assets and real creator/HUD/animation/audio in Unreal, run all manual QA, then create the first genuine packaged Windows demo.

**No claim of PC-ready final completion is made while the pre-PC backlog and these engine/art gates remain open.**

## Later realm source milestone (October 10)
- Six source-spawned world chunks have split walkable floor and actual missing chasms, separate safe entry positions, atlas-connected gates with equipment-derived attunement requirements, three named witnesses per realm and two permanent source-visible route changes per realm.
- First visit selects and operates a physical trial; revisiting the initiator opens a different aftermath branch requiring another physical mechanism, local evidence, witness testimony and deliberate F7/F8 confirmation.
- Native C++ tests cover six independent branches, all alternate first routes, corrupt snapshots and atlas progression. No UE compilation or real traversability claim.

### Realm danger source milestone (October 10)
The six further realms each expose a bounded first-arc danger and a safe lane, with per-pulse identity to avoid duplicate injuries, guarding counterplay for physical strikes and saturated nonlethal strain for intrusive anomalies. Local residents can respond to their own observed warning. Native/model tests and static source integration passed in the PR after implementation; actual UE compilation and real traversal remain outstanding. See `docs/qa/later-realm-hazards.md`.

## Production content pack (10 October)
**SOURCE WIRED / NATIVE TESTED:** 9 culture art/audio/customs briefs, 18 named distinct outer people with daily voices and knowledge limitations, and 10 established rite execution briefs are available as editable JSON and automatically generated native actor header/Unreal CSVs/complete artist dossier. One content consistency check runs in PR CI and on the first Windows PC build. **ENGINE REQUIRED:** UE Header Tool, DataTable CSV import/reimport, animated people, VO, local translations, soundscapes and visible ability VFX.

## Optional mastered ability observatories (2026-10-10)
**SOURCE COMPLETE, NATIVE TESTED, ENGINE UNVERIFIED:** Six post-Echo dual-rite observatories use all ten original abilities through real, persisted casts, with separate physical bridge/island and testimony archive, saved deadline and corruption refusal. The source records no spontaneous teleportation, duplicate spell credit or repeatable loot. **Requires Windows UE:** UHT/compile, actual collision, camera/gamepad and audio/FX tests, spell-cooldown fairness and combat effects after crossing. See `docs/qa/realm-resonance-observatories.md`.

## Six guardian trials (2026-10-10)
**SOURCE WIRED / NATIVE CI GATE:** Six optional, distinct telegraphed enemy guardians are created only after local Echo completion, with a saved two-press nonviolent pact or ordinary combat defeat. Source NPCs register local consequences. **ENGINE REQUIRED:** Unreal Header Tool and Win64 editor compilation, real combat hurtboxes, navigation, timed audio and VFX, individual creature rigs, gamepad accessibility, save fault injection and full optionality validation. These are not finished boss fights.

## The Answer That Ate Its Question — final-boss source handoff
**SOURCE TESTED / ENGINE UNVERIFIED:** Nhal-Vey has two required actual health-based stages, a deliberately previewed and twice-confirmed world-future choice, nine branch-specific public realm records, a waking-up relocation that preserves the original save, and an optional post-victory modified rematch with different patterns. All earlier choices remain in their existing slots. **WINDOWS UE REQUIREMENTS:** compile UHT and actor source, test real boss damage/fold/guard, two controls, world-map geometry, accessible real telegraphs, proper branch-specified materials and skybox/audio layers, narrative cinematics, staged quest dialogue and full load/restart/save-failure end-to-end acceptance. See `docs/qa/final-boss-second-reality.md`.

## 2026-10-10 — cohesive five-act main campaign source spine
**SOURCE WIRED / NATIVE CI TARGET:** a five-act non-linear, main-quest-based trail from Bellwold/Paperhaven to Nhal-Vey, nine physical inspection markers, locally earned revelation text, road-aware journal advice and final-boss eligibility that needs no optional Echo quests or all-nine realm completions. Saves are derived from existing story flags (no migration). **UNREAL PC GATE:** usable HUD/objective tracker, quest marker collision, actual route readability, companion/VO presentation, narrative pacing, old-save end-to-end regression and boss access verification in PIE.

## 2026-10-10 — Volume IX cinematic explainer / source-fed Unreal handoff
**DONE BEFORE PC:** original 16-shot, 2:29.5 moving illustrated proof-of-style with captions and offline temporary VO, exact art-source/shot times, coherent nine-realm execution specs, editor CSV generator, CI tests for canonical story/realm IDs, source consistency and public ending-spoiler protection. **PC DEPENDENT:** UHT/Editor compile, DataTable import and reimport, actual Level Sequence camera cuts, VO recording, accessibility, 3D landscapes/rigs, physical hazards, gameplay and full story QA. Story movie clips are reference imagery, **not a playable engine trailer**.

## October 2026 — all-main-quest interaction and resident/scene factory handoff
**PRE-PC SOURCE IMPLEMENTED:** 9 regional quest paths / 54 physical/text dialogue/recovery steps, four final encounter steps, 82 persistent personally remembered NPCs with distinct returning lines, two reimportable Unreal CSVs, deterministic source-driven 52-reference authoring map and safe dry-run idempotent Editor script. Saves are optional, old-version1 compatible, and guarded against malformed writes; C++17/native CI provides domain tests. **WINDOWS UNREAL STILL REQUIRED:** compile UHT/Editor, import 54+82 DataTables, stage a distinct map via Python plugin, build real gameplay mechanisms/motion/UI, bind early placeholder tags, author every cinematic, test navmesh/collision/VO/controls, verify all nine physical stories and both finale worlds in PIE.
