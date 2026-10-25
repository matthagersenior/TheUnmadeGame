Warning: truncated output (original token count: 38879)
Total output lines: 1513

## 2026-10-24 — Spatial Foley sources for directed Unpaid Shift beats

Loaded generated Foley now uses the local staged resident as its source when
available; stage-only beats originate in front of the player. The component
uses spherical attenuation and destroys itself after playback. Existing lower
third captions still carry the cue description independently of sound playback.
The spatial presentation remains a prototype pending Unreal Editor audio review.

## 2026-10-24 — Keep cinematic captions within narrow viewports

- Removed the fixed 280-pixel lower width bound; the panel now uses 86% of the
  live viewport for both narrow and wide windows.
- Updated source layout contracts. Very small-device font readability and safe
  area rendering still require Unreal acceptance.

## 2026-10-24 — Content-sized accessible cinematic captions

- Replaced the fixed-height lower-third region with a viewport-width-constrained
  size box that grows to the wrapped speaker, dialogue and Foley rows.
- The width updates after viewport resizes; source checks cover the layout
  policy. Portrait/small-screen safe areas and extreme-scale rendering still
  need Unreal acceptance.

## 2026-10-24 — Live accessibility settings for cinematic captions

- Applied the existing saved text scale to speaker, dialogue and Foley rows,
  refreshing while captions are active; the high-contrast option changes the
  lower-third backing to black.
- Added source checks for both preferences. Extreme-scale wrapping and backing
  readability still need verification in Unreal on PC.

## 2026-10-24 — Caption-aware timing for directed scenes

- Calculated a minimum beat duration from the visible speaker, subtitle and
  descriptive sound caption at 180 words per minute plus a one-second settling
  interval, retaining each authored duration as a floor.
- Shared the resulting duration across actor blocking, Blueprint event,
  caption fallback and scene advance. Source contracts cover the timing path;
  actual voice synchronization and Unreal UI reading tests remain pending.

## 2026-10-24 — Non-speech captions for the Unpaid Shift

- Added a canonical descriptive caption for each of the 35 Foley cues used by
  the directed scenes. Captions are shown with dialogue in a separate quieter
  lower-third row even when the associated audio asset is absent.
- Verified cue coverage with source-contract tests, the full Python suite and
  a C++17 portable scene-rules compile. Unreal UHT/PIE and in-engine caption
  readability remain pending.

## 2026-10-17 — Checked reality costs, crossing arrivals and revival

- Combined direct reality-action Strain, existing language clue and local
  witnesses before effects. Preserved prior transient windows on failed writes.
- Combined arrival/visits/directly verified roads, retained saved resume points,
  and kept realm fallbacks local across all nine realm identities.
- Saved movement/revival with bounded retries, protected failed movement rollback,
  and kept recovery available with pending/full-inventory rewards without
  declaring their encounter receipts complete. No authored choices reset.
- Evidence: 218 portable fault paths, 1,744 reloads, UBSan and eight compiled
  mutations. Native movement, collision, UI, serialization and full realm loops
  are unverified; no charges, new art/book requirement or merge into main.

## 2026-10-17 — Combined action consequences across related systems

- Combined the originating saves with existing witness/deed effects across the
  six resolution families, village completion, aid, discovery, conflict/supplies,
  rite/oath choices, cast witnesses and six Confluence conclusions.
- Bound resolution to its exact revalidated preview; failed saving cannot use
  the confirmation to attempt another family. Authored gates remain unchanged.
- Repair old split aid from direct history/receipts without new testimony;
  distinguish rumor and reopen earned-reward retries after startup.
- Evidence: 128 action fault paths, 1,536 reloads, eight compiled mutations,
  UBSan and portable/static checks. Added native memory-buffer rollback automation
  is unrun; all Unreal acceptance remains pending. No new save IDs, authored
  story content, paid services, artwork requirements or merge into main.

## 2026-10-17 — Chronicle consequence retention and oath intent

- Separated threats from deliberate restitution; kept the existing scar and
  Bellwold/faction/deed/firsthand-witness gates.
- Retained failed deed, verified-road and involuntary oath writes for retry.
  Every accepted combined save captures live Chronicle/Confluence alongside
  world memories. Both oath-choice bonuses clear on break; other buffs persist.
- Explicit oath choices roll back on failed writes with feedback. Portable
  evidence: 270 history fault paths, 3,240 reloads, four chosen-oath write faults,
  UBSan and seven compiled mutation regressions. Unreal verification is pending.
- Original world-action-to-consequence callback windows remain the next audit;
  no claim of atomicity across that earlier boundary. No authored content,
  stable save IDs, lore mystery, consent or optional illustration policy changed.

## 2026-10-17 — Combat reward and witness-memory retries

Melee and projectile impacts already share the same reward hook. Failed ordinary
first-Stalker/first-Watcher rewards now stay in a player-owned session queue,
retrying every five active-world seconds after the first attempt. Dead enemy
actors can stop ticking without losing the retry owner. Existing achievement
bits remain the durable receipts; no per-kill grants or new save identifiers.
Complete checkpoint/profile-save guards include this queue, while explicit
unsaved-discard confirmation remains available. Partial successes stay claimed;
capacity failures report the pending reward. Special realm/boss receipts retain
their existing owners and authored outcomes.

Direct witness saves now share the hub's dirty-memory flag and eight-second
quiet gossip retry. Failed writes keep observations in memory without replaying
events or generating replacement evidence IDs. Unnamed actors are excluded from
persistent witnesses; range, line of sight and firsthand/rumor provenance remain
unchanged. This does not make later legacy-deed/oath callbacks atomic with the
original action; trace those callbacks next.

Portable evidence: 345 Python tests, 79 strict C++17 executables, the new scenario
under UBSan, 48 dependency checks and four compiled mutation regressions pass. Coverage includes write
faults, recovery selection, repeated reloads, duplicate kills, partial rewards,
full inventories, checkpoint blocking, readiness and malformed NPC subrecords
without backup rotation. Static structure/literal/hygiene, six release scenarios,
transmedia, manuscript and whitespace checks pass. Actual Unreal/UHT, hit/LOS,
travel, UI feedback and interrupted platform IO remain pending. No charges,
artwork requirements or merge into main. Live main comparison before this batch:
15 commits ahead, zero behind; focused changes continue on PR #74.

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
- Preserved nine canonical realm enum IDs, original main-campaign branches, 82 registered NPC identities, ten disciplines, final boss optional route and th…18879 tokens truncated…fabricate a new local fight witness.
Named encounter defeat receipts keep their existing owners and retry policies.

Absent, defeated or unrestored players cancel transient armed enemy warnings,
patterns and locked aim. Ordinary/elder/expedition/observer/named/guardian/final
encounters must warn again on resumption; health, story outcomes, swing receipts
and failed victory retries remain. Durable defeat processing precedes the active
player guard. Final attacks use the authored rhythm damage directly, fixing
Unmasked/Echo phase bases being added twice; guard, armor and status modifiers
still apply normally. No authored pattern or balance value is changed.

Chamber/wing hazards and automatic arrivals/discoveries wait for an accepted,
restored same-world player. Construction still initializes gates, signs and lifts.
Original pulse timing, physical reach, sight, edge routes and free vents remain;
field kits require the same owner before spending and retain visit-only suppression.
Provision history cannot claim more once-per-day purchases than its last purchase
day. Reachable older and absent optional records remain accepted. Defeated
public-accord return-road threats now block complete checkpoints until their
hub-owned favor/defeat receipt saves; failed writes restore favor, explain pending
retry and grant it once. Explicit unsaved-discard and independent revival remain.

Portable evidence: recovery covers 432 write faults, 108 malformed subrecords,
216 successful recoveries and 5,184 reloads; provisions cover 108 malformed histories,
252 write faults and 3,744 reloads; attributed status covers six write faults and
768 reloads; return-road threats cover 18 faults and 432 reloads. Interruption
coverage adds 1,230 resumed warning paths, 384 authored damage/guard/armor checks
and 18 retained failed-receipt retries. Hazards cover 10,800 timing/readiness samples
and nine resumed pulses. All 380 Python tests, 93 strict portable C++17
executables and 76 dependency checks pass; 32 compiled mutations are caught.
Hygiene, six release scenarios, structure/literals, transmedia, manuscript and
whitespace checks pass. Actor adapters are statically reviewed; actual UHT/Editor,
component/actor tick order, projectiles, physical movement/rollback, warning UI and
interrupted platform IO remain pending. No Unreal playthrough or paid service.

## 2026-10-17 — Pre-PC controller binding and contextual prompts

- Preserve default controller trigger rebinding without accepting analog stick axes.
- Present the computed contextual interaction prompt rather than recomputing a
  potentially stale hint. Portable tests and source wiring cover the behavior.
- No new in-world story facts, optional quests, item quantities, rewards or
  permanent choice semantics; native UHT/UI/controller playtesting remains open.

### Pre-PC input continuation (17 October 2026)

Rebinding additionally captures high-value gamepad trigger analog events and
ignores unarmed/low-value/other-axis events. Authored outcomes, local consent,
reward IDs, lore and save schema remain unchanged. Portable press/noise tests
pass once the source gate completes; rendered controls still need Unreal testing.

## 2026-10-17 — Living encounter ecology, pre-PC source pass

- Ordinary wild enemies can navigate existing, threat-free forage/patrol
  destinations as well as return home; this corrects a movement condition
  that excluded idle behavior despite authored routines.
- Spatially nearby ordinary enemies coordinate visible warning windows;
  peers circle for openings, with boss/elder/expedition mechanics unchanged.
- Strict portable rules and actor wiring contracts are in CI. No save schema,
  dialogue, authority, reward, world identity or player choice changes.
  Real Unreal pathfinding, performance, sight and combat feel remain untested.

## 2026-10-17 — Ordinary creature stealth awareness

The existing Sneak control now feeds ordinary enemy detection: 6.5-metre front
cone and 2.5-metre near reach when crouched, four-second temporary attention
after a confirmed sighting, and solid-geometry line-of-sight requirements.
Alertness resets on invalid/unready player world. Elder, champion and boss
rules are isolated; narrative identity, rewards, testimony provenance,
consequence ledger and saves remain unchanged. Portable tests are source-only,
with Unreal perception and actual stealth routes still pending.

## 2026-10-17 — Combat, first-return and community feedback

- Twelve ordinary creature kinds gain deterministic species-level combat
  decisions and lateral cues; elite and named encounters stay independent.
- First-journey beats and nine investigation leads derive only from actual
  expedition/Depth flags. No journal-generated quest progress.
- Dual-rite challenges show next requirements for the established two real
  casts, altar and truthful witnesses.
- Household/community journal and named dialogue respond to recorded wages,
  heat, walkout and Mera's real guarantee. No fabricated reputation/loans.
- Added source handoff for 11 opening-to-first-return cinematic/subtitle
  beats. Real scenes, audio imports, actors and Unreal tests remain open.

Follow-up pre-PC check: existing attack windups keep ownership across species feint cycles, and expired paired-rite intervals prompt fresh casting. The first-return cue manifest is validated against the eleven real source-guidance titles. Audio/animation imports remain unverified.

## 2026-10-17 — Chapter 41: physical Cinderhold petition branches

- The actual Bes home book, Rheva civic ledger, separate private/public
  petition stations and next-day Bes conversation form an optional source
  gameplay loop. Each physical station requires reachability and clear sight.
- Permanent outcomes are confidential household protection or a redacted
  public hearing; both leave wage arrears, Mera's own marks, supplies and
  original quest outcomes unchanged. Two-step confirmation prevents
  accidental exclusivity.
- The records are expressed as mutually exclusive world markers and distinct
  personally attributable reactions only after actual conversation.
- New optional schema-1 fields restore existing saves to the unstarted
  petition, strictly validate chronology and testimony, and refuse writes
  if the stored branch is corrupt. Save rejection rolls back the attempt.
- Portable state and source wiring tests are CI-backed; actors/animations,
  sight, next-day visit and physical platform IO remain engine-unverified.

## 2026-10-17 — Three-realm, physically delivered Hearth Petition reply

The optional Cinderhold petition now offers a genuinely hand-carried
extension after Bes has heard the original outcome. A player must take
the copy from Cinderhold, personally deliver it to Mera in Saltwake,
return to Mera after a full following in-world day for her answer,
and personally deliver her reply to Orrel at the Crossings.

Local physical records differ for protected/private and redacted/public
branches. Mera explicitly questions both courses according to their
risks and grants **no second loan**. Orrel does not know the answer
before receiving it and does not declare a verdict. Bes's wages,
existing relief, named guests, Mera's consent, inventory and history
remain unchanged. The player's document journal may show custody,
but no unknown resident has universal or instantaneous information.

Saved history is additive, refuses impossible chronology or a different
ending from the original petition, and rolls back failed writes. There
are no new global testimony/deed flags, automatic couriers, permanent
power combinations or main-campaign gates. Real Unreal actor placement,
collision, voice, navigation, pacing and platform persistence remain
unverified.

## 2026-10-17 — Civic reverberation and the one-history question

A petition carried Cinderhold -> Saltwake -> Crossings now has independent,
locally visible consequences. Once Mera personally receives the paper,
the Saltwake civic dispute station exists. Only after Orrel physically
receives her reply does the Crossings gain a protected screened side
passage for confidential testimony OR a public redacted evidence platform.
The two optional, collidable sites are mutually exclusive and do not
override prior settlement choices, give any reward, or gate the campaign.

The **player's** campaign journal can then ask Nhal-Vey how one history
could contain protected identity and still honor a recorded obligation.
The main confrontation, other realms and all witnessed endgame outcomes
remain independent. Mera and Orrel retain their personally witnessed
custody accounts, and do NOT automatically learn later repayments in
Cinderhold. The player journal alone can show the actual later wage state.

Effects are derived solely from existing saved petition, relay and household
facts; no new schema or rumor flag has been added. Physical access,
collision, compiled Unreal code and cinematic performance remain unverified.

## 2026-10-17 — Nine realm-specific tactical boss counters

The nine existing boss encounters now have transient two-station
environmental counterplay requiring a real approach, positional order
and time-limited execution under the active boss's telegraphed attacks.
Each realm's counter uses its own physical motivation and timings.
Success exposes the *living* boss for a bounded interval, sometimes
opening temporary real cover; it cannot award an extra permanent victory,
rescue a captive, overwrite a surrender, or repeat for rewards.
Prior short grounding-lever and evidence-based mercy solutions remain.

Following an actual witnessed return, physical and journal aftermath
now distinguishes a defeated jailer with prisoners still held, prisoners
physically freed, and an enforcer who yielded alive. The nine local lines
remain grounded in each canon dungeon's actual people, records and
civic costs. Evidence and combat are alternatives with different
implications—not a disguised kill counter. The save schema, credits,
town services, Nhal-Vey fate and individual residents' knowledge do
not silently change. Source-only CI is not Unreal actor/animation QA.


## Coordinated boss support

Each active unresolved boss chamber spawns a shield sentinel (slot 11) and
an aimed marksman (slot 12) on top of its existing nine expedition enemy
slots. The sentinel's real combat guard can give its boss +4 armor in close
formation; the marksman pressures the traveler out of a static position.
Both become inactive on a boss death or evidence surrender.

Their defeat is temporary combat state: not `ExpeditionState.kills`, not
a repeatable XP/drop receipt, and not new durable quest history. The
player's normal melee and status defeat reward paths explicitly ignore
these two transient guard slots. Existing boss defeat, captive rescue,
and evidence choices remain the only source of permanent arena outcomes.


## 2026-10-17 — Equipment menu and explicit unsaved-session confirmation

- Kept the Inventory menu open during successful in-menu weapon, armor and charm cycling, while preserving gameplay shortcut access and existing saved gear rules.
- Skipped a second checkpoint write when the player deliberately confirms the same pending Load/New after failure. Preserved per-intent/per-profile scope and no-save backup recovery.
- Added red-first source tests plus a compiled portable confirmation-state test. This is source-only coverage; Unreal focus, input, travel and filesystem behavior remain unverified. No canon or save schema changes.


## 2026-10-17 — Nine-realm optional route playability pass

- Added nine distinct human-readable physical puzzle directions with per-attempt feedback for sequence resets, closed tide, time expiry, elevated controls and detected stealth attempts.
- Added live tide and kiln-time readouts to the existing native graybox route signage and interaction prompt; preserved already-earned route receipts, optionality and no-cost retries.
- Added cross-realm portable regression coverage and native wiring assertions before implementation. No new loot, save fields, permanent choices or canonical names. Actual player navigation, timing, UHT and rendered widget acceptance await Unreal.


## 2026-10-17 — Authored local fieldwork for all nine realms

- Replaced the generic cloned hunt, survey and roadpost task text with nine local descriptions tied to existing dungeon guards, three records, coupled mechanisms and witnessed return repairs.
- Generated the native registry from the single JSON authoring source and connected the real service signs and interaction responses to those fields.
- Added local journal tracking for actual saved hunt, hazard-map and 0–3 repaired-post progress; no new save schema, rewards or characters.
- Red-first source tests and source CI validate authored uniqueness and wiring. Engine build, physical readability and full nine-realm runs remain unverified.

- Corrected generated-registry drift after the first CI gate: the generator is the authority, and the native header contains no manual annotations.


## 2026-10-17 — Realm-shaped expedition danger fields

- Reused the nine already-authored boss warning shapes for each realm's optional third-room hazard footprint and readable, realm-specific safe-space directions.
- Kept the warning rhythm, finite vent suppression, existing field-kit and survey-map protection, damage amounts and no new persistence.
- Added failing-first C++ geometry and native wiring regression tests; source compilation/CI is the verification gate, not Unreal playability certification.


## 2026-10-17 — Return-dialogue provenance and memory continuity

- Found that later-realm daily voice switching reassigned the complete dialogue string after saved visits and physical delivery replies were composed, erasing those memories.
- Found the expedition-person shortcut returned immediately and skipped firsthand observations and later civic changes.
- Selected day/night authored voice before saved memories, joined actual expedition remarks without early return, and limited personal familiarity to the 82 registered resident IDs.
- Added regression checks before implementation. Actor rendering and speech timing remain unverified in Unreal; no save changes, extra testimony or new relationship credit.


## 2026-10-17 — Nine local, branch-specific witnessed return accounts

- Added two concrete observable accord aftereffects for each of nine realms in the canonical personal-story authoring source.
- Regenerated DepthSpec and appended the appropriate result to returned companions only when the saved accord exists; unreleased captives do not acquire knowledge of an unvisited outcome.
- Preserved leaders and named witnesses' private firsthand lines, existing choice gates, journal/reward receipts, and all original save fields.
- Added red-first nine-realm source and portable C++ coverage. Engine dialogue delivery, physical return and rendered cinematic context remain unverified.


## 2026-10-17 — Physical public accord marker parity

- Created one return-road civic result marker beside existing service markers in each of the nine realms and connected its displayed branch to the canonical public-return text.
- Gated visibility on the actual saved accord, witnessed expedition return and accepted quest/expedition state; hidden markers make no claim about pending choices.
- Source-only wiring and continuity checks exercise the nine-site count, branch selection and read-only effect; no new persistence, loot or requirements. Unreal physical placement and legibility remain unverified.

- Clarified direct canonical registry binding in the actor after a failing source check; keeps a single authoring authority for all eighteen public aftereffects.


## 2026-10-17 — Nine return-road work shifts and physical accord inspections

- Added deterministic two-branch daytime duty locations in all nine realms. Existing NPC roles leave their normal tasks only during nonurgent routine motion after the saved choice, witnessed return and rescue proof.
- Included guarded night/invalid-save behavior, distinct branch destinations and original swept navigation; no NPC spawn, forced position, invented contacts or auto-completed task.
- Connected the existing public accord notice to the physical E interaction, with branch-specific next steps based on live claimant/waypost receipts.
- Red-first Python/native C++ checks cover all nine sites, both choices, rescue restrictions and native wiring. Save schema unchanged. Unreal acceptance remains pending.

- Regression reproduced former captive duty locations outside the built-in 2000-unit home recovery radius. Shifted each sheltered-service worksite within a stricter 1850-unit envelope; retained nine distinct local positions and the original rescue/home protection.


## 2026-10-17 — Nine branch-specific worksite scenes and authored work cues

- Added 18 nonblocking graybox civic scene structures (two designs in each realm), only the saved public or private branch appearing after a witnessed return.
- Physical E inspection reports the existing local branch outcome without creating a save action, reward or remote eyewitness.
- Connected arriving daytime NPCs to the existing Repair versus Inspect animation clips, only during a legitimate routine shift; emergency behavior, night returns and existing earned services retain precedence.
- Red-first source tests gate all nine scene families, hiding, interaction, performance and no-reward boundaries; Unreal acceptance for asset imports, placement and timing remains pending.


## 2026-10-17 — Real second-stage sanctuary and road-watch NPC consequences

- Added per-realm save-backed second-shift positions to the existing deterministic accord planner: sanctuary only after three recorded waypost repairs; road watch only after the claimant's recorded defeat.
- Updated nine physical NPC routing behaviors, branch-specific motion cues, public notice and personal journal to distinguish unresolved duty from earned follow-through.
- Preserved no-save fallback, no captive teleport, night behavior and all original rewards/schemas, with a new red-first portable C++ test covering nine realms, two original branches, second-stage uniqueness and the strict home safety envelope.
- Pending Unreal acceptance for physical movement, performance, day-night transition and rendered sign accessibility.


## 2026-10-18 — Nine-realm physical custody and receiving-chief consequence

- Added an optional nine-city adjacent-realm delivery cycle, with nine independent persisted stages (waiting, player carrying, personally delivered); closing route Auvren to Crossings.
- Source-side pickup requires the genuine saved personal accord, witnessed return, and all three restored return wayposts. Destination delivery requires E interaction with the real chief and a registered destination visit.
- Built visible destination receipts and daytime receiving-chief inspection routines, with names and branch-specific text revealed only after actual handoff. Existing repair, hearing, and claimant aftermath remains distinct.
- Added an optional, additive schema-one payload, fail-closed restore validation and transactional save rollback; original Cinderhold→Saltwake→Crossings petition and witness dispatch systems stay intact.
- Red-first portable C++ and native wiring tests cover all nine source/destination pairs, source proof, recipient identity, save corruption/reload and non-omniscient response; no rewards or game-end gates. Unreal actor acceptance is pending.


## 2026-10-18 — Physical receiving-realm evidence comparison

- Added one inspectable comparison desk per realm, initially revealed only by a genuine personally delivered packet and updated after its receipt is compared against that destination's physically collected fourth-chamber evidence.
- The player's evidence discovery is *not* itself the chief's knowledge: a separate E interaction at the desk requires the actual named chief within speaking range and commits a durable Reviewed stage.
- Evolved the original cross-realm custody pack without changing version or size, preserving prior handoffs. Source proof, destination evidence, stage monotonicity, and failed-save rollback are enforced in both live restores and combined-save preflight.
- Changed the chief's daytime duty and attributed return dialogue only after confirmed presentation. The journal now directs players to the missing local evidence or physical review station; no reward, global witness, or forced game path.
- Red-first portable C++ plus native actor wiring regression checks cover all nine inbound relationships, review gates, attendance, save reload/tamper checks and daytime scene changes. In-engine user experience remains unverified.

- Follow-up proof guard: even after a received review stage, the NPC's completed-review station requires live saved destination evidence; portable integration test enforces the same rule.

## 2026-10-18 — Realm-specific physical evidence review Foley

- Added nine original deterministic triple-pulse review cues to the identity-audio generator; retained all thirty-six prior cue names, order and asset paths, validating forty-five generated sounds.
- Bound the nine realm IDs to exact generated asset names in cross-realm review rules and triggered the corresponding sound only after successfully saved physical evidence comparison with the actual chief. Existing audio preferences remain honored; generic inspect behavior remains for other interactions.
- Used the existing manifest-driven Unreal Editor importer and non-overwrite policy for custom audio. Prototype sounds are not claimed to be shipped, UE-imported, voiced or mastered.
- Added deterministic PCM audio, manifest and runtime trigger tests; Unreal Editor import and practical playback verification remain pending.


## 2026-10-18 — Nine-realm narrative purpose and consent audit

- Compared nine personal accord masters, main-quest act and director contracts, identity fieldwork, nine arena tactics, and the approved Impossible-origin game vision.
- Identified public-truth dialogue in Crossings, Vathless and Eillun that claimed named people or neighboring families had been disclosed without implementing individual consent. Rewrote the canonical choice/outcome and public witness prose to publish anonymous accounts, competing claims or voluntary appeal notices only; actual individual consent remains unimplemented and must not be implied.
- Added nine differentiated central questions, tangible investigation/combat steps, explicit signature reality-discipline interplay and earned forward mystery threads to the canonical personal-story authoring master and compiled DepthSpec. Surface those in the player's journal, without revealing future mysteries before earned return.
- Confirmed the core opening, frontier, middle, upper, Auvren and final gates remain authoritative, and that nine repeated carry/review packets remain optional source-level content instead of mandatory finale progression.
- Corrected the living canon's outdated assertion that later six realm NPCs have not been spawned; nine generated graybox realm sources now exist but are not engine-validated production levels.
- Added regression checks tying canonical narrative to physically authored tactical encounters, identity fieldwork, distinct choices, consent boundaries, source generation and journal gating. Remaining work: real Unreal compiler, physical playthrough, visual storytelling and arc pacing evaluations.

- Re-authored the three already implemented fieldwork activities in every realm with an explicit local conflict and player purpose, without increasing kill counts, inventing rewards or changing the actual survey, repair and save requirements. The one-time recovery ration remains unchanged.
- Shortened the tiny world-space service billboard to action labels and journal directions; detailed stakes still appear in quest interaction and journal text. Visual text legibility, timing and readability need Unreal verification.

- Corrected the compact source sign to use C++ escaped newline sequences in a single valid text literal. Full quest instructions remain in the journal and interaction feedback.


## 2026-10-18 — Nine physical discovery-to-combat consequences

- Added nine unique evidence-station interventions to active expedition boss arenas, each gated by the existing saved fourth-chamber personal evidence or three real inspected dungeon records. The original objective branch and physically active boss remain required.
- Each real E interaction creates a realm-specific temporary boss exposure and, in some realms, tangible temporary line-of-sight cover. Independent per-realm cooldowns block farming. The source never records a boss defeat, political outcome, rescue or reward.
- Kept the free grounding lever, two-position arena counter, alternate evidence surrender and ordinary fights available; original save schemas and credited personal witnesses remain authoritative.
- Added physically positioned stations with real READY, PROOF NEEDED, ARENA LOCKED, RECOVERING and RESOLVED prompts plus explicit journal tactics.
- Red-first portable C++ tests and native actor-wiring tests cover no-proof, boss-status, cooldown, nonmonotone clocks, nine distinct specs and no implicit quest resolution. Unreal Editor and playable combat acceptance remain pending.


## 2026-10-18 — Earned combat consequences propagate through nine realms

- Derived nine neighboring-frontier sentry responses from source boss defeated/yielded state, actual guard death bitmask, real rescued-person bitmask, witnessed return and saved player-carried report delivery. A destination without a real chief handoff has no inherited combat consequence.
- A killed guardian can increase the receiving jurisdiction's bounded ward-danger pulse; a living surrender can reduce it. Casualty and rescue counts moderate those values without assigning a universal morality score or violating original kill/surrender/return outcomes.
- Only a physically compared destination record tempers a provisional response. This preserves the separate local evidence review, the player's burden of proof and limited NPC knowledge.
- Added distinct destination watch names, physical readable warning signs, attributed receiving-chief dialogue and dynamic existing hazard damage. Existing chart-based field grounding and eight-second free vent still work.
- Added portable nine-realm defensive tests and native integration checks verifying no phantom report, no invalid save, no fake opponent death, no automatic rescue and no extra save/quest reward. UE pathfinding, actual hazard presentation and balance still require an engine playthrough.

- Corrected the rescued-person display and propagated combat facts to use the count of the two recorded rescue bits, not the underlying bitmask integer. Added direct all-realm assertions for zero, one and two freed people.

- Differentiated genuine low-casualty living surrender from a high-casualty capitulation: four or more recorded guard deaths produce a distinct casualty vigil rather than magically safe borders. Compared destination evidence moderates this response, without changing the authoritative record of lives lost. Added explicit nine-realm assertions for both stages.

- Added an explicit player-only BATTLE TESTIMONY preview while the report is physically in custody, counting saved guard and captive bits and warning that the receiving realm remains unchanged until hand-delivery. Regression tests guard the preview's stage and wording.

- Made the nine physical combat-response signs report whether a modified hazard is actually active or already grounded by an earned survey map/local boss resolution. Added explicit source tests so disabled fields never falsely advertise damage.


## 2026-10-18 — Fix unreachable cross-realm receiving review

- Found a native control-flow error: the kind-eight review branch sat inside the mutually exclusive kind-seven pickup branch. The genuine comparison could never trigger; the affected native C++ file also had unbalanced braces, undetected by previous source-only CI.
- Closed the pickup handler before the review handler, preserving separate stations and the atomic comparison with genuine destination evidence and chief attendance.
- Added red-first regression coverage for independent interaction scopes and balanced native source. Actual Unreal compilation and playable interaction acceptance remain outstanding.


## 2026-10-18 — Nine real witness-guided side-route assists

- Added one optional, temporary in-person route briefing per realm, offered only after the player has personally read local evidence and physically returned to its named witness.
- Added a separate side-route plan placard with live proof/briefing/route status. Using E while briefed pre-aligns one correct native control and consumes that briefing without marking the route, awarding favor or changing the save schema.
- All nine ordered, timed, weighted, elevated and stealth-dependent challenges retain their original remaining input and danger conditions. No research or NPC consultation is required to finish them normally.
- Added portable nine-realm priming tests and source wiring guards. The new transient guide cannot be mistaken for another witness's consent or knowledge; UE interactions and visual placement need engine validation.

- Corrected a source-wiring mistake found during review: the new witness-guide E prompt belongs in GetDepthPrompt, not the bool-returning TryDepthInteraction. Added an explicit method-boundary regression and preserved the actual station candidate in TryDepthInteraction.


## 2026-10-18 — Nine-realm progress-aware, spoiler-safe wayfinding

- Unified the tracked objective and the text map around a derived saved-state next-waypoint model in all nine realms.
- Direct bearings go only to already documented city/frontier/entered-chamber, earned council/boss/return-reward, and inspected personal-evidence locations; a not-yet-entered chamber uses the previous entered chamber as a search origin without disclosing the hidden room location.
- Character searches are directed to known districts, not to an unseen moving NPC's live coordinates. Uncharted optional wing locations remain indirect until the player has earned discovery.
- Labeled the core, optional personal and optional wing routes separately and preserved all actual quest progress, costs, decisions, saved records and ending conditions. No teleport, new save fields, NPC knowledge or reward.
- Added nine-realm portable progression regression tests and native journal/map integration checks. Actual in-engine compass and route readability remain unverified.

- Fixed pure-header include of the canonical expedition registry, removed a guessed wing branch room, and matched the final optional reward bearing to the real third-station claim marker. Added coordinates to nine-realm tests.


## 2026-10-18 — Nine first-discovery sensory moments

- Added nine separate non-spoiler first-fracture observations keyed to actually saved first-chamber entry from either physical E doorway interaction or actual movement into the chamber.
- A single authoritative saved room bit prevents replay across reloads. The cue and journal notice are emitted only after world-save success; failed writes, map inspection, equipment changes or returning visits do not manufacture a discovery.
- Authored nine deterministic original 1.6-second Foley stings distinct in frequency, pulse intervals and overtones; extended the existing identity-audio manifest from 45 to 54 without renaming/reordering its prior sources. Existing review-cue tests continue covering offsets 36–44.
- Preserved non-overwriting Unreal import, player audio-volume preferences and avatar control; no fabricated witness, XP, new quest gate, reward or save field.
- Red-first regression checks cover sound hashes and waveform measurements, nine non-spoiler cues, save-before-audio behavior, physical entry through both valid paths and no repeated forced animation. In-engine import, spatial sound and physical camera presentation remain unverified.


## 2026-10-18 — Nine physical first-fracture clue actors

- Added one distinct revisitable, nonblocking anomaly in each realm's first expedition chamber, grounded in the already authored first-fracture mystery and a physical follow-up action.
- Clue prompts and inspection require the player's actual room visit; their text directs players to physical local investigation rather than granting remote witness memories, quest completions, rewards or spoiler knowledge.
- Nine-realm portable and source-wiring checks verify distinct authored placements, observed/hidden state transitions and actual physical interaction.
- Native source, living-world canon, and the continuity log updated together to satisfy the project's gameplay-lore gate. Unreal geometry, actor/interaction readability and pathfinding still await in-engine acceptance.

- 2026-10-19: Cinderhold discovery-to-social consequence prototype. An entered and saved Cinderhold fracture chamber plus three Blackslag records and actual Bes/Varda conversation unlocks a communal accord **only after the living elder is quieted**. Separate E-interactable stone. Fourth route is persistent and physically opens both service gates day and night; return-road cargo inherits it only for Cinderhold. Attributed witnessed reports create local Varda/Bes/Rheva/Ovenna reactions without fake remote knowledge. Restore and combined-save guards protect against injected or corrupted route/expedition contradictions. No automatically paid wages, magic warmth, loot, boss victory, or irreversible global moral score. New portable and source guard tests; native Unreal playtest pending.

- 2026-10-19: Extended the shared-kiln social aftermath from attributed dialogue to physical daytime NPC routines for Varda, Rheva and Ovenna, gated individually by existing witnessed report bits and subordinate to emergency material duties. Portable/source regressions cover the connection; actual Unreal navigation remains unverified.

- 2026-10-19: Extend discovery/social/quest/world aftermath to the eight remaining realms. Seven previously narrative-only realm aftermaths now use the existing expedition actors, physically earned saved records and actual local witnesses, preserving two route endings and per-realm ethical contradictions. Saltwake keeps its established sluice/cistern/ledger quest, with first-chamber proof able to deepen local witnessed interpretation. Reuse existing RealmAftermathSnapshot (no new save shape); add cross-system restore checks against manufactured proof. No magic global information, free final quest, forced identity disclosure, unpaid-resource payoff or new rewards. Portable source tests precede implementation. Unreal level acceptance still necessary.

- 2026-10-19: Started seven physical civic frontier scenes with existing aftermath outcomes; reserved separate physical evidence and gate tags. Cross-system save validation checks expedition provenance; Unreal stage/playtest not claimed.

- 2026-10-19: Wired seven civic aftermaths through actual nearby NPC testimony, save-transaction rollback, real physical controls and mutually exclusive branch gates. Protected Crossings' correct event identity. NPC movement and engine acceptance still being verified.

- 2026-10-19: Wired personal realm testimony and civic work routines for seven real expedition resident identities; enhanced Saltwake's existing aftershock witnesses. No new money, role identity, save slot or remote witness authority.

- 2026-10-19: Added physical Saltwake invoice comparison station, E proximity interaction and evidence-conditional local report, retaining previous storm choice and saved records. Updated source assertions to check formatted physical tag creation rather than fictitious literal strings.

- 2026-10-19: Preserved schema-1 legacy aftermath saves without expedition subrecords, following an existing composed rite/recovery fixture failure. The current writer includes expedition state for new choices; proof-dependent physical gates/NPC shifts are suppressed whenever no actual saved expedition proof exists. Regression updated to distinguish valid legacy and invalid forged new composite records.

- 2026-10-24: Added a player-led resident conversation panel with memory, concern and firsthand-observation topics plus a bounded custom question. Deterministic offline responses use only the active resident's saved observations, distinguish firsthand knowledge from named rumor, and admit missing evidence. Optional local-model dialogue remains presentation-only; prompts and replies enter journal history. This does not yet provide individually authored multi-session conversations for all residents or Unreal-validated branch staging.
- 2026-10-24: Added 1,000 deterministic 45-day living-economy stress schedules. Wages, municipal accounts, cargo and durable receipts remained conserved across tested actions and cold restoration. This is native source simulation, not settlement pacing acceptance in Unreal.
- 2026-10-24: NPC dialogue responses now retain their requesting conversation session. Reopening the same resident starts a new session, so a late local-model reply cannot appear in the new transcript or as a stale HUD notice. One-off greetings retain their HUD notice. This changes presentation continuity only and adds no memory, quest fact or world consequence; Unreal delegate compilation and close/reopen playback remain unverified.
- 2026-10-24: Connected the existing 18 later-realm NPC dossiers' desire, fear, knowledge boundary and voice direction to optional local dialogue through generated source profiles. The model may use motives to shape voice but receives an explicit rule that the knowledge boundary limits factual claims; witnessed and rumored observations remain separate. No memory, quest, decision, or save state changed. Unreal compile and model behavior remain unverified.
- 2026-10-24: Optional local-model follow-up requests now carry at most 1,200 characters from the active conversation. The transcript is labeled untrusted dialogue and is not added to personal memory or evidence. Offline fallback and world state are unchanged. Unreal prompt behavior and multi-turn playback remain unverified.
- 2026-10-24: Focused verification passed: 4 world-authoring tests, 10 interactive-dialogue tests, 7 local-dialogue tests, and `Scripts/build_world_content.py --check`. No Unreal Editor/UHT run was available.
- 2026-10-24: Replaced box-shaped parts in the generic animated glTF archetypes with smoothly shaded rounded segments. Resident skin, linen clothing and trousers now use separate PBR surface definitions. Stable rig and animation identifiers remain unchanged; these are presentation previews and do not define canon appearance or affect NPC identity, evidence, dialogue, or gameplay.
- 2026-10-24: Added deterministic offline replies for later-realm residents' own authored desires, fears, and knowledge boundaries. These answer personal portrayal questions only; questions about what the resident remembers or witnessed still use the resident's saved, source-attributed observations. Optional local-model output and portrayal text do not create evidence or alter the world.
- 2026-10-25: Wired the authored personal concerns and next-visit hooks for all 82 stable residents into runtime data generated from the return-encounter JSON. The offline concern topic uses existing attributed observations when a concrete danger is known, otherwise it returns the resident's authored concern; follow-up prompts can surface the authored next-visit hook. No evidence, quest choice, consent, save state or world fact is created. Firsthand/rumor boundaries remain intact; Unreal conversation playback remains pending.
