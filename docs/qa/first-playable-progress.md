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

Integration watch: compare PR #74 with live main before publishing batches;
keep changes focused and avoid history rewrites. Current check: zero behind main
`9606fec4`, cleanly mergeable. Sync only when new base changes create a concrete
integration need; preserve development momentum and do not merge into main.

## Prioritized evidence backlog

| Priority / state | Evidence and next acceptance criterion |
| --- | --- |
| P1 timed progression implemented; UE pending | Source owners bind every combined write to the registered world. Blocked worlds stop clock/rite-day/rumor advancement. Visits retry in place; failed day transactions restore production, wages, support and households together. Quiet gossip retries retain absent residents. 192 fault paths and 4,800 reloads pass portable tests; verify real timer/lifecycle ordering in Unreal. |
| P1 implemented; UE pending | Startup rewards wait for registered player resume and world readiness. Failed starter/milestone/civic writes retry; checkpoints remain incomplete. Central saves require a restored owner and ready, accepted world. Older selected gear and spent starter supplies survive reloads. Verify actual BeginPlay/travel ordering and feedback in Unreal. |
| P0 implemented; UE pending | `Private/Items/UnmadeEquipmentComponent.cpp::UseConsumable` saved potion loss before healing, while `Private/Player/UnmadeCharacter.cpp::CaptureSession` captured old health. Shared `Public/Save/UnmadeRecoveryRules.h` now applies healing before synchronous save and restores health/stamina on failure; caller restores inventory. Immediate reload must preserve both cost and benefit. |
| P0 implemented; UE pending | `Private/World/UnmadePlayerExperience.cpp::ExperienceAction` saved finite specialty use before Tharniv healing / Saltwake Strain relief. Both persistent benefits now precede the same world commit; failed writes restore the service ledger and player values. Strain rollback copies the model to retain transient Fold/Glimpse windows. Temporary combat buffs retain existing session-only behavior. |
| P1 implemented; UE pending | `Public/Save/UnmadeCombinedSaveRules.h` preflights every present combined subrecord with disposable production models before profile availability, selection, restoration or backup rotation. Invalid records never reach reward reconciliation. Automatic recovery stays pinned until primary success. The legacy fieldwork sidecar is checked when needed and cannot bypass a rejected combined record. Portable malformed/failed-write/repeated-reload tests pass; actual platform IO remains unverified. |
| P1 implemented; UE pending | Potion adapters return unavailable / used / save failed. Storage failure stops the potion search, restores staged costs/benefits and reports the failure. Load/recovery notices and profile availability now explain rejected records. Verify actual widget visibility and IO errors in Unreal. |
| P1 implemented; UE pending | `Tests/integration/combined_slice_scenario.cpp` composes the opening, combat health, both civic choices, Saltwake/Cinderhold first chapters, both Bellwold return outcomes and consenting wage guarantees through the combined boundary. 306 successful/faulted world commits and 112 reward/contract write failures converge under reload. This exercises production models and shared predicates, not actors or platform IO. |
| P1 realm loops implemented; UE pending | `Tests/integration/realm_local_loops_scenario.cpp` composes all nine realm identifiers across 54 branch/accord variants: witnessed return and equipment, mechanisms/surveys, road repairs, personal routes/caches/mastery, resident days/scenes, wings/guardians/upgrades, finite services and field recipes. 3,378 combined reloads, 7,296 failed-write recovery cases and 216 healing-service transactions pass. See [the realm checklist](realm-local-loop-checklist.md); this is model coverage, not a physical playthrough. |
| P1 field kits implemented; UE pending | Earned kits can suppress any active unresolved realm chamber while preserving the supplying realm's favor/stock ledger. Save failures restore stock and report failure. All 81 supplier/destination pairs pass portable fault/reload checks; caches remain free and suppression session-only. |
| P1 chapter consistency implemented; UE pending | `Tests/integration/echo_guardian_recovery_test.cpp` composes 96 later-realm first/aftermath/Echo/guardian paths, independent choices, resonance and individual returns. 5,760 reloads and 3,648 failed-write recoveries pass. Combined preflight rejects contradictory present chapter records before availability or writes; missing optional sections stay optional. Generic strangers no longer claim remembered changed mornings. |
| P1 endgame consistency implemented; UE pending | Final-morning commits now save the return position with the chosen world and restore both on failure. Present final/Auvren and resonance/mastery/clock records must agree. Tests cover both mornings, six seeds, optional rematches, 216 reloads and 36 failed moves/writes. Optional side chapters remain optional. |
| P1 defeat retries implemented; UE pending | Guardian/final-defeat commits attempt immediately, then retry failure every five active-world seconds instead of every actor tick. Success suppresses repeat commits for that phase. Portable tests cover 24 guardian and 72 final-phase fault/retry paths; confirmation expiry, retargeting, cancellation and consumed consent remain safe. |
| P1 pending checkpoints implemented; UE pending | Session saves refuse to report success while an encounter receipt is pending. Read-only actor queries include hidden receipt owners and distinguish an old final body from the new phase. Load/New preserve unsaved-discard confirmation; explicit recovery never pre-saves. 114 portable fault sessions verify blocked checkpoint writes and normal saves after receipt commit. |
| P1 handoff ownership implemented; UE pending | Target selection stays staged until a new-world owner registers. Travel failure restores original selection; a missing owner/failed world cannot unlock writes. Reentrant requests are rejected and destination backup pins survive arrival. All 64 profile/recovery combinations pass portable checks; UE delegate/build, startup ordering and real failed travel remain pending. |
| P1 next connected batch | Audit action-triggered and component-only save writers against unsaved world/NPC state. Keep actual startup, travel, UI/controller and platform IO pending; recheck drift before publishing. |
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

Resume with the P1 pre-registration initialization/readiness batch above. Run
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


## Realm-local loops — 16 October 2026

Baseline `f209064`. Broadened the audit beyond the dungeon return marker, using
the [realm-local checklist](realm-local-loop-checklist.md) to track the place's
existing mechanisms, people, services, crafting and return consequences together.
No new quest, lore, art requirement or save field was added.

Found and fixed two additional capture-order sites across all nine variants:
`TryIdentityInteraction` (hunt ration / daily sanctuary) and `TryDepthInteraction`
(local-favor provisions). Both saved their spent ledger before Heal(35), so an
immediate reload could lose the benefit. The existing shared healing transaction
now stages health before the same world write; failure restores health/stamina
and the caller's identity/depth/challenge state. Eligibility, prices, health cap,
35-health amount, four-provision limit, hunt uniqueness and daily rules remain.
Old source-contract assertions that incorrectly required save-before-heal were
corrected. The cache sign now reflects its already free, route-gated discovery.
A stale engineering label was corrected: index-7 healing is Tharniv, while Eillun
(index 6) retains its existing quiet-indexing specialty. No realm ID was renamed.

The portable combined-record scenario covers 54 realm-local variants, 3,378 full
reload checkpoints, 7,296 failed-write recoveries and 216 successful hunt/rest/
provision transactions, including full-health refusal, clamped healing, once-only
and daily limits, ingredient/output/skill accounting and retained equipment
ownership. Reverting the shared helper to save-before-heal fails the saved-health
assertion. It executes production models/helpers and captured-record restoration;
actual actor conditions, rollback and serialization remain engine-dependent.

Fresh checks: 318 Python tests, 65 portable C++17 executables with warnings as
errors and 20 gameplay dependency checks pass. The expanded realm scenario passes
UBSan with recovery disabled and runs in CI/audit. Source structure/literals,
transmedia, hygiene, six Node release cases, manuscript reconstruction, lore sync
and whitespace checks pass. No paid services or illustrated publication used.

Unresolved gameplay choice: field kits cannot currently reach their intended
active local hazard under the authored purchase/use prerequisites. The optional
preference question is pending; existing ledgers and story gates remain intact.
Engine/UI/playthrough acceptance remains pending for every checklist row.

## 16 October 2026 — Portable field kits and realm returns

Earned field kits can suppress another realm's active, unresolved danger chamber.
The supplying realm retains its favor and kit ledger; local stock takes priority,
then the first stocked realm in stable order. Existing purchase prerequisites,
20/30-second local binding duration, chart/boss exclusions and free caches remain.
A failed world write restores the kit and applies no suppression; its specific
save-error feedback survives the player action. Suppression remains session-only.
No save fields, IDs, consent rules or authored consequences changed.

Portable coverage adds 81 supplier/destination transactions with failed backup
and primary writes, recovery and repeated reloads, plus both authored aftermath
endings across all nine realms combined with individual resident return ledgers.
This proves model/record composition, not actor reachability or witness knowledge.
Echo/guardian consequence composition and actual Unreal interaction/serialization
remain the next batch and engine acceptance respectively. Artwork stays optional.

Verification: 318 Python tests, 66 portable C++17 executables with warnings as
errors, and all 22 gameplay dependency checks pass. The new scenario passes UBSan;
removing failed-write kit rollback causes its retained-stock assertion to fail.
Structure, literal, transmedia, hygiene, six Node release cases, manuscript and
whitespace checks pass. Actual Unreal compilation and runtime remain pending.

## 16 October 2026 — Echo, guardian and return consistency

Combined-save preflight now checks the existing later-realm chapter gates when
both related optional sections are present: first-chapter completion before
later-realm aftermath/Echo progress, resolved aftermath before Echo progress,
and completed Echo before guardian resolution or resonance progress. Individually
well-formed but contradictory subrecords no longer appear available or reach
backup rotation. Missing optional sections retain the existing legacy policy;
there are no new save fields, identities or synthesized story outcomes.

Generic return dialogue no longer claims shared memories of a changed morning
for strangers or first-day acquaintances. Existing individual aid, consent and
public records remain separate. Echo conversation save failures now explain that
progress is unchanged and can be retried. A mismatched Echo control points to the
matching control rather than promising testimony changes the rules do not allow.

New portable composition covers 96 six-realm initial/aftermath/Echo/guardian paths,
5,760 full reloads and 3,648 failed backup/primary-write recoveries. It preserves
independent authored choices, both guardian outcomes, two-clue testimony, explicit
confirmation, resonance, individual distinct-day returns and non-farmable endings.
Contradictory outgoing records are refused before writes; invalid primaries select
the good backup. All sixteen optional chapter-presence combinations retain load
compatibility in the final valid record. This is source/model/record evidence,
not a claim of Unreal actor execution or actual filesystem crash safety.

Next batch: trace resonance/final-morning prerequisites and public-versus-personal
NPC recognition through their adapters. Engine compilation, actor reachability,
confirmation/controller presentation and actual interrupted IO remain pending.
Illustrations remain optional; no paid services or main merge are required.

Fresh verification for this batch: 319 Python tests, 67 warning-clean portable
C++17 executables, all 24 gameplay dependency checks and the new scenario under
UBSan pass. The pre-fix record is reproduced by the test; removing chapter checks
or removing the acquaintance gate is rejected by regression assertions. Source
structure/literals, transmedia, hygiene, six Node release cases, manuscript,
lore synchronization and whitespace checks pass. Prior commit `75a8da0` GitHub
source-checks also passed; new-head CI is reported separately.

## 16 October 2026 — Final-morning position and resonance prerequisites

The final-morning adapter previously saved the new world before relocating the
player, so immediate reload retained the old arena position. A shared portable
transaction now stages the existing return point before synchronous combined
capture. Failed movement writes nothing; failed writes restore the prior morning
and position. Success feedback/world refresh follows persistence. The real actor
movement callback and rollback still require Unreal verification.

Combined preflight checks Auvren first-chapter completion before final progression
when both optional records exist. Present Tenfold records must show the first
resonance rite mastered for a pending window, and both rites for an open archive.
Future-dated resonance windows are rejected against their saved world clock;
expired windows remain valid and restartable. Missing optional chapter/rite
sections retain their existing legacy policy. Optional Echo/guardian/observatory
quests remain unnecessary for the final encounter. No save IDs, authored choices,
consent, lore outcomes, item amounts or art requirements changed.

Portable tests cover both mornings and six memory seeds (12 paths), 216 full
reloads, 36 failed writes/moves, inventory/individual-history retention, optional
rematches, all nine public local records and 24 resonance/mastery combinations.
Expired windows restart and persist; contradictory records are rejected. A
save-before-relocation mutation fails the saved-return-position assertion.
The chapter fixture is shared by the existing 96-path scenario and final tests.

Next: bound automatic retry of failed guardian/final-defeat commits and verify
interrupted confirmations/repeated retries without duplicated progression or
misleading feedback. Engine build, world traversal, UI/controller and actual
platform serialization/interrupted IO remain pending. Artwork remains optional.

Fresh checks: 320 Python tests, 69 portable C++17 executables with warnings as
errors, all 28 gameplay dependency checks, final recovery and prerequisite cases
under UBSan, and the saved-position mutation pass. Structure/literals, transmedia,
hygiene, six Node release cases, manuscript, lore synchronization and whitespace
checks pass. Prior head `1cc6761` also passed GitHub source-checks. This remains
portable/static evidence; no Unreal build or playthrough was performed.

## 16 October 2026 — Bounded defeat-save retries

Guardian and final-boss defeat branches retried failed persistence on every
0.1-second actor tick. Both now use a shared session-only scheduler: attempt
immediately, retry failures after five active-world seconds, and stop after a
successful commit for that realm/phase. Actor/phase configuration resets the
scheduler. Existing defeated state and transaction rollback remain authoritative;
no victory, reward, health reset or next phase is invented on a failed save.
The first failure explains automatic retry and loss of unsaved progress on reload.
Other enemy receipt paths already have bounded delays and remain unchanged.

Portable coverage adds 24 guardian and 72 final-phase failed-write/retry paths,
including damaged backup/primary writes, recovery selection and suppression of
repeated commits. A minute-long outage produces 13 callback attempts rather than
601. Invalid clocks and new phases/resets are covered. Expired, changed-control,
cancelled, consumed and reloaded confirmations cannot replay prior consent; no
consent rule changed. Removing the delay or settled guard fails regression tests.

Fresh checks: 322 Python tests, 71 warning-clean portable C++17 executables and
all 32 gameplay dependency checks pass. New tests pass UBSan. Structure/literals,
transmedia, hygiene, six Node release cases, manuscript, lore synchronization and
whitespace checks pass. Prior head `ad96172` passed GitHub source-checks. Actor
execution, real IO, actual retry timing and feedback/controller flow remain
Unreal acceptance tasks. Save identifiers, lore and optional artwork are intact.

Next: audit manual saving and profile switching while a defeat receipt is still
pending, especially whether success feedback accurately describes the committed
state. Keep engine checks pending and preserve recovery-source protection.

## 16 October 2026 — Checkpoints with pending encounter receipts

A paused-menu Save Now previously wrote a checkpoint without an encounter's
pending defeat receipt and reported success. Load/New Profile therefore skipped
the existing unsaved-discard confirmation and could lose that victory. Session
checkpoints now return pending before their writer runs. The menu explains that
Continue lets automatic retries run; Load/New keep their existing explicit
unsaved-discard confirmation. Backup recovery still never saves the current
session first. Receipt transactions themselves retain access to the normal
writer so they can finish and unblock the next checkpoint.

The read-only query includes hidden defeated expedition actors, wing guardians/
observer, road/Cinderhold elders, realm guardians and the final boss. Ordinary
foes with no durable receipt do not block saves. Final actors compare configured
phase with durable phase, so the old defeated body does not imply a defeat of the
new, unfought health bar. No progression, consent, lore or save identifier changed.

Portable tests cover 114 failed-write sessions: 12 guardian, 12 final-phase and
90 expedition cases. They check zero checkpoint writes while pending, failed
receipt rollback/recovery, target-profile isolation, explicit discard/recovery
confirmation, normal post-commit saves and later ordinary write failures. Actor
queries and widget routing have supplementary source checks; actual menu/travel
and serialization are not executed. Removing the checkpoint gate or final-phase
identity guard fails the composed assertions.

Fresh checks: 325 Python tests, 72 warning-clean portable C++17 executables, all
34 gameplay dependency checks and the new scenario under UBSan pass. Structure,
literals, transmedia, hygiene, six Node release cases, manuscript, lore sync and
whitespace checks pass. Prior head `4b65dc8` passed GitHub source-checks. Unreal
actor discovery, pause/retry timing, feedback/controller and actual IO remain
pending. No paid service, artwork or book publication was required.

Next: inspect profile handoff failure/startup and restoration of the original
profile/settings if travel cannot complete, without redirecting old live state
into the selected target. Actual travel-failure verification needs Unreal.

## 16 October 2026 — Profile handoff ownership and integration watch

Rechecked PR #74 against live main: base `9606fec4`, starting head `5a4575d`,
10 commits ahead and zero behind; GitHub reported clean mergeability. No sync,
merge into main, rebase or force push was needed. Keep focused commits and check
base divergence/overlapping files before publishing batches. If main moves,
inspect and verify an isolated integration before updating this feature branch;
branch housekeeping must not stop useful development without a concrete issue.

Profile selection previously persisted the target before travel and relied on
any later player registration to clear the write lock. Handoff now stages its
destination in memory, gates writes and accepts a different world in the same
game instance before persisting the selection. Destination backup recovery is
pinned in that staged selection. Travel failure restores the original profile/
recovery preference and resumes the original live owner only if it is still the
active one. Otherwise, the failed/partially initialized world stays write-locked;
an explicit fresh profile load remains available. Reentrant Load/New requests do
not redirect the pending target or allocate extra profiles. Delegate hooks are
removed on acceptance/failure, and destination notices wait for its arriving
player. No save IDs, valid older saves, consent, authored outcomes or lore change.

All 64 profile/recovery combinations pass portable ownership, failed-arrival,
source/target isolation, reentrancy, fresh-owner and backup-pinning checks. Removing
the write barrier, old-world arrival check, missing-owner lock or failed-world
guard fails regression assertions. Source checks cover the UE event/delegate and
routing; actual travel failures, delegate signature/build and BeginPlay ordering
remain Unreal acceptance tasks. Registration does not prove every actor restored.

Fresh checks: 328 Python tests, 73 warning-clean portable C++17 executables, all
36 gameplay dependency checks and the new handoff test under UBSan pass. Structure,
literals, transmedia, hygiene, six Node release cases, manuscript, lore sync and
whitespace checks pass. Prior head `5a4575d` passed GitHub source-checks. Artwork
and book builds remain optional; no paid services or main merge were used.

Next: inspect initialization/readiness of inventory and reward writes before the
player registers, particularly fresh-profile startup and recovery. Keep actual
Unreal startup, UI/controller and interrupted platform IO pending.

## 2026-10-16 — Deferred startup rewards and complete save readiness

Equipment BeginPlay now restores without awarding or writing. Starter gear and
committed milestone/civic rewards reconcile after the registered player restores
its resume state and the world is ready. Failed commits remain pending and retry
at five-second intervals; a manual checkpoint reports failure until startup
reconciliation succeeds. The player receives a pending reward/save/capacity notice.
The shared writer refuses unregistered, unrestored, wrong-owner, rejected or
unready worlds before capturing or rotating records. Explicit unused-profile
creation still only creates a new slot. Existing IDs, reward amounts, authored
choices, consent and optional older save sections are unchanged. A starter kit
fills empty equipment slots only, preserving selected older equipment; its marker
prevents spent supplies being refilled on repeated reloads.

Portable evidence: startup tests combine the production kit transaction and
combined-record store with primary/backup write damage, recovery selection,
rollback, 400 reloads of spent supplies, retained legacy equipment and full-stack
rejection. Removing readiness, rollback or repeat suppression fails assertions.
Adapter checks verify startup deferral, retry and checkpoint gating. These are
portable rules and static wiring checks, not Unreal lifecycle or filesystem tests.

Next: audit world simulation and timed progression writes during initialization
and profile travel, then verify late-owner failures cannot advance durable time
or rewards. Unreal build/UHT, actual startup/travel, UI visibility and interrupted
platform IO remain pending. Artwork/books stay optional; no charges or main merge.

Fresh batch verification: 331 Python tests, 74 warning-clean portable C++17
executables and 38 gameplay dependency checks pass. Startup rules pass UBSan;
readiness, rollback and repeated-award mutations are rejected. Structure, literal,
transmedia, hygiene, six source-release scenarios, manuscript and whitespace checks
pass. Live main remains `9606fec4`, zero commits behind; PR #74 is mergeable.
Prior head `96d9702` passed source-checks; this new head requires its own CI.

## 2026-10-16 — World ownership and retryable timed progression

A blocked startup/profile handoff no longer advances the living-world clock,
rite-day effects or rumor propagation. Every combined writer now supplies its
source owner; that owner's world must match the registered, restored player's
accepted world before capture or backup rotation. Late callbacks from an old
world cannot populate a destination profile after its new owner registers.

Settlement visits now commit independently of arrival captions. A failed visit
rolls back and retries while the player remains nearby; it no longer requires
leaving and re-entering the settlement. Material production, named wages,
foreign support and Bes's household advance as one staged day transaction.
Failures restore all three models and retry no more than once per five active
world seconds. Catch-up uses each simulated material day, preserving walkout,
finite treasury and explicit Mera guarantee rules. Rite-day debt changes retry
with the same cadence, and bonuses refresh after the attempted transition.
First-failure notices explain pending visits, world days and rite consequences.
No identifiers, reward amounts, consent requirements or authored outcomes change.

Rumor failures remain pending for the next eight-second rumor timer even when
no new conversation occurs. World checkpoints also capture current NPC memories.
Both world and player memory saves replace rows only for the same stable person,
preserving histories of unloaded residents. Shared portable replacement rules
verify stable identity, repeated updates, retained absent witnesses and rumor
provenance; malformed observations cannot replace a good record or backup.

Portable coverage: 192 timed fault/recovery paths across all four profiles,
primary/backup failures, recovery pins and malformed outgoing subrecords; 4,800
reloads; ordinary work, Bes's walkout, Mera's explicit guarantee and actual
foreign shelter support; blocked clocks and old/new-world identity; lagging-day
catch-up and legacy initialization without retroactive production. These use
production rules and in-memory fault storage. Unreal actors, actual timer order,
serialization and filesystem interruption remain unverified.

Next: audit action-triggered checkpoint writers and active-owner state capture,
especially component-only saves versus unsaved world/NPC consequences. Keep
Unreal UHT/build, startup/travel, UI/controller and interrupted IO pending.
Artwork/books remain optional; no charges or merge into main.

Fresh batch checks: 335 Python tests, 75 warning-clean portable C++17 executables
and all 40 gameplay dependency checks pass. The final timed scenario passes UBSan.
Seven mutations removing readiness, visit/rite/day rollback, retry cadence, world
identity or resident identity are rejected. Structure, literals, transmedia,
hygiene, six Node release scenarios, manuscript reconstruction and whitespace
checks pass. Baseline `893c10a` passed GitHub source-checks; new-head CI is separate.
