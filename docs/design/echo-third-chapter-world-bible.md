# THE UNMADE — Six Echo Quests, Third-Chapter Narrative and World Implementation
**10 October 2026 — canonical source-backed side chapters, not a claim of finished Unreal levels.**

## Design intent
Every outer realm now has three different kinds of return: (1) the first practical intervention establishes a crossing, (2) the realm aftermath asks how people live with that intervention, and (3) an **optional Echo Quest** interrogates a person or object excluded by both official narratives. This is not a repeatable quest board: the chapter completes once, its two evidence sources are unique, its witnesses are locally situated, and its final resolution places a public object in the home realm and a physical dispatch in one connected land. Neither endpoint ends the game or erases the other chapters.

The third chapters require completion of that specific realm's first story and its independently confirmed aftermath. The player starts by visiting its civic keeper again, inspects **both** separate clues (at south-side X -1700/Y +200, and the upper civic terrace X +1500/Y +1800), questions the matching witness, and operates an intervention on the upper terrace. The first press previews the exact lasting consequence; a **second press on the same intervention within six seconds** is required. The upper terrace is navigable only after earlier realm choices, so world-space return matters. The second clue cannot be substituted by repeating the first. Wrong witness/approach and corrupt save data fail without changing the world. All six choices have independent, optional progress and save arrays; they cannot be farmed for gear.

**Important:** These coordinates, cube actors and timed interaction calls are *source implementation*, not tested navigation, authored art or an accessible UI.

## Six authored stories

### 1. Drevlach — The Child Who Repaid Tomorrow
**Opening witness:** Ledgerkeeper of the estuary `npc.tidal.ledgerkeeper.001`, whose first fear is that forgiving a debt will erase a child owed only a safe passage. **Material testimony:** a cradle seal dated in a winter that has not occurred, and a lender's keel with payment marks from a ship that has never sailed.

**Personal accounts:** The ferryman `npc.tidal.ferryman.001` shelters an unnamed dependent and refuses to trade a child's name for loan relief. The record witness `npc.tidal.witness.001` personally signed a contract by lantern light yet can prove the signature was placed a century later. Both may be sincere: one knows the person, the other knows the fraud.

**Playable consequence:** Ring Refuge Bell and establish a free child-safe ferry whose names are held in trust, or Open Archive and publish the impossible loan's provenance so borrowers can inspect contracts. A dispatch of this choice materializes on a connected shore, inviting later travelers to follow the record. **Ability bridges:** Tomorrow Debt can reveal the cost of borrowing strength; Oathbinding provides thematic protection, but mastering either is not required to finish.

**Revision hooks:** Audio cadence: ledger scratch, absent oar strokes, a dry bell over black water. Scene motifs: etched winter-date and keel, no generic gold chest. Return hook: the personally protected child may later grow into someone who can refute the court's false chronology.

### 2. Orravane — The Silence Between Two Notes
**Opening witness:** Choir master `npc.sky.choirmaster.001`, facing a dilemma: a support chord keeps homes suspended but forces erased singers to support a city they cannot enter. **Material testimony:** an empty musical score with the missing rests still counted, and a ceiling nail with an imposed name.

**Personal accounts:** Structuralist `npc.sky.structuralist.001` counted the children whose houses depend on this cadence. Dissenter `npc.sky.dissenter.001` preserved forbidden notes and can name the citizens excluded from the choir. Their conflict is not ignorance; it is different legitimate responsibility.

**Playable consequence:** Shared Meter distributes the load so no singer bears it alone, or Voice Stage restores silenced names in an open forum at the cost of an unstable harmony. The neighboring realm receives a song dispatch, not supernatural NPC knowledge. **Ability bridges:** Unwrite Law and Paradox Convergence can later support optional advanced traversal or scene variation; the source Echo quest has no skill gate.

**Revision hooks:** Vertical street traversal, choreography of tuning forks, reversible gravity cues, listening-based paths instead of invisible leaps. The player's earlier first intervention affects which passage is visible, not whether the erased singers deserve empathy.

### 3. Vathless — The Mason's Borrowed Grandmother
**Opening witness:** Quarry seamwarden `npc.bones.seamwarden.001`, who profits from safe stone but has reason to fear the erased people once quarried as building material. **Material testimony:** mortar bearing an impossible family name and a portrait of a grandmother remembered by children who never existed.

**Personal accounts:** Mason `npc.bones.mason.001` proposes immediate braces for threatened homes; descendant `npc.bones.descendant.001` recognizes an ancestor denied by every census. An intact home does not automatically prove its foundation was ethical.

**Playable consequence:** Hearth Brace permanently supports threatened homes without removing more timeline stone; Ancestor Wall preserves erased family workers and forces a new legal account. A neighboring quarry route carries the choice as a visible stone dispatch. **Ability bridges:** Legacy Forging reads the deeds of an object; Borrowed Lives hints at the identity hidden by the portrait. Neither is mandatory.

**Revision hooks:** Distinct fracture sounds, hand-coded safe alternative footholds, genuine interrupted extraction and changed house geometry. Never represent human lives as a random mining resource.

### 4. Eillun — An Address For Nobody
**Opening witness:** Census keeper `npc.unlived.censuskeeper.001`, who can count everyone entering a city but cannot legitimately compel residents to accept a name. **Material testimony:** an anonymous house key and an unsent welcome letter to a person the government claims is absent.

**Personal accounts:** Caretaker `npc.unlived.caretaker.001` knows guests by their voluntary stories and will not disclose them. Challenger `npc.unlived.challenger.001` wants official recognition while retaining the right to say no. These are competing strategies for agency, not heroes versus villains.

**Playable consequence:** Quiet Home opens protected anonymous lodging, or Public Threshold grants voluntary and revocable civic standing without automatic publication. A neighboring archive displays the consequence only after an actual traveler can read it. **Ability bridges:** Witnesscraft enables future cross-checking; Borrowed Lives invites dialogue about identity, never permission to overwrite someone else's.

**Revision hooks:** UI must never force disclosure to proceed. NPC memory stays scoped to observed and explicitly communicated events. The census-sweep hazard has nonlethal strain, not a death penalty for anonymity.

### 5. Tharniv — The Coronation of the Soil
**Opening witness:** Untitled steward `npc.orchard.untitled.001`, who cannot speak in the name of a crown nobody consented to grant. **Material testimony:** circular root-votes pre-dating kings and an empty crown carved with a question about authorization.

**Personal accounts:** Gardener `npc.orchard.gardener.001` understands which real households rely on public harvest. Historian `npc.orchard.historian.001` has the law signed by people who declined rule. A non-king could still become a tyrant by administering access without consent.

**Playable consequence:** Harvest Commons makes a public produce route accessible, or Court Without King archives every competing title without crowning anyone. Cross-world effects should bring visible civic information into Orravane or other established adjacent lands, not broadcast a telepathic new reign. **Ability bridges:** Living Roads can visualize communal routes; Oathbinding can carry a voluntary obligation.

**Revision hooks:** Responsive roots as actual walkable/blocked geometry, seasonal cycles, public harvest maintenance and dissenting local perspectives. Avoid a collectible-crown kingmaking trope.

### 6. Auvren — The Witness Who Arrived Before You
**Opening witness:** The first witness `npc.absence.firstwitness.001`, whose record predates the player's arrival and whose existence resists a singular origin. **Material testimony:** a footprint left before the land was made, and two contradictory beginnings known to be true.

**Personal accounts:** Caretaker `npc.absence.caretaker.001` wants one reliable refuge crossing for people who are disoriented. Other witness `npc.absence.otherwitness.001` has personally lived both beginnings and refuses to let the safer story invalidate the other. Neither path is declared canonical truth by fiat.

**Playable consequence:** Harbor of Starts preserves a reliable threshold for future arrivals; Many Mornings maintains an accessible public hall with incompatible histories. Earlier realms retain their saved endings. The visible dispatch reaches one real adjacent realm after resolution, not every NPC's memory.

**Ability bridges:** Unwrite Law and Unreliable Cartography can someday make contradictory thresholds locally traversable. No earned ability is required to consent to either account.

**Revision hooks:** Reality shifts need readable rhythm and color-blind-safe cues; high stakes must remain learnable and reversible until the explicit final press. This ending is not an automatic ending of the game.

## Character writing and believable knowledge boundaries
All 18 NPCs above have stable IDs already used as source-spawned actors. For now their spoken base lines are partly derived from the original first-chapter/aftermath authorship. The new third-chapter prose is canonical dialogue *direction* and evidence description; it is not yet eighteen recorded voice performances. **A local witness knows their own evidence, not world-wide choices.** The remote dispatch is an inspectable material record at a linked destination and appears only once the quest is committed. It is not distributed into NPC memory automatically.

For final performance writing, every NPC needs: three ordinary routine greetings (morning, day, night), one firsthand reaction, one rumor reaction, one perspective on both outcomes, a refusal line, an assist line, voice/animation direction, and state-aware dialogue for earlier/aftershock/Echo quests. The existing stable source IDs should drive this content; do not replace their identities during cinematics.

## Ten signature abilities — authored integration matrix
The ten established abilities remain authored by `UnmadeTenfoldChronicle.h`, not invented here. The third chapters provide future application sites without pretending those sites already cast a rite:
- **Unwrite Law:** briefly alter a harmonic or origin rule, with visible limits and strain.
- **Witnesscraft:** label contradictory testimony as firsthand versus rumor, never force a decision for the player.
- **Borrowed Lives:** reveal alternate craft/ancestry clues and their personal costs, without impersonating NPC consent.
- **Legacy Forging:** incorporate one-time protection, discovery and reconciliation deeds into tool history.
- **Living Roads:** expose communal access paths caused by agreements, not a warp menu.
- **Tomorrow Debt:** borrow strength against a clearly shown due date; never transfer it to another person.
- **Understanding Bosses:** discover the human stakes beneath local guardians, with mercy and combat viable.
- **Paradox Convergence:** briefly compare mutually incompatible environments; preserve both witness accounts.
- **Oathbinding:** strengthen protection in return for a specific observable promise.
- **Cartography:** expose the difference between a speculative map and personally verified traversal.

Rite availability and progress must be read from the authoritative `UUnmadeTenfoldComponent`. The optional Echo quests intentionally have no hard rite prerequisite, to avoid blocking discovery or imposing grind.

## Implementation and actor contracts
- **World:** `AUnmadePrototypeHub::BuildEchoQuests` adds two distinct physical clues, two mutually exclusive civic controls and two outcome monuments in each of the six later realms. A physical neighbor marker uses the existing `AtlasPassages` graph and is visible only after commitment.
- **Progression:** `UnmadeCore::EchoChronicle` advances strictly dormant→investigate→testified→committed; both clue bits are required, witness must match one route, and the final world's physical control must match the testimony.
- **Save:** four independent six-element arrays, an optional schema-1 flag, corrupt-data rejection without partial application, and rollback when a save write fails.
- **Consent:** E at the selected civic control previews the irreversible effect; the same control within six seconds commits. No background dialogue generator has permission to change this decision.
- **Layout:** `UnmadeRealmGeometryRules.h` establishes that the southern and northern floors do not touch, the chosen bridge reaches both, and the civic gates have non-overlapping wall segments. Unreal collision/navigation must still be tested manually.
- **Journal:** the existing player journal now exposes per-realm third-chapter stage, both clues and ending.

## Production cut list for a Windows UE workstation
Do not describe six prototype cube camps as finished continents. Required: properly authored levels/World Partition, procedurally backed terrain, biome assets, navigation meshes, distinct cultures, music & sound layers, localized UI and controller interaction, visual combat tells, reliable save/load after crash, consent-driven NPC dialogue, the ability-site execution layer, durable persistent crowds and economies, practical QA of all **twelve** optional Echo outcomes. The first PC compile will expose integration errors that Linux-only model tests cannot detect.

**Source truth as of this draft:** canonical third-chapter quest rules and Unreal world wiring exist; the engine cannot be certified from GitHub source tests alone.
