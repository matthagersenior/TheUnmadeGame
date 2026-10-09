# THE UNMADE

Dark, surreal, third-person 3D RPG. You play a customizable outcast whose origin lies in an impossible version of reality. Living civilizations respond differently to fractures, and NPCs remember what they have personally witnessed or credibly heard.

**Status:** foundational Unreal Engine **source scaffold**, not a playable release. No Unreal Editor compilation, map boot, packaged Windows build, or Android streaming session has been verified yet.

## Approved direction
- Third-person 3D; Windows PC first; controller + keyboard/mouse planned.
- Adaptive reality combat: Glimpse, Fold and warned persistent Rewrite.
- Every identifiable NPC has individuality and salient persistent memory; major NPCs have richer beliefs, relationships and motives.
- Names mix culturally consistent invented words with unsettling everyday language. Working names are **not canon**.
- Languages are an optional discovery mechanic.
- Creator has an Android phone and no PC; interactive remote tests require an approved GPU-equipped host. **Do not provision paid infrastructure without approval.**

See [approved design](docs/specs/2026-10-08-the-unmade-foundations-design-v0.3.md) and [implementation plan](docs/plans/2026-10-08-the-unmade-first-playable-slice.md).

## Development requirements
Unreal Engine 5.8 (provisional target), Windows C++ toolchain compatible with that engine, Python 3.11+ for engine-independent checks, and Git LFS for binary assets. Follow Epic's Unreal Engine license. Use original or properly licensed game content.

## Check the source scaffold (no Unreal installation needed)
```bash
python Scripts/validate_repo.py
python -m unittest discover -s Scripts/tests -v
```

GitHub's **Static checks** workflow runs only these commands. A green check does **not** mean this game compiles, launches, or plays.

## Test with Unreal installed
```powershell
.\Scripts\run_ue_tests.ps1 -UnrealRoot 'C:\Program Files\Epic Games\UE_5.8' -TestFilter 'Unmade.Bootstrap'
```
This script requires UnrealEditor-Cmd.exe; refuses missing installations and rejects empty or failing automation reports. Actual execution awaits an Unreal-equipped host.

## Initial C++ foundation
The repository contains the primary module, an `AUnmadeCharacter` camera/movement skeleton, and `AUnmadeGameMode` with that pawn as default. DefaultEngine references Unreal's built-in **Entry** map purely to avoid pointing to a nonexistent custom map. It has **no authored environment, meshes, quests, NPCs, or gameplay abilities yet**. The plan's actual region maps and Enhanced Input mapping assets must be created and tested using Unreal Editor. Legacy Action/Axis mappings in `Config/DefaultInput.ini` are only a temporary bootstrap, not the long-term input architecture.

## Honest review gates
1. **Repository ready:** Python checks pass and files are visible on `main`.
2. **Engine bootstrap verified:** Unreal Editor compiles, automation tests run, character actually spawns and responds to keyboard/controller on an authored map.
3. **Gameplay slice verified:** combat, fracture, NPC memories, world changes, save/load all pass Unreal engine tests.
4. **Creator playtest:** on-demand authenticated GPU Pixel Streaming works from Android, separately checked against native Windows performance.

See [status and verification](docs/development-status.md). Paid cloud hosting is **not** active.

## Prototype hub source added (not engine-tested)

The game mode now requests a runtime-generated primitive hub instead of claiming to include a serialized map. It uses built-in cube meshes for the walkable ground, buildings and an anomaly marker, and temporary cylinder visuals for the player and five NPCs. **Engine startup and geometry collision still need to be verified on a Windows Unreal host.** These are testing placeholders, not approved world art or place names.

Once the Unreal host is running: move with WASD/stick, look with mouse/right stick, jump with Space/A, **E/X** to talk to a nearby citizen, **H** to offer aid nearby (at most once to each recipient), and **F** near the anomaly marker to emit a prototype-only event. Revisit the citizens to read their different trust/fear reactions. Memory snapshots are written to the Unreal save slot `UnmadePrototypeNPC` and read when the hub is constructed; full quest/character save/load has not been built. The F key is **not** a functional Glimpse/Fold/Rewrite power.

NPC perception uses distance and line-of-sight; rumor evidence is represented separately but a first local-gossip timer lets nearby NPCs relay personally witnessed events once, preserving the original event ID and the immediate speaker. Meaningful NPC jobs, dialogue trees and schedules are subsequent implementation tasks. For actual tests run `Unmade.Npc` and `Unmade.Bootstrap` via the Windows test runner, then manually verify NPC witness, refusal, reload and camera behavior.

## Optional offline NPC dialogue (source added)

The local NPC dialogue adapter can send brief, provenance-aware prompts to **Ollama on the same Windows game machine**. It is **disabled by default** and always has a deterministic offline fallback. No commercial AI API is used. See [local dialogue architecture](docs/architecture/local-npc-llm.md) for install/configuration, trust boundaries and the Unreal test gate. This feature is not a bundled AI model, has not been compiled in Unreal, and must not be described as a released playable capability.

## Fracture prototype (authored-blockout demonstration)

The hub has one temporary static-mesh fracture target. Near it press **F** for Glimpse
(brief alternate-history clue, 8 Strain), **Q** for Fold (temporarily remove collision,
24 Strain), or **R/T** for two mutually exclusive permanent Rewrite variants
(open or seal, 60 Strain). A Rewrite needs a repeat press of the same key within
six seconds and can only happen once; it persists in the existing prototype save
without erasing NPC memories. Strain recovers at 3 units/second. Failure to
save a Rewrite rolls back the model and visuals. **These are placeholders**:
there is no authored level, animation, VFX or HUD. This source is NOT Unreal
compiled or gameplay-verified yet.

The shared deterministic fracture engine is tested on GitHub by compiling
`Tests/fracture/fracture_core_test.cpp` with g++. The Unreal editor must still
compile and run the scene and `Unmade.Fracture.CoreRules` automation test.
