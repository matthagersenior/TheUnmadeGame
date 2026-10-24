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

### Campaign journal snapshots and schema-1 restoration — 22 October

`RestoreRoadEvidence` reconstructs the journal from existing opening testimony, first-chapter receipts and final-journey state. It masks out bits beyond the nine canonical realms and adds no journal fields or save migration. The native campaign test locks the exact title, question, objective, branch choices and boss-access state across the opening, each route split, Auvren, the second boss form and post-ending return. It also covers older records with only Crossings testimony, realm progress, or final-journey progress. These are portable source tests; Unreal's actual deserialization and in-game journal rendering remain engine acceptance work.

## Branching rules we will not trade away

1. Never require an optional Echo, guardian kill, confluence, all-landmarks trophy or mythic forge for Nhal-Vey.
2. Only a physically resolved first chapter can raise the relevant witness/evidence bit; no journal page, map visit or quest description grants progress.
3. Repeated visits do not duplicate an item, witness vote, wage, or conquest.
4. Companions learn by participating, hearing testimony with provenance, or witnessing acts, not by accessing the omniscient save ledger.
5. All irreversible world changes have a preview, specific acceptance and an atomic save boundary. A failure cannot alter the prior good state.
6. Endings change the public world but do not erase earlier choices, economic obligations, player identity, or the player's right to revisit.
7. Any exact gameplay time, reward, progression or balance claim made pre-PC remains a test hypothesis until in-engine play confirms it.

## Recommended remaining pre-PC order

**Next:** (1) Strain/Rewrite rollback and conflict fuzz; (2) multi-NPC contradiction paths; (3) economy stress/softlock simulation; (4) role/counterplay and boss pacing; (5) UI navigation/caption/rebinding specifications; (6) immutable IDs and data import parity; (7) final status/README and slice acceptance.

For each domain, require **source identifier → input conditions → deterministic output → saved consequence → test link → engine playtest to run later**. Passing native tests closes *only* the source column. 

### Strain, Rewrite and conflict rollback stress — 22 October

`Tests/fracture/fracture_conflict_fuzz_test.cpp` runs 100,000 deterministic seeded transitions (`0x5EED1234`) across Glimpse, Fold, Rewrite, strain spend/recovery, environmental strain, conflict choices, and supply pickup/delivery. It checks strain bounds, supported Rewrite outcomes, one-time conflict decisions, monotonic supply delivery, and save snapshot round trips. Successful Rewrite transitions are immediately rolled back to the exact pre-commit snapshot to model a failed durable write; malformed restores must leave the current snapshot unchanged. A structural source test also checks that the actual Unreal Rewrite and rite handlers restore fracture, rite, confluence and paid Strain state after persistence failure. This closes portable stress and source-wiring evidence only; disk failure injection and engine behavior remain PC acceptance work.

**Next:** multi-NPC contradictory firsthand/hearsay paths.

### Realm skyline signatures — 22 October

Each of the nine existing frontier silhouette triplets now has its own bounded three-piece orientation profile, applied at spawn while retaining the existing realm-specific placement, primitive selection, noncollision, no-overlap, and no-navigation rules. Native checks verify all nine profiles differ and every angle stays within 20 degrees. This improves long-distance visual identification in the prototype; it does not add an asset, gameplay landmark, discovery, route, or lore fact. Unreal rendering and distance readability remain unverified.

Related: [current pre-PC readiness](pre-pc-readiness.md), [v21 parity floor](gameplay-parity-v21.md), [vertical-slice status](vertical-slice-status.md), [campaign specification](../design/unanswered-road-main-campaign.md).

### Interactive NPC dialogue and provenance responses — 24 October

Player interaction now opens a focused conversation panel with three grounded topics and a custom question capped at 400 characters. Async replies are accepted only for the currently open NPC. The deterministic offline path answers memory, concern and firsthand-witness questions from that resident's saved observations, identifies rumor speakers, and admits when no firsthand account exists. Replies and prompts are also retained in the existing conversation journal. The model remains presentation-only and cannot commit a quest choice or change world state.

Portable source contracts and the full Python suite pass. Unreal automation now covers separate NPC memories and offline reply provenance, but it has not run here. This closes the first conversation interaction slice only: each resident still needs individually authored multi-session dialogue and consequence-aware return lines; canonical quest branches need their authored responses, eligibility reasons and save effects wired to physical actors. Unreal Header Tool compilation, focus/controller checks, and a PIE conversation pass remain required.

### Living-economy stress schedules — 24 October

`Tests/world/living_economy_test.cpp` now adds 1,000 deterministic schedules of 45 days each. The LCG seed is fixed (`0xC0FFEE`); the schedules mix wages, walkouts, arrears, trade collection, wrong-destination and delayed deliveries, and cold snapshot restoration. Every day checks nonnegative balances and stores, bounded structural integrity, and exact restoration of municipal accounts, route receipts, and in-transit cargo. The warning-as-error native compile and executable pass. This supplements targeted chronology and malformed-save tests; it does not replace player trade and shortage pacing in Unreal.

**Next source-only work:** close the role/counterplay acceptance table and the remaining menu navigation, subtitle, and remap error-state contract. **First-PC work still required:** certify prototype mesh imports and skeletal compatibility, play the dialogue and quest routes, import/edit final character assets, record or mix production audio, stage performances in Sequencer, and render the cinematic. Current source models, synthetic WAV cues, and cinematic CSVs are prototypes and handoffs; they are not production-ready assets or a finished film.

### Asynchronous dialogue session isolation — 24 October

NPC reply events now carry a widget-local conversation session ID through the local inference subsystem. Starting a new conversation advances the ID; the UI accepts a reply only when both stable NPC ID and session ID match. This prevents a delayed Ollama response from a closed conversation appearing after reopening the same resident, in the modal transcript, or as a stale global HUD notice. One-off interaction greetings use session zero and retain their existing HUD notice. The focused dialogue source contracts pass. Unreal delegate generation, HTTP timing, and repeated close/reopen behavior still need Editor automation and a PIE run.

### Later-realm character profiles in optional dialogue — 24 October

The deterministic content generator now emits each of 18 later-realm residents' authored desire, fear, knowledge boundary and voice direction in the compiled profile alongside their day/night lines. The optional local model receives those fields as portrayal guidance, the current authored line and the resident's separate witnessed/rumor evidence. Its system prompt explicitly treats motives as non-evidence and the knowledge boundary as a factual limit. This improves use of existing character authoring; it does not add dialogue branches, save data, NPC decision inputs or world state. The generated-content check and authoring source tests pass; UHT and model behavior remain unverified in Unreal.

### Bounded multi-turn model context — 24 October

The optional local model now receives up to 1,200 characters of previous turns from the currently open resident conversation. The transcript is clearly marked as untrusted dialogue, not evidence, and is scoped with the active session. This gives follow-up questions conversational context without promoting generated statements into memory or facts. Offline deterministic dialogue remains unchanged. All 21 focused Python source tests and generated-content parity pass. UHT, prompt behavior across conflicting/generated claims, and in-engine multi-turn playback remain unverified.
