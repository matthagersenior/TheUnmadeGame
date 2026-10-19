# The Unmade — Canon-first admission policy

**Effective source gate:** Every new entity, mechanic, area, character, creature, boss, weapon, item, quest, story, ability, skill, cinematic story asset, or world-system addition must have a stable, reviewable canon identity **before merging**. This policy does not equate source implementation with engine-tested gameplay.

## What belongs to the encyclopedia

All additions belong to at least one of the eight [encyclopedia volumes](encyclopedia/README.md): world geography, people, enemies, armory, abilities, stories, lore or social systems. NPCs and creatures may have several facets, but keep **one stable ID** per canonical identity; use explicit source references and relationships rather than silently duplicating or renaming an entity.

A canonical entry must state:

1. Stable ID and unambiguous name, existing authority or new source registry, realm/region or applicable scope.
2. Narrative purpose, physical identity, chronology and relationship to established canon, including truthful uncertainty for in-world claims.
3. Acquisition/encounter/quest trigger or appearance and its **real prerequisites**. Observation is not an automatic skill grant; optional quests stay optional.
4. Limits, cost, counterplay or accessibility for mechanics and adversaries, and relevant inventory, profession, trade or save impacts.
5. Branches, affected NPCs, evidence provenance, consent and persistent outcome, without implying universal knowledge or a privileged ending.
6. **Implementation status:** literary/editorial, definition-only, source rule, Unreal handler, engine tested or publicly released. Do not infer the later stages from an earlier stage.
7. Automated validation/test cases and later Unreal/production acceptance requirements.

## Mandatory review sequence

1. **Register:** Extend the original authoritative JSON/C++ source and the correct linked codex entry; do not make the encyclopedia a second grant system.
2. **Reconcile:** Update [INDEX.json](encyclopedia/INDEX.json) and one or more of the eight human-readable volumes in the same PR. For names, costs, rewards, conditions and old saves, reconcile all mirrored sources and linked entities. If the source format is migrated, migrate the validator rather than disabling it.
3. **For new files:** Add a path to [SOURCE_COVERAGE.json](SOURCE_COVERAGE.json), with a real `codex:<stable-id>` or another existing `<family>:<id>` from INDEX. A new implementation of an existing mechanic can cite the existing stable ID. An entirely new named game feature needs its own source-backed ID and index/volume entry first.
4. **Noncanonical plumbing:** New technical-only source files may be declared `noncanonical_infrastructure` with a meaningful rationale **instead of** making up a game entity. This exception is for tests/adapters/utilities, *not* a way to hide new gameplay.
5. **Verify:** Run `python Scripts/validate_canon_encyclopedia.py`, `python Scripts/check_canon_change.py --base origin/main` from a PR branch, the current domain validators, and `python -m unittest discover -s Scripts/tests -v`. CI checks new sources and direct changes to authoritative registries.
6. **Respect continuity publishing:** Content-changing lore still follows [UPDATE_POLICY](../lore/UPDATE_POLICY.md), including protected mysteries, continuity log and dated illustrated release rules when a new full edition is actually issued.
7. **Merge only on passing required checks** and preserve a clean `main`. Review the final CI outcome and the exact merge commit. Unreal validation is a separate required release gate.

## Mandatory GitHub merge protection

The admission gate can only **block a merge** once GitHub enforces the `source-checks` result on `main`. See [main protection policy and activation](MAIN_PROTECTION.md) and [versioned active ruleset definition](../../.github/rulesets/main-canon-protection.json). The ruleset is a repository administration setting; storing its JSON alone does not activate it. The GitHub connection used for source/PR changes does not provide the Administration write operation, so confirm the live rule separately before claiming merge-time enforcement.

## Scope and limits of automatic enforcement

- `validate_canon_encyclopedia.py` checks existing registered IDs, names, source provenance and readable volume coverage.
- `check_canon_change.py` compares each PR against its base **and every direct push to `main` against the push's previous commit**. New C++ gameplay files, content assets, source authoring masters and lore manuscripts need a [SOURCE_COVERAGE](SOURCE_COVERAGE.json) mapping. Edits to critical machine-readable registries require the corresponding readable volume to change in the same PR.
- A bug fix **without** an added feature or changed canon may reuse existing identifiers; cosmetic or build-only work must not invent lore just to satisfy a checklist.
- **Not fully machine-decidable:** A new mechanic inserted inside an *existing* non-authority file, a hidden plot contradiction, a misleading preproduction claim, or an inaccurate explanation can still escape this check. Existing source validators, source review and first-person playtests remain necessary. Expand the affected validator when a new registry family is added; never falsely claim universal automatic protection.
- GitHub checks only prevent merges if the repository's branch rules make the applicable checks **required** and merges/admin bypass are controlled. Confirm repository rules separately.

## Example SOURCE_COVERAGE entry

```json
{
  "path": "Source/TheUnmadeGame/Public/World/UnmadeExampleNewFeatureRules.h",
  "classification": "implementation",
  "codex_keys": ["codex:rite.UnwriteLaw"]
}
```

For a truly new named ability, first add a stable `codex:<id>` to the canonical Ability Codex; update its readable volume and index, then register the new file against that key. If no source file was added and only an existing registry changed, the authoritative-registry chapter-sync gate still applies.

## Absolute exclusions

The codex must never authorize hidden grind for Nhal-Vey, NPC omniscience, coerced consent, fabricated reward grants, overwritten choice branches, universally canonical personal identity or forced Held/Many Morning outcomes. A source-green test cannot be sold as UE playability or a released game.
