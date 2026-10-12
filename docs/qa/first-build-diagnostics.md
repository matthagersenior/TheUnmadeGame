# First Unreal build: exact target and failure evidence

**Current project target:** Unreal Engine **5.8**, Windows x64, Development Editor.

The normal build script invokes Unreal's own Build.bat with the following target:

- Target: TheUnmadeGameEditor
- Platform/configuration: Win64 Development
- Project: TheUnmadeGame.uproject
- Source module: TheUnmadeGame
- Project plugins enabled: EnhancedInput and PythonScriptPlugin
- Module dependencies: Core, CoreUObject, Engine, InputCore, EnhancedInput, HTTP, Json, JsonUtilities, UMG, Slate, SlateCore, NavigationSystem

## Install checklist

Use Epic Games Launcher to install Unreal Engine 5.8. In Visual Studio Installer, install a UE 5.8-supported Visual Studio version with:

- Game development with C++ workload
- MSVC x64/x86 C++ build tools
- Windows 10 or Windows 11 SDK

Keep the checkout in a writable local folder, such as C:\Unmade\TheUnmadeGame. Avoid Program Files, synchronized folders, and paths with unusual characters. The current preflight asks for at least 100 GB free space.

The preflight now prints Windows edition/build, installed Unreal version and changelist from Engine/Build/Build.version, Visual Studio version/path, MSVC x64 compiler path, Windows SDK header versions, GPU adapter names, and available space. It stops early when the project/engine versions disagree or a required build executable, compiler, or SDK is missing.

## Start with compile only

From the repository folder, run:

```powershell
.\START_THE_UNMADE.cmd -BuildOnly
```

This runs the source registry checks and compiles TheUnmadeGameEditor. It does **not** run Unreal automation, import reference assets, or open the Editor.

The equivalent direct command is:

```powershell
.\Scripts\first_pc_build_and_test.ps1 -UnrealRoot "C:\Program Files\Epic Games\UE_5.8" -BuildOnly
```

For source checks, build and automation without importing reference assets or opening the Editor, run:

```powershell
.\START_THE_UNMADE.cmd -SkipStage -DontOpenEditor
```

The full double-click flow adds isolated staging imports and opens the Editor after its checks pass.

## Preserve the first failure

The launcher creates a timestamped report at TestReports/quickstart-<UTC>. The build step creates its own TestReports/first-pc-<UTC> folder. Keep both. Start with:

1. failure.txt
2. preflight.log
3. editor-build.log
4. automation.log and exported report JSON, if automation ran
5. Saved/Logs/TheUnmadeGame.log, if the Editor or commandlet started

Record the exact source commit (or ZIP date), Unreal version/changelist, Visual Studio version and Windows build. Send the first actual UBT/UHT/MSVC error with the logs; later compiler messages often cascade from it. Do not delete Intermediate or Saved before preserving the first failure.

## Identify the failing layer

| Evidence | Likely layer | What to do |
| --- | --- | --- |
| NOT_READY in preflight.log | Host prerequisite | Install the named Visual Studio component or point the script at the correct Engine root, then rerun preflight. |
| *_OUT_OF_SYNC or source registry failure before editor-build.log | Source data/check | Keep the named validation log and commit; fix the registry/generator mismatch first. |
| UnrealHeaderTool failed or reflected type/property errors | Unreal C++ declarations | Keep the complete UBT log and fix the first UHT error. |
| MSVC error C#### | C++ compilation | Record the first file and line plus exact Engine and compiler versions. |
| LNK#### | Link or module dependency | Check the missing symbol against the module dependency list above before adding dependencies. |
| Build succeeds, Editor/automation fails | Runtime or automation | Keep automation.log, report JSON, and the relevant Unreal log. Separate a test failure from a process crash. |

The GitHub Unreal workflow remains unavailable until a trusted Windows Unreal runner exists. Its skipped status is not compilation evidence. Once you have a PC, the two report folders provide enough detail to fix the actual first failure together.
