# THE UNMADE

A dark, surreal third-person RPG where you reshape reality, investigate conflicting memories, and return to communities that remember your choices.

Explore nine authored realms through combat, investigation, rescue and reality-changing abilities. Named residents, finite local economies and saved consequences connect adventures to the people living there.

## Current status

Current source milestone: **Nine Realm Journeys v18**. The repository contains Unreal C++ gameplay, generated graybox world assembly, portable rules, authored narrative data and prototype asset generators. No packaged playable release is available.

| Evidence | Scope |
| --- | --- |
| Verified in CI | Python validators and tests, portable C++17 gameplay rules, authored-data/output consistency |
| Source-only | Unreal gameplay integration, graybox settlements/dungeons, native menus, animation/audio import plans |
| Untested in engine | UHT/Editor compilation, imported assets, movement/navigation/collision, rendered UI, campaign playtests and packaging |

Detailed checks: [engine acceptance](docs/design/nine-realm-journeys-v18-acceptance.md), [Windows host setup](docs/qa/windows-unreal-host.md), [Unreal CI](docs/qa/unreal-ci.md).

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
