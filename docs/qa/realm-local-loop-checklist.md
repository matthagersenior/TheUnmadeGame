# Realm-local completion checklist

16 October 2026. Source/portable audit at the realm-loop level. A row is not a
claim that its actors, scenery or interactions were reached in Unreal. Current
realm names and IDs come from the authored registry; no new content is proposed.

## Check the place as a whole

For each realm, follow arrival/bearings, the expedition and witnessed return,
personal witness/evidence and both accords, optional route/cache, mechanism and
survey, road repairs, wing/guardian, distinct-day residents/scenes, local services,
mastery, equipment, and the existing field recipe. Check blocked/repeated attempts,
failed writes, reload and recovery at those boundaries. Preserve refusal/consent,
per-realm resource ownership, mutually exclusive choices and other realms' state.

The portable `Tests/integration/realm_local_loops_scenario.cpp` currently covers
54 branch/accord/rescue-timing variants across all nine identifiers, with 3,378
combined reload checkpoints, 7,296 failed-write recovery cases and 216 successful
hunt/sanctuary/provision transactions. It uses production models, save preflight,
commit/recovery and healing helpers. Recipe ingredients are explicit test-fixture
stock; the game receives no free ingredients. Source adapter checks supplement
this model coverage. Separate opening-to-return scenarios cover named wages and
Mera's consent; they do not turn every realm's resident into that storyline.

| Realm / city | Mechanism | Authored local task / wing guardian | Specialty | Existing field recipe |
| --- | --- | --- | --- | --- |
| Crossings | Charter Signal | Guide the road surveyor / The Unmeasured Bailiff | Lantern bearings | Surveyor's Stillness |
| Saltwake | Breakwater Pumps | Recover the drowned pump record / The Brine Tollkeeper | Salt purification | Drywake Dressing |
| Cinderhold | Kiln Pressure | Stabilize the abandoned foundry / The Unspent Furnace | Heat tempering | Cooling Forge Edge |
| Drevlach | Oath Buoys | Hear the duelist before judgment / The Unclaimed Duelist | Mediation | Unpledged Ink |
| Orravane | Choir Counterweights | Restore the missing bridge voice / The Falling Cantor | Choir tuning | Choir Rest Tonic |
| Vathless | Yesterday Seams | Reconcile the misplaced memory / The Borrowed Custodian | Memory recovery | Seamkeeper Mail |
| Eillun | Census Lamps | Audit the silent index / The Unlisted Watcher | Quiet indexing | Quiet Witness Seal |
| Tharniv | Root Grafts | Open a root route without taking the nursery / The Boundary Pruner | Living rootwork | Nursery Poultice |
| Auvren | Threshold Lamps | Test the door under changing weight / The Closing Registrar | Boundary guidance | Threshold Ash |

## Evidence and open acceptance

- Model-tested: both expedition/accord outcomes; captive return timing; earned
  weapon/armor selection with failed-write rollback; per-realm ownership and
  loadout validation; wing progression and Drevlach-only yield; distinct-day
  witness visits and four scenes; insight upgrades/repair/mediation; finite
  specialty/day limits; actual mechanism coupling and survey; all road repairs;
  optional route/cache and mastery; field-recipe material/output/skill accounting;
  once-only hunt and daily sanctuary/provision healing with reload/failure checks.
- Source correction: hunt, sanctuary and favor provisions previously saved their
  spent receipt before healing. They now stage up to 35 health in the same write,
  restoring the receipt, health and stamina on failure. Full health spends no use.
  The cache sign now agrees with its existing no-consumable requirement.
- Source-label correction: the existing healing specialty is Tharniv (index 7).
  Eillun (index 6) provides quiet indexing. Earlier engineering notes calling the
  healing specialty Eillun were inaccurate; authored lore and IDs are unchanged.
- Field-kit gap addressed: earned kits work across active unresolved chambers,
  retaining the supplying realm's ledger. Purchase gates and free caches remain.
  Portable tests cover all 81 supplier/destination pairs and failed writes.
- Aftermath coverage now composes both endings in all nine realms with individual
  resident return ledgers through combined-record reloads. This does not establish
  NPC witness knowledge or actor interaction coverage.
- Echo/guardian composition now covers 96 paths through first chapter, aftermath,
  Echo, guardian, resonance and individual return ledgers, including independent
  authored answers, confirmation and failed writes. Present chapter records must
  agree on their existing prerequisites; omitted optional sections stay optional.
- Final-morning progression now commits its existing return position with the
  world choice. Portable coverage preserves inventory and individual returns
  across both mornings, six memory seeds, write/move failures and optional rematch.
  Present final/Auvren and resonance/mastery/clock records must agree; optional
  side quests remain optional and expired resonance windows remain restartable.
- Guardian/final-defeat retries now attempt immediately, wait five seconds after
  failure and stop for a successfully committed phase. Portable fault/retry tests
  cover all six guardians and three final combat phases without repeat progression.
  Existing confirmation rules pass interruption, cancellation and retry checks.
- Checkpoints now stop before writing when an encounter receipt is pending;
  the paused menu explains retry and Load/New retain discard confirmation.
  Read-only queries include hidden receipt owners and guard final phase identity.
  Portable fault tests cover 114 sessions and normal post-receipt saves.
- Profile handoffs now stage the target until new-world registration; failures
  restore original selection and keep partially initialized worlds write-locked.
  All 64 profile/recovery combinations pass portable ownership/isolation tests.
  Actual delegate behavior, travel and startup ordering require Unreal acceptance.
- Startup rewards now wait for registered player resume/world readiness and retry
  failed commits. Timed day/visit/rite progression freezes during blocked ownership;
  failed material days restore wages, households and finite support together.
- Component saves now capture the accepted live world as well as current resume
  values. Portable coverage adds 216 paths across all nine realm identifiers and
  both aftermath endings, 1,080 reloads and 48 field-survey fault paths. Caller-owned
  gear/language/rite/Strain patches, resident days and absent histories survive.
  Static ownership parity covers 126 fields; native capture is not engine-tested.
- Rite cost/trial/cooldown and civic/resonance progress now stage before one
  writer; runtime effects follow success. Portable coverage adds 1,080 nine-realm,
  ten-rite fault cases, 3,240 reloads and 24 paired/expired resonance paths.
- The three named boss victories, including Hollow Keeper mercy, retain pending
  milestone receipts and retry before stopping actor ticks. Checkpoints include
  hidden pending actors. Twelve receipt fault cases and 300 reloads pass portable
  checks; actual ranged impacts, mercy sight gates and actor timing remain pending.
- Next source/portable expansion: scoped expedition/road/Cinderhold ability
  dispatch and finite service/mastery commits. Keep authored gates, per-realm
  receipts and consent; monitor branch divergence without unnecessary sync.
- Unreal pending for every row: actor/marker reachability, actual witness/refusal
  and line-of-sight conditions, schedules and service rooms, guide/stealth/height/
  platform challenges, collisions, combat and hazards, feedback/controller flow,
  actual serialization/interrupted writes, audiovisual presentation and frame time.

Artwork and illustrated books remain optional. The next engine visit should test
one complete realm-local loop with these criteria before expanding visual polish.

## 17 October: shared combat and witness failure checks

Across every realm, kill ordinary Stalkers/Watchers with melee and projectiles;
interrupt reward saves, fill inventory, wait five active seconds and reload.
The first-style milestone must grant once, retain failed receipts and block a
complete checkpoint until settled. Special guardian, expedition and boss receipts
keep their existing independent gates. Interrupt a direct NPC witness write,
then allow a quiet eight-second rumor tick: persist the original observation
without replaying the event. Portable rules pass; actual actors, line of sight,
controller feedback, travel and platform storage still require Unreal.

## 17 October: oath/deed/road persistence acceptance

For each realm, interrupt a deed or verified-frontier-road save, then permit five
active seconds and reload: retain earned facts exactly once. Threats must never
redeem a broken oath, even with sufficient restitution evidence. Explicit chosen
break/redemption must roll back if either storage write fails. Check both oath
choices immediately after casting and after the half-second refresh: remove the
oath bonus on betrayal, retain other rite bonuses/debt, and keep the scar after
witnessed redemption. Pending history must explain incomplete checkpoints and
retain explicit unsaved-discard confirmation. Portable fixtures cover all nine
realm IDs and isolated profile stores; actual input, visibility, observers,
equipment, timers, travel and filesystem failures remain Unreal acceptance work.

## 17 October: coordinated action acceptance

Across the nine realm identifiers, interrupt each local resolution, village
completion, aid, discovery, conflict/supply outcome, rite/oath choice, cast and
Confluence conclusion at either write. Check that neither the decision nor new
witness/deed effects survives rejection, and that a retry needs the original
consent gate. Move to another final witness after a failed save: the old preview
must not authorize that decision. On success, reload immediately and retain the
chosen consequence with its original local accounts; absent residents and rumor
sources remain intact. Repair older split aid once, with no extra trust event.
Fill inventory or fail a milestone write after startup, make room/re-enable
saving, wait five active seconds and reload without duplicate grants/refills.

Portable evidence adds 32 action cases, 128 faults and 1,536 reloads. The native
`Unmade.Npc.ActionConsequenceRollback` buffer/reaction test is pending, along with
UHT/Editor, physical sight/reachability, controller feedback, platform IO and the
full realm walkthrough. Do not count these fixtures as an Unreal playthrough.

## 17 October: reality, crossing and revival acceptance

For each realm, take every reachable outgoing passage and return, including an
already-known destination. Interrupt movement and either write; confirm the
arrival, visit and eligible road survive together only after success. Reload
immediately and remain at the saved arrival. A known city in another realm must
not redirect a new realm's safe entry. Exercise failed movement rollback and
confirm saving stays protected until return movement succeeds or a fresh load.

Check direct Glimpse/Fold/both Rewrite choices with prior temporary windows,
partial/understood language and storage failure: no free clue, witness, barrier or
enemy exposure; failed writes preserve the old window endpoints. Die in all nine
realms, fail recovery writes, wait five seconds and reload. Recovery saves wounds
and position, retains world histories and permits full-inventory/pending-reward
recovery while keeping complete-checkpoint guards. Portable coverage: 96 reality,
104 passage and 18 revival faults with 1,744 reloads. Actor geometry, collision,
physics, controller/UI, exact timing and actual platform IO remain unverified.
