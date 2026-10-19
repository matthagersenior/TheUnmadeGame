# THE UNMADE — Definitive Canon Encyclopedia

**Edition:** 19 October 2026 · **Current status:** source-indexed design, mechanics and literary canon · **Not** a packaged or fully engine-tested game.

The repository has one **unified directory to the entire established game universe**. It does not displace source-of-truth registries or manufacture playable features. Every stable ID is tied to a known authority; contradictions and unfinished systems are identified rather than papered over.

## Read by subject

- [I — All nine realms, local civilizations, eleven cities, customs, mysteries, everyday life and environment direction](01_WORLDS_AND_PLACES.md)
- [II — All 88 resident identities, character motives, testimony limits, relationships and returns](02_CHARACTERS_NPCS_AND_RELATIONSHIPS.md)
- [III — All 39 named boss encounters, creature patterns, guardians, counters, options and aftermath](03_BESTIARY_BOSSES_AND_GUARDIANS.md)
- [IV — All 60 inventory items, eighteen exclusive regional rewards, weapon styles, recipes and professions](04_ARMORY_ITEMS_AND_CRAFTING.md)
- [V — Every 256-record Ability Codex entry, costs, study and mastery requirements, limits and mechanics](05_ABILITIES_SKILLS_AND_RULES.md)
- [VI — All 99 grouped quests, twenty-three learned skills, nine physical main chapters and alternate endings](06_QUESTS_STORY_AND_ENDINGS.md)
- [VII — Volume X chronology, nine deliberately open mysteries, callbacks and adaptation boundaries](07_LORE_MYSTERIES_AND_CONTINUITY.md)
- [VIII — Three faction chronicles, worker pay, households and two conserved cross-realm trade contracts](08_FACTIONS_SOCIAL_SYSTEMS_AND_ECONOMIES.md)
- [Machine-readable searchable index of every stable source-backed codex record](INDEX.json)

## Canon coverage (distinct indexed families, not a total of independently playable assets)

| Indexed family | Entries |
|---|---:|
| codex | 256 |
| item | 60 |
| regional gear | 18 |
| boss | 39 |
| quest | 99 |
| learned skill | 23 |
| npc | 88 |
| realm | 9 |
| city | 11 |
| mystery | 9 |
| callback | 13 |
| volume | 10 |
| faction | 3 |
| trade contract | 2 |

**Total searchable index records:** 640. Entries in one family may describe the same character, weapon or encounter concept represented in another; the groups should not be added together and described as unique content.

## Authority hierarchy

1. **Mechanical truth:** Actual C++ source, stable save models, [dependency ledger](../dependency-ledger.json), and tested canonical authoring data. A document or dialogue line cannot grant an item, quest, route or power.
2. **Ability and learning authority:** [Authoring/ability_codex.json](../../../Authoring/ability_codex.json), including observation, experiment, mastery, costs and explicit implementation classifications. The live rule governs actual effects if authoring and code disagree.
3. **Storyworld/editorial truth:** [Volume X](../../lore/VOLUME_X_THE_UNANSWERED_ATLAS.md), [living continuity](../../lore/living-world-continuity.md), and [lore update policy](../../lore/UPDATE_POLICY.md). Canon literary motivation is not a rendered Unreal character scene.
4. **In-world claims and disputed history:** Mark what an individual witness knows, infers or heard. Opposing evidence can be legitimate without the story revealing an objective answer.
5. **Production and prototype direction:** Planned models, cinematics, recorded voices, music, imported maps, input layouts and encounter feel are NOT complete solely because an asset sheet or source handler exists.

## Non-negotiable canon contracts

- Nine realm journeys remain nonlinear; opening testimony → one earned frontier → one earned middle → one earned upper → Auvren trial → Nhal-Vey. Optional guardian/Echo/observatory/relic completion cannot be a hidden condition for the main ending.
- Reality manipulation requires appropriate limits, confirmation of lasting consequences and atomic saved outcomes. Observation does not automatically copy a creature's abilities.
- An NPC remembers local firsthand events and provenance-aware rumors, not omniscient world changes. Consent and private names are not disposable rewards.
- All occupations, trade stock, wages and household resources are finite; physical receipts may transfer resources but cannot mint currency or fabricate deliveries.
- Player origin and identity are customizable. Neither **The Held Morning** nor **The Many Mornings** may be elevated to the only universal canonical ending.
- Literary mysteries classified **OPEN**, **DISPUTED** or **SEALED** remain so until reviewed canon changes establish actual evidence.

## Current source gaps and reconciliations

- **256 codex entries:** Earlier static descriptions mention 182 or 186 entries; those are historical snapshots. Only the current Ability Codex file defines the present count, and not all entries are castable powers.
- **88 resident identities, 82 return dossiers:** The six newer scouts have source identity records but not the independent 82-person resident-return dialogue. The characters are preserved and their missing expansion is visible.
- **Eleven literary cities, three original playable-settlement source definitions:** Additional cities are real world-building and production contracts, not yet verified explorable Unreal levels.
- **Source evidence versus gameplay:** Real compiled behavior, character animations, VO/music, full accessibility, pathfinding, collision, packaging, all-save migrations and human playtesting remain explicit Unreal/production gates.
- **Illustrated novels and guides:** Historical publication files are dated artifacts. Updating this source index is not an assertion that the full illustrated book has been regenerated and visually approved.

## All new additions must pass the canon gate

**[Canonical admission and change policy](../CANON_ADMISSION_POLICY.md)** · **[Source registration manifest](../SOURCE_COVERAGE.json)**.

Future gameplay code, production assets, abilities, NPCs, quests, bosses and new lore manuscripts cannot be introduced as untracked new files. They must reference an existing canonical ID or define a new source-backed ID with a matching index entry and volume. Changes to authoritative authoring tables also require the relevant readable volume to be updated in the same PR. Native checks cannot replace a human continuity review or an actual Unreal playtest.

## Maintaining the encyclopedia

When a new canon ID, NPC, weapon, ability, enemy or quest enters an authoritative registry, update its entry in the correct readable volume and `INDEX.json` together. Run:

```bash
python Scripts/validate_canon_encyclopedia.py
python -m unittest Scripts.tests.test_canon_encyclopedia -v
```

These tests compare all indexed families against the current authoritative JSON and faction/trade C++ registries. Deleting a boss, item, NPC or stable ID from the encyclopedia fails CI. The validator does not establish that the prose is artistically complete or that the game runs in Unreal. When changing canonical lore (not merely compiling references), also comply with [the full continuity and publication policy](../../lore/UPDATE_POLICY.md).
