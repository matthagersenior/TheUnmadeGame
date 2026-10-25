# The Unpaid Shift — V6 Directed Playable Quest Handoff

## Verified source-level deliverable (not a shipping UE scene)

- **Playable in source after Unreal integration:** the already persistent v5 Cinderhold/Saltwake quest state, mutually exclusive real material/wage mutations, individually visited and verifiable physical stations, and now two-step confirmation of high-impact choices.
- **New V6 directed playback:** `UnmadeUnpaidShiftSceneRules.h` is a deterministic **39-beat / 13-scene** short screenplay. Every beat specifies an exact speaker ID, full caption, shot rig name, actor blocking, Foley event key, voice-performance note and beat duration. `UnmadeUnpaidShiftPresentation.cpp` emits these events **one at a time** via a Blueprint multicast, with timed fallback captions even when Unreal audio/camera tracks are not yet built. `SkipBesDirectedScene()` clears the active timer; skipping never changes a wage or material record.
- **Physical layout:** bespoke named source-level graybox interior components create a **doorway you can enter**. A small real runtime animated delivery cart and coal load move during routine work and stop while Bes's saved work refusal is active. The cart has collision disabled to avoid a movable obstacle blocking player/NPC navigation; this is NOT authored skeletal work animation.
- **Choice disclosure:** physically interact at a real station once to see *accurate cost and scope*; interact again with the same station within 12 seconds to commit. Home inspection remains immediate and non-committal. All costs, consent and denial are checked again at commit by the v5 authoritative code, never by screenplay state.

## Beat / shot / actor contract

Use the exact IDs and durations in `Source/TheUnmadeGame/Public/World/UnmadeUnpaidShiftSceneRules.h` (13 scenes, 3 beats each). **Do not invent additional lines** in Blueprints or override stable speaker identity with model-generated text. Performance recorded by actors should use one take per beat, with explicit room tone tails, and should be able to start when the saved world triggers that scene.

| Event | Required physical composition | At least one sound cue | Real gating |
|---|---|---|---|
| FirstWage | Wide ten-house heat lattice to Bes basket and worn hands | basket wood drop / continuing kiln | real `owedMarks>=2` and Bes personally heard |
| HomeCold | A real doorframe and waist-height shot of private hearth, bed and separate wage book | draft / dying ember | house inspected, not automatically paid |
| HearthWarm | Five source-debited coal baskets carried through actual open door | basket stack / imperfect ignition | only after actual `DeliverToNamedHome` and saved write |
| Ledger | Independent Rheva testimony, metal mark on paper, empty wage column | dry page, metal sound | `homeVisited` and named Rheva |
| Refusal | Bes unhooks work harness, cart's rhythm stops and station visibly remains empty | missing mechanical beat | only saved `WalkoutBegun`, next-day output changes |
| Restitution | Real pay marks counted; no stage-confetti | marks / actual signed page | employer can pay Bes and successor Mera debt in full |
| MeraOffer | Saltwake rain/rising dock shot and her capped funds | rope/rain/coin | firsthand Saltwake `ConsentRecorded` |
| MeraSacrifice | Money leaves Mera's owned account; Cinderhold owes her | coin transfer / wet paper | source-level `GuaranteePaid` saved |
| BesReturns | Repeat opening room shot with real warmth/cold change | returning room tone | saved reported wage; cooldown |
| MeraReturns | Repeat wet dock shot, date-stamped creditor claim | paper + distance bell | her recorded prior consent; cooldown |
| MaskPaid | Hearth and completed municipal mark overlay | same pay-mark sound from Rheva scene | saved settled; no boss skip |
| MaskUnderwritten | Different households separated by real owed debt | paper seal theme | saved Mera outstanding claim; no boss skip |
| MaskUnpaid | Empty cart image interrupts the boss claim | **missing** kiln beat | saved unresolved dispute; no boss skip |

## The workday street and Bes's home — practical build

Cinderhold origin for this authored slice remains `(0,50000)`. Bes's interior is centered around offset `(+1210,+615)` centimeters. Source names: `Bes.InteriorFloor`, `Bes.WestWall`, `Bes.EastWall`, `Bes.NorthWall`, `Bes.SouthWallWest`, `Bes.SouthWallEast`; **the front is not continuous**, leaving an opening. Bes's inspectable hearth is tagged `Bes.Home`. There are distinct `Bes.Bed`, `Bes.WarmthStorage`, `Bes.PayDesk`, `Bes.MissedWageBook`, `Bes.ChildHeightStool` and `Bes.DoorLintel` blocks. The child-height stool is a storytelling object only, **not proof of a newly implemented dependent NPC**.

Street source tags `Bes.WorkingCart` and `Bes.WorkingCoal` are cached and moved in tandem along a short safe path during a paid work routine, frozen on walkout, with physical collision disabled. A stopped animation visually announces an individual absence, not an invented town-wide work strike. For final art, replace with locally textured wheels, coal dust, harness and worker hand IK; read locomotion/anim state from the saved Bes work policy, never from a random theatrical timer.

The existing material warehouse, hearth home, walkout, full municipal payout and Saltwake guarantee stations remain distinct real world positions. E selects the nearest valid visible target. The direct LOS test, 245cm reach limit, no credit without actual v3 carrying, and v4 own-wage transfer remain authoritative. Do not cover the target selection with cinematic UI.

### Cinematic camera safe framing

Use the **fixed aspect-aware staging** approach rather than a poster that gets aggressively cropped. The actors, physical home and story props must remain in frame at 16:9 and on the user’s mobile portrait device. For in-engine 3D, camera rigs should have a “full actor and essential prop visible” acceptance overlay, shoulder-offset to keep line-of-sight from colliding with the doorway. Designers should store a safe aperture and camera collision/fallback per rig named in the scene table.

Subtitles: one row at a time, with speaker, typeface scaling, strong readable background or shadow, respect for system safe areas and reserved bottom UI. Speaker, dialogue and Foley rows follow the saved text-scale preference live (85–160%); the width stays within 86% of the viewport and the lower-third grows to the wrapped text's desired height. High-contrast preference changes the backing to black. Each of the 35 authored Foley keys also has a descriptive non-speech caption in its own quieter row, shown whether or not its WAV is available. Beat timing preserves the authored duration as a minimum and extends when the displayed speaker, dialogue and Foley caption need more reading time at 180 words per minute plus one second. The camera block, scene callback and skip timer share that extended duration. A scene can be skipped/advanced without forcing a dialogue panel to overlap the world, actor face, or a physical choice station. Accept large accessibility fonts, caption-while-muted and reduced camera motion. **Never use inaccessible text color as the only signal of owed versus paid.**

## Required audio and narration production

- Record **separate voice artists** for Bes, Rheva, Mera, Ovenna; reserve 48kHz/24bit mono dry takes and one room-tone take for each location. Do not hard-code a synthetic English male voice for all residents.
- Capture at minimum distinct Foley stems: wooden basket drop; ember rake/catch; three footstep surfaces; worn door and hinge; ledger paper turns; individual pay marks and seal; distant kiln wheel and its missing beat; Saltwake ropes/rain/dock water. Prefer authored sound over cheap oscillators for final release.
- Current generated Foley prototype cues use the local staged speaker or the stage point ahead of the player with distance attenuation. Verify the stereo spatialization, mix, room context and caption/audio timing in Unreal before treating this as final sound design.
- Provide weather/interior/acoustic states for Cinderhold at normal work, home cold, home with finite hearth reserve, walkout and genuine restitution; and Saltwake pier normal/guarantor sacrifice. Register spatialized and non-spatialized cue variants with accessible caption label.
- Establish song/stinger discipline: a **subtracted** kiln pulse on the true absence; no celebratory sting for a simple relief delivery while wages remain unpaid; a single wet paper sound at Mera’s owned-money transfer; branch-accurate returning mnemonic at Nhal-Vey's first mask-break.
- Synchronize each authored subtitle beat start/end to actual recorded phonemes/actor takes when assets exist. The authored durations are edit guidance, not a false assertion of automatically generated mouth movements.

## The exact result of a choice

| Action preview | Check before confirmation | On valid second press | Return visit |
|---|---|---|---|
| Inspect hearth (one E) | source claim heard / location reached | document home, no money/stock moved | future conversations have actual home evidence |
| Warm hearth | carried five-unit Cinderhold pack from debited depot; capacity | move pack into one private home, persist, cue HearthWarm | heat reserve decreases on workdays; debt unaffected |
| Stand with Bes | town actually owes wages; evidence verified | halt one actual Cinderhold producer and pay accrual, cue Refusal | cart stops; Bes returns home; shortage pressure may worsen |
| Public restitution | enough **actual municipal marks** for full remainder and Mera guarantee | exact money transfers, atomic saved settlement | Bes work and cart resume; cold-day history not erased |
| Mera guarantee | **her direct Saltwake consent** plus up to 6 owned marks | move owned marks, reassign true claim, cue MeraSacrifice | Mera remains owed until town makes her whole |

The first interaction **must never mutate** an account. The second must rerun source validation, preserve the original slot on write failure and show the scene only after a successful save. A different station or expired 12-second window must require a fresh preview. Do not permit confirmation via an unbounded UI callback that bypasses location/collision/consent.

## First Windows PC validation (not performed here)

1. Compile against actual Unreal version/UHT. Verify delegate `OnBesDirectedBeat` is Blueprint-bound, `SetTimer` runs three beats in sequence and `SkipBesDirectedScene` truly stops playback without mutating save data.
2. Build a temporary UMG subtitle ribbon, actual audio cue map and camera rig per shot ID. Detect missing assets at build/preflight; captions must remain even if a voice asset is missing. Test that voices are individually audible, narrative timing and captions are synchronized, no text is cropped at 16:9 or portrait.
3. Reach and walk through the full home via the street door with a real player capsule. Verify floor/walls collide correctly, no invisible ceiling or giant solid cube blocks movement, home hearth E selects over distant resident only when truly closer, pay book never hides the interactable.
4. Attempt E at each choice: first press yields accurate numeric, colored-and-text-described terms; second E commits only before 12 seconds and at the same real site. Switch sites midway and confirm no stale authorization. An interruption/skip cannot commit on the first press.
5. Physically withdraw and deliver a true 5-pack; test partial/failing saves, wrong realm, duplicate pickup, excessive room reserve and carried inter-realm cargo. Verify the count in private household and that public home reserves remain separately conserved.
6. Trigger shortage and Bes walkout. Advance a real workday and measure source-produced goods and wage records; ensure no phantom shifts while the visible cart is stopped. Resolve with municipal money / Mera from Saltwake, reload, confirm real cart work resumes or the successor claim persists.
7. Play all 13 shot timelines as actual Sequencer/camera bindings with distinct recorded performers, Foley and room tones. Verify cue-by-cue failover, subtitle resize, controller navigation, skip, non-voiced accessibility, motion sensitivity and interrupt.
8. Play Nhal-Vey mask scene in all three saved outcomes, with identical boss phases/health and non-automatic ending. Compare audiovisual motifs to earlier Bes and Mera encounter. Return to the exact NPCs after the boss and verify unaffected household history.
9. Add fully modeled interiors, animation blueprints, IK props, life-cycle schedules, doors and ambient crowd movements. Replace only visuals, preserving saved actor tags, stable IDs and ledger authority.

## Current honest boundary

The source now has deterministic performance timing/shot descriptions, a real event bridge and subtitle fallback, readable consequences before interaction, enterable scene blockout and a minimally animated economic prop. It does **not yet** have final sound recordings, a built Sequencer level, skeletal acting/lipsync, cinematographic lighting, rendered final art, a real UMG widget or Windows Unreal playtest certification. Next asset pass should use this contract so no scene has to be reinvented.
