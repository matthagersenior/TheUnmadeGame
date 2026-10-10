# THE UNMADE — Living Canon and Realm Continuity
**Volume II / October 10, 2026 | Source-aligned narrative guide and expansion standard**

## Four non-negotiable RPG pillars
1. **Freedom with consequences:** combat, investigation, diplomacy, crafting and reality manipulation should offer viable, materially different resolutions; the world reflects the choice.
2. **A genuinely living world:** NPCs have local routines, roles, memories, firsthand evidence and rumor, not universal awareness or quest-vending behavior.
3. **Challenge without wasted time:** mastery requires evidence, skill combinations, earned tactics, commitment and fair difficulty, not arbitrary repetitive grinding.
4. **Exploration worth the detour:** clues reveal new routes, capabilities, people, and histories instead of becoming a disposable collection checklist.

## Signature-gameplay requirement
Every named quest must name the existing signature mechanic(s) it uses and explain a **world-state consequence** that can be seen after return. No online LLM is necessary for game function. Unwrite Law, Witnesscraft, Borrowed Lives, Legacy Forging, Living Roads, Tomorrow's Debt, Boss Understanding, Paradox Convergence, Oathbinding and Unreliable Cartography must remain mechanics rather than plot-only language.

## Implementation truth
**Playable-source integration:** three starting villages in The Threefold Reach and two frontier outposts in Saltwake/Cinderhold; Bellwold's separate "The Second Night" aftermath is implemented in Unreal C++ source as graybox actions, with independent C++ tests but **no engine compile or playtest**.
**Authored rule models:** `UnmadeRealmAftermathRules.h` establishes nine separate first-after-arc narratives, evidence paths, witness gates and permanent choices; only the first three realm slots have physical footholds, and the other six are **design definitions, not navigable levels**.
**Concept development:** characters bearing `npc.tidal.*`, `npc.sky.*`, `npc.bones.*`, `npc.unlived.*`, `npc.orchard.*`, or `npc.absence.*` are narrative identifiers for future cast—not actors currently spawned.

## Bellwold Refuge — The Second Night (implemented source expansion)
Hessa asks the returning player to choose which emergency the rebuilt refuge can face first: surviving another night, or exposing the erased census. Inspect the relief cache and speak with Sorin to shelter families; alternatively inspect the altered register and seek Ivera's testimony to publish names publicly. Both require proximity to the matching physical evidence and named witness. A wrong choice cannot complete the quest. The branch persists in a different visible graybox landmark and one unique epic item: the **Ward of the Second Night** or **Lantern of Unredacted Names**. NPCs distinguish eyewitness reporting from hearsay. **The shelter-capacity economy and nighttime threat simulation remain unimplemented.**

## Nine realm aftershock narratives
### 1. The Threefold Reach — The Road After the Rescue
At The Crossings, displaced travelers follow a bridge that each witness claims was built in a different year. The road warden needs a new public accord. Examine the Shared Route stones or the censored Toll Archive; Orrel the bridge keeper can describe safe crossing, while the original records keeper supplies evidence of hidden toll claims. Shared passage physically opens an alternative route; published toll evidence changes the charter and the merchants' obligations.

**Signature interplay:** Living Roads + Witnesscraft. **Design acceptance:** Players should discover that two honest accounts can disagree without making the people dishonest.

**Aftermath mechanics:** A previously resolved local arc and visited location unlock the follow-up. An authored physical clue is inspected, then a matching named witness must confirm either care or open truth. A committed outcome has separate persisted effects; replaying the same dialogue cannot multiply rewards. The live implementation requires new environment assets, named NPCs, sound and meaningful interactive hazards.

### 2. The Rain That Forgot the Sea — The Storm's Second Invoice
Saltwake's community survives the harbor warden's first decision, then discovers that the fresh-water invoices were issued for rain that never fell. Visit the communal cistern with rain keeper Thenn, or compare the rain ledger with drowned navigator Sella. The first path funds a shared freshwater collector; the second exposes the false price and invites retaliation from profitable trade houses.

**Signature interplay:** Unreliable Map + Witnesscraft. **Design acceptance:** Map evidence changes safe routes, not just a collectible counter.

**Aftermath mechanics:** A previously resolved local arc and visited location unlock the follow-up. An authored physical clue is inspected, then a matching named witness must confirm either care or open truth. A committed outcome has separate persisted effects; replaying the same dialogue cannot multiply rewards. The live implementation requires new environment assets, named NPCs, sound and meaningful interactive hazards.

### 3. The Hearth Beneath — The Ember That Chose No Heir
After the hearth compact, Cinderhold's ember warden finds a deed claiming private rights over a flame that belongs to every family. Help Ovenna inspect the common kiln's capacity, or ask Tarin to read the ancient Ember Deed, which may reveal a claim that never could have existed. A public kiln alleviates cold, while revealing the forged deed threatens the ruling keeper's legitimacy.

**Signature interplay:** Oathbinding + Legacy Forging. **Design acceptance:** Sheltering neighbors is distinct from uncovering an institution's wrongdoing.

**Aftermath mechanics:** A previously resolved local arc and visited location unlock the follow-up. An authored physical clue is inspected, then a matching named witness must confirm either care or open truth. A committed outcome has separate persisted effects; replaying the same dialogue cannot multiply rewards. The live implementation requires new environment assets, named NPCs, sound and meaningful interactive hazards.

### 4. The Sea of Written Debts — The Unpaid Person
At a drowned counting house, creditors discover debts issued to people who were never born. A ferryman can help organize temporary refuge bonds, while a witness can testify that legal memory itself was sold. The player must choose between immediate debt cancellation and the broader public proof that could overturn a region's economy. Every loan of Tomorrow's Strength must still be repaid.

**Signature interplay:** Tomorrow's Debt + Oathbinding. **Design acceptance:** Future realm only: first make the city, debt system, witnesses and bank physically explorable.

**Aftermath mechanics:** A previously resolved local arc and visited location unlock the follow-up. An authored physical clue is inspected, then a matching named witness must confirm either care or open truth. A committed outcome has separate persisted effects; replaying the same dialogue cannot multiply rewards. The live implementation requires new environment assets, named NPCs, sound and meaningful interactive hazards.

### 5. The Upside-Down Choir — The Note That Held a Home
When a suspended district begins to fall, its choir can stabilize gravity only if it sacrifices one of its voices. Study the harmonic brace with a structuralist or the silenced verse with a dissenter. The player must navigate a city whose ceilings are another district's streets; unwrite gravity sparingly so a rescue does not collapse an adjoining home.

**Signature interplay:** Unwrite a Law + Paradox Convergence. **Design acceptance:** Future realm only: geometry and audio must support genuine traversable counterplay.

**Aftermath mechanics:** A previously resolved local arc and visited location unlock the follow-up. An authored physical clue is inspected, then a matching named witness must confirm either care or open truth. A committed outcome has separate persisted effects; replaying the same dialogue cannot multiply rewards. The live implementation requires new environment assets, named NPCs, sound and meaningful interactive hazards.

### 6. The Bones of Yesterdays — The Quarry of Other Yesterdays
The mountain seam-walkers mine yesterday's sediment to build today's homes. Each new stone silently erases a descendant from living memory. A mason argues for halting a dangerous quarry; a descendant seeks proof of the vanished bloodline. Historical mining must change future families and available crafting patterns, not reward endless digging.

**Signature interplay:** Legacy Forging + Borrowed Lives. **Design acceptance:** Future realm only: timeline changes need deterministic bounds and reversibility warnings.

**Aftermath mechanics:** A previously resolved local arc and visited location unlock the follow-up. An authored physical clue is inspected, then a matching named witness must confirm either care or open truth. A committed outcome has separate persisted effects; replaying the same dialogue cannot multiply rewards. The live implementation requires new environment assets, named NPCs, sound and meaningful interactive hazards.

### 7. The Hundred Unlived — The Street of Unclaimed Birthdays
The city was built for people whose births never happened. Its caretakers shelter unnamed arrivals, while officials demand a census before they recognize them. The player can create a shelter ledger without identities or expose a public voice ledger that gives the neverborn legal standing—and makes them visible to hostile powers.

**Signature interplay:** Witnesscraft + Borrowed Lives. **Design acceptance:** Future realm only: agency and privacy matter more than simply accumulating names.

**Aftermath mechanics:** A previously resolved local arc and visited location unlock the follow-up. An authored physical clue is inspected, then a matching named witness must confirm either care or open truth. A committed outcome has separate persisted effects; replaying the same dialogue cannot multiply rewards. The live implementation requires new environment assets, named NPCs, sound and meaningful interactive hazards.

### 8. The Orchard of Unwritten Kings — The Crown No One Wanted
The untitled courts grow trees from the laws their rulers refused to enact. When a neglected root destroys a village road, a gardener asks for a common orchard; a historian asks the player to reveal the last unclaimed throne. Refusing power is not sufficient unless ordinary people accept the consequences.

**Signature interplay:** Oathbinding + Living Roads. **Design acceptance:** Future realm only: governing needs consent, visible civic changes and meaningful alternate paths.

**Aftermath mechanics:** A previously resolved local arc and visited location unlock the follow-up. An authored physical clue is inspected, then a matching named witness must confirm either care or open truth. A committed outcome has separate persisted effects; replaying the same dialogue cannot multiply rewards. The live implementation requires new environment assets, named NPCs, sound and meaningful interactive hazards.

### 9. The Place Before Place — The Place After the Ending
At the universe's threshold, the player finds two mutually coherent beginnings. One world could be kept safe by stabilizing a shared path; another preserves many incompatible origins in public testimony. This must not be a surprise single-button ending: prior settlements, witnesses and realm arcs should shape the options, and older communities must remember the result.

**Signature interplay:** Unwrite a Law + Unreliable Map. **Design acceptance:** Future realm only: not a shipped ending, and no singular canonical moral answer.

**Aftermath mechanics:** A previously resolved local arc and visited location unlock the follow-up. An authored physical clue is inspected, then a matching named witness must confirm either care or open truth. A committed outcome has separate persisted effects; replaying the same dialogue cannot multiply rewards. The live implementation requires new environment assets, named NPCs, sound and meaningful interactive hazards.
## Expansion and illustrated-package update protocol
A game change is **not ready to describe as complete** until it has been reflected in:
- **Playable source and tests**, including explicit failure, save/reload and alternate-outcome verification.
- **Canon source:** this guide, `docs/lore/CONTINUITY_LOG.md`, and the exact affected named NPC/item/realm entries.
- **Illustrated package:** update the editable master DOCX, PDF, art atlas, source appendix, manifest, and ZIP as one dated edition. Illustrations are concept art, never evidence of Unreal engine rendering.
- **Cross-pillar review:** document where freedom, world memory, earned challenge and significant discovery appear in the actual player loop.

Do not promote future-realm concepts as constructed levels. Do not equate green native C++ source checks with Unreal compilation, balanced bosses, validated hours of campaign play, or a playable build. Every later release should be regenerated from the tracked canon and supplied as a verified artifact; the CI content-contract guard only verifies documentation coverage, not image-generation or visual QA.

## Volume III source expansion — The water gate and the ember vent (October 10, 2026)
Saltwake's return begins only after Harrow has recorded a final port settlement, then demands opening the physical **Storm Sluice** before either the common cistern or rain ledger can be examined. Thenn witnesses the shared-water route; Sella witnesses the public-price route. The unlocked sluice makes both investigations reachable, while the final resolution controls distinct traversable boardwalks and settlement works. No single evidence click can bypass the mechanism or witness.

Cinderhold's return requires its first hearth accord and the opening of the **Heat Vent**, allowing investigation of the common kiln with Ovenna or the disputed Ember Deed with Tarin. A concluded route changes which communal structures and mountain walks are usable; no NPC acquires magical knowledge of the result. Both realms preserve their player's choices and restored physical geometry. These are Unreal C++ graybox source targets, not engine-tested adventures.

The six later realms each have a named physical mechanism in `UnmadeRealmAftermathRules.h`. Those are contracts for future places, **not** proof their geometry, actors, cities, audio or hazards have been constructed. Later-world production must first create traversable terrain and encounters, then connect mechanism activation, evidence, witnesses, save rollback, local reactions and irreversible consequences. Each realm must pass the same four standards before it can be called playable.

## Release ledger
- **Volume I (October 9, 2026):** initial illustrated canon; nine realm atlas, 64 current NPCs, 58 then-authored items, three bosses, ten rites and six confluences.
- **Volume II (October 10, 2026):** Bellwold's Second Night, two epic items (60 total), living community consequence system, nine distinct proposed aftershock arcs, continuity test and revised illustrated master.
- **Volume III source pass (October 10, 2026):** physical mechanism gates and two alternative investigation routes for Saltwake and Cinderhold, with persistent geometry, source tests, and authored later-realm mechanism contracts. The illustrated master is pending regeneration and does not yet constitute an updated downloadable edition.
