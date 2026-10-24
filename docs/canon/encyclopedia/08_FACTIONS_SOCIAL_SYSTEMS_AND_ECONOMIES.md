# Volume VIII — Factions, Civic Institutions, Labor, Reputation and Finite Economy

**Canon class:** Source-level faction chronicle and living-economy rules; game-engine presentation and save integration remain unverified. The starting Reach factions below are not assumed to control other civilizations. Each realm has its own institutions, customs and ethical disputes (see Volume I).

## Three distinct starting faction chronicle arcs

### The Refuge Compact — `faction.Refuge`

**Purpose:** Find shared light; record witnesses; decide who may shelter.  
**Named sequence of witnesses:** `npc.bellwold.lamplighter.001` → `npc.bellwold.matron.001` → `npc.bellwold.guard.001`  
**Contract:** Each stage requires speaking to the correct resident; the third requires valid local evidence. The final choice is deliberate and exclusive. Ending choices: **Solidarity** or **Truth**, neither a mandatory moral alignment. Resolving the arcs is not an invisible Nhal-Vey gate.

### The Redacted Charter — `faction.Archive`

**Purpose:** Rescue testimony; verify the record; decide whether to publish.  
**Named sequence of witnesses:** `npc.paperhaven.scribe.001` → `npc.paperhaven.registrar.001` → `npc.paperhaven.archivist.001`  
**Contract:** Each stage requires speaking to the correct resident; the third requires valid local evidence. The final choice is deliberate and exclusive. Ending choices: **Solidarity** or **Truth**, neither a mandatory moral alignment. Resolving the arcs is not an invisible Nhal-Vey gate.

### The Roadbound Assembly — `faction.Roadbound`

**Purpose:** Gather three roads' testimonies; debate who owns the crossings.  
**Named sequence of witnesses:** `npc.guard.001` → `npc.roadwarden.001` → `npc.welllistener.001`  
**Contract:** Each stage requires speaking to the correct resident; the third requires valid local evidence. The final choice is deliberate and exclusive. Ending choices: **Solidarity** or **Truth**, neither a mandatory moral alignment. Resolving the arcs is not an invisible Nhal-Vey gate.

**Reputation rules (from C++):** unresolved stage × 5; solidarity result 40; truth result 25. A reputation score cannot substitute for genuine evidence or grant NPC omniscience. **Authority:** [Faction Chronicle](../../../Source/TheUnmadeGame/Public/World/UnmadeFactionChronicleRules.h).

## The three physically authored starting settlements

### The Crossings — `settlement.Crossings`

**Social inclination:** Independent  
**Source prototype position:** (0, 0)  
**Arrival:** Three roads meet where two histories disagree. Shelter and inquiry still argue over the passage.  
**Night:** Every door in the Crossings casts a different shadow than its owner.

### Bellwold Refuge — `settlement.Bellwold`

**Social inclination:** Shelter  
**Source prototype position:** (-18000, 0)  
**Arrival:** The bell-keepers offer sanctuary first and questions later. They live by shared labor and remembered loss.  
**Night:** Lanterns burn for people erased from the official ledgers.

### Paperhaven Archive — `settlement.Paperhaven`

**Social inclination:** Research  
**Source prototype position:** (18000, 0)  
**Arrival:** Names are registered, testimonies argued, and impossible histories bound into books.  
**Night:** The archive locks its doors, but missing pages whisper after sundown.

These source coordinates are prototype locations, not claims of finalized production geography.

## Named workers and obligations across all nine realms

### Kesta the porter — `npc.marketporter.001`

**Realm:** ThreefoldReach  
**Occupation:** market porter  
**Household:** Kesta's rented room

### Mera the tide courier — `npc.saltwake.courier.001`

**Realm:** WidowedRain  
**Occupation:** rain courier  
**Household:** Mera's tide-side home

### Bes the coal runner — `npc.cinderhold.courier.001`

**Realm:** HearthBeneath  
**Occupation:** ember runner  
**Household:** Bes's shared hearth

### Orshel Drywake — `npc.tidal.ferryman.001`

**Realm:** TidalLedger  
**Occupation:** ferryman  
**Household:** Orshel's dock shelter

### Vhaurn Counterweight — `npc.sky.structuralist.001`

**Realm:** SkyBelow  
**Occupation:** support steward  
**Household:** Vhaurn's suspended home

### Torrun-Mortar — `npc.bones.mason.001`

**Realm:** CinderSpine  
**Occupation:** seam mason  
**Household:** Torrun's quarry dwelling

### Auvet Keep-Quiet — `npc.unlived.caretaker.001`

**Realm:** HundredUnlived  
**Occupation:** refuge caretaker  
**Household:** Auvet's unnamed refuge

### Imro Root-Vote — `npc.orchard.gardener.001`

**Realm:** OrchardOfKings  
**Occupation:** root gardener  
**Household:** Imro's common plot

### Thae of the Near Door — `npc.absence.caretaker.001`

**Realm:** FirstAbsence  
**Occupation:** threshold keeper  
**Household:** Thae's doorway

**Economic conservation law:** Municipal treasuries begin with 32 marks per realm. One verified working shift transfers 2 marks from the municipal employer to the named worker or records 2 owed marks when funds are unavailable. No money is generated from a wage, and obligation balances remain attributable. A strike or walkout is not a fabricated completed shift. Worker wages are not player inventory, and delivery receipts are not global witnesses.

## Two finite cross-realm trade agreements

### WidowedRain → HearthBeneath — `trade.SaltToHearth`

**Cargo:** 5 units of sealed rainwater casks  
**Payment:** 6 marks per verified delivery  
**Purpose:** kiln cooling and safe drinking

### HearthBeneath → WidowedRain — `trade.HearthToSalt`

**Cargo:** 5 units of common ember baskets  
**Payment:** 6 marks per verified delivery  
**Purpose:** drying storm-soaked shelter beds

**Snapshot integrity:** Cargo can be carried only on an actual registered route. Delivery increments happen on verified arrival, not when NPCs elsewhere hear rumors; stock changes preserve units and money conservation. Corrupt negative balances, implausible delivery counts and inconsistent receipts are rejected before loading a save.

**Restore validation order:** A saved route's stock, delivery, payment, and
consumption totals are bounded and reconciled before those totals are used to
validate worker receipts. Invalid snapshots are rejected before they can
change the live economy. This preserves the same finite-treasury and witnessed-
delivery rules during recovery as during ordinary play.

## Nine cultural principles and civic differences

- **ThreefoldReach:** The Wayward Charter. Principle: A witnessed promise outranks an inherited seal. Community dispute: Shelter requires trust; records demand difficult public consent
- **WidowedRain:** The Rainward Account. Principle: Water belongs to its keepers only while they can prove it was shared. Community dispute: A broker sells remembered water to families who need actual rain
- **HearthBeneath:** The Ember Commons. Principle: A fire is a promise to the next household, not hereditary property. Community dispute: Protecting an ember can become a justification for excluding outsiders
- **TidalLedger:** The Estuary of Owed Days. Principle: Nobody may pledge the tomorrow of a person who cannot consent. Community dispute: Relief now may hide the architecture of generational exploitation
- **SkyBelow:** The Downward Cantors. Principle: No building should stand by erasing the person holding it up. Community dispute: Structural safety depends on forbidden unpaid labor
- **CinderSpine:** The Seam-Walkers. Principle: Stone cut from history still belongs to the people erased with it. Community dispute: The mine supports homes while it un-makes the lineages that built them
- **HundredUnlived:** The Unentered Citizenry. Principle: The right to be sheltered cannot depend on surrendering an origin. Community dispute: Recognition may expose the very people it is meant to protect
- **OrchardOfKings:** The Untitled Courts. Principle: No law is just because its author wears a name. Community dispute: Refusing power can conceal power exercised without consent
- **FirstAbsence:** The Before-Witnesses. Principle: More than one remembered beginning may be true without erasing any witness. Community dispute: An imposed single origin would stabilize space by invalidating lives

**Source links:** [Faction Chronicle](../../../Source/TheUnmadeGame/Public/World/UnmadeFactionChronicleRules.h) · [Living Economy](../../../Source/TheUnmadeGame/Public/World/UnmadeLivingEconomyRules.h) · [Settlement Registry](../../../Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h) · [Nine Realm Cultures](../../../Authoring/world_content_pack.json).
