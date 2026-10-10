# THE UNMADE — Six later-world hazard and counterplay acceptance
**2026-10-10 / Pre-PC source milestone. Do not call this a UE-verified encounter.**

Each danger must be readable in advance, and a safe route must exist. The source sets danger only inside a bounded approach lane after the local first-arc witness begins the expedition, and ends it permanently when the first realm-specific control is operated. The arrival platform, both original evidence flanks and the far civic terrace remain safe.

| Realm | Pulse character | Effect | Learnable counterplay |
|---|---|---|---|
| Drevlach, Sea of Written Debts | Debt tide, 14 s cycle | 14 base health damage | Slack tide window, outside lanes, guard |
| Orravane, Upside-Down Choir | Falling note, 11 s cycle | +18 nonlethal reality strain | Silent gap, edge of chord |
| Vathless, Bones of Yesterdays | Quarry split, 16 s cycle | 24 base health damage | Side survey route, guard |
| Eillun, Hundred Unlived | Intrusive census sweep, 13 s cycle | +22 nonlethal reality strain | Quiet time, sheltered anonymity path |
| Tharniv, Orchard of Kings | Law-root strike, 15 s cycle | 18 base health damage | Unbound side row, guard |
| Auvren, Place Before Place | Horizon unmooring, 17 s cycle | +26 nonlethal reality strain | Stable memory path, quiet time |

**Required on first PC:**
1. Run `Scripts/first_pc_build_and_test.ps1`. Automation includes `Unmade.World.SixLaterRealmHazards`. Capture Unreal compiler logs before gameplay claims.
2. First arrival and untouched story: visually no harmful pulse. Speak to the initiating witness; on entering the danger corridor, see a noncolliding floor warning before impact, then observe impact only once per unique world pulse.
3. Walk either evidence-flank route; no damage or strain outside the bounded zone. Test the southern arrival, the gap, keeper position, and far civic terrace remain safe, including during long sessions.
4. For health realms, guard before impact and verify mitigation, correctly isolated source IDs and no repeated damage from one pulse. For reality realms, verify strain, cap and gradual recovery; never instant lethal damage. Force failed save writes to confirm rollback and error text.
5. NPCs in an observed local warning should move toward nearer shelter and provide an accurate local tip. NPCs in other regions must not know about this danger. Verify pathfinding and animation do not cause an NPC to fall into the gap.
6. Operate each realm's first physical mechanism; danger must stop immediately and remain stopped across reload. Other realms continue unchanged. Repeated E presses must not re-enable hazards.
7. Test low FPS and long frame pauses: warning readability should not depend solely on debug text, and an impact must not apply multiple times. Provide audio, VFX, motion comfort, controller feedback, high contrast and accessibility.
8. Repeat each route from a clean state on Windows with native capsule collision. Do not accept static green GitHub CI as runtime verification.

**Still missing:** authored threat animation/audio/particle feedback, Unreal Editor compilation, playable platform collision verification, dynamic AI navigation meshes and real-time balance testing.
