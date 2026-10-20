# Nine original realm crowns — source-backed visual production pass

**Status:** 9 independent glTF static mesh prototypes generated outside Unreal.
They preserve the established nine-realm order and are anchored to real
city-assembly registry origins. **Neither their Unreal import nor gameplay
rendering has yet been verified on a Windows PC.**

This is an intentionally isolated production handoff from the older
draft-based [nine-realm art proposal](https://github.com/matthagersenior/TheUnmadeGame/pull/110).
That proposal targets the outdated save-recovery branch, so copying its
entire history or retargeting its PR directly to `main` would risk
contaminating the current source-of-truth. Instead, these nine *distinct
landmark shapes* are source-built from the current main's existing geometry
framework; the other art/refinement proposals remain separately reviewable.

| Canonical realm | Reference crown | Unique visual silhouette |
|---|---|---|
| ThreefoldReach | The Three-Span Charter | Asymmetric survey pillars and elevated ring |
| WidowedRain | The Dry-Tide Gauge | Paired gauge poles, conflicting water levels |
| HearthBeneath | The Borrowed Hearth Vent | Twin nonidentical furnace throats |
| TidalLedger | The Foreclosed Oath Pier | Suspended debt slates and chained ring |
| SkyBelow | The Choir Counterweight | Inverted suspended singing weight |
| CinderSpine | The Twin Ancestral Seam | Opposing quarry strata and uneven seams |
| HundredUnlived | The Occupied Blank Bed | Blank raised bed behind incomplete name frame |
| OrchardOfKings | The Unclaimed Graft | Asymmetric public tree-root lattice |
| FirstAbsence | The Double Honest Threshold | Two offset unfinished door frames |

The names above are **visual working motifs**, not an added lore resolution.
They are matched to existing realms by stable source IDs rather than being
generated as arbitrary interchangeable towers. The generated glTF models
use three original PBR prototype materials each, with per-realm color
values from the already implemented nine-realm model palette.

## Offline work — available immediately

Generate and verify:

    python Scripts/build_realm_crowns.py --check
    python Scripts/build_realm_crowns.py --out-dir Build/UnmadePrototypeCrowns
    python Scripts/build_realm_crowns.py --out-dir Build/UnmadePrototypeCrowns --verify-output
    python Scripts/unreal_editor/import_realm_crowns.py

The nine generated files each contain a mesh with embedded geometry,
normals, indices, three PBR materials and no remote resources. The exact
SHA-256 digest, canonical realm, source city, motif, triangle count,
provisional preview transform and **no-collision / no-quest-authority**
declarations are preserved in `manifest.json`.

Run the full existing Python suite for deterministic source and simulated
Unreal import tests. The crown importer rejects missing, inconsistent,
duplicate, altered or unexpectedly authoritative assets before import.

## First-PC behavior

`START_THE_UNMADE.cmd` now generates the crown kit with the other
193 models, validates all nine source hashes, and—only after the other
gates succeed—asks the real Unreal commandlet to import these nine
meshes into isolated folders:

`/Game/UnmadeProduction/PrototypeMeshes/RealmCrowns/`

It stages nine *additional, noncolliding visual reference actors* in
`/Game/UnmadeProduction/Maps/Unmade_PrototypeGeometryStaging`,
beside the existing 88 city modules. It never moves or replaces
previously placed actors, never overwrites imported static meshes
and never changes the authored gameplay map.

A successful engine-side run must produce
`TestReports/quickstart-<timestamp>/realm-crown-import-receipt.json`.
The receipt separately counts nine created/preserved static meshes
and nine created/preserved staging actors. The commandlet log is
`realm-crown-editor-import.log`.

**Not included in this milestone:** automatic real-world navigation,
quest collision or line-of-sight authority, fully designed roadways,
real actor animations, licensed art, gameplay-camera blocking, cooked
material validation, actual UE5.8 compilation, target-device budgets
and complete realm playthroughs.

These nine crowns are distinctive ***art starting points***.
They do not claim to be final production-quality world assets.
Production artists can replace them via the usual modular-mesh
pipeline without changing canonical IDs.
