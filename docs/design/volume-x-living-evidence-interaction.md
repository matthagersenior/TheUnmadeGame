# Volume X Chapter 18 — Living Evidence: interactive production contract

**Baseline:** Follow-up to merged Witness Braid PR #30. **Scope:** real source game mechanics, not just a speculative narrative.

## Core loop: one player's evidence is not every resident's knowledge

1. Travel to Bellwold, Paperhaven or the Crossings and physically inspect the existing relic within 280 cm unobstructed sight. InspectSite checks within 295 cm and writes the existing optional evidence snapshot.
2. Press **F9** to view the notebook (prototype: text overlay + log; future UMG can call `AUnmadePrototypeHub::GetEvidenceNotebook()` directly). Only personally inspected CALL.01/CALL.03/CALL.04 appear; speaker and best contradiction are included.
3. Resolve a legitimate local civic decision (e.g. Afterlight), return to the original relic and inspect it again. It changes meaning without erasing the initial observation.
4. When all three observed + one returned, F9 describes the comparison, not a universal verdict. Walk to Orrel's nail and use the existing protected F7/F8 two-press commitment to hang the Sheltered Thread or Public Docket.
5. One physically distinct marker persists. A real nearby witness with line of sight receives the corresponding `World.WitnessBraidShelter` or `World.WitnessBraidDocket` event.
6. Speak to an NPC near the act: a directly witnessed account says “I saw.” On subsequent local 8-second gossip iterations, the eyewitness may tell a nearby resident; the other person's saved observation is `Rumor` from that actual speaker ID and their line says they **did not** personally see the event.

## Trigger and failure table

| Player/engine action | Predicate | Persistent change | Recovery |
| --- | --- | --- | --- |
| E inspect relic | matching physical actor, accessible distance and visibility | existing FirstMask bit; return bit only after real local decision | Find unobstructed approach; impossible ID does not create a new relic |
| F9 show notes | Source has at least an observed relic | **none**; entirely read-only | Empty state explains missing requirements, no spoiler for unvisited objects |
| F7 or F8 at nail | all three FIRST and one RETURN bit, player within 295 cm | existing optional BraidOutcome once only | Same key twice within six seconds; failed SaveGame write restores pre-choice state |
| Spawn refuge cord | stored outcome 1 | state-derived visibility/collision only | Rebuild stage on startup; never show both |
| Spawn public docket | stored outcome 2 | state-derived visibility/collision only | Rebuild stage on startup; never show both |
| Witness report | NPC within 720 cm, clear sight | local NPC Witness event with GUID and exact branch | Offscreen remote NPC learns nothing |
| Local hearsay | eyewitness speaker + nearby listener within 400 cm, line of sight | deduplicated Rumor with speaker ID and original event GUID | Never re-broadcast a rumor as another firsthand event |
| NPC reaction | personally observed or named-source rumor of specific outcome | dialogue and capped trust/fear reactions | Uninformed residents keep their regular authored line |

## Dialogue performance and micro-details

* **The uncounted cup:** Hessa's hands stop at one unused cup while a guest in the background steps out of earshot. A player can read the physical cup but cannot treat the guest's private identity as a collectible clue.
* **The two-faced press:** Sevrin turns the sealed side toward the public archive and the blank side toward a door. His short hesitation must not be interpreted as guilt by a narrator; his own words carry the suspicion.
* **The bridge nail:** Orrel bends to touch a cord tied across mismatched paving stones. Three miniature map marks disagree by a few inches. Nothing glows or teleports: the difference is social and physical.
* **Sheltered Thread:** Cord fibers rasp beside a still-crowded market. Nearby listeners may feel relief. Nobody is automatically given a hidden name.
* **Public Docket:** Paper taps softly against wood, with three corner weights shaped from different districts. Visitors can question institutions without automatically forcing victims into public view.

**Individuality and agency**: Even when NPCs share one supported rumor, their stable biography, home, daily work, prior player relationship, immediate hazard response, personal fear/trust and voice are retained. A trustworthy guard may be cautiously skeptical; a fearful neighbor may be personally grateful. Player intent does not erase the cost.

## Accessibility and future UE widget

A proper **Evidence Atlas** should be a pause-safe, font-size-scalable, gamepad-controllable wall with three world-linked physical object cards, a source-identity tag and a two-state visible time ribbon. Each card has distinct *Seen* / *Heard from person* affordances. Avoid red/green-only morality coding, visual noise, minuscule labels or unskippable narration. The source method supplies deterministic text; the full widget assets remain **not yet implemented**.

## Acceptance and limitations

- Native C++17 test: initially zero visited nodes, no premature names, persisted return reading, comparison unlock, positive/negative social evidence, no speakerless rumors, no blanket knowledge.
- Python source guard: input F9, Unreal declaration, exact branch-specific event names, local-only testimony, bounded existing propagation, canon section and Windows first-PC validation path.
- Windows Unreal 5.8 required: BlueprintPure reflection, text display/caption readability, line of sight, character action timing, NPC social loop, saved loading, memory corruption, controller option, low-vision and audio settings.
- **Current implementations have no full UI screen, long-distance courier simulation, animation or voiced acted player/NPC dialogue.** The existing local gossip loop runs periodically, not with an invented cross-realm mail delay.
- Transmedia expansions must distinguish narration/character claim/evidence; no adaptation is allowed to canonize one mutually exclusive player choice or answer the nine OPEN mysteries without deliberate future authorial work.

## Next implementation order

1. Explicit finite **courier dispatch** from a personally informed character (Kesta, not an omniscient system), with world-clock delays, route obstacles, recorded sender and receipt; no remote character should hear a result without that delivery.
2. Physical case-table interaction and gamepad UMG with screen-reader equivalent captions, and reusable language discovery hooks.
3. Expanded named dialogue for Hessa, Sevrin, Orrel, Kesta after actually learning which public response occurred; preserve local memories across multiple world days.
4. Optional route to Saltwake's dry tariff and Drevlach's future invoice: two independent pieces, at least two contradictory explanations, and a bodily choice of whom the player informs. No forced single answer.
