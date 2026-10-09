# THE UNMADE — Living region and exploration pass

**Status:** Code committed; not Unreal Editor compiled, photographed or playtested.

## Authored world scope
The original center has been retained; a larger connected **5,200 cm × 5,200 cm graybox floor** allows a loop through six outward locations. All structural meshes are temporary engine cubes. No fabricated map assets or stock screenshots are presented as game footage.

| Stable ID | Working landmark title | Approx. world X/Y (cm) | Hook |
| --- | --- | --- | --- |
| site.echo_well | Well of Returned Voices | -1720 / 850 | Sounds seem to arrive before events |
| site.paper_orchard | Orchard of Unwritten Names | 1260 / 1490 | Names of the never-born |
| site.silent_mile | The Silent Mile | 1890 / 210 | Footprints with no beginning |
| site.bell_grave | Grave of the Last Bell | -1790 / -1270 | Shadows lag their owners |
| site.market_ledger | The Debt Market | 850 / -1440 | Future purchases in a ledger |
| site.shelter_threshold | Threshold of Two Claims | -1470 / 250 | Reflects shelter vs research outcome |

Descriptions have different day/night passages, and the shelter landmark has different lines for both permanent faction choices. The six-site discovery ledger refuses duplicates and invalid masks.

## Cycle and game simulation
- World begins at 07:00; one in-game 24-hour day takes 20 real minutes.
- Clock handles dawn, day, dusk, night; leap frames are capped. Time and unique discoveries persist in the existing prototype save.
- NPC routine destinations vary by phase. Higher-priority reactive actions (investigate, intervene, avoid the player) take precedence over the daily routine.
- Location marker point-lights change temperature and intensity at dusk/night; a short authored cue is presented upon approaching a landmark.
- Press **E** while near a site to inspect its story and record progress. Revisit for alternate phase/choice-dependent text. **J** shows discovered landmarks, world time, and previous journal data.
- Sight and proximity rules apply; no LLM or online service is needed.

## Unreal acceptance tests (currently unexecuted)
1. Compile Unreal Editor C++ and run `Unmade.World.LivingCycle` plus the existing full `Unmade` suite.
2. Confirm ground collision extends across the entire 52 m square and that the six landmark markers are accessible without being trapped in ruins.
3. Navigate to each marker; E shows its unique authored text; repeat visits do not award further discoveries. J accurately shows 0–6.
4. Restart the game and confirm clock/discovery persistence. Save changes to Rewrite, settlement choice, language evidence and NPC memories must remain intact.
5. Use a shortened debug clock to verify all four phases, lighting changes, and different NPC routine destinations. Verify a witnessed anomaly still overrides routine targets.
6. Confirm shelters tell different stories depending on whether the player committed to shelter or research.
7. Run the same session with Ollama stopped and no internet connection; no quest, navigation, NPC agency or interactions should fail.
8. Check frame-time cost of five actor-ticked NPCs and world cue scanning; replace prototype approaches with navigation/streaming before scaling to hundreds of residents.
9. Test all relevant keyboard/controller interaction with usable prompts, audio design and UI later.

GitHub CI proves only native gameplay contracts and source wiring. A true level, environmental art, footsteps, diegetic audio and weather do **not** yet exist.
