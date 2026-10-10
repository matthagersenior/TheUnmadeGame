# The Unmade — Witness Braid: gameplay and dialogue production specification

**Milestone:** Volume X, section 17 · **source implemented** · UE Editor/PIE not yet validated

## The playable situation

The player is not told the truth about the Unanswered Interval. Three geographically distinct, source-spawned objects can be physically reached in any order. At each, E requires distance and unobstructed sight as for existing LoreSites. Later, legitimate civic outcomes alter the words at one of those exact objects; the player must personally witness at least one such return. The Crossings nail can then be used to choose one of two public responses.

| Player verb | Physical location | Identifiable resident | Evidence/feedback | Failure and recovery |
| --- | --- | --- | --- | --- |
| E inspect uncounted cup | Bellwold refuge (-17290,750,90) | Hessa | Initially the chipped cup; later wording follows saved Afterlight decision, not a guessed ending | Walk closer or clear LOS; no automatic progress before inspection |
| E inspect reverse seal | Paperhaven (17760,1090,90) | Sevrin | Public archive seal with intentionally blank reverse | No name disclosure, no off-camera grant |
| E inspect inkless nail | Crossings (-1100,-540,90) | Orrel | Disputed third road and physically mismatched survey | No invented nexus lore; one clue is insufficient |
| Revisit any relic after saved local choice | Same real scene, not a menu | Original local witness remains a person | Meaning changes to care or truth version | Return after an actual saved civic resolution, no extra quest-gate bypass |
| F7/F8 twice at nail | Crossings, within 295 cm | Orrel's artifact (not global menu) | Explicit precommit consequence warning and irreversible confirmation | Choose again within six seconds; mismatched choice cancels old intent |
| Reload later | Crossings | Neighboring residents still hold their original memories | One visible refuge cord **or** public docket persists with its matching journal text | Corrupted state rejects writes; old-schema saves remain intact |

## Cinematic and animation notes

Opening blocking: in Bellwold, Hessa pauses while counting beds and places one chipped cup beyond the ledger's border. Hold the room tone instead of a scare sting. After the player repairs or discloses Afterlight's burden, match-cut to Sevrin's unmarked seal in Paperhaven, then Orrel placing his nail beside the third map at the Crossings.

The aftermath should not be a generic glow explosion. **Care** manifests as a low horizontal line inviting unnamed travelers, with a faint cloth rasp in otherwise busy market ambience. **Public challenge** manifests as a taller vertical record with paper striking stone and three mismatched witness marks. Neither cinematic voice calls its preferred meaning “the truth.”

## Resident specificity

Hessa (cup): “If you take a name from here, you take the right to come back without being counted.” She says this as a fear, not an absolute law.

Sevrin (seal): “The reverse is not a missing fact. It is the place where a fact would have required permission.” A different expert can reasonably contest his account.

Orrel (nail): “Three surveys cannot each own the same road. Yet three workers built it, and every one is owed payment.” His worry after publication is local, specific and unresolved.

Kesta (road courier): “I can carry a report to a place where I have been. I can't make everyone believe I was there.” Future rumor propagation must require a real, locally recorded dispatch and finite delivery.

## Mechanical restrictions

- No retroactive change to 54 authored main quest steps, 82 names, item economy, faction resolutions or ten reality rites.
- No XP or repeatable loot for replaying the braid choice.
- No name disclosure; the public board catalogs contradictory objects, not anonymous residents' identities.
- Choice is saved once; the two visual blocks occupy the same footprint but have clearly different silhouettes and cannot coexist after restoration.
- First visit and later revisit are already saved in WitnessEchoLedger; a new outcome integer is optional on older saves. A saved resolution with insufficient evidence is corrupt and must fail closed.
- No AI model may commit this choice. Any later conversational LLM remains read-only; quest/game state authority stays in native logic.

## Actual test scope and gaps

Portable source tests cover prerequisites, rejected choice IDs, irreversibility, snapshot corruption, recovery from absent old saves, and both forms of public response. Python tests inspect input, SaveGame, on-site cue, world spawn, rollback and canon. GH CI compiles the native domain with -Wall -Wextra -Werror.

**Windows UE 5.8 pending:** compile against UHT and Editor; actor collision/line-of-sight at all sites; controller/keyboard F7/F8 semantics; save failure injection; archive and refuge marker visibility at dawn/night; narrator and subtitle cues; performance; NPC motion; actual animations. No binary game experience is claimed from source checks.

## Next arc without false spoilers

1. Source-driven codex map with three inspectable node lines, provenance class (firsthand/rumor/public), and player-selectable non-authoritative hypotheses; preserve source IDs and avoid revealing unvisited nodes.
2. Orrel and Sevrin each gain locally delivered, different next-morning dialogue after the public docket; Hessa and Kesta gain credible delayed reports rather than instantaneous global knowledge.
3. The reopened question can lead to Saltwake's dry tariff or Drevlach's future invoice, with a finite courier delay and reversible private investigation, **without** enforcing a canonical resolution.
4. Fully animated third-person clue handling and an accessible dialogue interface once the first-PC Editor is ready.
