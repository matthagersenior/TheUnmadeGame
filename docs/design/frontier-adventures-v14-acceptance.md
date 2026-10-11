# Frontier combat and exploration acceptance

Source implemented across all nine canonical realms. Unreal compilation and engine acceptance remain deferred by the creator. The existing campaign travel/attunement, protagonist, ten signature rites, eleven original cities and OPEN mysteries remain intact.

## Scope and physical walkthrough

Nine arcs connect the existing realm city, a new frontier settlement, a new inhabited refuge and a six-chamber dungeon. Total authored settlements: 29 (11 original plus 18 new). The 45 new named residents include 18 actual captives; all original 88 residents/scouts retain their identities. Dungeon encounters total 72 non-boss enemies and nine bosses. Each arc contains three physical evidence records and two captive-release points.

Positions are relative to the realm city center in `Authoring/frontier_adventures_v14.json`. E at city `(700,-700,95)` accepts the adventure. Walk north to frontier `(0,8000)` and E at `(0,8500)` to register the visit; the refuge at `(-3500,11500)` is optional. No new teleport or campaign bypass is granted.

Dungeon room centers relative to city: `(0,15000)`, `(2600,15000)`, `(2600,17600)`, `(0,17600)`, `(0,20200)`, `(2600,20200)`. E at each center plus `(0,-650)` registers entry in sequence. Actual perimeter walls form a switchback; pillars break visibility. The boss arena's west gate remains closed before objective choice. A separate northern gate opens after boss resolution and leads onto the outside return walkway.

Read the three E/F records in chambers 2, 3 and 4 at `(-650,450)`. The chamber 4 danger field is centered at `(650,500)`, radius 280 cm. Two seconds of physical warning precede a one-second danger pulse every nine seconds; a source/pulse receipt prevents repeated damage. Edges are safe; E at `(-650,-650)` vents the field for eight seconds without Strain. The boss resolution permanently stops this field.

Chamber 5 holds four enemies and two real captives. Release target offsets `(-650,-550)` and `(-650,550)`. The respective guard slots 4 and 5 must actually be defeated, or the boss must have resolved and its guards stood down. Captives wait in the chamber; they do not walk an escort path. Once actually released and the frontier return is witnessed, those actors relocate to their new homes. This is a saved transition, not an authored escort simulation. Unreleased people remain in the dungeon even if the boss yields.

At chamber 5 `(650,0)`, E commits the rescue objective after both releases; F commits the evidence objective after all three records. The branches are mutually exclusive and permanent; objectives may both be completed. Rescue requires a boss fight. Evidence permits either combat or a living surrender. E at arena `(-650,650)` presents evidence to the actual living nearby boss; a dead actor cannot surrender. F there explains counterplay. The boss alternates crush, wide sweep and aimed seal; its half-health sweep has a shorter warning. Aim locks at warning start. Real line of sight is required throughout; cover, movement, guarding and earned Fold supply counters. E at `(650,-650)` interrupts the boss for three seconds, with eight seconds between uses, and costs no Strain.

Walk back to frontier equipment bench `(650,350)` and E to witness the return while the named local leader is nearby. E again chooses the named weapon; F chooses armor. The choice is explicitly irreversible, and either includes the realm technique. Revisit with E to activate that owned realm kit. One realm kit/technique is active at once; older selections stay owned. These supplementary bonuses compose with existing inventory and civic bonuses; no existing item IDs or saved stack lengths change. No money, cargo, wage or fuel is awarded or consumed.

Technique benefits by realm index (extra attack, extra armor, Glimpse discount, Fold seconds): `(3,0,3,0)`, `(0,2,2,0)`, `(2,2,0,1)`, `(1,1,1,1)`, `(0,1,0,2)`, `(2,0,2,0)`, `(0,2,3,0)`, `(1,3,0,1)`, `(0,3,1,2)`. Weapons give `7+2*realmIndex` attack; armor gives `5+realmIndex` defense. Existing ability costs and unlocks remain authoritative. These are passive techniques, not nine newly bound spells.

## Persistence and consequences

The optional additive save stores 93 integers: version, nine ten-field arcs, active gear and technique. Legacy saves initialize empty. Validation rejects invalid masks, disconnected room progress, unreal kills, impossible rescues, branch contradictions, unearned rewards or mismatched active ownership atomically. Failed world writes restore the entire previous model. Defeated actors retry their durable receipts. Reload restores defeated guards, dead versus yielded bosses, gates, released residents and selected bonuses; partial damage and temporary hazard vents/attack telegraphs are session-local.

Boss resolution removes hostile dungeon patrols, stops the danger field and opens the visible return gate. Surrender keeps the boss alive and disabled; defeat hides it. Frontier/refuge residents distinguish those outcomes when the return is witnessed. This changes the local expedition, not unrelated city government, all realm patrols or the canonical campaign ending.

## Later engine gate

Compile UHT/Editor, then build navmesh over approach, settlements, all chambers and outside return walkway. Verify actual geometry, slopes, door sizes, capsule clearance, ranged sight, locked warning marks, guarding/armor, animations and camera. New architecture remains graybox and uses existing optional resident/beast/wraith rigs. No final art, species-specific animations or VO is claimed.

Run all nine arcs with both objective branches and both equipment rewards; evidence has combat and surrender outcomes. Test sneaking around enemies for evidence, early arena attempts, death/revive, post-boss physical releases, reward reload and re-equipping previous kits. Test malformed/truncated saves, write failure after kills/rescues/choices/rewards, absent leader during return, duplicate rewards, cover breaks during warnings and partial damage across attack-pattern transitions. A dynamic damage change must not heal an enemy. Measure actor/nav/rendering performance before final asset density increases. Deliveries remain a supporting independent quest type.
