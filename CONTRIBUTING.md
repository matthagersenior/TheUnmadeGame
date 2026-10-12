# Development and release workflow

Use focused branches and squash-merge reviewed pull requests after required source checks pass. Preserve existing main history. Include canon updates with gameplay changes; record historical implementation details in docs/CHANGELOG.md rather than extending README.

The canonical Markdown manuscript is reproducible with Scripts/build_bible.py. Illustrated books are separately versioned editorial snapshots; see docs/publishing.md. Update their latest receipt in the same substantive publishing PR rather than opening a receipt-only PR. Never change artifact hashes without generating and verifying the corresponding files.

Use annotated source milestone tags and GitHub releases only against a passing main commit. Mark source milestones as prereleases and state whether engine verification occurred. A source release must not imply a playable executable. Upload illustrated binaries only when distributable inputs and actual artifact QA are available.

Repository metadata: description “A dark, surreal third-person RPG where reality and memory conflict.” Topics: unreal-engine, rpg, ue5. Prefer squash merging for future changes.
