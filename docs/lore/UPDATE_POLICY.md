# Canon synchronization rule

**When adding or changing a gameplay mechanic, realm, major quest, NPC, item, faction or consequence:**
1. Update `docs/lore/living-world-continuity.md` and log the change in `docs/lore/CONTINUITY_LOG.md`.
2. Edit the **complete** illustrated master (DOCX/PDF), the visual atlas if scenery changes, and ZIP with source, art and manifest. Do not deliver a partial addendum as a replacement for the master.
3. Explicitly mark source-implemented, engine-verified and future proposals. All ten signature abilities must remain grounded in player-visible interactions. All additions should be evaluated against the four RPG pillars.
4. Run the domain and source-contract CI. For file artifacts, render and inspect PDFs/DOCX, verify exact paths and ZIP integrity before linking.
5. Do not claim automatic artifact regeneration in ChatGPT without an actual repeatable, accessible build path. `Scripts/tests/test_lore_continuity.py` is a guard that catches missing canon coverage and stale item totals; it does not regenerate images or PDFs.

The source guide is version-controlled; the downloadable package is a dated release snapshot that must be rebuilt when canon changes.

## Transmedia canon and primary storyworld volumes

The historical illustrated editions I–IX are dated literary snapshots and must not be silently rewritten or referred to as self-updating. **Volume X — The Unanswered Atlas** (`docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md`) is a new primary editorial source, not merely a companion: future novels, television, films, episodes, and game expansions must cite its existing stable actor/city and mystery/callback IDs. Its machine-readable register is `Authoring/transmedia_storyworld_v1.json` and the regression gate is `python Scripts/validate_transmedia_storyworld.py --check`.

When issuing a new narrative work, record (a) canonical supporting sources, (b) authored facts versus in-world claims, (c) narrator/witness knowledge boundaries, (d) callbacks with physical provenance and real change in significance, (e) the OPEN mysteries deliberately left unanswered, and (f) whether its chronology assumes Held Morning, Many Mornings, or neither. Do not overwrite the player's chosen protagonist or canonize one mutually exclusive game ending. A secret revealed without revising the evidence trail and obtaining an explicit canon decision is a continuity error. Screen staging can change framing but not the causal core of an already established event.

The new literary visual motifs and performances are editorial production contracts only; they do not create engine actors, dialogue, recorded performances or save values. Refresh and render the full complete illustrated edition separately if distributing an updated binary book/PDF. Existing source CI does not rebuild books automatically.

**CI enforcement added:** `Scripts/check_lore_change.py` requires both `docs/lore/living-world-continuity.md` and `docs/lore/CONTINUITY_LOG.md` in the same commit as any changed Unreal source file. The illustrated PDF, Word book, art atlas and ZIP still require separate artifact regeneration and visual QA; this source guard does not create or update those downloads automatically.


## Volume X publication receipt and non-negotiable delivery gate (2026-10-10)

The complete illustrated Volume X edition was rendered on 2026-10-10 from the cumulative 84-page Volume IX illustrated Word source, the complete `docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md` literary manuscript, and `docs/design/volume-x-witness-echo-gameplay.md`. Outputs were inspected as a **104-page cumulative illustrated Bible PDF/DOCX** and a **29-page standalone Volume X PDF/DOCX**. The deliverable ZIP includes both editions, Markdown sources, selected original art, a release manifest, and an executable Python build recipe. This dated edition was delivered as conversation download attachments, not committed to GitHub or published as a GitHub Release; historical standalone illustrated editions remain unchanged.

**Every subsequent canon/gameplay update must close its publishing loop.** Update source canon, rebuild the cumulative illustrated Bible and current standalone volume, render/QA the PDF/DOCX, verify full content and archive integrity, and return direct download links in the same task completion reply. If a build/runtime limitation prevents regeneration, do not call the task fully delivered or claim the Bible is current: explicitly report the unissued book as an outstanding blocker. Passing CI is not a substitute for publication. New revisions should update `docs/lore/LATEST_ILLUSTRATED_RELEASE.md` with the date, source commit, included changes, artifacts actually produced, QA results, and any known gap. Version these edition receipts, never imply prior download links update in place.

The Volume X edition's portable publisher recipe is included in the previously issued ZIP, together with the base-source hashes and complete editable volumes. To make this fully automatic in GitHub CI in future, the repo still needs a legally distributable baseline illustrated asset and an artifact-upload workflow. This policy records the requirement honestly; a fully automated publisher is **not yet installed**.
