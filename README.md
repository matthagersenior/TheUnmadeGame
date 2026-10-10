## Volume VII illustrated canon — The World Withheld Its Answer

The current illustrated development edition is **66-page Volume VII** (DOCX and visually inspected PDF), retaining **43 embedded illustrations** and the original **30-piece concept art atlas**. Its integrity-verified **38-entry ZIP** includes a quality matrix, source appendix, release status and SHA-256 manifest. The new sections cover six optional third-return Echo Quests, nine distinct cultures, eighteen NPC identity/motivation briefs, all ten canonical ability production verbs, and a minimal-effort authoring/Unreal DataTable import workflow.

Source baseline: [merged gameplay PR #10](https://github.com/matthagersenior/TheUnmadeGame/pull/10) (`bb5a929bb5d4a4e794dfb6c55fd5899decd75c66`) and [merged authoring PR #11](https://github.com/matthagersenior/TheUnmadeGame/pull/11) (`e8c3948cdfc697c666621eab16670f7a55b7634a`). Both completed post-merge green native/static GitHub CI. The illustrated binary package is a conversation download, not a GitHub binary or an Unreal-game build. Illustrations are speculative references.

## Single-source production world pack (October 10)

The project now has a deterministic JSON → **three Unreal DataTable CSVs + 18 native NPC identities + full art/dialogue dossier** authoring pipeline, with CI drift protection and automatic verification in the first-PC preflight. It currently covers all nine realm cultures, eighteen individual later-realm characters with day/night lines and personal knowledge limits, and all ten existing abilities with production-friendly uses, limits and cues. The NPC actors read these actual source names/dialogue. [Authoring and Editor-import guide](Authoring/README.md). **Editor import has not yet been tested**.

## Six optional third chapters: Echo Quests (October 10)

The six later realms now have a third authored return investigation each, after their existing first-visit and aftermath arcs. The player must inspect two independent material clues, speak with a matching resident witness and confirm one of two long-term civic changes on an actual world control; a visible dispatch appears in a connected realm. Every outcome saves independently and cannot be repeated for free resources. The source also repairs the prototype ground-gap and gate-wall geometry (previously overlapping); native C++ asserts protect the layout, but **Unreal Editor/PIE still must verify player collision and navigation**. See [the six Echo quest writer/implementation bible](docs/design/echo-third-chapter-world-bible.md), [manual acceptance](docs/qa/echo-quest-acceptance.md).

## Current illustrated source edition — Volume VI: Lands That Teach

The latest rendered canon package is a **57-page Volume VI DOCX/PDF**, retaining all 43 embedded illustrations and 30 atlas concept/schematic images, with a verified **38-entry ZIP**, provenance, source appendix, quality matrix and SHA-256 manifest. The edition documents six distinct timed injury/strain danger loops, locally shelter-seeking NPC behavior, and real-Unreal acceptance still required. **It is a conversation download, not an engine screenshot, GitHub binary, or Windows game build.**

Game-source commit: `fe849e839cf7eb3550df9711d99fc42cf50f960f` (merged PR #8; native source CI green). The full Unreal Editor compile, navigability and balance are not tested.

## Six realm-specific hazard and counterplay loops (October 10, 2026)

The six later-realm first-visit approaches now have independent rhythmic warnings, safe sidelines and distinct stakes: injury from the debt tide, quarry fractures and law roots; nonlethal reality strain from the inverted choir, involuntary census and contradictory horizon. Physical damage respects guarding; nonlethal pressure saturates and recovers. Source-timed cue geometry and locally alert resident routines are wired. Completing the realm's first intervention ends that realm's hazard, with no perpetual penalty. **The hazards are native-test-verified only, not compiled or played in Unreal.** See [six-realm hazard QA](docs/qa/later-realm-hazards.md).

## Volume V illustrated package — Six Roads Beyond the Known
A 55-page Volume V DOCX/PDF was rendered from the Volume IV illustrated master; 30 prior concept/schematic art assets are preserved. The verified 38-file ZIP includes the editable DOCX, PDF, art atlas, quality matrix, source appendix and SHA-256 manifest. The package is a conversation download artifact, not a tracked GitHub binary or Unreal gameplay video.

Source baseline: [merged gameplay PR #6](https://github.com/matthagersenior/TheUnmadeGame/pull/6), commit `61a90178985b34dbffde5eb54a509562d7ae8c38`. The post-merge `main` static/native C++ CI succeeded; Unreal Engine compilation and playtesting remain blocked without a Windows host.

# THE UNMADE

Dark, surreal, third-person 3D RPG. You play a customizable outcast whose origin lies in an impossible version of reality. Living civilizations respond differently to fractures, and NPCs remember what they have personally witnessed or credibly heard.

**Status:** foundational Unreal Engine **source scaffold**, not a playable release. No Unreal Editor compilation, map boot, packaged Windows build, or Android streaming session has been verified yet.

## Source milestone — Saltwake and Cinderhold return visits (October 10)

The first frontier adventures now extend beyond entering an outpost and finishing its first three-witness story. After resolving that earlier story, return to Harrow in Saltwake or Rheva in Cinderhold. Each return begins with an active, distance- and sight-gated world control: the **Storm Sluice** or **Heat Vent**. Activating the mechanism disables its blocking collision barrier, allowing investigation of either the communal relief site or the concealed public record. A route-specific resident must testify before F7/F8 can commit the result near its warden.

Each of the two outcomes changes **different collision-enabled passages, walkway/platform geometry, public home-town commentary and witnessed versus rumored NPC memories**. Nine-realm snapshot restoration rejects malformed states and rolls back changes if saving fails. No rewards can be farmed from repeated decisions. The six remaining realms have named future controls in shared C++ rules but **do not yet have world actors or playable levels**.

See [frontier return QA](docs/qa/frontier-aftermaths.md) and [physical adventure standard for all nine realms](docs/design/2026-10-10-physical-realm-adventure-contract.md). **Unreal Editor compilation, traversal, performance and real player verification remain outstanding.**

## Deliberate irreversible story choices — source pass

All existing F7/F8 permanent outcomes (three faction chronicles, both first frontier accords, Bellwold's Second Night and the two actor-backed frontier return stories) now require two matching presses within six seconds **near the same qualified story representative**. The first press previews a concrete lasting consequence; changing target or ending restarts the warning. The native decision policy is tested without AI or network. **Unreal gameplay and accessible confirmation UI remain unverified.**

See [irreversible-choice QA](docs/qa/choice-commitment.md).

## Custom protagonist identity source pass — October 10

The new offline character profile supports eight bounded customizable dimensions (body, face, hair, voice, palette, reality mark, gait, calling), a UTF-8 name and an unchanged common impossible origin. Native C++ checks cover invalid selection, malformed names, atomic restore and independent character state. Unreal-facing Blueprint-callable setters save optional schema-1 fields without erasing prior missions, and primitive body/hair geometry offers only an initial silhouette check. Press **C** to view the temporary text profile. **A real 3D creator screen, skinned mesh, facial art, voice performances and full controller UX remain to be built and tested on Unreal.**

See [character identity QA](docs/qa/character-identity.md).

## The Second Night — Bellwold Afterlight

An authored return-visit chapter begins only after Bellwold's relief mission
and the Refuge Compact faction ending. Investigate either a shelter cache or
an erased census, seek Sorin or Ivera as appropriate, then return to Hessa
for a lasting ward or public monument. Nearby residents remember actual
witnessed choices; both routes grant mutually exclusive epic rewards.

See [Afterlight story](docs/design/2026-10-09-bellwold-afterlight.md)
and [Afterlight engine QA](docs/qa/bellwold-afterlight.md). **60 named items**
exist in source. Unreal compilation and in-engine playtesting are outstanding.

## Latest source milestone — Tenfold Chronicles and Confluence saga

**Ten authored signature mechanics, fifty five-stage quest beats, six paired-discipline challenges, and fifty-eight named equipment/material items are now represented in tested offline C++ source.** Skills make real choices about gravity, sound, witnessed bridges, alternate lives, evolving gear, roads, future debt, boss mercy, overlapping histories, oaths and truthful mapping. Outcomes persist with NPC-specific evidence and reward deduplication.

Six Confluence chambers require mastered ability combinations, a timed dual-cast and firsthand witnesses; the final chamber requires the full earlier saga. The last chamber is **not** a finished game ending. Original three villages and two frontier outposts remain graybox source, not completed levels.

See the [Tenfold quest/lore bible](docs/design/2026-10-09-tenfold-rites-quest-bible.md) and [engine QA checklist](docs/qa/tenfold-prototype-qa.md). **No Unreal compilation, animation, visual HUD, full quest production, complete nine-realm world, difficulty benchmark or playtime validation has occurred.** All critical logic works without AI.

## Six later realms: source graybox expansion (October 10)

Drevlach, Orravane, Vathless, Eillun, Tharniv and Auvren now have source-generated destination grounds, six physically different first-visit encounters, a real gap and one of two permanently selected bridges, a second return investigation, three source NPC witnesses apiece, and mutually exclusive civic gates. Travel follows the actual atlas edges and earned attunement; new optional save state restores progression independently. Native tests cover evidence, corruption, travel locks and endings. **This is not a finished level, Unreal-compiled game or full six-realm campaign.** See [six-realm manual acceptance](docs/qa/later-realm-expedition.md).

## Latest illustrated edition
Volume IV, **A World Prepared to Respond**, adds the source-aligned hazard/recovery/customization/choice-confirmation chapter to the previous illustrated master. A 52-page Word/PDF and 39-file checksum-verified ZIP were generated as **conversation download artifacts**, not source-controlled game assets, Unreal screenshots or a compiled build. The canonical code/lore remains in this repository.

## Pre-PC readiness and remaining production gaps

The [maintained readiness ledger](docs/qa/pre-pc-readiness.md) distinguishes what is verified in offline source checks from Unreal-only compilation, art and real gameplay acceptance. It also tracks source work that can be done before acquiring a PC; **an actual finished RPG is not simply waiting for hardware**.

## First-PC handoff and pre-PC source preparation (October 10, 2026)

Two new deterministic frontier dangers are wired in source: Saltwake's rain surge (16 base damage) and Cinderhold's heat pulse (22 base damage), each with a calm / visible warning / impact cadence. They affect only the marked approach while its mechanism is unresolved. Guard reduces injury, the player can retreat to a safe side route, and an operated mechanism stops later pulses. A defeated player returns after four seconds to the current realm's safe entry at 75% health, without resetting story or NPC memories. **No engine runtime, hazard balance, visuals or controller feel has been verified.**

The first Windows PC can run a **single script** to perform host preflight, compile the Unreal Editor target and execute Unreal automation, retaining diagnostic logs. The project still needs authored maps, actual engine verification, animations, UI and sound before it is a complete RPG.

```powershell
.\Scripts\first_pc_build_and_test.ps1 -UnrealRoot "C:\Program Files\Epic Games\UE_5.8"
```

See [Windows host acceptance](docs/qa/windows-unreal-host.md).

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
verification. See [epic gameplay and expansion QA](docs/qa/epic-world-systems.md).

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

See [approved design](docs/specs/2026-10-08-the-unmade-foundations-design-v0.3.md) and [implementation plan](docs/plans/2026-10-08-the-unmade-first-playable-slice.md).

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

See [status and verification](docs/development-status.md). Paid cloud hosting is **not** active.

## Prototype hub source added (not engine-tested)

The game mode now requests a runtime-generated primitive hub instead of claiming to include a serialized map. It uses built-in cube meshes for the walkable ground, buildings and an anomaly marker, and temporary cylinder visuals for the player and five NPCs. **Engine startup and geometry collision still need to be verified on a Windows Unreal host.** These are testing placeholders, not approved world art or place names.

Once the Unreal host is running: move with WASD/stick, look with mouse/right stick, jump with Space/A, **E/X** to talk to a nearby citizen, **H** to offer aid nearby (at most once to each recipient), and **F** near the anomaly marker to emit a prototype-only event. Revisit the citizens to read their different trust/fear reactions. Memory snapshots are written to the Unreal save slot `UnmadePrototypeNPC` and read when the hub is constructed; full quest/character save/load has not been built. The F key is now **mapped in source** to a provisional Glimpse ability near the fracture. Q maps Fold and R/T map permanent Rewrite. These mechanics are **not yet engine-tested**.

NPC perception uses distance and line-of-sight; rumor evidence is represented separately but a first local-gossip timer lets nearby NPCs relay personally witnessed events once, preserving the original event ID and the immediate speaker. Meaningful NPC jobs, dialogue trees and schedules are subsequent implementation tasks. For actual tests run `Unmade.Npc` and `Unmade.Bootstrap` via the Windows test runner, then manually verify NPC witness, refusal, reload and camera behavior.

## Optional offline NPC dialogue (source added)

The local NPC dialogue adapter can send brief, provenance-aware prompts to **Ollama on the same Windows game machine**. It is **disabled by default** and always has a deterministic offline fallback. No commercial AI API is used. See [local dialogue architecture](docs/architecture/local-npc-llm.md) for install/configuration, trust boundaries and the Unreal test gate. This feature is not a bundled AI model, has not been compiled in Unreal, and must not be described as a released playable capability.

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

See [AI-independent NPC design](docs/architecture/offline-npc-gameplay.md). GitHub CI compiles and runs the pure C++ NPC decision-policy tests. NPC movement, authored shops, Unreal compilation, and actual runtime behavior remain unverified/incomplete.

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
work. See [combat test scope](docs/qa/combat-prototype.md).

## Optional language discovery (Unreal source prototype)

Glimpse near the fracture and speaking with the records keeper yield two distinct
clues to interpret a provisional word, VEYL. Press E near the anomaly to read the
inscription once both clues are found. Clues persist in the existing prototype
save and never depend on Ollama or any AI model. This is authored, optional
discovery, not a grind or quest prerequisite. See
[language discovery QA](docs/qa/lexicon-prototype.md). Native lexicon tests pass
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
[branching conflict QA](docs/qa/branching-conflict.md).
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
See [living-region QA](docs/qa/living-world-region.md).

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
and mythic crafting. **As of the initial item milestone only the three starting villages had graybox actor source. The subsequent current milestone above adds two frontier outposts, not two finished continents; six atlas regions remain designs.** See [epic rewards and universe design](docs/design/2026-10-09-epic-rewards-and-nine-realms.md).
