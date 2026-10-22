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

### Import truth and next art pass

The models use **meters, Y up, positive Z forward** in glTF. Unreal coordinates are **centimeters, Z up**; use verified import conversion. All nodes are **static mesh objects**, including the individually named NPCs. They do **not** yet include skeletal joints, skinning, cloth simulation, blendshapes, authored collision, bone-weighting, texture atlases, normal map baking or generated Unreal navmeshes.

Import into a **separate, nonblocking art-review folder**; do not rename any existing player, enemy or NPC skeletal asset paths to these static previews. Do not replace authoritative city geometry, quest actors, evidence/interactive props, shipped material assets or save data. The nine environment and eleven city meshes are **source-authored spatial dioramas, not finished player-navigable levels**. Distinct GLB shapes do not prove collision, actual route connectivity, travel times or level design playtesting. They preserve realm identity without implying that geometry is UE-compiled or battle-tested.

When Unreal 5.8 is available, verify importer node separation, vertex color/material preservation, orientation/scale, contact/collision and GPU budget. Character models need proper sculpt/retopology, material detail and skeletal rigging; environment modules need separation into production kits, real navigation and lighting. Those are **remaining tasks**, not barriers to downloading and editing these actual GLB meshes now.

These new files are additive to earlier 202 static reference models and three original skinned prototype archetypes; they do not replace or falsely upgrade either.

### Bellwold hero art — embedded UV/PBR milestone

The Bellwold city GLB includes an **additional, canon-safe hero architecture layer** with physically sculpted ashlar/refuge facade elements, irregular arch voussoirs, timber porch beams and an **empty copper-green bell yoke** (the bell is never modeled). It uses locally and deterministically generated **192×192 color and tangent-space normal maps**, UV coordinates on the actual triangles, and embedded glTF 2.0 metallic/roughness PBR materials for weathered stone, salvaged timber and verdigris copper. The texture source is transparent Python in `Scripts/bellwold_hero_pbr.py`; it contains no licensed imagery or external downloads. Original 50-model identity and the earlier building-density, shape and canon tests remain in place; all other cities retain earlier vertex-color geometry for now.

The model is an **art-direction prototype**, not a photogrammetric sculpt, AAA environment, production-quality 4K texture set, or a playable Unreal level. A practical later artist pass must author hand-made UV layout/texture atlases, high/low mesh baking, LODs, collision proxies, corrected interior/exterior entrances, lighting scenarios, and a full Unreal import/visual review. The website inspector offers simulated three-point/dusk/overcast/night lighting for this purpose; it does **not** claim Unreal rendering was tested.

The exported `manifest.json` now includes `pbr_materials` (material names with embedded albedo+normal maps), so builds and the public viewer can distinguish true texturing from single-color vertex materials.
