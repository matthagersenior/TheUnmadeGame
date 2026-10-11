# Reality Consequence Engine v2 — Nine-Realm Reaction System

**Milestone:** Volume X Chapter 22. **Implementation level:** portable C++ and UE5 source, not Windows Editor/PIE certified.

## Actual gameplay call chain

1. A Tenfold invocation validates the player's original rite prerequisites, cooldown, strain and persistence.
2. The hub locates the precise region by actual caster XY inside the authored atlas radius. It does not grant a consequence in empty inter-realm space, and remote players/NPCs cannot inherit the result.
3. The legacy Reach ledger remains in its original save fields. The other eight ledgers are independent with one saved record each.
4. A temporary collidable safe, strained or contradictory structure becomes active only at the cast realm's physical base. Existing enemies standing in the same realm use locally adjusted pursuit speed. Existing story hazard pulses use the realm policy's injury/strain multiplier.
5. Actual residents within sight of the cast earn firsthand observations; local later gossip uses a named speaker and the realm's source identity. Without an eyewitness, no mastered rite contributes a civic proof bit.
6. Two **different** mastered, actually witnessed caring rites create Shared Passage. Two different interrogating rites create Public Inquiry. Both create a Contested Accord with two markers, still without solving the cosmological mysteries.
7. The world-save snapshot persists all new records. If a save fails, the new ledgers roll back without toggling public geometry. Old schema-1 games load with empty outer histories; bad nonempty masks/arrays block overwrites.
8. The J journal explains the local realm, its material dependence, institution and civic record. A Blueprint readout remains available. The existing Crossings market Buy path receives a bounded temporary +20 local trust input through the original economy price model when an applicable caring rite is actively shaping the area.

## Per-realm acceptance behaviors

| Realm | Local physical variable | Actual gameplay hook | Civic question for later advanced quests |
| --- | --- | --- | --- |
| Threefold Reach | contracted stone | enemy speed, short market trust/price relief, v1 architecture | owed builders and safe passage |
| Saltwake / Widowed Rain | water and coastal storms | storm injury exposure percentage | who sold the same rain twice? |
| Cinderhold / Hearth Beneath | distributed winter heat | heat injury exposure percentage | who pays to share the communal flame? |
| Drevlach / Tidal Ledger | future debts and ferry flow | Debt Tide injury severity | does an unpaid tomorrow own today's passage? |
| Orravane / Sky Below | load-bearing song | Falling Note strain severity | do missing notes endanger inhabited roofs? |
| Vathless / Cinder Spine | quarry instability | Quarry Split injury severity | who owns ancestral stone and wages? |
| Eillun / Hundred Unlived | protected identities | Census Sweep strain severity | whose consent authorizes a recorded existence? |
| Tharniv / Orchard of Kings | roots and unwritten law | Law Root injury severity | who owns a road no monarch authored? |
| Auvren / First Absence | memory and geometry | Unmooring strain severity | which mutually incompatible map deserves trust? |

**Example of player-learnable differences:** Living Roads is an existing Tenfold skill. It dampens Saltwake storm severity to 65% while the rite is active; the *same skill* dampens Orravane's sonic strain to 75%, with different pursuit speed. Tomorrow's Debt in Cinderhold raises current heat exposure to 125%, showing a real trade-off. These numbers are source rules and must be balance tested in Unreal; no extra immunity or bypass of existing timing and counterplay.

## Protection rules

- The Crossings v1 record is not migrated or reset.
- Existing Tenfold cast saves remain authoritative; the v2 projection cannot create a free spell or duplicate refund.
- Each outer realm has an independent array entry for care/inquiry rite bits and active rite/expiry. Restore is all-or-nothing; valid records of other realms cannot be quietly rewritten after one malformed entry.
- No distant NPC acquires personal firsthand experience from the saved global hub data.
- A repeat cast of one mastered rite doesn't produce another distinct municipal witness.
- No future-time economic rewards and no artificial full outer-world markets; only the actual already-supported purchase flow is modified.
- Temporary structures expire by the shared world clock, regardless of scene streaming; future renderer should never leave invisible collision behind.
- Public care and public inquiry can both coexist without overriding the protagonist's original endings.

## Windows Unreal engine test plan (not yet performed)

Compile UE/UHT, test all nine realm origin lookups, every temporary actor being initially hidden and noncollidable, the three architecture layouts, saved restoration after a restart, and the independent public markers for each realm. Verify character/AI collision paths aren't blocked by spawned proxies.

In Saltwake/Cinderhold, measure pre- and post-cast actual health pulse differences with the same pulse ID to ensure no double-damage. In each six later realms, measure actual injury or Strain respectively while retaining side-lane/guard counterplay and previously resolved hazard disablement. Inspect actual stalker/watcher swept movement before, during and after a cast, confirm no remote multiplier leakage. Test first-visit saves, corrupted array sizes, negative and impossible bit masks, save write injection and old-school Reach history.

Speak to the named local witness and an actual secondhand listener. The direct line must be locally authored; hearsay must identify a valid speaker ID. No remote character should be credited with a local cast or reveal protected names.

Check Crossings purchase price with/without the 20 trust adjustment using identical item and money snapshot. Other realm economies remain a staged roadmap, not an invented finished feature. Add accessibility options for high-contrast hazard cues, screen-reader-equivalent text, subtitles, gamepad navigation, low vision and voice options.

## Following production milestone: Local Material Consequence

Real producer-consumer material inventories per settlement, finite deliveries, worker routines, damaged/repaired structures, permissioned water/heat/ferry ledgers, livelihood prices and player-driven relief/complicity. Expand NPC next-day visits and integrate actual route planning and enemy tactics. This must preserve evidence-source limits and not replace the game with freeform AI-generated quest truth.
