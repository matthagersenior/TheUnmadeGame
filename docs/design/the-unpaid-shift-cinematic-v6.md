# The Unpaid Shift — Cinematic playable quest v6
## Production-ready authored sequence contract (Unreal implementation target)

**Canon:** Volume X §25. **Status:** authored scene/interaction specification; no claim of recorded voices, finished 3D assets or UE playtest. **Invariant:** never overwrite v5 economic truth with a cinematic choice.

### The playable emotional arc
The player meets Bes mid-shift at the Cinderhold coal track. His shoulders follow the weight of a loaded five-basket handcart; his empty hands at day's end signal the absence of payment. He does not immediately ask for help. The player can ask about the route, his home, or the city's shortage; the missed-pay branch unlocks only when the existing ledger actually owes two marks. Camera remains third person until the player chooses to speak; conversation can be skipped without losing earned pay, stock or state.

**Beat 1 — The shift (0–3 min).** Animated coal transfer (pickup, shoulder load, carry, set-down, exhale), footstep gravel and ember crackle. Player sees a stamped shift token and an unpaid ledger entry. Choice UI: **Ask what happened** / **Ask about the hearth** / **Leave**. Asking about pay records Bes's testimony, never pays wages. Leaving does not silently advance quest phase.

**Beat 2 — The house (3–7 min).** Enter Bes's home through an actual door, not a floating inspection box. Threshold changes the soundscape from public forge to low indoor embers and an occasional kettle knock. Interactable physical clues: cold hearth, stacked work boots, small table with unpaid token, folded blanket and a second place setting; **do not assign invented family identities**. The house is a small two-room floorplan: entry/work corner (3×3m), living/hearth space (4×4m), rear sleeping alcove (2×3m). Doorway >= 1.1m, navigation clear width >= 0.9m, low collision clutter, camera-safe ceiling height >= 2.7m. Hearth animation/lighting reads the saved basket reserve, not an independent scene flag. Bes can be found here during walkout; when he returns, his posture and line reflect saved cold days.

**Beat 3 — Rheva's accounting (7–10 min).** Municipal books in a working ledger station. Three natural approaches: **Show the unpaid token**, **Ask about town supplies**, **Ask who authorized the schedule**. Each reveals only what Rheva plausibly knows. Inspection before corroboration remains mandatory. Camera switches between the speaker, the stamped wage book, and Bes's empty shift marker. No generic approval meter.

**Beat 4 — Make a choice (10–20 min).** The world marks four *verbs* with matching focus text, icon, sound and button prompt, each with a cost preview:
- **Carry warmth to Bes** — requires the actual carried five-basket Cinderhold pack; confirmation states *Home warmth +5, wage debt unchanged*.
- **Support the stoppage** — requires genuine owed pay; confirmation states *Bes stops working; Cinderhold produces less until resolved*.
- **Pay from municipal funds** — requires exact full treasury coverage, including any outstanding Mera claim; confirmation shows the amounts before transfer.
- **Seek Mera's guarantee** — requires travel to Saltwake, in-person consent and her actual owned savings; confirmation states *Mera advances up to six marks; the city still owes her*.
All prompts distinguish **Inspect**, **Talk**, **Collect**, **Deliver**, **Refuse shift**, **Settle debt**. Disabled choices show the *specific* missing condition. Hold-to-confirm for financially irreversible actions; immediate E for ordinary observation. No automatic branch selection from proximity alone.

**Beat 5 — Consequence montage (20–23 min).** Do not fabricate passage of days. Play a brief 6–12 second shot only after the real save succeeds: handcart idle for walkout; basket placed inside for relief; marks physically counted for settlement; Mera signs a distinct claim for guarantee. If save fails, restore ledgers and show no success montage.

**Beat 6 — Return (after one or more real simulated days).** On re-entering Cinderhold, trigger at most one history-aware greeting per visit; no forced cutscene. Bes refers to the saved warmth, cold days, wages and walkout; Rheva to public pay records; Ovenna to observed heat; Mera to her personal loan. Never make them omniscient. On a second visit, prefer a short contextual line and allow full conversation optionally. The player's past is evidenced by environmental states: cold/warm hearth, absent handcart, wage book, guarantor notice.

### Exact dialogue fragments and state routing
| Speaker / condition | Line | Choice labels |
| --- | --- | --- |
| Bes, wages current | “It's the last turn that burns. The cart is light by then, but I can't feel my hands.” | Ask about route / Ask about home / Goodbye |
| Bes, missed wages | “The city counted my shift. It didn't count what it owed me.” | Ask for proof / Ask about home / Leave |
| Bes, cold day >= 1 | “The kettle didn't boil last night. I still woke before the bell.” | Offer to inspect / Ask about the ledger / Leave |
| Rheva, before home inspection | “Bring me something from his home that the ledger cannot tell me.” | Return to home / Ask about stock |
| Rheva, verified | “These marks were earned. I can show you the book; I can't pretend there's money in it.” | Ask about treasury / Discuss work stoppage / Leave |
| Bes, after relief | “You brought heat. I won't confuse that with being paid.” | Ask what remains / Leave |
| Bes, walkout | “The cart is still there. I'm not lifting it for another promise.” | Discuss next steps / Leave |
| Bes, settled | “The mark in my hand is mine. The cold nights are still mine too.” | Ask how home is / Leave |
| Mera, before consent | “If I give him my marks, whose ledger will remember I did?” | Explain successor claim / Leave |
| Mera, guarantee active | “His debt is paid with my work. The city hasn't paid for either of us yet.” | Ask what is owed / Leave |
| Nhal-Vey, unresolved stoppage | “You taught a man to stop carrying the fire. Who warmed the others?” | No combat shortcut |
| Nhal-Vey, guarantee active | “You called another person's hunger a solution.” | No combat shortcut |
| Nhal-Vey, fully settled | “A debt paid is not a night returned.” | No combat shortcut |

### Technical handoff for Unreal
**Assets:** `SM_Bes_HomeShell_A`, `SM_Bes_Hearth_A`, `SM_Cinderhold_Handcart_A`, `SM_ShiftToken_A`, `SM_Rheva_Ledger_A`, `SM_Mera_Claim_A`; animations `AN_Bes_LoadCart`, `AN_Bes_CarryBasket`, `AN_Bes_SetDown`, `AN_Bes_WalkoutIdle`, `AN_Bes_HomeCold`, `AN_Rheva_CountMarks`, `AN_Mera_SignClaim`. These are **asset contracts, not claims that meshes/animations exist**.

**Audio:** distinct Bes/Rheva/Mera/Ovenna voice IDs, speaker-anchored subtitles, forge/house/ledger ambiences, cart squeak, coin taps, paper seal, embers; dialogue ducking; accessible captions for every meaningful effect. Placeholder voice must not masquerade as final performance.

**Sequence contracts:** `LS_US_IntroShift`, `LS_US_HomeInspect`, `LS_US_Ledger`, `LS_US_Walkout`, `LS_US_Relief`, `LS_US_Restitution`, `LS_US_Guarantee`, `LS_US_Return`, `LS_US_NhalVeyRecall`. Sequences receive authoritative save snapshot read-only and issue state transitions only via existing hub/economy APIs. No Sequencer event directly edits money, cargo, or quest phase.

**Interaction focus priority:** NPC > usable tagged station only when within 245cm and line-of-sight; show one focus at a time, explicit verb and action cost, no hidden auto-trigger. E or gamepad south button activates, escape/back cancels, disabled choices explain gates. Camera cannot obscure subtitles or a choice menu. Add large-text/high-contrast presets and remappable input.

**Persisted visit metadata:** separate, additive per-character greeting cooldown / first-return flag; never use it to gate economic actions. Repeated visits must be idempotent and work after reload.

### First PC acceptance
1. UE 5.8 UHT and editor compile with zero errors; cooked Windows build launches.
2. Walk from Cinderhold route to interior and ledger with keyboard/controller; camera never clips walls; doorway accessible.
3. Play each branch from an isolated real save; compare marks, cargo, work output, home warmth and historical days before/after.
4. Interrupt and reload at every beat, including failed disk write; no duplicated marks, warmth or cutscene rewards.
5. Observe handcart and Bes's work animation cease on walkout and resume only on real resolution.
6. Return after 1, 3 and 10 genuine simulation days; dialogue references saved facts, not invented days.
7. Test every subtitle, speaker identity, audio cue and interaction affordance with sound muted and large text.
8. Verify Nhal-Vey recalls the correct historical outcome without altering combat balance.

**Release gate:** cinematic-quality playable quest remains **incomplete** until the assets, sequences, UI and end-to-end UE tests above actually exist and pass.
