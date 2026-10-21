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
