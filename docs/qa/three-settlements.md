# THE UNMADE: Three connected village prototypes

**Source-only implementation. Neither Unreal Editor nor actual gameplay
has been run or verified.**

The Crossings (0,0 cm) is the original crossroads of competing realities.
Bellwold Refuge (-18000,0 cm) is a shelter community organized around
bells, shared aid, and a refuge. Paperhaven Archive (+18000,0 cm)
collects conflicting histories and testimony. Each village contains
sixteen unique named residents with independent persistent memory and
author-written responses. They are separate places with separate
primitive-block village grounds connected by causeways.

## Interactive community arcs

- Crossings keeps the shelter/research decision, supply errand and
  fracture/lexicon gameplay.
- Bellwold: speak to Rook the lamplighter, then Hessa the matron to
  deliver lanterns.
- Paperhaven: speak to Toma the copyist, then Sevrin the registrar
  to preserve missing testimony.

Progress is saved in the existing schema-v1 save beside prior character
memories, exploration, Fracture, and conflict. Duplicate or out-of-order
conversation cannot skip steps; missing writes restore the former state.
Completed tasks emit local witnessed help; hearsay still requires a
physically close speaker. Learned shelter/research information is
interpreted according to village preferences.

## Verification still needed in Unreal

1. Compile the module and run Unreal automation tests.
2. Walk the entire 180 m road west and east, checking collision.
3. Visit 16 resident actors in each place, confirming unique identity
   and distinct memory after save/reload.
4. Complete both village conversations with save/reload between steps.
5. Confirm remote towns do not magically know a Crossings choice.
6. Advance dawn/day/dusk/night: routines stay in home villages.
7. Profile 48 resident actor updates, NPC gossip, and save writes.
8. Implement authored maps, proper navigation, health and journal UI,
   sound, animations and player accessibility before calling this
   a production-ready multi-village game.

GitHub CI validates only pure C++ rules and source contracts.
No cloud Unreal host or paid AI has been provisioned.
