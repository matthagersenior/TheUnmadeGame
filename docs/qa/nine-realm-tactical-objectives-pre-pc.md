# Nine-realm tactical boss objectives — pre-PC acceptance

## Shipped source mechanics

Nine `ArenaTacticSpec` entries, one per existing expedition realm.
Each already populated boss chamber contains a FIRST and SECOND
interaction station that must be approached in the realm's order before
the limited time window expires. The boss must be active, living, and
unresolved; a mere journal selection cannot complete the counter.
The second successful press interrupts the boss via `ExposeToFold`
for 4–6 seconds and starts a non-spammable counter cooldown.
Saltwake, Drevlach, Eillun and Auvren also have limited-duration
collision-bearing LOS shelter; all shelters vanish afterward.
Older 3-second grounding levers and the evidence-yield route remain
accessible as alternate styles.

The *tactical counter* is transient and intentionally unsaved. A reload
cannot leave a half-armed trigger, duplicated reward or a fake boss
interruption. Saved `ExpeditionState` continues owning the boss result,
real rescued prisoners, return witness and earned equipment. Physical
aftermath objects and journal lines derive only from that state.

## Test contracts

C++17 strict test for nine distinct specs, missing readiness, wrong
station order, timing expiry, valid success, cooldown and reset.
Multiple persisted outcomes: missing return, unfinished rescue,
real two-captive rescue, evidence-based living yield, all nine distinct
local consequence descriptions. Python contract checks live
station interactions, boss exposure, temporary shelter and
state-derived world/journal presentation.

## Unreal acceptance not yet performed

- Compile with project UE version and UHT on actual host
- Ensure both stations sit on collision-safe, reachable boss-room flooring
- Verify line-of-sight and E selection when boss, truce and old lever are close
- Test sprint/dodge versus shortest six-second window without mandatory damage
- Validate every unique boss shape and all subordinate role interrupts
- Check cover's collision, sight and disappearance during real combat
- Test pause/resume, player defeat, restart, reloading and boss death mid-counter
- Verify result markers for prisoners remaining / both rescued / yielded
- Test disabled/corrupt expedition saves stay fail-closed
- Test accessibility: players can still use cover and existing lever
- Run a complete realm-one-to-nine campaign and account for audio/performance

These are source-level additions and portable tests, not a complete
Unreal-playtested game or released combat content.


## Coordinated boss support

Each active unresolved boss chamber spawns a shield sentinel (slot 11) and
an aimed marksman (slot 12) on top of its existing nine expedition enemy
slots. The sentinel's real combat guard can give its boss +4 armor in close
formation; the marksman pressures the traveler out of a static position.
Both become inactive on a boss death or evidence surrender.

Their defeat is temporary combat state: not `ExpeditionState.kills`, not
a repeatable XP/drop receipt, and not new durable quest history. The
player's normal melee and status defeat reward paths explicitly ignore
these two transient guard slots. Existing boss defeat, captive rescue,
and evidence choices remain the only source of permanent arena outcomes.
