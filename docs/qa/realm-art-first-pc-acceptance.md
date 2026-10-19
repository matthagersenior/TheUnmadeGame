# Nine-Realm Visual Identity — First-PC Art QA

**Status:** Source-authored visual prototype; native Unreal Editor execution and asset cooking not yet verified. No packaged Content assets currently in repository. This is a **graybox replacement pass**, not final character/environment production art.

## Intent and visual design ledger

| Realm | Landmark | Base/accent RGB (8-bit) | Story-specific silhouette / gameplay read |
|---|---|---|---|
| Crossings | Three-Span Charter | 58/76/112 · 228/181/108 | Three unequal road columns, lintel and survey marker; distinguish refuge from public inquiry |
| Saltwake | Dry-Tide Gauge | 39/105/120 · 221/180/127 | Twin water poles, tide line and gauge; restored public cistern versus exposed invoice |
| Cinderhold | Borrowed Hearth Vent | 122/66/55 · 242/172/87 | Paired furnace flues, common-heat glow; kiln route and archive access remain distinct |
| Drevlach | Foreclosed Oath Pier | 58/88/111 · 213/164/109 | Suspended debt slates and two stacked tally crowns; care route versus accountable oath ledger |
| Orravane | Choir Counterweight | 78/100/152 · 232/212/163 | Upright supports, suspended weight and conducting bar; refuge span versus testimony dais |
| Vathless | Twin Ancestral Seam | 106/91/76 · 190/174/127 | Mismatched quarry pillars and opposing stone crowns; protected seam versus lineage records |
| Eillun | Occupied Blank Bed | 101/81/131 · 205/192/211 | Two shelter uprights with an absent central name; safe anonymous lodging versus witnessed census |
| Tharniv | Unclaimed Graft | 65/110/76 · 188/181/105 | Branching root spires and unequal graft horns; public orchard versus review of royal claim |
| Auvren | Double Honest Threshold | 79/75/124 · 172/220/216 | Two offset threshold frames with incompatible crowns; both plausible histories remain inspectable |

The nine landmark motifs use **distinct shapes**, not palette alone. Decorative components are deliberately non-colliding, non-overlapping and navmesh-irrelevant. Source validation must not depend on final textures or screenshots.

## Editor preparation (only when an Unreal-capable machine exists)

1. Enable the Unreal **Python Editor Script Plugin** in the project.
2. Run `Scripts/unreal/author_realm_palette.py` inside Unreal Editor Python (or commandlet) to create the cookable material `/Game/Art/Generated/M_UnmadeRealmPalette`. Verify its **Color** vector drives Base Color, **Roughness** drives roughness, and the material asset saves to **Content/**.
3. Open a test realm and verify the dynamic instance tint is actually visible. Until the asset is cooked, the renderer falls back to the engine's BasicShapeMaterial, which may ignore tint parameters; no colored build should be claimed from source-only CI.
4. Confirm the runtime material cache never exceeds **18** active instances, regardless of how many tagged props are spawned. Check hot-reload, asset failure, and deferred spawn behavior.
5. Rebuild the project with Unreal Header Tool and native C++; record a successful packaged Android/Windows build **only after actually obtained**.

## Collision / game access acceptance

- [ ] All original floors, bridges, doors and walls remain their original 100cm-base cube collision volumes. Existing tagged actors still toggle collision and hidden state after saves and reloads.
- [ ] Decorative children neither obstruct line of sight nor introduce extra overlap, navigation obstacles, touch damage, boss AI cover or unearned climb shortcuts.
- [ ] In each of the **nine** expedition frontiers, the landmark is visible but off the ordinary traversal/approach lane and does not block quest witnesses, workstations or future return routes.
- [ ] First-fracture ornament remains hidden until the room is actually entered; the discovery still requires E, gives no automatic fight/quest result, and never becomes collision.
- [ ] Care/truth gates in the seven newly physical aftermaths open only for the saved chosen branch; Saltwake and Cinderhold original paths remain mechanically independent.
- [ ] Day and night contrast works in dark, rainy and bright areas; text overlays remain readable when a landmark is behind a quest marker.
- [ ] Colorblind read: player distinguishes the nine silhouettes and care/truth signage **without** relying on hue. Check small phone previews and accessibility scaling.
- [ ] Save in another region, quit/reload, travel back: spawned deferred art appears once, with no duplicate mesh actors or loss of witness/road memory.
- [ ] Record frame time/draw calls and memory before/after the pass on the target mobile hardware. Replace repeated component meshes with HISM or authored static meshes if the measured perf budget requires it.

## Production upgrade path

Replace the procedural actors with cooked, authored modular meshes **one realm at a time**, keeping the same semantic tags, world centers, footprint/collision envelope, safe E-interaction distance and save-derived visibility. Art asset names should carry realm+role (e.g. `SM_Cinderhold_CommonVent_01`, `MI_Cinderhold_Stone`, `SM_Auvren_DoubleThreshold_01`), with separate LOD/collision and material profiles. Never change a stable ID just because a visual asset improved. This procedure protects the Atlas of Absence and witnesses from art-driven gameplay regression.
