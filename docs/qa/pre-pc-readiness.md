# THE UNMADE — PC-independent readiness ledger
**As of 2026-10-10 | This is a live engineering backlog, not a release-complete certification.**

## Evidence codes
- **NATIVE TESTED** — deterministic C++17 tests run under GitHub Actions (not Unreal).
- **SOURCE WIRED** — UE C++ and static contracts exist; compiler/gameplay unverified.
- **ENGINE REQUIRED** — this cannot be certified without Unreal Engine Editor/Windows test host.
- **CONTENT MISSING** — authored levels/performances/visual UX are still required; a PC alone does not create them.

## Already available before acquiring a PC
| System | Source preparation | Remaining approval gate |
|---|---|---|
| Third-person pawn, camera, travel | Runtime-generated prototype hub and gates (SOURCE WIRED) | ENGINE REQUIRED: actual map/boot/collision |
| Adaptive NPCs | 64 distinct identifiers; witness vs rumor, decisions and limited movement (NATIVE TESTED + SOURCE WIRED) | ENGINE REQUIRED: crowds, NavMesh and real animations |
| Player identity | Eight feature values, validated name, immutable origin, backward-compatible save, blueprint access (NATIVE TESTED + SOURCE WIRED) | CONTENT MISSING: UMG creator, rig, hairstyles, face/hair/skin materials |
| Reality and signature disciplines | Native Glimpse/Fold/Rewrite, ten rites and six confluences (NATIVE TESTED + SOURCE WIRED) | CONTENT MISSING: authored hazards/VFX, controller affordance, in-world puzzle play |
| Combat and recovery | Two enemy styles, three boss patterns, guarding, deduplication and 75%-health safe respawn (NATIVE TESTED + SOURCE WIRED) | ENGINE REQUIRED: hit VFX, stamina/difficulty tuning and real play |
| Bellwold, Paperhaven, Crossings | Three populated graybox settlements and returning Bellwold Afterlight (SOURCE WIRED) | CONTENT MISSING: authored map, quests, menus, ambience |
| Saltwake and Cinderhold | Distinct frontier story, post-choice paths, timed flood/heat and localized NPC memory (SOURCE WIRED) | ENGINE REQUIRED: collision/traversal and realistic hazard feel |
| Persistent rewards/economy | 60 named items, finite rewards, professions, receipts and multiple save subsystems (NATIVE TESTED + SOURCE WIRED) | ENGINE REQUIRED: entire cold-save/reload/failed-write acceptance |
| Nine-realm atlas | Distinct future scripts, witness names, controls, paths and ethical choices (NATIVE TESTED) | CONTENT MISSING: six graybox realm chunks are now source-spawned, but they are **not authored or engine-tested levels** |
| Windows host bootstrap | `Scripts/first_pc_build_and_test.ps1` with preflight, build and automation, per-attempt logs | ENGINE REQUIRED: actually running it on a supported host |

## Pre-PC backlog (can continue now, not yet complete)
1. **SOURCE WIRED / NATIVE TESTED:** irreversible F7/F8 previews now require a second same-arc, same-ending press within six seconds. The pending remaining gate is UE controller, UI and save-failure playtesting.
2. **SOURCE WIRED / NATIVE TESTED / ENGINE UNVERIFIED:** Six different world-timed dangers apply health injury or nonlethal reality strain. Six optional third-chapter Echo investigations add two proofs and a verified local witness per realm, plus source-world consequences. Now prioritize distinct enemies, advanced ability interaction, professional dialogue/cinematic/audio content and true navigation. No engine playtest yet.
3. Improve save transaction integrity across combat health/checkpoints, quests, NPC memory, equipment and worlds; test bad arrays, failed writes and repeated saves.
4. Author navigation and controller-accessible UI interaction specifications, first-party feedback, item/quest/inventory states, audio cue sheets and localization text independent of a PC.
5. Create source-verifiable quests and world consequences per realm, not merely names, stat counters or 3D placeholders; validate performance budgets and progression without infinite grinding.
6. Add automated release/quality gates for architecture coverage and progressive content migration, keeping original player-first promises intact.
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
