# THE UNMADE — First language discovery

**Status:** Source-only Unreal integration. No editor or gameplay runtime verification.

### Optional discovery chain
- The prototype has one provisional invented word: **VEYL** (term ID `term.veyl`). Its authored meaning is *the way that remains*.
- Using **Glimpse (F)** successfully near the fracture awards `evidence.glimpse`. Merely pressing F when out of range gives no clue.
- Speaking to the records keeper (`npc.archivist.001`) with **E** awards `evidence.archivist`. This is independent of the local LLM or generative dialogue.
- Both distinct clues unlock the full interpretation. Duplicates award no progress. Once understood, **E** near the fracture reads the inscription. This is optional world knowledge, not a quest blocker.
- The evidence merges into the prototype save alongside NPC memories, player Strain and Rewrite variant; invalid saves are not overwritten.
- Word and interpretation are provisional narrative material, not finalized cultural language design.

### Actual engine QA (not performed)
1. Compile Unreal Editor and execute `Unmade.Lexicon.Evidence`.
2. At a fresh save: pressing E near fracture yields unreadable inscription.
3. Glimpse near fracture: 1/2 clues. Repeating Glimpse must not count twice.
4. Speak with archivist: interpretation becomes understood. Reverse clue order in a fresh save to verify.
5. Reload and inspect inscription: still understood. Rewrite and NPC memories must remain intact.
6. Keep Ollama disabled, or uninstall it: the clue chain behaves identically.
7. Corrupt a copied saved evidence ID: restoration should reject it rather than invent evidence.
8. Test speaker proximity priorities: if a resident and the fracture are both nearby, NPC interaction currently takes precedence.

### Verification available without Unreal
CI executes a pure C++17 lexicon state-machine test, plus Python structural checks. That does not prove a live scene, GUI, gamepad action or save operation.
