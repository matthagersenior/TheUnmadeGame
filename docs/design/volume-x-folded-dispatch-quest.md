# The Folded Dispatch — source-to-Unreal playable quest contract

**Release slice:** Volume X, Chapter 19. Reuses actual Braid outcomes. Does **not** automatically create a traveling NPC or new Unreal level.

## Quest identity

Quest ID: `VOLX.DISPATCH.01`, optional side-story without a compulsory campaign gate. Source triggers are bound to existing Crossings and Bellwold neighborhoods. The player is the courier in this initial version, not Kesta.

## Interaction state machine

| State | Trigger | Physical objective | Saved information | Actor/world change | Failure / recovery |
| --- | --- | --- | --- | --- | --- |
| Before resolution | none | finish optional Braid honestly | prior Braid state only | no packet spawned visibly | no unearned note |
| Waiting | saved Braid outcome 1 or 2 | E within 260cm and LOS of `WitnessDispatch.FoldedRecord` at Crossings (-845,-680,90) | custody stage 0 (implicit in old slots) | small paper proxy visible | blocked LOS, closer NPC, or bad save: no collection |
| PlayerCarrying | actual pickup | travel on foot through existing world to Hessa, not any nearby character | stage 1, valid collection day | source prop hidden and uncollidable | interrupted journey persists; no repeated XP |
| HandDelivered | E beside actual Hessa NPC with clear LOS within 265cm | directly hand over one specific version | stage 2, receipt day no earlier than pickup | Hessa's authored response changes | bad save: rollback, no false receipt; repeated E cannot duplicate |

**Variants:** Sheltered Thread packet concerns a private refuge direction, Public Docket packet reports redacted civic contradictions. No protected guests are identified. Story reference: `docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md`, sections 17–19.

## Staging and player feedback

The letter is a minimal raised cube tagged `WitnessDispatch.FoldedRecord`. It should eventually be replaced with a readable folded sheet displaying no guest's name. Future picked-up animation must not freeze the camera, break third-person movement or be unskippable for screen-reader users. The existing E action is reused, so no hidden new control is required.

The current graybox prints “FOLDED RECORD COLLECTED” and marks that custody is saved. The J journal includes the status while carrying. Hessa speaks only after delivery, and the response is tied to her canonical stable ID `npc.bellwold.matron.001`; no global per-NPC quest completion is inferred. Adding an unrelated resident near the Crossings should not let the player misdeliver the object.

## Exact dialogue drafts for animation

When carrying the Sheltered Thread:

> HESSA: “You carried Orrel's refuge guide here yourself. I can protect these travelers; I still owe the unseen a hearing.”

When carrying the Public Docket:

> HESSA: “I hold the redacted docket you brought. You kept their names off it; now I must decide what a fair hearing can safely ask.”

Before receipt, Hessa's routine, concern, and other authored return lines remain hers, with **no magical revelation of a packet's contents**.

## Save/safety invariant

Use the same `UUnmadePrototypeSave` instance, optional fields only. Loading a schema-1 game with no dispatch data defaults to Waiting. Stage 1 requires a prior valid collection day and a resolved Braid. Stage 2 requires a collection date not later than its receipt. On any malformed field, reject future writes so known data is not overwritten. On a failed `SaveGameToSlot`, restore the in-memory custody snapshot, keeping the same available packet or carried document. A missed or rejected handoff cannot grant its ending.

## Windows and accessibility acceptance

- Launch UE Editor, compile UHT and C++; the portable C++17 CI alone is insufficient.
- Approach and see the packet from multiple directions; verify no NPC/shelter collider steals E and the letter is actually reachable at 260cm.
- Cross the world with the packet, restart and verify carried state stays; re-enter Crossings and verify there is no duplicated ground packet.
- Face Hessa within 265cm and test with clear view versus occluded; ensure E cannot deliver to any other character.
- Save failure injection, corrupt stage 3, impossible pickup day 0 and receipt earlier than collection must not change the prior file or award a false delivery.
- Confirm both Braid alternatives produce distinct lines and old saves with no optional dispatch field still load.
- Controller E-equivalent, subtitle readability, high-contrast interact focus, narration optional, memory accessibility and scalable notebook text require an actual UMG production pass.
- Future dynamic NPC deliveries require actual pathing, permissions, a timed route, failure handling and delivered evidence, not a timer granting remote knowledge. This is **not** implemented in this milestone.

## Next after this quest

Extend to a 4-scene optional chapter: Hessa's next-day response, the choice to keep or publish her answer, a timed physical Kesta dispatch with observable travel, and a return to the Crossings where Orrel must respond to what has genuinely arrived. Preserve all prior SaveGame outputs and the unresolved original question.
