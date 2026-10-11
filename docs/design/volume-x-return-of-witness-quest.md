# THE UNMADE — Return of the Witness, playable quest specification (Volume X §20)

**Quest:** VOLX.RETURN.01. Optional cross-city story arc. **Playable source in this milestone**, **not** Windows Unreal Editor or end-to-end verified.

## Deterministic route

1. Orrel's original Folded Dispatch physically arrives at Hessa (source `Dispatch.Stage()==HandDelivered`). Her day of original receipt is stored in the first custody ledger.
2. Hessa continues daily life. If the current game day equals that receipt day, the chapter is not actionable. The world journal explains why; travel, trade, combat and other main quests continue normally.
3. On a later world day, approach Hessa (stable ID `npc.bellwold.matron.001`, radius 390cm); F7 selects *Private Counsel*, F8 selects *Public Hearing*. The existing `CommitmentGate` requires the SAME choice within six seconds by the SAME witness. Another final quest can take priority according to existing rules.
4. An actual `WitnessReturn.ReplyNote` actor exists near Bellwold's refuge when the route is prepared. Standing within 255cm, with clear line of sight and a closer note than the nearest NPC, E takes the reply. The world actor becomes hidden/noncolliding. Custody is saved without any duplicable XP.
5. Travel back to the Crossings. E beside Orrel (`npc.bridgekeeper.001`, radius 260cm, line of sight) records delivery. No other NPC, map pin or completed stage of a different quest substitutes.
6. The answer becomes visible to Orrel himself in future dialogue. One of two physical monuments appears beside the crossing according to the *return disclosure route*; four exact lines depend on the original Braid disposition as well.

## Consequences matrix

| Original Braid | Reply method | Orrel's actual response | Material continuity |
|---|---|---|---|
| Sheltered Thread | Private Counsel | Keep the private crossing, admit owed builder wages | Low private marker; no redacted guests named |
| Public Docket | Private Counsel | Reconcile existing published discrepancy with private access | Low private marker, prior docket unchanged |
| Sheltered Thread | Public Hearing | Safe refuge road also becomes an accountability hearing | Taller hearing marker, prior refuge cord remains |
| Public Docket | Public Hearing | Public question advanced, no names and no cosmic verdict | Taller hearing marker, public docket remains |

These four responses do not create four morally ranked endings, and the single optional side story never overwrites the main finale.

## Branch, trigger and recovery details

- **Old save:** all return fields absent; stage zero with valid world clock. The player continues their existing story.
- **Corrupt save:** nonzero route without reply, impossible future dates, choice on/before original receipt, delivery before pickup, stage outside 0–3. Loading rejects the optional snapshot and future writes are protected until repaired. All other story save history remains present.
- **Failed save:** stage, route and dates rolled back to the prior domain snapshot; world actor visibility does not update, so neither pickup nor delivery is falsely shown.
- **Interrupted journey:** carrying stage and collection day persist. A day passing does not delete the reply.
- **Wrong speaker:** only the two stable authored identities can confirm the choice/receive the note. A stranger has no authority.
- **Blocked sight/another closer actor:** no E progression; walk around obstruction or move closer to the note.
- **Repeated input:** route cannot be reselected; paper pickup and Orrel handoff are single-use and do not grant infinite rewards.
- **Hessa and Orrel source text:** Hessa discusses the selected route only after the choice was made. Orrel refers to the specific delivered reply only after actual receipt. Other residents remain uninformed unless their own physical witness/rumor system informs them.

## The authored scenes

**Night before (optional preflight/dialogue):** Hessa rinses the chipped cup, does not finish the letter while refugees are asking for water. This is a proposed animation scene, not yet a filmed Unreal cutscene.

**The next morning:** She gives two equally defensible requests and refuses to name a guest. Player can choose neither and go elsewhere. Player picks up the reply when ready.

**Crossings resolution:** Orrel unfolds the paper twice beside three physically disagreeing surveys. The lower private marker and taller public marker should use distinctive texture/audio cues, not a generic burst of colored light.

**Return years later:** Orrel recognizes the player's physical part in keeping both towns connected; an expanded multi-year aftercare quest and full voice performances are future production tasks.

## Windows / Unreal acceptance

- UE 5.8 source/UHT compile + PIE spawn of source actor tags and correct initial hidden state
- Interact priority and line-of-sight from all approach directions at Hessa and Orrel
- F7/F8 two-press protection, local quest priority, controller navigation and accessible hint text
- Wait one in-world day (clock uses 20-minute day), test exact boundary before/after
- Save/reload at every state, backward schema-1 compatibility, malformed and failed disk write injection
- Verify all 4 crossing original-vs-reply permutations, monuments mutually exclusive, prior monuments preserved
- Named authored NPC state and bounded rumor propagation; no remote auto-knowledge
- Proper paper props, animations, subtitles, audible voices and responsive UMG evidence atlas remain to build
- Full route movement, enemies and interruption recovery in actual UE is not yet tested

## Next campaign expansion

An optional late follow-up would connect the Bridge Ledger Court with Saltwake's dry tariff and Drevlach's future-dated invoice as **distinct evidence**, requiring a new physical expedition, faction stakeholders and optional moral complications. No scripted revelation should solve the Unanswered Interval simply because the player follows these leads.
