# Reality fractures — first authorable prototype QA

**Status:** Source implemented; engine compilation and interactive testing have not been performed.

The prototype uses engine primitive meshes. It is not visual-quality footage or a finished quest.

## PC controls near the prototype anomaly
- **F** (Glimpse): +8 Strain, three-second alternate clue reveal; does not commit a world variant.
- **Q** (Fold): +24 Strain, barrier temporarily loses visibility and collision for six seconds.
- **R** then **R** within six seconds: preview, then commit permanently open variant (+60 Strain).
- **T** then **T** within six seconds: preview, then commit permanently sealed variant (+60 Strain).
- Pressing R or T only once is a warning, not commitment. Another variant cannot replace an existing committed choice.
- Strain recovers three units per second; no action can raise Strain above 100.

## Engine QA to execute before claiming gameplay works
1. On an Unreal-equipped Windows host, compile TheUnmadeGameEditor (Development Editor) and run `Unmade.Fracture.CoreRules`; inspect failures.
2. Start the prototype. Confirm the barrier, clue and five residents are present, character is visible and walkable floor has collision.
3. Press F far from target: no effect or Strain deduction. Move close; F briefly reveals clue and increases Strain by eight.
4. Press Q near target: collide before, walk through during the six-second Fold, collide after. Second press during an active Fold must not spend Strain again.
5. Enter R only once, wait beyond six seconds, press again: must only preview, not commit. Repeat within six seconds and ensure permanent change and clear feedback.
6. Reload save after committing R: barrier stays open. With a fresh save repeat for T: barrier stays sealed. Neither outcome erases NPC observations.
7. Force a full Strain meter and ensure Fold/Rewrite refuse without mutation. Corrupt a copied save and ensure it does not silently reset prior data.
8. Verify a witness reacts differently from someone who cannot see the player; local rumors remain labeled as hearsay.
9. Check gamepad separately: Left Shoulder=Glimpse, Right Shoulder=Fold, D-pad Left=Rewrite open, D-pad Right=Rewrite sealed. These mappings require engine/controller verification.
10. Profile frame rate and memory on both a native GPU Windows host and (later) an Android Pixel Streaming client.

## Current automation evidence
GitHub static checks compile and execute `Tests/fracture/fracture_core_test.cpp` using g++ without Unreal installed. These tests prove only the deterministic cross-platform rule implementation. They do not verify Unreal APIs, collision or screen visuals. A Windows/GPU environment remains required for the engine gate.
