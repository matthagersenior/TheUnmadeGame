# The Unmade — First-PC Production Readiness Implementation Plan

> **For agentic workers:** Use the host's available task-by-task implementation workflow. Steps use checkbox (- [ ]) syntax for tracking.

**Goal:** Arrive at the first Unreal-capable Windows PC with a deterministic, canon-consistent, actionable production package, then turn the staged source gameplay into an actually playable nine-realm RPG without rebuilding design decisions.

**Architecture:** Original C++ save/gameplay rules, Authoring JSON, and the canonical dependency ledger remain sources of truth. A read-only Python builder produces realm dossiers, a source-traced catalog, and a work-order CSV from those sources, while the existing Windows quickstart produces a dated receipt. UE Editor artifacts, assets, collision, input and playable outcomes are accepted only after an actual local build and walkthrough.

**Tech Stack:** Python 3.11+ and unittest; Unreal Engine project targeting 5.8; C++17 native rules and Unreal C++; PowerShell/Windows, Git and Git LFS; optional art, audio and animation assets created/imported only with valid rights.

## Global Constraints

- No scripted prototype guide, narrative registry, passing native check, or generated illustration counts as a playable scene.
- Protect stable IDs, current SaveGame compatibility, wages/consent, evidence provenance, XP/money/resource limits, OPEN lore mysteries and confirmed player choices.
- Do not tie essential gameplay to external AI, network calls, subscriptions, remote inference, paid services, or forced grinding.
- On irreversible action: visible preview, intentional confirmation, atomic saved result, and safe failed-write rollback.
- Player observation is a possible source of study, never an automatic new ability; require authored evidence and physical tests.
- NPCs react to their own witnessed or credibly received events, not global omniscient broadcasts.
- Every route supports safe access, a retry after failure, actual controller/keyboard use, captions and a non-audio cue for time-sensitive events.
- Source canon first; only engine receipts allow status to move from UNREAL_NOT_VERIFIED.
- Do not modify production gameplay maps as part of authoring reference imports; preserve artist replacements when restaging.
- Prefer first-playable end-to-end quality over inflated location, encounter or playtime counts.

---

### Task 1: Generate a canonical, source-linked production work package

**Files:**
- Create: Scripts/build_first_pc_production_pack.py
- Create: Scripts/tests/test_first_pc_production_pack.py
- Consumes: docs/canon/dependency-ledger.json, Authoring/main_quest_interaction_contracts.json, Authoring/lived_universe_atlas.json, Authoring/city_assembly_kits_v1.json, Authoring/resident_return_encounters.json, Authoring/ability_codex.json

**Interfaces:**
- Consumes: the committed JSON structures and their existing stable record IDs.
- Produces: build(root) returning a verification receipt and a file-content map; command-line --check for read-only validation and --out-dir PATH for writing a separately generated dossier package.

- [x] Step 1: Add tests for 9 realm dossiers, 11 city guides, 54 real main-story steps, 27 authored evidence clues, 9 optional side stories, 82 registered returning residents, 99 quest ledger rows, 39 boss rows, 60 item rows, 23 learned skills and 256 Ability Codex entries.
- [x] Step 2: Refuse duplicate record IDs, unknown referenced quest bosses, out-of-order realm chapters and missing physical failure/recovery/saved-state contracts.
- [x] Step 3: Create deterministic work-orders.csv, canon-index.md, README.md and realm-*.md content from source without claiming assets were created.
- [ ] Step 4: Confirm the focused tests actually pass in GitHub Actions. Run: python -m unittest discover -s Scripts/tests -p test_first_pc_production_pack.py -v. Expected: all seven tests pass.
- [ ] Step 5: Run the complete Python suite and dependency/source checks. Expected: no regression; never substitute these for UE gameplay verification.
- [ ] Step 6: Merge a verified change to main, retaining the source commit in the PR record.

### Task 2: Make the package appear automatically on the first PC

**Files:**
- Modify: Scripts/first_pc_quickstart.ps1
- Existing launcher: START_THE_UNMADE.cmd
- Existing first-host tests: Scripts/first_pc_build_and_test.ps1 and Scripts/run_ue_tests.ps1

**Interfaces:**
- Consumes: Scripts/build_first_pc_production_pack.py --out-dir PATH and existing Python executable resolution in first_pc_quickstart.ps1.
- Produces: TestReports/quickstart-TIMESTAMP/production-pack with authored work orders plus production-pack-validation.log.

- [x] Step 1: Generate the source-linked package before any optional Editor staging work.
- [x] Step 2: Fail closed if production source coverage cannot be validated; preserve the error in a dated report.
- [ ] Step 3: On a real Windows host, run START_THE_UNMADE.cmd with and without InspectOnly. Expected: inspect mode produces a package without modifying UE worlds; normal mode keeps its existing build/automation/staging gates.
- [ ] Step 4: Repeat after a clean fetch of main. Expected: the package follows the exact checked-out source revision and does not depend on private workstation files.
- [ ] Step 5: Preserve the current noncollision/non-authoritative 402-city-guide behavior and 7-DataTable import receipt.

### Task 3: Complete every PC-independent source and authored interaction

**Files:** Existing system owners under Source/TheUnmadeGame, Authoring, Scripts, docs/design, docs/canon; do not introduce competing gameplay/saving registries.

**Interfaces:**
- Consumes: existing C++ domain rules, canonical scene tags, quest authoring and generated work order CSV.
- Produces: tested source patches and a single canonical evidence trail. Each added interaction names its trigger, failed-state behavior, recovery, save owner, seen-by rule, and player-visible feedback.

- [ ] Step 1: Audit every MAIN_STEP order for a real C++ handler, not just a dialog line or staging marker. For missing handlers, add a source-level rule and a negative/failure test; preserve the original existing save owner.
- [ ] Step 2: Audit all SIDE_STORY orders, especially source-labeled authoring-only LUQ arcs, to distinguish written steps from actual playable and save-backed quest progression.
- [ ] Step 3: Reconcile quest prerequisites and 99 ledger records with actual reward acquisition and world aftermath; test mutually exclusive branches, failed saves, and replay grants.
- [ ] Step 4: Reconcile all 39 cataloged boss records with actual enemy rules, boss entrances, alternate defeat/yield options and finite rewards. Mark catalog-only or placeholder implementations explicitly.
- [ ] Step 5: Audit all Ability Codex records and 23 learned skill rows for truthful observation, understanding, experiment, mastery, prerequisite and UI explanations. No definition-only or proposed record is granted at runtime.
- [ ] Step 6: Confirm 60 catalog items and 18 regional gear rows are earnable only by their real pathways, with unique assets/slot preview requirements assigned to a future Editor work order.
- [ ] Step 7: For 82 saved resident return encounters, test independently witnessed/told/inferred memory, refusal, time-of-day, dialogue ownership and per-person saved replay, without forcing artificial NPC knowledge.
- [ ] Step 8: Produce stable offline subtitle/script IDs, UI focus maps, combat tells, fallback audio captions, and sensible non-grind encounter and item economy tuning sheets tied to canon and explicit test cases.
- [ ] Step 9: Run python Scripts/validate_repo.py, python Scripts/validate_ability_codex.py, python Scripts/validate_dependency_ledger.py, python -m unittest discover -s Scripts/tests -v, and python Scripts/audit_gameplay_dependencies.py --check. These gates prove only their recorded source scope.

### Task 4: First day on the Unreal-capable PC — real compilation, never imagined playability

**Files:** TheUnmadeGame.uproject, Config, Source, Scripts/first_pc_quickstart.ps1, Scripts/unreal_editor/first_pc_editor_batch.py, TestReports.

**Interfaces:**
- Consumes: checked-out main SHA, correct compatible Unreal installation and native toolchain, the generated production dossier package.
- Produces: a retained Unreal Header Tool/Editor build result, automation report, separate staging-world receipt and actual editor screenshots or logs.

- [ ] Step 1: Install/verify a compatible target UE build, Visual Studio C++/SDK, sufficient storage and required Git LFS assets; do not assume a version is present because it was documented.
- [ ] Step 2: Clone main and preserve SHA; execute START_THE_UNMADE.cmd. If host preflight fails, correct only the named prerequisite and rerun.
- [ ] Step 3: Compile the complete Editor target and run all registered Unmade Unreal automation filters. Failed or skipped tests are blocking, not green.
- [ ] Step 4: Inspect generated DataTables and separate guide worlds. Confirm all 402 city guides are noncolliding and cannot complete quests. Do not overwrite authored production maps.
- [ ] Step 5: Start PIE in the Entry world with a clean profile. Confirm character spawn, movement, camera, basic physical attack/guard, Glimpse/Fold/Rewrite cues, and a real interactable witness.
- [ ] Step 6: Record crashes, compiler errors, missing assets, unsafe collision, focus bugs and save failures with exact SHA, logs, screenshots and reproductions. Fix by severity before art expansion.

### Task 5: Turn the Threefold Reach into the first complete player-quality region

**Files:** Unreal maps/assets and existing native quest/interaction owners; assign stable tags from the per-realm dossier.

**Interfaces:**
- Consumes: all six Threefold main beats, existing Threefold settlements/city source, actual authority and save rules.
- Produces: one fully playable scene sequence, with physical choice, evidence, resident reaction and save-backed return.

- [ ] Step 1: Replace only the Threefold staging proxies with true navigable roads, doors, landscape, hazard indicators, geometry, bridges and ordinary community activity. Keep a safe accessible alternate route.
- [ ] Step 2: Stage the three physical evidence clues, correct dialogue and firsthand provenance, main quest six beats, both mutually exclusive permanent choices, and the optional local side story without accidental gating.
- [ ] Step 3: Add real targeting/input interactions, HUD/journal, readable confirmation, save/reload, rejection on failure, and different return reactions for the two outcomes.
- [ ] Step 4: Add polished combat tells, collision, player animation, hit reactions, boss defeat and evidence-based mercy; run controller-only and no-audio passes.
- [ ] Step 5: Complete a clean walkthrough on both divergent saves, force quit/reload during each transition, revisit on another day and measure frame times and navigation failure rates.
- [ ] Step 6: Promote this verified interaction + performance + accessibility recipe as a template for the other eight realms; do not duplicate lore or reuse identical world identity.

### Task 6: Scale only verified practices through eight additional realms, then certify a complete campaign

**Interfaces:**
- Consumes: nine generated realm dossiers, all cataloged cross-realm content and the proven Threefold interaction contract.
- Produces: 9 individually distinguishable playable regions, two outcomes per local first chapter, valid alternative main path, final boss and persistence, actual end-to-end release evidence.

- [ ] Step 1: Build Saltwake and Cinderhold by their own realm ecology, architecture, economy, hazards, dialogue, and branch/return assets; perform full route and save tests.
- [ ] Step 2: Build Drevlach, Orravane and Vathless, including their differently constrained mechanics and alternative ethically meaningful solutions.
- [ ] Step 3: Build Eillun and Tharniv with bounded consent and community rights, and preserve valid authored alternate main-route progression.
- [ ] Step 4: Build Auvren, both Nhal-Vey stages, two confirmed endings, return variants and voluntary rematch without replacing the old save.
- [ ] Step 5: Walk a legal short critical path and a nine-realm content-rich path. Show reachable objectives; no invisible level/skill/grind gate or unearned shortcut.
- [ ] Step 6: Test all optional guardians, Echo arcs, confluences, learning trials, workshops, equipment and economy flows as independently saveable additions, not hidden mandatory quests.
- [ ] Step 7: Produce real UI, animation, VFX, VO/music/ambience, subtitles and accessibility settings for any advertised player-facing feature. Keep fallback when voice or remote AI is disabled.
- [ ] Step 8: Run packaged Win64 acceptance: new and upgraded save, sleep/resume, alt-tab, controller-only, keyboard-only, corruption/IO failure, GPU/CPU/VRAM budgets, memory leak soak, subtitles, reduced-motion and difficult-choice warning.
- [ ] Step 9: Compare measured first-run/optional/completionist hours with projected playtime. Remove meaningless filler; add meaningful choices and consequences where depth is lacking.

## First-PC Definition of Done

- The project compiles in an actual installed Unreal Engine version with zero hidden toolchain assumptions.
- Every source-defined interactive object has an accountable actor, trigger, player-facing presentation, failure/retry and save authority; anything not yet built is visibly marked unverified.
- One full Threefold act, choice, resident response, travel and return works in PIE without cheats or invented progress.
- Keyboard and controller navigate all needed first-region menus; audio is optional to understand hazards.
- The first-PC creator can use one launcher, one dated report directory and a stable source-linked work-order list rather than reconciling many lore documents.
- Nine-realm/full-game release readiness is declared separately, only after actual in-engine and packaged walkthroughs.

## Unresolved decisions (not blockers for source handoff)

- Final PC GPU/CPU/RAM/storage limits determine LOD/performance targets and the art import order.
- Asset sourcing and actor/voice ownership licensing must be determined before final art and audio shipment.
- Final localization languages, store targets and release schedule determine packaging/distribution work, but do not change stable canon or the first-PC source/Editor tests.
