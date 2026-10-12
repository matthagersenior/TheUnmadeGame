# THE UNMADE — Engineering status

**Snapshot:** 2026-10-12, `main` at `6c9716abfaa37e7ac4321ee23f533d9ef2ea26b5` (Player Facing Reliability v20). Repository: [`matthagersenior/TheUnmadeGame`](https://github.com/matthagersenior/TheUnmadeGame).

## Current state

The repository contains a large Unreal C++ gameplay prototype, portable deterministic rules, authored quests and realm data, generated scene references, and source/contract tests. The source now covers nine-realm journeys and local consequences, optional abilities and combat, resident memory and schedules, finite economy and crafting, character identity, save profiles, recovery, controller-accessible menus, and authored animation/audio handoff data.

This is **not a packaged or engine-verified playable release**. Runtime world and actors remain a graybox prototype. Authored source data and staging guides do not mean that finished levels, art, animation, or cinematics have been imported into Unreal.

| Evidence | Current result |
| --- | --- |
| Main branch | v20 merged; worktree clean at the inspected commit |
| Open game PRs | None |
| GitHub Static checks | Passed on the current main commit |
| Portable gameplay dependency audit | Passed |
| Local source suite | 285 tests passed |
| Unreal Engine verification workflow | Skipped; no configured Windows Unreal host |
| Packaged game | Not available |

## What is implemented in source

- Nine-realm journey, hazard, guardian, dungeon, resident, return-consequence and optional-quest rules, with native C++ and Python source coverage.
- Offline-first NPC decisions, witness/rumor memory, authored dialogue, schedules, trade rules and saved identity.
- Combat and realm-specific abilities, gear, finite resources, crafting, trade, field recipes and recovery.
- Player identity controls, eight menu pages and 28 registered actions, profile save/recovery behavior, quest status and navigation prompts.
- First-PC preparation and Unreal staging scripts for reference DataTables, scene guides and city massing.

These are source implementation claims only. They do not establish that the Unreal project compiles or that the interactions work in PIE.

## Remaining verification and production gaps

1. Compile in the target Unreal 5.8 Windows environment; import the staging data and run the Unreal automation suite.
2. Exercise the actual controls, menus, controller focus, character entry, save/load and backup recovery in PIE. Verify that rejected or malformed saves remain unchanged.
3. Walk the nine realms and test collision, navigation, combat timing, boss counterplay, quest progression, return outcomes, frame time and memory.
4. Complete and integrate production levels, character/environment assets, animation, sound, UI, accessibility work and packaging. Current references and generated prototype performances are not final content.

The first-PC workflow is documented in [`docs/qa/first-pc-one-click-production.md`](qa/first-pc-one-click-production.md) and starts with `START_THE_UNMADE.cmd`. Static CI cannot replace this engine and human-playtest gate.

## Reproduce source verification

From the repository root with Python 3.11+ and a C++17 compiler:

```bash
python Scripts/validate_repo.py
python -m unittest discover -s Scripts/tests -v
python Scripts/audit_gameplay_dependencies.py --check --report /tmp/unmade-audit.json
```

For Unreal build and runtime verification, follow [`docs/qa/windows-unreal-host.md`](qa/windows-unreal-host.md) and [`docs/qa/unreal-ci.md`](qa/unreal-ci.md).