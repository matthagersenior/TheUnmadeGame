# The Unmade — Canonical equipment source meshes

This is a source-built asset pipeline, **not** a claim of completed player
equipment or Unreal engine import.

## Real output

Run:

    python Scripts/build_equipment_geometry.py --out-dir Build/UnmadePrototypeEquipment

The script builds 78 original low-poly glTF 2.0 static-mesh files:

- 60 items matching the canonical ItemCatalog's stable IDs, including 12 weapons,
  13 armor pieces, 23 charms, 5 consumables and 7 materials.
- 18 distinct optional regional expedition equipment rewards (9 weapons and
  9 armor pieces), sourced without altering their exclusivity or acquisition.

Each output has triangle geometry, embedded PBR materials, normals, and an
individual SHA-256 entry in manifest.json. It includes its original source ID,
item kind, source acquisition and eligibility when documented.

Source variation produces distinct previsualization silhouettes for swords,
spears, mauls, staves, armor, charms, vials and resources. Some names have no
fully authored visual silhouette yet, so all generated shapes remain provisional
rather than being promoted into canonical visual identity.

## Automated verification

    python Scripts/build_equipment_geometry.py --check
    python Scripts/build_equipment_geometry.py --out-dir Build/UnmadePrototypeEquipment --verify-output
    python -m unittest discover -s Scripts/tests -p test_equipment_geometry.py -v

The Windows first-PC launcher now generates this equipment alongside the
nine-realm environment geometry. It retains the item asset names and exact
file hashes for comparison after import.

## What can still be done before Unreal

- Expand original visual concepts, UVs, texture maps, multiple LOD levels and
  socket/attachment briefs for each actual stable equipment ID.
- Define equip slot fit, hand grip/weapon pivot, attack window, armor deformation
  and scabbard/sheath behavior using authored collision and animation reference.
- Validate acquisition, consumption, mutual exclusion, crafting and save
  rollback via existing portable rules; never infer game grants from mesh files.

## What needs actual Unreal verification

- Import each glTF and inspect orientation/centimeter scale and materials.
- Turn armor into actually skinned wearable parts or fitted modular attachments;
  a static breastplate preview is not a skinned character outfit.
- Attach weapons to the player rig and verify animation poses, hit collision,
  clipping, stance timing and readable effects.
- Build player inventory icons, tooltips, in-game shops and repair visual
  interactions, then exercise their real saved behavior.

The canonical dependency ledger at docs/canon/dependency-ledger.json controls
rewards, costs and alternate quest consequences. No new visual prototype may
grant an item or change save compatibility.
