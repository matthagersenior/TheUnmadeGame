# THE UNMADE — player identity acceptance gate
**Source state only — Unreal build, UMG creator and real character meshes remain unverified.**

1. Start new schema-1 slot: default name `The Unmade`, common origin `Unmade.Origin.Impossible`, eight bounded selection values, no duplicate/ghost actor parts.
2. Bind UMG creator widgets to `SetCharacterFeature`, `SetCharacterChosenName` and `GetCharacterChosenName`. The debug C key only displays, not an editor. Show all eight dimensions with controller/keyboard accessible focus, spoken labels and color-blind legibility.
3. Choose min/max for body (4), face (8), hair (10), voice (6), palette (10), reality mark (8), gait (5) and calling (4). Ensure no invalid index can crash or corrupt the save.
4. Verify changing body and hair visibly affects the primitive placeholder on a GPU host; once authored assets exist, map other dimensions to meshes, rigs, materials and voiced performances. Check clipping with all equipment and combat animations.
5. Rename using ASCII, diacritics, non-Latin and unusual hyphenated identities. Reject invalid UTF-8, control characters, blank names and values above 48 UTF-8 bytes. Confirm visual truncation and screenreader support.
6. Save, quit and relaunch. All eight values and name should match; shared origin never changes. An old profile-less schema-1 slot starts with valid defaults, **without overwriting** the player's existing world choices, gear or NPC memory.
7. Inject invalid array lengths, out-of-bounds choice numbers and malformed strings. Reject profile edits without erasing other progression. Simulate failed `SaveGameToSlot` and verify in-memory state rolls back.
8. Complete an entire story/realm loop and return. NPC relationships must remain tied to identity rather than regenerated on appearance/name change. No LLM or network required.
9. Manually capture run evidence, Unreal compiler logs, controller ergonomics, GPU frame performance and snapshots after cold save/reload.

**Not production-complete:** a built-in debug profile is not the final creator experience, nor a rendered 3D customizable hero.
