# The Living Economy v4 — implementation & first Windows PC acceptance

## Scope and trust model

This is a **first actually transaction-backed named-people economy**, not a complete simulation of all 82 residents. Nine existing stable IDs (one per atlas realm) are bound to municipal employers and separately owned earned wages, unpaid obligations, authored professions and identifiable homes. v3 stockroom materials, v3 household reserves, all existing dialogue, global time, and final-story choices are unchanged. New v4 data is additive to existing schema-1 save slots.

## Physical contracts and locations

| Manifest | Source physical E point | Destination physical E point | Physical quantity | Buyer escrow |
| --- | --- | --- | --- | --- |
| Saltwake to Cinderhold | Saltwake center +1360X/-830Y cm | Cinderhold center +1470X/-180Y cm | 5 sealed rainwater casks | 6 Cinderhold town marks |
| Cinderhold to Saltwake | Cinderhold center +1360X/-830Y cm | Saltwake center +1470X/-180Y cm | 5 common ember baskets | 6 Saltwake town marks |

The player must enter the correct realm, stand within 260cm and have direct line of sight. Stable actor tags are `LivingTrade.0.Start`, `LivingTrade.0.Finish`, `LivingTrade.1.Start`, and `LivingTrade.1.Finish`. Delivery is not a radius-only trigger and does not create a second backpack.

At departure, actual v3 warehouse source stock decreases by five and the buyer treasury reserves six marks. The one live cargo record persists through saved world days. At arrival, the manifest must match, its destination must be the player's actual locale, and the already committed escrow is released to the source employer. The receiving area's named worker records receipt. Different goods remain different imported stocks; casks do not become baskets, and baskets do not become local casks. The only v4 environmental use consumes one foreign support unit during a **genuine shortage day** to avert two points of structural deterioration. There is no automatic trade or simulated remote courier today.

## Named wages and finite ownership

Each of the nine named municipal employers starts with 32 marks. On each **actually simulated** v3 material day, that realm's named worker earns two marks if the employer can pay. The same two marks leave the municipal treasury. Otherwise two become a separate owed-wage entry. A trade receipt credits the source municipal employer, which can then pay actual arrears. Wages are not player money. Actor names and authored roles are preserved.

The sum of all employer balances, all worker-owned earnings and one in-flight contracted buyer escrow is **exactly 288 marks**. A save that invents currency, misattributes a buyer receipt, forges a foreign import, duplicates a cargo, or has impossible dates cannot overwrite the original game slot. Buyer escrow at collection prevents a long journey ending with an empty, unexpectedly unpaid buyer treasury.

## Role play and long-form consequences

The nine named residents state their earned and unpaid wages and truly received shipments in conversations. The J ledger shows the local employer, worker, earning, arrears, receipt history, foreign support held at Saltwake/Cinderhold and any carried consignment. These are persistent histories, not procedural NPC identities.

Nhal-Vey's mask-break reveal checks for **two actual, opposite-direction deliveries**. If both saved receipts exist, one additional optional line acknowledges demonstrable mutual aid. It does not alter boss combat, grant free health, skip the fight, identify the original missing person, or change the main ending.

## Native source validation and required Unreal PC test

Portable C++17 `Tests/world/living_economy_test.cpp` checks all nine stable workers and own homes, wage transfers, shortage debt, duplicate/mismatched cargo, real store stock deductions, exact escrow, foreign reserve identity, conserved 288 marks and corrupt save rejection. Python tests require actual gameplay wiring through Player E, World Hub, NPC dialogue, world clock, Game Save and FinalWorld.

**Still required on a Windows PC:** compile UHT and C++; verify both Start/Finish actors can be reached and selected in PIE while other nearby NPCs/props exist; test save/reload mid-journey; fail/inject a disk save error at departure/receipt and confirm rollback; check finances do not double-pay on clock resume; verify damage reduction only on a real v3 shortage pulse and never when no import exists; verify both Nhal-Vey countertestimony branches and preservation of all original boss mechanics. Validate narrative text, audible/subtitled reading, gamepad input, low-vision interact prompts, production mesh traversal, actor animations, memory/civic continuity and performance under repeated world transitions.

## Next expansion: person-to-person obligations

Expand one exact Cinderhold-Bes household into an authored wage dispute and personal relationship quest with worker schedules, meals, private room, injury, repayment and a player-negotiated plan that resolves differently after a real day. Add consent-based movement of named residents into the two trade routes, no global omniscient scripts, true multi-stop pathing and expenses, and fully authored cinematic consequences. Do not collapse materials, wages, humanitarian aid, trust and truth into a single alignment number.
