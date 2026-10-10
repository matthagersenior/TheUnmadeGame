# Saltwake & Cinderhold aftermath — Unreal acceptance gate
**October 10, 2026. Source exists; UE compile/playtest not yet done.**
Do not claim this checklist passed merely because GitHub static checks are green.

## Required playthroughs
1. Launch a fresh save. Travel through each gate. Before completing the original 3-witness frontier story, speaking to Harrow / Rheva must not start the new follow-up. E on a physical storm/heat control must report it locked.
2. Finish each region's original F7 or F8 resolution. Revisit the same warden. An independent new investigation begins; the previous outcome, other region and Bellwold state remain unchanged.
3. At Saltwake, find the Storm Sluice; at Cinderhold, the Heat Vent. E operates the nearer control even if an unrelated NPC is within speaking range. A blocked sightline or interaction outside 310 cm must not operate it.
4. Stand on the far side of each initial storm/heat barrier, attempt care and truth evidence. Neither progresses before operation. After operation the barrier must hide **and have no collision**.
5. Inspect **only one** branch's physical clue: Saltwake Cistern→Thenn or Rain Ledger→Sella; Cinderhold Common Kiln→Ovenna or Ember Deed→Tarin. A different NPC or wrong F-key must not commit. Switching to the alternate clue must be possible before any testimony, never afterwards.
6. Return to the initiating warden. F7 care or F8 truth commits only the matching witness path. A fresh replay/return must not re-award, re-open the choice or create duplicate props.
7. Inspect collision and visibility: each realm retains both sealed downstream route gates before resolution, then only the chosen gate opens. Saltwake builds SafeJetty *or* OpenTideWalk; Cinderhold builds PublicKiln *or* UnsealedPassage. Verify actual walking collision, not just actor visibility.
8. Travel to the other region and home, quit, reload and return. Both independent realm stages, preparation bits, endpoints, closed doors and one chosen traversable route must match the saved snapshot. The six unbuilt realms must still have zero state.
9. Talk to every regional resident after each outcome. Check public visible change lines are their own home's, while direct event witnesses speak in first person and rumor recipients hedge. No distant NPC should know an unwitnessed event instantly.
10. Save failure injection must restore stage and physical barrier rather than falsely claiming the mechanism/ending is committed. Corrupt arrays (length, prepared bit, conflicting stage/ending) must be rejected without replacing old lore, equipment or NPC memories. Older schema-1 saves without these optional arrays must load.
11. Test keyboard, controller, readability, failure hints, player recovery, returning without grinding, door obstruction and quest target discoverability. Replace transient debug messages with real UI before production signoff.
12. Test collision physics, NPC navigation around opened gates, geometry clipping, atmosphere, camera and save performance on Windows UE 5.8. Test real flooded and heated hazards before claiming these are complete adventures.

## Evidence required before closing
- Unreal Editor compilation logs, standalone level launch and saved video of each alternate ending.
- Automation test report plus manually recorded collision/traversal result from a cold save and restored save.
- Gate, witness, reward, and world-state table for both conclusions and both realms.
- Android streaming test requires a separately approved GPU host; no paid hosting has been authorized.

**Current level:** deterministic models and graybox source / GitHub CI source-contract pass, not a verified 3D build.
