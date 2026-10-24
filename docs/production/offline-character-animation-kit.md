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
Skinned body parts use rounded ellipsoid surfaces with smooth normals instead
of box primitives. The resident also separates skin, linen clothing and
trousers into roughness-tuned PBR materials, while retaining the established
bone and animation names. Faces, hands, footwear, hair and tailored garments
still require the separately authored resident previews or final character art.

You can run this independently:

    python Scripts/build_skeletal_performance_kit.py --out Build/UnmadePrototypeRigs
    python -m unittest discover -s Scripts/tests -p test_skeletal_performance_kit.py -v

## Source-validated and first-PC Unreal import

The launcher now also runs this strict offline preflight:

    python Scripts/unreal_editor/import_skeletal_performance.py

After a successful native Editor build and automation run, it invokes the
same importer inside Unreal Editor with a transient import approval and a
dated receipt path.

Each imported asset must have the **exact names and types already used by
the actual C++ characters**, under:

- `/Game/UnmadeProduction/Characters/Resident/`
- `/Game/UnmadeProduction/Characters/Beast/`
- `/Game/UnmadeProduction/Characters/Wraith/`

For each folder the actual Editor must produce one `SkeletalMesh`, named
`SK_Unmade<Kind>`, and 19 `AnimSequence` assets named
`AN_Unmade<Kind>_<Motion>`, all targeting that mesh's `Skeleton`. A
successful receipt is stored under
`TestReports/quickstart-<timestamp>/skeletal-import-receipt.json`,
with separate numbers for imported versus already existing meshes and the
verified **57** clips. The accompanying `skeletal-editor-import.log` holds
raw Interchange output.

All three conditions are required:

1. All four source glTF files must exactly match the deterministic generator,
   with three skinned rigs and 19 authored animation sequences each.
2. Imported `SkeletalMesh` and `AnimSequence` assets must appear at those
   exact runtime paths with compatible `Skeleton` references; just importing
   glTF as an ordinary static mesh is a failure.
3. A repeat run must leave all existing complete rigs and animations unchanged.
   Conflicting, partly imported, artist-modified or wrong-class assets are
   not overwritten, renamed or deleted automatically.

If the UE 5.8 Interchange importer creates unexpected asset names, omits the
animations, or requires a different skeletal pipeline configuration, the
launcher reports a failure and preserves the import log. It must not claim
a successful playable rig. The six-region PBR atlas is validated as a source
study but **is not automatically imported**.

Actual Unreal skeleton import, retargeting, root orientation, bone weighting,
loop playback, contact timing, materials, face/voice synchronization and
collision **remain real-PC acceptance tests** even after a successful import
receipt. Do not overwrite final artist rigs or bind gameplay damage to
unverified visual animation keyframes.

This pipeline does not change any NPC's identity, player appearance, SaveGame,
quest rewards or canonical lore. It will not resolve OPEN, DISPUTED or SEALED
mysteries, and does not make a packaged game.
