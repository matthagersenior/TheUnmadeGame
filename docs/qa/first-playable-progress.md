# First-playable audit and resume record

2026-10-16. Baseline: `9606fec4a3ceb3ad546e347d60a395628e1de8b1`.
Branch: `codex/first-playable-recovery`. No Unreal host or paid services used.
Git transport stalled; the GitHub connector retrieved all 479 files, each checked
against its original Git blob SHA before creating the isolated local branch.

## Authority and scope

Read CONTRIBUTING, configuration, authoring README, development status, production
gate, canon policy and first-PC scripts. Production authoring masters generate
headers/CSVs; native C++ rules own gameplay commitments. Existing stable IDs,
realm order, consent, wages, protagonist identity and OPEN mysteries are retained.
The older foundations spec contains superseded names and is not authority for
renaming current content. No new story or mechanic is introduced here.

Inspected the input/menu-to-equipment and settlement-service save paths in detail;
surveyed campaign/realm objectives, quest/return authoring, save routing and
rejection, UI preferences, asset preparation and build handoff. Ran the existing
portable scenarios across these domains. This is a repository-wide inventory
with a focused transaction audit, not an exhaustive proof of every runtime flow.

## Prioritized evidence backlog

| Priority / state | Evidence and next acceptance criterion |
| --- | --- |
| P0 implemented; UE pending | `Private/Items/UnmadeEquipmentComponent.cpp::UseConsumable` saved potion loss before healing, while `Private/Player/UnmadeCharacter.cpp::CaptureSession` captured old health. Shared `Public/Save/UnmadeRecoveryRules.h` now applies healing before synchronous save and restores health/stamina on failure; caller restores inventory. Immediate reload must preserve both cost and benefit. |
| P0 implemented; UE pending | `Private/World/UnmadePlayerExperience.cpp::ExperienceAction` saved finite specialty use before Eillun healing / Saltwake Strain relief. Both persistent benefits now precede the same world commit; failed writes restore the service ledger and player values. Strain rollback copies the model to retain transient Fold/Glimpse windows. Temporary combat buffs retain existing session-only behavior. |
| P1 implemented; UE pending | `Public/Save/UnmadeCombinedSaveRules.h` preflights every present combined subrecord with disposable production models before profile availability, selection, restoration or backup rotation. Invalid records never reach reward reconciliation. Automatic recovery stays pinned until primary success. The legacy fieldwork sidecar is checked when needed and cannot bypass a rejected combined record. Portable malformed/failed-write/repeated-reload tests pass; actual platform IO remains unverified. |
| P1 implemented; UE pending | Potion adapters return unavailable / used / save failed. Storage failure stops the potion search, restores staged costs/benefits and reports the failure. Load/recovery notices and profile availability now explain rejected records. Verify actual widget visibility and IO errors in Unreal. |
| P1 implemented; UE pending | `Tests/integration/combined_slice_scenario.cpp` composes the opening, combat health, both civic choices, Saltwake/Cinderhold first chapters, both Bellwold return outcomes and consenting wage guarantees through the combined boundary. 306 successful/faulted world commits and 112 reward/contract write failures converge under reload. This exercises production models and shared predicates, not actors or platform IO. |
| P1 next portable batch | Extend `Tests/world/realm_journey_test.cpp` paths through the combined record: nine realm rescue/evidence branches, boss resolution, witnessed return, owned weapon/armor selection and wing progress. Include failed claim/selection writes, reload and recovery without losing ownership or reporting uncommitted benefits. |
| P1 UE/UI acceptance | `Private/UI/UnmadeRpgWidget.cpp::Open/NativeOnKeyDown`, `Private/UI/UnmadePlayerPreferences.cpp`, `Config/DefaultInput.ini`: native pages, actions and mappings exist. Verify actual focus/scroll, all controller actions, name entry, rebound prompts, text scale, contrast and paused-menu feedback with a real widget tree. |
| P1 production work | `Authoring/main_quest_interaction_contracts.json`, `resident_return_encounters.json`, `generated/scene_build_manifest.json`: contracts and import references exist; they are not staged full dialogue/production levels. Replace the opening slice's proxies by stable actor/asset ID, with interruption and return variants tested before expanding. |
| P1 engine/build gate | `TheUnmadeGame.uproject`, `Config/DefaultEngine.ini`, `Scripts/first_pc_build_and_test.ps1`: provisional UE 5.8, built-in Entry map and runtime graybox. UHT/Editor build, automation, collision/navigation, animation contact, audiovisual cues, performance, cooking and packaging remain pending. |
| P2 asset capability | `Scripts/build_skeletal_performance_kit.py`, audio generators, `Scripts/prepare_unreal_authoring.py`, `Scripts/unreal_editor/install_combat_events.py`: offline generators/import plans exist; no tracked game binaries were present. `game-dev` is unavailable locally. No provider job or paid generation was attempted. Prototype output is not imported/final art. |
| Publication requirement removed | Per the creator's 2026-10-16 instruction, `Scripts/check_lore_change.py` requires canon and ledger only. Illustrations/books remain optional on request; external masters are relevant only when issuing a new book. Historical release receipts/hashes and optional builders are preserved. |

Source paths in this table are relative to `Source/TheUnmadeGame/` unless a
repository directory is explicitly named.

## Verification and resume

- Original baseline: 304 Python tests passed. Recovery batch: 306 passed; optional-illustration policy follow-up: 307 passed.
- 61 portable C++17 test executables compiled with `-Wall -Wextra -Werror` and ran
  successfully; the expanded recovery test also passed after the Strain change.
- Regression strength: reverting the helper to save-before-heal produced the
  expected saved-health assertion failure; the fixed helper passed again.
- Repository structure, C++ literal validation, gameplay dependency audit,
  manuscript reconstruction, hygiene and six Node release scenarios passed.
- New recovery behavior is in CI and `Scripts/audit_gameplay_dependencies.py`.
- All UE integration edits are statically reviewed only. Disk interruption,
  actual SaveGame serialization/reload, rendering, controller, collision,
  performance and packaging have not been verified in Unreal.

Resume with the P1 opening-to-return integration batch above. Run
`python Scripts/audit_gameplay_dependencies.py --check --report audit.json`,
`python -m unittest discover -s Scripts/tests -v`, and compile/run
`Tests/save/recovery_commit_test.cpp` with the public include directory. Keep the
optional illustration path available; update artifact receipts only for actual publications.


## Combined integrity batch — 16 October 2026

Reverified PR #74 open/draft at `ffdb2e7e98500f610513b5a09cd3d37d6a0911a2`.
The fresh workspace had no checkout and its configured proxy refused Git
transport. Retrieved all 483 branch blobs through the connected GitHub API and
verified every original Git blob SHA. The local transport snapshot is retained on its own branch. Reconstructed the
exact upstream commit and verified its SHA to use as the shallow parent; review
changes therefore descend from the real PR head.
No main merge or paid operation is authorized or performed here.

Trace: schema/resume was the only session-wide check. Equipment, economy,
loadout, language, identity, fracture/conflict, rites/confluences, NPC memory,
fieldwork and world ledgers restored separately. Some world failures retained
unaffected state; equipment could read completion flags before another owner
rejected them. The previous-record backup candidate used the same incomplete
session check. A complete schema-1 preflight now restores temporary models only,
validates array shapes before indexing/narrowing, and rejects before owners or
writers see the record. Existing identifiers and valid absent-field/28-item
predecessors remain supported. Present world snapshots require their world
clock; confluences require the chronicle their owner reads first. Wide rite bit
fields cannot truncate into plausible values. No narrative consequence changes.

`Tests/save/combined_integrity_test.cpp` runs the exact production preflight,
selection and commit helpers with a portable record fixture (field parity checked
by Python). It covers 40 malformed-record cases, bad values in populated late
arrays, NPC provenance/duplicates, valid fully populated and older records,
invalid outgoing writes, incompatible schemas, explicit/automatic recovery,
backup failure, partially damaged primary failure, repeated failed retries,
repeated reloads and idempotent milestone rewards. Potion search stops on failed
persistence. This is fault injection in memory, not Unreal serialization or a
claim of filesystem atomicity. A backup-write failure can damage that backup;
the untouched primary remains the recovery source. A primary-write failure after
backup success leaves the preceding complete record. Rejected originals are
never intentionally rewritten by the validation/selection paths.

Fresh checks: 311 Python tests and 62 portable C++17 executables passed with
warnings as errors. The combined test also passed AddressSanitizer/UBSan with leak detection disabled
(the sandbox prevents LeakSanitizer from running under ptrace). Removing
the late witness check or outgoing preflight causes assertion failures. Repository,
C++ literal, transmedia, hygiene, six Node release scenarios, gameplay dependency
audit and canonical manuscript reconstruction checks pass. CI now runs the new
combined test. No illustrated artifact was produced or required.

Pending Unreal acceptance: UHT/Editor compilation, actual SaveGame read/write and
partial-write behavior, profile travel failure, BeginPlay ordering, notices inside
the widget tree, legacy fieldwork import and a full opening-to-return walkthrough.
Before platform fault testing, copy originals outside the save directory. Exercise
both civic routes, repeated recovery and reward reloads without overwriting those
copies. Artwork, book publication and paid services are not prerequisites.


## Opening-to-return integration — 16 October 2026

Baseline `c8283b2`; existing work and identifiers preserved. The new composed
scenario uses the same production save preflight, recovery/commit helpers and
reward predicates as the adapters. Both civic branches reach a resolved first
frontier chapter and distinct Bellwold return outcomes. Complete captured state
is compared after reload: health/Strain, language, choices, discoveries, tasks,
factions, inventory, player contracts, material stocks, named wages and consent.
It exercises 306 successful/faulted world commits plus 112 actually reached
reward/contract write failures (including partial damage), repeated reloads and
recovery. Mera's owned marks transfer only after her persisted consent, with the
separate municipal obligation retained and player income unchanged. The test
store models IO faults in memory; it does not execute actor save/rollback code.
Static adapter checks supplement those portable model tests.

Supported fixes uncovered during this trace:
- `LivingEconomy::Restore` summed untrusted worker and route integers before
  bounding them. A malformed combined record reproduced signed integer overflow
  under UBSan. Existing range checks now precede the arithmetic; valid balances,
  wages, trade receipts and all original constraints remain unchanged.
- Realm reward handlers ignored failed equipment selection and announced
  "equipped" after only ownership had saved. Both weapon/reselection and armor
  callers now explain the failed selection and E-to-retry path. The shared
  selection transaction restores prior slots on failed persistence; ownership
  remains committed and retryable. Actual on-screen feedback remains unverified.

The 36 existing milestone predicates and three civic player contracts were moved
unchanged to `Public/Items/UnmadeMilestoneRules.h`, so scenario tests exercise
production eligibility rather than a copied list. No reward, quest, wage, consent
rule, lore fact or save field was added. The shared test store is reused by the
original combined-record regressions.

Fresh verification: 315 Python tests, 64 portable C++17 executables with warnings
as errors, and 18 gameplay dependency checks passed. The new scenario and extreme
wage-boundary regression pass UBSan with recovery disabled and are wired into
CI/the dependency audit. Swapping return reward eligibility or removing selection
rollback fails the corresponding portable assertion. Repository, literals,
transmedia, hygiene, six Node release scenarios, manuscript reconstruction and
lore synchronization checks pass. Unreal compilation, actor reachability/order,
platform interrupted writes, UI and full playthrough remain pending. No paid
service, artwork or book publication was used.
