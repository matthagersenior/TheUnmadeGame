# Cinderhold adventure v12 acceptance

The optional milestone links Bes's household, Varda's field investigation, Blackslag's particular Sootbound Elder and a remembered return. This is source implementation with portable tests; no Unreal executable or engine playtest is issued.

## Walkthrough

1. E Bes and the new return shelf inside his home at `(1370,50520,75)` in either order. Existing household wage and relief actions remain separately available.
2. E Varda at Blackslag's entrance. If the v11 passage report is pending, finish it first, then converse again. The completed Blackslag survey now yields to normal dialogue.
3. Read the tracks `(-3350,51550,75)`, service log `(-3350,52000,75)` and cadence plate `(-3350,52450,75)`. E is physical reading; F is optional Glimpse for eight Strain. Stand within 240 cm with clear sight.
4. Quiet the elder alive: E vent `(-2900,52600,75)` after learning tracks and log, then E cadence post `(-2900,52900,75)` after all three clues, within twelve seconds. Q at the vent costs twenty-four Strain, interrupts pending elder attacks and permits an eighteen-second cooling window. Or defeat the elder using its marked slam/sweep tells, guard and cover.
5. After the saved outcome, choose at the vent. Three E turns at least two seconds apart record maintenance: daylight kiln path and open archive. R/R within six seconds commits all-hour kiln access but closes the archive; T/T commits archive access but closes the kiln path. Each Rewrite costs sixty Strain. Same location, clear sight, sufficient stability and matching preview required. One world-slot write saves both route and strain; failure refunds and rolls back. Decisions are local and permanent.
6. E Varda with actual outcome and route. E Bes to report. Optional E Rheva/Ovenna gives only each told resident the specific response. E return shelf records day, real warmth, cold-day history and unpaid marks, then plays the remembered return.

## Required Unreal tests

Compile editor target through UHT and C++; test with both missing and imported optional character/audio assets. Regenerate a real navigation bounds volume over Blackslag including the new west path, apron, nest extension and covered passages. No source function is presented as an installed navmesh.

Walk each clue sight ray and home doorway. Verify the two gate doorways cannot be entered through surrounding geometry. Maintain route by daylight and wait for night; confirm kiln gate closes without trapping the player in an enclosed space. Open-vent route: kiln open, archive inaccessible. Sealed-vent route: archive open, kiln closed. Original surveys and household choices must remain reachable.

Fight the elder above and below half health. Aim mark must stop tracking when its warning begins. Dodging the marked radius or taking cover must cancel a hit; one warning can produce at most one damage attempt. Cooling or Folding cancels a pending strike. Territory exit stops pursuit without wall penetration; player defeat/checkpoint recovery remains available. Quieting requires an actual living elder. The quieted creature remains visible and cannot take player attacks or Unwrite Law displacement. It must reload as quieted; defeated elder must reload absent.

Test all six outcome/route combinations, each visit order, a completed and a pending v11 Blackslag survey, and repeated reports. Reload before and after every durable step. Preserve historical return numbers after current wages/fuel change. A malformed ten-value adventure snapshot or its dependency fields must disable adventure/world writes without overwriting the original. Inject a save failure at clue, route, defeat, report and return; no permanent appearance or acknowledged completion may precede a successful save. Failed paid route returns exactly its cost and preserves unrelated active global fracture windows.

Import the three original prototype cues using `Scripts/build_cinderhold_audio.py --check` followed by the guarded `Scripts/unreal_editor/import_cinderhold_audio.py` inside an approved engine. Verify audible positional cue locations, the cadence's two short knocks and delayed low knock, and safe levels. Missing sounds remain playable with full captions. No final actor VO is claimed.

Play and skip all four directed scenes. Camera must frame an actual local actor, avoid obstructing walls and restore the original view target on skip/end. Actor facing/clip requests must release. Check full caption readability, actual geometry, accessible markers and two-step Rewrite warnings on keyboard and controller. These are outstanding acceptance tests, not results of GitHub CI.

## Evidence and canon

Portable C++ behavior: `Tests/world/cinderhold_adventure_test.cpp`. Runtime contracts: `Scripts/tests/test_cinderhold_adventure_wiring.py`. Sound checks: `Scripts/tests/test_cinderhold_audio.py`. Canon: Volume X Chapter 32, earlier Bes chapters 25–27 and Blackslag Chapters 28–31. Existing stable NPC identities are reused. No ending or OPEN mystery is resolved. The immutable historical illustrated releases are retained; v12 is rebuilt from the full v11 masters.
