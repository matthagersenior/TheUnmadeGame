# THE UNMADE — Six Later-Realm Guardians: Engine Acceptance

**Pre-PC implemented source and native tests; Unreal Editor compilation and actual arena traversability are still pending.**

| Realm | Guardian | Warning and physical response | Moral context |
|---|---|---|---|
| Drevlach | Collector of Unborn Days | Future-chain sweep; leave arc, guard, or Fold | A child must not inherit another's debt |
| Orravane | Missing Chorister | Tightening harmonic note; use shelter/sight | A silenced voice cannot be an involuntary pillar |
| Vathless | Mortar Ancestor | Quarry-hammer windup; retreat or interrupt | An ancestor may not be building material |
| Eillun | Numberless Examiner | Narrowing census pulse; covered pathway | People need not register to exist |
| Tharniv | Root Bailiff | Law-root telegraph; move to unbound row | Rule by consent, not inherited title |
| Auvren | Unfinished Witness | Double silhouette warning; stable path | More than one true origin may survive |

## Hands-on verification
1. Finish each realm's Echo story and reload: its local guardian becomes active on the northern upper platform, outside the safe arrival area. With incomplete Echo, no guardian should appear, block, attack or be pacified.
2. A real enemy must appear at the configured location with its own shape/health/damage. Stand inside range: a **visible and readable windup** precedes any actual hit. Guard mitigates damage, evade or block line-of-sight during the windup to avoid impact. Leaving the arena clears a stale strike. Test frame-rate dips and world pause.
3. Fold exposure interrupts a warning; mastered Understanding Bosses can expose a nearby realm guardian for a stagger without automatically killing, hiding or coercing it. Verify normal weapon strikes still affect the enemy and defeat after actual damage is saved.
4. Mercy: complete the witnessed Echo, approach inside 310 cm with clear line of sight, press E once for specific written consequence preview, then press E on the **same** nearby guardian again within six seconds. The guardian ceases attacking only if the outcome successfully saves. A nonviolent player need not grind for a particular mastered rite.
5. Combat outcome: defeat by weapon after readable tells. Guardian remains unresolved on a simulated failed SaveGameToSlot; do not silently grant a kill or respawn loot. After successful write, defeat is permanent and the actor becomes hidden with collision disabled.
6. Both paths are mutually exclusive and irreversible per save. Check the three local residents respond differently to a peaceful agreement versus violent defeat and do not know the result in distant, unconnected settlements. Neither outcome locks the main quest or grants repeatable equipment.
7. Old schema-1 saves lacking `bHasGuardianSnapshot` load cleanly. Test six saved independent 0/1/2 results, invalid lengths/values, a write failure, a new game, reload, and interaction priority when an NPC and guardian are both nearby.
8. Real Unreal UHT/build, Automation `Unmade.Combat.SixOptionalRealmGuardians`, navmesh, capsule/floor collision, combat camera, animations, audio, controller, subtitles, accessibility, balancing and performance must all be checked in Windows PIE. Native GitHub CI alone never proves these.

## Additional completion work
This is the first source-complete guard AI and morality layer, **not** six finished bosses. Their visuals are reused Unreal primitive blocks; no full character meshes, voiceover, bespoke AI NavMesh, boss phases, original audio, or nuanced faction economy system is finished. The future encounter pass should add individually animated creature silhouettes, spatially varying moves, optional quest dialogue and environmental puzzles that can be solved without fighting.
