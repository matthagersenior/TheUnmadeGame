# Local Material Consequences v3 — precise playable-source / UE handoff

**Canon:** Volume X Chapter 23. **Implementation baseline:** main merge of PR #41. This is a first deterministic nine-realm economy slice, not the final material sim.

## Workday rules

- World day takes 1200 real-time game seconds. The nine-material ledger advances once per world-day transition, not every frame or time the journal opens.
- Each town starts with a finite store and household reserve; production depends on that realm's daily labor, output-per-worker, infrastructure condition and proven local civic support.
- Work makes stock **only in the store**. Ordinary local haulage removes actual stock and credits a bounded quantity to households. A household consumes its separate daily requirement.
- When homes receive too little, the shortfall increments the shortage-day record and lowers structural integrity. Lower integrity can reduce tomorrow's effective workers.
- A locally supported Shared Passage or Contested Accord (from two different mastered, physically witnessed caring rites) may supply one additional daily worker; no created goods or civic record propagates across realms.
- All material names, daily quantities, repair costs and base workforce values are defined only in `UnmadeMaterialConsequencesRules.h`, indexed by the stable nine-item atlas enum.

## E interaction route: three distinct physical sites

For each realm `i`, its physically located points are relative to that realm's atlas center:

| Site | Relative X / Y | Trigger | Source-state effect |
| --- | --- | --- | --- |
| `Material.i.Store` | +1000 / -830 cm | E within 250cm and clear line of sight, item closer than a conversation target | Removes five units from *this realm's* store and creates one saved carried parcel |
| `Material.i.Homes` | +1000 / -230 cm | E within 250cm with an actual parcel from this same realm and sufficient room in household reserve | Removes carried custody, adds exactly five units to homes, increments delivery history |
| `Material.i.Repair` | +1550 / +300 cm | E, actual wear below 90 and sufficient local stock | Removes that realm's finite repair cost; raises integrity by 18 up to 100, increments counted repairs |

Physical `Material.i.Shortage` and `Material.i.Damaged` objects show actual conditions. A repaired marker becomes visible only after a successful paid repair and sufficient integrity. These cubes must be replaced with believable landmark models/animation, not mistaken for final production assets.

## Delivery recovery and save invariants

- One cargo globally at a time, carrying stable origin realm ID, quantity exactly five, and real collection world-day.
- Wrong-realm delivery returns a block; goods never morph into another culture's stock.
- If stock falls below five, no pickup. If household capacity would exceed 100, no handoff. Neither failure changes the journal or creates duplicate rewards.
- If infrastructure is at 90 or above, repair is `NoWork`. A valid repair consumes real stock and cannot be repeated indefinitely to farm credit.
- New optional SaveGame fields are nine-element arrays for day, store, homes, integrity, produced, consumed, shortage days, labor hours, deliveries and repairs; a one-entry cargo origin/quantity/collection day lives alongside.
- Old version-1 saves without material fields begin with normal initial stock **at their current existing world day**, never 100 days of fabricated work. Invalid nonempty arrays/negative or future quantities and corrupted cargo block world writes rather than replacing the old slot.
- A failed actual SaveGame write restores the in-memory town snapshot and cargo; no false delivery, repair or printed "completed" result.

## Existing character behavior

When a *local public board* records shortage, available named courier/merchant roles head toward their own realm's warehouse and other suitable local roles toward homes. When structural integrity is low, idle guards/wanderers turn toward repair stations. The pre-existing authored immediate reactions to dangers, rumors and personally witnessed events override routine duties. Residents use their old stable IDs, roles, daily homes and authored biography text.

**Important limit:** the current production count is an aggregate shift figure, not a claim that each actual resident NPC's individual work shift already credits an exact labor contract. Later versions should record named workers, sickness/injury, consent, payment, absenteeism and household-level deliveries without creating remote knowledge.

## Nine-world acceptance

| Realm | Finite unit / specific problem |
| --- | --- |
| Crossings | bracing fittings, bridge workers and unpaid maintenance |
| Saltwake | rainwater casks delivered to storm shelters |
| Cinderhold | ember baskets moved from common kiln to cold dwellings |
| Drevlach | iron for actual ferry maintenance, no magically issued debt |
| Orravane | resonant support pins for occupied suspended houses |
| Vathless | bracing stone and dangerous seam repair |
| Eillun | consent-safe shelter kits, no mass identity publication |
| Tharniv | communal root grafts without awarding ownership |
| Auvren | memory anchor stakes and competing origin testimony |

## Mandatory Windows Unreal acceptance (pending)

1. UE 5.8 full UHT compile, run PIE and travel to every spawned site; check actors are truly reachable without collider blocking and E prioritizes a nearer resident.
2. Advance exactly one game-day boundary (before/after) and verify each local store's output, haulage, consumption, condition and time; save/restart and verify *no second production for the same day*.
3. Test real E pickup, forced failed write, return to the same site, retry, and restore actual pack custody on restart. Test wrong-region handoff and a second attempted packet without delivery.
4. Trigger Cinderhold shortages; verify named worker routines, correct journal amounts, damaged marker, a paid repair and later restored condition. Test all nine realms separately with original atlas transport.
5. Verify earlier Witness Braid/Return, named relationship memories, nine-realm cast consequences, main ending, local hazard pressure and cross-realm save states remain valid.
6. Add readable interact tooltips, audible and subtitled inventory state, scalable text, gamepad prompts, tactile/contrast-independent condition cues and fallbacks for low-vision players.
7. Stress test years of simulated in-world progression and corrupt/future save states. Check no overflow, negative stocks, false remote events or actor duplication.
8. Complete the visually authored warehouses, physically credible delivery props, NPC animations, distinct route and weather sound, readable home interiors and real repair consequences.

## Beyond v3

Build a second slice with a durable inter-region procurement contract where a named person authorizes a finite cargo, a route must actually be travelled, payment is recorded by the existing marks economy and receipts reconcile at both ends. Only then extend into per-person employment, injury, wages, civic patronage, climate-dependent crops/fuel, meaningful repair/evacuation quest branches, companion relationships and saved multi-season changes. No generic global economy or free omniscient NPC dialogue.
