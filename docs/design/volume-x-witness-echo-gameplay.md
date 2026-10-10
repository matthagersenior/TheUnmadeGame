# Volume X enters the game: Witness Echoes — three real inspections and returning meanings

**10 October 2026 · Source implementation slice, not an Unreal PIE acceptance claim**

## Why

Volume X is primary crossmedia lore, but a game also needs **physical verbs** for that lore. This change introduces **three tangible relics** as ordinary `AUnmadeLoreSite` actors in the already-authored Reach graybox. The player physically approaches within 295 Unreal centimeters with line of sight, presses the existing Interact input, reads the object rather than a global lore popup, and gets a bounded, optional record on the same persistent save as the main game.

The same object can be inspected again **after a matching public story choice**, at which point its physical wording changes without automatically proving a cosmic explanation or creating new required lore gates. Different earlier choices have different texts. The game journal reports how many of these objects have been seen and revisited.

## Three authored interactive sites

| Source callback | Stable physical actor tag | Graybox city, approximate coordinate | First personal observation | Choice-gated later meaning |
|---|---|---|---|---|
| CALL.01 | `site.callback.uncounted_cup` | Bellwold refuge (-17290, 750) | Hessa's uncounted chipped cup | Bellwold's already saved Afterlight Relief or Revelation |
| CALL.03 | `site.callback.two_faced_press` | Paperhaven (17760, 1090) | Sevrin's intentionally unmarked archive-seal reverse | Local first civic Shelter/Research commitment, not an invented Paperhaven global result |
| CALL.04 | `site.callback.inkless_nail` | The Crossings (-1100, -540) | Orrel's bent inkless nail | Local first civic Shelter/Research commitment |

**No other Volume X mystery has been secretly implemented.** The full registry has 13 stable callback numbers reserved and validated, but just three locations have source-spawned actors. None of the later six realms gains an actor, new engine map or compulsory protagonist story route from this patch.

## Runtime path and failures

1. `AUnmadePrototypeHub::BuildForPrototype()` spawns the three objects using `SpawnLoreSite`, the already-existing cube/point-light site actor, with unique physical IDs. All three use existing landmark districts as a rendering/collision reuse; they do **not** increase the six-area landmark counter or duplicate its reward.
2. `AUnmadeCharacter::Interact()` already finds the closest line-of-sight `AUnmadeLoreSite`, checks the nearby NPC and invokes `Hub->InspectSite`; new callback sites flow through that unchanged control rather than a command-only reward.
3. `UnmadeWitnessEchoRules.h` maps exact physical site ID to the stable Volume X callback ID and three separate strings (first evidence, after care, after truth). The C++17 `WitnessEchoLedger` remembers first reads and one meaningful later read with two bit fields. Repeat E cannot farm experience, money, new quest completion or a second reveal.
4. The hub takes both old ordinary-discovery and old callback snapshots before editing, makes a **single SaveGame write**, and rolls back both on failure. A malformed future callback snapshot blocks further saves rather than overwriting an unknown or damaged source; other valid saved quest branches can still restore in memory.
5. The same optional fields `bHasWitnessEchoSnapshot`, `WitnessEchoFirstMask`, and `WitnessEchoReturnMask` default to absent on old schema-1 saves; they do not change the top-level `SchemaVersion`.
6. `GetWitnessEchoJournal()` produces an early debug-journal summary, noting recorded evidence and returned meaning. This is still on-screen prototype feedback, not a polished UMG codex.

**Important UI/gameplay distinction:** the voiceover auditions in Runway are standalone audio media, **not** in-engine voice acting or actor lip sync. The new evidence texts do not require TTS and always have on-screen written counterparts. Neither the 9 OPEN cosmic mysteries nor the Nhal-Vey ending is auto-solved by inspecting a cup.

## QA requirements (portable checks already written)

The offline C++ test `Tests/world/witness_echo_test.cpp` checks first read, later care and truth text selection, re-reading, distinct sites, invalid IDs, bit-mask corruption, no-unearned return state, preserved snapshots, and correct stable callback IDs. CI compiles it under C++17 with `-Wall -Wextra -Werror`.

The Python test `Scripts/tests/test_witness_echo_wiring.py` ensures those three callback IDs actually exist in the primary Volume X JSON, all three physical actor tags occur in the hub, player journal/interaction sources are wired, optional SaveGame fields exist, and the rejection/rollback logic is present. Windows first-PC preflight also runs this check.

**On a future Unreal PC before labeling playable:**
- Walk Bellwold, Paperhaven and Crossings and **inspect** each site with the E input. Confirm no invisible blocker, player collision, misprioritized NPC or clipping.
- Reload an old schema-1 save and a new save; verify unrelated quests, loot and NPC return history are unchanged.
- Test site readability day/night, from both camera sides, line-of-sight blocking, gamepad Interact, accessibility and caption/hint UI.
- Complete Bellwold Afterlight in each of its two mutually exclusive paths on separate saves and revisit Hessa's cup. Only the legitimate saved ending must govern the changed line.
- Complete local civic Shelter/Research separately and revisit Sevrin's press and Orrel's nail. Check local implications remain accurate.
- Force a SaveGame failure, restart and confirm **neither** witness-evidence stage nor ordinary landmark discovery has advanced.
- Reject malicious/corrupt snapshot bits without losing already committed ordinary quest and relationship state in memory or overwriting the original slot.
- Confirm the journal can open after a return, and no unvisited item is listed as personally witnessed.

## Future slices

Convert the thirteen callback records into a standard `DataTable` so authored prose is translated, subtitled, and adjustable without recompiling C++. Move the temporary debug text into a searchable, source-attributed codex UX and voice-act character memories only after their origin is locally justified. Then expand the physical, optional evidence sites to each subsequent realm in the **same id and state model**, avoiding global omniscient quest-givers and artless text-only entries.

**Status:** GitHub C++ source and portable offline checks; engine compile/UMG/collision and real animation still require Unreal-equipped hardware. The dated illustrated DOCX/PDF/ZIP volumes remain unreissued.
