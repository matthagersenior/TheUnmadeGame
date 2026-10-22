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

### Import truth and next art pass

The models use **meters, Y up, positive Z forward** in glTF. Unreal coordinates are **centimeters, Z up**; use verified import conversion. All nodes are **static mesh objects**, including the individually named NPCs. They do **not** yet include skeletal joints, skinning, cloth simulation, blendshapes, authored collision, bone-weighting, texture atlases, normal map baking or generated Unreal navmeshes.

Import into a **separate, nonblocking art-review folder**; do not rename any existing player, enemy or NPC skeletal asset paths to these static previews. Do not replace authoritative city geometry, quest actors, evidence/interactive props, shipped material assets or save data. The nine environment and eleven city meshes are **source-authored spatial dioramas, not finished player-navigable levels**. Distinct GLB shapes do not prove collision, actual route connectivity, travel times or level design playtesting. They preserve realm identity without implying that geometry is UE-compiled or battle-tested.

When Unreal 5.8 is available, verify importer node separation, vertex color/material preservation, orientation/scale, contact/collision and GPU budget. Character models need proper sculpt/retopology, material detail and skeletal rigging; environment modules need separation into production kits, real navigation and lighting. Those are **remaining tasks**, not barriers to downloading and editing these actual GLB meshes now.

These new files are additive to earlier 202 static reference models and three original skinned prototype archetypes; they do not replace or falsely upgrade either.
