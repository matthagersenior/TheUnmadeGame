# Six Optional Echo Quests — Unreal/manual acceptance

The Native C++17 test `Tests/world/echo_quests_test.cpp` proves rule consistency, not Unreal runtime.

For **each of the six** realms:
1. On an old save with no Echo flag, earlier chapter outcomes must remain unchanged. The Echo quest starts dormant.
2. Finish first-visit arc and return aftermath; reapproach the unique original civic keeper and press E. A new third chapter starts and saves. It must not start before both prior chapters are complete.
3. Investigate the clue in the south quadrant and the distinct record on the upper civic terrace. Repeating one clue cannot substitute for the other; saving between two pieces and restoring works.
4. Talk to the matching care or truth witness. Attempting testimony before two clues or speaking to the wrong NPC does not progress. Do not block access to witnesses by NPC clipping or an inaccessible bridge.
5. Interact with a physical care/truth civic control **on the near side of the sealed gates**. One interaction previews the exact outcome; second same site within six seconds commits. Switching sites resets confirmation. Wrong testimony cannot commit.
6. Check the correct local monument and physically visible dispatch in **exactly one connected realm**, hidden on a clean save and correctly restored. Other realm stories remain independent.
7. Inspect the dispatch locally; distant NPCs must not magically know anything they haven't witnessed or heard. NPC reactions reflect public local changes and direct/rumor observations.
8. Walk the generated split arena before/after bridge selection. The gap must actually exist between terrain sections. Collidable wall geometry must not overlap the two gate apertures. Test actual capsule collision, fall recovery, traversal in both decisions and disallowed bypass.
9. Corrupt one saved Echo array's size or state and simulate SaveGameToSlot failure. Progress must be rejected and not silently reset. Reload all six in various completion orders.
10. Pass all Unreal automation, Win64 build, real keyboard/gamepad input, text accessibility, camera, audio cues, and native collision tests. Only then qualify the area as a demo candidate.

Check `docs/design/echo-third-chapter-world-bible.md` for individual scene briefs and character motivations.
