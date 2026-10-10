# Latest illustrated lore publication

**Current edition:** Volume X — *The Unanswered Atlas*, First-PC Development Supplement (2026-10-10)  
**Included source revision:** `0981b9108b44f191a7f53c366087edc415e21aaf` · GitHub PR #26  
**Delivery channel:** verified ChatGPT downloadable files (not GitHub Release binaries)  
**Canon change:** None from prior Volume X. This revision adds a separately labeled tooling/production appendix; all original literary material, story variants and 52 cumulative illustrations are retained.

| Updated publication | Verified result |
| --- | --- |
| `THE_UNMADE_Complete_Illustrated_Lore_Bible_Volume_X_First_PC_Update.pdf` | **106 pages**, complete cumulative Bible |
| `THE_UNMADE_Complete_Illustrated_Lore_Bible_Volume_X_First_PC_Update.docx` | Editable cumulative master; 52 embedded illustrations |
| `THE_UNMADE_Volume_X_Illustrated_Complete_First_PC_Update.pdf` | **32 pages**, standalone Volume X |
| `THE_UNMADE_Volume_X_Illustrated_Complete_First_PC_Update.docx` | Editable standalone master; 5 embedded illustrations |
| `THE_UNMADE_Volume_X_First_PC_Complete_Bible_Package.zip` | All updated books, full manuscript, gameplay appendix, prior art references, reproducible append/build scripts, manifest and checksums |

**QA:** Both DOCX editions were rendered to PDF and complete PNG page runs. The final supplement pages were visually inspected; extracted text includes the full canonical Volume X material, 198 authored CSV rows, 52+90 reference markers and preservation/engine-testing boundaries. All pages were inspected programmatically for text boxes outside page bounds (none found), and the ZIP passed CRC integrity checks.

**Source contribution:** `START_THE_UNMADE.cmd` now launches a Windows preflight/build/test/authoring sequence. The seven DataTables have 198 source-authored rows; two isolated maps have 142 noncolliding reference work orders. Real UE 5.8 Editor compilation, Editor Python commandlet imports, PIE collision and complete world production still require Windows verification. The illustrated book records these boundaries without claiming they were executed.

The actual updated file bytes live in conversation downloads, not automatically on GitHub or inside Unreal. **The previous publication receipt is preserved below as historical provenance.** Future canon/gameplay updates must issue new cumulative and current-volume PDFs, Word masters and complete ZIP before reporting publication complete.

---

# Prior illustrated publication — original Volume X edition

**Edition:** Volume X — The Unanswered Atlas  
**Publication:** 2026-10-10  
**Source revision:** `144b1eff9904dd97c0b507b24bf2584a2daf68fd`  
**State:** Completed as conversation file downloads; **not** stored as GitHub Release assets.

The last documented user-delivered edition includes **the original 84-page illustrated Volume IX Bible with its embedded artwork and prior volumes retained**, the **complete 9,000-word Volume X manuscript**, and the **latest in-game Witness Echoes appendix**.

| File supplied to user | Pages |
| --- | ---: |
| `THE_UNMADE_Complete_Illustrated_Lore_Bible_Volume_X.pdf` | 104 |
| `THE_UNMADE_Complete_Illustrated_Lore_Bible_Volume_X.docx` | editable master |
| `THE_UNMADE_Volume_X_Illustrated_Complete.pdf` | 29 |
| `THE_UNMADE_Volume_X_Illustrated_Complete.docx` | editable standalone |
| `THE_UNMADE_Volume_X_Complete_Illustrated_Bible_Package.zip` | complete editorial kit |

The ZIP also contains the **full canonical prose Markdown**, Witness Echoes gameplay specification Markdown, source conversion script, selected reused concept art and a production manifest with SHA-256 provenance. The ZIP was integrity-checked, and the DOCX books were raster-rendered to **104 and 29 pages** respectively. Both PDF outputs were examined for expected characters, OPEN mysteries, and Witness Echo save markers, with no extracted text blocks extending outside the page boundaries.

**Literary revision:** 11 recognizable existing settlements, 13 familiar cast profiles, 9 deliberately unresolved mysteries, 13 callback contracts, two original dramatic prose scenes, and four crossmedia expansion outlines.  
**Game-source revision:** three tangible optional Witness Echo objects—Hessa's cup (Bellwold), Sevrin's press (Paperhaven), Orrel's nail (The Crossings)—with persisted observations and choice-dependent return readings. The full 13 callback registry is editorial; only 3 objects are present in source.

**Limitations:** No Unreal Editor compile/playtest is claimed. The new PDF/DOCX/ZIP were generated outside GitHub; a passing source CI workflow cannot produce or attach these books by itself. The next edition must refresh this receipt **and issue new cumulative Bible and current-volume downloads** before task closure.

**Canonical input:** `docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md`  
**Gameplay addendum:** `docs/design/volume-x-witness-echo-gameplay.md`  
**Release rules:** `docs/lore/UPDATE_POLICY.md`.
