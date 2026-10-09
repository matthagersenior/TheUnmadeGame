# THE UNMADE — Combat verification gate

**Status:** Source implementation, not a gameplay-tested Unreal build.

## Controls / arena
- Left mouse button / controller right trigger: attempt melee strike. Must face a hostile target within 240 cm, with unobstructed line of sight.
- Right mouse button / controller left trigger: hold guard. Guard reduces received damage to 25% and prevents attacks until released.
- The hub spawns a Stalker and a Watcher near the far edges of its ground. Stalker advances and attacks up close; Watcher retreats if cornered and applies a sight-gated ranged placeholder hit. It has no projectile mesh, VFX, pathfinding, or authored combat animations.
- Fold near the fracture exposes nearby hostile actors for four seconds, increasing player damage by 50%. This is visible via higher numeric hit feedback, not finished VFX.

## Engine host test matrix (currently unexecuted)
1. Compile Unreal Editor C++ and run `Unmade.Combat.OfflineRules`. No engine compilation has occurred yet.
2. Open PIE; verify two hostiles and five existing civilians without duplicate spawns.
3. Hold guard and press attack: no hit. Release guard and strike in melee range: hit once, cooldown blocks rapid repeats.
4. Strike from beyond 240 cm, while turned away, or through a solid wall: no hit or damage.
5. With guard on, incoming damage should be 25% of normal. Enemy melee and ranged placeholder should differ in engagement distance and movement.
6. Fold near an enemy, then hit: compare damage to an unexposed enemy. The increased damage should expire after four seconds.
7. Defeat both enemies. Their actor collision should turn off; they should no longer damage the player. If the player dies, prototype movement stops with an informative message.
8. Verify NPC memories and Rewrite states survive combat activity and save/load.
9. Confirm controls on an actual controller and record frame rate; later replace graybox geometry and implement checkpoint, animations, spatial audio, health HUD, navmesh AI and ranged projectiles.

GitHub CI tests the pure native combat domain using g++ and Python source-contract wiring. **Passing CI does not prove Unreal Engine loads or plays this code.**
