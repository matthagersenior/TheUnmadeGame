# The Unmade — game-wide authored hotbox / graybox design ledger
_Source revision: October 19, 2026. Status: source-authored primitive art pass; no actual Unreal Editor render, asset cooking or packaged build has been verified._

## Scope and ownership

The live in-code art pass now attaches specific, nonblocking geometric cues to **34 reusable world prop kits**, the **nine realms**, **11 cargo road sites**, **88 homes**, **five civilian professions**, **12 monster species**, and the six optional combat/survey arenas. Its source-ledger also indexes **60 existing authored inventory items** for eventual replacement meshes. The full **228-entry art choice CSV** is at `docs/art/gamewide-graybox-choice-ledger.csv`. Items currently have production design rows, not a claim of new skinned weapons or gear models.

### Non-negotiable authority

The tagged base cube retains all original collision, traversal, interactable E/F range and save-owned on/off state. Decorative primitive child components have no collision, overlaps, navigation footprint, hitboxes, story triggers, or independently saved identity. If authored meshes load, they replace **visuals** first; never rewrite a quest tag or conjure a return/witness/boss result. The camp, cargo, household and faction consequences are state-driven. Do not materialize private names or claimed testimony in public art without previously earned consent.

## Nine realm landmarks

| Realm ID | Realm | Landmark | Silhouette/crown | Base RGB / accent RGB |
|---|---|---|---|---|
| 0 | **Crossings** | The Three-Span Charter | CharterArch | 58/76/112 / 228/181/108 |
| 1 | **Saltwake** | The Dry-Tide Gauge | TideGauge | 39/105/120 / 221/180/127 |
| 2 | **Cinderhold** | The Borrowed Hearth Vent | EmberFlue | 122/66/55 / 242/172/87 |
| 3 | **Drevlach** | The Foreclosed Oath Pier | LedgerChain | 58/88/111 / 213/164/109 |
| 4 | **Orravane** | The Choir Counterweight | ChoirWeight | 78/100/152 / 232/212/163 |
| 5 | **Vathless** | The Twin Ancestral Seam | SplitSeam | 106/91/76 / 190/174/127 |
| 6 | **Eillun** | The Occupied Blank Bed | OccupiedBed | 101/81/131 / 205/192/211 |
| 7 | **Tharniv** | The Unclaimed Graft | RootGraft | 65/110/76 / 188/181/105 |
| 8 | **Auvren** | The Double Honest Threshold | PairedThreshold | 79/75/124 / 172/220/216 |

Every landmark already has three authored world-space silhouettes. Decorative crowns distinguish the realm **without relying on color**. Final terrain/architecture remains for Unreal asset authoring. The reader should identify the core moral and physical principle in silhouette: charter/road, tide/invoice, borrowed shared heat, oath/debt, gravity/music, ancestral seams, anonymity, common ownership and incompatible origins.

## Reusable kit replacement contract (34 families)

| Visual family | Specific temporary detail | Proposed asset slot |
|---|---|---|
| Ground | No vertical trim; travelable surface is the canonical cube | `SM_Common_Ground01` |
| Causeway | Two edge bollards only on short pieces, never close travel lanes | `SM_Common_Causeway01` |
| Commons | Shallow roof ridge and common-use lintel; no new doorway | `SM_Settlement_Commons01` |
| Market | Counter lip and tied awning on facade | `SM_Settlement_TradeStall01` |
| Watch | Top lantern, two spare bell brackets and sight slits | `SM_Settlement_Watchtower01` |
| BellTower | An empty bell frame and suspended double hanger | `SM_Bellwold_UncastBelltower01` |
| Sanctuary | Roof hood, shared shelter lintel and lantern hearth | `SM_Sanctuary_ShelteredThreshold01` |
| ArchiveTower | Serrated scroll crown and reading-gallery bracket | `SM_Paperhaven_ArchiveTower01` |
| Records | One horizontal witness shelf and offset folio disc | `SM_Records_WitnessLedger01` |
| Scriptorium | Sloped reading surface and blind emboss seal | `SM_Paperhaven_Scriptorium01` |
| Forge | A ventilated chimney hood and furnace rim | `SM_Cinderhold_CommuneForge01` |
| Home | Low structural beam without added wall collision | `SM_Home_ModularWall01` |
| Bed | Pillow head and simple cylinder bedroll | `SM_Home_RefugeBed01` |
| Hearth | Wide low brazier with a visible central ember stone | `SM_Home_CommonHearth01` |
| Table | Flat circular vessel and a shallow ledger saucer | `SM_Home_Worktable01` |
| Bench | A wooden back rail; still usable as an inspectable fixture | `SM_Home_RepairBench01` |
| Gate | Two hinge barrels mounted on original door collision cube | `SM_Access_StatefulGate01` |
| Portal | Concentric crown loop outside the player's collision channel | `SM_Atlas_WitnessGateway01` |
| Civic | An asymmetrical public hearing shelf and non-identifying badge | `SM_Civic_ChoiceDocket01` |
| Mechanism | Vertical shaft and gear wheel, with one E contact point | `SM_Mechanism_WorkingCrank01` |
| Evidence | A framed artifact head and small visual-only evidence disc | `SM_Clue_FirsthandSpecimen01` |
| Cargo | Two banded parcels atop the existing cargo cube | `SM_Cargo_DatedReceipt01` |
| Ruin | Offset fallen lintel and broken column ends | `SM_Ruin_StratifiedMasonry01` |
| Arena | Silent markers only; never add unintended boss cover | `SM_Arena_ReadableBoundary01` |
| Cover | A modest top cap, no raised projectile obstruction | `SM_Combat_CoverBlock01` |
| Hazard | A conical warning crown; damage footprint unchanged | `SM_Hazard_TelegraphMarker01` |
| Ritual | Two concentric rings without an extra cast position | `SM_Resonance_AttunementPlinth01` |
| Signal | Small leaning lantern and delivery knot | `SM_Echo_SignalMarker01` |
| FinalChoice | Paired offset crowns; no automatic truth, boss or ending | `SM_Final_ConditionalAnswer01` |
| Landmark | Architectural crown determined uniquely by the realm | `SM_Realm_AuthoredSkyline01` |
| Bridge | Edge lighting only: cannot affect traversal collision | `SM_Route_TraverseBridge01` |
| Garden | A paired branch motif above the public threshold | `SM_Tharniv_PublicRoot01` |
| Workshop | Accessible rivet/anvil shape, not a second action key | `SM_Workshop_FieldRepair01` |
| Generic | Leave safe and plain until it receives an authored tag | `SM_Generic_Blockout01` |

These are production **names to approve**, not files claimed to exist. Keep the original transform, footprint, E/F targeting and door state when swapping in an actual asset; optimize LOD, occlusion, HISM and navigation only against measured editor performance.

## Eleven distinct cargo-road authored work yards

| City ID | City | Hand mechanism | Cargo physically carried |
|---|---|---|---|
| 0 | Crossings | chalk witness post | sealed kettle fittings |
| 1 | Bellwold | bell screen | lantern wick rolls |
| 2 | Paperhaven | copy press | blank repair folios |
| 3 | Saltwake | rain shutter | drying cloth bundles |
| 4 | Cinderhold | kiln baffle | kiln patch bricks |
| 5 | Drevlach | tide clamp | ferry patch boards |
| 6 | Orravane | counterweight brake | support pin cases |
| 7 | Vathless | seam brace | plain seam wedges |
| 8 | Eillun | privacy screen | unmarked door fittings |
| 9 | Tharniv | root sluice | common graft splints |
| 10 | Auvren | threshold brace | threshold anchor pegs |

The home records are **88 homes** with specific occupants, recorded preferences and local concerns; use each matching `Road.Home.<index>.*` row in the CSV. Furniture is independently saved, so a bed/hearth/table must reflect the actual household state and is not a new loot reward.

## Three settlement identity briefs

**Crossings:** split charter stone, converging roads, secular refuge-versus-accountability signage; central traversal stays unblocked. **Bellwold:** refuge-first chalkstone, an absent bell's framework, patched linen, open hearth and lantern path. **Paperhaven:** oxidized inkstone and tall archive, dual seal, folded-paper eaves, public review desks and privacy-conscious records. Neither town should use the other's generic village building model.

## Unanswered Road, campaign mysteries and the last encounter

**Unanswered Road** markers are physical, readable leads. Each must have an interrupted waypost, weathered local witness marks and a blank space where the missing account belongs. Their silhouettes can prompt inspection, but cannot award a main-story answer, reveal a private witness, or generate a compulsory nine-glyph checklist. The tag in the Campaign Spine remains the actual authority.

**Final Auvren / Nhal-Vey:** two asymmetrical, visibly different response plinths; unequal silver and unfinished glass, no automatic triumph portrait or false-victory reveal before the earned scene. Boss attacks keep their original warning shape, floor projection and hitbox. The ethical difference is evident through architecture and locally supported evidence, not a flashing "good ending" marker.

**Character and equipment continuity:** the 60 item-specific proposed designs have to fit the equipped slot and actual crafting/earned-reward provenance. These are approved art candidates, not new loot or already-existing skinned models. Cues for the player silhouette should read as a character with discoverable professions and no preordained heroic identity.

## Character / creature placeholder anatomy

Residents: merchant with balance scale, guard with high watch cap, scholar with archival satchel, courier with sealed off-center parcels, and wanderer with patched hood. The source renderer distinguishes these without changing the existing NPC capsule, memory, trust, or animation source.

Creatures: 12 distinct species silhouettes (and slightly larger champion visual scale) with unique body and crest shapes. Preserve the source `CreatureKind`, real telegraphed attacks, guard/retreat counters and contact ranges. **Nhal-Vey** receives a separate final-choice visual brief; this pass does not silently rewrite or reveal the false victory.

**Player and inventory:** the 60 individual item rows are tied to `ItemId`, kind, rarity and their specific story description. Equip appearance, VFX, crafting, and UI icon decisions remain for actual Unreal build and assets. Player remains a neutral early prototype silhouette; do not conflate it with any one player progression path.

## Approved asset replacement workflow

The asset replacement rule is to preserve every original collision owner, stable ID, source-of-truth interaction and save-backed visibility until a genuine cooked asset has been tested. Choose an item's final silhouette and finish from the CSV before changing its in-game representation. A replacement is considered finished only when actual Unreal visuals, collisions, gameplay and performance have been inspected.

## Specific art and QA decisions, in order

1. **Critical / first-person proximity E checks:** Inspect from gameplay camera, with text disabled; can a player distinguish a real mechanism, private note, gate, monster windup, shelter, civic record and collectible? Any object only understood from a floating caption fails.
2. **Physical correctness:** Verify each originally closed/open gate, roof, bridge and floor at the original position, plus exit, boss arena sight lines, traversal support, creature pathfinding and home entry after reload.
3. **Lore and branch-aware state:** Care vs public-inquiry variants need different site dressing, but no extra reward, NPC omniscience, absolute moral verdict or leaked identity. Repair, cold/heated hearth, night shift, granted access and cargo are taken from saved state.
4. **Material detail choice:** Choose one characteristic base substance and one functional highlight, approved per realm. Weathering should follow climate (salt, soot, ink, fracture pressure, root scars) and local maintenance, never random fantasy ornament.
5. **Sound and light:** Choose one ambience, one mechanisms' foley, one hazard pre-strike cue and an ambient lighting solution per biome. Do not invent success cues that imply unearned quest completion.
6. **Accessibility:** Shapes must remain recognizable in grayscale and from a small screen; readable contrast in day/night, no flashing warning without shape/audio redundant cue.
7. **Performance:** Measure draw calls, transient component count, material instances (12 zones × 2 = 24), GPU cost and streaming before converting repeated details to HISM or cooked Nanite/static meshes. No invented FPS claim.
8. **Asset replacement and documentation:** Record selected mesh/material/sound icon IDs in CSV, make a live engine asset, verify collision and persistence, add screenshot and test evidence, then mark that specific row `Engine-tested`. Never mark whole realms complete based on source CI.

## Current known limits

The GitHub repository currently has **no cookable Content/** authoring assets committed. `Scripts/unreal/author_realm_palette.py` can create the tintable master when an Unreal Editor machine is available; before then, Engine BasicShapeMaterial fallback may not tint. GitHub source CI validates deterministic rules and static wiring, **not** mesh cooking, Unreal C++/UHT compilation, imported rigs, material appearance, frame rate, navigation, collision meshes, audio files, or package readiness. The recovery/Unreal PR remains distinct from a playable release.
