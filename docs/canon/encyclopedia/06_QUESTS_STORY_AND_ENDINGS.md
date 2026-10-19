# Volume VI — Main Campaign, Every Quest Group, Journeys, Quest Outcomes and the Two Mornings

**Canon status:** Source-level quest/dependency documentation, not an Unreal-playtested complete campaign. Genuine progress comes from real first-chapter and final-fight saved states; journal, Atlas descriptions and optional lore never fabricate progress.

## The Unanswered Road — five investigative stages and the confrontation

**Mandatory:** genuine opening testimony or earned first region resolution → ONE frontier first chapter (rain or hearth) → ONE middle first chapter (ledger, choir or cinder) → ONE upper first chapter (unlived or orchard) → Auvren's physical first trial → Nhal-Vey. Follow actual Atlas passages at attunement earned from coherent first stories or optional relics; optional relics never spoof first-story evidence. **Never require** Echo completion, guardian deaths, observatory mastery, every realm, or all landmarks.

### Act 0 — `act.0`

**eligible:** ThreefoldReach  
**requires:** A true opening local choice OR two visited settlements OR two discovered lore sites  
**journal:** A bell without a last note  
**optional:** false  

### Act 1 — `act.1`

**eligible:** WidowedRain; HearthBeneath  
**requires:** Opening witnessed; resolve one frontier first story  
**journal:** Where did the missing thing go?  
**optional:** false  

### Act 2 — `act.2`

**eligible:** TidalLedger; SkyBelow; CinderSpine  
**requires:** One frontier first story resolved; resolve one middle first story  
**journal:** Who bears the hidden cost?  
**optional:** false  

### Act 3 — `act.3`

**eligible:** HundredUnlived; OrchardOfKings  
**requires:** One middle first story resolved; resolve one upper first story  
**journal:** Who may be a true witness?  
**optional:** false  

### Act 4 — `act.4`

**eligible:** FirstAbsence  
**requires:** One upper first story resolved; resolve Auvren first trial  
**journal:** The witness before place  
**optional:** false  

### Act 5 — `act.5`

**eligible:** FirstAbsence  
**requires:** Coherent opening + frontier + middle + upper + Auvren completed, or progressed legacy final state  
**journal:** Face Nhal-Vey; survive false victory and choose the next morning  
**optional:** false  

### Act 6 — `act.6`

**eligible:** ThreefoldReach; WidowedRain; HearthBeneath; TidalLedger; SkyBelow; CinderSpine; HundredUnlived; OrchardOfKings; FirstAbsence  
**requires:** True final choice saved  
**journal:** Explore the same nine realms in the chosen morning; rematch optional  
**optional:** true  


## Physical main-quest scene contracts, recoveries and evidence provenance

### QUEST_MAIN_ThreefoldReach — `ThreefoldReach`

**realm:** ThreefoldReach  
**director note:** Bell silence is later heard in rain, fire and choir; never say Nhal-Vey here.  
**environment:** Roads stitched through contradictory maps, refugee lanterns and a split-record archive.  
**player verbs:** Ring the silent bell and mark two observable landmarks; Carry supplies through the contested neighborhood and speak to local witnesses; Choose shelter or research testimony with a clear permanent preview  
**npc ids:** npc.archivist.001; npc.bellwold.matron.001; npc.paperhaven.registrar.001  
**real source tags:** {"evidence":["Reach.Bell.Remembered","Reach.Paperhaven.AbsenceRecord"],"mechanism":"Reach.CrossingsWitness","contract_status":"frontier/Reach art-direction tag placeholders; verify against actual UE actor before binding"}  
**chapter level:** 0  
#### arrival

**trigger:** player within safe southern arrival zone after earned AtlasPassages crossing  
**interaction:** Inspect the real realm public Campaign.Thread marker or speak to a resident  
**objective:** Ring the silent bell and mark two observable landmarks  
**speaker:** npc.archivist.001  
**line:** I heard a bell from a road that doesn't exist. Who would carve a road into silence?  
**failure:** No valid sightline or current quest not started  
**recovery:** Return to safe arrival path; map marker remains readable without taking rewards  
**saved as:** world visit/initial dialogue; no new required quest flag  

#### investigation

**trigger:** local first quest begun by actual relevant NPC  
**interaction:** Inspect either physical evidence object, never resolve by dialogue text alone  
**objective:** Carry supplies through the contested neighborhood and speak to local witnesses  
**speaker:** npc.bellwold.matron.001  
**line:** Get these supplies to the refuge. A roof should not depend on what anyone can prove about that bell.  
**evidence tags:** Reach.Bell.Remembered; Reach.Paperhaven.AbsenceRecord  
**failure:** Wrong site, unreadable text or hazard damage before evidence was collected  
**recovery:** Evidence remains independently inspectable; stable safe route, no story skip or permanent lockout  
**saved as:** Existing frontier/Reach quest evidence; original authoritative state  

#### mechanism

**trigger:** matching locally saved evidence acquired; mechanism is interactable  
**interaction:** Use actual site actor; check line of sight, stage and evidence match  
**objective:** Choose shelter or research testimony with a clear permanent preview  
**speaker:** npc.archivist.001  
**line:** The document says no bell was made. I heard it. What could make both accounts true?  
**design notes:** The unanswered interval is the bell without a last note. Set the document's denial beside the archivist's hearing and leave the cause unresolved.  
**physical tag:** Reach.CrossingsWitness  
**failure:** Mechanism blocked, invalid/wrong evidence or missing power prerequisite  
**recovery:** Return to the matching clue and retry physical mechanism; never consume evidence on failed persistence  
**saved as:** Original first-quest mechanism action; its current stage/choice  

#### care_commit

**trigger:** mechanism and care evidence valid; chosen care route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Preserve public shelter access and lantern use.  
**speaker:** npc.bellwold.matron.001  
**line:** If we keep a roof open tonight, someone may live long enough to tell us what happened.  
**outcome:** care  
**failure:** SaveGameToSlot failure, mismatched scope, missed confirmation window  
**recovery:** Keep previous world and pending quest state; repeat deliberate confirmation after save works  
**saved as:** Original first-story outcome 1; physical care passage and local visible public effect  

#### truth_commit

**trigger:** mechanism and truth evidence valid; chosen truth route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Make the missing record inspectable without forcefully identifying every protected resident.  
**speaker:** npc.paperhaven.registrar.001  
**line:** The archive can be public without naming people who asked for refuge.  
**outcome:** truth  
**failure:** SaveGameToSlot failure, consent condition unfulfilled, mismatched confirmation  
**recovery:** Restore previous route; allow player to change evidence route before irreversible save  
**saved as:** Original first-story outcome 2; public evidence record and local visible physical effect  

#### return

**trigger:** reenter region after either outcome, optionally after Guardian/Echo/resonance  
**interaction:** talk to individual NPC by stable ID; inspect saved physical consequence  
**objective:** Inspect the saved physical consequence and speak with an individual resident.  
**speaker:** npc.archivist.001  
**line:** You came back. The bell road carries the mark your choices left. I'll record what it shows and leave the rest to its witnesses.  
**design notes:** Return staging note: Let the visible change carry the cumulative choices saved for this route. The archivist records what the change shows; residents speak only from what they witnessed or learned.  
**failure:** Area unloaded or a followup system still dormant  
**recovery:** Core first story remains complete; later optional chapters appear only when their own prerequisites hold  
**saved as:** One-time local first story outcome plus optional separate relationship visits  

#### Design notes

**npc.archivist.001 — voice and register**
Sentence length: measured, mostly short; use questions when evidence remains incomplete.  
Metaphors: roads, margins, records, and the space between a claim and its witness.  
Taboos: never state rumor as testimony or claim knowledge of a resident's private choice.  
Would never say: “The record proves everything.”

**npc.bellwold.matron.001 — voice and register**
Sentence length: short and practical; lengthen only when weighing what shelter can protect.  
Metaphors: doors, roofs, lamps, weather, and the work of keeping a place open.  
Taboos: never treat refuge as a debt or identify someone who sought shelter without consent.  
Would never say: “A roof matters more than the people under it.”

**npc.paperhaven.registrar.001 — voice and register**
Sentence length: precise and balanced; clauses distinguish what a record says from what it cannot prove.  
Metaphors: pages, margins, seals, ledgers, and blank spaces.  
Taboos: never turn a protected name into public evidence or let paperwork impersonate testimony.  
Would never say: “If it is written, it must be true.”

### QUEST_MAIN_WidowedRain — `WidowedRain`

**realm:** WidowedRain  
**director note:** A missing tide is dated like a loan due before birth.  
**environment:** Dry docks under persistent rain, wet ledgers, hollow ship-roof shelters.  
**player verbs:** Collect a shoreline impression from a physical pier and a contradictory wet map; Avoid a telegraphed tidal storm pulse and use an elevated safe route; Commit to public rain access or a protected account of loss  
**npc ids:** npc.saltwake.navigator.001; npc.saltwake.rainkeeper.001; npc.saltwake.harborwarden.001  
**real source tags:** {"evidence":["Saltwake.Clue.WetLedger","Saltwake.Clue.TideChart"],"mechanism":"Saltwake.StormValve","contract_status":"frontier/Reach art-direction tag placeholders; verify against actual UE actor before binding"}  
**chapter level:** 1  
#### arrival

**trigger:** player within safe southern arrival zone after earned AtlasPassages crossing  
**interaction:** Inspect the real realm public Campaign.Thread marker or speak to a resident  
**objective:** Collect a shoreline impression from a physical pier and a contradictory wet map  
**speaker:** npc.saltwake.navigator.001  
**line:** I charted a sea yesterday. Tell me why its shore is still wet but its water isn't here.  
**failure:** No valid sightline or current quest not started  
**recovery:** Return to safe arrival path; map marker remains readable without taking rewards  
**saved as:** world visit/initial dialogue; no new required quest flag  

#### investigation

**trigger:** local first quest begun by actual relevant NPC  
**interaction:** Inspect either physical evidence object, never resolve by dialogue text alone  
**objective:** Avoid a telegraphed tidal storm pulse and use an elevated safe route  
**speaker:** npc.saltwake.rainkeeper.001  
**line:** I count every drop, because someone is selling us the same storm twice.  
**evidence tags:** Saltwake.Clue.WetLedger; Saltwake.Clue.TideChart  
**failure:** Wrong site, unreadable text or hazard damage before evidence was collected  
**recovery:** Evidence remains independently inspectable; stable safe route, no story skip or permanent lockout  
**saved as:** Existing frontier/Reach quest evidence; original authoritative state  

#### mechanism

**trigger:** matching locally saved evidence acquired; mechanism is interactable  
**interaction:** Use actual site actor; check line of sight, stage and evidence match  
**objective:** Commit to public rain access or a protected account of loss  
**speaker:** npc.saltwake.navigator.001  
**line:** I charted a sea yesterday. Tell me why its shore is still wet but its water isn't here. The missing water as an economic and environmental absence gives you one chance to change the place responsibly.  
**physical tag:** Saltwake.StormValve  
**failure:** Mechanism blocked, invalid/wrong evidence or missing power prerequisite  
**recovery:** Return to the matching clue and retry physical mechanism; never consume evidence on failed persistence  
**saved as:** Original first-quest mechanism action; its current stage/choice  

#### care_commit

**trigger:** mechanism and care evidence valid; chosen care route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Shelter ferry users without requiring old ownership papers.  
**speaker:** npc.saltwake.rainkeeper.001  
**line:** Share our rain before another family sleeps in an empty boat.  
**outcome:** care  
**failure:** SaveGameToSlot failure, mismatched scope, missed confirmation window  
**recovery:** Keep previous world and pending quest state; repeat deliberate confirmation after save works  
**saved as:** Original first-story outcome 1; physical care passage and local visible public effect  

#### truth_commit

**trigger:** mechanism and truth evidence valid; chosen truth route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Preserve the ledger and collateral history without declaring private survivors debtors.  
**speaker:** npc.saltwake.harborwarden.001  
**line:** Show us the missing water's accounts, and let us choose whom to protect.  
**outcome:** truth  
**failure:** SaveGameToSlot failure, consent condition unfulfilled, mismatched confirmation  
**recovery:** Restore previous route; allow player to change evidence route before irreversible save  
**saved as:** Original first-story outcome 2; public evidence record and local visible physical effect  

#### return

**trigger:** reenter region after either outcome, optionally after Guardian/Echo/resonance  
**interaction:** talk to individual NPC by stable ID; inspect saved physical consequence  
**objective:** Storm records update to acknowledge actual Rainward public choice; no generic all-knowing NPC reaction.  
**speaker:** npc.saltwake.navigator.001  
**line:** You came back. Storm records update to acknowledge actual Rainward public choice; no generic all-knowing NPC reaction. I have not forgotten the path you chose.  
**failure:** Area unloaded or a followup system still dormant  
**recovery:** Core first story remains complete; later optional chapters appear only when their own prerequisites hold  
**saved as:** One-time local first story outcome plus optional separate relationship visits  

### QUEST_MAIN_HearthBeneath — `HearthBeneath`

**realm:** HearthBeneath  
**director note:** Flames lack exactly the same note missing from the Reach's bell.  
**environment:** Inward mountain city with public kilns, soot families and thermal canal controls.  
**player verbs:** Trace a heat canal from the common kiln to a cold household; Manually divert a heat pulse through an insulated channel; Balance immediate warmth for children against truth about inherited fuel costs  
**npc ids:** npc.cinderhold.hearthreader.001; npc.cinderhold.emberwarden.001; npc.cinderhold.healer.001  
**real source tags:** {"evidence":["Cinderhold.Clue.ColdHearth","Cinderhold.Clue.EmberDeed"],"mechanism":"Cinderhold.KilnVent","contract_status":"frontier/Reach art-direction tag placeholders; verify against actual UE actor before binding"}  
**chapter level:** 1  
#### arrival

**trigger:** player within safe southern arrival zone after earned AtlasPassages crossing  
**interaction:** Inspect the real realm public Campaign.Thread marker or speak to a resident  
**objective:** Trace a heat canal from the common kiln to a cold household  
**speaker:** npc.cinderhold.hearthreader.001  
**line:** The kiln casts heat from a sun that has never risen. A family is cold because of that contradiction.  
**failure:** No valid sightline or current quest not started  
**recovery:** Return to safe arrival path; map marker remains readable without taking rewards  
**saved as:** world visit/initial dialogue; no new required quest flag  

#### investigation

**trigger:** local first quest begun by actual relevant NPC  
**interaction:** Inspect either physical evidence object, never resolve by dialogue text alone  
**objective:** Manually divert a heat pulse through an insulated channel  
**speaker:** npc.cinderhold.emberwarden.001  
**line:** A public flame is only public if the farthest house may share it.  
**evidence tags:** Cinderhold.Clue.ColdHearth; Cinderhold.Clue.EmberDeed  
**failure:** Wrong site, unreadable text or hazard damage before evidence was collected  
**recovery:** Evidence remains independently inspectable; stable safe route, no story skip or permanent lockout  
**saved as:** Existing frontier/Reach quest evidence; original authoritative state  

#### mechanism

**trigger:** matching locally saved evidence acquired; mechanism is interactable  
**interaction:** Use actual site actor; check line of sight, stage and evidence match  
**objective:** Balance immediate warmth for children against truth about inherited fuel costs  
**speaker:** npc.cinderhold.hearthreader.001  
**line:** The kiln casts heat from a sun that has never risen. A family is cold because of that contradiction. The an impossible sun's heat and shared physical obligations gives you one chance to change the place responsibly.  
**physical tag:** Cinderhold.KilnVent  
**failure:** Mechanism blocked, invalid/wrong evidence or missing power prerequisite  
**recovery:** Return to the matching clue and retry physical mechanism; never consume evidence on failed persistence  
**saved as:** Original first-quest mechanism action; its current stage/choice  

#### care_commit

**trigger:** mechanism and care evidence valid; chosen care route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Keep unregistered homes warm without demanding signatures.  
**speaker:** npc.cinderhold.emberwarden.001  
**line:** Give the households heat now; my neighbors cannot eat an argument.  
**outcome:** care  
**failure:** SaveGameToSlot failure, mismatched scope, missed confirmation window  
**recovery:** Keep previous world and pending quest state; repeat deliberate confirmation after save works  
**saved as:** Original first-story outcome 1; physical care passage and local visible public effect  

#### truth_commit

**trigger:** mechanism and truth evidence valid; chosen truth route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Expose the true provenance of fuel and require a public repair plan.  
**speaker:** npc.cinderhold.healer.001  
**line:** Name the price of the ember. We can repair a lie only after we see it.  
**outcome:** truth  
**failure:** SaveGameToSlot failure, consent condition unfulfilled, mismatched confirmation  
**recovery:** Restore previous route; allow player to change evidence route before irreversible save  
**saved as:** Original first-story outcome 2; public evidence record and local visible physical effect  

#### return

**trigger:** reenter region after either outcome, optionally after Guardian/Echo/resonance  
**interaction:** talk to individual NPC by stable ID; inspect saved physical consequence  
**objective:** Citizens relight hearths based on the route player actually maintained.  
**speaker:** npc.cinderhold.hearthreader.001  
**line:** You came back. Citizens relight hearths based on the route player actually maintained. I have not forgotten the path you chose.  
**failure:** Area unloaded or a followup system still dormant  
**recovery:** Core first story remains complete; later optional chapters appear only when their own prerequisites hold  
**saved as:** One-time local first story outcome plus optional separate relationship visits  

### QUEST_MAIN_TidalLedger — `TidalLedger`

**realm:** TidalLedger  
**director note:** The ledger implies the world has been taking loans against nonexistent histories.  
**environment:** Tide clocks in stranded boats, ferry passages and flooded civic ledgers.  
**player verbs:** Examine ferry receipt and collateral tide impressions on opposite routes; Work a debt latch to open a permanent physical care or truth bridge; Return for an Echo investigation and optional two-rite observatory  
**npc ids:** npc.tidal.ledgerkeeper.001; npc.tidal.ferryman.001; npc.tidal.witness.001  
**real source tags:** {"evidence":["Tidal.Trial.FerryReceipt","Tidal.Trial.CollateralTide"],"mechanism":"Tidal.Trial.DebtLatch","contract_status":"authoritative tags checked against LaterRealms source"}  
**chapter level:** 2  
#### arrival

**trigger:** player within safe southern arrival zone after earned AtlasPassages crossing  
**interaction:** Inspect the real realm public Campaign.Thread marker or speak to a resident  
**objective:** Examine ferry receipt and collateral tide impressions on opposite routes  
**speaker:** npc.tidal.ledgerkeeper.001  
**line:** The ninth debt is dated before its debtor's birth. The ink isn't lying; the law is.  
**failure:** No valid sightline or current quest not started  
**recovery:** Return to safe arrival path; map marker remains readable without taking rewards  
**saved as:** world visit/initial dialogue; no new required quest flag  

#### investigation

**trigger:** local first quest begun by actual relevant NPC  
**interaction:** Inspect either physical evidence object, never resolve by dialogue text alone  
**objective:** Work a debt latch to open a permanent physical care or truth bridge  
**speaker:** npc.tidal.ferryman.001  
**line:** The ferried child never agreed to any creditor; protect the boat from the books.  
**evidence tags:** Tidal.Trial.FerryReceipt; Tidal.Trial.CollateralTide  
**failure:** Wrong site, unreadable text or hazard damage before evidence was collected  
**recovery:** Evidence remains independently inspectable; stable safe route, no story skip or permanent lockout  
**saved as:** LaterRealm.stage=2 and pending care/truth evidence  

#### mechanism

**trigger:** matching locally saved evidence acquired; mechanism is interactable  
**interaction:** Use actual site actor; check line of sight, stage and evidence match  
**objective:** Return for an Echo investigation and optional two-rite observatory  
**speaker:** npc.tidal.ledgerkeeper.001  
**line:** The ninth debt is dated before its debtor's birth. The ink isn't lying; the law is. The contracts dated before their signers' birth gives you one chance to change the place responsibly.  
**physical tag:** Tidal.Trial.DebtLatch  
**failure:** Mechanism blocked, invalid/wrong evidence or missing power prerequisite  
**recovery:** Return to the matching clue and retry physical mechanism; never consume evidence on failed persistence  
**saved as:** Original first-quest mechanism action; its current stage/choice  

#### care_commit

**trigger:** mechanism and care evidence valid; chosen care route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Make a child-safe ferry that never charges future passengers.  
**speaker:** npc.tidal.ferryman.001  
**line:** Let tomorrow's children cross without taking on yesterday's payments.  
**outcome:** care  
**failure:** SaveGameToSlot failure, mismatched scope, missed confirmation window  
**recovery:** Keep previous world and pending quest state; repeat deliberate confirmation after save works  
**saved as:** Original first-story outcome 1; physical care passage and local visible public effect  

#### truth_commit

**trigger:** mechanism and truth evidence valid; chosen truth route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Publish verifiable fraud without forcibly identifying protected borrowers.  
**speaker:** npc.tidal.witness.001  
**line:** Publish the fraudulent contracts, not the private addresses of the borrowers.  
**outcome:** truth  
**failure:** SaveGameToSlot failure, consent condition unfulfilled, mismatched confirmation  
**recovery:** Restore previous route; allow player to change evidence route before irreversible save  
**saved as:** Original first-story outcome 2; public evidence record and local visible physical effect  

#### return

**trigger:** reenter region after either outcome, optionally after Guardian/Echo/resonance  
**interaction:** talk to individual NPC by stable ID; inspect saved physical consequence  
**objective:** Local residents remember ferry or archive outcomes, guardian mercy or defeat, and observatory archive.  
**speaker:** npc.tidal.ledgerkeeper.001  
**line:** You came back. Local residents remember ferry or archive outcomes, guardian mercy or defeat, and observatory archive. I have not forgotten the path you chose.  
**failure:** Area unloaded or a followup system still dormant  
**recovery:** Core first story remains complete; later optional chapters appear only when their own prerequisites hold  
**saved as:** One-time local first story outcome plus optional separate relationship visits  

### QUEST_MAIN_SkyBelow — `SkyBelow`

**realm:** SkyBelow  
**director note:** The exact missing musical rest is identical to the Unanswered Interval.  
**environment:** Hanging neighborhoods and two load-bearing musical support scores.  
**player verbs:** Observe the actual support-counterweight under a precarious building; Tune the fork to a recorded note while keeping an alternate stable load; Choose shared meter or public excluded chorus, and see the hanging path respond  
**npc ids:** npc.sky.choirmaster.001; npc.sky.structuralist.001; npc.sky.dissenter.001  
**real source tags:** {"evidence":["Sky.Trial.Counterweight","Sky.Trial.ForgottenChorus"],"mechanism":"Sky.Trial.TuningFork","contract_status":"authoritative tags checked against LaterRealms source"}  
**chapter level:** 2  
#### arrival

**trigger:** player within safe southern arrival zone after earned AtlasPassages crossing  
**interaction:** Inspect the real realm public Campaign.Thread marker or speak to a resident  
**objective:** Observe the actual support-counterweight under a precarious building  
**speaker:** npc.sky.choirmaster.001  
**line:** Count the missing singers: the houses will fall if you remove their support blindly.  
**failure:** No valid sightline or current quest not started  
**recovery:** Return to safe arrival path; map marker remains readable without taking rewards  
**saved as:** world visit/initial dialogue; no new required quest flag  

#### investigation

**trigger:** local first quest begun by actual relevant NPC  
**interaction:** Inspect either physical evidence object, never resolve by dialogue text alone  
**objective:** Tune the fork to a recorded note while keeping an alternate stable load  
**speaker:** npc.sky.structuralist.001  
**line:** A counterweight can take the load, but the chorus has to choose its part.  
**evidence tags:** Sky.Trial.Counterweight; Sky.Trial.ForgottenChorus  
**failure:** Wrong site, unreadable text or hazard damage before evidence was collected  
**recovery:** Evidence remains independently inspectable; stable safe route, no story skip or permanent lockout  
**saved as:** LaterRealm.stage=2 and pending care/truth evidence  

#### mechanism

**trigger:** matching locally saved evidence acquired; mechanism is interactable  
**interaction:** Use actual site actor; check line of sight, stage and evidence match  
**objective:** Choose shared meter or public excluded chorus, and see the hanging path respond  
**speaker:** npc.sky.choirmaster.001  
**line:** Count the missing singers: the houses will fall if you remove their support blindly. The songs physically holding inverted architecture aloft gives you one chance to change the place responsibly.  
**physical tag:** Sky.Trial.TuningFork  
**failure:** Mechanism blocked, invalid/wrong evidence or missing power prerequisite  
**recovery:** Return to the matching clue and retry physical mechanism; never consume evidence on failed persistence  
**saved as:** Original first-quest mechanism action; its current stage/choice  

#### care_commit

**trigger:** mechanism and care evidence valid; chosen care route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Stabilize homes through a voluntary shared support rhythm.  
**speaker:** npc.sky.structuralist.001  
**line:** Keep the houses aloft and teach a voluntary new refrain.  
**outcome:** care  
**failure:** SaveGameToSlot failure, mismatched scope, missed confirmation window  
**recovery:** Keep previous world and pending quest state; repeat deliberate confirmation after save works  
**saved as:** Original first-story outcome 1; physical care passage and local visible public effect  

#### truth_commit

**trigger:** mechanism and truth evidence valid; chosen truth route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Recover forbidden singers' names without forcing their return to compulsory work.  
**speaker:** npc.sky.dissenter.001  
**line:** Print the forbidden verse and all its names. Let those named decide whether to sing.  
**outcome:** truth  
**failure:** SaveGameToSlot failure, consent condition unfulfilled, mismatched confirmation  
**recovery:** Restore previous route; allow player to change evidence route before irreversible save  
**saved as:** Original first-story outcome 2; public evidence record and local visible physical effect  

#### return

**trigger:** reenter region after either outcome, optionally after Guardian/Echo/resonance  
**interaction:** talk to individual NPC by stable ID; inspect saved physical consequence  
**objective:** A household's safety and a dissenting score remain independently inspectable.  
**speaker:** npc.sky.choirmaster.001  
**line:** You came back. A household's safety and a dissenting score remain independently inspectable. I have not forgotten the path you chose.  
**failure:** Area unloaded or a followup system still dormant  
**recovery:** Core first story remains complete; later optional chapters appear only when their own prerequisites hold  
**saved as:** One-time local first story outcome plus optional separate relationship visits  

### QUEST_MAIN_CinderSpine — `CinderSpine`

**realm:** CinderSpine  
**director note:** The name sealed in mortar is not simply old: it belongs to an event removed from recorded chronology.  
**environment:** Quarry seams, ancestor mortar, rail braces and fossil family portraits.  
**player verbs:** Read alternate ancestral inscriptions from two sides of a geological cut; Place a seam clamp or named brace that changes collision-supported access; Test a dangerous rock lane after its visible fracture warning  
**npc ids:** npc.bones.seamwarden.001; npc.bones.mason.001; npc.bones.descendant.001  
**real source tags:** {"evidence":["Bones.Trial.SafeWedge","Bones.Trial.LineageStone"],"mechanism":"Bones.Trial.SeamClamp","contract_status":"authoritative tags checked against LaterRealms source"}  
**chapter level:** 2  
#### arrival

**trigger:** player within safe southern arrival zone after earned AtlasPassages crossing  
**interaction:** Inspect the real realm public Campaign.Thread marker or speak to a resident  
**objective:** Read alternate ancestral inscriptions from two sides of a geological cut  
**speaker:** npc.bones.seamwarden.001  
**line:** Each stone has an older maker's mark. Someone called these people rubble.  
**failure:** No valid sightline or current quest not started  
**recovery:** Return to safe arrival path; map marker remains readable without taking rewards  
**saved as:** world visit/initial dialogue; no new required quest flag  

#### investigation

**trigger:** local first quest begun by actual relevant NPC  
**interaction:** Inspect either physical evidence object, never resolve by dialogue text alone  
**objective:** Place a seam clamp or named brace that changes collision-supported access  
**speaker:** npc.bones.mason.001  
**line:** An unsafe brace will crush the family whose wall you mean to save.  
**evidence tags:** Bones.Trial.SafeWedge; Bones.Trial.LineageStone  
**failure:** Wrong site, unreadable text or hazard damage before evidence was collected  
**recovery:** Evidence remains independently inspectable; stable safe route, no story skip or permanent lockout  
**saved as:** LaterRealm.stage=2 and pending care/truth evidence  

#### mechanism

**trigger:** matching locally saved evidence acquired; mechanism is interactable  
**interaction:** Use actual site actor; check line of sight, stage and evidence match  
**objective:** Test a dangerous rock lane after its visible fracture warning  
**speaker:** npc.bones.seamwarden.001  
**line:** Each stone has an older maker's mark. Someone called these people rubble. The erased families compressed into history-bearing stone gives you one chance to change the place responsibly.  
**physical tag:** Bones.Trial.SeamClamp  
**failure:** Mechanism blocked, invalid/wrong evidence or missing power prerequisite  
**recovery:** Return to the matching clue and retry physical mechanism; never consume evidence on failed persistence  
**saved as:** Original first-quest mechanism action; its current stage/choice  

#### care_commit

**trigger:** mechanism and care evidence valid; chosen care route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Preserve accessible braced route and shelter without another erased generation.  
**speaker:** npc.bones.mason.001  
**line:** Build a safe passage without quarrying another remembered ancestor.  
**outcome:** care  
**failure:** SaveGameToSlot failure, mismatched scope, missed confirmation window  
**recovery:** Keep previous world and pending quest state; repeat deliberate confirmation after save works  
**saved as:** Original first-story outcome 1; physical care passage and local visible public effect  

#### truth_commit

**trigger:** mechanism and truth evidence valid; chosen truth route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Display a public family record of the quarry's origin with informed consent.  
**speaker:** npc.bones.descendant.001  
**line:** Carve the erased lineages where descendants can contest the record.  
**outcome:** truth  
**failure:** SaveGameToSlot failure, consent condition unfulfilled, mismatched confirmation  
**recovery:** Restore previous route; allow player to change evidence route before irreversible save  
**saved as:** Original first-story outcome 2; public evidence record and local visible physical effect  

#### return

**trigger:** reenter region after either outcome, optionally after Guardian/Echo/resonance  
**interaction:** talk to individual NPC by stable ID; inspect saved physical consequence  
**objective:** Repair homes using explicitly sourced materials while keeping the earlier choice visible.  
**speaker:** npc.bones.seamwarden.001  
**line:** You came back. Repair homes using explicitly sourced materials while keeping the earlier choice visible. I have not forgotten the path you chose.  
**failure:** Area unloaded or a followup system still dormant  
**recovery:** Core first story remains complete; later optional chapters appear only when their own prerequisites hold  
**saved as:** One-time local first story outcome plus optional separate relationship visits  

### QUEST_MAIN_HundredUnlived — `HundredUnlived`

**realm:** HundredUnlived  
**director note:** Unregistered residents prove that absence and nonexistence are not the same.  
**environment:** Doorways without census numbers, shelters behind name latches and voluntary public thresholds.  
**player verbs:** Open a private shelter without entering a guest's name; Examine an official recognition board and a hidden shelter without merging their records; Work an identity latch in the ethical direction supported by the witnesses  
**npc ids:** npc.unlived.censuskeeper.001; npc.unlived.caretaker.001; npc.unlived.challenger.001  
**real source tags:** {"evidence":["Unlived.Trial.HiddenShelter","Unlived.Trial.ConsentBoard"],"mechanism":"Unlived.Trial.NameLatch","contract_status":"authoritative tags checked against LaterRealms source"}  
**chapter level:** 3  
#### arrival

**trigger:** player within safe southern arrival zone after earned AtlasPassages crossing  
**interaction:** Inspect the real realm public Campaign.Thread marker or speak to a resident  
**objective:** Open a private shelter without entering a guest's name  
**speaker:** npc.unlived.censuskeeper.001  
**line:** The census says this shelter contains nobody. I can hear the children behind the door.  
**failure:** No valid sightline or current quest not started  
**recovery:** Return to safe arrival path; map marker remains readable without taking rewards  
**saved as:** world visit/initial dialogue; no new required quest flag  

#### investigation

**trigger:** local first quest begun by actual relevant NPC  
**interaction:** Inspect either physical evidence object, never resolve by dialogue text alone  
**objective:** Examine an official recognition board and a hidden shelter without merging their records  
**speaker:** npc.unlived.caretaker.001  
**line:** A welcome cup does not ask for a birth record. Can you promise the same?  
**evidence tags:** Unlived.Trial.HiddenShelter; Unlived.Trial.ConsentBoard  
**failure:** Wrong site, unreadable text or hazard damage before evidence was collected  
**recovery:** Evidence remains independently inspectable; stable safe route, no story skip or permanent lockout  
**saved as:** LaterRealm.stage=2 and pending care/truth evidence  

#### mechanism

**trigger:** matching locally saved evidence acquired; mechanism is interactable  
**interaction:** Use actual site actor; check line of sight, stage and evidence match  
**objective:** Work an identity latch in the ethical direction supported by the witnesses  
**speaker:** npc.unlived.censuskeeper.001  
**line:** The census says this shelter contains nobody. I can hear the children behind the door. The people without official beginnings whose consent matters gives you one chance to change the place responsibly.  
**physical tag:** Unlived.Trial.NameLatch  
**failure:** Mechanism blocked, invalid/wrong evidence or missing power prerequisite  
**recovery:** Return to the matching clue and retry physical mechanism; never consume evidence on failed persistence  
**saved as:** Original first-quest mechanism action; its current stage/choice  

#### care_commit

**trigger:** mechanism and care evidence valid; chosen care route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Shelter stays reachable without public identity disclosure.  
**speaker:** npc.unlived.caretaker.001  
**line:** Open a refuge that does not require anyone's name.  
**outcome:** care  
**failure:** SaveGameToSlot failure, mismatched scope, missed confirmation window  
**recovery:** Keep previous world and pending quest state; repeat deliberate confirmation after save works  
**saved as:** Original first-story outcome 1; physical care passage and local visible public effect  

#### truth_commit

**trigger:** mechanism and truth evidence valid; chosen truth route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Public recognition is opt-in and revocable, not a forced population dump.  
**speaker:** npc.unlived.challenger.001  
**line:** Let residents speak publicly by choice, not under a compulsory registry.  
**outcome:** truth  
**failure:** SaveGameToSlot failure, consent condition unfulfilled, mismatched confirmation  
**recovery:** Restore previous route; allow player to change evidence route before irreversible save  
**saved as:** Original first-story outcome 2; public evidence record and local visible physical effect  

#### return

**trigger:** reenter region after either outcome, optionally after Guardian/Echo/resonance  
**interaction:** talk to individual NPC by stable ID; inspect saved physical consequence  
**objective:** A previously hidden resident chooses whether to speak; never auto-publish a name.  
**speaker:** npc.unlived.censuskeeper.001  
**line:** You came back. A previously hidden resident chooses whether to speak; never auto-publish a name. I have not forgotten the path you chose.  
**failure:** Area unloaded or a followup system still dormant  
**recovery:** Core first story remains complete; later optional chapters appear only when their own prerequisites hold  
**saved as:** One-time local first story outcome plus optional separate relationship visits  

### QUEST_MAIN_OrchardOfKings — `OrchardOfKings`

**realm:** OrchardOfKings  
**director note:** The first legal rule was never signed; an answering mechanism continued enforcing it anyway.  
**environment:** Root courts, communal fruit stores, roads grafted from consent and a physically empty throne.  
**player verbs:** Compare a shared graft to evidence of an empty coronation; Turn a root-channel control to build one permanent walkable civic road; Question the untitled court about a legal mandate with no author  
**npc ids:** npc.orchard.untitled.001; npc.orchard.gardener.001; npc.orchard.historian.001  
**real source tags:** {"evidence":["Orchard.Trial.SharedGraft","Orchard.Trial.EmptyCoronation"],"mechanism":"Orchard.Trial.RootChannel","contract_status":"authoritative tags checked against LaterRealms source"}  
**chapter level:** 3  
#### arrival

**trigger:** player within safe southern arrival zone after earned AtlasPassages crossing  
**interaction:** Inspect the real realm public Campaign.Thread marker or speak to a resident  
**objective:** Compare a shared graft to evidence of an empty coronation  
**speaker:** npc.orchard.untitled.001  
**line:** Every crown in this orchard is empty. Yet the roads obey a law we never passed.  
**failure:** No valid sightline or current quest not started  
**recovery:** Return to safe arrival path; map marker remains readable without taking rewards  
**saved as:** world visit/initial dialogue; no new required quest flag  

#### investigation

**trigger:** local first quest begun by actual relevant NPC  
**interaction:** Inspect either physical evidence object, never resolve by dialogue text alone  
**objective:** Turn a root-channel control to build one permanent walkable civic road  
**speaker:** npc.orchard.gardener.001  
**line:** A graft is an agreement between living branches; you cannot command either one.  
**evidence tags:** Orchard.Trial.SharedGraft; Orchard.Trial.EmptyCoronation  
**failure:** Wrong site, unreadable text or hazard damage before evidence was collected  
**recovery:** Evidence remains independently inspectable; stable safe route, no story skip or permanent lockout  
**saved as:** LaterRealm.stage=2 and pending care/truth evidence  

#### mechanism

**trigger:** matching locally saved evidence acquired; mechanism is interactable  
**interaction:** Use actual site actor; check line of sight, stage and evidence match  
**objective:** Question the untitled court about a legal mandate with no author  
**speaker:** npc.orchard.untitled.001  
**line:** Every crown in this orchard is empty. Yet the roads obey a law we never passed. The unwritten laws growing from living roots gives you one chance to change the place responsibly.  
**physical tag:** Orchard.Trial.RootChannel  
**failure:** Mechanism blocked, invalid/wrong evidence or missing power prerequisite  
**recovery:** Return to the matching clue and retry physical mechanism; never consume evidence on failed persistence  
**saved as:** Original first-quest mechanism action; its current stage/choice  

#### care_commit

**trigger:** mechanism and care evidence valid; chosen care route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Share the road and harvest without an inherited crown.  
**speaker:** npc.orchard.gardener.001  
**line:** Open the harvest paths to everyone willing to care for them.  
**outcome:** care  
**failure:** SaveGameToSlot failure, mismatched scope, missed confirmation window  
**recovery:** Keep previous world and pending quest state; repeat deliberate confirmation after save works  
**saved as:** Original first-story outcome 1; physical care passage and local visible public effect  

#### truth_commit

**trigger:** mechanism and truth evidence valid; chosen truth route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Expose the unratified court law while preserving living people's right to dissent.  
**speaker:** npc.orchard.historian.001  
**line:** Show the unratified law. A crown without a wearer cannot consent for us.  
**outcome:** truth  
**failure:** SaveGameToSlot failure, consent condition unfulfilled, mismatched confirmation  
**recovery:** Restore previous route; allow player to change evidence route before irreversible save  
**saved as:** Original first-story outcome 2; public evidence record and local visible physical effect  

#### return

**trigger:** reenter region after either outcome, optionally after Guardian/Echo/resonance  
**interaction:** talk to individual NPC by stable ID; inspect saved physical consequence  
**objective:** Roots remember elected access and old disputes instead of being magically healed.  
**speaker:** npc.orchard.untitled.001  
**line:** You came back. Roots remember elected access and old disputes instead of being magically healed. I have not forgotten the path you chose.  
**failure:** Area unloaded or a followup system still dormant  
**recovery:** Core first story remains complete; later optional chapters appear only when their own prerequisites hold  
**saved as:** One-time local first story outcome plus optional separate relationship visits  

### QUEST_MAIN_FirstAbsence — `FirstAbsence`

**realm:** FirstAbsence  
**director note:** Reveal that Nhal-Vey suppresses questions; don't reveal false-victory twist during ordinary traversal.  
**environment:** Doorways without foundations, contradictory source maps, stable return line and northern final arena.  
**player verbs:** Read both stable-memory and contradictory-map evidence before choosing an origin anchor; Open one real threshold with stable fallback travel; Confront Nhal-Vey's mask, unveiled form, and deliberate future-choice controls  
**npc ids:** npc.absence.firstwitness.001; npc.absence.caretaker.001; npc.absence.otherwitness.001  
**real source tags:** {"evidence":["Absence.Trial.StableMemory","Absence.Trial.ContradictoryMap"],"mechanism":"Absence.Trial.OriginAnchor","contract_status":"authoritative tags checked against LaterRealms source"}  
**chapter level:** 4  
#### arrival

**trigger:** player within safe southern arrival zone after earned AtlasPassages crossing  
**interaction:** Inspect the real realm public Campaign.Thread marker or speak to a resident  
**objective:** Read both stable-memory and contradictory-map evidence before choosing an origin anchor  
**speaker:** npc.absence.firstwitness.001  
**line:** I remember your arrival before this threshold existed. I don't know which memory came first.  
**failure:** No valid sightline or current quest not started  
**recovery:** Return to safe arrival path; map marker remains readable without taking rewards  
**saved as:** world visit/initial dialogue; no new required quest flag  

#### investigation

**trigger:** local first quest begun by actual relevant NPC  
**interaction:** Inspect either physical evidence object, never resolve by dialogue text alone  
**objective:** Open one real threshold with stable fallback travel  
**speaker:** npc.absence.caretaker.001  
**line:** A safe door is more useful than deciding whose beginning is true.  
**evidence tags:** Absence.Trial.StableMemory; Absence.Trial.ContradictoryMap  
**failure:** Wrong site, unreadable text or hazard damage before evidence was collected  
**recovery:** Evidence remains independently inspectable; stable safe route, no story skip or permanent lockout  
**saved as:** LaterRealm.stage=2 and pending care/truth evidence  

#### mechanism

**trigger:** matching locally saved evidence acquired; mechanism is interactable  
**interaction:** Use actual site actor; check line of sight, stage and evidence match  
**objective:** Confront Nhal-Vey's mask, unveiled form, and deliberate future-choice controls  
**speaker:** npc.absence.firstwitness.001  
**line:** I remember your arrival before this threshold existed. I don't know which memory came first. The two truthful beginnings at one usable threshold gives you one chance to change the place responsibly.  
**physical tag:** Absence.Trial.OriginAnchor  
**failure:** Mechanism blocked, invalid/wrong evidence or missing power prerequisite  
**recovery:** Return to the matching clue and retry physical mechanism; never consume evidence on failed persistence  
**saved as:** Original first-quest mechanism action; its current stage/choice  

#### care_commit

**trigger:** mechanism and care evidence valid; chosen care route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Reliable passage exists without erasing other origins.  
**speaker:** npc.absence.caretaker.001  
**line:** Leave a stable return for every traveler, regardless of origin.  
**outcome:** care  
**failure:** SaveGameToSlot failure, mismatched scope, missed confirmation window  
**recovery:** Keep previous world and pending quest state; repeat deliberate confirmation after save works  
**saved as:** Original first-story outcome 1; physical care passage and local visible public effect  

#### truth_commit

**trigger:** mechanism and truth evidence valid; chosen truth route explicitly previewed and confirmed  
**interaction:** two deliberate matching confirmations; physical branch and collision must change  
**objective:** Both maps remain inspectable without one mandatory universal history.  
**speaker:** npc.absence.otherwitness.001  
**line:** Preserve both honest maps. Don't grant one witness the power to erase the other.  
**outcome:** truth  
**failure:** SaveGameToSlot failure, consent condition unfulfilled, mismatched confirmation  
**recovery:** Restore previous route; allow player to change evidence route before irreversible save  
**saved as:** Original first-story outcome 2; public evidence record and local visible physical effect  

#### return

**trigger:** reenter region after either outcome, optionally after Guardian/Echo/resonance  
**interaction:** talk to individual NPC by stable ID; inspect saved physical consequence  
**objective:** Held Morning uses bell/names echo attacks; Many Mornings uses horizon/counterfactual echoes.  
**speaker:** npc.absence.firstwitness.001  
**line:** You came back. Held Morning uses bell/names echo attacks; Many Mornings uses horizon/counterfactual echoes. I have not forgotten the path you chose.  
**failure:** Area unloaded or a followup system still dormant  
**recovery:** Core first story remains complete; later optional chapters appear only when their own prerequisites hold  
**saved as:** One-time local first story outcome plus optional separate relationship visits  


## Full grouped quest and adventure ledger

### EXPEDITION QUEST FAMILY (9)

#### The Split Charter Vault — `quest:expedition:ThreefoldReach`

**realm:** ThreefoldReach  
**entry:** Take the board at Crossings, travel to Rookstep, enter The Split Charter Vault after passing Lantern Rest.  
**choices:** Rescue two captives and face boss; Inspect three independent records to permit the boss's living surrender  
**aftermath:** Return gate opens and patrols stop; individually released captives return, unreleased captives remain in custody; Boss remains alive and patrols stand down; captivity state is not overwritten  
**rewards:** gear:ThreefoldReach:weapon; gear:ThreefoldReach:armor; skill:technique:ThreefoldReach  
**boss:** boss:expedition:ThreefoldReach  
**exclusive reward:** true  
**source:** World/UnmadeExpeditionRegistry.h  

#### The Hollow Breakwater — `quest:expedition:WidowedRain`

**realm:** WidowedRain  
**entry:** Take the board at Saltwake, travel to Brinewatch, enter The Hollow Breakwater after passing Drywake.  
**choices:** Rescue two captives and face boss; Inspect three independent records to permit the boss's living surrender  
**aftermath:** Return gate opens and patrols stop; individually released captives return, unreleased captives remain in custody; Boss remains alive and patrols stand down; captivity state is not overwritten  
**rewards:** gear:WidowedRain:weapon; gear:WidowedRain:armor; skill:technique:WidowedRain  
**boss:** boss:expedition:WidowedRain  
**exclusive reward:** true  
**source:** World/UnmadeExpeditionRegistry.h  

#### The Furnace Below Blackslag — `quest:expedition:HearthBeneath`

**realm:** HearthBeneath  
**entry:** Take the board at Cinderhold, travel to Emberwatch, enter The Furnace Below Blackslag after passing Snowmelt Rest.  
**choices:** Rescue two captives and face boss; Inspect three independent records to permit the boss's living surrender  
**aftermath:** Return gate opens and patrols stop; individually released captives return, unreleased captives remain in custody; Boss remains alive and patrols stand down; captivity state is not overwritten  
**rewards:** gear:HearthBeneath:weapon; gear:HearthBeneath:armor; skill:technique:HearthBeneath  
**boss:** boss:expedition:HearthBeneath  
**exclusive reward:** true  
**source:** World/UnmadeExpeditionRegistry.h  

#### The Foreclosed Anchorage — `quest:expedition:TidalLedger`

**realm:** TidalLedger  
**entry:** Take the board at Drevlach, travel to Oathshoal, enter The Foreclosed Anchorage after passing Reedwake.  
**choices:** Rescue two captives and face boss; Inspect three independent records to permit the boss's living surrender  
**aftermath:** Return gate opens and patrols stop; individually released captives return, unreleased captives remain in custody; Boss remains alive and patrols stand down; captivity state is not overwritten  
**rewards:** gear:TidalLedger:weapon; gear:TidalLedger:armor; skill:technique:TidalLedger  
**boss:** boss:expedition:TidalLedger  
**exclusive reward:** true  
**source:** World/UnmadeExpeditionRegistry.h  

#### The Fallen Choir Engine — `quest:expedition:SkyBelow`

**realm:** SkyBelow  
**entry:** Take the board at Orravane, travel to Downsong, enter The Fallen Choir Engine after passing Quiet Span.  
**choices:** Rescue two captives and face boss; Inspect three independent records to permit the boss's living surrender  
**aftermath:** Return gate opens and patrols stop; individually released captives return, unreleased captives remain in custody; Boss remains alive and patrols stand down; captivity state is not overwritten  
**rewards:** gear:SkyBelow:weapon; gear:SkyBelow:armor; skill:technique:SkyBelow  
**boss:** boss:expedition:SkyBelow  
**exclusive reward:** true  
**source:** World/UnmadeExpeditionRegistry.h  

#### The Buried Yesterday Foundry — `quest:expedition:CinderSpine`

**realm:** CinderSpine  
**entry:** Take the board at Vathless, travel to Seamguard, enter The Buried Yesterday Foundry after passing Kinrest.  
**choices:** Rescue two captives and face boss; Inspect three independent records to permit the boss's living surrender  
**aftermath:** Return gate opens and patrols stop; individually released captives return, unreleased captives remain in custody; Boss remains alive and patrols stand down; captivity state is not overwritten  
**rewards:** gear:CinderSpine:weapon; gear:CinderSpine:armor; skill:technique:CinderSpine  
**boss:** boss:expedition:CinderSpine  
**exclusive reward:** true  
**source:** World/UnmadeExpeditionRegistry.h  

#### The Unentered Census House — `quest:expedition:HundredUnlived`

**realm:** HundredUnlived  
**entry:** Take the board at Eillun, travel to Elseward, enter The Unentered Census House after passing Unlisted Hearth.  
**choices:** Rescue two captives and face boss; Inspect three independent records to permit the boss's living surrender  
**aftermath:** Return gate opens and patrols stop; individually released captives return, unreleased captives remain in custody; Boss remains alive and patrols stand down; captivity state is not overwritten  
**rewards:** gear:HundredUnlived:weapon; gear:HundredUnlived:armor; skill:technique:HundredUnlived  
**boss:** boss:expedition:HundredUnlived  
**exclusive reward:** true  
**source:** World/UnmadeExpeditionRegistry.h  

#### The Crownroot Keep — `quest:expedition:OrchardOfKings`

**realm:** OrchardOfKings  
**entry:** Take the board at Tharniv, travel to Rootwatch, enter The Crownroot Keep after passing Graft Rest.  
**choices:** Rescue two captives and face boss; Inspect three independent records to permit the boss's living surrender  
**aftermath:** Return gate opens and patrols stop; individually released captives return, unreleased captives remain in custody; Boss remains alive and patrols stand down; captivity state is not overwritten  
**rewards:** gear:OrchardOfKings:weapon; gear:OrchardOfKings:armor; skill:technique:OrchardOfKings  
**boss:** boss:expedition:OrchardOfKings  
**exclusive reward:** true  
**source:** World/UnmadeExpeditionRegistry.h  

#### The Unfinished Bastion — `quest:expedition:FirstAbsence`

**realm:** FirstAbsence  
**entry:** Take the board at Auvren, travel to Threshold Watch, enter The Unfinished Bastion after passing Nearrest.  
**choices:** Rescue two captives and face boss; Inspect three independent records to permit the boss's living surrender  
**aftermath:** Return gate opens and patrols stop; individually released captives return, unreleased captives remain in custody; Boss remains alive and patrols stand down; captivity state is not overwritten  
**rewards:** gear:FirstAbsence:weapon; gear:FirstAbsence:armor; skill:technique:FirstAbsence  
**boss:** boss:expedition:FirstAbsence  
**exclusive reward:** true  
**source:** World/UnmadeExpeditionRegistry.h  

### RITE QUEST FAMILY (10)

#### The Law Beneath the Last Bell — `quest:rite:UnwriteLaw`

**realm:** Bellgrave  
**entry:** Find the actual inscription at Bellgrave, speak to npc.bellmaker.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:BellmakersVerse  
**source:** World/UnmadeTenfoldChronicle.h  

#### The Bridge of Living Testimony — `quest:rite:Witnesscraft`

**realm:** EchoWell  
**entry:** Find the actual inscription at EchoWell, speak to npc.welllistener.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:RememberedBridge  
**source:** World/UnmadeTenfoldChronicle.h  

#### The Life Not Taken — `quest:rite:BorrowedLives`

**realm:** PaperOrchard  
**entry:** Find the actual inscription at PaperOrchard, speak to npc.orchardexile.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:SecondName  
**source:** World/UnmadeTenfoldChronicle.h  

#### A Blade That Was There — `quest:rite:LegacyForging`

**realm:** BellwoldForge  
**entry:** Find the actual inscription at BellwoldForge, speak to npc.bellwold.stonewright.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:ThreeDeedSteel  
**source:** World/UnmadeTenfoldChronicle.h  

#### No Road Belongs to One — `quest:rite:LivingRoads`

**realm:** SilentMile  
**entry:** Find the actual inscription at SilentMile, speak to npc.roadwarden.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:CommonCauseway  
**source:** World/UnmadeTenfoldChronicle.h  

#### The Ledger of Borrowed Dawn — `quest:rite:TomorrowDebt`

**realm:** DebtMarket  
**entry:** Find the actual inscription at DebtMarket, speak to npc.tollbroker.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:FutureReceipt  
**source:** World/UnmadeTenfoldChronicle.h  

#### The Name Beneath the Monster — `quest:rite:UnderstandingBosses`

**realm:** BellOssuary  
**entry:** Find the actual inscription at BellOssuary, speak to npc.bellwold.matron.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:HollowMercy  
**source:** World/UnmadeTenfoldChronicle.h  

#### The Tower That Fell and Stood — `quest:rite:ParadoxConvergence`

**realm:** PaperhavenScribe  
**entry:** Find the actual inscription at PaperhavenScribe, speak to npc.paperhaven.mapmaker.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:BetweenHistories  
**source:** World/UnmadeTenfoldChronicle.h  

#### The Lantern You Promised — `quest:rite:Oathbinding`

**realm:** BellwoldRefuge  
**entry:** Find the actual inscription at BellwoldRefuge, speak to npc.bellwold.guard.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:UnbrokenLantern  
**source:** World/UnmadeTenfoldChronicle.h  

#### The Map of Disagreeing Shores — `quest:rite:Cartography`

**realm:** SaltwakeHarbor  
**entry:** Find the actual inscription at SaltwakeHarbor, speak to npc.saltwake.navigator.001, cast real trial, then secure firsthand witnesses.  
**choices:** Protect; Reveal  
**aftermath:** A witnessed civic choice is saved and rite mastery modifies allowable reality actions; effects incur Strain/cooldowns.  
**rewards:** item:CoastTrueAtlas  
**source:** World/UnmadeTenfoldChronicle.h  

### CONFLUENCE QUEST FAMILY (6)

#### The Alarm Nobody Heard — `quest:confluence:SilentAlarm`

**realm:** ThreefoldReach  
**entry:** Master UnwriteLaw and Witnesscraft, cast each at the chamber within 120 seconds, bring two actual witnesses.  
**choices:** Protect; Reveal  
**aftermath:** Two-power mastery and witnessed decision persist; single earned mythic relic.  
**rewards:** item:LastWarningAegis  
**source:** World/UnmadeConfluenceRules.h  

#### A Blade for Both Your Names — `quest:confluence:TwoNames`

**realm:** WidowedRain  
**entry:** Master BorrowedLives and LegacyForging, cast each at the chamber within 120 seconds, bring two actual witnesses.  
**choices:** Protect; Reveal  
**aftermath:** Two-power mastery and witnessed decision persist; single earned mythic relic.  
**rewards:** item:ElsebornSmithEdge  
**source:** World/UnmadeConfluenceRules.h  

#### The Route Nobody Owned — `quest:confluence:UnmappedWay`

**realm:** HearthBeneath  
**entry:** Master LivingRoads and Cartography, cast each at the chamber within 120 seconds, bring two actual witnesses.  
**choices:** Protect; Reveal  
**aftermath:** Two-power mastery and witnessed decision persist; single earned mythic relic.  
**rewards:** item:CommonPathChart  
**source:** World/UnmadeConfluenceRules.h  

#### Tomorrow Is Not Yours to Spend — `quest:confluence:TomorrowPromise`

**realm:** TidalLedger  
**entry:** Master TomorrowDebt and Oathbinding, cast each at the chamber within 120 seconds, bring two actual witnesses.  
**choices:** Protect; Reveal  
**aftermath:** Two-power mastery and witnessed decision persist; single earned mythic relic.  
**rewards:** item:BorrowedDawnHeart  
**source:** World/UnmadeConfluenceRules.h  

#### The Bell Fell and Stood — `quest:confluence:TwoKeepers`

**realm:** SkyBelow  
**entry:** Master UnderstandingBosses and ParadoxConvergence, cast each at the chamber within 120 seconds, bring two actual witnesses.  
**choices:** Protect; Reveal  
**aftermath:** Two-power mastery and witnessed decision persist; single earned mythic relic.  
**rewards:** item:TwoKeepersMirror  
**source:** World/UnmadeConfluenceRules.h  

#### The Place Before a First Story — `quest:confluence:FirstAbsence`

**realm:** CinderSpine  
**entry:** Master UnwriteLaw and Cartography, cast each at the chamber within 120 seconds, bring two actual witnesses.  
**choices:** Protect; Reveal  
**aftermath:** Two-power mastery and witnessed decision persist; single earned mythic relic.  
**rewards:** item:FirstAbsenceWitness  
**source:** World/UnmadeConfluenceRules.h  

### WING QUEST FAMILY (9)

#### Lantern Survey Gallery — `quest:wing:ThreefoldReach`

**realm:** ThreefoldReach  
**entry:** Open Lantern Survey Gallery from the expedition and complete three physical mechanisms.  
**choices:** Defeat  
**aftermath:** Saved wing completion grants 5 nonrepeatable insight for local build/tool/recipe/service upgrades.  
**rewards:** resource:insight:5  
**boss:** boss:wing:ThreefoldReach  
**source:** World/UnmadePlayerExperienceRegistry.h  

#### Drowned Pump Annex — `quest:wing:WidowedRain`

**realm:** WidowedRain  
**entry:** Open Drowned Pump Annex from the expedition and complete three physical mechanisms.  
**choices:** Defeat  
**aftermath:** Saved wing completion grants 5 nonrepeatable insight for local build/tool/recipe/service upgrades.  
**rewards:** resource:insight:5  
**boss:** boss:wing:WidowedRain  
**source:** World/UnmadePlayerExperienceRegistry.h  

#### Cooling Ledger Foundry — `quest:wing:HearthBeneath`

**realm:** HearthBeneath  
**entry:** Open Cooling Ledger Foundry from the expedition and complete three physical mechanisms.  
**choices:** Defeat  
**aftermath:** Saved wing completion grants 5 nonrepeatable insight for local build/tool/recipe/service upgrades.  
**rewards:** resource:insight:5  
**boss:** boss:wing:HearthBeneath  
**source:** World/UnmadePlayerExperienceRegistry.h  

#### Broken Oath Court — `quest:wing:TidalLedger`

**realm:** TidalLedger  
**entry:** Open Broken Oath Court from the expedition and complete three physical mechanisms.  
**choices:** Defeat; Earn guard-and-oath surrender  
**aftermath:** Saved wing completion grants 5 nonrepeatable insight for local build/tool/recipe/service upgrades.  
**rewards:** resource:insight:5  
**boss:** boss:wing:TidalLedger  
**source:** World/UnmadePlayerExperienceRegistry.h  

#### Choir Lift Observatory — `quest:wing:SkyBelow`

**realm:** SkyBelow  
**entry:** Open Choir Lift Observatory from the expedition and complete three physical mechanisms.  
**choices:** Defeat  
**aftermath:** Saved wing completion grants 5 nonrepeatable insight for local build/tool/recipe/service upgrades.  
**rewards:** resource:insight:5  
**boss:** boss:wing:SkyBelow  
**source:** World/UnmadePlayerExperienceRegistry.h  

#### Seam Memory Archive — `quest:wing:CinderSpine`

**realm:** CinderSpine  
**entry:** Open Seam Memory Archive from the expedition and complete three physical mechanisms.  
**choices:** Defeat  
**aftermath:** Saved wing completion grants 5 nonrepeatable insight for local build/tool/recipe/service upgrades.  
**rewards:** resource:insight:5  
**boss:** boss:wing:CinderSpine  
**source:** World/UnmadePlayerExperienceRegistry.h  

#### Unobserved Index Vault — `quest:wing:HundredUnlived`

**realm:** HundredUnlived  
**entry:** Open Unobserved Index Vault from the expedition and complete three physical mechanisms.  
**choices:** Defeat  
**aftermath:** Saved wing completion grants 5 nonrepeatable insight for local build/tool/recipe/service upgrades.  
**rewards:** resource:insight:5  
**boss:** boss:wing:HundredUnlived  
**source:** World/UnmadePlayerExperienceRegistry.h  

#### Ungrafted Root Nursery — `quest:wing:OrchardOfKings`

**realm:** OrchardOfKings  
**entry:** Open Ungrafted Root Nursery from the expedition and complete three physical mechanisms.  
**choices:** Defeat  
**aftermath:** Saved wing completion grants 5 nonrepeatable insight for local build/tool/recipe/service upgrades.  
**rewards:** resource:insight:5  
**boss:** boss:wing:OrchardOfKings  
**source:** World/UnmadePlayerExperienceRegistry.h  

#### Threshold Counterweight Hall — `quest:wing:FirstAbsence`

**realm:** FirstAbsence  
**entry:** Open Threshold Counterweight Hall from the expedition and complete three physical mechanisms.  
**choices:** Defeat  
**aftermath:** Saved wing completion grants 5 nonrepeatable insight for local build/tool/recipe/service upgrades.  
**rewards:** resource:insight:5  
**boss:** boss:wing:FirstAbsence  
**source:** World/UnmadePlayerExperienceRegistry.h  

### ROAD QUEST FAMILY (11)

#### The Uncounted Kettle — `quest:road:Crossings`

**realm:** ThreefoldReach  
**entry:** Speak to npc.marketporter.001, inspect all three roadside records and visit chalk witness post.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite sealed kettle fittings, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Crossings  
**boss:** boss:road:Crossings  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### A Wick Beyond the Bell — `quest:road:Bellwold`

**realm:** ThreefoldReach  
**entry:** Speak to npc.bellwold.woodcarrier.001, inspect all three roadside records and visit bell screen.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite lantern wick rolls, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Bellwold  
**boss:** boss:road:Bellwold  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### The Second Copy Arrives — `quest:road:Paperhaven`

**realm:** ThreefoldReach  
**entry:** Speak to npc.paperhaven.lettercarrier.001, inspect all three roadside records and visit copy press.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite blank repair folios, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Paperhaven  
**boss:** boss:road:Paperhaven  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### Dry Bedding, Wet Road — `quest:road:Saltwake`

**realm:** WidowedRain  
**entry:** Speak to npc.saltwake.courier.001, inspect all three roadside records and visit rain shutter.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite drying cloth bundles, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Saltwake  
**boss:** boss:road:Saltwake  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### The First Cart Through — `quest:road:Cinderhold`

**realm:** HearthBeneath  
**entry:** Speak to npc.cinderhold.courier.001, inspect all three roadside records and visit kiln baffle.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite kiln patch bricks, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Cinderhold  
**boss:** boss:road:Cinderhold  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### Cargo Without Collateral — `quest:road:Drevlach`

**realm:** TidalLedger  
**entry:** Speak to npc.tidal.ferryman.001, inspect all three roadside records and visit tide clamp.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite ferry patch boards, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Drevlach  
**boss:** boss:road:Drevlach  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### When the Choir Rests — `quest:road:Orravane`

**realm:** SkyBelow  
**entry:** Speak to npc.sky.structuralist.001, inspect all three roadside records and visit counterweight brake.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite support pin cases, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Orravane  
**boss:** boss:road:Orravane  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### Stone That Keeps Its Kin — `quest:road:Vathless`

**realm:** CinderSpine  
**entry:** Speak to npc.bones.mason.001, inspect all three roadside records and visit seam brace.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite plain seam wedges, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Vathless  
**boss:** boss:road:Vathless  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### An Unnamed Door Opens — `quest:road:Eillun`

**realm:** HundredUnlived  
**entry:** Speak to npc.unlived.caretaker.001, inspect all three roadside records and visit privacy screen.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite unmarked door fittings, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Eillun  
**boss:** boss:road:Eillun  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### The Common Graft Travels — `quest:road:Tharniv`

**realm:** OrchardOfKings  
**entry:** Speak to npc.orchard.gardener.001, inspect all three roadside records and visit root sluice.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite common graft splints, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Tharniv  
**boss:** boss:road:Tharniv  
**source:** World/UnmadeRememberedRoadRegistry.h  

#### A Doorway Still Receives — `quest:road:Auvren`

**realm:** FirstAbsence  
**entry:** Speak to npc.absence.caretaker.001, inspect all three roadside records and visit threshold brace.  
**choices:** Defeat resident elder; Cool nest mechanism and quiet elder with evidence  
**aftermath:** Open or maintain the passage, deliver finite threshold anchor pegs, record receipt, then return to people who physically learned the result.  
**rewards:** world:road:Auvren  
**boss:** boss:road:Auvren  
**source:** World/UnmadeRememberedRoadRegistry.h  

### CAMPAIGN QUEST FAMILY (9)

#### A Bell That Had No Last Note — `quest:campaign:ThreefoldReach`

**realm:** ThreefoldReach  
**entry:** Why does the Crossings bell ring from a road that no one has walked?  
**choices:** Resolve eligible chapter and follow one of its supported alternate routes  
**aftermath:** In Bellwold a refugee remembers a bell that has never been cast. In Paperhaven its absence has a signed record. Neither settlement can prove the other wrong.  
**rewards:** world:campaign_progress  
**source:** World/UnmadeCampaignSpineRules.h  

#### The Shore Without Its Water — `quest:campaign:WidowedRain`

**realm:** WidowedRain  
**entry:** Who can remember a sea that is missing without owning the grief of those left ashore?  
**choices:** Resolve eligible chapter and follow one of its supported alternate routes  
**aftermath:** Sella's sea chart is not an error. The rain preserved a record of something intentionally withheld, and the debt dates are written in another city's script.  
**rewards:** world:campaign_progress  
**source:** World/UnmadeCampaignSpineRules.h  

#### The Heat Lent By No Sun — `quest:campaign:HearthBeneath`

**realm:** HearthBeneath  
**entry:** Who keeps a common flame burning when the sun whose gift it was never existed?  
**choices:** Resolve eligible chapter and follow one of its supported alternate routes  
**aftermath:** Tarin reads a fire with the same missing second found in the bell. The city's warmth survives because someone accepted a cost they did not create.  
**rewards:** world:campaign_progress  
**source:** World/UnmadeCampaignSpineRules.h  

#### The Account That Borrowed Tomorrow — `quest:campaign:TidalLedger`

**realm:** TidalLedger  
**entry:** Can a debt be legitimate when its signatory has not been born?  
**choices:** Resolve eligible chapter and follow one of its supported alternate routes  
**aftermath:** Veynu's impossible dates show that the world's missing things were pledged as collateral; a contract is written on the underside of reality itself.  
**rewards:** world:campaign_progress  
**source:** World/UnmadeCampaignSpineRules.h  

#### The Chorus That Holds The Roof — `quest:campaign:SkyBelow`

**realm:** SkyBelow  
**entry:** How many people were removed from history to keep a single hanging street above the ground?  
**choices:** Resolve eligible chapter and follow one of its supported alternate routes  
**aftermath:** Yllesh's missing voice proves that the same absences are doing work in several realms. The common pattern is a decision, not an accident.  
**rewards:** world:campaign_progress  
**source:** World/UnmadeCampaignSpineRules.h  

#### The Stone That Remembered Workers — `quest:campaign:CinderSpine`

**realm:** CinderSpine  
**entry:** When a family is mined out of time, who gets to call its descendants fictional?  
**choices:** Resolve eligible chapter and follow one of its supported alternate routes  
**aftermath:** Ghraet's seams show erased lineages in the mountain's living load. Someone has been converting impossible people into stable places.  
**rewards:** world:campaign_progress  
**source:** World/UnmadeCampaignSpineRules.h  

#### The Census Of Those Not Born — `quest:campaign:HundredUnlived`

**realm:** HundredUnlived  
**entry:** Can a person be a valid witness without having a beginning recognized by the city?  
**choices:** Resolve eligible chapter and follow one of its supported alternate routes  
**aftermath:** Eillun's unentered people are not echoes; they live and disagree. A missing origin is not evidence that someone deserves to be erased.  
**rewards:** world:campaign_progress  
**source:** World/UnmadeCampaignSpineRules.h  

#### The Crown Nobody Accepted — `quest:campaign:OrchardOfKings`

**realm:** OrchardOfKings  
**entry:** Who has been enforcing the world's oldest rule after every prospective ruler refused it?  
**choices:** Resolve eligible chapter and follow one of its supported alternate routes  
**aftermath:** Tharniv's root-court remembers a law signed by nobody. Its force survived because an answering consciousness was made responsible for every unresolved question.  
**rewards:** world:campaign_progress  
**source:** World/UnmadeCampaignSpineRules.h  

#### The Answer With No Question — `quest:campaign:FirstAbsence`

**realm:** FirstAbsence  
**entry:** Why does the first witness remember a traveler who has not yet arrived?  
**choices:** Resolve eligible chapter and follow one of its supported alternate routes  
**aftermath:** Auvren reveals a seam older than either origin. Nhal-Vey did not create the contradictions; it has been eating the questions people were afraid to ask.  
**rewards:** world:campaign_progress  
**source:** World/UnmadeCampaignSpineRules.h  

### AFTERMATH QUEST FAMILY (9)

#### The Road After the Rescue — `quest:aftermath:ThreefoldReach`

**realm:** ThreefoldReach  
**entry:** After the first local arc, meet npc.roadwarden.001, operate Crossings.Aftermath.RoadSwitch, inspect evidence and secure a real witness.  
**choices:** Care at Crossings.Aftermath.SharedRoute; Truth at Crossings.Aftermath.TollArchive  
**aftermath:** The public crossing remains open after dark.; Hidden toll histories become public records.  
**rewards:** world:aftermath:ThreefoldReach  
**source:** World/UnmadeRealmAftermathRules.h  

#### The Storm's Second Invoice — `quest:aftermath:WidowedRain`

**realm:** WidowedRain  
**entry:** After the first local arc, meet npc.saltwake.harborwarden.001, operate Saltwake.Aftermath.StormSluice, inspect evidence and secure a real witness.  
**choices:** Care at Saltwake.Aftermath.Cistern; Truth at Saltwake.Aftermath.RainLedger  
**aftermath:** Rain is collected into a shared cistern.; The rain broker's false tide prices become public.  
**rewards:** world:aftermath:WidowedRain  
**source:** World/UnmadeRealmAftermathRules.h  

#### The Ember That Chose No Heir — `quest:aftermath:HearthBeneath`

**realm:** HearthBeneath  
**entry:** After the first local arc, meet npc.cinderhold.emberwarden.001, operate Cinderhold.Aftermath.HeatVent, inspect evidence and secure a real witness.  
**choices:** Care at Cinderhold.Aftermath.CommonKiln; Truth at Cinderhold.Aftermath.EmberDeed  
**aftermath:** Households share a heat-safe communal kiln.; An illegal ownership claim on the eternal ember is exposed.  
**rewards:** world:aftermath:HearthBeneath  
**source:** World/UnmadeRealmAftermathRules.h  

#### The Unpaid Person — `quest:aftermath:TidalLedger`

**realm:** TidalLedger  
**entry:** After the first local arc, meet npc.tidal.ledgerkeeper.001, operate Tidal.Aftermath.DebtLock, inspect evidence and secure a real witness.  
**choices:** Care at Tidal.Aftermath.RefugeBonds; Truth at Tidal.Aftermath.MissingDebtor  
**aftermath:** Future debts are forgiven for the stranded.; The unborn people used as collateral are named.  
**rewards:** world:aftermath:TidalLedger  
**source:** World/UnmadeRealmAftermathRules.h  

#### The Note That Held a Home — `quest:aftermath:SkyBelow`

**realm:** SkyBelow  
**entry:** After the first local arc, meet npc.sky.choirmaster.001, operate Sky.Aftermath.HarmonicTuner, inspect evidence and secure a real witness.  
**choices:** Care at Sky.Aftermath.HarmonicBrace; Truth at Sky.Aftermath.SilencedVerse  
**aftermath:** A choir-supported neighborhood gains stable footing.; Forbidden notes become audible to the public.  
**rewards:** world:aftermath:SkyBelow  
**source:** World/UnmadeRealmAftermathRules.h  

#### The Quarry of Other Yesterdays — `quest:aftermath:CinderSpine`

**realm:** CinderSpine  
**entry:** After the first local arc, meet npc.bones.seamwarden.001, operate Bones.Aftermath.SeamBrace, inspect evidence and secure a real witness.  
**choices:** Care at Bones.Aftermath.ProtectedSeam; Truth at Bones.Aftermath.DescendantLedger  
**aftermath:** Timeline mining is halted near living homes.; The erased lineage of quarry workers is documented.  
**rewards:** world:aftermath:CinderSpine  
**source:** World/UnmadeRealmAftermathRules.h  

#### The Street of Unclaimed Birthdays — `quest:aftermath:HundredUnlived`

**realm:** HundredUnlived  
**entry:** After the first local arc, meet npc.unlived.censuskeeper.001, operate Unlived.Aftermath.DoorOfNames, inspect evidence and secure a real witness.  
**choices:** Care at Unlived.Aftermath.ShelterRegistry; Truth at Unlived.Aftermath.VoiceLedger  
**aftermath:** A shelter recognizes residents without birth documents.; The city publicly acknowledges its impossible citizens.  
**rewards:** world:aftermath:HundredUnlived  
**source:** World/UnmadeRealmAftermathRules.h  

#### The Crown No One Wanted — `quest:aftermath:OrchardOfKings`

**realm:** OrchardOfKings  
**entry:** After the first local arc, meet npc.orchard.untitled.001, operate Orchard.Aftermath.RootValve, inspect evidence and secure a real witness.  
**choices:** Care at Orchard.Aftermath.CommonRoots; Truth at Orchard.Aftermath.UnclaimedThrone  
**aftermath:** The untitled courts maintain a public orchard.; The last concealed claim to kingship is revealed.  
**rewards:** world:aftermath:OrchardOfKings  
**source:** World/UnmadeRealmAftermathRules.h  

#### The Place After the Ending — `quest:aftermath:FirstAbsence`

**realm:** FirstAbsence  
**entry:** After the first local arc, meet npc.absence.firstwitness.001, operate Absence.Aftermath.OriginDial, inspect evidence and secure a real witness.  
**choices:** Care at Absence.Aftermath.SharedBeginning; Truth at Absence.Aftermath.ManyOrigins  
**aftermath:** A stable passage remains open for other travelers.; Multiple mutually true beginnings are preserved.  
**rewards:** world:aftermath:FirstAbsence  
**source:** World/UnmadeRealmAftermathRules.h  

### ECHO QUEST FAMILY (6)

#### The Child Who Repaid Tomorrow — `quest:echo:TidalLedger`

**realm:** TidalLedger  
**entry:** Complete the first arc and aftermath, speak with npc.tidal.ledgerkeeper.001, investigate two distinct physical clues.  
**choices:** Care witnessed resolution; Truth witnessed resolution  
**aftermath:** One of two durable public records/monuments and a localized report to a neighboring realm; optional guardian becomes accessible.  
**rewards:** world:echo:TidalLedger  
**source:** World/UnmadeEchoQuestRules.h  

#### The Silence Between Two Notes — `quest:echo:SkyBelow`

**realm:** SkyBelow  
**entry:** Complete the first arc and aftermath, speak with npc.sky.choirmaster.001, investigate two distinct physical clues.  
**choices:** Care witnessed resolution; Truth witnessed resolution  
**aftermath:** One of two durable public records/monuments and a localized report to a neighboring realm; optional guardian becomes accessible.  
**rewards:** world:echo:SkyBelow  
**source:** World/UnmadeEchoQuestRules.h  

#### The Mason's Borrowed Grandmother — `quest:echo:CinderSpine`

**realm:** CinderSpine  
**entry:** Complete the first arc and aftermath, speak with npc.bones.seamwarden.001, investigate two distinct physical clues.  
**choices:** Care witnessed resolution; Truth witnessed resolution  
**aftermath:** One of two durable public records/monuments and a localized report to a neighboring realm; optional guardian becomes accessible.  
**rewards:** world:echo:CinderSpine  
**source:** World/UnmadeEchoQuestRules.h  

#### An Address For Nobody — `quest:echo:HundredUnlived`

**realm:** HundredUnlived  
**entry:** Complete the first arc and aftermath, speak with npc.unlived.censuskeeper.001, investigate two distinct physical clues.  
**choices:** Care witnessed resolution; Truth witnessed resolution  
**aftermath:** One of two durable public records/monuments and a localized report to a neighboring realm; optional guardian becomes accessible.  
**rewards:** world:echo:HundredUnlived  
**source:** World/UnmadeEchoQuestRules.h  

#### The Coronation of the Soil — `quest:echo:OrchardOfKings`

**realm:** OrchardOfKings  
**entry:** Complete the first arc and aftermath, speak with npc.orchard.untitled.001, investigate two distinct physical clues.  
**choices:** Care witnessed resolution; Truth witnessed resolution  
**aftermath:** One of two durable public records/monuments and a localized report to a neighboring realm; optional guardian becomes accessible.  
**rewards:** world:echo:OrchardOfKings  
**source:** World/UnmadeEchoQuestRules.h  

#### The Witness Who Arrived Before You — `quest:echo:FirstAbsence`

**realm:** FirstAbsence  
**entry:** Complete the first arc and aftermath, speak with npc.absence.firstwitness.001, investigate two distinct physical clues.  
**choices:** Care witnessed resolution; Truth witnessed resolution  
**aftermath:** One of two durable public records/monuments and a localized report to a neighboring realm; optional guardian becomes accessible.  
**rewards:** world:echo:FirstAbsence  
**source:** World/UnmadeEchoQuestRules.h  

### FACTION QUEST FAMILY (3)

#### The Refuge Compact — `quest:faction:Refuge`

**realm:** ThreefoldReach  
**entry:** Speak in order with npc.bellwold.lamplighter.001, npc.bellwold.matron.001, npc.bellwold.guard.001 and verify local evidence.  
**choices:** Solidarity; Truth  
**aftermath:** Saved faction reputation: solidarity and published truth differ; either outcome enables real faction memory.  
**rewards:** item:RefugeOathguard  
**source:** World/UnmadeFactionChronicleRules.h  

#### The Redacted Charter — `quest:faction:Archive`

**realm:** ThreefoldReach  
**entry:** Speak in order with npc.paperhaven.scribe.001, npc.paperhaven.registrar.001, npc.paperhaven.archivist.001 and verify local evidence.  
**choices:** Solidarity; Truth  
**aftermath:** Saved faction reputation: solidarity and published truth differ; either outcome enables real faction memory.  
**rewards:** item:ArchivistQuill  
**source:** World/UnmadeFactionChronicleRules.h  

#### The Roadbound Assembly — `quest:faction:Roadbound`

**realm:** ThreefoldReach  
**entry:** Speak in order with npc.guard.001, npc.roadwarden.001, npc.welllistener.001 and verify local evidence.  
**choices:** Solidarity; Truth  
**aftermath:** Saved faction reputation: solidarity and published truth differ; either outcome enables real faction memory.  
**rewards:** item:RoadboundCompass  
**source:** World/UnmadeFactionChronicleRules.h  

### DEPTH QUEST FAMILY (9)

#### The Last Lantern Charter — `quest:depth:ThreefoldReach`

**realm:** ThreefoldReach  
**entry:** Choose which road the surviving lantern charter will serve.  
**choices:** Keep the lantern reserve on the refuge road; Publish the excluded names and reserve the archive room  
**aftermath:** We will keep a bed ready. Jessa still asks why the list is folded shut.; The names are posted. I have moved a lantern indoors; the outer road will need your field kit.  
**rewards:** resource:regional_favor; world:community_accord  
**source:** World/UnmadeAdventureDepthRegistry.h  

#### Water For Those Still Waiting — `quest:depth:WidowedRain`

**realm:** WidowedRain  
**entry:** Decide who controls the recovered pump schedule.  
**choices:** Guarantee a protected family water hour; Keep access public and post every withdrawal  
**aftermath:** Havin keeps the protected hour. The provisioner charges less; travelers wait until the bell.; The withdrawal sheet is public. We spend more time checking it, but newcomers can ask for water.  
**rewards:** resource:regional_favor; world:community_accord  
**source:** World/UnmadeAdventureDepthRegistry.h  

#### The Heat Left Over — `quest:depth:HearthBeneath`

**realm:** HearthBeneath  
**entry:** Decide where the recovered heat should go.  
**choices:** Warm the refuge houses first; Keep a rescue reserve with a public meter  
**aftermath:** The houses are warm. Ilyet checks the reserve before agreeing to send a crew underground.; The meter is where everyone can read it. The houses keep their blankets; the rescue crew keeps its heat.  
**rewards:** resource:regional_favor; world:community_accord  
**source:** World/UnmadeAdventureDepthRegistry.h  

#### Whose Oath Remains — `quest:depth:TidalLedger`

**realm:** TidalLedger  
**entry:** Decide how the surviving debt oath will be held.  
**choices:** Keep the family promise and seal the forced margin; Expose the whole oath and renegotiate it publicly  
**aftermath:** The families still have a guarantor. Deyra keeps the sealed copy so the margin cannot return unnoticed.; The oath is being read aloud. Some people trust us more; others will not wait for the negotiation.  
**rewards:** resource:regional_favor; world:community_accord  
**source:** World/UnmadeAdventureDepthRegistry.h  

#### A Voice On The Bridge — `quest:depth:SkyBelow`

**realm:** SkyBelow  
**entry:** Choose the obligation that the restored span will carry.  
**choices:** Restore the anchor and limit access to its record; Open the crossing record and use manual load watches  
**aftermath:** The anchor carries the weight. Kelm answers for the locked record; the watch still counts every load.; The record is open. Liora has asked for another watcher, because reading it does not hold a bridge up.  
**rewards:** resource:regional_favor; world:community_accord  
**source:** World/UnmadeAdventureDepthRegistry.h  

#### The Name In The Seam — `quest:depth:CinderSpine`

**realm:** CinderSpine  
**entry:** Decide what to do with an ancestor record found in the foundry.  
**choices:** Protect the family claim in a sealed local trust; Publish the neighboring names with the claim  
**aftermath:** The local trust holds the copy. Seyl visits the seam because the other names have not stopped mattering.; The neighboring names are on the wall. Aren has had to defend our claim in front of people it once excluded.  
**rewards:** resource:regional_favor; world:community_accord  
**source:** World/UnmadeAdventureDepthRegistry.h  

#### Names For A Safe House — `quest:depth:HundredUnlived`

**realm:** HundredUnlived  
**entry:** Decide how rescued people will appear in the local census.  
**choices:** Register household totals and seal personal names; Register names openly with an appeal witness  
**aftermath:** The storehouse has the totals. Ovin keeps the name page sealed and has promised to ask before opening it.; The names have a witness beside them. Neris is still making sure nobody mistakes an appeal for permission.  
**rewards:** resource:regional_favor; world:community_accord  
**source:** World/UnmadeAdventureDepthRegistry.h  

#### The Branch They Did Not Graft — `quest:depth:OrchardOfKings`

**realm:** OrchardOfKings  
**entry:** Decide who may harvest the restored root passage.  
**choices:** Keep the household boundary with a public harvest share; Restore the common path and appoint rotating keepers  
**aftermath:** The household boundary stays. Alva leaves a share at the common post, where everyone can see whether it arrived.; The path is common again. Sivra takes the first watch and cannot ask Alva to carry every hungry season alone.  
**rewards:** resource:regional_favor; world:community_accord  
**source:** World/UnmadeAdventureDepthRegistry.h  

#### The Door We Can Describe — `quest:depth:FirstAbsence`

**realm:** FirstAbsence  
**entry:** Decide how to record the reopened threshold.  
**choices:** Keep the working route in a rescue registry; Publish the route with the untested doors marked  
**aftermath:** The rescue registry holds the route. Essel left the untested doors in the drawing instead of pretending they were gone.; The route is public, with its uncertainty intact. Thaelin asks every traveler to read the warning before choosing.  
**rewards:** resource:regional_favor; world:community_accord  
**source:** World/UnmadeAdventureDepthRegistry.h  

### IDENTITY QUEST FAMILY (9)

#### Regional Survey, Repairs and Rest — `quest:identity:ThreefoldReach`

**realm:** ThreefoldReach  
**entry:** Enter dungeon and discover three coupled-valve controls, clues and repair posts.  
**choices:** Solve reversible puzzle and survey local chart; Earn finite hunt ration or repair roads and gain bounded rest  
**aftermath:** Valves and charts persist; repair changes roads and local dialogue, with day-bounded recovery and no infinite weak-foe loot.  
**rewards:** world:regional_survey  
**source:** World/UnmadeIdentityRules.h  

#### Regional Survey, Repairs and Rest — `quest:identity:WidowedRain`

**realm:** WidowedRain  
**entry:** Enter dungeon and discover three coupled-valve controls, clues and repair posts.  
**choices:** Solve reversible puzzle and survey local chart; Earn finite hunt ration or repair roads and gain bounded rest  
**aftermath:** Valves and charts persist; repair changes roads and local dialogue, with day-bounded recovery and no infinite weak-foe loot.  
**rewards:** world:regional_survey  
**source:** World/UnmadeIdentityRules.h  

#### Regional Survey, Repairs and Rest — `quest:identity:HearthBeneath`

**realm:** HearthBeneath  
**entry:** Enter dungeon and discover three coupled-valve controls, clues and repair posts.  
**choices:** Solve reversible puzzle and survey local chart; Earn finite hunt ration or repair roads and gain bounded rest  
**aftermath:** Valves and charts persist; repair changes roads and local dialogue, with day-bounded recovery and no infinite weak-foe loot.  
**rewards:** world:regional_survey  
**source:** World/UnmadeIdentityRules.h  

#### Regional Survey, Repairs and Rest — `quest:identity:TidalLedger`

**realm:** TidalLedger  
**entry:** Enter dungeon and discover three coupled-valve controls, clues and repair posts.  
**choices:** Solve reversible puzzle and survey local chart; Earn finite hunt ration or repair roads and gain bounded rest  
**aftermath:** Valves and charts persist; repair changes roads and local dialogue, with day-bounded recovery and no infinite weak-foe loot.  
**rewards:** world:regional_survey  
**source:** World/UnmadeIdentityRules.h  

#### Regional Survey, Repairs and Rest — `quest:identity:SkyBelow`

**realm:** SkyBelow  
**entry:** Enter dungeon and discover three coupled-valve controls, clues and repair posts.  
**choices:** Solve reversible puzzle and survey local chart; Earn finite hunt ration or repair roads and gain bounded rest  
**aftermath:** Valves and charts persist; repair changes roads and local dialogue, with day-bounded recovery and no infinite weak-foe loot.  
**rewards:** world:regional_survey  
**source:** World/UnmadeIdentityRules.h  

#### Regional Survey, Repairs and Rest — `quest:identity:CinderSpine`

**realm:** CinderSpine  
**entry:** Enter dungeon and discover three coupled-valve controls, clues and repair posts.  
**choices:** Solve reversible puzzle and survey local chart; Earn finite hunt ration or repair roads and gain bounded rest  
**aftermath:** Valves and charts persist; repair changes roads and local dialogue, with day-bounded recovery and no infinite weak-foe loot.  
**rewards:** world:regional_survey  
**source:** World/UnmadeIdentityRules.h  

#### Regional Survey, Repairs and Rest — `quest:identity:HundredUnlived`

**realm:** HundredUnlived  
**entry:** Enter dungeon and discover three coupled-valve controls, clues and repair posts.  
**choices:** Solve reversible puzzle and survey local chart; Earn finite hunt ration or repair roads and gain bounded rest  
**aftermath:** Valves and charts persist; repair changes roads and local dialogue, with day-bounded recovery and no infinite weak-foe loot.  
**rewards:** world:regional_survey  
**source:** World/UnmadeIdentityRules.h  

#### Regional Survey, Repairs and Rest — `quest:identity:OrchardOfKings`

**realm:** OrchardOfKings  
**entry:** Enter dungeon and discover three coupled-valve controls, clues and repair posts.  
**choices:** Solve reversible puzzle and survey local chart; Earn finite hunt ration or repair roads and gain bounded rest  
**aftermath:** Valves and charts persist; repair changes roads and local dialogue, with day-bounded recovery and no infinite weak-foe loot.  
**rewards:** world:regional_survey  
**source:** World/UnmadeIdentityRules.h  

#### Regional Survey, Repairs and Rest — `quest:identity:FirstAbsence`

**realm:** FirstAbsence  
**entry:** Enter dungeon and discover three coupled-valve controls, clues and repair posts.  
**choices:** Solve reversible puzzle and survey local chart; Earn finite hunt ration or repair roads and gain bounded rest  
**aftermath:** Valves and charts persist; repair changes roads and local dialogue, with day-bounded recovery and no infinite weak-foe loot.  
**rewards:** world:regional_survey  
**source:** World/UnmadeIdentityRules.h  

### LOCAL QUEST FAMILY (3)

#### Bellwold Lanterns — `quest:local:BellwoldLanterns`

**realm:** ThreefoldReach  
**entry:** Speak to the lamplighter then refuge matron.  
**choices:** Protect the refuge  
**aftermath:** Once-only Bellwold task and shelter material/gear; enables afterlight return.  
**rewards:** item:BellwoldBulwark  
**source:** World/UnmadeRegionalTaskRules.h  

#### Paperhaven Testimony — `quest:local:PaperhavenTestimony`

**realm:** ThreefoldReach  
**entry:** Speak to the scribe and registrar in Paperhaven.  
**choices:** Preserve true testimony  
**aftermath:** Once-only evidence, ink and heirloom; unlocks later reality mastery.  
**rewards:** item:PaperhavenLens  
**source:** World/UnmadeRegionalTaskRules.h  

#### The Second Night — `quest:local:Afterlight`

**realm:** ThreefoldReach  
**entry:** Finish Bellwold lanterns and Refuge Compact then return to matron, investigate evidence and speak with relevant witness.  
**choices:** Relief; Revelation  
**aftermath:** A permanent Safe Ward and +2 shelter capacity; A permanent Open Census and +2 published witness records  
**rewards:** item:AfterlightWard; item:UnredactedLantern  
**exclusive reward:** true  
**source:** World/UnmadeAfterlightRules.h  

### FRONTIER QUEST FAMILY (2)

#### The Shore Without Its Water — `quest:frontier:WidowedRain`

**realm:** WidowedRain  
**entry:** Visit Saltwake, follow navigator → rain keeper → harbor warden with physical clue.  
**choices:** Care; Truth  
**aftermath:** Durable harbor choice, distinct local response and realm gear milestone.  
**rewards:** item:StormglassCompass  
**source:** World/UnmadeFrontierRealmRules.h  

#### The Heat Lent By No Sun — `quest:frontier:HearthBeneath`

**realm:** HearthBeneath  
**entry:** Visit Cinderhold, follow hearth reader → warmth keeper → ember warden with physical clue.  
**choices:** Care; Truth  
**aftermath:** Durable ember choice, distinct local response and realm gear milestone.  
**rewards:** item:EmberheartBlade  
**source:** World/UnmadeFrontierRealmRules.h  

### STUDY QUEST FAMILY (4)

#### Study: Kiln Ward / Cinderjaw — `quest:study:Cinderjaw`

**realm:** Creature encounter territory  
**entry:** Physically observe Cinderjaw and collect two independent firsthand evidence bits before understanding.  
**choices:** Test the inference twice using valid paid physical trial; Keep the ability unmastered until evidence is convincing  
**aftermath:** Source study state becomes Mastered only after two real experiments, and can combine with another mastered Study. This is a saved personal knowledge outcome, not a claim of civic/world changes yet.  
**rewards:** skill:study:Cinderjaw  
**source:** World/UnmadeAbilityStudyRules.h  

#### Study: Seam Step / Pale Strider — `quest:study:PaleStrider`

**realm:** Creature encounter territory  
**entry:** Physically observe PaleStrider and collect two independent firsthand evidence bits before understanding.  
**choices:** Test the inference twice using valid paid physical trial; Keep the ability unmastered until evidence is convincing  
**aftermath:** Source study state becomes Mastered only after two real experiments, and can combine with another mastered Study. This is a saved personal knowledge outcome, not a claim of civic/world changes yet.  
**rewards:** skill:study:PaleStrider  
**source:** World/UnmadeAbilityStudyRules.h  

#### Study: Redaction Read / Redaction Eye — `quest:study:RedactionEye`

**realm:** Creature encounter territory  
**entry:** Physically observe RedactionEye and collect two independent firsthand evidence bits before understanding.  
**choices:** Test the inference twice using valid paid physical trial; Keep the ability unmastered until evidence is convincing  
**aftermath:** Source study state becomes Mastered only after two real experiments, and can combine with another mastered Study. This is a saved personal knowledge outcome, not a claim of civic/world changes yet.  
**rewards:** skill:study:RedactionEye  
**source:** World/UnmadeAbilityStudyRules.h  

#### Study: Memory Lens / Echo Well — `quest:study:EchoWell`

**realm:** ThreefoldReach  
**entry:** Physically observe EchoWell and collect two independent firsthand evidence bits before understanding.  
**choices:** Test the inference twice using valid paid physical trial; Keep the ability unmastered until evidence is convincing  
**aftermath:** Source study state becomes Mastered only after two real experiments, and can combine with another mastered Study. This is a saved personal knowledge outcome, not a claim of civic/world changes yet.  
**rewards:** skill:study:EchoWell  
**source:** World/UnmadeAbilityStudyRules.h  


## Learned skills tied to authentic quest evidence

### Marked Opening — `skill:technique:ThreefoldReach`

**realm:** ThreefoldReach  
**learning quest:** quest:expedition:ThreefoldReach  
**teacher or source:** Earn the region's permanent weapon or armor; the associated technique can then be selected.  
**source:** World/UnmadeExpeditionRegistry.h  

### Pressure Read — `skill:technique:WidowedRain`

**realm:** WidowedRain  
**learning quest:** quest:expedition:WidowedRain  
**teacher or source:** Earn the region's permanent weapon or armor; the associated technique can then be selected.  
**source:** World/UnmadeExpeditionRegistry.h  

### Ember Counter — `skill:technique:HearthBeneath`

**realm:** HearthBeneath  
**learning quest:** quest:expedition:HearthBeneath  
**teacher or source:** Earn the region's permanent weapon or armor; the associated technique can then be selected.  
**source:** World/UnmadeExpeditionRegistry.h  

### Oath Interrupt — `skill:technique:TidalLedger`

**realm:** TidalLedger  
**learning quest:** quest:expedition:TidalLedger  
**teacher or source:** Earn the region's permanent weapon or armor; the associated technique can then be selected.  
**source:** World/UnmadeExpeditionRegistry.h  

### Grounded Fold — `skill:technique:SkyBelow`

**realm:** SkyBelow  
**learning quest:** quest:expedition:SkyBelow  
**teacher or source:** Earn the region's permanent weapon or armor; the associated technique can then be selected.  
**source:** World/UnmadeExpeditionRegistry.h  

### Seam Read — `skill:technique:CinderSpine`

**realm:** CinderSpine  
**learning quest:** quest:expedition:CinderSpine  
**teacher or source:** Earn the region's permanent weapon or armor; the associated technique can then be selected.  
**source:** World/UnmadeExpeditionRegistry.h  

### Unlisted Step — `skill:technique:HundredUnlived`

**realm:** HundredUnlived  
**learning quest:** quest:expedition:HundredUnlived  
**teacher or source:** Earn the region's permanent weapon or armor; the associated technique can then be selected.  
**source:** World/UnmadeExpeditionRegistry.h  

### Root Counter — `skill:technique:OrchardOfKings`

**realm:** OrchardOfKings  
**learning quest:** quest:expedition:OrchardOfKings  
**teacher or source:** Earn the region's permanent weapon or armor; the associated technique can then be selected.  
**source:** World/UnmadeExpeditionRegistry.h  

### Nearness Hold — `skill:technique:FirstAbsence`

**realm:** FirstAbsence  
**learning quest:** quest:expedition:FirstAbsence  
**teacher or source:** Earn the region's permanent weapon or armor; the associated technique can then be selected.  
**source:** World/UnmadeExpeditionRegistry.h  

### The Law Beneath the Last Bell — `skill:rite:UnwriteLaw`

**realm:** Any at physical site Bellgrave  
**learning quest:** quest:rite:UnwriteLaw  
**teacher or source:** npc.bellmaker.001  
**source:** World/UnmadeTenfoldChronicle.h  

### The Bridge of Living Testimony — `skill:rite:Witnesscraft`

**realm:** Any at physical site EchoWell  
**learning quest:** quest:rite:Witnesscraft  
**teacher or source:** npc.welllistener.001  
**source:** World/UnmadeTenfoldChronicle.h  

### The Life Not Taken — `skill:rite:BorrowedLives`

**realm:** Any at physical site PaperOrchard  
**learning quest:** quest:rite:BorrowedLives  
**teacher or source:** npc.orchardexile.001  
**source:** World/UnmadeTenfoldChronicle.h  

### A Blade That Was There — `skill:rite:LegacyForging`

**realm:** Any at physical site BellwoldForge  
**learning quest:** quest:rite:LegacyForging  
**teacher or source:** npc.bellwold.stonewright.001  
**source:** World/UnmadeTenfoldChronicle.h  

### No Road Belongs to One — `skill:rite:LivingRoads`

**realm:** Any at physical site SilentMile  
**learning quest:** quest:rite:LivingRoads  
**teacher or source:** npc.roadwarden.001  
**source:** World/UnmadeTenfoldChronicle.h  

### The Ledger of Borrowed Dawn — `skill:rite:TomorrowDebt`

**realm:** Any at physical site DebtMarket  
**learning quest:** quest:rite:TomorrowDebt  
**teacher or source:** npc.tollbroker.001  
**source:** World/UnmadeTenfoldChronicle.h  

### The Name Beneath the Monster — `skill:rite:UnderstandingBosses`

**realm:** Any at physical site BellOssuary  
**learning quest:** quest:rite:UnderstandingBosses  
**teacher or source:** npc.bellwold.matron.001  
**source:** World/UnmadeTenfoldChronicle.h  

### The Tower That Fell and Stood — `skill:rite:ParadoxConvergence`

**realm:** Any at physical site PaperhavenScribe  
**learning quest:** quest:rite:ParadoxConvergence  
**teacher or source:** npc.paperhaven.mapmaker.001  
**source:** World/UnmadeTenfoldChronicle.h  

### The Lantern You Promised — `skill:rite:Oathbinding`

**realm:** Any at physical site BellwoldRefuge  
**learning quest:** quest:rite:Oathbinding  
**teacher or source:** npc.bellwold.guard.001  
**source:** World/UnmadeTenfoldChronicle.h  

### The Map of Disagreeing Shores — `skill:rite:Cartography`

**realm:** Any at physical site SaltwakeHarbor  
**learning quest:** quest:rite:Cartography  
**teacher or source:** npc.saltwake.navigator.001  
**source:** World/UnmadeTenfoldChronicle.h  

### Kiln Ward / Cinderjaw — `skill:study:Cinderjaw`

**realm:** Encounter territory  
**learning quest:** quest:study:Cinderjaw  
**teacher or source:** Two independent direct observations of a living Cinderjaw encounter  
**source:** World/UnmadeAbilityStudyRules.h  

### Seam Step / Pale Strider — `skill:study:PaleStrider`

**realm:** Encounter territory  
**learning quest:** quest:study:PaleStrider  
**teacher or source:** Two independent direct observations of a living PaleStrider encounter  
**source:** World/UnmadeAbilityStudyRules.h  

### Redaction Read / Redaction Eye — `skill:study:RedactionEye`

**realm:** Encounter territory  
**learning quest:** quest:study:RedactionEye  
**teacher or source:** Two independent direct observations of a living RedactionEye encounter  
**source:** World/UnmadeAbilityStudyRules.h  

### Memory Lens / Echo Well — `skill:study:EchoWell`

**realm:** ThreefoldReach  
**learning quest:** quest:study:EchoWell  
**teacher or source:** Two independent signals at the physical Echo Well  
**source:** World/UnmadeAbilityStudyRules.h  


## Finale — Nhal-Vey and both persistent worlds

- **First encounter:** requires legitimately resolved Auvren first chapter, not merely arriving at the arena. The first defeat breaks Nhal-Vey's veil and exposes its next stage.
- **True victory:** the second fight is not the end-choice; the player deliberately chooses a changed morning with an explicit persistence boundary.
- **The Held Morning:** a world with stable visible seams keeping obligations and promises. Earlier choices, people, resources and quests remain.
- **The Many Mornings:** contradictory public histories coexist, while firsthand memories and actual decisions remain intact.
- **Optional Echo:** rematch is accessible only after a chosen world; it is never a main ending requirement and does not rewrite the original choice.

**References:** [campaign spine](../../../Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h), [final encounter rules](../../../Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h), [canonical dependency ledger](../dependency-ledger.json), [physical quest contracts](../../../Authoring/main_quest_interaction_contracts.json).
