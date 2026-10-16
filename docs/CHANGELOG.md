## 16 October 2026 — Profile handoff ownership and integration watch

Rechecked PR #74 against live main: base `9606fec4`, starting head `5a4575d`,
10 commits ahead and zero behind; GitHub reported clean mergeability. No sync,
merge into main, rebase or force push was needed. Keep focused commits and check
base divergence/overlapping files before publishing batches. If main moves,
inspect and verify an isolated integration before updating this feature branch;
branch housekeeping must not stop useful development without a concrete issue.

Profile selection previously persisted the target before travel and relied on
any later player registration to clear the write lock. Handoff now stages its
destination in memory, gates writes and accepts a different world in the same
game instance before persisting the selection. Destination backup recovery is
pinned in that staged selection. Travel failure restores the original profile/
recovery preference and resumes the original live owner only if it is still the
active one. Otherwise, the failed/partially initialized world stays write-locked;
an explicit fresh profile load remains available. Reentrant Load/New requests do
not redirect the pending target or allocate extra profiles. Delegate hooks are
removed on acceptance/failure, and destination notices wait for its arriving
player. No save IDs, valid older saves, consent, authored outcomes or lore change.

All 64 profile/recovery combinations pass portable ownership, failed-arrival,
source/target isolation, reentrancy, fresh-owner and backup-pinning checks. Removing
the write barrier, old-world arrival check, missing-owner lock or failed-world
guard fails regression assertions. Source checks cover the UE event/delegate and
routing; actual travel failures, delegate signature/build and BeginPlay ordering
remain Unreal acceptance tasks. Registration does not prove every actor restored.

Fresh checks: 328 Python tests, 73 warning-clean portable C++17 executables, all
36 gameplay dependency checks and the new handoff test under UBSan pass. Structure,
literals, transmedia, hygiene, six Node release cases, manuscript, lore sync and
whitespace checks pass. Prior head `5a4575d` passed GitHub source-checks. Artwork
and book builds remain optional; no paid services or main merge were used.

Next: inspect initialization/readiness of inventory and reward writes before the
player registers, particularly fresh-profile startup and recovery. Keep actual
Unreal startup, UI/controller and interrupted platform IO pending.

## 16 October 2026 — Checkpoints with pending encounter receipts

A paused-menu Save Now previously wrote a checkpoint without an encounter's
pending defeat receipt and reported success. Load/New Profile therefore skipped
the existing unsaved-discard confirmation and could lose that victory. Session
checkpoints now return pending before their writer runs. The menu explains that
Continue lets automatic retries run; Load/New keep their existing explicit
unsaved-discard confirmation. Backup recovery still never saves the current
session first. Receipt transactions themselves retain access to the normal
writer so they can finish and unblock the next checkpoint.

The read-only query includes hidden defeated expedition actors, wing guardians/
observer, road/Cinderhold elders, realm guardians and the final boss. Ordinary
foes with no durable receipt do not block saves. Final actors compare configured
phase with durable phase, so the old defeated body does not imply a defeat of the
new, unfought health bar. No progression, consent, lore or save identifier changed.

Portable tests cover 114 failed-write sessions: 12 guardian, 12 final-phase and
90 expedition cases. They check zero checkpoint writes while pending, failed
receipt rollback/recovery, target-profile isolation, explicit discard/recovery
confirmation, normal post-commit saves and later ordinary write failures. Actor
queries and widget routing have supplementary source checks; actual menu/travel
and serialization are not executed. Removing the checkpoint gate or final-phase
identity guard fails the composed assertions.

Fresh checks: 325 Python tests, 72 warning-clean portable C++17 executables, all
34 gameplay dependency checks and the new scenario under UBSan pass. Structure,
literals, transmedia, hygiene, six Node release cases, manuscript, lore sync and
whitespace checks pass. Prior head `4b65dc8` passed GitHub source-checks. Unreal
actor discovery, pause/retry timing, feedback/controller and actual IO remain
pending. No paid service, artwork or book publication was required.

Next: inspect profile handoff failure/startup and restoration of the original
profile/settings if travel cannot complete, without redirecting old live state
into the selected target. Actual travel-failure verification needs Unreal.

## 16 October 2026 — Bounded defeat-save retries

Guardian and final-boss defeat branches retried failed persistence on every
0.1-second actor tick. Both now use a shared session-only scheduler: attempt
immediately, retry failures after five active-world seconds, and stop after a
successful commit for that realm/phase. Actor/phase configuration resets the
scheduler. Existing defeated state and transaction rollback remain authoritative;
no victory, reward, health reset or next phase is invented on a failed save.
The first failure explains automatic retry and loss of unsaved progress on reload.
Other enemy receipt paths already have bounded delays and remain unchanged.

Portable coverage adds 24 guardian and 72 final-phase failed-write/retry paths,
including damaged backup/primary writes, recovery selection and suppression of
repeated commits. A minute-long outage produces 13 callback attempts rather than
601. Invalid clocks and new phases/resets are covered. Expired, changed-control,
cancelled, consumed and reloaded confirmations cannot replay prior consent; no
consent rule changed. Removing the delay or settled guard fails regression tests.

Fresh checks: 322 Python tests, 71 warning-clean portable C++17 executables and
all 32 gameplay dependency checks pass. New tests pass UBSan. Structure/literals,
transmedia, hygiene, six Node release cases, manuscript, lore synchronization and
whitespace checks pass. Prior head `ad96172` passed GitHub source-checks. Actor
execution, real IO, actual retry timing and feedback/controller flow remain
Unreal acceptance tasks. Save identifiers, lore and optional artwork are intact.

Next: audit manual saving and profile switching while a defeat receipt is still
pending, especially whether success feedback accurately describes the committed
state. Keep engine checks pending and preserve recovery-source protection.

## 16 October 2026 — Final-morning position and resonance prerequisites

The final-morning adapter previously saved the new world before relocating the
player, so immediate reload retained the old arena position. A shared portable
transaction now stages the existing return point before synchronous combined
capture. Failed movement writes nothing; failed writes restore the prior morning
and position. Success feedback/world refresh follows persistence. The real actor
movement callback and rollback still require Unreal verification.

Combined preflight checks Auvren first-chapter completion before final progression
when both optional records exist. Present Tenfold records must show the first
resonance rite mastered for a pending window, and both rites for an open archive.
Future-dated resonance windows are rejected against their saved world clock;
expired windows remain valid and restartable. Missing optional chapter/rite
sections retain their existing legacy policy. Optional Echo/guardian/observatory
quests remain unnecessary for the final encounter. No save IDs, authored choices,
consent, lore outcomes, item amounts or art requirements changed.

Portable tests cover both mornings and six memory seeds (12 paths), 216 full
reloads, 36 failed writes/moves, inventory/individual-history retention, optional
rematches, all nine public local records and 24 resonance/mastery combinations.
Expired windows restart and persist; contradictory records are rejected. A
save-before-relocation mutation fails the saved-return-position assertion.
The chapter fixture is shared by the existing 96-path scenario and final tests.

Next: bound automatic retry of failed guardian/final-defeat commits and verify
interrupted confirmations/repeated retries without duplicated progression or
misleading feedback. Engine build, world traversal, UI/controller and actual
platform serialization/interrupted IO remain pending. Artwork remains optional.

## 16 October 2026 — Echo, guardian and return consistency

Combined-save preflight now checks the existing later-realm chapter gates when
both related optional sections are present: first-chapter completion before
later-realm aftermath/Echo progress, resolved aftermath before Echo progress,
and completed Echo before guardian resolution or resonance progress. Individually
well-formed but contradictory subrecords no longer appear available or reach
backup rotation. Missing optional sections retain the existing legacy policy;
there are no new save fields, identities or synthesized story outcomes.

Generic return dialogue no longer claims shared memories of a changed morning
for strangers or first-day acquaintances. Existing individual aid, consent and
public records remain separate. Echo conversation save failures now explain that
progress is unchanged and can be retried. A mismatched Echo control points to the
matching control rather than promising testimony changes the rules do not allow.

New portable composition covers 96 six-realm initial/aftermath/Echo/guardian paths,
5,760 full reloads and 3,648 failed backup/primary-write recoveries. It preserves
independent authored choices, both guardian outcomes, two-clue testimony, explicit
confirmation, resonance, individual distinct-day returns and non-farmable endings.
Contradictory outgoing records are refused before writes; invalid primaries select
the good backup. All sixteen optional chapter-presence combinations retain load
compatibility in the final valid record. This is source/model/record evidence,
not a claim of Unreal actor execution or actual filesystem crash safety.

Next batch: trace resonance/final-morning prerequisites and public-versus-personal
NPC recognition through their adapters. Engine compilation, actor reachability,
confirmation/controller presentation and actual interrupted IO remain pending.
Illustrations remain optional; no paid services or main merge are required.

## 16 October 2026 — Portable field kits and realm returns

Earned field kits can suppress another realm's active, unresolved danger chamber.
The supplying realm retains its favor and kit ledger; local stock takes priority,
then the first stocked realm in stable order. Existing purchase prerequisites,
20/30-second local binding duration, chart/boss exclusions and free caches remain.
A failed world write restores the kit and applies no suppression; its specific
save-error feedback survives the player action. Suppression remains session-only.
No save fields, IDs, consent rules or authored consequences changed.

Portable coverage adds 81 supplier/destination transactions with failed backup
and primary writes, recovery and repeated reloads, plus both authored aftermath
endings across all nine realms combined with individual resident return ledgers.
This proves model/record composition, not actor reachability or witness knowledge.
Echo/guardian consequence composition and actual Unreal interaction/serialization
remain the next batch and engine acceptance respectively. Artwork stays optional.

## Realm-local loop audit — 16 October 2026

Added combined-record coverage across nine realms for local witnesses, mechanisms,
returns, repairs, wing progression, equipment, finite services and field recipes.
Hunt rations, sanctuary rests and local-favor provisions now save healing with
their spent receipt and roll back together on failure. Corrected the cache's
phantom kit-cost prompt and the engineering label for Tharniv's existing healing
specialty. Added the realm-local acceptance checklist; field-kit eligibility is
an identified open design gap. No lore, IDs, prices, consent or authored quest
prerequisites changed. Portable/UBSan checks pass; Unreal acceptance is pending.

## Opening-to-return save scenarios — 16 October 2026

Added composed fault/reload coverage for both civic/frontier/return routes,
once-only rewards/contracts, conserved named wages and Mera's persisted consent.
Moved existing reward eligibility into shared production rules for these tests.
Fixed untrusted wage-record arithmetic occurring before range checks. Realm
reward selection now preserves prior active slots on failure and reports saved
ownership plus a retry path instead of claiming the item was equipped. Existing
IDs, rewards, outcomes and financial rules remain unchanged. UBSan regressions
are in CI; Unreal/platform storage and playthrough checks remain pending.

## Combined save integrity — 16 October 2026

Complete-record preflight now gates loading, profile availability/selection and
backup rotation. Malformed optional ledgers cannot partially restore or feed
reward reconciliation. Recovery stays selected until a primary write succeeds;
legacy fieldwork imports cannot bypass rejection. Potion failures stop fallback
consumption and explain that recovery/costs were rolled back. Added portable
combined-record fault injection, reward reload tests and CI wiring. Valid older
schema-1 records, stable IDs and authored consequences are preserved. Unreal
compilation, platform IO and playthrough acceptance remain pending; see
[the compact backlog](qa/first-playable-progress.md).

## Optional illustrations — 16 October 2026

Removed mandatory illustrated-book publication from the gameplay source gate.
Canon and continuity co-updates remain required. Illustrations, authoring briefs,
historical editions and book builders remain available on request; actual
publication still requires truthful provenance and verified artifact hashes.

## Recovery commit correction — 16 October 2026

Potion healing and finite Tharniv healing / Saltwake Strain relief now enter the
same save as their cost. Failed writes restore staged player recovery and caller
costs. Portable fault tests cover capture order, reload values and rollback;
Unreal compilation and storage acceptance remain pending. See
[the audit and resume record](qa/first-playable-progress.md). Illustrated artifacts
were not regenerated; illustration publication is now optional.

# Nine Realm Journeys v18

Physical chamber arrival and encounter receipts now agree across nine realms. Added explicit journey objectives, route signs, confirmation previews and composed reload tests. Fixed journal equipment ownership and restricted wing surrender to Drevlach. Full illustrated v18 publication extends v17; engine acceptance is deferred.

# Development history

Historical milestones describe their dated baseline. Consult the root README for current status.

## Player Experience v17 — 12 October 2026

Timed contact and traveling shots, unified equipment, optional dungeon wings and guardians, furnished settlement services, conditional relationships, earned builds, objectives/maps, accessibility settings and input rebinding. [Acceptance](design/player-experience-v17-acceptance.md).

## Adventure Depth v16 — 11 October 2026

Nine distinct dungeon routes/challenges, personal accords, services in 29 settlements, weapon styles, stamina defenses, equipment techniques and native menus. [Acceptance](design/adventure-depth-v16-acceptance.md).

## World Identity v15 — 11 October 2026

Expanded prototype animation/audio, nine boss signatures, nine valve mechanisms and 27 optional local objectives. [Acceptance](design/world-identity-v15-acceptance.md).

# Frontier Adventures v14

Nine realm combat and exploration arcs, eighteen new inhabited settlements, nine six-room dungeons, four enemy roles, nine bosses, rescue/evidence resolutions, permanent equipment choices and learned passive techniques. Deliveries remain supporting quests. See `docs/design/frontier-adventures-v14-acceptance.md`. Unreal build and playtesting deferred.

# World of Remembered Roads v13

Implemented source now covers nine realms, eleven optional local delivery adventures and all 88 authored people (82 registered residents plus six scouts). Finite carried cargo, physical route checks, named receiving shelves, personal return memories, 87 furnished graybox homes plus Bes's existing house, 22 directed scenes and bounded creature routines.

Unreal compilation/import/PIE is deferred at the creator's request. [Walkthrough and engine acceptance](../docs/design/remembered-roads-v13-acceptance.md). [Full canonical chapter](../docs/lore/CHAPTER_33_REMEMBERED_ROADS.md). [Authored city and person registry](../Authoring/remembered_roads_v13.json).

## Cinderhold adventure v12

The optional connected milestone is now implemented in source: Bes's household and remembered return, Varda's three-clue fieldwork, Blackslag's authored Sootbound Elder with combat or learned cooling/cadence resolution, and three local route outcomes. Physical inspection and hand maintenance sit alongside Glimpse, Fold combinations and explicit two-step permanent Rewrites. Four directed caption/camera scenes and three generated prototype stereo cues are included; resident memories require actual local reports. Wage, warmth, consent and campaign authority stay with their existing systems. [Walkthrough and outstanding Unreal acceptance](../docs/design/cinderhold-adventure-v12-acceptance.md) · [Canonical Chapter 32](../docs/lore/CHAPTER_32_CINDERHOLD_ADVENTURE.md) · [Actual illustrated release receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md). Portable C++ and source tests do not prove Unreal compilation, navigation, imported voices/animation or gameplay.

## Paths and Predators v11 — 11 October 2026

Source adds twelve distinct creature profiles, six elder variants with a half-health rally, six E-operated covered passages and six save-backed local scout surveys. Fold interrupts an enemy wind-up; combat includes a recovery interval. Survey saves fail without applying geometry or objective changes, and malformed saves are preserved. No campaign, wage, inventory or final-boss shortcut is added. **Unreal UHT/PIE and real playtesting remain outstanding.** Updated complete illustrated books and package accompany this source release; see `docs/lore/LATEST_ILLUSTRATED_RELEASE.md`.

## NEW — Performance and Peril v10 (11 October 2026)

This source pass adds a **real portable skinned glTF kit**: Resident, Beast and Wraith meshes, each with five keyed motion clips; six original PBR material swatches; Unreal skeletal mesh/animation runtime lookups with safe earlier graybox fallbacks; per-site distinctive fossil, prism, bell, archive, wreck and kiln geometry; navigation-path-based enemies (local LOS movement only when nav data is missing); timed warning/impact combat, impact cues; and speaker-scoped physical cinematic blocking with obstruction-checked shots. Actual skeletal import, navigation generation and Unreal 5.8 UHT/PIE must be tested on an engine PC, not inferred from source CI.

**The complete illustrated Bible was actually rebuilt and issued alongside the code**: 140-page cumulative PDF+DOCX / 54 illustrations and 79-page standalone Volume X PDF+DOCX / 7 illustrations, with new Chapter 30. A verified 31-entry ZIP (30 independently hashed content items) includes both books, retained art/audio, reproducible builders and the model kit. Downloads are in the dated ChatGPT Library path `/The Unmade/Illustrated Bible Editions/2026-10-11 Performance and Peril v10/` and in the companion publication reply. Binary books are NOT public GitHub Release assets. [Current release record](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md) | [Complete current literary manuscript](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md) | [First PC import/QA](../docs/design/performance-and-peril-v10-first-pc-acceptance.md).

---

## CURRENT RELEASE — Playable World v9, Chapters 28–29 (2026-10-11)

**The illustrated Bible has actually been regenerated:** 137-page complete Word/PDF (53 images), 74-page standalone Volume X Word/PDF (6 images), and verified complete development ZIP. Source chapters cover the six playable-region grayboxes and twenty-four new NPC/enemy placements, plus the camera-directed Unpaid Shift scenes and live truthful quest tracker. [Release receipt + digests](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md) | [Canonical Volume X](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md) | [Publication policy](../docs/lore/UPDATE_POLICY.md).

**Downloads:** User's ChatGPT Library `/The Unmade/Illustrated Bible Editions/2026-10-11 Playable World v9`, plus the conversation's direct sandbox download links. Binary files are not committed to GitHub; no GitHub Release is claimed. Source CI green on `main` does not constitute a built or playtested Unreal executable.

---

## The Unpaid Shift — V7 The Audible Shift (2026-10-11)

The 39-beat source cinematic can now generate and load **35 actual original stereo Foley WAV cues** (32 kHz, 16 bit) instead of carrying unused sound names only. Source audio generation includes a reproducible hashed manifest and acoustic nonclipping tests; an authorized Windows Unreal import stage places missing SoundWaves under `/Game/UnmadeProduction/Audio/UnpaidShift/` without replacing custom sounds. During a beat, runtime plays the matching imported effect *if present*. **Actor speech and narration have not been recorded.** Even with no audio, a nonmodal native UMG lower-third shows the attributed speaker and wrapped timed subtitle with safe horizontal margins, and clears on skip. Existing v5/v6 wages, Bes's home, Mera's guarantee and Nhal-Vey memories remain true to their own saves.

**Updated cumulative illustrated lore Bible:** 132-page PDF and editable Word document, with all 52 original illustrations; standalone Volume X: 67-page PDF and Word, retaining five. The 35-cue WAV kit includes a 57-second listening reel; a separate offline phone/browser *storyboard* demonstrates choices and sounds using existing uncropped concept art. The complete verified conversation ZIP bundles all four books, concept images, Foley, scripts and the non-Unreal interactive storyboard. [Release receipt and precise status](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md). [Canonical Volume X Chapter 27](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md). [Windows cinematic production handoff](../docs/design/the-unpaid-shift-cinematic-v6.md).

**Remaining actual Unreal work:** UHT/editor/PIE compile, finished character and environment art, facial/hand animation, sound staging and recorded actor lines, localized accessibility and complete quest playtesting. A successful source CI is not a shipped or fully cinematic RPG.

## The Unpaid Shift — V6 Directed Playable Quest Source (2026-10-11)

The v5 save-backed Bes/Rheva/Mera wage-dispute quest now has a deterministic **13-scene, 39-beat** theatrical specification (each beat carries full subtitles, real actor IDs, shot/voice/Foley/blocking instructions and pacing). A skippable Unreal Blueprint multicast emits the beats one at a time and provides readable offline caption fallback. Existing E interactions now preview exact saved-state costs before a second E at the **same physical station within 12 seconds** commits the original resource/wage/guarantor transaction. The giant solid Bes home block has been replaced by **enterable partial-wall interior graybox** with distinct hearth, bed, material storage, pay book, stool and doorway; a nonblocking cart/coal prop actually moves during normal working time and stops when Bes's previously saved walkout is active. Source-limited returning Bes/Mera beats and earned Nhal-Vey mask callbacks are wired in without shortcuts to victory. [Authored scene rules](../Source/TheUnmadeGame/Public/World/UnmadeUnpaidShiftSceneRules.h) · [Unreal production handoff](../docs/design/unpaid-shift-v6-directed-quest-acceptance.md) · [Canonical Chapter 26](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md).

**Latest illustrated books:** full **130-page** cumulative DOCX/PDF master retains all **52** earlier illustrations, while standalone Volume X is **64 pages** with five earlier illustrations. The current conversation-download ZIP has **34 CRC-verified entries** and a manifest with **33 independently verified member hashes**, including original concept art and production/source references. [Current release receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md).

**Important:** This is a source-level cinematic gameplay vertical slice, **not** proof of a finished Unreal playable performance. Voice recordings, Foley, scored mixing, Sequencer shots, skeletal animations, professional interior models, accessible UMG and Windows Unreal Editor/UHT/PIE acceptance are still required.

## Reality Consequence Engine v5 — Lives Behind the Ledger (2026-10-11)

The first authored **individual Cinderhold household quest** connects Bes's real wages, home heat, lost workdays, family-like neighbor obligations and actual consequences visible when a player returns. Bes cannot claim missed pay unless the v4 municipal payroll actually owes him. The player must personally inspect his finite-warmth hearth and ask Rheva to corroborate the municipal ledger before a real choice: deliver a source-debited five-basket Cinderhold relief pack (warmth, **not** pay), support a **one-worker reduction in true Cinderhold production** while Bes refuses unpaid shifts (no phantom wage accrual), arrange full municipal restitution from real treasury funds, or travel to Saltwake and obtain Mera's *in-person consent* to risk up to six of **her own earned marks** in a physically recorded guarantee. The city still owes Mera; debt is not erased. Existing NPC identities, household outcomes and the distinctive Nhal-Vey first-mask callbacks are saved, with no boss victory shortcut. [Canonical Volume X Chapter 25](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md) · [Exact first Windows UE test plan](../docs/design/lives-behind-the-ledger-v5-acceptance.md).

**Current illustrated lore Bible:** a complete **127-page** cumulative Word/PDF pair retaining 52 illustrations; standalone **59-page** Volume X Word/PDF with five retained illustrations. Both were fully rendered and visually checked; the conversation release ZIP contains **32 entries and 31 independently verified SHA-256 content digests**, including all earlier art and source references. [QA/publication receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md). These are conversation-download binary documents; they are **not** falsely represented as GitHub Release assets. Source CI is green, but Windows Unreal Editor/PIE, actual 3D collision and movement, accessible choice UI, music, distinct voice performances and complete gameplay testing remain unverified production work.

## Reality Consequence Engine v4 — The Living Economy (2026-10-11)

The finite-material v3 model now has an actual **named livelihood layer**: nine existing stable residents have professions, identifiable households, finite municipal wage treasuries, earned wages and unpaid obligations. The system moves two existing marks per recorded paid shift, with **288 aggregate marks conserved** across employers, workers and signed-in-flight buyer escrow; it does not mint cash or deposit wages into player inventory. Two fully physical, E-interactable, save-persistent trade contracts link Saltwake rainwater to Cinderhold and Cinderhold embers to Saltwake. Exact five-unit warehouse exports, reserved six-mark buyer payments, destination-matched E receipts, named worker memory and separate foreign-goods support in an actual shortage are implemented. Return NPC conversations and the J ledger now expose these obligations. Two genuinely completed reciprocal receipts can add a non-victory Nhal-Vey mask-break line without changing the boss encounter. [Implementation / first PC test plan](../docs/design/living-economy-v4-first-pc-acceptance.md) · [Volume X Chapter 24](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md).

The latest **complete illustrated Bible is 125 pages with all 52 prior illustrations**, and standalone Volume X is **56 pages with its five prior illustrations**. Editable Word/PDF pairs and earlier art/source references are distributed in a 29-entry, 28-checksum-verified conversation ZIP. [Release QA receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md). **Still pending:** real Windows UE Editor/UHT/PIE compile, character voice and movement, complete person-by-person livelihoods, autonomous courier AI, finished UI/3D art and full playtesting.

## Reality Consequence Engine v3 — Local Material Consequences (2026-10-11)

Nine independent, **finite** realm-specific warehouses and household reserves now advance by the existing twenty-minute in-game day. Workers produce bounded goods, counted local haulage moves goods to homes, households consume daily, shortages worsen structural integrity and civic care evidence can add an actual next-day worker instead of free supplies. Every realm has tagged, E-interactable physical stockroom, household and repair stations. One five-unit relief pack may be carried at a time, cannot teleport across realms, and needs same-realm physical receipt. Repairs spend genuine warehouse materials. Named NPC daytime routines and journal/condition props respond to shortages and actual repair. New additive schema-1 saves preserve all nine accounts, carried inventory custody, labor/delivery records and safe rollback. [Source rules](../Source/TheUnmadeGame/Public/World/UnmadeMaterialConsequencesRules.h) · [Canonical Volume X Chapter 23](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md).

The complete cumulative illustrated lore Bible is **123 pages with 52 retained illustrations**, and standalone Volume X is **53 pages with five**. Both editable DOCX files, both PDFs, prior concept art and production references are supplied in the checksum-verified conversation ZIP (27 entries, 26 member SHA-256 checks). [Latest publication receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md). A green native/source test suite does **not** establish a completed UE Editor/PIE build, full per-person wage economy, inter-realm trading or finished animation/cinematics.

## Reality Consequence Engine v2 — Nine-Realm Reaction System (2026-10-11)

The existing **ten Tenfold rites** now project different physically and socially bounded consequences across **all nine atlas realms**, without reusing the Crossings as a universal civilization. The original Reach ledger remains authoritative; eight independently persisted optional schema-1 records prevent remote civic consensus. A supported cast changes temporary actual world-space architecture, real local stalker/watcher swept movement, existing realm-specific health/Strain hazard pulses, source-tagged direct-or-heard local NPC reactions and either or both civic structures when distinct mastered eyewitness evidence supports them. The existing Crossings market can also receive a small, time-limited actual purchase-price trust adjustment. Outer realms' detailed producer-consumer economies remain **unfinished**, not merely implied by material names. [v2 implementation and Windows UE acceptance](../docs/design/nine-realm-reaction-system-acceptance.md) · [Canonical Volume X §22](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md).

The **latest cumulative illustrated Bible is 121 pages with all 52 prior illustrations**; standalone Volume X is **50 pages with five**. Both have updated editable DOCX and PDF editions in the checksum-verified 25-entry conversation ZIP, retaining earlier art and production references. [Current publication receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md). Windows Unreal Editor/UHT/PIE compilation, finished environments, advanced AI and cinematics are **not** certified by green source CI.

## Reality Consequence Engine — source-implemented Crossings vertical slice (2026-10-11)

**Ten distinct reality disciplines now connect to one set of local consequences**: actual source-spawned and expiring physical architecture, stalker/watcher movement speed, remembered eyewitness versus named-source NPC rumors and trust/fear, and durable Shared Passage / Public Inquiry / Contested Accord structures after two distinct mastered witnessed casts. No single spell can farm a civic verdict and opposing human accounts can remain true to their evidence. Source scope is **the Crossings**, not all nine realms; existing Tenfold cooldowns/strain and main quests are unchanged. [Engineering/Unreal acceptance](../docs/design/reality-consequence-engine-vertical-slice.md) · [Volume X §21 The Law of Consequence](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md).

**Latest cumulative illustrated lore Bible:** 119 pages, 52 retained illustrations, with an updated editable DOCX and PDF. **Standalone Volume X:** 47 pages, five images. Four book files, original concept art, prior authoring references and the new chapter are available as integrity-checked conversation downloads in a 23-entry ZIP. [Publication receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md). These assets are not falsely claimed as GitHub binaries; **Windows UE 5.8 Editor/PIE remains unverified**.

## The Return of the Witness — source-implemented optional quest (2026-10-10)

Volume X §20 now closes a complete *two-way* physical testimony loop: first deliver Orrel's record to Hessa, wait for the next **in-world** morning, choose how her reply will be disclosed using guarded F7/F8 near the correct NPC, pick up her actual paper with E, and return to Orrel in person for one of **four** distinct remembered lines and a persisted private/public physical marker. The main quest remains independent, NPCs do not learn by global telepathy, and stage/day/save corruptions are rejected. This is **source-implemented, not UE5 Editor/PIE tested**. [Quest triggers and recovery](../docs/design/volume-x-return-of-witness-quest.md) · [Canonical Chapter 20](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md).

The latest fully **cumulative** illustrated Bible (117 pages, 52 existing illustrations) and standalone Volume X (44 pages, five illustrations) are available as verified PDF/DOCX downloads in the conversation, plus a 21-entry integrity-checked ZIP. [Edition receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md). Source-only CI passing does not mean the Unreal game, voices, animations or UMG interface are finished.

## A mystery that physically travels: Living Evidence and Folded Dispatch (2026-10-10)

Two new optional Volume X playable-source milestones put provenance and believable information travel into third-person interactions. **Living Evidence**: F9 opens a read-only firsthand notebook of Hessa's cup, Sevrin's press and Orrel's nail; theories remain attributed, and only actual physical visits reveal clues. Source-limited NPC testimony changes after the sheltered/public Crossings decision, where eyewitnesses and named-source rumor recipients say different things. **Folded Dispatch**: the completed Braid enables a small Crossings packet. E-collect it, travel physically to Bellwold and E-hand it to Hessa; optional same-slot state persists collection/receipt days and rejects impossible delivery, and only Hessa recognizes receipt. [Mechanics and future UMG evidence UI](../docs/design/volume-x-living-evidence-interaction.md) · [Physical letter quest](../docs/design/volume-x-folded-dispatch-quest.md).

The latest **complete cumulative illustrated Bible is 115 pages with 52 embedded images**; standalone Volume X is **41 pages with five images**. Both Word and PDF books, previous source references and checked art live in the release ZIP supplied through ChatGPT, not as binary GitHub assets. [Latest publication receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md). These remain **source-level gameplay implementations**: real Windows UE Editor compile, full actor animation, running 3D world, adaptive UI, performance/QA and game release are not yet certified.

## Volume X gameplay: The Witness Braid (10 October 2026)

A new optional, repeatable-source-driven **three-city investigation** connects Hessa's cup, Sevrin's seal and Orrel's inkless nail. The player must physically inspect all three and earn a later outcome-dependent revisit before F7/F8 can commit either a *Sheltered Thread* (private refuge guide) or a *Public Docket* (redacted civic record) at the Crossings. Both become different real, persisted graybox structures, with optional backward-compatible SaveGame fields, a two-press confirmation, rollback and portable CI regression tests. Neither alternative settles an OPEN mystery or blocks the main story. This is source-implemented, **not Windows UE 5.8 Editor/PIE verified**. [Investigation and voiced character writing](../docs/design/volume-x-witness-braid-investigation.md) · [Volume X canon](../docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md).

**Illustrated Bible release:** The complete cumulative master has been refreshed to **111 pages with all 52 embedded illustrations**; standalone Volume X is **37 pages with five images**. The validated DOCX/PDF/ZIP artifacts were delivered as separate conversation downloads, not committed as binary GitHub files. [Current release receipt](../docs/lore/LATEST_ILLUSTRATED_RELEASE.md).

## Eleven-city world-assembly factory — all nine realms (2026-10-10)

The first-PC project now has an **11-city physical planning factory** instead of stopping at two staging reference levels. Each canonical city has eight unique visual-silhouette modules and its own blueprint in `Authoring/city_assembly_kits_v1.json`, grounded in Volume X's settlement identities. `Scripts/build_city_assembly.py --check` produces **402 stable source-linked, noncolliding reference actors**: 88 landmark/civic shapes, 44 road strips, 88 housing massing blocks, 82 residents, 54 quest beats, ten abilities, 27 evidence objects and nine optional side-story plans. The guarded first-PC importer stages them in **a third isolated Unreal reference map**. These are **not real game quests, finished animated characters, completed city art, or gameplay-ready streets**; further 3D implementation and a real Windows UE host are still required. [Detailed city-by-city factory and missing-work ledger](../docs/design/nine-realm-city-assembly-factory.md).

## First Windows PC: double-click to prepare The Unmade

A guarded `START_THE_UNMADE.cmd` launcher now detects UE 5.8 and a usable Python interpreter, validates generated content, runs strict native builds/automation, and—only after those pass—invokes an Unreal Python Editor batch for seven C++-struct-backed **authoring reference DataTables (198 data rows)** and two separate staging maps (**52** quest/cinematic guides and **90** lived-world guides). It opens the Editor after passing, writes dated receipts and fails closed on missing prerequisites. This **does not build real 3D levels or ready-to-ship cinematics** and has not yet been run in Unreal on Windows. [Step-by-step first-PC guide](../docs/qa/first-pc-one-click-production.md).

## Exact main-quest specification, 82 remembered residents and reusable UE scene staging (2026-10-10)

The nine-realm main campaign is now expressed as **54 specific interaction steps**, plus four explicit Nhal-Vey finale beats and seven existing-story act gates. Each step records exact authored dialogue, physical objective, trigger, failure/recovery, two-way choice and persisted outcome without inventing a second quest SaveGame. **All 82 existing residents** now have personally saved day-separated return visits and one-time aid recognition, compiled individualized lines and new-morning/local public conversation responses. An offline Python generator emits **54-row quest** and **82-row NPC** Unreal DataTables and a stable **52-actor scene staging manifest** (36 realm and 16 cinematic references). An Unreal Editor Python importer only applies to a separate `Unmade_AuthoringStaging` level, skips existing actor labels and never replaces runtime quest controls. This is pre-PC source/test work, not finished UMG branching conversations, UE levels, voice performance or finished 3D cinematics. [Full first-PC guide](../docs/design/quest-relationships-scene-factory.md) · [Engine QA](../docs/qa/quest-relationships-scene-factory.md).

## Volume IX: cinematic explainer and production-ready nine-realm recipes (2026-10-10)

The illustrated Bible now has a **149.5-second / 16-shot moving concept-art companion** with timed narration, a temporary original synthetic score, captions and a separate source/recording script. It uses previous concept art and **is explicitly not Unreal gameplay footage**. A nine-realm JSON production binder specifies concrete traversal mechanisms, three physical actions per realm, 3+ identifiable witnesses, two persistent civic outcomes, legible hazards, ability synergies, optional guardian encounters, both changed-world presentations, QA and at least four named Unreal production assets for each realm. The reusable `Scripts/build_cinematic_handoff.py` generator emits two Unreal DataTable CSV files, validated against the canonical atlas in CI and first-PC preflight. The actual playable 3D worlds/Level Sequencer cuts remain Windows UE work. [First-PC cinematic handoff](../docs/design/cinematic-volume-ix-unreal-handoff.md) · [Technical acceptance](../docs/qa/cinematic-realm-production-acceptance.md).

## One continuous campaign: The Unanswered Road

The nine-realm adventure now has a source-wired **five-act mystery plus the Nhal-Vey confrontation and post-victory continuation**. A player finds the absent bell, follows either rain or hearth, investigates erased labor/debts, confronts the rights of impossible people, discovers Auvren's missing origin and meets Nhal-Vey. Each step has alternate legitimate quest routes rather than a mandatory nine-realm checklist. The journal gives an actual **next narrative question, local story objective, and atlas-based route with earned-attunement checks**. Nine E-inspectable world markers and local first-quest resident discoveries keep the thread visible during exploration. All progress is **derived from existing completed quests**; no new save state or artificial collectibles. [Campaign story / production Bible](../docs/design/unanswered-road-main-campaign.md) and [first-PC acceptance](../docs/qa/unanswered-road-acceptance.md). The Unreal scene is still a graybox, not an Editor-certified world.

## Nhal-Vey: a final boss that changes the same world (October 10)

A first-defeat **false victory** transitions Nhal-Vey to a more dangerous revealed form. The actual second defeat unlocks two physically confirmed world outcomes—**The Held Morning** and **The Many Mornings**—that do not reset the player's save. Nine locations reveal distinct branch-specific physical inscriptions (18 different authored records); local NPCs react to the changed sky without omniscient knowledge. An **optional** echo rematch has different attack rhythms in each version. All battle phases preserve predictable warning intervals, combat guarding and Fold interruption. This source feature is native-testable and Unreal-actor-wired, **not yet compiled, animated or balance-tested in UE**. [Nhal-Vey story and full production design](../docs/design/nhal-vey-final-battle-reality-bible.md) / [Win64 acceptance criteria](../docs/qa/final-boss-second-reality.md).

## Volume VIII illustrated canon — The World Learns to Answer

The latest illustrated development edition is **73-page Volume VIII**, preserving **43 embedded concept illustrations** and the existing **30-artwork atlas**. A checksum-verified **38-file ZIP** includes the editable DOCX, rendered/visually inspected PDF, art, production quality matrix, source appendix and release manifest. New chapters document all six optional dual-rite observatories, all six optional guardians with explicit mercy and combat paths, local witness limitations, and a deterministic eight-minute first-PC demonstration and acceptance procedure.

Source baseline: [merged observatories PR #13](https://github.com/matthagersenior/TheUnmadeGame/pull/13) (`72792b910f4ef661307c1a62c77814ba3df8e1db`) and [merged guardian PR #14](https://github.com/matthagersenior/TheUnmadeGame/pull/14) (`1e7da798ce1acb3a72aecb6258a27b722ece38b2`). Both passed post-merge GitHub native/static CI. The illustrated binaries are **conversation downloads, not checked-in GitHub gameplay assets or Unreal screenshots**.

## Six optional ethical guardian encounters (October 10)

After completing an outer realm's witnessed Echo story, a unique guardian with a learnable, telegraphed strike is source-spawned near the optional observatory. The player can either combat the guardian using normal health, guarding, Fold and an Understanding Bosses stagger, or use **two deliberate interactions** to make a grounded nonviolent pact. Both endings are saved independently, do not farm rewards and are discussed only by residents of that realm. [Full guardian histories](../docs/design/six-realm-guardians.md) and [Win64 acceptance checklist](../docs/qa/six-realm-guardians.md). Source tests pass only when CI runs; **the Unreal Editor and final encounter assets remain unverified**.

## Six dual-discipline observatories (10 October)

All ten mastered signature rites can now be paired at six **optional, post-Echo-quest physical sites**. Each pair is different, requires two successful real casts at the same upper-terrace plinth within a 120-second saved-world window, and permanently opens a bridge to a local, non-farmable archive that reflects the previously chosen moral outcome. The rules enforce no fake/duplicate casts, old-save compatibility, rollback on write failure and independent state per realm. Source-generated cube locations and local NPC reactions are wired, while **Unreal Editor compilation and physical level playtesting remain pending**. See [full authoring canon](../docs/design/realm-resonance-observatories.md) and [Win64 QA checklist](../docs/qa/realm-resonance-observatories.md).

## Volume VII illustrated canon — The World Withheld Its Answer

The current illustrated development edition is **66-page Volume VII** (DOCX and visually inspected PDF), retaining **43 embedded illustrations** and the original **30-piece concept art atlas**. Its integrity-verified **38-entry ZIP** includes a quality matrix, source appendix, release status and SHA-256 manifest. The new sections cover six optional third-return Echo Quests, nine distinct cultures, eighteen NPC identity/motivation briefs, all ten canonical ability production verbs, and a minimal-effort authoring/Unreal DataTable import workflow.

Source baseline: [merged gameplay PR #10](https://github.com/matthagersenior/TheUnmadeGame/pull/10) (`bb5a929bb5d4a4e794dfb6c55fd5899decd75c66`) and [merged authoring PR #11](https://github.com/matthagersenior/TheUnmadeGame/pull/11) (`e8c3948cdfc697c666621eab16670f7a55b7634a`). Both completed post-merge green native/static GitHub CI. The illustrated binary package is a conversation download, not a GitHub binary or an Unreal-game build. Illustrations are speculative references.

## Single-source production world pack (October 10)

The project now has a deterministic JSON → **three Unreal DataTable CSVs + 18 native NPC identities + full art/dialogue dossier** authoring pipeline, with CI drift protection and automatic verification in the first-PC preflight. It currently covers all nine realm cultures, eighteen individual later-realm characters with day/night lines and personal knowledge limits, and all ten existing abilities with production-friendly uses, limits and cues. The NPC actors read these actual source names/dialogue. [Authoring and Editor-import guide](../Authoring/README.md). **Editor import has not yet been tested**.

## Six optional third chapters: Echo Quests (October 10)

The six later realms now have a third authored return investigation each, after their existing first-visit and aftermath arcs. The player must inspect two independent material clues, speak with a matching resident witness and confirm one of two long-term civic changes on an actual world control; a visible dispatch appears in a connected realm. Every outcome saves independently and cannot be repeated for free resources. The source also repairs the prototype ground-gap and gate-wall geometry (previously overlapping); native C++ asserts protect the layout, but **Unreal Editor/PIE still must verify player collision and navigation**. See [the six Echo quest writer/implementation bible](../docs/design/echo-third-chapter-world-bible.md), [manual acceptance](../docs/qa/echo-quest-acceptance.md).

## Current illustrated source edition — Volume VI: Lands That Teach

The latest rendered canon package is a **57-page Volume VI DOCX/PDF**, retaining all 43 embedded illustrations and 30 atlas concept/schematic images, with a verified **38-entry ZIP**, provenance, source appendix, quality matrix and SHA-256 manifest. The edition documents six distinct timed injury/strain danger loops, locally shelter-seeking NPC behavior, and real-Unreal acceptance still required. **It is a conversation download, not an engine screenshot, GitHub binary, or Windows game build.**

Game-source commit: `fe849e839cf7eb3550df9711d99fc42cf50f960f` (merged PR #8; native source CI green). The full Unreal Editor compile, navigability and balance are not tested.

## Six realm-specific hazard and counterplay loops (October 10, 2026)

The six later-realm first-visit approaches now have independent rhythmic warnings, safe sidelines and distinct stakes: injury from the debt tide, quarry fractures and law roots; nonlethal reality strain from the inverted choir, involuntary census and contradictory horizon. Physical damage respects guarding; nonlethal pressure saturates and recovers. Source-timed cue geometry and locally alert resident routines are wired. Completing the realm's first intervention ends that realm's hazard, with no perpetual penalty. **The hazards are native-test-verified only, not compiled or played in Unreal.** See [six-realm hazard QA](../docs/qa/later-realm-hazards.md).

## Volume V illustrated package — Six Roads Beyond the Known
A 55-page Volume V DOCX/PDF was rendered from the Volume IV illustrated master; 30 prior concept/schematic art assets are preserved. The verified 38-file ZIP includes the editable DOCX, PDF, art atlas, quality matrix, source appendix and SHA-256 manifest. The package is a conversation download artifact, not a tracked GitHub binary or Unreal gameplay video.

Source baseline: [merged gameplay PR #6](https://github.com/matthagersenior/TheUnmadeGame/pull/6), commit `61a90178985b34dbffde5eb54a509562d7ae8c38`. The post-merge `main` static/native C++ CI succeeded; Unreal Engine compilation and playtesting remain blocked without a Windows host.

# THE UNMADE

Dark, surreal, third-person 3D RPG. You play a customizable outcast whose origin lies in an impossible version of reality. Living civilizations respond differently to fractures, and NPCs remember what they have personally witnessed or credibly heard.

**Status:** foundational Unreal Engine **source scaffold**, not a playable release. No Unreal Editor compilation, map boot, packaged Windows build, or Android streaming session has been verified yet.

## Source milestone — Saltwake and Cinderhold return visits (October 10)

The first frontier adventures now extend beyond entering an outpost and finishing its first three-witness story. After resolving that earlier story, return to Harrow in Saltwake or Rheva in Cinderhold. Each return begins with an active, distance- and sight-gated world control: the **Storm Sluice** or **Heat Vent**. Activating the mechanism disables its blocking collision barrier, allowing investigation of either the communal relief site or the concealed public record. A route-specific resident must testify before F7/F8 can commit the result near its warden.

Each of the two outcomes changes **different collision-enabled passages, walkway/platform geometry, public home-town commentary and witnessed versus rumored NPC memories**. Nine-realm snapshot restoration rejects malformed states and rolls back changes if saving fails. No rewards can be farmed from repeated decisions. The six remaining realms have named future controls in shared C++ rules but **do not yet have world actors or playable levels**.

See [frontier return QA](../docs/qa/frontier-aftermaths.md) and [physical adventure standard for all nine realms](../docs/design/2026-10-10-physical-realm-adventure-contract.md). **Unreal Editor compilation, traversal, performance and real player verification remain outstanding.**

## Deliberate irreversible story choices — source pass

All existing F7/F8 permanent outcomes (three faction chronicles, both first frontier accords, Bellwold's Second Night and the two actor-backed frontier return stories) now require two matching presses within six seconds **near the same qualified story representative**. The first press previews a concrete lasting consequence; changing target or ending restarts the warning. The native decision policy is tested without AI or network. **Unreal gameplay and accessible confirmation UI remain unverified.**

See [irreversible-choice QA](../docs/qa/choice-commitment.md).

## Custom protagonist identity source pass — October 10

The new offline character profile supports eight bounded customizable dimensions (body, face, hair, voice, palette, reality mark, gait, calling), a UTF-8 name and an unchanged common impossible origin. Native C++ checks cover invalid selection, malformed names, atomic restore and independent character state. Unreal-facing Blueprint-callable setters save optional schema-1 fields without erasing prior missions, and primitive body/hair geometry offers only an initial silhouette check. Press **C** to view the temporary text profile. **A real 3D creator screen, skinned mesh, facial art, voice performances and full controller UX remain to be built and tested on Unreal.**

See [character identity QA](../docs/qa/character-identity.md).

## The Second Night — Bellwold Afterlight

An authored return-visit chapter begins only after Bellwold's relief mission
and the Refuge Compact faction ending. Investigate either a shelter cache or
an erased census, seek Sorin or Ivera as appropriate, then return to Hessa
for a lasting ward or public monument. Nearby residents remember actual
witnessed choices; both routes grant mutually exclusive epic rewards.

See [Afterlight story](../docs/design/2026-10-09-bellwold-afterlight.md)
and [Afterlight engine QA](../docs/qa/bellwold-afterlight.md). **60 named items**
exist in source. Unreal compilation and in-engine playtesting are outstanding.

## Latest source milestone — Tenfold Chronicles and Confluence saga

**Ten authored signature mechanics, fifty five-stage quest beats, six paired-discipline challenges, and fifty-eight named equipment/material items are now represented in tested offline C++ source.** Skills make real choices about gravity, sound, witnessed bridges, alternate lives, evolving gear, roads, future debt, boss mercy, overlapping histories, oaths and truthful mapping. Outcomes persist with NPC-specific evidence and reward deduplication.

Six Confluence chambers require mastered ability combinations, a timed dual-cast and firsthand witnesses; the final chamber requires the full earlier saga. The last chamber is **not** a finished game ending. Original three villages and two frontier outposts remain graybox source, not completed levels.

See the [Tenfold quest/lore bible](../docs/design/2026-10-09-tenfold-rites-quest-bible.md) and [engine QA checklist](../docs/qa/tenfold-prototype-qa.md). **No Unreal compilation, animation, visual HUD, full quest production, complete nine-realm world, difficulty benchmark or playtime validation has occurred.** All critical logic works without AI.

## Six later realms: source graybox expansion (October 10)

Drevlach, Orravane, Vathless, Eillun, Tharniv and Auvren now have source-generated destination grounds, six physically different first-visit encounters, a real gap and one of two permanently selected bridges, a second return investigation, three source NPC witnesses apiece, and mutually exclusive civic gates. Travel follows the actual atlas edges and earned attunement; new optional save state restores progression independently. Native tests cover evidence, corruption, travel locks and endings. **This is not a finished level, Unreal-compiled game or full six-realm campaign.** See [six-realm manual acceptance](../docs/qa/later-realm-expedition.md).

## Latest illustrated edition
Volume IV, **A World Prepared to Respond**, adds the source-aligned hazard/recovery/customization/choice-confirmation chapter to the previous illustrated master. A 52-page Word/PDF and 39-file checksum-verified ZIP were generated as **conversation download artifacts**, not source-controlled game assets, Unreal screenshots or a compiled build. The canonical code/lore remains in this repository.

## Pre-PC readiness and remaining production gaps

The [maintained readiness ledger](../docs/qa/pre-pc-readiness.md) distinguishes what is verified in offline source checks from Unreal-only compilation, art and real gameplay acceptance. It also tracks source work that can be done before acquiring a PC; **an actual finished RPG is not simply waiting for hardware**.

## First-PC handoff and pre-PC source preparation (October 10, 2026)

Two new deterministic frontier dangers are wired in source: Saltwake's rain surge (16 base damage) and Cinderhold's heat pulse (22 base damage), each with a calm / visible warning / impact cadence. They affect only the marked approach while its mechanism is unresolved. Guard reduces injury, the player can retreat to a safe side route, and an operated mechanism stops later pulses. A defeated player returns after four seconds to the current realm's safe entry at 75% health, without resetting story or NPC memories. **No engine runtime, hazard balance, visuals or controller feel has been verified.**

The first Windows PC can run a **single script** to perform host preflight, compile the Unreal Editor target and execute Unreal automation, retaining diagnostic logs. The project still needs authored maps, actual engine verification, animations, UI and sound before it is a complete RPG.

```powershell
.\Scripts\first_pc_build_and_test.ps1 -UnrealRoot "C:\Program Files\Epic Games\UE_5.8"
```

See [Windows host acceptance](../docs/qa/windows-unreal-host.md).

## Current source milestone — October 9, 2026

- **Three original villages and two frontier outposts** in graybox actor source:
  The Crossings, Bellwold, Paperhaven, Saltwake and Cinderhold. Two returnable
  gateway paths lead from the first realm into the other two.
- **64 uniquely named NPCs**, each with an independent stable identity,
  personal memory and authored fallback dialogue (48 original + 16 frontier).
- **60 authored items (including the newer rites and Confluences)**: weapons, armor, charms, consumables and materials;
  finite one-time rewards, unique relic effects, inventory/equipment and save
  merging. The catalog includes three boss trophies and unique faction/realm gear.
- **Three named boss encounters**: the Hollow Bell creates a telegraphed
  shockwave; the Curator backs away to cast a long-range attack; the
  Pilgrim closes distance with a charge. Fold can interrupt their attack
  windups. Source AI uses no language model.
- **Six recipes and three professions**: smithing, apothecary, and scribing.
  Village prices differ, local merchants have transaction rules, civic
  payments only occur once, and failed inventory/economy saves roll back.
- **Three multi-stage faction arcs** and **two separate frontier narratives**
  with named witnesses, evidence checks, immutable conclusions and
  story-specific equipment rewards.
- **Nine-realm atlas**. Three realms now contain graybox footholds; the other
  six retain only future content descriptions and tested route-planning rules.

**These numbers refer to implemented data and Unreal C++ source**, not
rendered, editor-compiled, gameplay-tested or professionally produced
content. A working 3D game still needs an Unreal-equipped machine, authored
levels/art/animations, navigation, audio, UI, checkpoints and engine-level
verification. See [epic gameplay and expansion QA](../docs/qa/epic-world-systems.md).

### Additional temporary keyboard controls

**Y** buy the current village's commodity near its merchant; **U** sell
surplus; **P** craft at the Bellwold workshop or Paperhaven scriptorium.
**F7/F8** choose an irreversible end to a nearby faction/frontier narrative
when ready. **G** crosses a nearby realm gateway; **E** speaks or inspects a
local evidence tablet. **I** shows the current bag, money and profession skill.
These are prototype actions, not finished menus or tested bindings.

## Approved direction
- Third-person 3D; Windows PC first; controller + keyboard/mouse planned.
- Adaptive reality combat: Glimpse, Fold and warned persistent Rewrite.
- Every identifiable NPC has individuality and salient persistent memory; major NPCs have richer beliefs, relationships and motives.
- Names mix culturally consistent invented words with unsettling everyday language. Working names are **not canon**.
- Languages are an optional discovery mechanic.
- Creator has an Android phone and no PC; interactive remote tests require an approved GPU-equipped host. **Do not provision paid infrastructure without approval.**

See [approved design](../docs/specs/2026-10-08-the-unmade-foundations-design-v0.3.md) and [implementation plan](../docs/plans/2026-10-08-the-unmade-first-playable-slice.md).

## Development requirements
Unreal Engine 5.8 (provisional target), Windows C++ toolchain compatible with that engine, Python 3.11+ for engine-independent checks, and Git LFS for binary assets. Follow Epic's Unreal Engine license. Use original or properly licensed game content.

## Check the source scaffold (no Unreal installation needed)
```bash
python Scripts/validate_repo.py
python -m unittest discover -s Scripts/tests -v
```

GitHub's **Static checks** workflow runs these checks plus native C++17 gameplay tests for NPCs, combat, bosses, crafting, economy, factions and frontier travel. A green check does **not** mean Unreal C++ compiles, launches, or plays.

## Test with Unreal installed
```powershell
.\Scripts\run_ue_tests.ps1 -UnrealRoot 'C:\Program Files\Epic Games\UE_5.8' -TestFilter 'Unmade.Bootstrap'
```
This script requires UnrealEditor-Cmd.exe; refuses missing installations and rejects empty or failing automation reports. Actual execution awaits an Unreal-equipped host.

## Initial C++ foundation
The repository contains the primary module, an `AUnmadeCharacter` camera/movement skeleton, and `AUnmadeGameMode` with that pawn as default. DefaultEngine references Unreal's built-in **Entry** map purely to avoid pointing to a nonexistent custom map. It has a **runtime-generated graybox hub, prototype actors, NPC memories and decisions, combat, fracture abilities, clues, and branching local events in source code**. It has no authored `.umap`, production-ready meshes/animation, or engine-verified playable release. Unreal Editor must build and test the source and eventually create the actual maps, animations, and Enhanced Input assets. Legacy Action/Axis mappings in `Config/DefaultInput.ini` are only a temporary bootstrap, not the long-term input architecture.

## Honest review gates
1. **Repository ready:** Python checks pass and files are visible on `main`.
2. **Engine bootstrap verified:** Unreal Editor compiles, automation tests run, character actually spawns and responds to keyboard/controller on an authored map.
3. **Gameplay slice verified:** combat, fracture, NPC memories, world changes, save/load all pass Unreal engine tests.
4. **Creator playtest:** on-demand authenticated GPU Pixel Streaming works from Android, separately checked against native Windows performance.

See [status and verification](../docs/development-status.md). Paid cloud hosting is **not** active.

## Prototype hub source added (not engine-tested)

The game mode now requests a runtime-generated primitive hub instead of claiming to include a serialized map. It uses built-in cube meshes for the walkable ground, buildings and an anomaly marker, and temporary cylinder visuals for the player and five NPCs. **Engine startup and geometry collision still need to be verified on a Windows Unreal host.** These are testing placeholders, not approved world art or place names.

Once the Unreal host is running: move with WASD/stick, look with mouse/right stick, jump with Space/A, **E/X** to talk to a nearby citizen, **H** to offer aid nearby (at most once to each recipient), and **F** near the anomaly marker to emit a prototype-only event. Revisit the citizens to read their different trust/fear reactions. Memory snapshots are written to the Unreal save slot `UnmadePrototypeNPC` and read when the hub is constructed; full quest/character save/load has not been built. The F key is now **mapped in source** to a provisional Glimpse ability near the fracture. Q maps Fold and R/T map permanent Rewrite. These mechanics are **not yet engine-tested**.

NPC perception uses distance and line-of-sight; rumor evidence is represented separately but a first local-gossip timer lets nearby NPCs relay personally witnessed events once, preserving the original event ID and the immediate speaker. Meaningful NPC jobs, dialogue trees and schedules are subsequent implementation tasks. For actual tests run `Unmade.Npc` and `Unmade.Bootstrap` via the Windows test runner, then manually verify NPC witness, refusal, reload and camera behavior.

## Optional offline NPC dialogue (source added)

The local NPC dialogue adapter can send brief, provenance-aware prompts to **Ollama on the same Windows game machine**. It is **disabled by default** and always has a deterministic offline fallback. No commercial AI API is used. See [local dialogue architecture](../docs/architecture/local-npc-llm.md) for install/configuration, trust boundaries and the Unreal test gate. This feature is not a bundled AI model, has not been compiled in Unreal, and must not be described as a released playable capability.

## Fracture prototype (authored-blockout demonstration)

The hub has one temporary static-mesh fracture target. Near it press **F** for Glimpse
(brief alternate-history clue, 8 Strain), **Q** for Fold (temporarily remove collision,
24 Strain), or **R/T** for two mutually exclusive permanent Rewrite variants
(open or seal, 60 Strain). A Rewrite needs a repeat press of the same key within
six seconds and can only happen once; it persists in the existing prototype save
without erasing NPC memories. Strain recovers at 3 units/second. Failure to
save a Rewrite rolls back the model and visuals. **These are placeholders**:
there is no authored level, animation, VFX or HUD. This source is NOT Unreal
compiled or gameplay-verified yet.

The shared deterministic fracture engine is tested on GitHub by compiling
`Tests/fracture/fracture_core_test.cpp` with g++. The Unreal editor must still
compile and run the scene and `Unmade.Fracture.CoreRules` automation test.

## AI-free gameplay is the default (approved design)

**No AI download, subscription, internet access or local inference process is required for gameplay.** NPC roles, temperament, perception, memory, trust/fear, available actions and authored dialogue belong to Unreal's native game rules. `AUnmadeNpcCharacter::CanTradeWithPlayer()` and `GetCurrentActionId()` expose independent gameplay decisions. The optional local LLM is disabled by default and must never control quests or NPC actions.

See [AI-independent NPC design](../docs/architecture/offline-npc-gameplay.md). GitHub CI compiles and runs the pure C++ NPC decision-policy tests. NPC movement, authored shops, Unreal compilation, and actual runtime behavior remain unverified/incomplete.

## Physical combat — source prototype (not yet Unreal-compiled)

The hub spawns two temporary enemies away from the civilian center: a **Stalker**
that closes in for melee and a **Watcher** that retreats when cornered and uses
a visible debug-message *placeholder* ranged impact when clear sight permits.
Neither invokes any LLM. The player's left mouse button/right trigger attacks
one facing, close, line-of-sight target; right mouse button/left trigger holds
guard (75% reduced damage, no attacking while held). Attacks have cooldowns,
enemy health cannot be reduced twice by the same swing, and defeat disables
that enemy's collision/appearance. **Q/Fold** temporarily exposes nearby enemies
to 50% extra incoming attack damage. Player defeat stops movement; checkpoints,
loot, animations, hit VFX, projectiles and proper navmesh pursuit remain future
work. See [combat test scope](../docs/qa/combat-prototype.md).

## Optional language discovery (Unreal source prototype)

Glimpse near the fracture and speaking with the records keeper yield two distinct
clues to interpret a provisional word, VEYL. Press E near the anomaly to read the
inscription once both clues are found. Clues persist in the existing prototype
save and never depend on Ollama or any AI model. This is authored, optional
discovery, not a grind or quest prerequisite. See
[language discovery QA](../docs/qa/lexicon-prototype.md). Native lexicon tests pass
through GitHub CI; Unreal engine behavior and save/reload remain unverified.

## Local settlement conflict (AI-independent source prototype)

Near the central fracture, press **Z / D-pad Up** to favor community shelter or
**X / D-pad Down** to favor anomaly research. A second press within six seconds
confirms the choice and opens exactly one graybox route permanently for the
save slot. This is independent of the earlier reality Rewrite mechanic.
An optional supply run uses **V / controller top face button** to collect near
the market and deliver once at the shelter. **J** shows a temporary text
journal with current choices, supply stage, language clues and Strain.
All outcomes merge into the existing local save; adaptive residents can
witness and remember resulting actions without AI. See
[branching conflict QA](../docs/qa/branching-conflict.md).
This source has not been compiled or playtested in Unreal Editor.

## Physical NPC responses — source prototype

The five named residents now use the offline decision policy for simple in-world
movement: investigating the fracture, approaching a trusted player, withdrawing
when frightened, or running a tiny courier route. This is **swept graybox actor
motion**, not animation, crowd avoidance or NavMesh AI. The deterministic
motion policy has native CI tests. Unreal compilation and interactive gameplay
still require the Windows Unreal host.

## Expanding into a living region (source prototype, October 9)

The original small hub now has a larger interconnected primitive floor, outer
ruins, and **six inspectable authored landmarks** with distinct stories. A 20-minute
day/night simulation changes location glows, ambient prose and five residents'
routine destinations; witnessed events still override their schedules. Press
**E** near a landmark for its description and one-time discovery record; **J**
shows the in-world day/time and sites found out of six. The Threshold of Two
Claims reacts to the permanent shelter-vs-research choice. Exploration state
persists alongside existing NPC, conflict, Strain and lexicon saves.

No model download or external API is required. This is source code with native
CI checks, **not** a verified rendered environment, asset pass or Unreal build.
See [living-region QA](../docs/qa/living-world-region.md).

## Peripheral residents and authored reactions

Six additional named residents bring the source roster to **eleven**.
They include a well listener, displaced orchard keeper, future-debt keeper,
road warden, bell maker, and night courier. NPCs have individually located
daytime stations and role-based dusk/night routines. Their authored dialogue
responds to time, witnessed events and uncertain rumors without an LLM.
Physical proximity and a clear line of sight limit overheard social exchanges.
The temporary directional-light cycle augments the six landmark lights. This
remains uncompiled Unreal source, not a released world.

## Three connected villages and 48 residents

The graybox source now contains The Crossings (center), Bellwold Refuge
(west) and Paperhaven Archive (east), each with 16 named characters.
Ground-height causeways connect the three spaces 180 meters apart.
People have separate persistent memories, authored voices, local daypart
routines, settlement-specific attitudes toward learned events, and an
independent conversation/aid interface without any language model.

Bellwold has an optional lantern-relief task: talk to Rook the lamplighter
and then Hessa the refuge matron. Paperhaven has missing testimony: talk
to Toma the copyist and then Sevrin the registrar. Quest progress survives
save/reload and cannot be rewarded repeatedly. Press E to interact, H to
help an individual, J to see community visits and stories. Existing
Crossings missions remain.

Tests cover the engine-independent policies and Unreal source wiring.
No Unreal compilation, native render or full gameplay pass has occurred.
See docs/qa/three-settlements.md.

## First earned loot and equipment pass (initial historical baseline)

The deterministic C++ source now defines **28 authored items** across gear,
consumables and materials with Common, Uncommon, Rare, Epic and Mythic rarity.
Quest rewards, first enemy victories, unique landmarks, all three villages,
the settlement decision and complete language evidence feed an item system
that remembers each milestone and prevents repeating the same claim.
Two village stories plus a cross-village journey and rare crafting materials
allow **Waybreaker, the Impossible Road** to be forged once at Bellwold's
workshop. It is not a random loot-box drop.

Gear affects real native combat attack/armor. Unique relics also change
reality abilities: a Returned Voice or Paperhaven charm makes Glimpse cost
less Strain, Waybreaker extends Fold by three seconds, and the Unwritten
Crown accelerates passive Strain recovery. All modifiers are bounded and
tested without AI. Potions heal health or recover Strain only when useful.

Prototype keyboard controls: **I** inventory text, **B/M/N** cycle owned
weapon/armor/charm, **1** heal, **2** restore Strain, **K** forge when near
Bellwold's workshop. True equipment meshes, inventory UI, controller menu,
shop transactions and Unreal Editor/runtime tests are still unfinished.

A separate future-world atlas in source defines nine distinct realms and a
tested route graph with stages based on earned exploration, village progress
and mythic crafting. **As of the initial item milestone only the three starting villages had graybox actor source. The subsequent current milestone above adds two frontier outposts, not two finished continents; six atlas regions remain designs.** See [epic rewards and universe design](../docs/design/2026-10-09-epic-rewards-and-nine-realms.md).


## Lived universe preproduction work orders (10 October 2026)

The canonical nine realms gained a non-authoritative detailed living-world production layer: 27 inspectable physical contradictions, 18 routine encounter scripts, nine optional five-step quests (each with two civic endings), nine cross-realm comparison links and 36 art/audio work packages. They are declared in [the lived-universe JSON](../Authoring/lived_universe_atlas.json), rendered to [90 editor reference work orders](../Authoring/generated/lived_universe_scene_manifest.json), and explained in [the microdetail production bible](../docs/design/lived-universe-microdetail-bible.md).

Run `python Scripts/validate_lived_universe.py --check` before changing or importing the master; `--write` regenerates the work orders. CI and first-PC preflight reject source/output drift. **These documents and GUIDE labels are not UE actors, voice performances, finished side quests or compiled levels.** All preexisting actual main-quest completion and save semantics remain unchanged.


## Full-universe audio director animatic — October 10

The initial 16-shot 149.5-second Volume IX film was a spoiler-light illustrated teaser, not an immersive summary of the game. A separate **56-shot / 12:15.03 spoiler-inclusive director edition** now covers all nine societies and their daily struggles, 82 source NPC identities, exploration and ten signature disciplines, fair combat, physical quest returns, Nhal-Vey's false victory and both changed-world mornings. The conversation production kit contains the audio-equipped MP4, subtitle SRT, temporary synthetic narrated dialogue, and lossless scratch voice/score stems; **none of these are Unreal renders or checked-in binary gameplay assets**.

The source index and 7-chapter timing are in [Authoring/cinematic_directors_cut_v2_index.json](../Authoring/cinematic_directors_cut_v2_index.json), and the sound+scope production acceptance is in [docs/design/cinematic-directors-cut-sound-and-scope.md](../docs/design/cinematic-directors-cut-sound-and-scope.md). Run `python Scripts/validate_directors_cut.py` for CI source checks and `--media <MP4> --subtitles <SRT>` when the actual output is available. Green index CI alone does not guarantee audible media or finished cinematography.


## Volume X: primary canon for book, screen and game expansions

The books now have an actively managed literary storyworld: docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md is an original primary narrative volume, not merely a game manual. It dramatizes Bellwold and Orravane, fixes distinct material/civic identities for eleven existing cities, deepens thirteen original actor identities, preserves nine intentional unsolved questions, maps thirteen meaningful callbacks, and prepares novel/film/television/optional game-expansion narratives. Authoring/transmedia_storyworld_v1.json holds stable links and adaptation protections; run python Scripts/validate_transmedia_storyworld.py --check. It does not alter source gameplay, player origins or choose a universal final ending. Earlier illustrated binary editions remain dated snapshots pending deliberate new releases.


### Playable-source lore link: Witness Echoes (Volume X)

Volume X is beginning to affect the **game's actual world interactions**, not only its books. Three new physically inspectable graybox relics—Hessa's cup, Sevrin's press and Orrel's nail—are wired through the existing player Interact control, saved as optional first-seen/returned clues, and summarized in the debug journal. Each changes meaning only after a legitimate related local story decision; none hands out a secret finale, forces optional content or resolves an OPEN mystery. See [Volume X Witness Echoes gameplay contract](../docs/design/volume-x-witness-echo-gameplay.md). CI tests the portable domain and source integration; Unreal Editor compilation and real level/playtest verification remain unperformed.
