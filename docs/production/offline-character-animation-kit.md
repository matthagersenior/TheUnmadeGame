# The Unmade — one-click prototype character rigs and animations

The first-PC launcher now produces additional tangible 3D assets in
Build/UnmadePrototypeRigs from Scripts/build_skeletal_performance_kit.py.

Outputs, all original source-built glTF 2.0:
- SK_UnmadeResident.gltf — skinned humanoid reference model
- SK_UnmadeBeast.gltf — skinned quadruped reference model
- SK_UnmadeWraith.gltf — skinned supernatural reference model
- UnmadeSixRegionPBR.gltf — material swatches with six PBR definitions

Each rig includes **19 named animation clips**, giving 57 authored prototype
motion sequences across the three archetypes: idle, walk, attack, gesture,
speak, guard, stagger, death, cast, inspect, repair, release, ranged, crush,
sweep, seal, yield, dodge and parry.

The files are actual skeletal geometry/animation data generated outside Unreal,
not final humanoid rigs or fully dressed character performances. Their shapes
are generic staging archetypes rather than official NPC/boss appearance canon.

You can run this independently:

    python Scripts/build_skeletal_performance_kit.py --out Build/UnmadePrototypeRigs
    python -m unittest discover -s Scripts/tests -p test_skeletal_performance_kit.py -v

When Unreal is available, import these into a separate
/Game/UnmadeProduction/PrototypeRigs folder first. Verify Interchange skin and
skeleton import, clips, animation retargeting, root orientation, bone weighting,
actual playback, hit timing, material fidelity and collision before using
them in a playable level. Do not overwrite final artist rigs or bind enemy
attack damage to visual keyframes until tested.

This pipeline does not change any NPC's identity, player appearance, SaveGame,
quest rewards or canonical lore. It will not resolve OPEN, DISPUTED or SEALED
mysteries, and does not make a packaged game.
