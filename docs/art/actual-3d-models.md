# Actual GLB models · 50-piece source-authored production pass

**These are real 3-D vertex meshes** produced by `Scripts/build_hero_models.py`, not written descriptions, screenshots substituting for files, or Unreal-engine compilation claims.

There are **12 unique creature designs**, **18 individually accessorized NPC characters** (matching the exact names, IDs and visual directions in `Authoring/world_content_pack.json`), and **nine physically distinct realm overview dioramas and eleven canon-named city/settlement dioramas**. Each exported `.glb` contains actual triangles, vertex colors, descriptive individually named constituent nodes and the complete embedded glTF binary payload. `manifest.json` retains canonical ID, category, realm, individual triangle count, physical dimensions, file SHA-256 and the truthful unrigged/unimported state.

### Get the binary meshes

Use [Actions → Generate actual The Unmade 3D meshes](../../actions/workflows/hero-3d-models.yml), select a successful run and download its artifact named **TheUnmade-50-Creature-NPC-Realm-City-GLB**. These are the binary files; this repository commits the transparent, change-reviewable generator instead of inflating Git history with 50 unreviewable generated models per iteration.

Or generate them locally on any Python 3.11+ computer:

```bash
python -m pip install 'numpy==2.3.5' 'trimesh==4.11.1' 'Pillow==12.3.0'
python Scripts/build_hero_models.py --out-dir Build/UnmadeHeroModels
python Scripts/build_hero_models.py --out-dir Build/UnmadeHeroModels --verify-output
```

Four generated directories:

- `Build/UnmadeHeroModels/Creature/`: species-specific bodies and signatures; no shared generic wolf placeholder for watchers, glass moths, drowned sentinels, or letter-mouth monsters.
- `Build/UnmadeHeroModels/NPC/`: 18 canonical named individuals with unique actual geometrical accessories (ledger cord, rope boat hook, monocle, tuning pin, quarry visor, census tokens, blank banner, bark crown, lamp, etc.).
- `Build/UnmadeHeroModels/Environment/`: one distinct route network and material environment per each of the nine realms (no shared straight street or square plan).
- `Build/UnmadeHeroModels/City/`: eleven additional, independent source-grounded layouts for the three Reach towns (Crossings, Bellwold, Paperhaven) and Saltwake, Cinderhold, Drevlach, Orravane, Vathless, Eillun, Tharniv, and Auvren; all have their own route geometry and named landmark structures.

### Density restored after the sparse-layout regression

The unique-route version inadvertently replaced some earlier housing clusters with nearly empty landmark scenes. The revised geometry retains nine different regional path networks and eleven different city layouts while **adding 12 individually constructed dwelling/working-building exterior shells to each region overview and 20 to each city diorama**, organized into three and four neighborhood clusters respectively. This is **328 new source-authored building shells across those 20 previews** (9 × 12 + 11 × 20) beyond the existing individual landmark architecture, not 328 finished/enterable playable interiors. Homes have different materials, rooflines, and local additions according to civic function: shelters and lanterns in Bellwold; chalk records in Paperhaven; inverted hull dwellings in Saltwake; furnace chimneys in Cinderhold; counted canal lockhouses in Drevlach; suspended houses in Orravane; quarry ledgers in Vathless; consenting thresholds in Eillun; grown petition shelters in Tharniv; and limestone witness rooms in Auvren.

The manifest records the exact generated `building_shells` and `district_count` and the public 3D viewer shows them. `Scripts/tests/test_distinct_realm_city_layouts.py` now fails if a future art pass deletes the neighborhood fabric or substitutes only sparse landmarks. Overlap clearance, physical entrance access, real occupancy and final playability must still be assessed in Unreal.

### Creature face detail pass

The eleven creatures with eyes now use four independently named geometric layers: a dark socket, colored iris, forward pupil and small light-catching glint. The added parts remain vertex-colored GLB geometry, with no external textures, shared creature placeholder, or changes to enemy behavior. The 50-model workflow checks the exported components in each generated creature GLB. KilnWisp retains its established faceless coal-and-flame silhouette. These additions improve the source-authored visual references; they are not rigged or Unreal-rendered models.

### Import truth and next art pass

The models use **meters, Y up, positive Z forward** in glTF. Unreal coordinates are **centimeters, Z up**; use verified import conversion. All nodes are **static mesh objects**, including the individually named NPCs. They do **not** yet include skeletal joints, skinning, cloth simulation, blendshapes, authored collision, bone-weighting, texture atlases, normal map baking or generated Unreal navmeshes.

Import into a **separate, nonblocking art-review folder**; do not rename any existing player, enemy or NPC skeletal asset paths to these static previews. Do not replace authoritative city geometry, quest actors, evidence/interactive props, shipped material assets or save data. The nine environment and eleven city meshes are **source-authored spatial dioramas, not finished player-navigable levels**. Distinct GLB shapes do not prove collision, actual route connectivity, travel times or level design playtesting. They preserve realm identity without implying that geometry is UE-compiled or battle-tested.

When Unreal 5.8 is available, verify importer node separation, vertex color/material preservation, orientation/scale, contact/collision and GPU budget. Character models need proper sculpt/retopology, material detail and skeletal rigging; environment modules need separation into production kits, real navigation and lighting. Those are **remaining tasks**, not barriers to downloading and editing these actual GLB meshes now.

These new files are additive to earlier 202 static reference models and three original skinned prototype archetypes; they do not replace or falsely upgrade either.

### Bellwold hero art — embedded UV/PBR milestone

The Bellwold city GLB includes an **additional, canon-safe hero architecture layer** with physically sculpted ashlar/refuge facade elements, irregular arch voussoirs, timber porch beams and an **empty copper-green bell yoke** (the bell is never modeled). It uses locally and deterministically generated **192×192 color and tangent-space normal maps**, UV coordinates on the actual triangles, and embedded glTF 2.0 metallic/roughness PBR materials for weathered stone, salvaged timber and verdigris copper. The texture source is transparent Python in `Scripts/bellwold_hero_pbr.py`; it contains no licensed imagery or external downloads. Original 50-model identity and the earlier building-density, shape and canon tests remain in place; all other cities retain earlier vertex-color geometry for now.

The model is an **art-direction prototype**, not a photogrammetric sculpt, AAA environment, production-quality 4K texture set, or a playable Unreal level. A practical later artist pass must author hand-made UV layout/texture atlases, high/low mesh baking, LODs, collision proxies, corrected interior/exterior entrances, lighting scenarios, and a full Unreal import/visual review. The website inspector offers simulated three-point/dusk/overcast/night lighting for this purpose; it does **not** claim Unreal rendering was tested.

The exported `manifest.json` now includes `pbr_materials` (material names with embedded albedo+normal maps), so builds and the public viewer can distinguish true texturing from single-color vertex materials.

### Realm-specific material and civic landmark pass (all nine realms / eleven cities)

`Scripts/realm_hero_pbr.py` adds original locally generated **embedded glTF PBR** 160×160 albedo and tangent-space normal textures, with distinct roughness/metallic settings, UVs and individually modeled public-landmark components for Crossings, Paperhaven, Saltwake, Cinderhold, Drevlach, Orravane, Vathless, Eillun, Tharniv, and Auvren. Bellwold retains its existing 192×192 hand-oriented procedural stone/timber/verdigris hero layer. This extends the original art from **one** textured showcase to **all twenty realm/settlement geographical GLBs**, without changing the total 50-model roster or reducing existing neighborhood density. A realm preview uses its own representative settlement's distinct material and public-landmark architecture rather than sharing a single generic house kit.

| Location | Actual new modeled hero focus | Material construction language |
|---|---|---|
| Crossings / Threefold Reach | Disagreeing three-way survey forks and charter gantries | Surveyor limestone and river-patinated iron |
| Bellwold | Stone/timber refuge facade; **empty** copper-green yoke | Patched stone, salvaged timber, oxidized copper |
| Paperhaven | Tiered archives, buttresses and folio niches | Chalk archive stone and dark storage fittings |
| Saltwake / Widowed Rain | Upturned hull ribs, dry slipways, rain catchments | Salt-weathered hull timber and dock bronze |
| Cinderhold / Hearth Beneath | Banded high thermal kilns and feed lines | Sintered basalt and thermal copper |
| Drevlach / Owed Estuary | Public tide-accounting clock, counterweights and ledger rails | Wet sluice granite and ledger brass |
| Orravane / Inverted Choir | Suspended walkways, cable anchors, acoustic weights | Suspension slate and resonant alloys |
| Vathless / Yesterday Quarry | Layered bearing-stone switchbacks and ledger faces | Memorial stone and support iron |
| Eillun / Unclaimed City | Reversible privacy screens and voluntary thresholds | Reused plaster and privacy timbers |
| Tharniv / Untitled Orchard | Branch-built petition court, grafted civic roots | Living bark and grown resin |
| Auvren / Before-Place | Disconnected contradictory portals and witness platforms | Unfounded limestone and witness alloy |

Each asset manifest includes `hero_label`, `hero_focus`, and embedded `pbr_materials` for inspectability; the public viewer displays a location-specific **Hero landmark** action. CI tests distinct geometry names, UV/PBR export, material identities, texture preservation, density and deterministic GLB equality. **These maps are compact procedural visual-development textures, not handmade production 2K/4K scan-quality materials.** Without Unreal import, collision, LODs, hand-scanned textures, lighting, rigging and gameplay testing, no preview should be marketed as final in-engine imagery.

### Residential surface and terrain fidelity — full twenty geographic assets

The residential exteriors now go beyond vertex-colored architectural blockouts: `Scripts/district_surface_detail.py` adds a textured physical pitched-roof skin, front and side weathered façade surfaces and locally relevant functional accessories to **each** existing habitation shell. That is 20 façades/roofs in each of the 11 separately modeled cities and 12 in each of the nine realm overviews (328 residential surfaces across the 20 geographic GLBs). It does not create, remove, or relocate any of the original 328 houses. The independent PBR layer and density counts coexist; floorplans and interiors remain unimplemented.

Each neighborhood also receives a **real triangulated terrain surface**, with authored functions varying by construction culture: quarry steps in Vathless, water-cut basins in Saltwake, raised suspended courts in Orravane, thermal ripples in Cinderhold, canal-like grading in Drevlach, grown root berms in Tharniv, unresolved limestone irregularities in Auvren, and other locally distinct ground rhythms. These are non-collision-tested scenic terrain, **not playable roads, functioning water systems, or Unreal landscape components**.

The original locally generated realm materials are now 256×256 albedo/normal textures, and there is a separately weathered UV/PBR roof material family for each city, including Bellwold. The attached GLB has real UV coordinates and tangent-space normal textures for building and ground surfaces. Source remains deterministic, self-contained, and without external textures. The manifest records `textured_roofs` and `sculpted_districts` for measurable non-regression. The public viewer shows these counts.

These textures are deliberately development assets. They are not high-resolution hand-painted production materials or detailed architectural sculpture. Distinctive low-poly building forms remain visible beneath the new PBR skins. Material improvements must be evaluated with visual screenshots and actual Unreal import/lighting before representing them as finished game art.


### Street-level façade depth and inspection (all 328 source-generated dwellings)

The previous UV-mapped `rainworn_facade` was **one opaque quad** in front of each house. This could conceal doors and bright windows and was a visual-fidelity regression despite its new material textures. The street-level pass replaces that quad with a triangulated, actual **apertured front finish**, aligned to the existing doors and upper windows on all **20 × 11 city** and **12 × 9 regional** residential previews. Each house gains combined PBR-trimmed casing geometry, recessed panels visible through the cutouts, and a small layered public entry threshold. This preserves all 328 original inhabited shells, distinct street plans, roof skins, civic structures and textures rather than adding density at the expense of detail.

`Scripts/tests/test_street_level_facades.py` checks apertures geometrically: triangles must not cover the door or window centers; each city/region model must have one textured apertured finish, window inset, surround and entry threshold per dwelling. `test_district_surface_fidelity.py` continues to require source-generated UV/normal-mapped front surfaces and roofs. The model inspector adds explicit **Street view** buttons for each district; these are model-viewer camera placements, not verified player spawn points, interiors, ramps, collision or Unreal traversal. The old structural wall shell still exists behind the visible openings, so these are recesses rather than architectural openings through an enterable building.

The pass uses the existing cultural materials without importing external images. Real production locations will still require modeled door openings through collision meshes, field-of-view checks, walkway gradients, pathfinding, layered lighting, unique wear sculpting, and per-location level art review.

### Terrain-grounded dwelling foundations and approach paths (October 2026)

Previously, the higher-detail residential shells were centered around fixed floor levels, while each district's UV/PBR landscape relief was generated separately. This risked detached steps, floating foundations and disconnected civic approaches. `Scripts/district_surface_detail.py` now exposes `terrain_surface_height()`, which is reused exactly by the rendered ground mesh and the new `Scripts/terrain_street_joinery.py` footing/path module. Every existing building has four individually terrain-sampled footing locations, plus two-footed cultural bracing in raised settlements, and a physically modeled curved pedestrian approach that rises from the district's public center to that building's entrance. The approach path is not claimed to be safe for walking or a usable navigation link.

For the existing 20 buildings in each of 11 cities and 12 in each of nine realm overviews, this adds **328 PBR footing assemblies and 328 graded access surfaces** without duplicating/replacing the habitation shells or changing any of the fifty original GLB asset identities. Site inspection displays counts named `grounded_homes` and `graded_walkways`, directly calculated from geometry. Material construction proportions vary by culture (raised hull timber, suspended choir bracing, stepped quarry footing, rooted civic support, and ordinary stone/timber thresholds), and each footpath uses the city's own ground function.

**Scope is visual 3D proof of concept only.** The meshes and path ramps have not been evaluated for Unreal collision/navmesh, ergonomic dimensions, actually continuous passability, accessibility, photorealism, architectural load safety or overlap with other buildings. The 15×15 district relief sampling still requires production retopology, seam stitching, and careful field-of-view evaluation before use as a finished landscape. Regression tests explicitly compare numerical terrain heights with exported ground mesh vertices and check every existing building's two grounding mesh groups.

### Local terrain edge sculpting — eliminate naked square district slab borders

Source previews previously terminated each 15×15 sculpted terrain mesh at a **perfectly square border**, leaving the large foundation pad visible as a cut-off plateau. `Scripts/district_surface_detail.py` now generates one **three-band, 56-perimeter-point sculpted transition apron** around every neighborhood's terrain relief. The apron shares the **exact vertex positions and heights** at all 56 grid edge points, and fades toward a separately distorted outer contour with location-specific relief/erosion rhythms; the underlying rectangular pad is buried beneath those modeled surfaces, without moving its authored district, houses, streets, or civic centers. The test compares the actual geometry at those seam points, rather than only checking a manifest count.

This means **four transitions per city** and **three per realm overview** (71 total across the 20 geographic models), all with locally generated UV/PBR texture and normal maps. `blended_edges` in the manifest and public model viewer gives a reliable count and guards against later density/terrain quality regressions. Edges now soften into the underlying diorama, but do not prove seamless welding across independently generated overlapping districts or to Unreal landscape tiles. Actual level-art seams, natural heightfield retopology, final rock materials, shading, collision, and traversal still require Unreal/Blender review.

