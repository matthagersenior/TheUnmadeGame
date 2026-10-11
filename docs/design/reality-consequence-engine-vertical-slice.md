# The Unmade — Reality Consequence Engine v1, implementation and acceptance

**Status:** Source implemented; Windows Unreal 5.8 Editor/PIE and authored art not yet verified.  
**First vertical slice:** The Crossings (focus radius 2600 cm). **Not a claim of all-nine-realm integration.**

## Actual casting-to-consequence pipeline

1. Player selects any of the ten existing Tenfold rites. `TenfoldChronicle::Invoke()` remains the authoritative source of evidence, mastery, strain cost, cooldown, and expiry.
2. Existing world geometry/magic and combat powers still execute. After a successful Tenfold save, the caster's real location, actual rite ID, duration and mastery go to `AUnmadePrototypeHub::RecordRealityConsequence`.
3. Outside the Crossings focus, the new rules do **nothing**. Inside, check for actual stable residents within 720 cm with unobstructed line of sight. No offscreen imagined witness or global approval is permitted.
4. The single `RealityConsequenceLedger` sets time-bound architecture and combat profile, and if the cast is mastered *and* physically observed adds the corresponding distinct evidence bit. Repeat casts preserve the one bit; no farmable public status.
5. Save the changed civic record into optional schema-1 fields in the same world slot. Only after success update world actors and return a success to the character for a sourced personal `Reality.CareWitnessed` / `Reality.InquiryWitnessed` event.
6. Nearby actual NPCs who see the real cast store a direct observation. Existing one-hop nearby spoken gossip can transmit it as rumor with an actual speaker ID, never as another eyewitness claim.
7. An enemy inside the same focus and time window uses that rite's speed multiplier for actual swept pursuit/retreat movement; outside the region, or when the effect expires, movement returns to its existing source policy.
8. Physical temporary actors and persisted public markers are mutually specified by the one ledger. The player's J journal and a future Blueprint widget share the same record.

## Tenfold profile acceptance matrix

| Rite | Architecture | Enemy speed | Civic school |
|---|---|---:|---|
| Unwrite Law | Strained Span | 140% | Inquiry |
| Witnesscraft | Safe Span | 75% | Care |
| Borrowed Lives | Double Threshold | 120% | Inquiry |
| Legacy Forging | Safe Span | 85% | Care |
| Living Roads | Safe Span | 80% | Care |
| Tomorrow's Debt | Strained Span | 130% | Inquiry |
| Understanding Bosses | Safe Span | 70% | Care |
| Paradox Convergence | Double Threshold | 125% | Inquiry |
| Oathbinding | Safe Span | 75% | Care |
| Cartography | Safe Span | 90% | Care |

The ten legacy rites themselves remain fully distinct. This maps the **cross-domain consequence** of each cast; it does not replace their authored spell, equipment and guardian behavior.

## Public reactions as testable evidence, not alignment

- **Zero or one distinct witnessed mastered rite on a side:** nobody can erect a permanent city consensus. Temporary structures and individually sourced memories still exist.
- **Two distinct care rites, witnessed:** Shared Passage; physical protective civic marker is enabled.
- **Two distinct inquiry rites, witnessed:** Public Inquiry; physical survey question marker is enabled.
- **Both kinds meet thresholds:** Contested Accord; **both** physical markers remain in the city simultaneously. No side silently wins.
- **No eyewitness despite a real cast:** temporary architecture/combat still occurs; no public evidence bit added. A remote resident never gains direct knowledge.
- **Repeated single cast:** cannot increment a mask a second time. Existing rite cooldown is also unchanged.
- **Unmastered cast:** temporary local consequence is allowed but cannot create official long-lasting proof. This allows players to learn without forcing an irreversible civic reaction early.

## Authored feedback and environmental storytelling

The rite activation must briefly affect physical traversal and enemy positioning, not just repaint UI. A safe span is a real 3D collidable prototype actor; the strained span obstructs space and makes nearby enemies move faster. Double threshold is visibly distinct. These props are proof of source wiring, **not credible final navigational architecture**. The post-UE production pass must build meaningful approaches, ramps, safe edges, stable collision and opportunities for counterplay.

Nearby residents should say one of these variants only if a true observation is in their personal memory:

- Direct/care: “I saw the reality rite make a passage less dangerous. The debts it left are still ours.”
- Named rumor/care: “I heard from [stable speaker] that a rite opened a safer way; I did not see it.”
- Direct/inquiry: “I watched the structure change under the rite. I want the surveyors to examine it.”
- Named rumor/inquiry: “I heard from [stable speaker] that a structure moved. I have not inspected it.”

There is no claim that the NPC is suddenly conscious of an unrelated remote realm. Localized voice/lip sync and gesture staging remain future tasks.

## Save and test risks

Old schema-1 saves set `bHasRealityConsequenceSnapshot=false`, with empty masks, no active rite and zero expiry. Restoring verifies that every bit belongs to the rite's specified school, the 10-bit range is valid and two schools never claim the same rite. Every temporary record is finite, has a valid rite ID, and expires using the same saved world clock. Corrupt records activate preservation mode; failed writes roll back the new ledger and do not toggle a civic structure.

Native CI tests cover ten profiles, durations, no witness, unmastered casts, duplicate cast spam, three public outcomes, expiry, save restores, invalid masks and invalid clocks. Python source CI tests verify that Tenfold actually calls the Hub, an enemy actually consumes the view, world actors use that very same ledger, NPC memories are source-limited, and all optional save fields are guarded.

## Real Windows UE acceptance (not yet completed)

- Compile UHT and game source. No acceptance from portable GCC alone.
- Verify all ten rites really invoke the shared hub only after strain/cooldown success, and none can accidentally grant duplicate NPC/civic effects if persistence fails.
- View/collide with the three transient structures; verify active times, camera, cover, ramps, NPC and player navmesh continuity from different approach directions.
- In combat with stalker/watcher, record measured pursuit speed before/during/after 70% vs 140% effects, and ensure no multiplier leaks outside the 2600 cm zone.
- Prove a false/no-observer cast cannot generate a civic marker; two **different** directly witnessed mastered casts can; exactly one repeated cast cannot.
- Test directly witnessed vs spoken rumor branches, trust/fear, persisted NPC memory, accessible subtitles/interaction hints, and clean gamepad options.
- Restart after a temporary cast, cross expiry boundary, and verify public structures persist while old temporary actors expire.
- Test invalid masks and simulated failed saves, ensuring all other established quest outcomes survive.
- Profile TActorIterator interactions and event-driven visibility updates. The NPC/social event remains limited by existing actor proximity, not a per-frame all-realm broadcast.

## Next expansion wave

A **realm policy table** should map the same consequence events onto different local systems: Bellwold winter heat and protection, Paperhaven index/consent, Saltwake rainwater trade, Cinderhold kiln logistics, Orravane sonic supports, Drevlach ancestral occupation and the remaining worlds' specific constraints. Then add authored path/cover deformation, tactical enemy reactions beyond speed, post-event resident duties, physical messengers and cinematic reaction chains. None of these is marked implemented today.

Volume X §21, **The Law of Consequence**, is the canonical authored story rule. It must remain consistent with every book, adaptation, gameplay table and saved-world migration.
