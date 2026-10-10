# THE UNMADE — Windows Unreal build host gate

**Status:** Written procedure only. No Unreal-capable host is provisioned or tested.

## No-cost checks on a prospective Windows GPU host

Use a Windows host with Unreal Engine 5.8, Visual Studio C++ build tools, a capable GPU and sufficient disk space. Clone the public repository and open PowerShell:

```powershell
git clone https://github.com/matthagersenior/TheUnmadeGame.git
cd TheUnmadeGame
git lfs install
git lfs pull
.\Scripts\check_unreal_host.ps1 -UnrealRoot "C:\Program Files\Epic Games\UE_5.8"
```

This check does not install, purchase, or provision any software. If it exits 3, correct missing prerequisites. A passed preflight **does not** prove the source compiles.

## Actual Unreal verification (not yet executed)

1. Generate Visual Studio project files for the `.uproject` using the installed Unreal version. Compile `TheUnmadeGameEditor` (Development Editor / Win64). Record compiler errors and commit hash; if compilation fails, keep this gate failed.
2. Open Unreal Editor and run PIE. Verify a visible third-person placeholder pawn, game camera, collisions on the temporary walkable ground, five NPCs and nearby blockout structures.
3. Run engine automation, examine process exit and exported test JSON:

```powershell
.\Scripts\run_ue_tests.ps1 -UnrealRoot "C:\Program Files\Epic Games\UE_5.8" -TestFilter "Unmade"
```

4. Check E interaction near and far from NPCs; H helping once versus repeatedly; F anomaly signal near the marker versus far away; line-of-sight restrictions; eight-second proximity-gated, one-hop rumors.
5. Exit and relaunch the game, verify identity-bound fear/trust memories persist without duplicates and a remembered rumor retains its immediate speaker. Validate snapshot rejection leaves prior state untouched.
6. Only after the local package compiles, runs and passes tests, evaluate a time-limited authenticated Pixel Streaming session from Android. A remote preview does not substitute for native Windows QA.

**Required evidence:** exact game SHA, Unreal version/build, Windows/toolchain/GPU model, compiler logs, test-run logs/report, functional QA video/screenshots and save/reload evidence.

**Cost gate:** No GPU cloud infrastructure may be provisioned until the creator approves a provider and cost cap.

## Single-command first-PC gate (added October 10)
On a Windows PC with the correct licensed Unreal Engine and Visual Studio C++ workload already installed, use:

```powershell
.\Scripts\first_pc_build_and_test.ps1 -UnrealRoot "C:\Program Files\Epic Games\UE_5.8"
```

This script runs hardware/toolchain preflight, **Build.bat** for the Editor target and the `Unmade` automation group. It stops on nonzero exit and writes timestamped build and test logs under `TestReports/first-pc-*`. Use `-BuildOnly` for compiler debugging without pretending tests ran. It **does not** install UE, generate authored maps, cook a build, provision cloud resources, prove in-game save/load or certify real-time frame rates. Retain logs and follow the manual QA chapters for combat, frontiers, Afterlight, NPCs and all nine-realm progress.
