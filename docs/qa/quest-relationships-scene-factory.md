# THE UNMADE — Quest, Relationship, Scene Factory Acceptance

## Offline / CI (must pass on branch and final main)
- Nine canonical regional main-quest contracts contain exactly six labeled beats each, including 2 distinct deliberate commitment resolutions, a physical objective, exact speaker/dialogue, trigger, failure/recovery and persisted-state directions
- Four finale stages represent Nhal-Vey's false mask break, actual core victory, deliberate new morning, postgame return
- Every later first-quest evidence + mechanism token originates from `UnmadeLaterRealmRules.h`; early symbolic references are explicitly marked as **not yet bound**
- All 82 stable resident ids remain in native registry order (48/16/18), and individual first-return, aid, local-story and post-morning lines appear in the generated lookup and CSV
- Native `ResidentContinuity` enforces one familiarity increment per in-game day, rejects day reversal, caps level at five, records aid once, refuses corrupt snapshots and leaves 82 residents isolated from each other's state
- Game runtime E interaction records actual conversations; OfferAid records aid; `WriteWorldSnapshot` stores the exact arrays and rejects malformed data, restores on save failure
- Source scene manifest contains exactly 36 realm references and 16 cinematic guides, with stable unique labels, no collision and no existing quest-control tags
- Offline Editor script defaults to dry-run; `--apply` is only accepted in a level actually named `Unmade_AuthoringStaging`; existing labels skipped, original live levels remain untouched
- Existing native C++ / Python regression checks and both cinematic/workflow drift checks remain green

## First Windows PC / Unreal Editor and packaged game (NOT YET VERIFIED)
- Run `Scripts/first_pc_build_and_test.ps1`; compile UHT including `UnmadeQuestSceneRows.h`; import main quest (54) and resident returns (82) CSV DataTables, along with existing world authoring/cinematic tables
- Generate dedicated staging map, run the importer twice; first run creates 52 no-collision placeholders and second creates 0; no changed runtime quest actors or SaveGame
- Real Reach opening (3 alternatives), frontier (Saltwake/Cinderhold), middle (Drevlach/Orravane/Vathless), upper (Eillun/Tharniv) and Auvren paths all playable through actual clue objects and choice controls
- For every branch, fail one evidence requirement, one line-of-sight check, one time-window confirmation and one simulated SaveGame write; verify **no false quest or relationship completion** and a safe recovery path
- Speak twice to the same resident on day 1: familiarity unchanged second time; visit next game day: unique returning line; help a resident and reload: personally remembered aid survives
- Load earlier version-1 saves lacking relationship fields; confirm NPCs begin at Stranger but their prior public quest choices remain
- Corrupt one relationship array length or count and attempt a write: reject without erasing existing inventory/quests; repair from backup before accepting again
- Inspect resident dialogue in a **different realm**; no impossible first-person knowledge of private remote quests appears
- Trigger optional third Echo chapters, guardians and paired-runes separately; main boss remains reachable without those completionist detours
- Build genuinely navigable/animatable scenes for each location, accessible multi-turn UMG dialogue, 3D collision, actor pathfinding, 16 Sequencer shots and meaningful soundscapes
- Test no real button-reversal/accessibility regression across the Nhal-Vey phases or changed-world return

**No native or editor-based test from a Windows Unreal workstation has run yet.**
