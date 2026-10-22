# The Unmade — reactive creature habitats, first six source-backed sites

**Status: gameplay source implementation / provisional habitat art.** These are six existing bounded optional encounter sites and their existing creature pairings, **not a new canonical account of where creatures originated or how every species nests**. Open, disputed and sealed mysteries remain intact.

## Why this exists

The earlier encounter generator put three enemies in each of six arenas, with no physical evidence of occupation and no meaningful response from a den, cave, roost or burrow. A house-shaped decorative object does not meet the player's request. The new loop ties environment, enemy routine and player action into one visible sequence:

**Approach → tracks → territory warning → break line of sight using placed cover → inspect from nearby → open shelter alcove → watch the creature return toward its forecourt or patrol**.

No ability is automatically copied, no quest token is awarded simply by investigating, no den resets canon testimony or save outcomes, and defeat is not the only useful response.

## Existing site and proposed habitat arrangements

| Existing optional combat location | Existing creatures (not newly invented) | Provisional physical home | Environmental response and counterplay |
| --- | --- | --- | --- |
| Reach.AshRoad | Cinderjaw, Pale Strider | JawDen — layered fossil ribs, gnaw-tracks and stone jaw cavity | Ground tracks, brighter cue on close approach; guard/retreat or take rib cover |
| Reach.GlassCut | Veilwatch, Gleam Moth | GlassRoost — large fractured spires and an elevated watcher perch | Warning pulses, glass ribs that physically block watcher sightline |
| Bellwold.Forgotten | Hollow Hound, Grief Lantern | HoundUndercroft — isolated low vault, worn approaches and side pillars | Shelter and undercroft reaction away from the civilian refuge; no literal bell body invented |
| Paperhaven.Torn | Inkmaw, Redaction Eye | ArchiveBurrow — collapsed shelf galleries with a sheltered end | Record debris warns of den use; surviving shelves interrupt ranged attacks |
| Saltwake.Saltglass | Saltglass Crawler, Drowned Watch | TideCave — pilings, tide cave, damaged shoreline | Watermark clues and sheltered tide approaches |
| Cinderhold.Slag | Ashmaw, Kiln Wisp | SlagMaw — multi-vent slag chamber and structural bracing | Bright heat-colored response; approach behind substantial stone braces |

**Implementation boundaries.** `UnmadeCreatureHabitatRules.h` contains the deterministic six-site response contract. `UnmadePlayableEncounters.cpp` places collision-bearing mesh proxies, sight cover, inspection signs, transient shutters and reactive point lights. `UnmadePrototypeHub.cpp` samples the player's position and visibility, brightens/dims the local cues, and permits a line-of-sight/proximity inspection that opens a cover alcove. `UnmadeEnemyCharacter::ConfigureHabitatHome` now places existing routine origins toward each habitat's forecourt instead of arbitrary arena spawn positions. Creature combat/defeat logic and the existing optional field survey remain unchanged.

**Experience this without an Unreal PC:** `site/play-habitats.html` is a standalone *browser-only* interactive concept demonstration, with six switchable sites, directional controls, cover, defender awareness, composure loss, retreat and a persistent opened shelter in browser storage. That demo deliberately cannot modify the real game save or perform Unreal collision/navmesh.

## Acceptance checklist for production

- [x] Six different source-backed physical habitat silhouettes are authored beside existing optional encounters.
- [x] Native, independently testable response rules depend on player distance and physical cover.
- [x] Player action at the habitat changes a collision-bearing screen, rather than only displaying text.
- [x] Existing creature home/forage routines use placed habitat forecourts.
- [x] Standalone browser test offers the same basic approach/cover/inspect loop.
- [x] Canon source coverage ties the new code to existing twelve creature IDs; no new creature is declared canonical.
- [ ] Unreal Editor compilation, actual collision and navmesh baking, creature traversal and fight fairness on target PC.
- [ ] Production cave/den sculpting, foliage, sound occlusion, animations and physically accurate entrances.
- [ ] Real save-backed environmental changes where and only where canon explicitly authorizes them.
- [ ] Additional habitat design for the remaining realms and future bestiary placements after their source lore is grounded.

**Avoid regressions:** no one generic cave applied to all species; no automatic copied powers from observation; no endlessly respawning grind; no civilian-refuge monster spawn without source evidence; no false promise that the browser is a real Unreal level.
