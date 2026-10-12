# THE UNMADE

A dark, surreal third-person RPG where you reshape reality, investigate conflicting memories, and return to communities that remember your choices.

Explore nine authored realms through combat, investigation, rescue and reality-changing abilities. Named residents, finite local economies and saved consequences connect adventures to the people living there.

## Current status

Current source milestone: **Player Facing Reliability v20, with the v21 cross-realm parity gate**. The repository contains Unreal C++ gameplay, generated graybox world assembly, portable rules, authored narrative data and prototype asset generators. No packaged playable release is available.

| Evidence | Scope |
| --- | --- |
| Verified in CI | Python validators and tests, portable C++17 gameplay rules, authored-data/output consistency |
| Source-only | Unreal gameplay integration, graybox settlements/dungeons, native menus, animation/audio import plans |
| Untested in engine | UHT/Editor compilation, imported assets, movement/navigation/collision, rendered UI, campaign playtests and packaging |

Detailed checks: [gameplay parity and completion standard](docs/qa/gameplay-parity-v21.md), [PC-independent readiness](docs/qa/pre-pc-readiness.md), [Windows host setup](docs/qa/windows-unreal-host.md), [Unreal CI](docs/qa/unreal-ci.md).

## Quick start

### Source checks (no Unreal installation)

Requires Python 3.11+, Git and Git LFS; native tests additionally require a C++17 compiler.

```bash
git clone https://github.com/matthagersenior/TheUnmadeGame.git
cd TheUnmadeGame
git lfs install
git lfs pull
python Scripts/validate_repo.py
python -m unittest discover -s Scripts/tests -v
python Scripts/build_bible.py --check
```

### Windows Unreal development

Install the project’s provisional Unreal **5.8** target and compatible Visual Studio C++ tools. From the repository root:

```powershell
.\Scripts\first_pc_build_and_test.ps1 -UnrealRoot "C:\Program Files\Epic Games\UE_5.8"
```

This builds the Editor target and runs Unreal automation with retained reports. Then open `TheUnmadeGame.uproject` in the matching Editor and follow the engine acceptance checklist. `START_THE_UNMADE.cmd` provides the interactive first-PC preparation flow. The default Entry map uses runtime graybox assembly; authored production maps are still required.

## Repository guide

- `Source/`: Unreal integration and portable gameplay rules.
- `Authoring/`: narrative registries, world data and generated import references.
- `Scripts/` and `Tests/`: generators, validation and engine handoff.
- [Canonical manuscript manifest](docs/lore/bible_sources.json) and [publication workflow](docs/publishing.md).
- [Development history](docs/CHANGELOG.md) and [latest dated illustrated edition](docs/lore/LATEST_ILLUSTRATED_RELEASE.md).
- [Binary asset admission](docs/qa/git-lfs.md) and [contribution/release workflow](CONTRIBUTING.md).

Gameplay is deterministic and offline by default. Optional local dialogue uses a disabled-by-default adapter with authored fallback; see [local dialogue architecture](docs/architecture/local-npc-llm.md).

## Rights

Copyright © 2026 Matthew Hager. All rights reserved. See [LICENSE](LICENSE). Unreal Engine and third-party dependencies retain their own license terms.


## Complete RPG Loops v19 12 October 2026

Source implemented: deed-gated free mastery across nine realms; kits restricted to danger benefit; four independent non-destructive profiles with prior-record backup and safe-settlement health/stamina resume; optional fieldwork profile migration; 27 controller-accessible actual actions on seven menu pages; selected six workshop and nine earned field recipes, five trade goods; mapped prompts and signs; deferred distant primitive blocks, cached actors and distance cadence independent of the clock; distinct boss signatures/warnings and timed service gestures. Existing IDs, supplies, consent, wages, endings and OPEN mysteries remain authoritative.

Second audit fixed isolated fieldwork saves, stale profile confirmation and automatic crafting. Repeat with `python Scripts/audit_gameplay_dependencies.py --check --report audit.json`. Unreal compilation, imports, storage interruption, focus, navigation, timing and complete walkthroughs remain deferred. See Chapter 39 and `docs/design/complete-rpg-loops-v19-acceptance.md`.


## Player Facing Reliability v20 12 October 2026

Six follow-up audit fixes: scoped, visibly explained backup recovery without overwriting its target; single-pass binding labels; unobstructed alive/consenting merchant checks in both trade directions; native chosen-name and eight-aspect Character page; held stagger/death priority; production-rule behavioral audit coverage. Added actual Toggle Sneak access, eight menu pages and 28 complete action callbacks. A second inspection fixed feedback hidden behind the open menu. Existing saved IDs, wages, consent, protagonist origin, rewards and OPEN mysteries remain authoritative. Identity art remains prototype; no final face, hair, voice or mark assets claimed.

Repeat `python Scripts/audit_gameplay_dependencies.py --check --report audit.json`. Engine compilation, imports, storage interruption, controller focus/platform keyboard, collision, clip/audio timing and complete walkthroughs remain deferred. Chapter 40 and `docs/design/player-facing-reliability-v20-acceptance.md` record the source scope.


## Cross-realm gameplay parity v21 — 12 October 2026

Added a shared, CI-enforced authoring floor for all nine realms and a completion standard for menus, cinematics, exploration, abilities, enemies/bosses, economies, relationships, saves, accessibility and performance. The parity test checks realm ordering and identity, main/optional branches and return content, evidence, sensory cues and safe-access notes. It verifies source data only: Unreal compilation, authored/imported assets, real UI/controller behavior, full campaign play and packaging remain unverified. See [the v21 parity standard](docs/qa/gameplay-parity-v21.md).
