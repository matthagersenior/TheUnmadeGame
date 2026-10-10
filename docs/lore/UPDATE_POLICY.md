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
