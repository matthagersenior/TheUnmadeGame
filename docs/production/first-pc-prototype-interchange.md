# The Unmade — first-PC guarded static-mesh import and staging

**Scope:** 193 original source-generated static glTF models, a dedicated
non-authoritative 88-actor city preview map, and a dated editor receipt.
This is **not a playable level**, final art, or certification of Unreal import.

## What happens on the first Unreal-capable Windows PC

Double-click `START_THE_UNMADE.cmd` as before.

The quickstart validates the nine-realm canon and builds source assets using:

- `Scripts/build_realm_geometry.py`: 115 world models (88 architectural
  modules and 27 evidence props)
- `Scripts/build_equipment_geometry.py`: 78 equipment, item and reward models
- `Scripts/build_skeletal_performance_kit.py`: 3 skinned preview archetypes,
  19 keyed clips per rig, and one material study atlas

The skeletal glTF files are **not part of this static-mesh batch**. Actual
skeletal import and animation retargeting remain a separate acceptance gate.

After the existing UE Editor C++ build, automation tests and source-reference
staging succeed, quickstart runs the guarded prototype importer, using only
these script-owned folders:

- `/Game/UnmadeProduction/PrototypeMeshes/World/<unique source asset>`
- `/Game/UnmadeProduction/PrototypeMeshes/Equipment/<unique source asset>`
- `/Game/UnmadeProduction/Maps/Unmade_PrototypeGeometryStaging`

Each world/equipment glTF is independently imported to an isolated folder;
interchange-created sub-assets never share a destination with another source
model. The preview map positions the 88 *architectural* modules at their
canonical staging coordinates with collision disabled. The 27 clue models
**are imported but not placed**, because precise local site positions have
not been authored. The 78 items also are **not spawned or granted**.

## Verification and proof

The first phase always runs offline without Unreal and checks every one of
193 source file hashes, the exact 115 + 78 asset inventories, distinct source
IDs, destination isolation and 88 placement guides.

Dry run after file generation:

    python Scripts/unreal_editor/import_prototype_geometry.py

If source bytes, placements or counts do not match their manifests, the build
fails before any editor import. CI performs this dry run plus deterministic
faked-editor tests for import, idempotence and sandbox isolation.

The real Editor-only phase requires two environment values set transiently
by the Windows launcher:

- `UNMADE_PROTO_IMPORT_APPROVED=1`
- `UNMADE_PROTO_IMPORT_RECEIPT=<path inside TestReports>`

It must import all 193 or the launcher reports an incomplete/failure result.
A successful attempt records
`TestReports/quickstart-<timestamp>/prototype-import-receipt.json` with
created versus previously present meshes and staged actors. A separate
`prototype-editor-import.log` shows importer/Interchange failures.

**These receipts certify only the import and reference preview steps.**
They do not certify collision, playability, animation, quest interactions,
frame rate, narrative fidelity or packaging.

## Repeated runs and artist protection

- The script never imports to `/Game/Maps`, Entry, another gameplay world,
  or over a previously imported mesh.
- If a unique source folder already contains exactly one StaticMesh, the
  import is skipped without modifying that asset; it remains artist-controlled.
- If a source folder contains conflicting, absent or multiple StaticMesh
  objects, the importer refuses to proceed rather than guessing.
- Existing preview actors are not replaced, moved or reset. Artists can use
  the initial 88 guides to build authored maps without losing their edits.
- The preview is noncolliding, separate from the 402 original marker actors,
  and has no ability to satisfy evidence/quest/reward/save prerequisites.
- A failed or incomplete real import must be fixed using the recorded
  log before the launcher is considered green.

## Still required on real Unreal Engine 5.8

- Validate the project Interchange plugins and actual unattended glTF import
  behavior in a real installed Editor. If an API or pipeline option differs,
  use the log to adjust the adapter rather than reporting a fake green state.
- Inspect centimeters versus glTF meters, Y-up to Z-up orientation,
  materials, normals, LOD, target hardware frame rates and collisions.
- Make true roads, doorways, interiors and navigable routes; prototype city
  meshes are *not* an authored travel graph.
- Give physical clues real sites and witness/source authorization before
  making them gameplay interactions.
- Convert the generic resident/beast/wraith rigs and item preview objects
  into actual appearance-authored NPCs, bosses and player equipment only
  after import, sockets, animation and authoritative save rules work.

The relevant Unreal interfaces are the standard `AssetImportTask` /
`AssetToolsHelpers` import path, the Interchange glTF pipeline and
`LevelEditorSubsystem` level creation/loading. Their real UE 5.8 behavior
has **not been executed in GitHub CI**.

Canon remains in `docs/canon/dependency-ledger.json` and `Authoring/`.
The `SOURCE_MESH_ENGINE_UNVERIFIED` contract is not promoted to
ENGINE_VERIFIED until actual Editor logs and asset/scene inspection exist.
