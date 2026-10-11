# Reality Consequence Engine v5 — Lives Behind the Ledger: Bes quest acceptance

**Canon:** Volume X, Chapter 25. **Scope:** Cinderhold / Saltwake first individually authored household branch. **Status:** deterministic C++17 and Unreal source with CI tests, **not yet Windows Unreal Engine UHT/Editor/PIE approved**.

## Starting facts, geographical grounding and resident authority

- Bes is the *existing* canon resident `npc.cinderhold.courier.001`; Rheva is `npc.cinderhold.emberwarden.001`, Ovenna is `npc.cinderhold.healer.001`, and Mera is `npc.saltwake.courier.001`.
- Bes's physical home is placed at local Cinderhold XY=(+1210,+610) cm relative to the realm center (0,+50000). It has its own private basket reserve; no other family member is fabricated.
- The wage complaint is impossible until the existing `LivingEconomy` Cinderhold employer has **actually** missed at least two marks. Speaking to Bes then records his first report. The player must E-inspect the nearby home, then speak to Rheva to establish a distinct municipal witness. Merely standing at a quest marker or seeing a faraway journal entry cannot fake this evidence.
- Ovenna reads the public/local warmth condition without knowing other cities' private conversations. Mera may offer a guarantee only through her direct conversation at Saltwake.

## Three Cinderhold physical choices plus named Saltwake consent

| World actor tag | Exact local position (relative to realm center) | Action, gate and actual cost |
| --- | --- | --- |
| `Bes.Home` | Cinderhold +1210/+610 | First E inspects Bes's private hearth. After Rheva corroborates, carrying exactly one previously withdrawn **5-unit Cinderhold local relief pack** permits a real named-home delivery. The v3 warehouse stock was spent on pickup and the pack disappears on delivery. It does **not** satisfy municipal wages. |
| `Bes.WalkoutChoice` | Cinderhold +1430/+790 | E after real wage verification and an unpaid balance. Bes's routine returns him to his own home. Next actual Cinderhold workdays have **one less productive worker**; Bes receives no fictitious shift credit or new wage claim while refusing. |
| `Bes.RestitutionChoice` | Cinderhold +650/+300 | E after verification and available municipal treasury balance sufficient to cover **both** remaining Bes wages and any existing Mera guarantee. The preflight must check the whole sum *before* moving either account. If insufficient, no partial move or fake “resolved” status. |
| `Bes.MeraGuarantee` | Saltwake +1230/+360 | E only after talking in person to Mera. She may advance up to **six of her own already-owned earned marks** to Bes, lowering his unpaid balance and recording the same claim against Cinderhold owed to Mera. One-time; no repeating consent for unlimited financial transfers. |

The three additional physical view markers are `Bes.ColdHearth`, `Bes.WarmHearth`, `Bes.WalkoutBanner`, `Bes.PaidRecord`, and `Bes.GuarantorRecord`. Only the authoritative snapshot determines their visible/collidable state. Both the public journal and named NPC dialogues must reflect the same saved state.

### Timing and outcomes

A home begins with one private warming basket. Every *actual* material-ledger day consumes one unit. Days with no personal reserve increase `daysCold`, an accumulated historical harm that cannot be retroactively undone. A correctly delivered five-basket pack raises only that private reserve; it does not double-count v3 aggregate household stock.

- **Warmth first:** an immediate physical benefit and real supply withdrawal; due wages survive. The player may subsequently support a refusal or seek payment rather than being placed into a false final moral answer.
- **Stand together:** Bes's voluntary work refusal reduces a real Cinderhold production shift and its salary/arrears record; ordinary life and shortages continue. Only credible restitution or a sufficiently funded personal guarantee ends the refusal.
- **Fund public pay:** actual employer marks are transferred to the named worker and any guarantor. If Cinderhold is empty, player must arrange real liquidity (for example receipts from existing finite inter-realm exports)—no invented infinite municipal cash.
- **Mera's risk:** she gives owned funds only by direct consent; if her bounded help covers the full old wage debt, Bes resumes his life but Mera's claim remains visibly unpaid until the town can really make her whole. If the amount covers only part, the remaining claim stays unresolved.

### Returning character relationship

Bes comments on his home warmth, actual days refused and wages still due. Rheva refers to the public municipal record only after it was authenticated. Ovenna comments on observed warmth, not on a private financial negotiation she didn't attend. Mera says what she volunteered and what Cinderhold owes her. The same four stable resident identities and original biographies remain present after saves/visits.

The current game uses existing source-generated NPC motion and text UI; physical apartment interiors, actors interacting with 3D coal baskets, separately recorded voice tracks, lip sync, cinematic subtitles and advanced dialogue choice UI are still planned work. All existing important player interactions and enemy-threat interrupts must retain priority over ordinary NPC routines.

### Nhal-Vey continuity (no victory bypass)

At the existing first mask-break reveal only, the authored source distinguishes (1) municipal restitution; (2) Mera having underwritten Bes but with an outstanding successor claim; or (3) an ongoing unpaid walkout. It never modifies boss health, bypasses phases, grants an automatic ending, or makes the universe's missing sound mystery answerable by finance.

## Fail-safe migration, rollback and testing

New optional schema-1 SaveGame fields store household phase, clock day, finite hearth basket count, recorded cold days, strike days, real relief counts, first report day, home inspection and municipal corroboration flags, Mera's consent/used flag and successor debt. Old saves initialize this personal ledger at their existing in-game day—no forged earlier private gifts.

Strict restoration rejects negative/future dates, contradictory phase/provenance, duplicated guarantee and impossible reserves. The world-save write gate includes `bBesHouseholdRejected`. A failed transaction restores v3 material store + existing carried cargo, all v4 employer/worker wages and the v5 household in-memory record, and must never show success. Domain tests include missing wages, missing home/source testimony, no free delivery, no duplicate guaranteed funds, one less real worker after refusal, no phantom wage growth, snapshot tampering and full-sum preflight.

Native CI: `Tests/world/bes_household_test.cpp`. Integration wiring test: `Scripts/tests/test_bes_household_wiring.py`. Both must pass in the source branch and on final `main`. Check existing v1–v4 tests unchanged.

**Windows UE Editor/PIE remaining gates:** compile/UHT, verify actual actor reachability (245 cm LOS), source-correct NPC proximity, E selection precedence, low-vision/high contrast/object sounds, saved return after time travel and interrupted writes, real AI material movement, actual cinematic/audio, 4-way branch outcomes and exact same fight without different health/damage balance. A green native/source check must not be described as this UE validation.
