# THE UNMADE — 11-city assembly factory and honest production ledger
**10 October 2026 | Nine realms · 11 canon cities · 82 known residents · 10 rites · 54 primary authored quest steps**

## Why the project needed this

Installing Unreal is a delivery task, not a substitute for producing a huge RPG. The player's criticism is correct: the city layouts, 82 believable residents, actual 10 performed powers, quest-stage UI, cinematic performances, sophisticated AI routines, and miles of level art remain substantial unfinished production.

The factory introduced in PR #28 (follow the merged main revision) attacks **repeated setup and creative reentry** first. It is designed to produce an immediately recognizable *outline* of every city and associate real canonical people, quests, abilities, clues and optional story tasks with **physical reference positions** in one Unreal Editor staging map. All source IDs remain unmodified.

This system **does not** claim that the marker actors are gameplay quests or finished world geography. All 402 actors are noncolliding reference cubes in `Unmade_CityAssemblyStaging`, with original native quests and `UnmadePrototypeNPC` save authority unaffected. The host Unreal Editor execution is not yet verified.

## The 11 individually authored city visual grammars

| Realm | City | Staging profile / recognizable silhouette | Existing residents linked |
| --- | --- | --- | ---: |
| Threefold Reach | The Crossings | Braided footbridges, mile marker, roadstone and public ledger court | 16 |
| Threefold Reach | Bellwold | Empty bell yoke, refuge meal hall, witness steps and uncounted cup | 16 |
| Threefold Reach | Paperhaven | Chalk index vaults, dry garden, two-faced seal press and archive scale | 16 |
| Widowed Rain | Saltwake | Two dry piers, keel roofs, harbor council and sealed chart | 8 |
| Hearth Beneath | Cinderhold | Subfoundation furnace, vent ribs, communal ovens and shadow stack | 8 |
| Tidal Ledger | Drevlach | Ninth Due Exchange, inverted clock, accounting bridges and cradle seal | 3 |
| Sky Below | Orravane | Counterweighted hanging terraces, seventh lectern and choirs' support rail | 3 |
| Cinder Spine | Vathless | Quarry-wall housing, structural supports, mason mark and smaller hand | 3 |
| Hundred Unlived | Eillun | Paired address doors, anonymous lane and worn unregistered household | 3 |
| Orchard of Kings | Tharniv | Empty crown plinth, petition tree, circular hearing space and common harvest | 3 |
| First Absence | Auvren | Twin thresholds, inside-out rooms and witness plateau | 3 |

The eight **individually chosen silhouette modules per city** are authored in `Authoring/city_assembly_kits_v1.json`, with coordinates and UE centimeter-scale cubic massing. Each city receives eight residential massing guides plus four straight reference streets; **these are not pathfinding paths or polished believable street plans**. Refine them with actual place-specific materials, architecture, housing types, food sources, slopes and local economies before calling a city realized.

## Exact linked staged production breakdown

| Scene-guide class | Count | Source of identity | What it is not |
| --- | ---: | --- | --- |
| Distinct civic/silhouette modules | 88 | 11 authored city kits × 8 | Complete buildings |
| Open-air road reference geometry | 44 | 11 centers × four conceptual street strips | Traversal/navmesh |
| Modular residential block massing | 88 | 11 centers × eight shell positions | Occupied living households |
| Named resident actor guides | 82 | Unmodified `resident_return_encounters.json` IDs | Fully animated NPC AI |
| Principal quest-step guides | 54 | Nine six-beat authored main quests | Confirmed runtime quest completions |
| Power/rite site guides | 10 | Unmodified `world_content_pack.json` ability IDs | Working casts and VFX |
| Physical lore evidence guides | 27 | Nine lived-universe sets of three | Collected or validated evidence |
| Optional side-story guides | 9 | Nine explicitly *not yet built* LUQ contracts | Created quest/save state |
| **Total** | **402** | Stable source IDs and read-only reference labels | **Not a playable game** |

The **3 + 8 + 6 + 54** uneven city population/quest distribution is deliberate. There are 16 existing named records in each Reach city, eight in each frontier city, three in each later realm. All nine realm main quests' six beats are represented: for Threefold Reach arrival/return goes to Crossings, investigation/care to Bellwold, mechanism/truth to Paperhaven; other realms' six beats stay in their canonical settlement. This is a layout rehearsal only, not an imposed fast travel route.

`Source gameplay → 11 city kits → bounded deterministic 402-actor guide plan → guarded Unreal Editor map` is the production flow. The plan is reproducible on any clone and actor labels are source-stable; artist-made replacement assets in the same map can be layered incrementally.

### Offline and first-PC commands

```shell
python Scripts/build_city_assembly.py --check
python Scripts/build_city_assembly.py --output city-assembly-preview.json
python Scripts/unreal_editor/stage_city_assembly.py
```

When the Windows PC has Unreal 5.8 and Visual Studio C++ correctly installed, double-click `START_THE_UNMADE.cmd`. The prior strict build/preflight and Windows automation test run still gate all editor imports. The existing seven DataTables, 52 quest/cinematic guides and 90 lived-world work-order guides are followed by the new `/Game/UnmadeProduction/Maps/Unmade_CityAssemblyStaging` map with **402 noncolliding, non-authoritative, stable-label proxy cubes**. The receipt must prove 402 total before reporting success.

A user can explore the city-planning map with the Editor viewport once the Windows importer has been engine-validated, but should use **the separate real prototype gameplay map** for real Interact, combat, abilities and save/load testing.

### Named gaps: the next build order

1. **Physical first vertical slice:** In Bellwold replace placeholders with a navigable refuge, tower, meal and witness workspaces and a readable approach for Hessa; wire existing native Witness Echo and Afterlight states to a real UMG codex/dialogue UI. Test the real two mutually exclusive outcomes on separate saves.
2. **Distinct people:** Deepen supporting residents beyond 13 lead dossiers; authored home/work/sleep positions, relationships, visible consequences, 3–5 conversation moves per person, knowledge provenance, posture and speaking direction. Preserve their 82 stable persistent IDs.
3. **Ability actor/feedback layer:** Make each of ten powers visible with animation/VFX/audio/accessibility cues, target requirements, cancel/failure and bounded costs. Test that original native rite mastery/save flags alone control progression.
4. **Full quest staging:** Replace the 54 guide markers with in-world sightlines, physical evidence tasks, dialogue options, confirmable irreversible decisions, durable save/rollback, alternate safe pathways and genuine return conversations. Do not reimplement progression via marker counts.
5. **World expansion:** Use the same template for all eleven locations, but ensure each has lived-in culture, ecology, architectural signature, sound motif, hazards, local trade and family relations. Beyond the Reach, prove earned Atlas travel, loading, NPC navigation and quest locality.
6. **Production polish:** Professional voice direction, cinematics in Sequencer, music, accessibility, localization, performance, packaged PC build and actual game testing.

## Fidelity and future source changes

The canonical city identities, 82 save-indexed resident IDs, 10 rite names, real quest-state guards, two final variants and nine OPEN mysteries remain fixed. Every future scene change must update the versioned kit and rerun `python Scripts/build_city_assembly.py --check`. The adapter **refuses mismatched city names, realm drift, unknown rite IDs, malformed positions, duplicate source actors, invalid totals and altered NPC identity count before opening Unreal**.

Source check success confirms the preparation plan, **not** Unreal Python compatibility or final visual quality. The Bible update is an additional dated technical supplement with the exact 402-scene coverage and missing-work ledger, not a retroactive literary rewrite. Do not count any staging proxy as a playable level or score it as an implemented quest.
