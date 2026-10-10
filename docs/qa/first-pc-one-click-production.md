# THE UNMADE — One-click first-PC Unreal production setup

**10 October 2026 · scripts implemented and statically tested; actual UE 5.8 Windows execution requires your PC.**

This removes repetitive setup work once you have a Windows machine. The project is still a development graybox, **not a finished commercial game**, and the 142 generated markers are deliberately noncolliding authoring references.

## One simple workflow

1. Install Epic Games Launcher and **Unreal Engine 5.8**, plus **Visual Studio 2022 or a compatible supported version with C++ Game Development / MSVC x64 and Windows SDK**. Hardware requirements vary. The launcher never buys, installs, or provisions software.
2. Download and extract [the repository as a ZIP](https://github.com/matthagersenior/TheUnmadeGame/archive/refs/heads/main.zip) (or `git clone https://github.com/matthagersenior/TheUnmadeGame.git` followed by `git lfs pull` when real assets are present). Keep the folder outside Program Files or other protected directories.
3. **Double-click `START_THE_UNMADE.cmd` at the repo root.** No typing into Unreal, no separate Python installation if Unreal's bundled Python executable is present. The launcher discovers the usual UE 5.8 install, otherwise it tells you precisely what is missing.
4. It validates the seven source-generated CSVs and two map reference plans **before touching the Editor**; runs the existing native source build, strict Unreal Automation tests and preflight; then uses **Unreal's Python Editor commandlet** to create or update seven **reference-only** DataTables under `/Game/UnmadeProduction/Data`, and two **isolated authoring levels**:
   - `/Game/UnmadeProduction/Maps/Unmade_AuthoringStaging` — 52 noncolliding quest/cinematic guide blocks (36 realm references, 16 shot guides).
   - `/Game/UnmadeProduction/Maps/Unmade_LivedWorldStaging` — 90 noncolliding lived-world work orders (27 evidence, 18 resident scenes, nine side story leads and 36 art/audio tickets).
5. It writes a dated `TestReports/quickstart-YYYYMMDD-HHMMSS/` run folder with dry-run checks, build/test logs, import logs and a receipt listing all seven imported tables, including 198 total CSV rows. **If something fails, it stops, reports the stage, and will not report success.** After a successful run, it opens the real Unreal Editor.

This creates only *authoring reference data*, not the final cinematic, voice performances, real character animations, playable nine-realm architecture, polished dialogue UMG, complete collision or a packaged Windows executable.

## Optional modes without losing work

| Example | Effect |
| --- | --- |
| `START_THE_UNMADE.cmd` | Full local preflight + compile + tests + isolated staging + Editor |
| `START_THE_UNMADE.cmd -InspectOnly` | Source-plan validation only; no asset changes |
| `START_THE_UNMADE.cmd -SkipStage` | Compile, test and open Editor; do not import staging assets |
| `START_THE_UNMADE.cmd -DontOpenEditor` | Compile, test and stage, but do not open the GUI |
| `START_THE_UNMADE.cmd -UnrealRoot "D:\UE_5.8"` | Use a nonstandard installed Engine location |

If the PC has no valid Python interpreter, no UE 5.8, insufficient disk/GPU prerequisites or no Visual Studio C++ toolset, the preflight explains the blocker. Some versions of Unreal ship bundled Python inside `Engine/Binaries/ThirdParty/Python3/Win64/python.exe`; if your distribution omits it, install Python 3.11+ separately. The Editor Python plugin is explicitly enabled in `TheUnmadeGame.uproject`.

### Important safety and repeatability

- Reference asset destinations are fixed: `/Game/UnmadeProduction/Data` and *dedicated* `/Game/UnmadeProduction/Maps/*Staging`. The Editor script **refuses** to overwrite an unrelated asset or a DataTable with a different row struct.
- Existing expected labels are skipped, not blindly duplicated; repeated runs update authored content and retain marker identities. Never hand-rename prefixed authoring markers.
- This workflow **never changes gameplay's `UnmadePrototypeNPC` save slot**, player endings, authoritative quest physics, original asset master JSON, source quest progress or the real Editor default map.
- It will create .uasset/.umap authoring files on the local Windows clone, **not** push those binaries to `main`. Review and commit them using Git LFS after visual QA.
- The Editor importer fails if no valid completion receipt can be written. It does not assert that imported DataTables are consumed at runtime: the source quest code remains authoritative.
- If an Unreal 5.8 API or plugin differs and the Python commandlet fails, consult `editor-import.log`; this is a **first-PC integration issue**, not permission to bypass or manufacture a success receipt.
- The version-controlled script does not auto-regenerate the separate illustrated Bible. **Canon changes still require new Volume X/next edition PDF, DOCX, ZIP and publication receipt under `docs/lore/UPDATE_POLICY.md`.** No new lore fact is asserted in this tool-only build.

## Smallest useful proof on first PC

After successful launch, open `Unmade_AuthoringStaging`: it should show 52 guides, then `Unmade_LivedWorldStaging`: 90 guides. Open the seven DataTables and inspect exact row counts **18 + 9 + 10 + 16 + 9 + 54 + 82 = 198**. These are reference blueprints for professional scene building, not production actors.

Next open PIE in the normal starter map: walk with a third-person placeholder, inspect Hessa's Witness Echo near Bellwold, talk to a resident, test safe story choice, save/reload, then test a second visit. Write down whether the real runtime passes each test; engine compile alone cannot certify it.

## Before calling it 'ready to ship'

Still outstanding: first UE 5.8 engine compilation and validation, authentic terrain/buildings and occupied-world art, real UMG conversations/codex, distinct performances/animation/VO, integrated cues/lighting, safe cross-realm traversal, combat feel, boss polish, accessibility, and full load/save fault injection. The scripts reduce reentry, not creative or testing requirements.

## 11-city assembly extension — third safe map

The first-PC importer now also produces `/Game/UnmadeProduction/Maps/Unmade_CityAssemblyStaging`, a separate 402-reference-actor **city massing** level. This is not a successor to the real gameplay graybox; it is a city-by-city visual plan that connects all existing quest/cast/rite/evidence identities to recognizable city silhouettes. Use `python Scripts/build_city_assembly.py --check` without Unreal to inspect coverage, or `--output city-preview.json` to export the whole actor plan. The quickstart's receipt requires 402 third-map markers as well as 52 and 90 from the prior two maps. None has collision, navmesh or completed interactivity. Reference: [all-realm assembly contract](../design/nine-realm-city-assembly-factory.md).
