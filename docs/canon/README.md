# THE UNMADE — Canonical Dependency Ledger

**Status: enforceable, source-level cross-reference. Unreal Editor/PIE and package verification are still outstanding.**

[Browse the ledger](dependency-ledger.json).

The ledger ties five otherwise independent registries together: **60 ordinary inventory items, 18 separately owned regional weapon/armor choices, 19 learned disciplines/techniques, 39 named encounters, and 95 grouped quests**. Content is grouped by source family rather than by every NPC interaction; the latter remain tracked by the C++ encounter code.

## What this ledger guarantees

- Every ordinary item has a source-backed grant, vendor, recipe or unique forge, **or is explicitly reserved**. Multiple legitimate ways to obtain an item are recorded separately.
- Each regional expedition presents two mutually exclusive equipment rewards and a learned selectable technique. These are not silently added to `ItemCatalog`.
- Every learned ability has a physical inscription/teacher or a prerequisite quest. A grant description cannot authorize an invented skill.
- Every named boss has a location, access conditions, attacks, resolution routes, and a saved consequence or reward. Killing and pacification are not interchangeable.
- Every major quest has an entry condition, player choices, reward type and local persistent aftermath. Resource/world rewards are not fabricated inventory.
- Core lore constraints remain visible: evidence provenance, consent, human agency, mysteries that stay open, and earlier decisions surviving the final boss.

The C++ data remains authoritative. This file is a **validated index**, not an independent mechanism that can grant anything in gameplay.

## CI gate

Run from repository root:

```text
python Scripts/validate_dependency_ledger.py
python -m unittest Scripts.tests.test_dependency_ledger -v
```

The validator runs in GitHub's **Static checks** on push and PR. It reads actual source registries and compares item IDs, names, stats and acquisitions; regional weapons/armor and their values; rite teachers, quests and mastery rewards; 3 original bosses, 9 expedition bosses, 9 wing guardians, 6 Echo guardians, 11 road elders, and Nhal-Vey; plus the major quest families, exact aftermath branches, and links. It fails with a `CANON DRIFT` diagnostic if a source change is not reconciled here, or if ledger references are dangling.

The validator deliberately fails closed if an existing C++ registry stops matching its recognizable literal format. If the registry changes shape, update the parser and test within the same PR rather than suppressing a failing gate.

## Already identified open conflicts

1. **AtlasOfAbsence** exists as a mythic charm but has no source grant, shop, forge, or recipe. It is correctly marked `reserved` until a legitimate acquisition route is implemented.
2. **TomorrowDebt** rite prose advertises *The Unpaid Sunrise*, but the actual awarded item is `FutureReceipt`, named *Tomorrow's Paid Receipt*. Both names are recorded to expose the mismatch—not silently declared equivalent.
3. **Main campaign versus atlas attunement** needs a clean-save traversal acceptance test. Optional exploration rewards currently raise travel attunement; verify that the player can reach Auvren without unintentionally mandating optional questlines.

Do not invent a new reward or change canon just to make this document look complete.

## Release and content expansion workflow

When adding a weapon, wearable, enemy, boss, quest, region, skill or reward, update its authoritative source and the matching dependency record in **the same PR**. Verify the learning/acquisition path, give each combat encounter fair and visible counterplay, and record both local and durable consequences. Update `docs/lore/living-world-continuity.md`, `docs/lore/CONTINUITY_LOG.md` and the existing illustrated publication receipts when gameplay source changes, according to the independent lore gate.

The ledger does **not** certify level geometry, skeletal animations, voices, visual storytelling, performance, accessibility, save restoration in Unreal, or a packaged game. Those need a full Editor and playtest pass.
