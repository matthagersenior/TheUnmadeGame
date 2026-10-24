# The Hearth Petition — pre-Unreal QA and handoff

## Genuine new gameplay loop

Cinderhold: hear Bes and inspect his enterable home (existing), read the
physical **Bes.MissedWageBook**, hear Rheva's corroboration (existing), inspect
**Petition.CivicLedger** outside, then choose **Petition.PrivateChoice** or
**Petition.PublicChoice** with a second E at that same station within 12
active-world seconds. Only the matching permanent record is visible and
interactable afterward: **Petition.PrivateMarker** or
**Petition.PublicMarker**. The **Petition.ReturnWitness** marker directs the
player to speak to Bes on a subsequent in-world day; it cannot grant a
remote NPC acknowledgment.

The new branch is a *petition about wages*, never an automatic repayment.
It does not consume or create currency, repair housing, grant a skill or
automatically resolve Bes's existing labor dispute. Ongoing wage and stock
rules remain authoritative.

## Offline acceptance

- Both mutually exclusive final outcomes and the next-day return are
  exercised by strict C++17 state tests.
- Invalid stage/chronology/ending combinations and attempted re-writes
  are rejected, without changing the accepted record.
- Failed save callbacks roll back the petition step; repeated interactions
  cannot duplicate choice/reward entries.
- Python wiring tests check actual UE actor sites, LOS/distance, two-press
  confirmation, integration with Bes's physical conversation, additive
  storage, canonical journal, world markers and local NPC reaction.
- Source change requires simultaneous canon and changelog updates.

## First Unreal PC acceptance still required

Compile UHT/UE 5.8; reach physical book, ledger, petition tables and return
marker on a clean profile; verify LOS and visibility; test both endings and
alternate choices after save/reload; reboot at each stage; test save-file
corruption and storage interruption with an untouched backup; advance time
without skipping other household/day updates; speak to Bes personally and
check the returned line is shown on subsequent conversations. Confirm old
schema-1 saves still load and can begin the optional branch. Verify both
monuments visually, collision-safe placement, accessibility focus, dialogue,
animation, timing and audio.

**No claim of imported cinematic or verified Unreal executable.**
