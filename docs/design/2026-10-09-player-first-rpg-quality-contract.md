# THE UNMADE — Player-first RPG quality contract
*Approved direction / implementation standard, 9 October 2026. This is a product quality target, not a claim the graybox prototype already achieves it.*

## The promise

Make a substantial RPG that respects curiosity, time, choice, and skill. The signature player fantasy is **"people and places remember me, and I can remember versions of reality that should not exist."** Originality has to be felt in the controller, world and relationships—not only in item names, prose, menus or trailers.

The game should never depend on paid AI, online inference or a subscription for quest logic, dialogue fallbacks, combat, saves or completion.

## Ten player-first design tests

1. **Meaningful freedom.** Major conflicts support more than one reasonable response: tactical combat, informed nonviolent solutions, socially earned access, or manipulation of the environment. If one response is clearly and always optimal, redesign it.
2. **Honest world memory.** An NPC may know what they saw or what rumor reached them, not arbitrary global variables; their responses reflect temperament, memory and affiliations.
3. **Show the consequence.** Within the next local visit, a significant choice should affect geometry, access, price, routine, available evidence, danger or dialogue. A journal-only announcement does not suffice.
4. **Discoveries change play.** Reading a clue, understanding Veyl, verifying a map or learning a boss's old name should reveal a new action or route. Don't fill maps with collectible dots alone.
5. **Fair difficulty.** Bosses require observation, positioning and choices; attacks must be readable and counterable. Tougher must not simply mean more HP or grind, and players need multiple viable builds.
6. **Rewards carry a story.** Relics have a source and a reason to exist. Evolving gear responds to different actual deeds, not repetitive kills. Rewards never duplicate through save/reload.
7. **Lived-in settlements.** Residents have routines and motives. Social ties, public projects and conflicts change how a community functions; players are visitors rather than the center of every NPC's life.
8. **No synthetic length.** Never impose arbitrary fetch padding, excessive travel resets, daily chores, repetitive filler, unskippable dialogue or paid grinding to make the game "long." Depth comes from distinct encounters, meaningful decisions and changed contexts.
9. **Accessibility is not an afterthought.** Remappable controls, readable text, subtitles, feedback for sound-only cues, safe checkpoints, camera options and difficulty assistance should be tested as mechanics are built.
10. **Real verification.** A green CI for engine-independent C++ tests does not prove Unreal builds, animations, player navigation, saves or pacing. Distinguish implemented rules from graybox actors and editor-tested gameplay.

## First delivered step: five decision-reactive communities

Source now evaluates five local conditions from existing saved facts: unsettled, rebuilding, public/shared institutions, or publicly exposed evidence. The three original villages respond to their regional tasks and faction endings, and the two frontier communities to their local story endings. The system uses existing stored facts so it cannot be farmed for "reputation points" or diverge from old save state.

Each resolved condition has a distinct authored line, one of three visible graybox landmarks, and (for existing traders) a bounded local trust discount. Home-town NPCs refer to public changes without becoming omniscient about the player's personal history.

**Verification:** native \`Tests/world/community_consequences_test.cpp\`, source wiring \`Scripts/tests/test_community_consequences_wiring.py\`, and the existing CI. Graybox shapes, line-of-sight interaction, accessibility and actual world enjoyment are Unreal-unverified.

## The next three playable milestones

**1. One deeply authored return visit (Bellwold).** Play 20–30 minutes of dense, designed content with the shelter pressure, saved lantern quest, oath choice and Hollow Keeper. After a chosen outcome, the player returns to a visibly changed refuge, sees changed routines and goods, hears credible resident reports and encounters a distinct follow-up conflict. Verify both a protective and an investigative solution before extending the template.

**2. Frictionless controller loop and diegetic quest journal.** Replace debug text and function-key combinations with one interaction model, readable journal, map with confirmed/uncertain information, inventory comparison and proper warnings before irreversible acts. Include gamepad and accessibility QA.

**3. Combat and discovery with meaningful tension.** Add animated tells, audio substitutions, collision-verified arenas, fixed hit logic and a nonviolent boss alternative. Run player tests for clarity and challenge; tune only with evidence. No damage sponges.

### Acceptance, not marketing

A region is not "complete" until an Unreal-equipped QA host verifies traversal, saves, choices, combat alternatives, localization and controller UX. A whole game is not "long" because a spreadsheet counts 68 quest stages or nine atlas labels. The region needs enough unique encounters and consequences for real players to want to return. Optimize for that feedback, not an arbitrary hour count.
