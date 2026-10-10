# The Unmade — The Lived Universe Production Bible

2026-10-10 • preproduction source companion • nine realms, 27 clues, 18 daily scenes, nine optional quests, 90 non-authoritative work orders

## Status / ownership

This preproduction source chapter extends daily life in all nine realms without changing canonical source quest progress, SaveGame, inventory or the boss. The master file is Authoring/lived_universe_atlas.json; its generated 90-order reference plan is Authoring/generated/lived_universe_scene_manifest.json. Scripts/validate_lived_universe.py --check prevents drift. None of the new scenes, actor labels, side quest flags or assets is a verified Unreal actor, compiled level, voiced line or gameplay implementation. Existing main quest, 82 stable residents, 10 rites and Nhal-Vey's FinalJourney remain authoritative.

## World-first storytelling

A universe feels lived in when ordinary behavior expresses its history. A caretaker setting aside a cup before anyone arrives is more compelling than a paragraph explaining that their refuge officially never existed. Every place must have an ordinary human need, physical routine, culturally meaningful refusal, discoverable contradiction, reaction to player conduct, and changed-but-recognizable aftermath. Player freedom includes ignoring optional scenes. The world should retain context rather than framing all society as puzzle dispensers.

## Nine lived realities

The Threefold Reach ties lantern shelter, meal chits and contested registration to a bell that lacks its final note. Saltwake repairs a dry ferry berth and measures rain against an erased harbor. Cinderhold's bakers and vent technicians owe duties to an impossible fire, not ownership of other people. Drevlach's accountants and merchants distinguish true observed repairs from futures falsely pledged before birth. Orravane's singers, builders and rest-keepers struggle to hold suspended homes without treating exhaustion as a permanent obligation. Vathless repairs inhabited masonry where erased workers' names still hold roofs up. Eillun makes safe navigation possible without coercive birth registration, using volunteered doorstep gestures. Tharniv's orchard growers must balance public food and unsigned law without appointing a monarch by default. Auvren maintains safe public thresholds and contradictory founding witnesses without turning the player's impossible origin into an imposed royal bloodline.

## Physical evidence contract

Each LUE clue lists the exact site, player verb and grounded discovery. The editor staging task must add reachable collision-safe approach, proximity and sight rule, keyboard/controller focus, tactile surface wear, sensory cue with subtitle/visual alternative, personally limited witness, journal interpretation and meaningful competing hypothesis. Interruption, wrong angle and accidental repeated use cannot grant fake quest progress or destroy the evidence. The same clue remains inspectable after local permanent choices and in either postboss morning.

## Daily resident loop

Every LUR scene supplies location, daypart, trigger, physical action, individual response, a complete spoken line, world feedback, failure, recovery and persistence policy. The game must simulate arrivals, work, meals, shelter, rest, repairs and credible social disputes: don't teleport residents whenever the camera turns. Schedule causality persists while streamed out; a resident can only remember firsthand events or attributed, legitimately delivered rumors. A public fact does not magically provide private eyewitness memories across distant realms. Respect all 82 stable identities.

## Optional quests, no grind

Each of nine LUQ side stories includes exactly five physical beats and two different, equally defensible care/truth outcomes. They are voluntary and never unlock a mandatory act, travel gate, mastered rite or Nhal-Vey entrance. A safe route and retrievable clue remain after errors; repeated submissions never farm rewards. The two-choice interaction shows exact permanent consequences, requires deliberate confirmation, and writes atomically. These new optional side story save slots are unimplemented; until a specific versioned isolated save contract is tested, the spec must not pretend that outcomes persist in-engine.

## Sensory and accessibility

Use distinct acoustic spaces, materials, temperature cues, labor noise and purposeful empty intervals in each realm. Render cues must never be audio-only. Every warning has a non-color-only silhouette, caption and optional high-contrast representation; player remapping is stable, even when reality distorts. Camera, horizon and animation shake require reduced-motion/static-horizon modes. All dialogue and films support pause, skip, subtitle speaker identity, reading speed, audio ducking and localization. No automatic button inversion. A difficult encounter must have learnable windows and recoverable failures, not arbitrary trap damage.

## Cross-realm mystery boundaries

Nine LUL links use inspectable public artifacts, earned crossing, local testimony or delivered records; no telepathic automatic reveal. The central Unanswered Interval is the pattern, but every society has a distinct responsibility for exploiting it. Bell silence becomes a storm tariff; future dates appear in financial instruments; flame rhythm corresponds to a load-bearing missing note; birth ledgers overlap erased stone names; voluntary Eillun addresses appear in Tharniv's orchard petitions. Until the source atlas says a route is earned, a clue may suggest it without pretending immediate travel. Auvren arrival material must not spoil the false boss victory or the ending choices.

## Hint ladder and humane failures

Optional hints unlock at 60, 120 and 180 seconds of meaningful puzzle stall: first evidence category, then safe physical place, then action order without auto-solving. A wrong physical interaction dims a lantern, produces a refused signature, tips a scale or requires a fresh safely retrievable brace. Death, accidental permanent debt, coerced names and irreversible route locks are never employed as consequences for curiosity. The canonical environmental pulses still own actual damage; never add invisible new hazards in scene scripts.

## 90-stage Unreal handoff

For every work order provide an authored level and actor ownership, stable key and source binding, streaming cell, collision/navmesh and safe bypass, material and LOD plan, animation, performance direction, voice take, caption, actual physical interaction, repeat/interrupt behavior, persistence owner, explicit rollback, local eyewitness limits and reference screenshot or captured test. GUIDE_* labels are no-collision, no-gameplay-authority work tickets. The manifest contains 27 clue sites, 18 daily scenes, 9 optional stories and 36 art/audio packages. New SM_/NS_/SFX_/SEQ_ names do not mean those Unreal binary assets exist.

## Bells and a human problem: sample staged sequence

In Bellwold, a caretaker counts refuge cups and deliberately leaves one uncounted. The player carries a kettle along a worn threshold; steam reveals a physically safe line, and a meal chit contradicts the age of a building. A Paperhaven registrar explains why public documentation and protected personal identities must be separated. The optional wick story offers either an anonymous shared shelf or an opt-in ledger. On a return day the player sees a newcomer use the chosen provision, and the caretaker recalls only what they witnessed. In Held Morning the shelter scar is readable on one wall; in Many Mornings a static guide makes both routes navigable. The player can bypass the entire optional sequence and still finish the original opening act.

## Voice, performance and cinema

For each identified person, plan three expression passes: ordinary labor, public moral dispute, private return. Document intonation, pace, gesture, interruption, posture, proximity, local reverberation, facial needs, subtitle timing, VO recording stem and fallback text. Don't hand another character's speech to an interchangeable NPC. Each directed film must begin from an inspectable world object, never replace physical quest action, and remain skippable and reload-safe. Public trailers should not reveal the mask fracture, two future outcomes or optional rematch.

## Engine acceptance (still outstanding)

Validate source with python Scripts/validate_lived_universe.py --check and python -m unittest discover -s Scripts/tests -v. On an actual Windows host, run Scripts/first_pc_build_and_test.ps1 before binding assets. Verify every region's 3 physical clues and 2 daily reference scenes, two save branches on independent saves, no reward duplication, accessible inputs and safe routes, world return after local and global choice, original main quest unlock independence, NPC witness limits, navigation and collision, packaging and performance on a selected reference system. No static validator makes a scene compiled or playable.

## Remaining pre-PC production debt

This adds specificity but does not finish every personal relationship arc, all 82 distinct full dialogue trees, every town's daily schedule, full bestiary, nine authored geographic map layouts, combat balance, complete voice score, cinematic camera keys, all localizations, optional LUQ save implementation, or comprehensive QA. Those elements still require iterative creative production as well as Unreal. The honest completeness condition is evidence-backed acceptance for the entire scope, not declaring that a very large document proves there are no omissions. Expand details in source until every accepted interactive moment has a known owner, state transition, physical actor, error/recovery and verification contract.

