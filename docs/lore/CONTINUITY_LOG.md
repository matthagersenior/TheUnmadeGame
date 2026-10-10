# THE UNMADE — Illustrated Lore Continuity Log

Every substantial gameplay addition needs a companion canon update and a regenerated illustrated package edition. This history describes what was actually updated; it is not a claim that engine content was playtested.

## 2026-10-10 — Volume II: The World Remembers
- Codified the four player-first standards and ten signature mechanics as required narrative and interaction checks.
- Added fully authored follow-up adventures for the **nine** established realms. The rules are implemented in a deterministic C++17 source model with identity-specific evidence, witness and choice gates. Six realms are still conceptual for Unreal runtime.
- Expanded the aftermath of The Threefold Reach to include Bellwold's **The Second Night** and The Crossings' **Road After the Rescue**.
- Updated the canonical item count from 58 to **60** after adding Ward of the Second Night and Lantern of Unredacted Names.
- Retained 64 currently authored NPC identities (48 core and 16 frontier); later realms have proposed, unspawned witness IDs.
- Updated the illustrated package master, art atlas, source documentation, quality matrix and ZIP. The imagery remains speculative artwork rather than actual Unreal gameplay screenshots.

- Added a CI-level source-change gate: gameplay commits must carry updated canonical lore and the continuity log together, in addition to the dated downloadable illustrated edition.

## 2026-10-10 — Volume III: Saltwake / Cinderhold physical return sequence (source pass)
- Added deterministic, non-farmable physical mechanism requirements to all nine realm aftermath rules; Saltwake's storm sluice and Cinderhold's heat vent are the next actor-connected milestones.
- Preserved the two ethically distinct evidence/witness paths and permanent outcomes; later six mechanism IDs remain authored contracts without spawned worlds.
- Updated the living canon in this same source commit. The full illustrated DOCX/PDF/art atlas/manifest/ZIP package is **not yet regenerated** and must not be represented as shipped.
- Reserved a separate save schema-compatible nine-realm aftermath snapshot and a dedicated Hub/Player API, keeping earlier frontier and Bellwold states intact. Pending Unreal-host runtime acceptance.\n- Actor integration now includes physical sight/proximity inspection, storm/heat barriers, two exclusive follow-on routes each, outcome refresh on load, and atomic save rollback. Source contract tests cover the wiring; Unreal Editor behavior remains unverified.\n- Player E interaction now prioritizes the nearer physical control/clue over a farther NPC, journal tracks both returning realms, and F7/F8 resolves matching testimony near a warden.
- Saltwake and Cinderhold residents use public home-town world states plus separately witnessed/rumored choice events; the deterministic offline speech remains sufficient without AI.
- Added a world-actor QA checklist and realm-by-realm physical adventure acceptance standard; all six later realms remain future production until engine-tested.\n
## 2026-10-10 — PC-readiness source pass (hazard and fail-safe recovery)
- Added a world-clock-anchored, location-bounded rain/heat pulse domain; hazard damage and warning periods are deterministic, avoid safe settlement routes and end when the corresponding mechanism is operated. Windows Unreal runtime still unverified.
- Added checkpoint revival to prevent permanent loss of movement on player defeat. Story/world saves are deliberately not reset on recovery; game feel and performance remain engine/creator-gated.
- Wired visible non-colliding warning strips into Saltwake and Cinderhold, world-clock hazard sampling into the player, guarding counterplay, deduplicated environmental hits and safe checkpoint respawn after four seconds. Gameplay verification awaits Unreal Editor.\n- Corrected initial warning-strip visibility: the noncolliding cues are hidden on spawn and revealed only during timed warning/impact windows.
- Authored a one-command Windows preflight + Editor build + automation suite runner with retained logs; this script has not yet been exercised on a licensed Unreal GPU host.\n- Added a native Unreal automation test for timed hazard, guarding, duplicate-pulse rejection and checkpoint revival; CI runs pure C++ separately, but UE automation remains unverified.\n
## 2026-10-10 — PC readiness: protagonist identity source
- Introduced an offline canonical 8-dimension customization model, distinct Unicode-capable name validation, atomic snapshot restoration and immutable shared impossible origin. Unreal character creator art and menus are still unbuilt.
- Added backward-compatible Unreal SaveGame profile fields, Blueprint-callable name/feature changes, rollback on failed writes, basic body/hair preview geometry and a keyboard profile readout. Full asset-driven creator screen remains outstanding.\n- Added an editor-side automation source test for profile selection and atomic restoration, and recorded a full creator QA gate. This is not a verified UMG creator or shipping character assets.\n
## 2026-10-10 — Informed irreversible decisions
- Added offline deterministic six-second confirmation with scope/choice matching, invalid-input rejection, cancellation and expiry.
- Integrated scoped source previews before F7/F8 mutations for the faction, frontier and aftermath chapters. No single accidental keypress can commit these paths through the player input entrypoint. Real UI/controller acceptance still pending.
- Added a dedicated Unreal automation source test and engine/manual QA gate for choice preview, path-change cancellation, save failure and controller accessibility. Actual Unreal acceptance remains outstanding.\n
## 2026-10-10 — Volume IV illustrated edition: A World Prepared to Respond
- A new **52-page editable DOCX and rendered PDF** were produced from the Volume III illustrated master, preserving the existing concept-art atlas and Saltwake/Cinderhold route diagrams.
- Added Part XVII with source-aligned hazard counterplay, safe checkpoint, eight-dimensional identity, two-press story confirmation, the first-PC build/test command and a truthful content readiness matrix.
- The complete package includes the 30 original/schematic art assets, quality matrix, source appendix, release status and a SHA-256 manifest; all included ZIP files passed checksum verification.
- **Distribution boundary:** Volume IV ZIP/DOCX/PDF are generated conversation artifacts, *not committed binary files in this GitHub repository*. Canonical runtime code remains on main. Images remain concept art, never proof of Unreal gameplay.

## 2026-10-10 — Six later realm foundations (source pass)
- Created six distinct first-visit realm journey rules with protected initial evidence, two physical strategies, control activation, durable route change and old-save compatibility.
- Added atlas travel scaffolding, per-realm generated landscape blockouts and locally persistent post-arc choices; the game still requires an engine compile/playtest and full art/encounter pass.
- Repaired duplicate `UPROPERTY(SaveGame)` annotation on the realm aftermath snapshot, an Unreal UHT risk discovered during source review.

## 2026-10-10 — Volume V source milestone: Six Roads Beyond
- Extended six later realms from future atlas and return-arc data into source-generated physical gap/bridge and civic gate geometry, 18 stable actor identities, earned-atlas travel, first-visit trial decisions and second-visit aftermath evidence/witness/choice paths.
- A new six-slot optional save snapshot preserves visited, stage and route results without replacing old schema-1 slots. Source checks cover state corruption, rollback and separate realm progress. No Unreal Editor compilation or gameplay performance has been demonstrated.
- This substantial code milestone requires regenerating the illustrated package as Volume V rather than silently labeling the Volume IV binary current.

## 2026-10-10 — Volume V illustrated canon package verified
- Produced a 55-page illustrated Volume V Word master and PDF from the Volume IV edition, with the existing 30 art assets retained; rendered and visually inspected the new pages.
- Assembled 38-file ZIP with quality matrix, source appendix, release status and SHA-256 manifest; verified all entries for integrity and checksums.
- Source game baseline `61a90178985b34dbffde5eb54a509562d7ae8c38` is merged into main and the GitHub native source CI is green. Volume V package is provided as a conversation download, not a tracked GitHub binary.
- Honest limit: six later realm levels remain primitive cube source blockouts, not authored UE terrain or verified 3D gameplay.

## 2026-10-10 — Six differentiated environmental counterplay loops
- Wrote independent C++17 harm/strain terrain danger samplers for all six future regions. Safe entrance and evidence flanks, warning windows, clock identities and persistent disabled-on-quest-completion semantics are tested before Unreal runtime.
- Integrated source-visible noncolliding warning strips, player per-realm pulse deduplication, combat guarding for injurious hazards, and capped recoverable reality strain for intrusive/memory hazards. Failed strain writes are rolled back in memory.
- No Unreal Engine compilation or visual/audio playtest. The last rendered Volume V Bible predates this change; a future illustrated update must incorporate it.
- Local later-realm NPCs seek a deterministic safe lane during an observed local warning/impact and provide hazard-specific guidance. The source model tests the shelter destination for every realm; eventual Unreal NavMesh and locomotion still require engine acceptance.
- Added a dedicated Unreal automation test `Unmade.World.SixLaterRealmHazards`, a realm-by-realm manual acceptance sheet, and technical pre-PC status. These are source documents and have not been run in UE. Next illustrated Bible volume must capture the implementation; Volume V remains the latest verified rendered art edition.

## 2026-10-10 — Volume VI illustrated edition: Lands That Teach
- Produced and visually reviewed a new 57-page Word/PDF compendium with two additional sections: six differentiated hazard/counterplay loops and first-PC verification/production provenance. Kept all 43 embedded illustrations and all 30 existing atlas image assets from prior editions.
- Generated a 38-entry distribution ZIP with source appendix, release status, a six-realm quality matrix and SHA-256 manifest; checked every ZIP entry for integrity and digest correctness.
- Record pinned to merged gameplay source `fe849e839cf7eb3550df9711d99fc42cf50f960f`, PR #8. GitHub main CI verified only native C++17/Python source tests; Unreal compilation, a runtime hazard experience and GPU frame acceptance are still pending.
- The illustrated files are provided as downloadable conversation artifacts and are **not** committed to this repository. New concept visuals are **not** claimed as Unreal render footage.

## 2026-10-10 — Third-return Echo Quests and physical graybox correction
- Authored six unique tertiary quest narratives (two non-repeatable clues, conflicting evidence, separate care/truth witnesses, a named irreversible council ritual with two-press confirmation, a civic monument and one adjacent-realm physical dispatch).
- Added data-only C++17 `EchoChronicle` with atomic six-realm saved progress, four optional schema-1 save arrays, invalid-state refusal, independent outcomes and rollback after failed Unreal SaveGameToSlot.
- Integrated E interaction and source-spawned physical sites and public outcomes with the existing nine-realm first and aftermath quests. Added optional journal status and local NPC aftermath commentary; remote NPCs do not gain magical knowledge.
- Corrected a serious previous graybox geometry error: original north/south grounds overlapped, and walls overlapped their decision gate apertures. A compile-time geometric invariants header now checks that both bridges reach the two separate grounds and exclusive wall gate apertures do not overlap. Only real UE collision/navigation tests can confirm the blockout physically works.
- Detailed third-chapter writer guide and Unreal acceptance matrix added; all ten rites have authoring cross-references but their advanced runtime combinations remain future work. Volume VI remains the latest illustrated PDF; the next art edition must include these narratives.

## 2026-10-10 — Nine-realm production authoring and NPC identity pass
- Established one editable offline JSON source of production authoring truth: 9 realm cultures, 18 named stable-ID later witnesses, 10 pre-existing signature rites. Wrote distinct desires/fears, day/night lines, knowledge limits, costumes, motions, scene audio and ability uses.
- Added deterministic tested Python generator for a portable C++ actor registry, three Unreal DataTable CSVs and full human-readable production dossiers; early CI and first-PC command fail on drift.
- Wired the actual 18 later-realm source NPCs to generated display names and day/night dialogue while preserving existing quest and memory rules. All native RPG progress/state remains independently authoritative.
- Added Blueprint DataTable row declarations and first-PC import instructions. Generated files are a production convenience, **not finished Unreal assets or recorded voice lines**. Illustrated volume remains VI until the next PDF/DOCX build.

## 2026-10-10 — Volume VII illustrated canon and build-ready production dossier
- Published **The World Withheld Its Answer**, a 66-page Word and PDF edition, preserving the 43 previous embedded illustrations and 30 original art atlas files while adding fuller six-Echo-quest stakes, nine civilization habits, 18 named people, ten ability pairings, editor source-to-CSV authoring and first-PC traceability.
- Generated and SHA-256-verified an archive of 38 entries; visually inspected new 21-page sample montage and final chapter page. The editable Word, PDF, art, quality matrix and source appendix are included.
- Pinned gameplay source to PR #10 / `bb5a929bb5d4a4e794dfb6c55fd5899decd75c66` and authoring source to PR #11 / `e8c3948cdfc697c666621eab16670f7a55b7634a`. Both post-merge source workflows green.
- The package remains conversation-only binaries, not committed source or actual Unreal screenshots. No UHT/UE Editor compilation, avatar rigs, animations, full voice recordings or finished 3D continent playtest is claimed.

## 2026-10-10 — Six optional post-Echo dual-rite observatories
- Authored six ability pairings covering all ten canonical rites and representing distinct relationships among voluntary debts, unheard voices, lineage, identity, public access and contradictory origins.
- Implemented source-spawned post-Echo plinth, bridge, archive island and E-readable consequence record for every later realm. Mastered spells must be *successfully cast and saved*, not merely held, twice at the same physical site with distinct IDs and a persisted-clock 120-second window.
- Added optional 6-slot stage, first-rite and deadline save arrays, atomic C++17 restore, corruption/time-reversal defenses, world-save rollback and unique permanent branch state without reward farming. Local NPCs acknowledge a real public archive only in their own realm.
- Added portable layout invariants, C++17/Unreal automation tests and manual engine acceptance checklist. Engine build, physical traversal, VFX/audio and encounter balance are outstanding. Volume VII is still the last fully rendered illustrated edition.

## 2026-10-10 — six authored guardian encounters, nonviolent alternatives
- Introduced six unique optional guardian profiles and a deterministic time-telegraphed enemy behavior model, reusing authentic combat damage, guard and interruption rather than an unavoidable instant strike.
- Guardians spawn on each upper realm terrace after its Echo decision. Both a deliberate twice-confirmed merciful local pact and actual combat defeat produce exclusive saved 1/2 guardian outcomes; old SaveGames default to unresolved and corrupt states fail closed. Failed writes never silently mark the guardian finished.
- Mastered Understanding Bosses can briefly expose a nearby guardian to interruption without granting automatic pacification. Local witnesses comment on the aftermath; the player cannot farm the fight or coerce a peaceful choice without prior real testimony.
- Added native rule tests, source integration checks, Unreal test source, manual realm-by-realm QA and full background/production direction. Actual UHT/UE build, camera/animation, VFX and real combat reachability still require a Windows PC. Volume VII remains the last verified illustrated artifact.

## 2026-10-10 — Volume VIII publication: The World Learns to Answer
- The 73-page Volume VIII illustrated development Bible expands the merged source-world lore with six post-Echo dual-rite observatories, six optional guardian life histories and combat/mercy alternatives, and a repeatable, scoped eight-minute Windows UE smoke demonstration.
- Rendered the editable Word to PDF and visually checked the new pages, preserving 43 embedded images and all 30 prior atlas artworks. Produced a 38-entry ZIP with SHA-256 manifest, independently verifying every included digest and ZIP integrity.
- Gameplay source references: merged PR #13 `72792b910f4ef661307c1a62c77814ba3df8e1db` and PR #14 `1e7da798ce1acb3a72aecb6258a27b722ece38b2`, both already source-CI-green on main.
- These binaries exist as conversation downloads only. No final 3D models, runtime Unreal screenshots, recorded dialogue, Windows UHT compile or play-tested combat sequences are claimed.

## 2026-10-10 — Final answer, false victory and transformed continuation
- Implemented an authored optional final enemy, **Nhal-Vey**, requiring only the first Auvren story. First health-bar defeat saves a mask-breaking reveal; second defeats the unveiled core and requires a twice-confirmed physical choice.
- Both endings **transform the same saved nine-realm world** without changing the character's custom origin, inventory, skills, previously saved moral choices or NPC private memories. Spawned branch-specific public artifacts across nine places, local community dialogue and a Reach waking-up teleport. Both branches have distinct sound/visual production designs; actual finished world remodelling, audio and cinematic implementation remain editor work.
- Added independently optional altered-reality boss echo for a voluntary rematch, with a branch-dependent learnable attack sequence; no repeatable loot/forced secondary win. Timed telegraphs and fracture interruption are source logic, not realized special-effect hitbox animations.
- New append-only schema-1 saved final state and strict restoration protect older saves, reject invalid phases/choices/memory signatures and roll back on failed writes. Source and Unreal automation tests document both outcomes and three combat forms; Windows UE compilation still pending.
- Extended detailed Nhal-Vey production Bible and first-PC acceptance, prioritizing accessible player controls, saved meaningful world differences and avoiding accidental reversal of a final choice. Latest verified illustrated art edition remains Volume VIII.

## 2026-10-10 — The Unanswered Road main-campaign continuity bridge
- Unified the existing disparate first-visit realms around the recurring **Unanswered Interval**, a missing note whose identity changes as it is manifested by water debt, shared heat, forced choir labor, erased ancestry, unentered citizens and unclaimed laws, foreshadowing Nhal-Vey's role as a suppressive answer.
- Implemented an offline deterministic five-act campaign spine with at least two alternate first-visit routes at each branching tier, optional extra lore and no nine-marker fetch-loop. Reads original region/faction/landmark completion and existing FinalJourney for up-to-date source-authoritative progress; adds **no save schema**.
- Added E-inspectable authored plot markers in all nine established realm grayboxes, an explicit next-step/actual atlas attunement route in the player journal, and home-region public NPC dialogue after their specific first story resolves.
- The initial Nhal-Vey fight now respects contiguous main plot progression while old saves already in the final sequence remain accessible. Separate optional Echo chapters, ten-ability observatories and guardians remain optional and unmodified.
- Added portable route and gating tests, Python source checks and Unreal automation source; the detailed scene Bible and Win64 source-to-runtime QA acknowledge untested visuals, navmesh, actors, animation and UI.

## 2026-10-10 — Volume IX cinematic and production-recipe milestone
- Authored a self-contained 149.5-second, 16-shot moving **concept-art** explainer using the earlier illustrated atlas. Delivered a scored/captioned version and a temporary offline-voice version with timecoded shot IDs, editable SRT, sound direction and explicit not-gameplay disclaimer.
- Expanded the existing 73-page Volume VIII Bible without replacing it. Volume IX includes an explainable video-storyboard, nine realm-by-realm practical implementation chapters, ten bounded ability verbs and ethical constraints, NPC knowledge/evidence routines, post-victory consequences and a source-to-Unreal importer plan. Original 43 embedded illustrations retained and four production stills appended.
- Committed canonical `Authoring/cinematic_explainer_v1.json` (public spoiler policy) and `Authoring/cinematic_realm_playbook.json` (mechanisms, quests, hazards, NPCs, optional combat, both outcome variants, named assets and QA for every Realm enum).
- Added deterministic Python export to two `UDataTable`-ready CSV files, proper Blueprint row declarations, source validity tests and early CI + Windows-first-PC drift prevention. No altered gameplay SaveGame schema.
- Art and synthesized voice are scratch preproduction; actual UE level Sequencer, physical art, recorded character performances and PC packaging are still required. Detailed handoff and explicit first-PC QA live in `docs/design/cinematic-volume-ix-unreal-handoff.md` and `docs/qa/cinematic-realm-production-acceptance.md`.

## 2026-10-10 — 54 exact main quest steps, 82 personal return encounters, 52-actor Unreal staging
- Added complete main-campaign step contracts for nine actual region first-story loops (six steps each, including two irreversible outcome variants) and Nhal-Vey's four-stage conclusion. Exact named dialogue, real/placeholder actor contracts, evidence and mechanism requirements, save-failure recovery and local consequences are first-class stable-authoring data. Seven act gates match existing branching quest logic without adding fake required collectibles.
- Implemented offline, safe C++17 `ResidentContinuity` tracking original 82 residents, with at-most-once-per-day talk progression and personally remembered aid. Gameplay E/OfferAid now call actual save-backed resident actions, and returning actors can speak one of 82 individual authored lines. The existing direct-witness versus rumor memory rules retain authority.
- Added optional version1 SaveGame resident arrays with strict order and length validation, fail-closed mutation guard and rollback after failed writes. Old slots are compatible. Indexed order cannot be reordered without deliberate migration.
- Added canonical 82-person JSON, deterministic C++ dialogue data header, 54-step/82-person Unreal DataTable CSV outputs and a non-colliding 52-actor Editor-only staging manifest. First-PC dry-run works without Unreal; explicit apply guards the dedicated `Unmade_AuthoringStaging` map and leaves existing stage actors untouched on rerun.
- Added native and Python tests; CI and PC bootstrap must validate all generated products against source tags and resident registry before accepting any build. Full UHT compile, DataTable import, real 3D scenes, UMG dialogue branching and entire alternate nine-realm cinematics remain PC-dependent.
