# QA — Tenfold signature disciplines and Confluence challenges

**Engine status:** Native C++17 gameplay tests and Python source wiring checks exist. **No Unreal Editor compile, PIE session, Android/PC build, visual test, playtime measurement or profiling has occurred.**

## Technical source checks
- Native rules: \`Tests/world/tenfold_chronicle_test.cpp\` and \`Tests/world/confluence_rules_test.cpp\`.
- Integrated scenario: \`Tests/integration/offline_slice_scenario.cpp\`.
- Source wiring: \`Scripts/tests/test_tenfold_unreal_source_contract.py\`.
- All under \`.github/workflows/static-checks.yml\`. These cannot substitute for UnrealHeaderTool or a packaged game.

## Temporary prototype keyboard inputs

| Key | Action |
| --- | --- |
| \`,\` / \`.\` | Select one of the ten disciplines |
| \`L\` | Display its current quest chapter and requirements |
| \`E\` | Inspect the selected rite stone, or converse with its matching NPC |
| \`F3\` | Advance a discovery/testimony/mastery chapter near its correct target |
| \`F4\` | Invoke the selected ability (a valid trial automatically records progress) |
| \`F5\` / \`F6\` | Make its protect/reveal choice, near the named witness |
| \`F12\` | Cycle gravity/sound/momentum for Unwrite, or warrior/artisan/archivist for Borrowed Lives |
| \`F10\` | Break an active oath, or attempt earned redemption after betrayal |
| \`F1\` / \`F2\` | Select/inspect one of six Confluence chambers |
| \`3\` / \`4\` | Commit the Confluence outcome once two powers and witnesses are present |
| Existing \`Q\`, \`F\`, \`J\` | Fold, Glimpse and the earlier general journal |

These should **not** ship as the final UI. Build UMG/Enhanced Input menus and accessible gamepad interaction after engine availability.

## Mandatory Unreal acceptance cases

1. UnrealBuildTool and UnrealHeaderTool compile **all added classes**; inspect generated headers and source includes. No success claim before that.
2. Verify ten site markers in physically navigable locations: Bellgrave, Echo Well, Paper Orchard, Bellwold Forge, Silent Mile, Debt Market, Bellwold Ossuary, Paperhaven Scriptorium, Bellwold Refuge and Saltwake Harbor.
3. Every rite needs earlier real saved exploration/quest flags; press F3 repeatedly without moving and ensure no chapter skips.
4. Speak to the correct named NPC with line of sight. Distant, occluded, unrelated and generative dialogue must never satisfy an authored quest stage.
5. Gather actual nearby resident eyewitnesses by performing witnessed events; ensure NPCs who only heard a rumor cannot count as direct independent testimony.
6. Test three Unwrite laws: visible vertical launch, temporary threat interruption, finite momentum push, distinct cost and time. No explosion of physics or stuck collision.
7. Witnesscraft should reveal a timed bridge and stabilize it only after mastery; verify save/reload and route traversal.
8. Try all three Borrowed Lives; only artisan should grant temporary advanced crafting, warrior should affect attack, archivist defense and identity-sensitive NPC lines. Skills cannot outlive their borrowed duration.
9. Record three distinct Legacy deeds from help, exploration and a faction resolution. Repeat the same action and ensure it does not raise the deed count or duplicate mythic gear.
10. Stabilize a road only after resolving *both* Bellwold and Paperhaven faction arcs. Changing the world must not obscure adjacent NPCs or remove essential walkable ground.
11. Borrow Tomorrow's Debt near the ledger, fight while empowered, roll to the next in-world day, experience temporary weakening, and verify you cannot borrow a second future until repayment. Save/reload through the due date.
12. Approach Hollow Keeper with actual witnesses; test combat death versus researched nonlethal mercy. Both end the encounter only once. A mastered memory echo must permit the late paired challenge even after pacifism or a prior kill.
13. Toggle the intact and ruined tower. The permanent form must respect the selected historical choice and persist through reload.
14. Break an oath before mastery; the NPC's trust/fear and words change. Validate no irreversible main-quest soft lock: Bellwold faction resolution, three distinct deeds and three firsthand witnesses permit costly redemption.
15. Chart a provisional Saltwake route. Verify only after true frontier travel with navigator testimony; rumor alone cannot activate the reliable marker.
16. Complete all ten five-step quests and six three-step chambers; repeat, reload and test corrupt fields. Rewards must remain once-only and compatible with the older 28/42-item saves.
17. Two trained disciplines must be used **at the same chamber within the 120-second window**. Repeating the same spell or standing in another village fails, and a timed-out pair resets safely.
18. The final chamber must reject missing rites, earlier unfinished chambers or missing frontier travel.
19. Profile world actor count, file size, 64 individual NPC memories, a growing quest snapshot, physics, available frame headroom and map streaming. Verify offline and with Ollama disconnected.
20. Test difficulty, discoverability and pacing with real players. Confirm there are multiple meaningful strategies, clear risks, accessibility features and a compelling sense of progress rather than arbitrary grind.

A green GitHub CI badge represents native logic and source contracts, **not** a played and beatable or balanced 3D RPG.
