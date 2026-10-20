# THE UNMADE — Pre-PC closure matrix

**Source audit updated: 20 October 2026.** This is a *work queue*, not an assertion that the game is playable. Track source evidence and physical Unreal evidence independently. The v21 parity contract is a necessary quality floor, not proof of nine finished realms.

## Closure definitions

- **NATIVE TESTED:** portable C++17 or Python behavior passed in CI against authoritative registries, not an Unreal simulation.
- **SOURCE WIRED:** Unreal-facing C++ handler/source assets reference the portable rule; no verified UHT/Editor runtime.
- **AUTHORED ONLY:** a design or JSON description exists but no executable gameplay effect has been accepted.
- **CONTENT MISSING:** final music, recorded dialogue, production maps, camera performances, final character/creature rigs, effects and production UI remain missing or unverified.
- **ENGINE REQUIRED:** compilation, input focus, geometry, VFX/audio timing, physical play, save/load, accessibility and packaging must be tested on a real supported Unreal host.
- **BLOCKED BY EVIDENCE:** cannot mark a system complete merely because its tests are green; attach the absent proof to its task.

## New October 20 source-production evidence

- 202 independent source-authored static prototype glTFs: 88 city modules, 27 clue props, 78 catalog equipment models and 9 visually distinct realm crowns. These are **not** complete maps or final production art.
- Three skinned glTF archetype prototypes (Resident, Beast, Wraith), with 19 source-authored motion clips apiece (57 total). First-PC import must certify the actual Unreal `SkeletalMesh`, `AnimSequence` and compatible skeletons; Source CI is not the Unreal Editor.
- The current native nine-realm scenes have 27 noncolliding decorative silhouette guides, distinct role profiles for five NPC occupations and 12 creature species. They do not alter gameplay collision, enemy AI or earned discovery.
- Threefold's F9 journal, evidence notebook and tracked HUD now derive optional witness-route guidance from existing saved firsthand observations and physical custody; four return-choice combinations are source-tested.
- The first-PC script attempts guarded asset imports with dated receipts and halts on missing/partial assets. Actual Editor imports, animation playback, VFX, player movement, packaging and target hardware performance remain unverified.

## Verified source coverage versus missing closure

| Domain | Current evidence | Next source-only closure | Beyond source/PC requirement |
| --- | --- | --- | --- |
| Main campaign + atlas + two mornings | `UnmadeCampaignSpineRules.h`, `UnmadeCampaignWorld.cpp`, `UnmadeFinalEncounterRules.h`; native branching tests, now exhaustive witness-state/route walk | Record 9 marker pre/post readings, all act journal texts, migrated-save cases and outcome provenance as golden snapshots; fuzz restored quest evidence | First-PC walkthrough of real portals, first-story mechanisms, journal and both endings |
| Reality shifts + ten rites + six confluences | Portable fracture, rite, confluence, consequence rules; persistence contracts | Parameter/failure matrix; conflicting and double-commit hazards; separately prove paid physical study and save rollback | Real effect targeting, visible preview/confirmation, frame timing, accessibility alternatives |
| NPC knowledge, resident return, local consequences | Stable NPC dossiers and witnessed-versus-rumored memory rules; return-state snapshots | Multi-NPC contradictory firsthand/hearsay suite; all local return lines, schedule/date/consent coverage | Visible NPC dialogue, daily motion, navigation, actor persistence |
| Economy, items, recipes, four save profiles | Finite stock, wage, materials and recipe source; portable tests and dependency ledger | 1,000+ deterministic seeded settlement scenarios; no negative stock or lost wages; recipe provenance and migration fuzz | Trading/UI, clock feel, actual fail-save and backup-recovery in Unreal |
| Enemies, final boss, hazards | Portable attack/tell/guard/Strain, guardians and original bosses; native tests | Multi-phase balance sweeps, role/tell/counterplay acceptance table; nonviolence exits and hazard dedup fuzz | Animation cues, hitbox geometry, controller timing, combat enjoyment |
| Progression and abilities | mixed-status Ability Codex, learned rites/techniques/studies, physical Atlas conditions | Match all codex rewards, costs, observation stages and side-effect claims to executable rule IDs; baseline 30–45 minute slice target *as an unverified hypothesis* | Measure player pacing, difficulty, level design and discovery friction; revise assumptions after actual play |
| Menu, HUD, accessibility, bindings | Eight-page, 28-action source menus and parity accessibility requirements | Action/focus/back graph, objective HUD contract, captions/localization keys, remap conflicts and error states | Rendered UI, keyboard/controller remapping, readable screenshots, subtitles/sound/motion tests |
| Narrative, companions, final-act presentation | Nine named realm beats, source-spawned story markers and read-only Threefold witness objectives | Improve Act 4–5 scripted testimony/questions; define recurring witness interaction without fabricating a playable party; decide Ink/Yarn import after evaluating migration costs | Performance, VA, companion navigation, actual cinematic staging |
| Content pipeline + continuity | JSON/CSV/header generation, ID and parity validators, lore release records | Row-count/source parity for every new data table, stable ID migration fixtures, exact audio/mesh/FX/camera handoff lists | Import meshes, maps, animations, voice, music, VFX and validate actual gameplay capture |
| First shippable slice | Crossings + Bellwold/Paperhaven first experiences already scoped across design/QA docs | Consolidate one 30–45 minute proposed player journey: witness → study → confrontation → witnessed aftermath → cold save/reload, with a feature freeze for the slice | Build a real traversable map, perform two playthrough paths, fix issues, capture footage and package a playable build |

## Main-story source acceptance

`Tests/story/campaign_exhaustive_walkthrough_test.cpp` runs as its own named CI step. It covers all **1,024 combinations** of nine completion bits and the presence/absence of opening testimony. It checks safe act progression, no invented boss access, legal journal alternatives, reachable atlas graph passages and monotonic attunement when optional evidence is added. It also exercises every *offered* frontier → middle → upper route without relics, the Auvren chapter prerequisite, Held Morning / Many Mornings saved results, and an optional post-victory Echo. 

**Important limits:** a synthetic chapter-completion receipt is not proof that an NPC/portal can actually complete that quest in Unreal. It verifies the consequences of bona fide events, not level geometry or save files from an installed game. Exact prose/localization snapshot coverage is a *subsequent* source task, not silently claimed done.

## Branching rules we will not trade away

1. Never require an optional Echo, guardian kill, confluence, all-landmarks trophy or mythic forge for Nhal-Vey.
2. Only a physically resolved first chapter can raise the relevant witness/evidence bit; no journal page, map visit or quest description grants progress.
3. Repeated visits do not duplicate an item, witness vote, wage, or conquest.
4. Companions learn by participating, hearing testimony with provenance, or witnessing acts, not by accessing the omniscient save ledger.
5. All irreversible world changes have a preview, specific acceptance and an atomic save boundary. A failure cannot alter the prior good state.
6. Endings change the public world but do not erase earlier choices, economic obligations, player identity, or the player's right to revisit.
7. Any exact gameplay time, reward, progression or balance claim made pre-PC remains a test hypothesis until in-engine play confirms it.

## Recommended remaining pre-PC order

**Next:** (1) authoritative journal/golden-text and migrated-save test coverage; (2) Strain/Rewrite rollback and conflict fuzz; (3) multi-NPC contradiction paths; (4) economy stress/softlock simulation; (5) role/counterplay and boss pacing; (6) UI navigation/caption/rebinding specifications; (7) immutable IDs and data import parity; (8) final status/README and slice acceptance.

For each domain, require **source identifier → input conditions → deterministic output → saved consequence → test link → engine playtest to run later**. Passing native tests closes *only* the source column. 

Related: [current pre-PC readiness](pre-pc-readiness.md), [v21 parity floor](gameplay-parity-v21.md), [vertical-slice status](vertical-slice-status.md), [campaign specification](../design/unanswered-road-main-campaign.md).
