# THE UNMADE — First Playable Slice Implementation Plan

> **For agentic workers:** Use the host's available task-by-task implementation workflow. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build and verify a compact third-person Unreal Engine RPG slice in which the player explores a populated hub, fights using physical and reality-altering abilities, deciphers an inscription, changes a location permanently, and encounters NPCs who remember and adapt.

**Architecture:** An Unreal Engine C++ project supplies small, independently testable gameplay services, while Blueprints/data assets configure presentation, characters, authored world states, and encounters. A stable identity-and-event ledger separates facts from individual NPC beliefs; events drive AI reactions, the lexicon, quest conditions, and save data. A Windows/GPU engine host builds and tests the game; an optional on-demand Pixel Streaming endpoint allows the creator to play it on Android without owning a PC.

**Tech Stack:** Recommended Unreal Engine **5.8**, C++17+/Unreal Build Tool, Blueprint assets, Enhanced Input, Gameplay Tags, AI Perception, StateTree where useful, Unreal Automation Tests/Functional Tests, Windows packaged game, Git/Git LFS, GitHub Actions for engine-independent checks, and Pixel Streaming infrastructure matched to the chosen UE version. **5.8 is a technical recommendation, not an already-provisioned installation.**

## Global Constraints

- Source design: `docs/specs/2026-10-08-the-unmade-foundations-design-v0.3.md` (approved as design baseline 2026-10-08; filename/path in the GitHub repository is **proposed**, not currently present).
- Source repository: `matthagersenior/TheUnmadeGame`, public, default branch `main`; GitHub reported size 0 at plan time. **No existing gameplay files, automated builds, or Unreal host were found/verified.** Every file below is proposed.
- PC-first, third-person 3D, keyboard/mouse and controller; creator currently has Android and **no PC**. Streaming is a test-access path, not an Android port.
- Dark, surreal, living world; player is a customizable wandering outcast from an impossible reality. Make one small hub, adjoining path, and anomaly interior before expanding.
- Combat: one conventional weapon style, one defense, two distinguishable enemy behaviors; three fracture tiers (Glimpse, Fold, warned Rewrite). Glimpse is cheap/repeatable, Fold resolves, Rewrite persists with previewed consequence categories.
- Every **identifiable** NPC retains ID, individuality, and salient persistent memory; around five key NPCs, at least two with rich memories. Others use event-driven light simulation. Personal observations and hearsay are not interchangeable; NPCs are not omniscient.
- Include one optional activity; one short language-discovery chain; at least two distinct, persistent local conflict outcomes; contextual journal and observable NPC reactions.
- Preserve character state, lexicon, NPC memories, and world results over save/load. Do not silently delete quests, items, or actors after a Rewrite.
- Fantasy names in earlier designs remain **provisional**; code uses stable semantic IDs rather than text labels so later renames are safe.
- No paid infrastructure or GitHub publishing is presumed. The creator's explicit approval is required before purchasing cloud resources or publishing/committing changes to their repository. Keep test evidence separate from assertions of shipping readiness.
- Implement in incremental reviewable deliverables. For each task: write a failing test, verify the failure on an engine-equipped host, implement, verify passing result, run integration checks, and commit **only when GitHub write approval is obtained**.

## Proposed repository layout

```text
TheUnmadeGame.uproject
.gitignore
.gitattributes
README.md
Config/DefaultEngine.ini
Config/DefaultGame.ini
Source/TheUnmadeGame.Target.cs
Source/TheUnmadeGameEditor.Target.cs
Source/TheUnmadeGame/TheUnmadeGame.Build.cs
Source/TheUnmadeGame/Private/... (C++ gameplay and automation tests)
Source/TheUnmadeGame/Public/...  (C++ public contracts)
Content/Characters/...             (binary UE assets tracked with LFS)
Content/Inputs/...
Content/Maps/M_AlmostHub.umap
Content/Maps/M_AnomalyInterior.umap
Content/Data/...
Content/UI/...
Scripts/validate_repo.py          (engine-independent static checks)
Scripts/run_ue_tests.ps1           (engine-capable Windows host)
Scripts/package_windows.ps1        (engine-capable Windows host)
Tests/Fixtures/...                 (data expected by game tests)
docs/plans/...
docs/specs/...
.github/workflows/static-checks.yml
```

Use Unreal-generated paths for the engine project template rather than improvising serialized `.uasset`/`.umap` files as text. Text C++ code and scripts can be developed without the editor; real assets and gameplay validation need Unreal Editor or Unreal Automation Tool. Generated `Binaries/`, `Intermediate/`, `DerivedDataCache/`, `Saved/`, packaged builds, and credentials are excluded from Git. `.uasset` and `.umap` use Git LFS; do not place a multi-gigabyte engine installation in the repository.

## Interfaces (stable across tasks)

- `FUnmadeEventRecord`: `FGuid EventId`, `FName ActorId`, `FName TargetId`, `FGameplayTag EventType`, `FName RegionId`, `FName WorldVariantId`, `int64 Sequence`, `EUnmadeKnowledgeSource Source`, `FName SourceNpcId`. `Source` distinguishes `Witnessed`, `Rumor`, and `Inferred`.
- `UUnmadeWorldLedgerSubsystem::RecordEvent(const FUnmadeEventRecord&)`; `GetCurrentVariant(FName RegionId) const -> FName`; `TryApplyRewrite(FName RegionId, FName ChoiceId, FText& OutReason) -> bool`; broadcast `OnWorldEvent` and `OnWorldVariantChanged` on successful commits only. Authoritative facts, not NPC recollections, determine physical state.
- `UUnmadeStrainComponent::CanUse(EUnmadeFractureTier Tier) const -> bool`; `ApplyCost(Tier)`; `Recover(float DeltaSeconds)`; `GetWarning(Tier, FName TargetId) -> FUnmadeRiskPreview`. Parameter values configured from data assets, not hidden global constants. Rewrite must obtain user confirmation before ledger commit.
- `UUnmadeFractureComponent::TryGlimpse(AActor* Target) -> bool`; `TryFold(AActor* Target) -> bool`; `TryRewrite(FName RegionId, FName ChoiceId, bool bConfirmed) -> bool`. Only authored targets implement `IUnmadeFracturable` (`GetSupportedStates`, `ApplyTemporaryState`, `RestoreDefaultState`); invalid/null targets fail without mutation.
- `UUnmadeMemoryComponent::Witness(const FUnmadeEventRecord&)`; `HearRumor(const FUnmadeEventRecord&, FName SpeakerId)`; `GetBelief(FGameplayTag Subject) const -> FUnmadeBelief`; `WriteSnapshot()/ReadSnapshot()`. Each stable `NpcId` has traits and memory; rumor provenance is retained.
- `UUnmadeLexiconSubsystem::RecordEvidence(FName TermId, FName EvidenceId)`; `GetInterpretation(FName TermId) -> EUnmadeInterpretationState`; `OnInterpretationChanged` notifies journal and contextual interactions.
- `UUnmadeSaveSubsystem::SaveToSlot(const FString& SlotName) -> bool`; `LoadFromSlot(const FString& SlotName) -> bool`. Save format version, player choices, stable IDs, ledger, lexicon, memories, and relationships; invalid slots fail without altering the live session.

All APIs above are **proposed contracts**. Engine type/class names are fixed inside the plan for cross-task compatibility; their exact Unreal headers/overrides are established during first compilation. `FName` IDs are stable machine keys and never translated/displayed as player-facing names.

---

### Task 1: Establish an engine-valid, testable repository

**Files:**
- Create: `.gitignore`, `.gitattributes`, `README.md`, `TheUnmadeGame.uproject`, `Config/DefaultEngine.ini`, `Config/DefaultGame.ini`, `Source/TheUnmadeGame.Target.cs`, `Source/TheUnmadeGameEditor.Target.cs`, `Source/TheUnmadeGame/TheUnmadeGame.Build.cs`, `Source/TheUnmadeGame/Public/TheUnmadeGame.h`, `Source/TheUnmadeGame/Private/TheUnmadeGame.cpp`.
- Create: `Scripts/validate_repo.py`, `Scripts/run_ue_tests.ps1`, `.github/workflows/static-checks.yml`.
- Test: `Scripts/validate_repo.py`; `Source/TheUnmadeGame/Private/Tests/BootstrapAutomationTest.cpp`.

**Interfaces:** Consumes the approved spec and engine template; produces a launchable blank `TheUnmadeGame` module and `Unmade.Bootstrap` automation suite.

- [ ] **Step 1: Add a focused failing check.** Python verifier asserts existence of `.uproject`, module targets, key config files, and that `.gitattributes` routes `*.uasset` and `*.umap` to `filter=lfs`; assert `.gitignore` excludes generated folders. Add Unreal automation case `Unmade.Bootstrap.WorldLoads` expecting a basic map and valid game mode.
- [ ] **Step 2: Verify initial failure.** `python Scripts/validate_repo.py` should fail for the missing project files before they are created; on an Unreal host the new automation case must initially fail for the missing map or game mode.
- [ ] **Step 3: Create the minimum valid project.** Start from the UE 5.8 Third Person **C++** template in the Unreal Editor on an engine host; verify module names match `TheUnmadeGame`. Use stock template meshes as temporary art; create `Content/Maps/M_AlmostHub.umap` with basic collision and spawn point. Add `README.md` with engine-host prerequisites and license-safe asset instructions. Configure Git LFS and stable project defaults. Static GitHub workflow runs only Python checks and never pretends to compile Unreal.
- [ ] **Step 4: Focused verification.** `python Scripts/validate_repo.py` exits 0. `Scripts/run_ue_tests.ps1 -TestFilter 'Unmade.Bootstrap' -UnrealRoot <UE_ROOT>` invokes `UnrealEditor-Cmd.exe` with `-unattended -nop4 -ExecCmds='Automation RunTest Unmade.Bootstrap;Quit' -ReportExportPath=<reports>`; expect automation `success=true` and no project load errors.
- [ ] **Step 5: Integration verification.** Editor can open the map; third-person character spawns without warnings. `.github/workflows/static-checks.yml` succeeds without requiring Unreal installed on a standard runner.
- [ ] **Step 6: Prepare reviewed commit** `chore: bootstrap unreal project and static checks`; publish only with authorization.

### Task 2: Player controls, identity, and traversal

**Files:**
- Create: `Source/TheUnmadeGame/Public/Player/UnmadePlayerCharacter.h`, `Private/Player/UnmadePlayerCharacter.cpp`, `Public/Player/UnmadePlayerProfile.h`, `Private/Player/UnmadePlayerProfile.cpp`, `Content/Inputs/IMC_Player.uasset`, `Content/Inputs/IA_Move.uasset`, `IA_Look.uasset`, `IA_Interact.uasset`, `IA_Attack.uasset`, `IA_Defend.uasset`, `IA_Glimpse.uasset`, `IA_Fold.uasset`.
- Create: `Content/UI/WBP_CharacterSetup.uasset`, `Content/Data/DA_CharacterPresets.uasset`.
- Test: `Private/Tests/PlayerAutomationTest.cpp`, `Content/Tests/FT_PlayerTraversal.uasset`.

**Interfaces:** Consumes Enhanced Input actions and static appearance/background presets; produces `FUnmadePlayerProfile { FName AppearancePresetId; FName BackgroundId; }`, move/look/interact input callbacks and player profile snapshot for later persistence.

- [ ] **Step 1: Write tests.** `Unmade.Player.Profile` verifies valid preset and background can be selected and retained in profile; `Unmade.Player.Input` verifies no duplicate mapping contexts after re-possession; functional test verifies forward movement from spawn, rotation, and interact-distance rejection.
- [ ] **Step 2: Confirm initial failures.** Run `Unmade.Player` via `run_ue_tests.ps1`; tests fail because profile/input/actions are absent.
- [ ] **Step 3: Implement.** Use a UE `ACharacter` with spring-arm camera, Enhanced Input mapping, stable action names, and a simple setup menu. Define keyboard/mouse and gamepad bindings for locomotion/camera/interaction. A player outside interact range must receive a clear prompt or no interaction, never mutate the target. Character presets alter an actual visible mesh/material option, not only a stored label; background choice affects at least one dialogue/skill flag, not locked classes.
- [ ] **Step 4: Focused pass.** `Unmade.Player` suite and `FT_PlayerTraversal` pass on the engine host; player moves and interacts in the map.
- [ ] **Step 5: Integration.** Respawn/re-possess leaves one mapping context; opening/closing setup menu restores input focus.
- [ ] **Step 6: Prepare reviewed commit** `feat: add player profile and third-person controls`.

### Task 3: Deterministic world facts, Strain, and fracture targets

**Files:**
- Create: `Public/World/UnmadeEventRecord.h`, `Public/World/UnmadeWorldLedgerSubsystem.h`, `Private/World/UnmadeWorldLedgerSubsystem.cpp`.
- Create: `Public/Fracture/UnmadeFracturable.h`, `Public/Fracture/UnmadeFractureComponent.h`, `Private/Fracture/UnmadeFractureComponent.cpp`, `Public/Fracture/UnmadeStrainComponent.h`, `Private/Fracture/UnmadeStrainComponent.cpp`.
- Create: `Content/Data/DA_FractureRules.uasset`, `Content/Props/BP_FoldBridge.uasset`, `Content/Props/BP_RewriteStructure.uasset`, `Content/UI/WBP_FractureWarning.uasset`.
- Test: `Private/Tests/FractureAutomationTest.cpp` and `Content/Tests/FT_FractureEnvironment.uasset`.

**Interfaces:** Implements `UUnmadeWorldLedgerSubsystem`, `UUnmadeStrainComponent`, `UUnmadeFractureComponent`, `IUnmadeFracturable` specified above; emits world events consumed by NPCs, journal, and save system.

- [ ] **Step 1: Write tests.** `Unmade.Fracture.GlimpseRepeat` reveals clue without ledger variant mutation; `FoldTimeout` toggles collision/appearance and restores both after configured duration, even if player leaves range; `RewriteRequiresConfirmation` rejects without confirmation; `RewriteOneCommit` changes authored region variant once and broadcasts once; `UnknownChoiceSafe` rejects unrecognized region/choice without deleting world actors; `StrainRecovery` recovers toward stable baseline; functional test checks readable on-screen warning.
- [ ] **Step 2: Confirm absence.** `run_ue_tests.ps1 -TestFilter 'Unmade.Fracture' -UnrealRoot <UE_ROOT>` initially finds no registered suite / fails the test discovery gate; discovery absence is a **failed** verification, not a passing test.
- [ ] **Step 3: Implement minimum.** Restrict transitions to authored props and explicit choice sets. Glimpse highlights an unseen route, Fold enables a temporary bridge state, Rewrite resolves the hub structure into exactly one of two authored states. Make irreversible choice confirmation separate from ability-cost deduction, and mark WorldLedger commit only after successful validated transition. Emit witnessed events when an NPC has actual sensory evidence, not to all NPCs globally.
- [ ] **Step 4: Focused pass.** All `Unmade.Fracture` cases pass; single Rewrite produces one authoritative transition, no spurious duplicates.
- [ ] **Step 5: Integration.** `FT_FractureEnvironment` demonstrates visible geometry/collision state, timer reset, and warning; invalid target leaves world unchanged.
- [ ] **Step 6: Prepare reviewed commit** `feat: add authored reality fractures and strain`.

### Task 4: Physical combat and two enemy behaviors

**Files:**
- Create: `Public/Combat/UnmadeCombatComponent.h`, `Private/Combat/UnmadeCombatComponent.cpp`, `Public/Combat/UnmadeEnemyCharacter.h`, `Private/Combat/UnmadeEnemyCharacter.cpp`, `Content/Enemies/BP_StalkerEnemy.uasset`, `Content/Enemies/BP_WatcherEnemy.uasset`.
- Test: `Private/Tests/CombatAutomationTest.cpp`, `Content/Tests/FT_CombatEncounter.uasset`.

**Interfaces:** Consumes action inputs, character health/defense state, and fracture-target state tags; exposes `RequestAttack()`, `SetGuarding(bool)`, and `ApplyHit(FUnmadeHitContext)` with typed hit result; emits combat events to WorldLedger when witnessed.

- [ ] **Step 1: Write tests.** `Unmade.Combat.GuardReducesDamage` verifies guarding changes damage in a consistent direction; `AttackRejectsDeadTarget`; `EnemyArchetypesDiffer` asserts one enemy closes distance and the other maintains range/controls space; `FractureOpensWeakness` verifies an authored Fold changes a target's combat vulnerability temporarily.
- [ ] **Step 2: Confirm missing behavior.** Focused `Unmade.Combat` suite fails while combat actors/components are unimplemented.
- [ ] **Step 3: Implement.** One melee attack with readable windup/hit/cooldown; defense and hit reaction; two AI policies configured via data assets/StateTree, not entirely separate copied controllers. Cap spam and ensure attacks cannot hit the same target repeatedly within one swing. On player death, restore last safe checkpoint without rewriting ledger state.
- [ ] **Step 4: Focused pass.** All `Unmade.Combat` tests pass; the same action sequence gives consistent state changes in fixed-seed test conditions.
- [ ] **Step 5: Integration.** `FT_CombatEncounter` completes combat with and without using Fold; encounter can be exited/re-entered without duplicated hostile spawns.
- [ ] **Step 6: Prepare reviewed commit** `feat: add readable combat and fracture counterplay`.

### Task 5: Persistent NPC individuality, memories, and social adaptation

**Files:**
- Create: `Public/NPC/UnmadeNpcIdentity.h`, `Public/NPC/UnmadeMemoryComponent.h`, `Private/NPC/UnmadeMemoryComponent.cpp`, `Public/NPC/UnmadeNpcCharacter.h`, `Private/NPC/UnmadeNpcCharacter.cpp`, `Public/NPC/UnmadeNpcSocialSubsystem.h`, `Private/NPC/UnmadeNpcSocialSubsystem.cpp`, `Content/NPC/DA_NpcRoster.uasset`, `Content/NPC/ST_NpcRoutine.uasset`.
- Test: `Private/Tests/NpcAutomationTest.cpp`, `Content/Tests/FT_MerchantAndGuard.uasset`.

**Interfaces:** Implements `UUnmadeMemoryComponent` with witnessed-vs-rumor distinction; `FUnmadeNpcIdentity { FName NpcId; FName TemperamentId; FName OccupationId; FName RoutineId; }`; relationship state per known actor; `OnWorldEvent` updates only appropriately informed NPCs. Identity and memory snapshots consumed by save task.

- [ ] **Step 1: Write tests.** `Unmade.NPC.StableIdentity` asserts each resident has unique ID and traits; `WitnessVsRumor` preserves provenance and differentiated confidence; `MerchantMixedEmotions` raises gratitude after rescue and fear after witnessed Fold without collapsing them into one score; `GuardLearnsByRumor` shows guard belief updates only after credible contact with merchant, not from distant global event; `BeliefRevision` corrects suspicion after evidence; `PromotePreservesMemory` changes fidelity level without resetting ID/facts; `BoundedMemory` retains pivotal events while compressing routine details; `UnloadedNpcEvent` queues or applies a relevant regional fact once when resident becomes active.
- [ ] **Step 2: Confirm focused failure.** `Unmade.NPC` suite and merchant/guard functional test fail before implementation; unregistered test is not accepted as passing.
- [ ] **Step 3: Implement.** Build five authored key residents with distinct priorities and daily routine slots, plus several lightweight identifiable residents. Use AI Perception stimuli for direct witnessing, distance/social graph for optional rumor propagation, compact event facts and authored behavior modifiers. Merchant visibly changes trade interaction and seeks guard; guard visibly investigates after report; a witness of an alternate Rewrite holds a different belief than an unwitnessed neighbor. Never let NPCs know an unwitnessed player action merely because it is stored in the global ledger.
- [ ] **Step 4: Focused pass.** All `Unmade.NPC` cases pass; event processing idempotent by `EventId`, bounded memory retains salient evidence and provenance.
- [ ] **Step 5: Integration.** `FT_MerchantAndGuard` proves behavioral, not dialogue-only, adaptation; visit another area and return to verify stable identity and routine restoration.
- [ ] **Step 6: Prepare reviewed commit** `feat: give residents persistent memories and adaptive routines`.

### Task 6: Lexicon, branching conflict, UI, and location content

**Files:**
- Create: `Public/Lexicon/UnmadeLexiconSubsystem.h`, `Private/Lexicon/UnmadeLexiconSubsystem.cpp`, `Public/Quest/UnmadeQuestSubsystem.h`, `Private/Quest/UnmadeQuestSubsystem.cpp`, `Content/Data/DA_LanguageClues.uasset`, `Content/Data/DA_OpeningConflict.uasset`, `Content/UI/WBP_Lexicon.uasset`, `Content/UI/WBP_Journal.uasset`, `Content/UI/WBP_QuestChoice.uasset`, `Content/Maps/M_AnomalyInterior.umap`.
- Test: `Private/Tests/LexiconAndQuestAutomationTest.cpp`, `Content/Tests/FT_OpeningQuest.uasset`.

**Interfaces:** `UUnmadeLexiconSubsystem` tracks evidence and changes interpretation state; `UUnmadeQuestSubsystem::EvaluateConditions(FName QuestId) -> FUnmadeQuestOptions` reads world facts, player background, and lexicon—not NPC rumors as objective truth. Emits journal updates upon world/lexicon events.

- [ ] **Step 1: Write tests.** `UnknownTerm` starts unresolved; `EvidenceChangesInterpretation` two independently discovered clues unlock a safer fracture interpretation; `InsufficientEvidence` does not auto-translate; `TwoRewriteOutcomes` each produce distinct map/quest behavior; `UnchosenFaction` allows quest completion without joining a faction; `OptionalJob` is independently accessible; journal updates after event and retains earlier contradictory names as discovered records.
- [ ] **Step 2: Confirm initial failure.** `Unmade.Lexicon` and `Unmade.Quest` suite fail before subsystems/data are present.
- [ ] **Step 3: Implement.** Add concise hub, path, and anomaly interior with authored discovery: inscription, speaker, object evidence, two regional resolution choices, and one side job. Author location/NPC display names as revisable localized text in assets rather than hardcoded enum strings. Journal shows witnessed facts and unconfirmed interpretations separately; missed optional clues never soft-lock the principal conflict.
- [ ] **Step 4: Focused pass.** `Unmade.Lexicon` and `Unmade.Quest` tests pass; optional language evidence improves choices but is not mandatory.
- [ ] **Step 5: Integration.** `FT_OpeningQuest` verifies both persistent outcomes visually and materially, including altered NPC routine and available path.
- [ ] **Step 6: Prepare reviewed commit** `feat: add first mystery and language-driven choices`.

### Task 7: Save/load and end-to-end evidence

**Files:**
- Create: `Public/Save/UnmadeSaveData.h`, `Public/Save/UnmadeSaveSubsystem.h`, `Private/Save/UnmadeSaveSubsystem.cpp`, `Scripts/package_windows.ps1`, `Content/Tests/FT_VerticalSlice.uasset`.
- Test: `Private/Tests/SaveAutomationTest.cpp`, `Content/Tests/FT_SaveRestore.uasset`.

**Interfaces:** `UUnmadeSaveSubsystem` encodes a versioned `UUnmadeSaveData` snapshot with profile, world fact revision, quest choices, evidence IDs, NPC stable IDs, relationships, memory provenance, and inventory/checkpoint; returns an explicit error status to UI on failed disk operations.

- [ ] **Step 1: Write tests.** `Unmade.Save.RoundTrip` restores character preset, background, world variant, lexicon, and independent NPC gratitude/fear; `WitnessRumorRoundTrip` retains evidence provenance; `RepeatedLoadIdempotent` adds no duplicate event/memory/actor; `BadSlotDoesNotMutateLiveState`; `UnsupportedFutureSchemaFailsSafely`; `NoQuestDestructionAfterRewrite` keeps quest records consistent.
- [ ] **Step 2: Confirm test failures.** `Unmade.Save` suite fails until serialization/restore is implemented.
- [ ] **Step 3: Implement.** Snapshot compact authoritative state; restore in order: fixed IDs and authored objects → ledger → individual NPC belief/memory → quests/lexicon → UI. Write to temporary file/slot then replace on successful serialized write, and surface saving failures. Do not serialize graphics caches or transient Fold timers as permanent changes; on load, resolve those to default state.
- [ ] **Step 4: Focused pass.** All `Unmade.Save` tests pass with deterministic fixtures, including repeated reloads and an intentionally invalid slot.
- [ ] **Step 5: Integration.** Engine host runs `Unmade` automation group with `Scripts/run_ue_tests.ps1 -TestFilter 'Unmade' -UnrealRoot <UE_ROOT>`, exports structured reports, packages a Windows Development build with `Scripts/package_windows.ps1 -UnrealRoot <UE_ROOT>`, boots it, and runs one `FT_VerticalSlice` flow: character setup → job/encounter → Glimpse → Fold → language clue → warned Rewrite A or B → merchant/guard reactions → save/reload → return. A native Windows tester verifies mouse/keyboard and controller. Do not mark success if any test was undiscovered, skipped without rationale, or package fails to start.
- [ ] **Step 6: Prepare reviewed commit** `feat: preserve player and world state across sessions`.

### Task 8: Budget-aware Android remote playtesting

**Files:**
- Create: `docs/qa/android-remote-playtest.md`, `Scripts/start_pixel_streaming.ps1`, `Scripts/stop_pixel_streaming.ps1`, `Scripts/verify_stream_session.py`.
- Modify (proposed): `Config/DefaultEngine.ini` to enable the matching Pixel Streaming plugin only for remote-test packaging profile; `README.md` with documented build/test procedure.
- Test: `Scripts/verify_stream_session.py` network-unavailable/failure-mode fixtures; remote Android checklist in `docs/qa/android-remote-playtest.md`.

**Interfaces:** Consumes a verified packaged Windows UE build and an approved GPU host; produces a short-lived authenticated HTTPS playtest endpoint and a session report (host identifier, build SHA, start/end, controls used, frame/latency notes) with secrets redacted.

- [ ] **Step 1: Write tests/checklist.** Verify the script refuses missing executable, incompatible infrastructure branch, absent credentials, and port-binding conflict; records failed session startup as failure rather than publishing a broken link. A live test must demonstrate Android Chrome connects, movement/camera/attack/Glimpse/Fold/Rewrite controls work, and loss/reconnection does not erase game progress.
- [ ] **Step 2: Confirm failure.** On an unprovisioned host, startup fails explicitly with `GPU_HOST_NOT_CONFIGURED`, rather than pretending a stream is ready. The no-cost local script fixture should pass its *expected-failure* assertion.
- [ ] **Step 3: Implement after budget/host approval.** Use a GPU-equipped cloud Windows instance meeting Unreal and encoder requirements; provision a secured Pixel Streaming frontend/signaling stack **from the version-matched Epic infrastructure release**. Prefer time-limited, single-user access; secure HTTPS and TURN/STUN where required; do not expose the host's remote-desktop credentials or open a public unauthenticated control endpoint. Provide gamepad mapping and simple Android touch overlay when needed. Provision/start/stop is on-demand, not always-on. Disable billing or deprovision idle instances after a session according to provider capability.
- [ ] **Step 4: Focused pass.** Session script shows supported Android browser connected and authenticated, game footage and interactive controls received; capture connection failure cases (bad token, unsupported GPU encoder, network failure) and human-readable fixes.
- [ ] **Step 5: Integration.** Creator plays a packaged slice remotely and verifies at least one NPC adaptation and save/reload. Separately validate native PC performance and full keyboard/gamepad input with a Windows tester; remote browser responsiveness is not proof of native PC release quality.
- [ ] **Step 6: Prepare reviewed commit** `test: support on-demand android playtests` after functional verification.

## Build/test commands (engine-equipped Windows host)

The scripts are proposed by Tasks 1 and 7; they do not exist in the repository yet. Example invocations after installing the chosen Unreal Engine version:

```powershell
python Scripts/validate_repo.py
.\Scripts/run_ue_tests.ps1 -UnrealRoot $env:UE_ROOT -TestFilter 'Unmade'
.\Scripts/package_windows.ps1 -UnrealRoot $env:UE_ROOT -Configuration Development
```

The PowerShell test script should invoke Unreal's documented `-ExecCmds="Automation RunTest Unmade;Quit"` and `-ReportExportPath` flags, then parse the exported test JSON, reject missing expected suites/empty reports, reject failing cases, and exit nonzero on UE process failure. GitHub Actions without an Unreal host only checks plaintext repository structure and conventional scripts; a separate, authenticated self-hosted Windows runner or cloud build machine is needed to execute UE compilation, cook/package, and functional tests.

## Review gates and acceptance

- **Gate A: Bootstrap** — Valid UE project opens in Editor and initial map spawns a controllable pawn; engine-independent CI is truthful.
- **Gate B: Moment-to-moment play** — Character movement, basic combat, Glimpse, Fold, and deterministic hazards work without save corruption.
- **Gate C: Living world** — Five key NPCs and lightweight citizens have stable IDs; two named characters preserve distinct memories and change meaningful behavior; one rumor spreads only via evidence-supported social contact.
- **Gate D: First local narrative** — Two distinct authored outcomes to a local anomaly plus discoverable language clue affect actual world state, navigation, dialogue/behavior, and journal.
- **Gate E: End-to-end** — Save/reload continuity, package boot test, keyboard/gamepad native QA, and an authenticated creator Android remote test all pass and are documented.

A passing source-only static check is **not** a passed Unreal gameplay test. Logs and recordings must identify the exact build commit, engine version, test host, and failed/skipped checks.

## Unresolved externally observable decisions

1. **Cloud cost/host choice:** no provider or spending authorization has been granted. This blocks paid editor/build hosting and an actual Android playtest endpoint, but does **not** block drafting text files or data/test logic.
2. **Final Unreal install/version:** UE 5.8 is the current documentation/recommended target at planning time; a provisioned machine must confirm availability, plugins, build toolchain, and matching Pixel Streaming infrastructure before the project version is finalized.
3. **Art pipeline:** first slice uses temporary engine-provided or freely licensed assets until custom art and sound style are approved; provenance/license of every shipped asset must be recorded.
4. **Player-facing difficulty, control binding refinements, and exact NPC numbers beyond the approved minimum:** prototype parameters are data assets for tuning through observation, not hardcoded canon.
5. **Final names/language grammar:** present working names are non-canonical, so display strings are data-driven and localization-ready.
6. **GitHub write/publish:** baseline approval authorizes design planning, not an automatic commit to `main`. Request explicit confirmation before uploading files, creating commits, or merging in the user's repository.

## References

- Approved local design baseline: `docs/specs/2026-10-08-the-unmade-foundations-design-v0.3.md` (available in session; not yet pushed).
- Unreal engine hardware: https://dev.epicgames.com/documentation/unreal-engine/hardware-and-software-specifications-for-unreal-engine
- Enhanced Input: https://dev.epicgames.com/documentation/unreal-engine/enhanced-input-in-unreal-engine
- Unreal automation commands: https://dev.epicgames.com/documentation/unreal-engine/run-automation-tests-in-unreal-engine
- Pixel Streaming: https://dev.epicgames.com/documentation/en-us/unreal-engine/pixel-streaming-in-unreal-engine
- Pixel Streaming infrastructure (version branches): https://github.com/EpicGames/PixelStreamingInfrastructure