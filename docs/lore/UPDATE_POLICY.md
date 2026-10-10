# Canon synchronization rule

**When adding or changing a gameplay mechanic, realm, major quest, NPC, item, faction or consequence:**
1. Update `docs/lore/living-world-continuity.md` and log the change in `docs/lore/CONTINUITY_LOG.md`.
2. Edit the **complete** illustrated master (DOCX/PDF), the visual atlas if scenery changes, and ZIP with source, art and manifest. Do not deliver a partial addendum as a replacement for the master.
3. Explicitly mark source-implemented, engine-verified and future proposals. All ten signature abilities must remain grounded in player-visible interactions. All additions should be evaluated against the four RPG pillars.
4. Run the domain and source-contract CI. For file artifacts, render and inspect PDFs/DOCX, verify exact paths and ZIP integrity before linking.
5. Do not claim automatic artifact regeneration in ChatGPT without an actual repeatable, accessible build path. `Scripts/tests/test_lore_continuity.py` is a guard that catches missing canon coverage and stale item totals; it does not regenerate images or PDFs.

The source guide is version-controlled; the downloadable package is a dated release snapshot that must be rebuilt when canon changes.
