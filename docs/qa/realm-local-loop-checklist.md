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
- Next source/portable expansion: failed profile travel/startup and safe original
  profile/settings restoration. Actor discovery, pause timing, feedback and actual
  target-world handoff still require Unreal acceptance.
- Unreal pending for every row: actor/marker reachability, actual witness/refusal
  and line-of-sight conditions, schedules and service rooms, guide/stealth/height/
  platform challenges, collisions, combat and hazards, feedback/controller flow,
  actual serialization/interrupted writes, audiovisual presentation and frame time.

Artwork and illustrated books remain optional. The next engine visit should test
one complete realm-local loop with these criteria before expanding visual polish.
