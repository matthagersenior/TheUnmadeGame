# Public website — THE UNMADE

This is a **non-canon publishing surface**, not an additional source of game rules. Canon lives in the authoritative `Authoring/`, `Source/` and `docs/canon/` files. Marketing text must not invent quests, stable IDs, release windows, engine tests or final mysteries.

The site is built from `site/index.html`, `site/styles.css`, `site/main.js`, Markdown reading editions and locally generated original SVG artwork. `Scripts/publishing/build_public_site.py` renders print PDF editions (including an optional full-spoiler source-faithful eight-volume encyclopedia), accessible reader pages and a published spoiler-light director's film treatment. The previous 54-second procedurally illustrated motion-art prototype was withdrawn after visual-quality feedback; it is not part of the site artifact or a finished trailer.

The GitHub Pages workflow is `.github/workflows/pages.yml`. It builds `site/dist`, validates outputs, then uses official GitHub Pages deployment actions. It requires repository Settings → Pages → Build and deployment → Source: **GitHub Actions**, and Actions permission to deploy Pages. Publishing may not go live until Pages is enabled in repository settings.

Beta interest and cinematic-direction feedback use GitHub's public issue forms. **No personal emails, names, phone numbers or addresses** belong in those submissions. This is a low-friction initial GitHub-user-only registry, not a private mailing list or live beta distribution system.

Local build:

```bash
python -m pip install reportlab markdown
python Scripts/publishing/build_public_site.py
python Scripts/publishing/build_public_site.py --check
```

No FFmpeg or placeholder film generation is part of public site publishing. A finished cinematic must pass separate visual-quality and canon review before a new media link is introduced. The Python build does not change the canon encyclopedia or source code. Outputs in `site/dist/` are generated and should not be committed as source.

**Publicity safety:** Never claim playable Unreal completion or call a rendered concept frame gameplay. Do not spoil OPEN, DISPUTED, SEALED or final-ending mysteries.

## Interactive source-authored 3D preview

`site/real-models.html` is a source-accurate model inspection surface linked from the landing-page hero. The Pages workflow **generates** 68 original `.glb` meshes into `site/dist/models/` from `Scripts/build_hero_models.py`, and verifies them against `models/manifest.json`: nine realm previews, eleven city models, twelve creature meshes, eighteen resident prototypes, nine canon-linked expedition boss silhouette studies and nine named lair entrance studies. The October 2026 character pass gives residents layered faces, articulated hands and individualized clothing; bosses receive distinct proportions and armor; and creatures gain species-specific anatomical details. Boss and lair appearance is marked as proposed visual direction. The page reads manifest dimensions, parts and triangle counts and uses Three.js to orbit the GLB files in the browser. It also includes a source-grounded Bellwold Afterlight example explaining evidence, witness, choice and aftermath. These are not playable Unreal content or a simulation of the actual final UI.

Publishing must not imply the GLBs are final hero art, physical levels, rigged animation, Unreal captures, or an installable beta. The actual source and production status are explained in `docs/art/actual-3d-models.md` and `docs/design/2026-10-09-bellwold-afterlight.md`.

## Bellwold interactive browser prototype

`site/play-bellwold.html` is a small standalone, explicitly non-Unreal browser demonstration of third-person movement, inspecting an authored physical relief cache / census, securing the corresponding first-hand witness (Sorin or Ivera), committing with Hessa and seeing a simplified saved visible aftermath on return. The page uses the original generated `models/Environment/SM_Unmade_ThreefoldReach.glb` asset while all graybox actors/markers and their positions are illustrative, not a final Bellwold map. It uses browser-local `localStorage` to remember a choice; that does not integrate with the canonical game save. The earlier lantern/Refuge Compact arcs are assumed already completed solely for this scene. Every label must state no actual Unreal gameplay, navigation/collision, skeletal animation or real quest build is proven by this demo. This is a hands-on design explanation, not a beta or trailer.

### Distinct realm and city topology

`Scripts/build_hero_models.py` now defines nine independent region route graphs and eleven separate city route graphs. Unlike the former common four-corner grid, each has a source-inspired ground/vertical silhouette, roads, and recognizable landmarks. Bellwold has its own uncast empty bell yoke and refuge/kitchen, Paperhaven has archive vaults, and other cities use appropriate topology. `test_distinct_realm_city_layouts.py` enforces non-template authored layouts. All remain dioramas; real navigation/collision and in-engine lighting are future work.

### Restored housing density (source preview)

After user feedback that the 50-model topology pass had removed too many buildings, regenerated the same 50 GLBs with **12 new inhabited-building exterior shells per region overview** and **20 per canon city**, laid out across distinct district clusters. The actual three-dimensional geometry includes local roofs, working props, civic textures and neighborhood road spurs; the viewer reports exact `building_shells` and `district_count` from the manifest. These are non-enterable, untested art blockouts, not polished full-size settlements or Unreal levels. Maintain minimum-density regression tests and check visual framing before reducing settlement fabric again.
