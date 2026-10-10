# THE UNMADE — World Content Authoring / Minimal-Effort Unreal Handoff
**2026-10-10 | Nine realms, 18 later-realm named characters, ten reality abilities**

## Why this pack exists
The expensive failure mode in a solo Unreal RPG is having story rules, dialogue, model briefs, spreadsheets and editor assets disagree. This pack makes one well-typed offline JSON file the master for **production-facing authoring** while C++ source rules remain authoritative for quest commitments and save results. No paid service or cloud inference is needed for builds or play.

**Edit just** `Authoring/world_content_pack.json` for the nine cultures, the eighteen outer-realm character dossiers, and the ten canonical rite art/use briefs. Never manually edit the generated header, CSVs or dossier, and never change existing stable `npc.*` identifiers or enum IDs after saves have shipped.

Run on any Python 3.11+ host (including phone-supported terminal environments):
```bash
python Scripts/build_world_content.py --write
python Scripts/build_world_content.py --check
```
The generator checks the hard-coded `Realm` and `RiteId` enum orders, requires all eighteen registered witness IDs, validates three distinct characters per outer realm, and generates deterministically without packages/network/LLMs. Commit **all five outputs and the JSON** together. CI and the Windows first-PC script run `--check` and stop if there is drift.

## What is already prepared
| Artifact | Purpose |
|---|---|
| `Authoring/world_content_pack.json` | Single authored master, all 9 cultures / 18 people / 10 rites |
| `Source/TheUnmadeGame/Public/Authoring/UnmadeOuterPeopleData.h` | Compiled, deterministic lookup of each NPC's stable ID, name and day/night lines |
| `Authoring/generated/npcs_unreal.csv` | 18 people, voice/performance/wardrobe directions, knowledge limits, desire/fear |
| `Authoring/generated/realms_unreal.csv` | 9 distinct biome/culture/ritual/architecture/audio/activity briefs |
| `Authoring/generated/abilities_unreal.csv` | 10 rite environment cues, physical verbs, limitations and synergies |
| `Authoring/generated/WORLD_CONTENT_DOSSIERS.md` | Readable, complete output for performers, environment artists and writers |
| `Source/TheUnmadeGame/Public/Authoring/UnmadeAuthoringRows.h` | Blueprint-visible `FTableRowBase` row definitions for the above CSVs |

The existing 18 later-realm actor instances now use the generated names, voice-like daytime statements and separate nighttime statements. Earlier Threefold Reach/Saltwake/Cinderhold identities are preserved. This adds **authored dialogue**, not true lip-sync, full conversations or speech synthesis.

## First supported Windows PC / UE handoff
1. Clone the clean `main` branch and install the UE/Visual Studio versions verified by `Scripts/check_unreal_host.ps1` on a supported Windows PC. Existing `Scripts/first_pc_build_and_test.ps1` automatically checks content consistency and saves `content-validation.log` before C++ compilation.
2. Only **after successful Unreal Header Tool and Editor build**, import the generated CSVs as DataTable assets under `/Game/Authoring`. Select row structs:
   - `npcs_unreal.csv` → `FUnmadeNpcAuthoringRow`
   - `realms_unreal.csv` → `FUnmadeRealmAuthoringRow`
   - `abilities_unreal.csv` → `FUnmadeRiteAuthoringRow`
3. Save the three editor DataTables in Content Browser; when the JSON changes, regenerate and **reimport the same CSV files**, preserving asset paths/Blueprint references. Content Browser reimport and UHT compilation have not been performed in this environment.
4. Assign scene assets and animation/voice performances by stable NPC ID, not by display name; map a `DataTableRowHandle` keyed to the automatically generated `Name` column if building Blueprint NPC archetypes.
5. Attach map-specific geometry and cinematics to the source-tagged actors already created in `UnmadePrototypeHub`, `UnmadeLaterRealmWorld` and `UnmadeEchoQuestWorld`. Preserve their tags and state gates until you have a tested replacement or you will silently break saved quests.
6. Create localization tables keyed by stable NPC/site/ritual IDs. Day/night dialogue in JSON is English production source text; multilingual translations have not been produced.
7. Test two complete story resolutions plus an Echo side chapter per realm, all six dangers, save/resume, movement, camera, accessibility and the full `Unmade` UE automation filter. The import/export tests do **not** constitute a compiled level.

## Minimum-art-to-first-playable strategy
Keep the full world canon. **Do not attempt to hand-model nine continents before verifying one vertical slice.** The runtime already constructs primitive blockouts so the first host can check that gameplay APIs, collisions and quest logic work. Replace geometry incrementally:
- **Tier 0 source smoke:** the world boots; pawn moves; a keeper can be talked to; one first-visit choice changes a bridge; local hazard warns and stops; saves survive a reload
- **Tier 1 interactive graybox:** each realm gains a distinct traversable route, clue location, local witness and a short optional detour, with both outcomes manually playtested
- **Tier 2 authored region:** terrain, lights, Niagara/hazard cues, navigation meshes, model materials, audio, 18 performances and encounter variations
- **Tier 3 release candidate:** art direction cohesion, nine connected realm route QA, NPC consistency, performance tests, UI localization/accessibility, Steam/Win64 packaging and a complete creator test

## Production artifact ID rules
Use IDs instead of ad hoc labels: `NPC_<stable-id-with-underscores>`, `DT_UnmadeNPCs`, `DT_UnmadeRealms`, `DT_UnmadeRites`, `SM_<Realm>_<Prop>`, `SFX_<Realm>_<Beat>`, `ABP_<Realm>_<Role>`, `NS_<Realm>_<Hazard>`. Build gameplay events around source tags (`Tidal.Echo.CradleSeal`, etc.). Never infer that a rendered illustration is an in-game screenshot.

## Capability truth
Source-level JSON-to-C++/CSV export, native tests and committed data all exist. **Editor CSV import, UHT success, actual mesh production, staged dialogues, voice performance, map art and real PC gameplay remain unverified.** The JSON pack is not a full substitute for Unreal content authoring; its purpose is to eliminate duplicate writing, ID churn and manual reentry.

The latest illustrated volume stays Volume VI until a newly rendered volume is produced and its art/source provenance validated.


## Primary literary continuity for new volumes and screen adaptations

Authoring/transmedia_storyworld_v1.json anchors eleven existing settlements, thirteen established character actor IDs, nine OPEN mysteries and thirteen callback threads to current story/game source; docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md is the new long-form source. Run python Scripts/validate_transmedia_storyworld.py --check for historical identity, place and reveal gates. Literary characterization and visual/voice descriptions are editorial authoring directions, not live level actors, audio performances or save-state additions.
