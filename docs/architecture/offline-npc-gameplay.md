# THE UNMADE — AI-independent gameplay (approved design addendum)

## Non-negotiable gameplay rule
**The complete game is playable and simulation-consistent when no AI model is installed, loaded or running.**
Local LLMs are an optional dialogue paraphrasing/enrichment layer only. They must never be a prerequisite for a quest, route, combat, NPC action, memory, faction decision, inventory, save/load or conversation exit.

## Native NPC pipeline
1. Unreal sensory events/validated social gossip become identity-bound memories with preserved evidence provenance.
2. Unreal calculates trust/fear; authored traits and role come from each NPC's stable roster identity.
3. `UnmadeCore::ChooseNpcAction` chooses a deterministic behavior based on authoritative state, the specific character's memories, and context. Pure C++17, no Unreal/HTTP/LLM library dependency.
4. Unreal exposes the action via `AUnmadeNpcCharacter::GetCurrentActionId` and trade permission via `CanTradeWithPlayer`. Authored fallback dialogue uses the same action; optional local inference may rephrase the dialogue but cannot change the decision.
5. World systems will implement the selected actions as navigation, trading, combat, faction policies, and quests in subsequent milestones. **Current prototype has the policy and trade permission, not a built shop, full schedules or visual NPC navigation.**

### First version's identities
- Merchant: cautious; offers trade/discount, avoids, or refuses depending on direct evidence and trust.
- Guard: patrols, verifies rumors before treating them as fact, investigates directly witnessed anomalies, intervenes after directly witnessed threats.
- Scholar: shares knowledge with trusted players, verifies rumors, researches witnessed anomalies.
- Courier: follows delivery behavior and avoids danger.
- Wanderer: follows routine, offers help to trusted players, or avoids witnessed danger.

### Fairness and accessibility
Rumors carry less confidence than directly witnessed evidence. Players must be able to correct beliefs using evidence. Major NPCs can later develop more nuanced relationships and priorities. None of these behaviors depends on unpredictable generated content; all significant choices need clear UI feedback.

## AI boundaries
`UUnmadeLocalDialogueSubsystem` is disabled by default. When enabled, it may suggest a short dialogue line locally; validation failures and network timeouts use deterministic authored dialogue. AI must not return gameplay commands or be allowed to write to the world, save or decision systems. The user's machine may have no Ollama or model weights and must still run the game. Do not bundle AI model weights as a required install.

## Tests and readiness
Native C++17 tests in `Tests/npc/adaptive_npc_test.cpp` prove policy outputs for merchant trade, direct-vs-rumor guard decisions, scholarly investigation, traveler assistance and stable repeatability. Python source-contract checks verify Unreal wiring and disabled AI default. The CI commands need no Unreal installation and no AI service.

**Not yet verified:** Unreal C++ compile, in-engine interaction, real trade transactions, NPC navigation and generated voice lines. Windows GPU/Unreal Editor testing is still required.
