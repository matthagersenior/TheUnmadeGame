# Canon and illustrated publication

Illustrations are optional. Gameplay development, pull requests, CI and source
releases do not require a new illustration, illustrated book, PDF/DOCX/ZIP, release
receipt or artifact hash update. Canon and continuity updates remain required for
gameplay-source changes. This policy supersedes earlier mandatory illustrated
delivery language in dated design documents and historical logs.

For an illustration when needed, start from the existing realm, character and
scene briefs in `Authoring/world_content_pack.json` and
`Authoring/cinematic_realm_playbook.json`. Request or commission only the needed
image; a standalone illustration does not require rebuilding the books. Label
concept art as concept art and record its source/provenance when admitted to the
repository. Paid generation or commissioning requires approval.

Existing illustrated-edition references remain in
[the latest historical receipt](lore/LATEST_ILLUSTRATED_RELEASE.md) and
[edition history](lore/ILLUSTRATED_RELEASE_HISTORY.md). Their external Library
paths are historical references, not verified downloads from a fresh checkout.
The optional book builders remain under `Scripts/publishing/`.

Version-controlled Markdown is authoritative. docs/lore/bible_sources.json orders the literary Volume X and supplemental chapters available in this repository. It does not claim to reconstruct historical illustrated volumes I–IX, whose masters and art are external.

Run `python Scripts/build_bible.py` to generate PublicationBuild/THE_UNMADE_CANON.md and manifest.json using Python 3.11 and no third-party packages. Each source and the output receive a SHA-256 digest; no timestamps or host paths enter the output. `--check` validates sources and deterministic reconstruction. CI builds and uploads this manuscript for every source workflow run.

The existing Scripts/publishing builders preserve and extend previous illustrated DOCX masters. They still require external books, python-docx, matplotlib and a document renderer; they are archival editorial tools, not a fresh-clone illustrated build. Do not substitute a plain manuscript for the complete illustrated books or discard earlier editions.

For an illustrated edition, obtain the exact preceding masters and distributable art, execute its builder, render both full books, inspect layout and inherited content, verify ZIP integrity and hashes, then update LATEST_ILLUSTRATED_RELEASE.md and RELEASE_SHA256_MANIFEST.json together with that publication. Retain older receipts in ILLUSTRATED_RELEASE_HISTORY.md. The latest receipt must name actual artifacts and their source baseline; source changes do not imply a newly issued illustrated edition.

A future fully illustrated fresh-clone publisher requires legally distributable baseline masters/art and pinned rendering dependencies. Until then, repository CI independently reproduces the canonical Markdown only.
