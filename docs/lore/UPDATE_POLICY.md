# Canon updates and optional illustrations

Current policy: 16 October 2026, per the creator's instruction. This replaces
prior requirements to regenerate illustrated books with each gameplay update.
Earlier requirements quoted in dated logs, editions and receipts are historical;
they do not impose current development or release gates.

## Required source continuity

Canonical Markdown is the repository authority. Keep gameplay changes aligned
with `living-world-continuity.md` and `CONTINUITY_LOG.md` in the same commit;
`Scripts/check_lore_change.py` enforces these two source co-updates only.
Record engineering changes in `docs/CHANGELOG.md`. Preserve stable actor/city IDs,
consent, saved consequences, mutually exclusive endings and OPEN mysteries.

Volume X (`VOLUME_X_THE_UNANSWERED_ATLAS.md`) and the machine-readable
`Authoring/transmedia_storyworld_v1.json` remain primary editorial references.
Distinguish authored facts from narrator/witness claims, preserve knowledge
boundaries and evidence provenance, and do not canonize a player's chosen ending
or reveal an OPEN mystery without a creative decision. Run
`python Scripts/validate_transmedia_storyworld.py --check` when relevant.

## Illustrations available when needed

Illustrations, illustrated books and their PDF/DOCX/ZIP packages are optional.
They are not prerequisites for gameplay development, pull requests, passing CI,
source releases or declaring an engineering task delivered. There is no required
illustration count, cadence, automatic generation or book-rebuild obligation.

Keep existing art/scene briefs, historical edition references and
`Scripts/publishing/` builders available for on-demand illustration or publishing.
A requested standalone concept image can be produced independently of a book.
See [publishing workflow](../publishing.md) for inputs and provenance. Paid image
providers, commissions or infrastructure require approval before spending.

When an illustrated edition is actually issued, preserve earlier editions,
obtain the required masters/art, render and inspect the actual output, verify
archive integrity and hashes, then update `LATEST_ILLUSTRATED_RELEASE.md` and
`RELEASE_SHA256_MANIFEST.json`. Never invent receipts or refresh artifact hashes
without producing the corresponding files. Missing external masters block only
that requested book publication, not ongoing game development.

## Evidence remains explicit

Concept illustrations are not engine screenshots. A glTF, animation clip or
import plan is not an imported Unreal asset. Report source tests, actual asset
imports, Unreal compilation, rendering and playtest evidence separately.
