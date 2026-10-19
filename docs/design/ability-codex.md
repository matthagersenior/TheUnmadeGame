# THE UNMADE — Ability Codex (source milestone)

**Canonical index:** [`Authoring/ability_codex.json`](../../Authoring/ability_codex.json)

The Ability Codex registers **182** named or defined entries across inherent reality effects, ten earned rites, six Confluences, fundamental combat, combat builds, nine regional techniques, nine realm weapon styles, the entire 60-item catalog (including seven materials), three professions, six workshop recipes, nine field recipes, nine local privileges, creatures, bosses, and observed enemy patterns.

This is a **discoverability, provenance and learning index**, not a claim that all entries are independent usable spells. Every entry has a stable ID, name, category, acquisition and prerequisites, effect, cost, duration, limitations, combinations, observation policy, implementation status and source file reference. Some fields are marked unspecified rather than fabricated.

## Authoritative boundaries

- The JSON is the canonical **index and discovery/learning policy**. Gameplay and save contracts in cited C++ remain authoritative for runtime values until formally migrated. Mirrored numeric values are checked by source parity tests.
- `source_rule` means deterministic offline rule exists; `prototype_handler` means Unreal gameplay code exists but has **not** been compiled and played in the Editor; `definition_only` means no credible acquisition path or complete active effect is established. The `proposed` designation must only be used for explicitly future designs.
- Equipment can confer attack/armor, consumable recovery or explicitly named mechanics. **Item lore is not evidence of a unique executable spell.** The catalog identifies items with only basic bonuses for improvement.
- Enemy powers are labeled **observable, not automatically learnable**. Understanding an attack's counterplay does not transfer the attack to the player. The game does not currently provide a general enemy power-copying system.
- Do not turn the observation policy into a global hidden XP meter or inventory of involuntarily copied powers. An observation must be physically sourced, reliable, repeatable and context-specific.

## Intended discovery-to-mastery loop

1. **Encounter:** The player physically sees a repeatable action or magical principle. A personally witnessed event is not the same thing as a rumor, a forged document or a rumor repeated by an NPC.
2. **Understand:** Read its tells and evidence, survive/counter it, ask a relevant resident if appropriate. Record credible understanding in a journal with source provenance.
3. **Experiment:** Reproduce a bounded action **only when an authored training pathway exists**: skill gate, materials/teacher or witnessed trial, real time and location, Strain/stamina, safe target and cooldown.
4. **Earn:** A legitimate mastery test allows a save-backed unlock. No generic observation automatically grants a combat move, boss ability, NPC profession, identity or forbidden technology. Consent, memory and other characters' agency remain intact.
5. **Transform:** A verified mastery can later unlock authored combinations with other skills or relics, with predictable costs, genuine tradeoffs and observable local consequences.
6. **Return:** Witnesses and settlements remember what happened; subsequent quests, dialogue, access, prices or structures change only from credible saved decisions.

The sequence above is **approved design direction and catalog metadata**, not a statement that all six phases have player-facing runtime interfaces.

## Acquisition taxonomy

- **Inherent:** fundamental Glimpse, Fold, Rewrite, Strain recovery and fracture interruption.
- **Studied/quest-mastered:** five-stage authored reality rites; eventual combined challenges require dual mastery and credible witnesses.
- **Earned/learned:** real Insight purchases, region field techniques, practice-based crafting skills, earned recipes and tools.
- **Equipped/consumed:** permanent ownership is separate from slot-limited equipment bonuses and single-use consumables.
- **Gift/service:** earned local privileges and relief are finite and geographically bound, not permanent learned spells.
- **Observed:** enemy patterns are readable and can teach counterplay, but require a new authored implementation before they are acquired.
- **Definitions reserved for future:** cataloged resources or artifacts can exist with no verified acquisition or active mechanic. Do not grant them silently.

## Validation and gaps

From the repository root:

```bash
python Scripts/validate_ability_codex.py --report
python -m unittest Scripts.tests.test_ability_codex -v
python -m unittest discover -s Scripts/tests -v
```

The validator checks unique IDs, mandatory fields, real source paths, no automatic observation grants, complete 60-item inventory with modifiers, the rite IDs and exact costs, Confluence pairings, regional weapon and technique identity, creature/boss/guardian names and learned recipe coverage. Static CI runs the report and the regression tests.

The report separately identifies:
- **Definition-only items** lacking an earned acquisition mechanism.
- **Observed enemy techniques** which currently have no authored player learning path.
- **Stat-only equipment** whose lore implies richer powers than the current effects implement.

None of these categories should be disguised as finished content. Future work can add source-backed, tested mechanics and an explicit persisted research/learning ledger. Do not mutate existing saved enum IDs, imply AI-generated factual memory, grant a learned profession by rumor, invent testimony, or undermine the tenfold rite quest gates.

## Release boundaries

The Codex and its tests can be authored and exercised without the Windows game PC. No claim of imported assets, compiled Unreal gameplay, UI integration, combat balance, engine save-reload, enemy capture, or packaged play is made by this source milestone.
