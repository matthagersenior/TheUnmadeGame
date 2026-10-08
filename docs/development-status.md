# THE UNMADE — Engineering status

**Snapshot:** 2026-10-08. Source repository: `matthagersenior/TheUnmadeGame` (`main`).

## Ready in source control
- Approved game design v0.3 and first playable plan.
- Unreal C++ project descriptor, game/editor targets and runtime module.
- Bootstrap game mode, third-person character movement/camera source, and temporary input mappings.
- Source-level validator, unit tests and GitHub Actions workflow.
- Unreal bootstrap automation source and a fail-closed Windows test harness.

## Not yet verified or implemented
- Unreal Engine compilation and Editor opening (requires GPU-equipped Windows/Unreal host).
- Authored map, meshes, animation, visual character customization and Enhanced Input mapping assets.
- Fracture abilities, combat, NPC memory simulation, languages, quest/world state, save/load.
- Packaged Windows build, controller acceptance testing, or Android remote streaming.
- Cloud hardware and hosting budget; none has been provisioned.

**Do not label an engine-independent CI success as Unreal gameplay readiness.**

## Next independently verifiable gate
Open `TheUnmadeGame.uproject` using the selected Unreal version, compile the module, generate an actual third-person editor map with player start and collision, run `Unmade.Bootstrap` automation, and capture report metadata. Fix any compile or input issues found. Only then record Gate A as complete.

## Naming and game behavior
Working fantasy place and faction names are non-canonical. NPCs must remember salient personal experiences and never gain omniscient knowledge; adaptive behavior needs Unreal tests before claimed implemented.
