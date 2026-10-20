# The Unmade: Source-built geometry before the first Unreal PC

**Status:** Actual low-poly static-mesh files can be generated outside Unreal. Their
import, materials, collision, navigation, and gameplay remain UNREAL_NOT_VERIFIED.
Nothing here adds lore or changes SaveGame.

## What is produced

Command:

    python Scripts/build_realm_geometry.py --out-dir Build/UnmadePrototypeGeometry

Creates **115 independent original glTF 2.0 files** plus a JSON manifest:
88 geometry modules sourced from the eight individually authored silhouettes of
each of the 11 cities, and 27 physically identifiable clue prototype props
from the three documented firsthand investigations in each of nine realms.

The generator uses Python 3.11+ standard-library modules only. No Unreal,
Blender, paid asset store or network access is needed. Each asset contains
triangle geometry, normals, packed vertex/index buffers and distinct regional
PBR material colors. These are not final, textured production meshes.

Verification commands:

    python Scripts/build_realm_geometry.py --check
    python Scripts/build_realm_geometry.py --out-dir Build/UnmadePrototypeGeometry --verify-output
    python -m unittest discover -s Scripts/tests -p test_realm_geometry.py -v

The generated manifest includes source ID, source file, asset name,
triangle count, SHA-256 fingerprint, source-defined placement when available,
and an explicit SOURCE_MESH_ENGINE_UNVERIFIED status.

START_THE_UNMADE.cmd generates the files automatically into
Build/UnmadePrototypeGeometry after source-data checks and before Unreal
compilation. A failure is reported at
TestReports/quickstart-*/prototype-geometry-validation.log.

## Importing and validating in Unreal

1. Run the existing Windows quickstart and resolve any build/UHT errors.
2. Open Unreal Editor and create an isolated content folder:
   /Game/UnmadeProduction/PrototypeGeometry. Do not replace artist assets.
3. Import a few .gltf prototype files through Unreal Content Browser. Use its
   Interchange pipeline for static meshes, leaving skeleton/animation imports
   off for these particular files.
4. Check scale and axis. A glTF unit is one meter; manifest ue_position_cm
   coordinates use Unreal XYZ centimeters. glTF uses Y-up; confirm Unreal's
   orientation conversion before mass import.
5. Position city modules using the manifest in a separate non-gameplay
   staging map. Each model is independently addressable by its source stable_id.
   Placement coordinates are authoring hints; test doors, ramps and sightlines
   before adding collision.
6. The 27 physical clue assets intentionally have no fabricated coordinates.
   Place each in the actual authored site and bind its original game logic.
   A geometric clue does not by itself grant story progress.
7. Test real triangles, visuals, materials, collision, lighting, LOD,
   performance and accessibility. Record build SHA and an editor receipt.

Epic's UE 5.8 Interchange documentation lists glTF/GLB among the supported
Import Into Level formats. Source generation alone does not prove Editor import.

## Scope boundaries

- These are **individual structural prototypes**, not 11 completed cities.
  Authentic roads, interiors, foliage, weather, and civilian activity still
  require dedicated production.
- The generator never edits Source, Content, SaveGame, NPC memories, quests or
  gameplay authority. Imported UStaticMesh assets do not exist yet.
- The 402 existing noncolliding city guide markers remain independent from
  these new models. That separation makes art replacement reversible.
- Evidence must be physically observed and its source verified, rather than
  automatically granted by inspection of authoring records.
- Never disclose or resolve OPEN, DISPUTED or SEALED lore mysteries via
  prototype silhouettes or materials.
- Unreal compilation, animation retargeting, map traversal and packaged builds
  still require an actual Unreal-capable machine.

## Canon inputs and related production systems

- Authoring/city_assembly_kits_v1.json — city IDs, profiles and module roles
- Authoring/lived_universe_atlas.json — evidence site names and required verbs
- Authoring/main_quest_interaction_contracts.json — real quest authority
- docs/canon/dependency-ledger.json — earned systems and consequences
- Scripts/build_first_pc_production_pack.py — existing production dossier pack
- Scripts/build_skeletal_performance_kit.py — existing pre-Unreal rig and
  animation clip generator

All new module shapes and colors are replaceable, noncanonical prototype art.
