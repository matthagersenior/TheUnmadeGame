# Combat presentation and world identity v15

Source implementation and portable production assets across all nine existing realms. Unreal import, compilation and playtesting remain deferred. Original campaign endings, nine OPEN mysteries, ten powers, inventory IDs, wages and consent remain authoritative.

## Asset production

The deterministic skeletal builder produces three original low-poly skinned glTF rigs with 17 keyed clips each (51 total), preserving the original five clip names. Added Guard, Stagger, Death, Cast, Inspect, Repair, Release, Ranged, Crush, Sweep, Seal and Yield. These are prototype motions, not final motion capture, retargeting, root motion, lip sync or animation blueprints. Stable optional asset paths preserve graybox fallback. The builder is executable without paid providers or Blender. The game-dev CLI is unavailable in this environment; existing project builders supply the local production workflow.

The identity audio builder produces 36 original mono PCM WAVs: 18 general combat, interaction and material-footstep cues; nine realm-specific boss signature sounds; nine eight-second ambience loops. SHA-256, PCM format, signal levels and quiet endpoints are checked. This is procedural prototype Foley/ambience, not recorded voice acting or a final music/mix production. Combat strikes, guard entry, player/enemy injuries, ability successes, persisted interactions, rewards, warnings and boss signature events attempt stable imported sound paths. Stone footsteps are rate-limited; wood and salt footsteps are prepared sources for later surface routing. Nine spatial ambience components are created only when their sounds are imported. Gameplay authority never depends on a missing sound or clip.

Build: `python Scripts/build_identity_audio.py --out assets/audio` and `python Scripts/build_skeletal_performance_kit.py --out assets/rigs`. Editor audio import: set `UNMADE_IDENTITY_AUDIO` to the absolute WAV folder, then execute `Scripts/unreal_editor/import_identity_audio.py` in Unreal Editor Python. The existing skeletal import acceptance remains manual; automatic retargeting is not claimed.

## Boss and dungeon identity

Every third expedition boss attack is now a realm-specific locked-aim signature, with a 2.2-second first-phase warning, 1.5-second second-phase warning, 3.2-second recovery and 420 cm footprint radius. Cover, leaving the footprint, guard and the free grounding lever remain counterplay. World X/Y coordinates are used explicitly. Nine signatures: inner-safe ring; narrow north/south lane; north-facing semicircle; cross with diagonal safe spaces; opposite-sign quadrant safety; safe central vertical strip; safe middle horizontal band; square; thin outer rim. Graybox sampled floor tiles and text preview these footprints; exact tile boundary readability and collision must be reviewed in the Editor. Tiles are cues, not damaging actors. A living surrendered boss remains visible, inactive and untargetable.

Each dungeon has a different named three-valve mechanism with distinct coupled light changes and target. The target and each valve's affected light mask are displayed. Choices are reversible until solved, with no Strain cost or irreversible failure. Every target is exhaustively proven reachable. This is nine variants of a shared valve puzzle system, not nine entirely different puzzle genres or rebuilt dungeon layouts. Existing six-room layouts remain; realm-specific landmark silhouettes and colors are added outside chamber edges.

## Local quests and rewards

Each realm's service marker offers a guard hunt (four actual recorded kills; one 35-health recovery while injured), an independent F survey (three actual records plus solved mechanism; permanent local hazard suppression), and a road restoration quest (three physical repairs after witnessed return; opens 35-health sanctuary recovery once per game day). These add 27 optional quest objectives with three reward categories. Supply recovery is immediate health, not an invented inventory item. Full health never consumes a ration or a rest day. No currency, new spell unlock or commerce stock is added.

Old saves omit the new field and receive default state. A versioned 46-integer optional snapshot validates values and dependencies against existing expedition state and current game day, rejecting corrupt data atomically and protecting the original world save. Writes happen before healing, feedback or physical updates. Failed writes restore the whole identity model. Finite hunt and survey rewards cannot be claimed twice; sanctuary refresh is day-bound.

## Remembered return

Dead bosses leave lower damaged wayposts; surrender leaves the less damaged baseline. Repairs visibly straighten and raise each post. After all three repairs and witnessed return, the existing refuge resident numbered 3 relocates to road-watch duty with a new home anchor. No generic extra NPC IDs are added. Captives already released by actual guard/boss resolution appear home after witnessed return, and their conversation remembers the opened cell. Local residents acknowledge repairs and the dead/yielded outcome. These are saved relocations and bounded routines, not a cinematic walking escort. Existing removed enemy patrols and opened return gates persist.

## Required engine acceptance

Import all optional rigs, 51 clips and 36 WAVs; inspect scale, bone transforms, clip looping, endpoint poses, pop-free ambience, attenuation and volume. Exercise all nine signature footprints and safe areas with cover, interrupt, death, revive and reload. Check floor tile placement and warning readability at normal camera distance. Exercise all valve combinations, independent survey without hunt, full-health ration/rest refusal, duplicate reward attempts, next-day rest, write failure and corrupt optional data. Inspect service and repair LOS, navigation to relocated residents, frame time with nine boss telegraph grids, and retained old gameplay. Static source checks and portable C++ tests do not prove any of these engine behaviors.
