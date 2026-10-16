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
- Remaining supported gap: field kits can only be purchased after the local boss,
  while their current use condition requires that boss to remain unresolved.
  Cache discovery deliberately consumes no kit and must stay independent. Resolve
  useful kit eligibility without erasing a saved accord, inventing free stock or
  silently moving another realm's ledger. Gameplay preference has been requested.
- Next source/portable expansion: compose first-chapter aftermath/Echo/guardian
  consequences and their resident return recognition with these local loops;
  inspect targeting, feedback and empty/full-capacity behavior in each real
  adapter. Existing unit coverage is not proof of a composed realm visit.
- Unreal pending for every row: actor/marker reachability, actual witness/refusal
  and line-of-sight conditions, schedules and service rooms, guide/stealth/height/
  platform challenges, collisions, combat and hazards, feedback/controller flow,
  actual serialization/interrupted writes, audiovisual presentation and frame time.

Artwork and illustrated books remain optional. The next engine visit should test
one complete realm-local loop with these criteria before expanding visual polish.
