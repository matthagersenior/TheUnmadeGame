# Cross-realm Petition Relay — pre-PC verification and import handoff

## Playable-source handoff
- Start: Cinderhold Hearth Petition stage 4 (Bes actually told).
- Pickup: interact with **Relay.CinderholdCopy** after its appearance in
  Cinderhold. No global teleport of a document.
- Saltwake: take the copy in person to Mera
  (`npc.saltwake.courier.001`). Her line and exclusive
  **Relay.SaltwakePrivate** or **Relay.SaltwakePublic** record only
  become available after hand-delivery.
- On a later in-world day, speak to Mera again to pick up her reply.
- Crossings: hand the reply to Orrel (`npc.bridgekeeper.001`) through
  actual dialogue. Only then may **Relay.CrossingsPrivate** or
  **Relay.CrossingsPublic** appear. The outcome is visible and exclusive.
- Interactions obey distance, line-of-sight and competing nearby NPC.
  Inactive optional markers cannot intercept original quest actions.

## Persistent commitments
`PetitionRelayState`: stage, original private/public route, collection
day, delivery-to-Mera day, reply day, delivery-to-Orrel day.
Must have valid parent HearthPetition and real clock; stage0 is a
backward-compatible optional unstarted state. The combined preflight
checks saved chronology before commit. A failed write rolls back the
live quest step. No new spending, income, possession stack or stock change.

## Source acceptance
Portable C++ tests: both branches, consent boundary, time progression,
failed-write rollback, invalid stages, impossible future days, and
no changes to the accepted ending on failed restore.
Native wiring checks: physical actor sites, journal, local dialogue,
capture/restoration, combined preflight, portable fixture parity.
Combined source CI must pass in the active development branch.

## Engine-only acceptance
UHT/Editor compile, actor reachability and collisions in all three
cities, weather/night NPC routines, interaction focus and first-hand
conversation, visual placement, physical travel, interrupted storage,
real game clock transitions, old profile migration, reset/reload
integrity, and subtitles/animation/sound. No Unreal build claimed.
