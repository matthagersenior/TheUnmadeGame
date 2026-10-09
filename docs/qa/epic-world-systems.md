# THE UNMADE — Epic systems and first other-realm footholds

**Source-ready graybox prototype, Unreal runtime unverified.** Do not call
these changes a compiled Unreal game, visually complete realms, production
economy, or real boss gameplay until there is engine verification.

## What this pass actually adds

| Part | Native source behavior | Actor or player-facing source |
| --- | --- | --- |
| Named bosses | 3 authored boss policies, warning windows, HP phases, counterplay and once-only victory inventory flags | 3 unique boss actors at their respective village edges, different graybox silhouettes, Fold anchors, reward hooks |
| Items and gear | 42 IDs, rarity, finite stacks, healing and passive perks | inventory and equipment component uses the old merged save slot |
| Crafting/professions | 6 recipes, smith/apothecary/scribe practice, material costs, skill gate and no free repeated output | P at a valid village workshop; items and skills persist together |
| Village economy | local prices, safe coin transactions, non-profitable buy/sell loops, civic payments exactly once | Y/U near a local merchant, shared finite marks |
| Faction chronicles | 3 four-state stories with 3 separate witnesses, canonical evidence gate and two immutable endings | E to speak, F7/F8 near final representative; reputation affects merchant prices |
| New realm footholds | two story arcs, distinct NPC registry, visit/clue masks, save snapshots | G for two returnable gateways, 2 primitive grounds and 16 NPC actors |
| Combined scenario | Native C++17 tests exercise all the above with previous combat, fracture, lexicon and regional state | Unreal event wiring checked by Python contracts only |

## Boss encounter specifics

- **The Bell That Buried Its Keeper** is placed on Bellwold's outskirts.
  It signals a radial shockwave before striking, and Fold can interrupt it.
  Winning grants the unique Aegis of the Hollow Bell.
- **The Curator of Missing Names** appears outside Paperhaven. It retreats
  when crowded, telegraphs longer-range attacks and yields the Scepter of
  Missing Names.
- **The Unfinished Pilgrim** threatens a broken route in the Crossings.
  It closes distance in a charge, warns before damage, and drops the
  Pilgrim's Signet once.
- These are unanimated cube/cylinder actors, not finished bosses. Their
  danger cues are currently temporary debug messages; 3D hitboxes,
  animation telegraphs, sound/VFX and collision need Unreal playtests.

## Profession loops

**Smith:** common iron + quest-earned bellmetal makes a weapon; a higher
skill tier can craft lanternwoven mail. **Apothecary:** herbs and scrap
make salves; rare materials can produce Ash of Possible Lives.
**Scribe:** pages and herbs make Stillness Vials; a higher tier seals
testimony into a unique charm.

The named recipes are finite and use real inventory stacks, but a finished
crafting table UI, herb foraging nodes, ore mining, vendor stock management,
NPC schedules in interiors, player skill tree and dynamic supply/demand are
not implemented. Starting marks and civic contract earnings permit limited
transactions and should be retuned during a real playtest.

## Multi-step factions and endings

- **Refuge Compact:** Rook the lamplighter, Hessa the matron, Bram the guard.
  Evidence: the Bellwold relief story. Final choice: solidarity or truth.
- **Redacted Charter:** Toma the copyist, Sevrin the registrar, Calder the
  keeper of hours. Evidence: Paperhaven's saved testimony.
- **Roadbound Assembly:** gate watchkeeper, road warden and well listener.
  Evidence: visiting all three first-region villages.

**E** speaks to named witnesses; **F7/F8** commits to the final outcome in
front of the proper third witness. The player receives one-time epic gear
per resolved arc. This is authored, independent of AI or cloud inference.
Choices adjust available trade discounts but do not yet change architecture,
NPC patrol routes, voiced cutscenes or story-wide ending cinematics.

## Expansion to two other realms

- **The Rain That Forgot the Sea — Saltwake:** primitive drowned-port
  silhouettes, eight separate memory-bearing NPCs, a disappearing-ocean
  clue, and a three-witness question about water and testimony.
- **The Hearth Beneath — Cinderhold:** primitive cavern/forge silhouettes,
  eight NPC identities, an impossible shared ember, and a separate
  three-witness question about ownership of warmth.

G at a marked gate travels to one foothold and G at the local return gate
returns to the Threefold Reach. E near the clue tablet records evidence,
then E with the listed witnesses advances the authored story, with F7/F8
as permanent resolutions. Items are rewarded from actual saved results.
These two footholds **are not finished realm maps**. The other six atlas
destinations remain written concepts and route-graph design only.

## Mandatory Unreal acceptance tests still outstanding

1. Open the project in supported Unreal Editor and compile all project C++
   with its real reflection/engine modules; address any compiler errors.
2. Walk the three villages and use G to travel into both source outposts;
   confirm portal placements, spawn collision, ground seams and safe return.
3. Inspect all 64 NPCs (48 original, 16 frontier), no duplicate stable IDs;
   verify rumor spread cannot cross huge distances without proximity.
4. Play each named boss phase, observe the actual telegraph *before*
   damage, dodge and Fold interruption, death and unique reward across reload.
5. Trade, sell, craft each of six recipes at permitted locations, practice
   all three skills, test underflow/full bags, and reload money/skills/items.
6. Complete three faction arcs and two new realm arcs, with game restarts
   between each conversation, clue discovery and final outcome.
7. Verify rewards are one-time, unique equipped gear still applies to
   combat, older 28-item saves load safely, and corrupt arrays never
   cause duplication or silent overwrites.
8. Add actual 3D meshes, boss VFX/animation and sound, localization,
   configurable controls, a visible inventory/journal, proper merchants,
   consumable feedback, checkpoints, player death/respawn and accessibility.
9. Profile actors/tick, gossip O(N²), physics/lighting, and world partition
   before pushing beyond 64 NPCs; add LOD for off-screen settlements.
10. Test completely offline and with Ollama stopped. No critical mechanic
    may use generated dialogue, a remote API or a subscription.

**CI conclusion is restricted to Python structural tests and native C++17
domain tests. No Unreal Engine build or shipped executable is claimed.**
