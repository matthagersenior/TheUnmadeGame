# THE UNMADE — Illustrated Lore Continuity Log

## 2026-10-22 — Six provisional native creature habitats respond to player movement

- Previously, three enemies occupied each of six bounded optional arenas, but the arenas had no individual creature homes or inspectable territory. A source-authored habitat layer supplies distinct prototype den/roost/undercroft/cave geometry while retaining the six existing encounter stable IDs and creature assignments.
- Sightline-aware approach reports tracks or warnings; a placed glow rises with disturbance and drops behind physical cover. Pressing E at a separate inspection marker releases a shelter alcove shutter. This is optional environmental feedback with no extra combat skills, gear, money, XP, permanent quest state, or change to the prior field-survey latch.
- The new placement details remain **visual and behavioral proposals**, not canon certification of origins or additional invented creatures. All inspected flags are transient and the source-level tests are not proof of Unreal compilation or actual collision/playability.
- The existing illustrated v20 edition, historic release binaries and recorded SHA-256 hashes remain unchanged.


## 2026-10-21 — Verified backup copy into an unused save profile
- A separate, two-press-confirmed Session action can copy a structurally validated old backup into an actually unoccupied numbered profile. The source primary and source backup are never written, deleted or migrated.
- The copied record is validated on disk before selecting the new profile for normal saving; the action refuses if all destination slots are occupied. Unlike merely loading the backup, the player can now resume saving without overwriting an incompatible source record.
- Source-only native slot-selection checks and UI path wiring tests cover preservation and confirmation; actual Unreal storage failure, race conditions, disk corruption and UI playtest require first-PC acceptance.


## 2026-10-21 — User-selected backup takes precedence over unreadable primary
When a player deliberately selects a validated backup profile, the loader now honors that selection even if the primary save is incompatible. Ordinary loading still refuses to silently replace or overwrite an incompatible primary. This corrects the previously tested source recovery policy without migrating saves, discarding primary bytes or changing any canonical journey/quest state. Actual Unreal backup UI and corrupted-file playback remain to be tested.


## 2026-10-21 — Stable resident memory survives off-screen save cycles
- Citizen-memory saves now preserve records belonging to named residents who are not currently spawned or streamed, updating only the same stable ID when a resident is actually present.
- Saved NPC observations are validated against their original GUID, resident stable ID, bounded 64-observation memory, unique event records, evidence type and named speaker for rumors before any profile load or write.
- The memory update prepares a validated copy rather than clearing the saved list; malformed gossip refuses the write and leaves prior resident records intact. Direct witnessing cannot be invented from gossip.
- This changes no quest outcomes, faction values, witness consent or authored resident identities. Unreal streaming/playthrough and storage-failure behavior still need real-engine acceptance.


## 2026-10-21 — Cross-realm finite warehouse, wage and cargo save integrity
- Added a source-only preflight that reuses the existing nine-realm MaterialWorld and LivingEconomy validators *together*, using the original saved in-world clock. Partial warehouse/worker array snapshots, illegal negative stock, nonconserved municipal payroll and simultaneous incompatible carried cargo are rejected before restore or overwrite.
- Accepted schema-1 saves with the optional snapshots absent continue using their previous defaults. This adds no resources, wages, shipments, item grants, quest progression, or new canonical IDs. Real Unreal restore, power-loss and crash recovery remain unverified.


## 2026-10-21 — Prove earned expedition gear and physical Ability Study before restoring profiles
- Strengthened the existing source-only save preflight to call the authoritative nine-realm `Expeditions::Restore` before accepting packed expedition progress or selected realm rewards. In-range equipment slots alone no longer permit forged reward ownership.
- Saved Observation → Understanding → Experimentation → Mastery must pass the existing `AbilityStudy::Restore` requirements, including firsthand evidence, an independent signal and paid experiments, before the profile is made available.
- No new abilities, rewards, locations, save-schema fields or quest resolutions were invented. Older schema-1 profiles with absent optional expedition/research flags remain valid. Unreal runtime persistence and full-playthrough acceptance remain unverified.


## 2026-10-21 — Critical save preflight before restore and overwrite
- Existing schema-1 save fields are checked for invalid player resume values, inventory/equipment array shapes, finite world clock, economy values, one-of-nine loadout indices, witnessed echo masks and chronology of carried testimony.
- Invalid records are refused on read or before overwriting primary/backup; the previous disk records remain untouched. Disabled optional flags retain the old empty-state compatibility. Actual subsystem Restore continues to enforce deeper rules; this preflight is not a full-save migration or Unreal-verified disk-fault test.


## 2026-10-21 — Transaction-safe player healing
- Potions now stage health recovery before the combined profile save captures the combatant, not afterward. A synchronous failed save restores the exact prior combatant (hit receipts/cooldowns included) and potion inventory; it cannot silently promote the next potion in the preference list.
- Zero-health-restore Strain consumables still write a synchronous combined save; the already existing fracture recovery follows that successful save. No new item, species, ending, save field or lore answer was introduced.
- Native rollback tests and source wiring checks cover these rules, but Unreal asset builds, staged engine gameplay and actual disk-fault recovery require later UE hardware acceptance.


## 2026-10-20 — Fallback character silhouettes
- Added source-defined but visually distinct fallback proportions/crest profiles for five roles and twelve creature species. No new inhabitants or enemies were spawned.
- The existing game-collision capsules, AI, combat profile, witness memory, skeletal assets and narrative decisions are unchanged. An imported rig hides the fallback rather than showing both.
- This is source-only; hardware/lighting/accessibility, actual model quality and Unreal compilation remain unverified.


## 2026-10-20 — Native visual source port without recovery branch ancestry
- Ported nine distinctive crown motifs to current Unreal source and spawned three purely decorative noncolliding silhouettes per expedition frontier.
- Neither original cube collision owners nor player decisions, witness, journal, gear, map discoveries or save fields were replaced.
- Old PR #110 remains available for review; it targets an obsolete draft and was not merged or used to resurrect the recovery branch.
- Added C++ source rules and Python wiring regression checks; no actual UE5.8 rendering or animation QA was performed.


## 2026-10-20 — Threefold HUD source wiring
- Added a compact optional witness-route cue under the existing selected Threefold expedition objective, keeping tracked realm choice and full F9 evidence journal intact.
- Nothing automatically reveals objects, overrides saved choices, grants gear or changes the story. Other realms retain their original tracker and the v20 illustrated package has not been rebuilt.
- Native/source CI is not Unreal gameplay QA; confirm small-screen wrapping, readable controls and correct source-state text in the real Editor.


## 2026-10-20 — Source-only guided Threefold witness journey
- Expose one next-action prompt from canonical witnessed evidence, saved braid decision, player-held letter, valid next-day Hessa reply and second physical delivery to Orrel.
- Preserve all three clue ID gates, four distinct moral choice combinations and save-failure protections. No unvisited evidence, refuge names or sealed answers appear in the guidance.
- This is a new player-readable hint and source test, not a completed map, UI screen, cinematic, Unreal-compiled feature or new illustrated release.


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


## 2026-10-10 — Preproduction lived-universe atlas, beyond the quest markers
- Preserved nine canonical realm enum IDs, original main-campaign branches, 82 registered NPC identities, ten disciplines, final boss optional route and the two postboss world variants.
- Added Authoring/lived_universe_atlas.json as a non-authoritative detailed society, climate, ecology, labor, cultural customs and mystery master: 27 physical evidence objects, 18 resident routines, nine optional five-objective/dual-outcome civic quests, nine comparative inter-realm evidence links and 36 specified asset packages.
- Generated 90 distinct reference-only Unreal work orders; implemented a deterministic standard-library Python validator, unit tests and CI/first-PC drift checks. New GUIDEs have no collision or gameplay authority and are explicitly marked unbuilt.
- Updated canonical continuity and design guidance. The existing illustrated Volume IX remains the most recent rendered package; no DOCX/PDF/atlas/ZIP regeneration, Unreal Editor compile or new gameplay implementation is claimed here.


## 2026-10-10 — Cinema director edition v2 / sound and nine-realm scope
- Preserved the 16-shot spoiler-light Volume IX public film and its existing Unreal editor DataTable contract. Did **not** replace or modify those canonical shots.
- Authored a separate 56-scene, seven-chapter, 12:15.03 spoiler-inclusive director cut to explain nine settlements, 82 individual source resident identities, ten disciplines, optional lived-world and Echo content, Nhal-Vey's false first victory, both postboss worlds and voluntary echo.
- Validated rendered MP4 has 48 kHz two-channel AAC for the entire 735.03-second runtime, with audible temporary synthesized narration and scratch music; the source has a deterministic scope index plus a CLI gate that verifies real delivered media when supplied. Media, subtitles and stem downloads remain conversation production artifacts, **not checked-in Unreal assets or playable engine proof**.
- Source documentation and audio QA updated. The legacy illustrated Volume IX book/art ZIP has not automatically been regenerated. This new editorial edition remains conceptual until actor, animation, professional voice/music and Unreal Sequencer production.


## 2026-10-10 — Volume X: The Unanswered Atlas / primary storyworld canon
- Added a long-form literary Volume X with 11 recognizable canonical settlements, 13 source-registered leads, 9 deliberately open mysteries, 13 multi-use callbacks, original dramatic scenes, a 6-episode seasonal outline and 4 independent proposed expansion narratives.
- Added Authoring/transmedia_storyworld_v1.json and strict actor/city/mystery/callback validity gates. All original realm IDs, 82 resident identities, story save gates, customizable protagonist and two valid postboss morning variants are preserved.
- Distinguishes authored objective/source facts from attributed rumors, disputed recollections, open mysteries and adaptation-only staging. Mystery answers need an explicit later canon update; no convenient retcon is permitted.
- Updated editorial release policy, CI and Windows preflight. No previous illustrated DOCX/PDF was automatically regenerated, and no literary scene is claimed to be a finished Unreal asset or completed novel/series.


## 2026-10-10 — Volume X Witness Echoes: first physical canon callback implementation
- Added three independent source-spawned physical markers for Hessa's uncounted cup (Bellwold, CALL.01), Sevrin's two-faced press (Paperhaven, CALL.03), and Orrel's inkless nail (The Crossings, CALL.04). All three use existing proximate E interaction and authored writing, not distant omniscient auto-unlock.
- Implemented a bounded C++17 `WitnessEchoLedger`: firsthand observation and one later meaning after matching local committed consequence, without farming, hidden realm progression gates or asserted cosmic resolution.
- Appended two optional save masks and a presence bit to the existing schema-1 slot; added restore validation, corruption preservation and failed-save rollback of new and old landmark state; exposed discovered/returned evidence in prototype journal.
- Added portable C++17 gameplay-state test to CI plus source wiring/first-PC preflight tests; engine-native compile, world navigation, gamepad accessibility, authored levels and spoken character performances await first Unreal PC.
- Existing 10-volume literary assets and dated illustrated DOCX/PDF/ZIP package have not been automatically rebuilt for this new source implementation.

## 2026-10-10 — 11-city authoring layout factory across all nine realms
- Authored eight distinct cinematic/civic silhouette guides for each of the eleven existing cities, with 44 reference road strips and 88 housing proxy shapes.
- Added noncolliding source-linked actor guides for all 82 saved resident IDs, ten original mastered rite IDs, 54 source-authored main quest steps, 27 physical evidence contracts and nine still-unimplemented side-story concepts; complete isolated planning map has 402 reference actors.
- Connected the third safe editor map and source validator to the first-PC command and CI. The prototype's actual quest/saves, stage gates, NPC identity continuity, ending alternatives and nine OPEN mysteries remain unchanged.
- Technical publication update required: these are authored build instructions and markers, **not** fully built or tested Unreal cities, animated characters, voice performances, playable story interactions, finished user interface or shipped content.


## 2026-10-10 — Source implementation: the Witness Braid (Volume X section 17)

- Extended existing physical callback investigations across Bellwold, Paperhaven and the Crossings into one **optional** three-source puzzle. All three observations and a later changed reading must be earned before Orrel's bridge nail allows a conclusion; a single lore menu, off-screen speculation or repeated input cannot skip the route.
- Authored separate human-scale outcomes: **Sheltered Thread** (private refuge cord) or **Public Docket** (redacted public discrepancy record). They create different physically spawned structures and journal text, reuse two-press commitment, and never resolve the nine OPEN mysteries or require a particular protagonist history.
- Added optional old-save-safe outcome persistence, strict prerequisite validation, fail-closed rejection of corrupted snapshots, rollback when saving fails, non-farmable commitment and no new compulsory boss or quest gate.
- Updated primary Volume X Markdown canon, living continuity, native C++ regression and source wiring checks. Original illustrated complete edition is still a dated asset, not falsely marked republished. UE compile/PIE and actual embodied content still pending.


## 2026-10-10 — The Living Evidence milestone (Volume X Chapter 18)

- Added firsthand-only, source-attributed evidence notebook API with F9 player activation and a BlueprintPure readout for future UMG. Three established physical clues retain stable IDs; the journal never prints an unvisited secret.
- Distinct sheltered/public Witness Braid events are witnessed only by physically nearby residents with clear sight. Existing local one-hop social transfer marks later listeners as *rumor*, preserves the named origin and cannot promote gossip into observation. Per-person fear/trust and context-specific authored lines change only when memory has the source.
- Added no new hidden ending, no undisclosed protected person's identity, no global auto-knowledge and no mandatory quest gate. Existing optional schema-1 evidence and Braid state remain the source of truth. Added offline C++17 and Python source-wiring tests to CI and the first Windows PC preflight.
- Editorial primary source Volume X Chapter 18 and cumulative illustrated Bible explain the resident theories, authorial ambiguity and current implementation status. Real Unreal Editor test and physical long-distance courier delivery remain clearly pending.


## 2026-10-10 — The Folded Dispatch (Volume X Chapter 19)

- Made the optional Witness Braid's consequence **carriable**: an E-inspectable folded Crossings packet exists only after the choice, and the player transports it physically rather than triggering a global message.
- Added a named Hessa-only E handover in Bellwold; her response depends on the stored Sheltered Thread or Public Docket only after direct receipt. Other NPCs are never silently told the document's contents.
- Added optional custody stage + collection/receipt day SaveGame fields; malformed or paradoxical snapshots block save overwrites, and failed real writes restore the previous stage. No rewards or main-quest gates.
- Authored Volume X Chapter 19 and source tests (native and Python) along with first-PC preflight. The written future courier/ambush sequence is separate from today's graybox, and real Windows UE5/physical QA remain pending.


## 2026-10-10 — The Return of the Witness (Volume X Chapter 20)

- Implemented optional next-day Hessa reply with 2 safeguarded F7/F8 dispositions; original Folded Dispatch must be physically delivered first, and the response cannot be chosen on that same world day.
- Added separate in-world E-pickup near Hessa and E-delivery to Orrel, saved custody days and a real named recipient. Failed writes roll back. Uncollected answers cannot be delivered or duplicated.
- Authored four response intersections (original Sheltered Thread/Public Docket crossed with private counsel/public hearing), plus mutually exclusive low/tall return markers after genuine Orrel receipt.
- Wired source + native unit and source-integration regression tests into CI and the first-PC preflight. Section 20 remains non-omniscient and refuses to make Kesta's future physical delivery an already implemented feature.
- **Not yet Unreal compiled/playtested**: visual collision, E priority, F7/F8 interaction, companion animations, full dialogue UI and in-person voice. Complete cumulative Bible must be republished separately and retains all earlier volumes.


## 2026-10-11 — Reality Consequence Engine: first Crossings vertical slice (Volume X §21)

- Connected ten already-authored Tenfold rites to one deterministic local world-clock consequence system. Casts affect real tagged world-space graybox structure and actual enemy pursuit speed, beyond prior disconnected spell presentation; old Tenfold strain and cooldown remain authoritative.
- Added separate source-limited local resident observations for caring vs interrogating alterations, with direct-vs-named-rumor dialogue, individual trust/fear and no remote telepathy. Only two different mastered and personally witnessed rites support a lasting civic record; after evidence in both directions, the city holds both protective and inquiring physical markers.
- Added optional schema-1 save fields for distinct witnessed rite bitsets and temporary effect expiry, fail-closed bad snapshots and in-memory rollback on failed persistence. Portable native and Python source-contract tests plus first-PC preflight added; no new forced main quest, lore answer, or XP loop.
- Updated authoritative Volume X section 21, living continuity and design specification. Complete illustrated PDF/DOCX remain a separately rendered publication and must be refreshed as part of release; green source CI does not represent a successful Unreal Editor compile or full nine-realm gameplay.


## 2026-10-11 — The Nine-Realm Reaction System (Reality Consequence Engine v2)

- Extended v1 from one bounded Crossings projection to nine region-specific policies with materially distinct local lore/physics: rain accounting, heat sharing, future debt, load-bearing song, quarry lineage, protected identities, roots of unwritten law and inconsistent origin geometry. The Crossings old save remains untouched.
- Added eight independently validated optional schema-1 civic histories with distinct mastered/witnessed rite evidence, active world-clock effects, fail-closed corrupted arrays and full-state rollback. Each realm's physical proof is represented with temporary collidable geometry and independent care/inquiry/contested civic structures.
- **Real mechanics:** actual local enemy movement changes, actual frontier/later-realm hazard injury and strain modifications; only the Crossings’ existing buy flow gets a bounded real price effect. Do not describe the other worlds' institutional dependency names as completed resource production or trade simulation.
- Source-limited local NPC memory events now identify the realm and use unique eyewitness or named-source rumor lines. Remote residents are not omniscient; old 82-resident identities and original choice outcomes remain.
- Added strict native C++17 nine-policy, isolation, expiry and restore tests, Python source wiring and first-PC preflight. Volume X Chapter 22 and detailed design are canonical. Windows Unreal/UHT/PIE, collision/balance, full 3D buildings and cinematic production remain separate unverified gates.


## 2026-10-11 — Local Material Consequences (Reality Engine v3, Volume X §23)

- Introduced nine distinct, finite physical settlement materials with warehouse, household, production worker count, daily need, automatic counted local haulage, persistent shortages and damage; each realm's unit is named and non-interchangeable.
- Added actual E interactions for withdrawing one finite five-unit relief pack, in-person same-realm delivery, and repair that consumes local stock. Both custody and repairs save atomically; failed writes roll back. No no-cost resource spawning, inter-realm item teleportation, duplicate packs or unlimited repair farming.
- Linked actual previously witnessed civic Shared Passage to one additional daily worker in that realm. Named NPCs with local roles shift schedules toward real shortages, supplies, homes and damage; their authored biographies and dangerous-event priority remain intact.
- Added local journal, visible shortage/damage/repaired actors, optional old-schema-save-safe 9-town ledger, corrupted-array write guard, portable test and first-PC source-wiring validation. The Volume X cumulative manuscript now includes Chapter 23; illustrated PDF/DOCX publication is a separate distribution step.
- Not yet complete: UE5 UHT/PIE test, authored full 3D economy levels, individual worker employment contracts, cross-world trading, equipment inventory goods, cinematic voice and complex economic quest branching.


## 2026-10-11 — The Living Economy (Reality Consequence Engine v4, Volume X Chapter 24)

- Connected nine *existing*, source-stable residents to separate owned wages, specific homes, finite 32-mark employer treasuries and accrued payment obligations (2 marks per actual day); preserving player marks, communal stock and all pre-v4 saves.
- Added two on-foot inter-realm cargo contracts: Saltwake water casks to Cinderhold; Cinderhold ember baskets to Saltwake. Five local units must really leave the originating warehouse; six destination marks are escrowed; only correct E-receipt pays the source town and records foreign typed inventory and a named receiving worker. A future shortage can consume imported support, never turn casks into fuel.
- Prevented simultaneous relief/trade parcels, wrong-recipient deposits and forged currency/receipts; saved all balances, wage debt, foreign stock, consumed aid and carried manifest atomically with corruption guards and rollback on disk write failure.
- Returning worker NPCs acknowledge genuinely recorded pay, unpaid wages and receipt histories. Bidirectional delivered aid produces optional Nhal-Vey mask counter-testimony, without bypassing any boss combat or outcome.
- Authored Volume X §24 and rigorous domain/source tests. Full NPC wage contracts, dynamic shops, trade taxes, escort AI, multi-tenant families and UE5 Windows Editor/PIE/cinematic QA remain pending.


## 2026-10-11 — Lives Behind the Ledger (Reality Consequence Engine v5, Volume X §25)

- First source-implemented named **individual home**: Bes at Cinderhold, with real collidable structure, private finite warmth, one unit consumed per actual day, days cold and authored return lines. His complaint is only available after the v4 real municipal pay ledger owes at least two marks.
- Investigation is not a fake branch: hear Bes, inspect his own hearth, corroborate with Rheva; only then choose physical relief (real carried Cinderhold pack), true walkout (one fewer next-day city worker and no invented shift/salary), or exact funded municipal restitution. Relief does not mark a wage claim paid.
- Optional named Saltwake guarantor Mera personally consents, then the player travels to a real guarantee station. One bounded owned-wage transfer to Bes preserves money and creates a new accountable claim on Cinderhold **owed to Mera**, not debt erasure. Full restitution must preflight all obligations to prevent partially succeeding on blocked payment.
- Additive schema-1 household warmth/workday/phase/report/guarantor save records and corruption protection; both existing economy ledgers and household roll back on failed disk writes. NPC routines and real house/civic marker state respond; no global personal gossip.
- Nhal-Vey's first mask-reveal has differentiated callback for municipal restitution, Mera's personal sacrifice or an unresolved refusal; the existing combat and endings remain mandatory. Native domain tests, source-connection tests, updated cumulative illustrated Bible and explicit Windows UE acceptance remain required release steps.


## 2026-10-11 — The Unpaid Shift — Directed Quest (Volume X §26)

- Implemented deterministic 13-scene/39-beat short screenplay: real canonical Bes/Rheva/Ovenna/Mera speaker IDs, subtitle text, timed camera/actor-blocking cues, source-labeled Foley and voice direction, Blueprint-delegated sequential playback, skip and offline caption fallback. No synthetic claim that a missing voice asset is narration.
- Replaced the giant inaccessible Bes.Home cube with physical floor/partial walls and real doorway, inspectable hearth, bed, warmth store, paid-work desk, missed-wage document, low stool and door lintel; all work as currently graybox source props. Added a moving visible Cinderhold cart and coal that freezes during the saved named worker's work refusal.
- Added first E press for explicit consequence and numerical cost preview, second E within 12 seconds at the same physical choice location for commit. The original save-transaction and one-pack/civic-currency/family-consent rules remain in full force. Investigation itself remains immediate and reversible.
- New optional exact timed return scenes for Bes and Mera depend on real, locally learned facts and are throttled, and the endgame mask scene is selected from saved restitution, Mera claim or unpaid state without auto-combat shortcuts.
- Native/source CI checks confirm storyboard completeness, choice UI wiring, enterable building geometry and unchanged world-save contracts. Separate pending Windows UE 5.8 UHT/PIE/Sequencer/audio/actor-playback/render quality and actual accessibility acceptance.


## 2026-10-11 — The Audible Shift (Volume X Chapter 27)

- Created original, deterministic and independently checked 35-cue stereo WAV Foley build from the authoritative 39-shot screenplay, using native Python with no external downloads or synthetic imitation of human actors. WAVs are reproducible files on build; no fake final voice.
- Wired the Unreal timed screenplay to real optional SoundWave playback at canonical import paths, preserving fallback captions when SoundWaves are absent.
- Added a real non-modal UMG speaker/subtitle lower-third anchored within the image with automatic wrapping, contrast and skip clearing. Added UMG/Slate dependency and source/UI regression tests.
- Extended Windows quickstart and direct-PC handoff to generate/check Foley and guard imported SoundWave assets from overwriting authored work; run sound validation under CI.
- Preserved v1–v6 saves, economic transaction authority, 39 screenplay beats and the optional Nhal-Vey branches. Still outstanding: UE/UHT/PIE, recorded voice acting, synchronized lip motion, Foley spatial mixing, tested high-contrast/gamepad accessibility and actual finished cinematics.


## 2026-10-11 — Runtime v8: playable-directed cameras and six encounter grounds
- Wired sequential Unpaid Shift subtitles and Foley to an actual camera rig, range-checked named speakers, differentiated shot lenses, and player skip restoring the original view. Nhal-Vey boss-memory beats remain on the gameplay camera.
- Spawned six explorable source-runtime ruins outside civilian roads, with obstacles, label and light, plus eighteen named foes reusing the actual attack/guard/fracture systems and two existing enemy attack styles.
- Added source regression tests and updated living canon in the same gameplay commit. This is neither a regenerated downloadable illustrated volume nor an Unreal Editor/Windows playtest; final assets, animation, navigation and VO remain distinct production work.
- Source API compatibility review: use the public APointLight::PointLightComponent property in the lighted encounter blockouts; no gameplay or save-state contract changed.
- Added six named field scouts with physically present inherited memory/response interactions and four labeled E interaction stations for the Bes/Mera wage dispute. These complement, not replace, the original resident population.


## 2026-10-11 — Runtime v9: live quest instructions inside the world
- Created a non-modal native UMG objective and verified-state ledger overlay for The Unpaid Shift. Its route and current goal change when Bes is heard, his home is inspected, Rheva documents unpaid wages, Mera consents, or settlement occurs. Local radius and cinematic hiding prevent intrusive omniscient information.
- Wired bounded refresh from the existing prototype hub Tick without creating new quest flags, duplicating evidence or consuming player input. Added regression source tests and updated the canonical continuity text. Unreal UHT/PIE and new illustrated Bible edition are still unverified.


## 2026-10-11 — Publication v9 repaired, full illustrated binaries issued
- Rebuilt the real 132-page master as a 137-page cumulative book and the original 67-page Volume X as 74 pages, preserving all previous chapters, embedded images (52/5 previously) and adding one schematic figure each (53/6 now).
- Published the two Word masters and two PDFs to the user's private file Library, plus Chapter 28/29 source, atlas artwork and 22-entry ZIP (every member CRC checked, 21 manifest content hashes independently verified).
- Visual QA: page PNG renders, 0 blank PDF pages and 0 out-of-bounds text blocks. The site schematic is not an Unreal gameplay image. GitHub receipts link Library locations, not invented public release files.
- Updated the full Volume X editorial source with Chapters 28–29 and strengthened the workflow gate so source commits must also update the release receipt and SHA manifest, not just lore notes. Unreal compilation remains outstanding.


## 2026-10-11 — Performance and Peril v10: three rig families, six environments and cinematic gameplay
- Authored original, offline-generation glTF 2.0 skinned resident (7 joints), beast (7 joints), wraith (5 joints) with five keyed named motion clips per model (15 total) and six PBR material source swatches, plus a playable-asset import acceptance guide. The skinned models are EDITABLE source, not cooked Unreal uassets.
- Source-wired optional skeletal animation for player, residents and enemies with safe static graybox fallback; resident moving and speaking clip selection and physical temporary scene blocking with clean skip restoration.
- Expanded six previously generic expedition grounds with 18 distinct source-runtime primitive-mesh landmark placements (ribs, prisms, bells, archive folios, quay piles and kiln stacks), collision choices and optional per-site materials. Older scenery, scout names and save states remain intact.
- Switched enemy movement from straight swept root offsets to bounded NavigationSystem waypoints and CharacterMovement collision; if no navmesh exists, only line-of-sight local movement is allowed. Added pre-attack warning timing, player counterplay by sight-breaking, impact cues and health hit indication without bypassing original guard/fracture/cooldown rules.
- Regenerated the COMPLETE illustrated master (140-page PDF/DOCX, 54 images) and standalone Volume X (79 pages, 7 images), adding Chapter 30 and a new technical plate; visually inspected final spreads and checked zero blank/off-page blocks. Issued the 31-entry CRC/SHA-verified bundle and a separate rig source ZIP in the user's Library at `/The Unmade/Illustrated Bible Editions/2026-10-11 Performance and Peril v10`.
- Work remains unverified in Unreal Editor/UHT/PIE, especially GLTF import paths/scale, skeletal playback, generated NavMesh bounds, occluded camera, player combat balance, content authoring and whole-quest playtesting. The v10 files are a real editorial/software source release, not a completed executable.


## 2026-10-11 Paths and Predators v11
Twelve named creature profiles replace site aliases; six elder variants rally below half health. Six saved, physical latch-and-scout surveys open covered passages. Fold interrupts a pending strike with recovery. Existing campaigns, money, consent, mysteries and endings are unchanged. Source-only validation; Unreal UHT/PIE remains outstanding. Complete illustrated books rebuilt from v10.


## 2026-10-11 Cinderhold and the Remembered Road v12

Implemented all six requested development priorities within one optional connected adventure: specific relationship memories, interactive environmental evidence and gates, territorial creatures, an authored Blackslag elder with locked tells and a living alternative, grounded ability combinations and warned permanent choices, and intersecting household/fieldwork/return objectives. Added Volume X Chapter 32 and engine acceptance plan. Exact wages, warmth and Mera's consent remain separate authoritative domains; no OPEN mystery or ending was canonized.

Four new three-beat directed scenes and three actual original prototype stereo effects accompany the source; no final voice recording, bespoke skeletal clip or verified Unreal import is claimed. Versioned dependent snapshot validation, atomic paid route persistence, failed-save rollback, actual living elder checks, earned local reports and historical return values are tested. The v11 Blackslag survey remains compatible and yields to Varda after its existing report. The illustrated master and standalone volume are extended from the intact full v11 editions, retaining prior artwork. See current illustrated receipt/digests for delivered files and explicit engine-validation gap.


## 2026 10 11 World of Remembered Roads v13

Implemented optional physical delivery adventures in all nine realms and eleven settlements; all 82 registered residents and six scouts retain stable IDs. Forty-four finite sealed units, carried custody, eleven local mechanism/elder/route branches, dated receiving shelves, witnessed reports and personal home returns. 87 new graybox homes reuse Bes's original household. 22 directed scenes and bounded creature forage/patrol/rest hooks. Existing campaign access, wages, fuel, inventory, consent, endings and OPEN mysteries retain authority. Unreal compilation and playtesting deferred by explicit creator instruction. See Chapter 33 and the v13 acceptance walkthrough.


## 11 October 2026 Frontier Adventures v14

Source implementation adds nine combat/exploration arcs connecting existing cities, eighteen new settlements and six-chamber dungeons. Forty-five new residents, seventy-two enemies, nine named bosses, rescue/evidence branches, 18 alternative equipment rewards and nine passive techniques persist with physical return consequences. Existing 88 people, campaign access, wages, fuel, consent and OPEN mysteries retain authority. Total settlement coverage is 29. Boss surrender permits post-boss physical releases without invented rescues. NPC declaration repair removes a malformed earlier function signature. Full cumulative books and package are rebuilt. Unreal compilation and playtesting remain deferred by the creator.


## Combat presentation and world identity v15 — 11 October 2026

All nine expedition realms now have a distinct boss signature footprint, a named coupled-valve exploration mechanism, optional guard hunt/survey/road restoration objectives and sanctuary recovery. Return paths show damaged or repaired posts, a refuge resident returning to road-watch duties and remembered rescue/repair dialogue. Surrendered bosses remain visibly alive. Three original prototype rigs now contain 51 keyed clips; 36 new original procedural sound sources accompany optional runtime animation/audio hooks. An independent versioned 46-integer identity save validates expedition/day dependencies and rolls back failed writes. No original endings or OPEN mysteries are resolved; no inventory IDs, wages or consent rules change. This is source implementation and portable prototype asset production; Unreal import, compilation and engine acceptance are deferred. Details: `docs/design/world-identity-v15-acceptance.md`.


## Adventure depth v16 — 11 October 2026

Nine dungeon layouts and optional sequence, tide, heat, weight, height, stealth and mirror challenges now accompany nine personal frontier accords with actual named witnesses, physical evidence, confirmed choices and return dialogue. Provisioner/crafting/training stations cover all 29 settlements with finite shared regional favor/stock. Three mastery levels and selected active equipment techniques accompany distinct weapon styles, stamina, combos, dodge, parry, stagger, status and sentinel support. A native UMG journal/equipment/map/conversation interface replaces the converted debug paths while preserving diagnostic fallbacks. Three rigs now contain 57 clips. The optional 110-integer depth save rejects invalid dependencies and rolls back failed writes. The v15 duplicate guard declaration and revival receipt reset are corrected. Original people/item IDs, wages, consent, campaign endings and OPEN mysteries retain authority. All changes are source implemented; Unreal compilation/import and engine acceptance remain deferred. See `docs/design/adventure-depth-v16-acceptance.md`.

Public-witness accords additionally create nine conditional named return-road claimants, one per realm. Actual combat defeat commits a non-respawning outcome and two local favor; failed writes retry without inventing victory. Protected-reserve accords avoid this encounter. New total content adds nine conditional opponents to the existing expedition roster. This does not select a campaign ending.

### 12 October 2026 UTC Player experience v17

Implemented the seven approved source upgrades and both integration corrections: unified independent equipment slots; bounded melee contact and traveling ranged attacks; nine optional authored wings/guardians; 36 day-aware scenes for existing expedition residents; all29 service rooms/hours/specialties and visible repair/occupation/access changes; finite insight/build/tool/binding/privilege rewards; discovery-gated maps, tracked objectives, settings and core rebinding. Existing stories, 133 registered residents, item IDs, ten disciplines, wages/consent, player endings and OPEN mysteries remain. NPC scenes require the real speaker; Crossings guidance respects refusal. Mediation adds access without undoing the accord. Hall-entry waypoints remain collision-safe, never teleporting residents. Complete primary Volume X extends Chapter37. Portable validation and separate cumulative Bible/VolumeX publication receipt accompany this milestone; Unreal compilation, imports, native rendering and playtesting remain deferred. No finished navmesh, voices, environmental art, executable or animation graph is claimed.

### 2026-10-12 — Nine Realm Journeys v18

Closed journal equipment-owner compile error, early-encounter receipt gap, missing reward/fieldwork tracker transitions and unseen return witnesses. Added directional route signs, transient confirmation for irreversible branch/equipment choices, and Drevlach-specific yield validation. No new canonical answers or wage/consent changes. Full illustrated books extend v17 intact; engine verification remains deferred.


## Complete RPG Loops v19 12 October 2026

Source implemented: deed-gated free mastery across nine realms; kits restricted to danger benefit; four independent non-destructive profiles with prior-record backup and safe-settlement health/stamina resume; optional fieldwork profile migration; 27 controller-accessible actual actions on seven menu pages; selected six workshop and nine earned field recipes, five trade goods; mapped prompts and signs; deferred distant primitive blocks, cached actors and distance cadence independent of the clock; distinct boss signatures/warnings and timed service gestures. Existing IDs, supplies, consent, wages, endings and OPEN mysteries remain authoritative.

Second audit fixed isolated fieldwork saves, stale profile confirmation and automatic crafting. Repeat with `python Scripts/audit_gameplay_dependencies.py --check --report audit.json`. Unreal compilation, imports, storage interruption, focus, navigation, timing and complete walkthroughs remain deferred. See Chapter 39 and `docs/design/complete-rpg-loops-v19-acceptance.md`.


## Player Facing Reliability v20 12 October 2026

Six follow-up audit fixes: scoped, visibly explained backup recovery without overwriting its target; single-pass binding labels; unobstructed alive/consenting merchant checks in both trade directions; native chosen-name and eight-aspect Character page; held stagger/death priority; production-rule behavioral audit coverage. Added actual Toggle Sneak access, eight menu pages and 28 complete action callbacks. A second inspection fixed feedback hidden behind the open menu. Existing saved IDs, wages, consent, protagonist origin, rewards and OPEN mysteries remain authoritative. Identity art remains prototype; no final face, hair, voice or mark assets claimed.

Repeat `python Scripts/audit_gameplay_dependencies.py --check --report audit.json`. Engine compilation, imports, storage interruption, controller focus/platform keyboard, collision, clip/audio timing and complete walkthroughs remain deferred. Chapter 40 and `docs/design/player-facing-reliability-v20-acceptance.md` record the source scope.

## 2026-10-19 — Four authored Ability Study subjects and earned Atlas (source-only)

- Added a four-subject, evidence- and encounter-gated Observation → Understanding → two physical Experiments → Mastery loop, with save integrity, actual Strain costs and earned tactical synergies, not generic automatic enemy skill copying.
- Added native UI buttons, keyboard inputs, journal and physical telegraph/strike inspection. The true Echo Well is the only provenance-checked origin for the Memory Lens.
- Gave the previously definition-only Atlas of Absence a one-time, place-bound, mastery-gated reward and distinct +Fold/-Glimpse equipment effects; existing equipment/item ID order was not changed.
- Added the portable C++ regression executable and integrated source parity checks. The 186-entry Codex distinguishes real active effects from observations and stat-only equipment.
- Updated living canon and release receipts. The **v20 illustrated artifacts have not been regenerated**, and their original SHA-256 file digests are preserved. Unreal Header Tool compilation, save-load and combat tests inside the Editor are unverified.

## 2026-10-19 — Tomorrow's Debt award and witnessed Auvren route (source-only)
- Reconciled Tenfold rite reward with existing `FutureReceipt` award: **Tomorrow's Paid Receipt**, while the quest remains *The Ledger of Borrowed Dawn* and the debt cost continues to apply.
- Main-story witnessed first chapters now provide level 1/2/3/4 atlas attunement for opening/frontier/middle/upper, independent of optional mythic gear, without making skipped chapters count as completed. The same level is used by journal route advice and actual portals, with legacy relic shortcuts preserved.
- Added portable regression coverage for alternative branches, absent evidence, and relic shortcuts. Saved character identities, NPC witness rules, and endings are not overwritten. Actual Unreal Editor travel/save-load testing remains outstanding.
- The v20 illustrated artifacts and original SHA-256 receipts are unchanged, not regenerated or rehashed.

## 2026-10-19 — Lore-audited Atlas requirement checklist (source pass)

- Required first-hand Echo Well inspection, Memory Lens and two separate enemy masteries before the Atlas may be claimed.
- Added the pre-existing Cartography mastery, physical route verification, and double-rite First Absence witnessed resolution to the Atlas reward gate; no new lore shortcut or automatic power absorption.
- Added authoritative save-derived `[x] / [ ]` requirements for each study technique and the Atlas, displayed in the real Actions page and evidence notebook.
- Tightened physical field experiments to require fresh enemy telegraphs; remained save-backed and Strain-priced.
- Added portable C++ and wiring tests for missing evidence, confluence, verified route, place, duplicate reward, and displayed checklist. No Unreal compilation or regenerated illustrated release claimed.

## 2026-10-19 — Unified 243-record Codex and generated player checklists

- Unified all 186 previously indexed entries with 9 realm specifications, 9 expedition journeys, 9 optional experience wings, 9 personal arcs, 9 campaign chapters, 9 aftermath narratives, and 3 settlements in the **existing** Codex.
- Added typed workshop recipe costs/workbench/skill/output contracts and explicit realm/journey dependency references without inventing any saved rewards or quest outcomes.
- Replaced manually authored Study and Atlas checklist words with Codex-generated C++ reference tables. A reproducible generator supplies nine realm summaries; CI checks for drift.
- Preserved existing runtime models, active player saves, provenance and consent rules; full engine runtime data migration and Unreal compilation remain unverified. No illustrated artifact was regenerated.

## 2026-10-19 — Generated workshop recipe rule source

- Replaced the six manually maintained runtime workshop recipe table rows with a deterministic table generated directly from the 243-record canonical Codex.
- Preserved RecipeId and ItemId numbering, ingredient amounts, skill levels, settlement workbenches, output counts and existing atomic save/economy behavior.
- Generator checks both Study/Atlas/realm reference tables and the compiled crafting recipe table. Full Unreal compilation and recipe playtesting remain unverified; no illustrated release was rebuilt.

## 2026-10-19 — Codex-owned realm and passage topology

- Added 13 append-only named route dependencies to the existing Codex (now 256 records); preserved all nine original realm names, statuses, conflicts, and attunement tiers.
- Moved executable WorldAtlas and AtlasPassages tables to deterministic generated `UnmadeCodexAtlasGenerated.inc`, consumed by the original path-planning runtime.
- Added source, JSON and compiled-C++ tests to detect incorrect edge thresholds, invalid endpoints, shortcut routes and drift between Codex and world descriptions.
- Existing realm scenes are not claimed to exist without an actual Unreal level; v20 illustrated files were not rebuilt.

## 2026-10-19 — Codex-generated expedition registry

- Migrated the nine executable expedition specification records from independently maintained C++ tables to generated entries in the unified Codex (256 total records).
- Preserved exact original NPCs, clue text, boss identities, realm positioning and combat/mercy return narration; did not rewrite quest stages or saved outcomes.
- Added C++ tests and source-parity contracts for all nine expedition rules; no Unreal Editor compile or illustrated-release rebuild claimed.

## 2026-10-19 — Nine optional experience wings become Codex-authored runtime tables

- Moved all nine optional wing gameplay specification tables, including three physical room coordinates each, all task instructions and their NPC witness/choice-dependent reactions, into the existing Codex.
- Added a generated C++ table, source/JSON parity tests and a portable realm-wing geometry test. Preserved saved indices, physical chapter gates and already-authored social consequences.
- No Unreal compilation, v20 illustrated file regeneration, or fabricated available levels claimed.

### 2026-10-19 — Generated wing table compiler correction

- Fixed the C++ generator's excess aggregate braces for each three-room grid, preserving all existing coordinates and original scene text. Portable regression compilation verifies the generated source.

## 2026-10-19 — Main-story campaign chapter parity

- Migrated the nine canonical Unanswered Road chapter descriptions, physical discoveries and next-road guidance to the existing Codex and generated `UnmadeCodexCampaignGenerated.inc` consumed by C++.
- Added C++ and JSON tests for nine ordered chapters, source event IDs and truthful mystery evidence.
- Preserved the original physically earned campaign, branch alternatives, final-boss gate, save contract and illustrated v20 artifacts.

## 2026-10-19 — Nine authored personal-choice definitions now Codex-owned

- Ported complete witness/physical-evidence/two-choice/lasting aftermath/secret/counterparty texts into the nine existing personal quest Codex entries and generated `UnmadeCodexPersonalQuestsGenerated.inc` consumed by runtime C++.
- Converted the older adventure-depth builder into a Codex-versus-historical-manuscript compatibility verifier, retaining original dungeon positions.
- Added regression tests for the nine saved ordinals and mutually exclusive social outcomes. No earned milestones, engine build claims or illustrated release bytes were altered.

## 2026-10-19 — Realm aftermath rules become Codex-generated

- Moved all nine realm aftermath source contracts—mechanism sites, attributed witnesses, mutually exclusive evidence/outcome branches and future local world changes—into canonical Codex records and a compiled generated C++ table.
- Added compiled portable and Codex-validator regression checks for differing consequences, real witnesses and the truth of level availability.
- Preserved old game saves, original physical quest gates and narrative choice freedom; no new illustrated assets or Engine builds claimed.

## 2026-10-22 — Campaign journal text and old-save reconstruction

- Added exact native snapshots for campaign journal titles, questions, objectives, lead choices and boss access across each act transition.
- Reconstructs guidance from existing schema-1 evidence and final-journey fields without adding a journal save field or writing a migration.
- No canon event, ending, mystery answer, quest reward or completion state changed. Unreal rendering and disk deserialization remain unverified; the historical v20 illustrated package was not regenerated.

## 2026-10-22 — Realm skyline identity profiles

- Added nine source-authored three-piece orientation profiles to distinguish the existing realm skyline silhouettes.
- Kept decorative meshes noncolliding and outside overlap, navigation and gameplay state.
- No canon facts, IDs, rewards, save fields or illustrated assets changed. Native checks pass; Unreal rendering remains unverified.

## 2026-10-23 — Nearby study sources and expedition visual studies

- The Study journal now shows visible nearby living creatures, the Echo Well, and named rite witnesses as local learning leads; its established firsthand, payment and rite requirements still govern progress.
- Added proposed boss and lair visual studies keyed to the nine authored expedition arcs without asserting final level layouts or gameplay behavior.
- Native tests and reproducible 68-model GLB generation pass. The historical v20 illustrated edition and hashes remain unchanged; no Unreal compile or in-engine visual review is claimed.
