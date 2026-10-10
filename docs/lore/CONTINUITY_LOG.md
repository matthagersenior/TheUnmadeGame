# THE UNMADE — Illustrated Lore Continuity Log

Every substantial gameplay addition needs a companion canon update and a regenerated illustrated package edition. This history describes what was actually updated; it is not a claim that engine content was playtested.

## 2026-10-10 — Volume II: The World Remembers
- Codified the four player-first standards and ten signature mechanics as required narrative and interaction checks.
- Added fully authored follow-up adventures for the **nine** established realms. The rules are implemented in a deterministic C++17 source model with identity-specific evidence, witness and choice gates. Six realms are still conceptual for Unreal runtime.
- Expanded the aftermath of The Threefold Reach to include Bellwold's **The Second Night** and The Crossings' **Road After the Rescue**.
- Updated the canonical item count from 58 to **60** after adding Ward of the Second Night and Lantern of Unredacted Names.
- Retained 64 currently authored NPC identities (48 core and 16 frontier); later realms have proposed, unspawned witness IDs.
- Updated the illustrated package master, art atlas, source documentation, quality matrix and ZIP. The imagery remains speculative artwork rather than actual Unreal gameplay screenshots.

- Added a CI-level source-change gate: gameplay commits must carry updated canonical lore and the continuity log together, in addition to the dated downloadable illustrated edition.

## 2026-10-10 — Volume III: Saltwake / Cinderhold physical return sequence (source pass)
- Added deterministic, non-farmable physical mechanism requirements to all nine realm aftermath rules; Saltwake's storm sluice and Cinderhold's heat vent are the next actor-connected milestones.
- Preserved the two ethically distinct evidence/witness paths and permanent outcomes; later six mechanism IDs remain authored contracts without spawned worlds.
- Updated the living canon in this same source commit. The full illustrated DOCX/PDF/art atlas/manifest/ZIP package is **not yet regenerated** and must not be represented as shipped.
- Reserved a separate save schema-compatible nine-realm aftermath snapshot and a dedicated Hub/Player API, keeping earlier frontier and Bellwold states intact. Pending Unreal-host runtime acceptance.\n- Actor integration now includes physical sight/proximity inspection, storm/heat barriers, two exclusive follow-on routes each, outcome refresh on load, and atomic save rollback. Source contract tests cover the wiring; Unreal Editor behavior remains unverified.\n- Player E interaction now prioritizes the nearer physical control/clue over a farther NPC, journal tracks both returning realms, and F7/F8 resolves matching testimony near a warden.
- Saltwake and Cinderhold residents use public home-town world states plus separately witnessed/rumored choice events; the deterministic offline speech remains sufficient without AI.
- Added a world-actor QA checklist and realm-by-realm physical adventure acceptance standard; all six later realms remain future production until engine-tested.\n
## 2026-10-10 — PC-readiness source pass (hazard and fail-safe recovery)
- Added a world-clock-anchored, location-bounded rain/heat pulse domain; hazard damage and warning periods are deterministic, avoid safe settlement routes and end when the corresponding mechanism is operated. Windows Unreal runtime still unverified.
- Added checkpoint revival to prevent permanent loss of movement on player defeat. Story/world saves are deliberately not reset on recovery; game feel and performance remain engine/creator-gated.
- Wired visible non-colliding warning strips into Saltwake and Cinderhold, world-clock hazard sampling into the player, guarding counterplay, deduplicated environmental hits and safe checkpoint respawn after four seconds. Gameplay verification awaits Unreal Editor.\n- Corrected initial warning-strip visibility: the noncolliding cues are hidden on spawn and revealed only during timed warning/impact windows.
- Authored a one-command Windows preflight + Editor build + automation suite runner with retained logs; this script has not yet been exercised on a licensed Unreal GPU host.\n