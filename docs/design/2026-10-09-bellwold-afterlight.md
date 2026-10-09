# Bellwold Afterlight — The Second Night
*THE UNMADE | Authored return-visit story | Source milestone October 9, 2026*

> "The refuge never asks where you came from." — Hessa, refuge matron

Bellwold's lanterns are restored and the Refuge Compact is settled. Most RPGs would now mark the village complete. THE UNMADE asks the harder question: what happens to people history refuses to admit exist?

During the second night, Hessa learns there are families left out of the official census. The public refuge has warmth but not enough space for everyone, while the erased register contains proof of official denial. The player can choose which urgent truth Bellwold acts on first. Both paths matter; neither is labeled good or evil.

## Authored follow-up route — four saved stages

1. **Return after resolution.** First complete the existing Rook/Hessa lantern task and the three-witness Refuge Compact arc. Return to Hessa. Her new appeal starts the Afterlight investigation; the earlier faction outcome remains intact.
2. **Investigate real evidence.** Find either the uncatalogued relief cache to the west (tag Bellwold.Afterlight.Relief; near x=-19350, y=1100) or the altered census to the south (Bellwold.Afterlight.Census; near x=-18500, y=-1450). Interact within actual proximity and line of sight. Before finding a matching witness, the player may switch the focus.
3. **Ask the matching witness.** The relief cache leads to **Sorin the healer** (npc.bellwold.healer.001); the erased census leads to **Ivera the tutor** (npc.bellwold.childtutor.001). An unrelated resident, invented AI line, or faraway interaction cannot advance the story.
4. **Decide in Hessa's presence.** F7 opens the ward, or F8 publicly engraves lost names. A choice without matching evidence and a proper witness is rejected. Once resolved, the outcome persists and cannot be farmed.

### Resolution A: The Ward of the Second Night

Sorin's testimony convinces Hessa the unregistered families cannot survive another cold night without shelter. Bellwold reveals its **Safe Ward** blockout structure. Eyewitnesses recall seeing the choice made; others only know a rumor. The player earns one **Ward of the Second Night**, epic armor (item code armor.afterlight_ward).

This decision does not erase the missing census. Paperhaven's future investigations should question why official memory stays dangerous even after the immediate lives are sheltered.

### Resolution B: Lantern of Unredacted Names

Ivera remembers the children whose names the registrars excluded. Hessa commissions a public **Open Census**, exposing the missing names in a permanent monument. Direct witnesses describe the carving; hearsay does not become eyewitness proof. The player earns one **Lantern of Unredacted Names**, epic charm (item code charm.unredacted_lantern).

The newly visible truth does not magically shelter families. Later choices should challenge the player to deal with immediate deprivation while preserving evidence.

## What actually exists in source

A deterministic Afterlight model, four quest stages, separate investigation markers, two NPC witnesses, branching committed outcomes, save fields and rollback on failed writes, two mutually exclusive graybox landmarks, two exclusive one-time epic items, altered nearby NPC testimony and rumor lines, journal counter, offline C++ tests and Unreal source contracts. No LLM or network connection is required.

## Honest limits

These are simple Unreal C++ actor source hooks and tested engine-independent rules, **not Unreal-compiled, playtested or visually polished gameplay**. The ward does not yet simulate sleeping spaces, cold exposure, population growth or resource allocation. The census does not yet schedule a trial or enable a city-wide archival court. The chapter needs collision checks, navigation, proper animations, architecture, original sound, gamepad UX, accessibility and full quest replays in an Unreal-capable environment.

The greater design principle: **The community keeps living after you thought you saved it.**
