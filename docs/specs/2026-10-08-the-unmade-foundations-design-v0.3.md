# THE UNMADE — Foundational Game Design

**Version:** 0.3 (approved design baseline)  
**Date:** 2026-10-08  
**Project:** `matthagersenior/TheUnmadeGame`  
**Intended repository path:** `docs/specs/2026-10-08-the-unmade-foundations-design-v0.3.md`  
**Status:** Approved by the creator on 2026-10-08 as the starting design baseline. No Unreal game has been implemented by this document.

## 1. Vision

**THE UNMADE** is a third-person, 3D, PC-first action RPG set in a populous yet ontologically unstable world. Its tone is dark, mysterious, strange, and imaginative. The player is a fully customizable wandering outcast with a common origin: **The Impossible** — someone who comes from a version of reality that should never have existed.

The design balances exploration, responsive combat, character development, story, and freedom. Its distinctive premise is not simply that the world changes, but that the player can learn to interact with mutually contradictory realities. Actions have personal and social consequences that are legible enough to learn, yet substantial enough to matter.

**Creative rule:** avoid conventional fantasy naming, renamed genre tropes, and novelty for novelty's sake. Originality must appear in actions players can take, not only lore.

## 2. Approved player-facing direction

- **Platform and presentation:** PC first; third-person 3D; plan for mouse/keyboard and controller.
- **Engine:** Unreal Engine 5 family. Exact version/toolchain to be selected during technical planning.
- **World:** Interconnected civilizations, trade, travel, populated settlements, wilderness, contested borders, and unsettling mysteries beneath apparently normal life. Build one dense opening region before expanding.
- **Combat:** Real-time physical attacks, positioning, defense, and adaptive reality manipulation that can alter tactics, objects, encounters, and enemy forms.
- **Character:** Appearance, background, capabilities, and ethics customizable; no compulsory heroism or permanently locked moral alignment. Shared impossible origin does not prescribe personality.
- **Civilizations:** They interpret fractures differently — suppression, study/exploitation, and acceptance — with competing valid insights and harmful blind spots.
- **Naming:** Hybrid naming — evocative recognizable language plus invented words with consistent linguistic and cultural logic; contradictory place names may be lore and gameplay clues.
- **Languages:** Discovery mechanic, not a mandatory language-learning grind. Interpretation changes what the player can perceive, negotiate, investigate, and sometimes accomplish in combat.
- **Reality-cost rule:** Both personal and world consequences, with understandable trade-offs and warnings; routine abilities should remain fun and accessible.
- **NPC agency (user approved):** Every identifiable NPC has consistent individual traits and at least a lightweight persistent memory. Significant NPCs have richer episodic memories, relationships, motives, and context-sensitive behavior. Reactions depend on perception, rumor, and changes in reality; NPCs cannot be omniscient.
- **Testing access:** The creator has no PC. Design must provide useful playtesting via Android using a remote GPU-hosted Unreal runtime or cloud PC, with budget-aware session control and eventual native PC QA.

## 3. Core gameplay loop

1. **Enter a lived-in place:** observe people with routines, factions, rumors, and material interests.
2. **Notice a contradiction:** a location, inscription, memory, creature, or event does not fit the record.
3. **Investigate freely:** explore, speak, collect clues, interpret languages, and choose whom to trust.
4. **Confront or bypass danger:** use physical combat, dialogue, stealth, environmental knowledge, or reality abilities.
5. **Choose a resolution:** favor an individual, faction, settlement, or hypothesis; larger intervention can alter what persists.
6. **See the aftermath:** schedules, relationships, access routes, descriptions, and quests update; new mysteries emerge.

The player must regularly make interesting decisions even while ignoring the main storyline.

## 4. The Impossible: character and progression

The player designs their character's appearance and background, selecting initial strengths rather than a permanent class. Their memories of an ordinary former life may be sincere even if the remembered reality never occurred here. The origin is common to all characters, but responses to it are personal and player-directed.

**Progression tracks (conceptual):**
- **Practical mastery:** weapons, movement, defense, survival, and trained skills.
- **Fracture literacy:** recognizing phenomena, anticipating risk, and manipulating select alternate states.
- **Interpretive knowledge:** vocabulary, cultures, records, and contradictions identified in a personal lexicon.
- **Relationships and reputation:** situated trust among NPCs and factions rather than a single universal morality meter.

Training, choices, and discovery should unlock combinations; avoid requiring a narrow build to complete the main path.

## 5. Signature system: The Strain (working title)

### 5.1 Action tiers

| Tier | Typical player action | Cost and consequence profile |
|---|---|---|
| **Glimpse** | Momentarily perceive an alternate enemy tell, reveal a concealed route, hear an echo of an inscription | Low personal strain; no automatic permanent world rewrite. Intended for frequent use. |
| **Fold** | Temporarily change a bridge segment, interrupt an attack through an alternate enemy state, expose another form of cover | Recoverable personal strain plus a localized, time-bounded world disturbance. NPCs or enemies may react. |
| **Rewrite** | Anchor a contested building, restore one history of a place, shift a lasting environmental or social fact | Significant, explicitly previewed possible consequences to the player and world; persistent outcome recorded as a world-state choice. |

These are design categories, not finalized ability names, numerical costs, or a promise of arbitrary world generation.

### 5.2 Personal consequences

Strain can temporarily affect perception, movement/ability behavior, or the stability of powers. Mechanisms for recovery, mitigation, and mastery must exist. Avoid untelegraphed permanent character loss or punishing the basic act of using the headline mechanic. Consequences must support engaging decisions rather than a frustrating stamina tax.

### 5.3 World consequences

Major fracture decisions can affect geometry, safe routes, NPC memory, settlement access, regional reputation, and quest availability. Design the playable alternatives as authored world states with explicit dependencies. An inhabitant might persist in one state and be absent from another; the system must not silently invalidate the player's quests, inventory, or save data.

### 5.4 Fairness contract

- **Readable warning:** before an irreversible Rewrite, communicate the kind of risk without spoiling every result.
- **Learnable rules:** observed clues, languages, and prior encounters improve prediction.
- **Consistent outcomes:** the same local situation under the same relevant conditions yields consistent results.
- **Agency:** players may abstain, seek information, negotiate, or use a less destructive strategy where authored.
- **Persistence:** consequences survive save/load; revisit and dialogue must reflect them.
- **Recovery:** transient Fold states naturally return or resolve; persistent Rewrite choices require explicit authored reversals if reversibility is offered.

## 6. Combat pillars

Physical combat provides a reliable foundation: movement, targeted strikes, defensive options, enemy reads, and meaningful weapon differences. Adaptive reality combat adds tactical possibilities, not mandatory particle effects. The player may expose a form in which an armored enemy is vulnerable, turn cover into a temporary obstruction, or uncover a contradiction that enables a non-combat response. Enemies should have counterplay and react visibly.

Initial combat is deliberately narrow: one melee archetype, one ranged/alternate threat, and a few usable fracture interactions. Expansion to equipment trees and broad enemy catalogs follows only after the core loop is enjoyable and testable.

## 7. A living world with incompatible histories

Three **cultural positions**, rather than fixed good/evil alignments, establish the geopolitical backbone:

1. **Suppression:** fractures threaten public stability and official continuity. This authority protects many people while marginalizing anomalies.
2. **Study and commerce:** fractures permit discoveries, instruments, and wealth, while shifting risk onto others.
3. **Coexistence and transformation:** communities adapt to contradictory realities, providing refuge while sometimes normalizing dangerous change.

Players may align with, oppose, or cooperate selectively with these interests. Their responses should be regionally grounded; joining one does not trigger a universal moral score.

**Naming prototypes — not approved canon:** *The Almost* (opening region), *Stillhere* (main settlement), *Oruvel* (wilderness), *The Second Below* (subterranean site), *The Only* (suppression institution), *Ketruun* (research culture), *The Manywake* (coexistence communities). Earlier conventional names (*The Hollow March*, *Harrowgate*, *The Weeping Wood*, *The Underwell*, *Veyr Dominion*, *Meridian Houses*, *Elseborn Communion*) are superseded as **working concepts**, not locked nomenclature.

**Linguistic design rule:** build short sound inventories, morphological patterns, name components, and translated cultural meanings before producing hundreds of invented names. Let a locale have multiple truthful names when its histories conflict.

## 8. Language discovery: Echo Tongues (working title)

The journal begins with unknown terms and competing interpretations. Players collect context from dialogue, inscriptions, objects, and conversations; hypotheses gain or lose support as discoveries accumulate. Context-sensitive translations unlock clues, dialogue choices, navigation options, or optional combat insights.

The language system should reveal and reward rather than gate the essential main story behind repetitive collection. Some terms may remain contested: a civilization's accepted translation need not be objectively correct.

**Prototype interaction:** a three-part inscription begins unknown. Contexts gained from a speaker and an artifact reveal enough meaning to identify a safer fracture action at a ruin. The notebook updates without mechanically auto-solving all future texts.

## 9. First playable region and opening

**Region:** a dense intersection of cultures — a working town, a traversable wilderness edge, and a subterranean anomaly. The inhabitants trade, maintain routines, disagree, and remember conflicting histories.

**Opening:** After character creation, the player arrives as a wandering outcast. They can take a practical job, explore, meet residents, or follow rumors. A structure appears with inhabitants who insist it has always been there. Each cultural interest proposes a different response. The player gathers evidence and can make a consequence-bearing choice. The deeper anomaly foreshadows the mystery of the Impossible origin.

**Opening design rule:** no lengthy compulsory exposition; teach interacting, moving, fighting, deciphering, and choosing through situations that could plausibly occur in this world.

## 10. First playable vertical slice — proposed boundaries

**In scope:**
- One small, connected neighborhood or settlement hub, an adjoining path/wilderness pocket, and one anomaly interior.
- Third-person controller, camera, keyboard/mouse and controller input, basic interaction, save/load.
- Placeholder character customization covering appearance presets and one meaningful background choice.
- Basic real-time combat with two distinct enemy behaviors, one conventional weapon style, and one defensive action.
- Three testable fracture actions: one Glimpse, one Fold, one clearly warned Rewrite.
- One short language discovery chain recorded in the lexicon.
- Approximately five key NPCs with schedules, event-driven reactions, and distinct motivations; at least two demonstrate rich persistent episodic memory and altered relationships. Every additional identifiable resident in the playable hub has a stable identity, baseline temperament, and at least one persistent response to a consequential local event; background simulation remains lightweight.
- One branching local conflict with at least two persistent outcomes, plus a nonmandatory activity.
- A functional menu, journal/quest tracking, and visible personal/world consequence feedback.

**Explicitly out of scope for this slice:** a fully seamless continent, hundreds of simulated citizens, large-scale dynamic world generation, dozens of weapon schools, broad crafting/economy systems, cinematic campaign, multiplayer, and simultaneous console/mobile shipping.

**Acceptance demonstrations:**
1. Player creates a character and can move, interact, and fight in a 3D space.
2. A routine fracture works repeatedly without permanently damaging the world state.
3. A Fold visibly changes tactical/environmental affordances and resolves reliably.
4. A warned Rewrite chooses one of at least two persistent, observable settlement outcomes.
5. A learned word changes a relevant interpretation or available response.
6. Relevant NPC dialogue/behavior and journal react to the chosen outcome.
7. At least two named NPCs remember different player interactions across departures and save/load, changing behavior (not merely a dialogue line); at least one distinguishes personally witnessed events from rumor. Every identifiable hub resident retains their baseline identity and any flagged consequential event across save/load.
8. An NPC visibly changes behavior when witnessing a localized Fold or hearing credible news of a major Rewrite.
9. The creator can playtest via an Android-accessible remote session without running Unreal locally.
10. Saving and reloading preserves the chosen outcome, lexicon findings, NPC memories, and character state.
11. The prototype remains completable without requiring a single moral alignment or faction allegiance.

## 11. Adaptive NPCs: personal memory, beliefs, and agency

**Product requirement (explicitly approved):** Every identifiable NPC has individuality and some persistent memory. High-importance NPCs have more nuanced, persistent memories, beliefs, relationships, competing motivations, and adaptive decisions. Reactions extend to the player, other NPCs, factions, local events, language discoveries, and reality changes. A living world means more than changing scripted dialogue; characters visibly change behavior and choices.

**Three fidelity levels (all maintain continuity):**
- **Ambient residents:** Stable or reproducibly generated identity, disposition, home/work affiliation, simple daily routine, and compact flags/summaries of consequential events. These residents may use pooled/offscreen simulation, but must not reset into contradictory strangers when the player returns.
- **Connected NPCs:** Merchants, guards, neighbors, and other repeat contacts additionally track direct encounters, a limited set of relationships, rumor sources, personal priorities, and reactions to the player's conduct.
- **Principal NPCs:** Companions, major rivals, leaders, and quest-critical characters maintain richer episodic memory, competing goals, revised beliefs, relationship arcs, and authored responses to contradictory histories.

**Identity and persistence invariants:** Each identifiable NPC has a stable ID (or a deterministic identity seed for reproducibly generated residents), individual traits, and a persistent summary of consequential events; simulation fidelity may change with narrative importance without wiping identity or memories. Unnamed visual-only background extras are not promised individually authored life stories, and cannot become persistent quest actors without first receiving an identity. Memory persistence uses bounded salient events plus summarized history—not exhaustive diaries or continuous expensive AI inference.

**Proposed behavioral architecture:**
- **Perception:** Sight, sound, proximity, direct interaction, and subscribed world-state events determine what each NPC could know. An NPC must not react to unseen events as if personally witnessed.
- **Episodic memory:** Store selected consequential events as compact facts: actor, action, target, witnessed/heard, certainty, time, location, and world-state version. Bound memory quantity and retain pivotal events.
- **Relationships:** Maintain independent trust, fear, gratitude, resentment, obligation, and respect values, without collapsing moral complexity into a single approval score.
- **Needs and motives:** Daily work, family, safety, ambition, faction duties, immediate threats, and personal values influence actions. NPC goals can conflict with one another.
- **Social propagation:** Characters share rumors according to proximity, social ties, interest, and credibility. A rumor may be distorted; characters can correct beliefs when shown evidence.
- **Contradictory reality:** A Rewrite may alter a character's apparent history or remembered facts. The world-state ledger records the authoritative events while individual NPC belief states may disagree. Contradictions must be story-authored and save-safe.
- **Visible consequences:** Adaptive reactions change patrol routes, trade access, prices, willingness to help, who warns guards, avoidance, alliances, offers, and dialogue—not only text.
- **Player agency:** Intimidation, explanation, restitution, concealment, discovery, and language comprehension offer routes to change a damaged relationship where contextually valid.

**Concrete prototype test:** The player protects a merchant from a creature but later performs a frightening Fold in view of the merchant. The merchant becomes grateful yet wary, trades cautiously, and tells a guard what they witnessed. If the player demonstrates control or resolves the merchant's concern, some behaviors recover. After a Rewrite, another witness recalls a conflicting sequence and responds differently. Re-entering the town and reloading preserves appropriate divergent reactions.

**Implementation preference:** Use Unreal AI Perception for sensory stimuli, StateTree / Behavior Trees for decisions, Smart Objects for meaningful daily interactions, and event-driven world facts/Gameplay Tags for consequence propagation. Keep authored quests and world truths authoritative. An optional language model may later vary *presentation* of speech within facts known to the NPC; it must not fabricate quest outcomes, discover secrets on the NPC's behalf, or control essential gameplay. NPC AI must work offline without inference service fees.

**Performance design:** Simulate near-player NPCs in detail; run coarse routines and relationship updates for distant NPCs; update on meaningful events rather than asking every NPC to reason continuously. Persist compact facts and stream state by region. When an ambient resident becomes important, promote their existing ID, traits, and memories into a richer simulation tier; test demotion/promotion and save/load for continuity.

## 12. No-PC playtesting and development access

**User constraint:** The creator currently has an Android phone, but no PC. The shipped game remains PC-first and Unreal-native. Phone testing is a development access route, not a mobile release promise.

**Two viable remote paths:**
1. **Remote cloud PC** (e.g., a GPU-equipped Windows environment accessible from Android): useful for opening Unreal Editor, reviewing project assets, and testing builds interactively. Keyboard/mouse or controller may make use easier. Verify chosen instance's VRAM, RAM, disk space, Unreal compatibility, accessibility, cost, and availability before purchase.
2. **Unreal Pixel Streaming** for focused gameplay reviews: run a packaged game on a provisioned GPU instance; stream audiovisual output plus input to a secured browser page on Android. Use on-demand sessions, stop billing when idle, require access control, and verify remote networking / TURN / HTTPS as needed. A phone browser does not natively execute the Unreal PC build.

**Recommended phased validation:**
- First, develop a minimal controller + NPC reaction + Fold demonstration and test data/state logic with automated checks in an Unreal-capable build environment.
- Next, provide recorded videos and screenshots where interactive remote streaming is not yet provisioned.
- Then, enable on-demand Pixel Streaming on a cloud GPU for hands-on Android testing; add touch overlay or Bluetooth controller mapping as necessary. Monitor streaming latency separately from gameplay responsiveness.
- Finally, obtain native Windows hardware tests from an external tester or an accessible cloud Windows PC before claiming PC release readiness. Mobile streaming alone cannot validate native PC performance and input behavior.

**Cost and capability guardrails:** Cloud GPUs, storage, bandwidth, and build minutes may incur charges; no cloud instance or paid service is assumed to exist yet. Standard GitHub CI cannot be assumed to have installed Unreal Engine, licensed engine source, a hardware GPU, or sufficient storage. Establish an appropriately configured build host when implementation begins. The assistant can contribute design, code, repository changes, and test scripts through available tools, but cannot claim to have launched or visually verified Unreal gameplay without a working engine runtime.

## 13. Unreal Engine technical concept (proposal, not implementation)

**Engine:** Unreal Engine 5, PC-targeted project. Choose a stable version and source-control/LFS policy during implementation planning.

**Independent gameplay units:**
- **Character + input:** movement, camera, interaction, equipment, action requests.
- **Combat:** hit detection, defensive states, enemy behavior, damage/readability.
- **Fracture service:** validates ability requirements and cost, switches an authored target between supported states, announces effects.
- **Strain state:** manages personal cost, recovery, warnings, and mitigation.
- **World-state ledger:** persists resolved local changes and supports quest/NPC subscriptions.
- **NPC routines and dialogue:** senses, memory provenance, belief revisions, motives, social rumor propagation, adaptive schedules, contextual dialogue and behavioral choices.
- **Lexicon:** evidence, interpretations, and unlock conditions.
- **Quest rules:** branching resolution keyed to world-state facts, not to hardcoded level scripts.
- **Save/load:** captures only authoritative persistent values and reconstructs presentation correctly.

Prefer Unreal's native Enhanced Input and data-driven assets. Consider Gameplay Tags and the Gameplay Ability System for abilities when their complexity justifies it. Blueprint implementation may speed the first playable iteration; performance-sensitive or foundational systems can be moved to C++ as justified by profiling and maintainability. No final engine-version, plugin, code-language split, or world-streaming strategy is fixed here.

**Error-handling invariant:** unsupported transitions fail safely; missing authored state data never silently deletes quests or objects; save migrations must be considered before the first distributable release.

## 14. Design risks and responses

- **Scope creep:** prove the loop in one small region, then grow it in measured stages.
- **Opaque consequences:** expose risk categories and teach cause/effect through repeated examples.
- **Simulation cost:** continuity of identity and salient memory for every identifiable NPC; rich schedules and motives for major characters; lightweight background representation, event-driven offscreen changes, and bounded memory storage.
- **NPC coherence:** separate observed facts, rumors, and authored reality history; deterministic fallback behavior when any optional narrative generation is unavailable.
- **Creator has no PC:** prioritize Android-accessible cloud review and automated tests, but require later native PC testing; do not imply cloud GPU sessions are free.
- **World-state explosion:** authored alternatives keyed by explicit state tags; cap interacting variables within a region.
- **Generic fantasy feel:** cultural naming grammar and surprising everyday-life details, not arbitrary misspellings.
- **Combat overwhelmed by magic:** physical mechanics must stand alone; fractures expand tactical options.
- **Language becoming homework:** short, contextual, optional discovery puzzles with useful rewards.

## 15. Pending decisions for subsequent design passes

The foundational vision is approved through discussion, **but the following are not approved or finalized**: final names and language phonologies; exact character creation depth; final combat controls and difficulty; art direction/style guide; HUD/UI; engine version and hardware targets; mature-content rating; voice/audio direction; progression balance; and final production milestones; numeric NPC memory capacities and rumor propagation rates; cloud GPU provider, ongoing hosting budget, and control scheme for Android tests. Their absence must not be mistaken for permission to invent constraints silently.

## 16. Next step after review

Review and approve this document as the v0.3 design baseline. With approval, prepare a phased Unreal Engine implementation plan for the first vertical slice, including source-control setup, asset strategy, tests, build/run instructions, and milestones. Commit or publish to the GitHub repository **only with explicit authorization**.

## 17. Technical references (reviewed 2026-10-08)

- Epic Pixel Streaming: https://dev.epicgames.com/documentation/en-us/unreal-engine/pixel-streaming-in-unreal-engine
- Epic Pixel Streaming hardware/browser support: https://dev.epicgames.com/documentation/unreal-engine/unreal-engine-pixel-streaming-reference
- Epic AI Perception: https://dev.epicgames.com/documentation/unreal-engine/ai-perception-in-unreal-engine
- Epic StateTree: https://dev.epicgames.com/documentation/en-us/unreal-engine/overview-of-state-tree-in-unreal-engine
- Epic Smart Objects: https://dev.epicgames.com/documentation/unreal-engine/smart-objects-in-unreal-engine---overview