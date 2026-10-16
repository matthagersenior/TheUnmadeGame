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
| P1 next supported batch | `Private/Save/UnmadeSaveSession.cpp::Read` validates schema/resume only, but `SelectProfile` and `DescribeProfiles` call it as an availability check. Inventory/world validators run later in separate owners. Compose validation before profile switching and backup selection; inject malformed optional subrecords, preserve original bytes, and verify no mixed state or rewards. Do not silently migrate or erase identifiers. |
| P1 feedback gap | `Private/Player/UnmadeCharacter.cpp::UseHealthPotion` / `UseStrainPotion` report no useful item when the bool adapter also represents failed persistence. Add explicit failure results so storage failure is distinguishable from no benefit; avoid trying subsequent potions after storage failure. |
| P1 integrated first-playable evidence | `Public/World/UnmadeCampaignSpineRules.h`, `UnmadeRealmJourneyRules.h`, `Private/World/UnmadeCampaignWorld.cpp`, `Tests/integration/offline_slice_scenario.cpp`, `Tests/world/realm_journey_test.cpp`: portable branches/rewards/returns exist. Add a composed fault-injected save boundary across owners; then walk opening, investigation, combat, both civic choices, return feedback, recovery and reload in UE. Source/data checks do not prove reachable actors. |
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

Resume with the P1 composed save-validation batch above. Run
`python Scripts/audit_gameplay_dependencies.py --check --report audit.json`,
`python -m unittest discover -s Scripts/tests -v`, and compile/run
`Tests/save/recovery_commit_test.cpp` with the public include directory. Keep the
optional illustration path available; update artifact receipts only for actual publications.
