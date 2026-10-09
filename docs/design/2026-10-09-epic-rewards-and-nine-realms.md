# THE UNMADE: rewards worth remembering and a universe built to grow

**Approved direction:** substantial character progression; discovery over repetitive
fetch/grind; authored rewards that alter play; completely playable with AI disabled.

**Verification state:** Native C++17 policy tests and GitHub source-contract CI
exercise the item and atlas models. Nothing described here constitutes a
compiled Unreal asset, playable new continent, animated epic item, shipped HUD
or functioning commerce economy.

## The game's promise

A person who visits somewhere forbidden, protects a village at personal cost,
hears a language no empire admits exists, and returns from a fracture should
come away **changed**, not merely with a larger number. Items should carry
provenance, tie back to people the player has met, and sometimes make future
choices harder rather than uniformly easier.

### Actual first item-system milestone

The checked-in C++17 catalog has 28 named item definitions across weapons,
armor, charms, potions and forging materials, from Common to Mythic.
Each has a unique stable code, authored lore, equipment slot, stack cap,
bounded numeric modifiers and optional healing/Strain recovery.

- Melee attack bonus and physical armor mitigation are connected to the
  existing native combat logic through an Unreal component (uncompiled).
- A starter kit grants a salvage weapon, mantle and potions only once.
- Quests, discoveries, personal first victories, visiting all villages and
  faction commitments grant deterministic once-only item rewards.
- Inventory/equipment, claimed-milestone bits and consumable counts persist
  in the existing version-1 prototype save without deleting prior NPC,
  world-choice, exploration or lexicon data.
- Potions don't consume if they wouldn't help. Invalid or corrupted saves
  are rejected; failed save writes roll back item state. Strain recovery is
  saved atomically with the spent potion.
- Equipment can be cycled in the temporary prototype. There is not yet
  a rendered backpack UI, item meshes, crafting animation, vendor economy,
  storage containers, controller menus or production-level balancing.

## The three villages should pay off differently

| Earned action | Primary item reward | Why it matters |
| --- | --- | --- |
| Help the Bellwold lamplighter and refuge matron | **Bellwold's Last Refuge** (Epic armor), Bellmetal | Reliable protection with the memory of solidarity |
| Preserve the lost Paperhaven testimony | **Lens of the Unentered** (Epic charm), Unwritten Ink | Attack and armor hybrid, proof against erased testimony |
| Reach The Crossings, Bellwold and Paperhaven | **Three Roads, One Name** (Rare weapon), Path of Three Settlements (Epic charm) | Long-travel recognition through meaningful gear |
| Find the Well of Returned Voices | **The Voice Returned** (Rare charm), Echo Glass | A riddle with mechanical and material value |
| Find the grave of the last bell | **Mantle of the Missing Hour** (Epic armor) | A single place rewards careful exploration |
| Find all six original landmarks | **The Unwritten Crown** (Mythic charm), Rift Seed | Exploration rewards commitment, not luck |
| Defeat first Stalker/Watcher | Finite materials, useful consumables, Watcher weapon | Combat achievement matters without farming weak enemies |
| Resolve the central settlement choice | Distinct shelter/research gear | Both sides grant useful but different outcomes |
| Complete the shelter supply run | **Heart of the Communal Flame** (Epic charm) | Local effort counts as much as elite combat |

### One truly earned mythic artifact

**Waybreaker, the Impossible Road** is not a lucky drop. It can be forged at
Bellwold's workshop once the player has: (1) completed Bellwold's lantern story,
(2) preserved Paperhaven's testimony, (3) visited all three villages,
(4) collected two Echo Glass, (5) saved Bellmetal and (6) saved Unwritten Ink.
Those materials are consumed once. The forged weapon is unique and grants a
large attack bonus in the prototype. The universe remembers the accomplishment
through a separate persistent reward item. It is not available at the start.

Further cataloged items, including Atlas of Absence, are **future-content
reservations** only: they have definitions but currently no legitimate
acquisition path, visual mesh or quest implementation.

## Expand to nine realms — not nine identical maps

The source includes a deterministic nine-realm atlas and traversal-graph
contract with authored identity and reward themes. Only the first region
(The Threefold Reach) has any actual Unreal graybox settlement source today.

1. **The Threefold Reach** — three charterless communities divided by what
   to remember. Ruins, bells, witness records, the first reality fracture.
2. **The Rain That Forgot the Sea** — hollow beaches under rain that lost its
   ocean. Flotilla civilizations weigh absent water against living passengers.
3. **The Hearth Beneath** — cavern villages preserved by a communal ember
   under a frozen sky; warmth is an obligation, not an exclusive possession.
4. **The Sea of Written Debts** — black currents collect bargains made
   tomorrow; navigators trade contracts instead of coins.
5. **The Upside-Down Choir** — hanging islands sustained by songs; losing
   cultural knowledge literally endangers the settlement's gravity.
6. **The Bones of Yesterdays** — mountains assembled from dead timelines;
   mining them changes whether descendants were ever born.
7. **The Hundred Unlived** — cities built for people history never permitted
   to exist; the census has become both prison and revolutionary scripture.
8. **The Orchard of Unwritten Kings** — courts without official titles or
   pasts; rulers are judged by what their decisions erase.
9. **The Place Before Place** — reality that resists being mapped; the late
   game's dilemmas center on whether one consistent history is desirable.

Each requires original landscape silhouettes, interactive settlements,
different factions and local languages, side quests, ecosystems, relevant
combat and noncombat encounters, craftsmanship, unique lore artifacts and
playable alternatives to violence. Identity matters more than raw map size.

The atlas's tested route planner is a **future travel graph**, not implemented
fast travel. Its stage gates use deterministic achievement/gear progression;
it does not invoke AI, random content generation or microtransactions.

## Long-term loot / crafting design rules

- Never let random trash drops outperform an artifact tied to a difficult
  memorable journey. Small practical loot can still exist.
- Named bosses, regional quests, exploration puzzles, diplomacy, forbidden
  knowledge, nonlethal outcomes and exceptional craftsmanship should have
  comparably meaningful rewards.
- Major gear is unique, with story provenance and optional upgrade paths.
  No real-money power purchases, luck boxes, forced daily timers or grinding
  the same easy enemy until a microscopic chance finally pays out.
- Limit inventories and define costs with explicit saved state, rollback,
  dedupe and version migrations before shipping an economy.
- Balance changes must consider playstyles: fighter, investigator, protector,
  explorer, negotiator and reality manipulator.
- More regions eventually need streaming/partitioning, NPC simulation LOD,
  asset budgets, waypoint navigation and profiling before population grows.

## Immediate verification gates

1. Compile the UE project in Unreal Editor when a supported host is available.
2. Run all C++ automation tests, enter PIE and inspect inventory/menu actions.
3. Confirm equipment meaningfully affects real damage and potions heal the
   correct character without duplicating stacks or losing the saved world.
4. Complete village tasks in/out of order, revisit all three villages and
   collect each legitimate one-time item; reload repeatedly.
5. Defeat each enemy type once, check no repeated grant after relaunch,
   then forge Waybreaker only at Bellwold workshop with true materials.
6. Check all other save domains after item activity and corrupted/missing files.
7. Measure readability for keyboard/controller, add item descriptions,
   model assets, FX, animations and accessible gear controls.
8. Build each realm as a separately playable, verified vertical slice before
   describing it as complete. Do not confuse source-domain CI with UE builds.

## Mechanically unique relic passives added

Beyond basic numeric stats, real pure C++ fracture rules now accept carefully
bounded modifiers from earned equipment. **The Voice Returned** reduces
Glimpse's eight Strain cost by three; **Lens of the Unentered** reduces it
by two, and only one charm may occupy that slot. **Waybreaker** extends
Fold by three seconds. **The Unwritten Crown** increases normal Strain
recovery by 50 percent. Invalid or out-of-range modifiers fail without
changing the world or spending Strain. Native regression tests validate
costs, timings, stacking, preview consistency and snapshot restoration.
These are source-level game behaviors; particles, item models, ability
visuals and Unreal live interactions still await an Editor-equipped host.

## Follow-on source expansion update — October 9

The original atlas paragraph above described the initial milestone. Since
then, Unreal C++ source now spawns one primitive foothold in each of the
two adjacent realms: **Saltwake** in The Rain That Forgot the Sea and
**Cinderhold** in The Hearth Beneath. Both have returnable gates, eight
uniquely named NPCs, realm evidence and local three-witness chronicles.
These are small graybox environments, not complete open-world regions.

The item catalog has expanded from 28 to **42** named items including three
boss trophies, three faction rewards, two realm-story relics, and materials
and crafted gear. Native boss, profession/economy, faction and frontier
tests now extend the earlier fracture and inventory suite.

The other six atlas regions do not yet have source-spawned settlements,
combat encounters, weather, custom sound or environmental art.
