# Volume I — The Nine Realms, Cultures, Places and Everyday Life

**Canon class:** The local world atlas is an authored world and exploration reference. Volume X adds editorial civilization and city details. This does not mean all places are rendered, navigable Unreal levels. **Source authorities:** `Authoring/lived_universe_atlas.json`, `Authoring/world_content_pack.json`, `Authoring/transmedia_storyworld_v1.json`, and `UnmadeWorldAtlas.h`.

## The Threefold Reach — `ThreefoldReach`

**settlements:** The Crossings; Bellwold; Paperhaven  
**ordinary:** Refuge kettles are counted before wages; Bellwold lantern tenders walk the same lane that Paperhaven record runners insist never existed.  
**architecture:** A three-way road knot of mismatched mile markers, split-roof shelters, chalk archive vaults and a bell tower missing its casting seam.  
**custom:** A visitor is offered a place to sit before a request for a name; recording a stranger's name without consent is considered a quiet theft.  
**taboo:** Never ring a bell during the interval reserved for those no ledger can verify.  
**governance:** Shelter stewards negotiate resources with archive registrars; neither holds authority over the other's witnesses.  
**labor:** Lantern wick trimming, route repair, water carting, archive copying and informal shelter kitchens.  
**trade:** Measured lantern oil, copied route notes and shelter meal tokens; no reward for registering a displaced person's private name.  
**ecology:** Pale waymoss grows toward walked paths, not sunlight; little paper-wings nest where contradictory maps overlap.  
**weather:** The day's shadow agrees with the road until the missing bell note; during the interval only the mile markers disagree.  
**culture:** The Wayward Charter  
**public principle:** A witnessed promise outranks an inherited seal.  
**ritual:** At first light two neighbors reread a road's name aloud, including those denied the original charter.  
**sound direction:** Struck copper bells, wagon-wheel rests and overlapping spoken distances  
**visual direction:** Mist gold and ink blue; stable landmarks before every camera turn  
**optional activity:** Repair a communal mile marker using testimony, never repeated commodity grinding  
**central dispute:** Shelter requires trust; records demand difficult public consent  

### Sensory and accessibility continuity

**day:** Wet paper and kettle steam; boot soles on mismatched paving; warm pockets of lantern gold.  
**night:** Shelter fires persist; a bell's absent final note lowers the ambience without producing sudden full silence.  
**hazard cue:** Route splits acquire high-contrast chalk edging before the safe way changes.  
**access:** Persistent map and text clue; bell silence is also indicated by visible wick tremor and caption.  

### Mystery, physical evidence and competing readings

**question:** How can a refuge be remembered and officially never built?  
**visible:** A shelter hearth has more soot than the building's documented age.  
**false lead:** The archivist burned a page; investigation shows an old fire unrelated to the missing date.  
**interpretation:** The interval was socially necessary before it was made official; neither ledger nor testimony can erase the people sheltered.  
**link:** A rain-soaked meal chit carries an imprint of Saltwake's older harbor tariff.  
**hint:** The child's chalk crosses the road only when the bell stops.  
**do not reveal:** Do not mention Nhal-Vey or a final boss by name.  
- **`LUE.ThreefoldReach.01` — Bellwold uncast-bell mold:** Compare the mold rim with the tower's missing casting seam. Metal never poured; indentations show hands holding a bell-shaped void.
- **`LUE.ThreefoldReach.02` — Paperhaven cross-index desk:** Align a refugee meal chit with a registry's blank date. The receipt predates the legal approval yet bears the same worn thumbmark.
- **`LUE.ThreefoldReach.03` — Crossings mile marker:** Follow a child's chalk route against a surveyor's stone mile. Both lead to the same kettle house from opposing directions.

### Ordinary residents' routines, consent and recoverable failure

#### Bellwold refuge kitchen — `LUR.ThreefoldReach.01`

**window:** dawn  
**trigger:** player enters kitchen after first arrival  
**action:** Carry a warm kettle over a damaged threshold or ask a lantern keeper for a stable route  
**reaction:** The keeper first secures a seat for a nameless visitor before discussing the cracked threshold  
**line:** There is always one cup left uncounted. It is not an accounting error.  
**feedback:** Steam reveals a footworn path and reveals a safe crossing  
**failure:** Dropping the kettle extinguishes one local lantern but does not lose the evidence  
**recovery:** Refill at the kettle stand; route marker remains visible  
**persistence:** Only one personal witnessed-aid event if authentic resident interaction fires; otherwise ambience only  

#### Paperhaven witness steps — `LUR.ThreefoldReach.02`

**window:** dusk  
**trigger:** the player has inspected at least one bell discrepancy  
**action:** Choose to read a public redaction or request a protected spoken account  
**reaction:** Registrar redirects curious listeners away from a protected refugee while offering the public index  
**line:** Truth without a door can still be a kind of hunting.  
**feedback:** One of two nonexclusive clue formats is accessible; no main quest decision changes  
**failure:** If consent is declined the private name is not exposed  
**recovery:** Read the public version at any future daypart  
**persistence:** Optional discovery note only; no XP farm  

### Optional local story: The Uncounted Wick — `LUQ.ThreefoldReach.WickDebt`

**unlock:** after opening Reach investigation; optional  
**objective:** Restore a lantern line without turning its residents into registered collateral  
**steps:** Inspect a dark shelter marker and match two lengths of used wick; Speak to the keeper privately; learn that the spare oil belongs to unregistered travelers; Carry oil to the public crossing without crossing the unsafe invisible footbridge; Choose an anonymous supply tally or a named but consent-based open ledger; Return next day to see either a communal wick shelf or a consent station  
**branches:** {"care":"Anonymous lantern shelf stays accessible but cannot be used as proof of residency.","truth":"Consent-based register helps supplies arrive but has clearly marked refusal rights."}  
**failure:** Wrong wick makes lantern dim, not cause instant death or fake story completion.  
**recovery:** Trim and retry with both samples; no consumable loss beyond a reversible local wick.  
**return dialogue:** You repaired a light. You did not ask who deserved to stand under it.  
**save:** New optional side-story authored contract only; requires a future isolated LUQ slot, no runtime flag yet  

### Production-only staging requirements

**0:** SM_Reach_UncastMold  
**1:** SM_Reach_MealChit  
**2:** SFX_Reach_AbsentBell  
**3:** SEQ_Reach_WickDelivery  

### Saved world aftermath and return

**held:** Soot and legal date remain simultaneously legible on a single restored shelter wall.  
**many:** Two shelter thresholds overlap but both lead to the same kettle, with optional stable-view setting.  

### Registered cities and civic power (3)

#### The Crossings — `CITY.01`

**recognition:** three roads meet under one missing mile  
**skyline:** Timber footbridges tied with three competing knots; street tiles from three abandoned maps  
**sound:** A humming roadstone counterpoint to market kettles  
**social rule:** Wards refuse to ask a traveler for a name until they have eaten  
**civic power:** Bridge Ledger Court  
**personal stakes:** Orrel marks crossings with an inkless nail; Kesta can deliver testimony but cannot decide which road is right  
**anomaly:** The third road runs through an address erased from Paperhaven's publicly dated surveys  
**reveal state:** open  
**visual icon:** three mismatched road knots  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

#### Bellwold — `CITY.02`

**recognition:** the bell tower whose bell was never cast  
**skyline:** Copper-green tower with an empty suspended yoke; refuge lanterns reflected in washed gray stone  
**sound:** A missing last bell note followed by the continued noise of people eating  
**social rule:** An uncounted cup is set out even on nights when there are fewer chairs than guests  
**civic power:** The Refuge Ward  
**personal stakes:** Hessa weighs survival against exposure of the erased census; Sorin and Ivera disagree without being enemies  
**anomaly:** Rook lights a lantern on a doorstep that only some returning travelers can locate  
**reveal state:** open  
**visual icon:** empty bronze yoke with one lit refuge cup  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

#### Paperhaven — `CITY.03`

**recognition:** the city where evidence is physically heavier than paper  
**skyline:** Chalk vaults, narrow roof-high archives and the dry garden that never grew in rain  
**sound:** A scratch of quills that never aligns with the tower clock  
**social rule:** Residents can correct a public record but cannot publish another person's private name without their consent  
**civic power:** Index Hall  
**personal stakes:** Sevrin's authority depends on the library surviving; Toma notices crossed-out lines survive copying  
**anomaly:** An index entry cites a volume printed before its press existed  
**reveal state:** open  
**visual icon:** two dry paper leaves under a weight stone  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

**Source provenance:** [Living Universe](../../../Authoring/lived_universe_atlas.json) · [World Content](../../../Authoring/world_content_pack.json) · [Literary Cities](../../../Authoring/transmedia_storyworld_v1.json)

---

## The Widowed Rain — `WidowedRain`

**settlements:** Saltwake  
**ordinary:** Dry-dock families mend nets for a sea absent from maps; public rain catchers are checked before any ferry launches.  
**architecture:** Hull-roof shelters, tide gauges anchored in dry stone, sloped breakwaters and an open storm sluice court.  
**custom:** People greet strangers by offering a dry knot of rope rather than a handshake; a knot accepted means safe shelter, not ownership.  
**taboo:** No one claims to own another family's collected rain by reading a debt aloud in a storm.  
**governance:** Harbor wardens hold flood authority; navigators challenge them with physical measurements and common rain counters.  
**labor:** Net darning, empty buoy inspection, slate-chart repair, storm-shelter watch and ferry maintenance.  
**trade:** Rain allotments and ferry repair credits; storm insurance cannot be collected twice on the same missing tide.  
**ecology:** Salt fern clings to dry piers; tidecrabs hide below no-water marks and emerge with loud pressure shifts.  
**weather:** Rain has weight and rhythm without a reachable sea; the storm pulse telegraphs on marked lanes with a safe elevated bypass.  
**culture:** The Rainward Account  
**public principle:** Water belongs to its keepers only while they can prove it was shared.  
**ritual:** A departing sailor leaves a dry cup on a sill for a sea not yet returned.  
**sound direction:** Rain counted on glass, wet rope under wind, a bell with no answering buoy  
**visual direction:** Desaturated jade and pewter; reachable high ground and rainfall visibility options  
**optional activity:** Compare a chart with a first-hand shoreline witness  
**central dispute:** A broker sells remembered water to families who need actual rain  

### Sensory and accessibility continuity

**day:** Blue rust, wet rope, slate ticking beneath a missing waterline.  
**night:** Empty buoy chime has no splash; rain lamp halos stretch toward impossible offshore distance.  
**hazard cue:** A low rail vibrates, sluice counter flashes blue and the storm lane glows before impact.  
**access:** Visual counter, captioned pressure click and high-contrast elevated safe line, never audio-only.  

### Mystery, physical evidence and competing readings

**question:** Who wrote a tariff for a harbor that disappeared before the tariff's issuer was born?  
**visible:** A buoy ledger records departures from dry ground.  
**false lead:** A missing boat was stolen; the inspected sail seams reveal it never touched this shore.  
**interpretation:** A vanished channel was made useful as transferable future debt; residents still must decide fair access to real rain.  
**link:** The negative arrival is charged to a child's future birth in Drevlach.  
**hint:** Read the public counter in the lull, not while running from the warning.  
**do not reveal:** Do not frame all debtors as villains or reveal final enemy.  
- **`LUE.WidowedRain.01` — Dry Pier 3 mooring post:** Touch three tide grooves and mark which groove is newer than the post. The newest groove lies beneath a mark carved centuries earlier.
- **`LUE.WidowedRain.02` — Saltwake public rain counter:** Read numbers only after the storm warning has settled. The last tally includes a negative arrival not charged to any fisher.
- **`LUE.WidowedRain.03` — Ferry repair bench:** Fit an older rope knot to a measured missing-buoy hole. A net repair tag from Drevlach lists the same impossible date.

### Ordinary residents' routines, consent and recoverable failure

#### Saltwake rope square — `LUR.WidowedRain.01`

**window:** work  
**trigger:** player visits before completing rainward first intervention  
**action:** Help coil a rope before the surge or use the marked elevated bypass  
**reaction:** Navigator acknowledges the chosen safe method; bystanders flee only when they hear their own storm warning  
**line:** Knots hold where charts do not. Keep your hand on the dry part.  
**feedback:** Rope line leads to accessible clue at the mooring post  
**failure:** Storm hit injures only in source-defined danger lane  
**recovery:** Retreat to signed shelter; source checkpoint behavior remains authority  
**persistence:** Ordinary local help has a single id and cannot resolve a story by itself  

#### Empty ferry quay — `LUR.WidowedRain.02`

**window:** night  
**trigger:** first local story resolved  
**action:** Inspect an empty boarding call and compare the ledger to a returned ferryman's testimony  
**reaction:** Ferry worker respects the public outcome but remembers personally observed aid separately  
**line:** We keep a berth for those who never made land.  
**feedback:** A night-only reflection reveals submerged letters on dry boards  
**failure:** Player arrives before the night shift; clue is safely repeatable  
**recovery:** Return at night or ask worker for the written schedule  
**persistence:** Optional codex addition; never a required boss gate  

### Optional local story: The Berth Without Water — `LUQ.WidowedRain.LastBerth`

**unlock:** Saltwake first-story completion; optional  
**objective:** Maintain a public ferry stop even if two witnesses disagree about the tide  
**steps:** Inspect dry berth number and a current shelter passenger tally; Interview a navigator and a rain collector without presenting either as omniscient; Carry a replacement plank along the storm-safe route; Repair a handrail using an actual dock mechanism; Choose a shared public waiting rail or a recorded tide archive, then revisit at night  
**branches:** {"care":"Shared handrail serves stranded travelers without ownership tests.","truth":"Tide archive preserves contradiction for navigators while keeping a sheltered entry."}  
**failure:** Missed warning interrupts the plank carry; it remains recoverable at the safe shelter.  
**recovery:** Restart the short carry after the current storm pulse; never replay interviews for rewards.  
**return dialogue:** The berth was empty. That did not mean nobody needed somewhere to wait.  
**save:** New optional authored side arc; no Unreal stage flag exists  

### Production-only staging requirements

**0:** SM_Rain_TideGrooves  
**1:** SM_Rain_RopeKnot  
**2:** NS_Rain_SurgeCounter  
**3:** SEQ_Rain_LastBerth  

### Saved world aftermath and return

**held:** The honest last waterline is engraved around accessible dry piers.  
**many:** Two tidal silhouettes can be read together with a stable single-outline option.  

### Registered cities and civic power (1)

#### Saltwake — `CITY.04`

**recognition:** the harbor which has weather but no sea  
**skyline:** Pier skeletons standing above slate flats; inverted keel roofs and immaculate tide grooves  
**sound:** Rain on hulls with no answering water slap  
**social rule:** Leave a rope knot with dry hands before requesting refuge  
**civic power:** Mooring Council  
**personal stakes:** Sella maps what others call impossible; Harrow protects lives on the dry docks but signed old ownership papers  
**anomaly:** One buoy rings before the storm instead of after, even when its rope is cut  
**reveal state:** open  
**visual icon:** dry cup inside a tide circle  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

**Source provenance:** [Living Universe](../../../Authoring/lived_universe_atlas.json) · [World Content](../../../Authoring/world_content_pack.json) · [Literary Cities](../../../Authoring/transmedia_storyworld_v1.json)

---

## The Hearth Beneath — `HearthBeneath`

**settlements:** Cinderhold  
**ordinary:** Households share a public heat allowance without agreeing who originally lit the flame; the heat vent is inspected before breakfast.  
**architecture:** Sunken communal kilns, basalt house ribs, ash sieves and a vent court whose shadows disagree with its fire.  
**custom:** Offering a cooled coal signals willingness to listen rather than consent to repay a family obligation.  
**taboo:** Never attribute another person's warmth to inherited debt without hearing that household.  
**governance:** Kiln guild weighs public safety; hearth readers hold independent records of each fuel claim.  
**labor:** Clay tempering, vent grating, stone-lid casting, communal bread baking and ash sorting.  
**trade:** Cooled ceramic, workshop hours and public vent shares; heat claims are witnesses, not currency for buying people.  
**ecology:** Glass moths fold their wings near unsafe heat; ash root threads retain warmth long after fire goes dark.  
**weather:** Borrowed sun glow rises from beneath streets; source heat pulses give a legible orange warning and signed refuge route.  
**culture:** The Ember Commons  
**public principle:** A fire is a promise to the next household, not hereditary property.  
**ritual:** Each street relights an empty hearth before feeding the communal flame.  
**sound direction:** Low stone resonance, breath, hammers and furnace air  
**visual direction:** Ember ochre, moss-black stone; non-flicker alternatives to fire flashes  
**optional activity:** Tend a heat channel and decide who receives warmth during shortages  
**central dispute:** Protecting an ember can become a justification for excluding outsiders  

### Sensory and accessibility continuity

**day:** Basalt dust, fermented dough, slow metal ping from vent housings.  
**night:** Coals cast a second wrong-way shadow across floor tiles.  
**hazard cue:** Vent shutters chatter and floor seams brighten before danger.  
**access:** Shutter silhouettes and text hazard timer accompany sound; no photo-flash requirement.  

### Mystery, physical evidence and competing readings

**question:** Why does a hearth cast warmth from an hour nobody lived?  
**visible:** An ash register shows the kiln burning before any fuel receipt.  
**false lead:** A guild master deliberately lit all fires; the mark predates the guild and is acknowledged by its critics.  
**interpretation:** Survival is being subsidized by an unasked temporal obligation that ordinary citizens did not individually choose.  
**link:** The impossible pause in flame is mirrored by a missing sustaining note in Orravane.  
**hint:** Wait for the ash pan's cooled edge; the safe clue outlasts the pulse.  
**do not reveal:** No pre-Auvren speech may confirm Nhal-Vey's creation.  
- **`LUE.HearthBeneath.01` — Old vent winding wheel:** Compare wear on two teeth without engaging hazardous mechanism. The wheel has been wound one turn before its manufacture stamp.
- **`LUE.HearthBeneath.02` — Shared bread oven:** Lift a cooled ash pan after its safety indicator. Unburned paper inside describes a childhood that has not yet occurred.
- **`LUE.HearthBeneath.03` — Tarin's public heat slate:** Align shadow scratches with a real lantern. A missing beat matches the choir pauses of Orravane.

### Ordinary residents' routines, consent and recoverable failure

#### Public hearth pantry — `LUR.HearthBeneath.01`

**window:** dawn  
**trigger:** player enters after reaching Cinderhold  
**action:** Carry a clay loaf tray between counters rather than through live vent corridor  
**reaction:** A baker deliberately feeds a family that cannot prove inherited heat rights  
**line:** There is always bread for the one who didn't sign the furnace.  
**feedback:** Second shadow marks a clue in soot  
**failure:** Tray cools if delayed; no permanent penalty  
**recovery:** Return to the oven, collect fresh warm tray, and use obvious sheltered route  
**persistence:** Optional ambient scene; never converts into debt  

#### Vent court listening bench — `LUR.HearthBeneath.02`

**window:** dusk  
**trigger:** Cinderhold first act resolved  
**action:** Listen to two opposed household accounts before selecting public record format  
**reaction:** Hearth reader keeps disagreement visible but does not announce a single winner  
**line:** We can owe the flame care without owing it ourselves.  
**feedback:** Public display changes commentary only, does not overwrite earlier choice  
**failure:** If a household refuses a name, only anonymized account is readable  
**recovery:** Use the anonymized paper anytime  
**persistence:** One codex variant per choice; no local story reset  

### Optional local story: The Second Shadow — `LUQ.HearthBeneath.SecondShadow`

**unlock:** Cinderhold first story resolved; optional  
**objective:** Protect a communal oven without endorsing an invented inherited fuel debt  
**steps:** Inspect a cold ash pan and compare two furnace shadow marks; Ask a bread maker and hearth reader what they independently know; Retrieve vent grating from the safe stone yard; Install grate using the real heat-control interaction after warning ends; Choose a public fuel audit or a household warmth reserve; observe evening shadow  
**branches:** {"care":"Household reserve guarantees safe bread without requiring genealogy.","truth":"Public fuel audit preserves contradictory records and anonymous family testimony."}  
**failure:** Installing during a heat warning is refused, not a forced damaging cutscene.  
**recovery:** Wait at the sheltered bench for cooldown and try again.  
**return dialogue:** The second shadow is still there. It no longer decides whose bread gets baked.  
**save:** Production-only optional arc; native main story unaffected  

### Production-only staging requirements

**0:** SM_Hearth_CooledCoal  
**1:** SM_Hearth_AshPan  
**2:** NS_Hearth_VentTeeth  
**3:** SEQ_Hearth_SecondShadow  

### Saved world aftermath and return

**held:** Two cooling marks remain on one public oven, each labelled with a remembered obligation.  
**many:** Paired shadows share the kiln floor; static-light toggle preserves readable traversal.  

### Registered cities and civic power (1)

#### Cinderhold — `CITY.05`

**recognition:** the city whose sun rises beneath its foundations  
**skyline:** Basalt ventilator ribs, public furnace court, deep angular steps and doubled chimney shadows  
**sound:** Metal vent shutters and loaves cracking over too-even heat  
**social rule:** The first cooled coal is given, not sold  
**civic power:** The Ember Commons  
**personal stakes:** Tarin knows the fire's missing beat; Rheva maintains public safety; Ovenna sees the cost borne by vulnerable households  
**anomaly:** One oven records a child's birthday in ash before that child's birth  
**reveal state:** open  
**visual icon:** coal with two opposing shadows  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

**Source provenance:** [Living Universe](../../../Authoring/lived_universe_atlas.json) · [World Content](../../../Authoring/world_content_pack.json) · [Literary Cities](../../../Authoring/transmedia_storyworld_v1.json)

---

## The Sea of Written Debts — `TidalLedger`

**settlements:** Drevlach  
**ordinary:** Children learn to count tide-ledger stamps before they learn genealogies; grown debt carriers argue whether future entries are valid.  
**architecture:** Ledger canals, inkstone lockhouses, portable counting bridges and thresholds etched with dates not yet reached.  
**custom:** A contract offered to a traveler must be read both forwards and backwards; silence is not acceptance.  
**taboo:** Do not assign a future child's signature to an adult's current hardship.  
**governance:** Counter-clerks control public transit slips; debt dissidents keep receipts in an unlicensed dry archive.  
**labor:** Ink harvesting, sluice scribing, canal tow, account witnesses and slip repair.  
**trade:** Repair vouchers and witnessed labor credits; speculative birth collateral is an institutional abuse, not a valid player currency.  
**ecology:** Date-eels gather by inkstone sluices one moment before rain; ink reeds bloom with letters running the wrong direction.  
**weather:** Water follows recorded debits in some canals; nearby tide surge has readable boundaries and a safe bank.  
**culture:** The Estuary of Owed Days  
**public principle:** Nobody may pledge the tomorrow of a person who cannot consent.  
**ritual:** A lender writes a due date, then washes the book and preserves the erased impression.  
**sound direction:** Distant oar strokes without water, scratch of debt stylus, delayed bell  
**visual direction:** Black teal with silver date-stamps; rising-tide warning indicators  
**optional activity:** Audit a ferry manifest by meeting a borrower and examining a physical seal  
**central dispute:** Relief now may hide the architecture of generational exploitation  

### Sensory and accessibility continuity

**day:** Ink brine, paper scraping over stone, regular tapping from canal clocks.  
**night:** Ledger lamps shine through reversed dates on translucent water.  
**hazard cue:** Debt tide pulls paper flags against their stakes before collision pressure.  
**access:** Tactile marker relief and high-contrast arrows identify safe banks; written equivalents to number sounds.  

### Mystery, physical evidence and competing readings

**question:** Who can own a debt dated before its debtor exists?  
**visible:** A child's future signature appears in a ledger sealed while the child was unborn.  
**false lead:** The child is secretly an immortal debtor; no evidence supports this, and the story must not confirm it.  
**interpretation:** Contracts became infrastructure; an accounting system exploited questions no one dared demand answered.  
**link:** An erased family reference in the dry archive points toward Vathless stone deeds.  
**hint:** The two stamps must be read in opposite directions.  
**do not reveal:** The final entity is not the bank manager and debt exploitation remains a civic choice.  
- **`LUE.TidalLedger.01` — Public twin receipt press:** Make one non-binding rubbing of reversed embossed entries. Debit appears before the credit on both impressions.
- **`LUE.TidalLedger.02` — Dry archive towline box:** Place a Saltwake repair tag beside a birth-date tab. The missing sea's tariff was securitized as someone's unwritten years.
- **`LUE.TidalLedger.03` — Clerks' broken canal gate:** Watch water change direction at an unoccupied account post. The canal still serves a debt whose witness cannot be found.

### Ordinary residents' routines, consent and recoverable failure

#### Two-slip market — `LUR.TidalLedger.01`

**window:** work  
**trigger:** player first arrives at Drevlach  
**action:** Witness a purchase without signing and compare a forged balance slip to a merchant's spoken price  
**reaction:** Merchant thanks the player for insisting on reversible evidence before any accusation  
**line:** There is a difference between balancing a book and balancing a life.  
**feedback:** Slip shows an impossible offset only if physically held against light  
**failure:** Wrong slip is returned without a fine  
**recovery:** Ask for duplicate public copy and inspect again  
**persistence:** Discovery only; non-farmable  

#### Canal debt bridge — `LUR.TidalLedger.02`

**window:** night  
**trigger:** first story resolved  
**action:** Escort an older counter-clerk by a safe bank as dates glow in the canal  
**reaction:** The clerk can admit personal guilt without erasing neighbors' survival needs  
**line:** I filed the number. I did not make the child.  
**feedback:** Public plaque acknowledges local committed outcome  
**failure:** Tide warning blocks exposed route  
**recovery:** Use signed upper bank, no escort permadeath  
**persistence:** Single optional witnessed meeting; no currency duplication  

### Optional local story: A Receipt for an Unborn Hour — `LUQ.TidalLedger.UnbornReceipt`

**unlock:** Drevlach first chapter resolved; optional  
**objective:** Stop a duplicate future debt without deleting the records that prove abuse  
**steps:** Rubb two public receipt faces using the fixed press; Compare a dry archive tag with a witness clerk's original; Cross the safe upper canal path during the tide interval; Present discrepancy at a functioning nonviolent public account desk; Choose a protected redaction plus restitution route or a public audit with consent safeguards  
**branches:** {"care":"Protected household claim is frozen; a reachable restitution desk remains.","truth":"Audit plaque names the institutional failure, not an unconsenting child."}  
**failure:** Submitting one stamp is refused with an explicit missing-evidence message.  
**recovery:** The press and duplicate copies remain available.  
**return dialogue:** The canal still runs. The child no longer has to be its missing signature.  
**save:** New optional LUQ contract; not a source bank-transaction modification  

### Production-only staging requirements

**0:** SM_Ledger_ReceiptPress  
**1:** SM_Ledger_DryTowline  
**2:** SFX_Ledger_ReverseClock  
**3:** SEQ_Ledger_UnbornHour  

### Saved world aftermath and return

**held:** Contradictory stamps preserved as public scar in one legal record.  
**many:** Two valid receipt directions become publicly visible; consent archive remains protected.  

### Registered cities and civic power (1)

#### Drevlach — `CITY.06`

**recognition:** the debt city where tomorrow is stamped before dawn  
**skyline:** Black-teal paper docks, dry counting bridges and silver inverted-date clocks  
**sound:** Ink nibs striking account stone as absent oars move  
**social rule:** Read every contract in both directions; silence cannot sign  
**civic power:** Ninth Due Exchange  
**personal stakes:** Veynu keeps contradictory books to protect those named in them; Orshel ferries evidence rather than people  
**anomaly:** A negative deposit is attributed to the same unregistered witness in two centuries  
**reveal state:** open  
**visual icon:** upside-down date stamp  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

**Source provenance:** [Living Universe](../../../Authoring/lived_universe_atlas.json) · [World Content](../../../Authoring/world_content_pack.json) · [Literary Cities](../../../Authoring/transmedia_storyworld_v1.json)

---

## The Upside-Down Choir — `SkyBelow`

**settlements:** Orravane  
**ordinary:** Suspended houses are serviced by singers who must rest; nobody agrees whether the missing choir note is a worker or a building support.  
**architecture:** Inverted terraces, hanging floors, counterweight choirs and bridges pinned by acoustic chambers.  
**custom:** Visitors tap a support rail before speaking so residents can tell a body from an echo.  
**taboo:** No one is required to sing past exhaustion to earn shelter; dissenting silence is a valid testimony.  
**governance:** Bridge wardens and choir stewards dispute who may interrupt load-bearing performances.  
**labor:** Resonance tending, rope weighing, breath-rest scheduling, tuning-hammer repair and hanging-garden care.  
**trade:** Repair parts, rest shifts and verified safe-path charts; voices are not tradable property.  
**ecology:** Ceiling swifts nest beneath streets; air lichen curls when load-bearing intervals weaken.  
**weather:** Inverted choir draft disorients briefly but warning rails and a fully safe sideline remain reliable.  
**culture:** The Downward Cantors  
**public principle:** No building should stand by erasing the person holding it up.  
**ritual:** Neighbors sing a support note to each other before ascending.  
**sound direction:** Voices above/below, stretched string harmonics, silent counted rests  
**visual direction:** Lavender slate with gold tuning points; optional static camera horizon  
**optional activity:** Help a family adjust a counterweight after observing its true load  
**central dispute:** Structural safety depends on forbidden unpaid labor  

### Sensory and accessibility continuity

**day:** A low resonant seam under wooden rail taps; dangling soil falls upward.  
**night:** The unoccupied singer's platform hums while all visible throats rest.  
**hazard cue:** Support-rail beads tremble before pressure interval; stable colored cue persists without audio.  
**access:** Subtitles identify note direction; alternative static horizon and rail pulse support vestibular accessibility.  

### Mystery, physical evidence and competing readings

**question:** Who should be allowed to stop singing when the missing note holds up homes?  
**visible:** An unused score stand supports a terrace with no musician present.  
**false lead:** The singers freely enjoy endless work; several expressly deny this, others find meaning in singing.  
**interpretation:** The absence physically stabilizes a community but cannot justify continuing compelled labor.  
**link:** The missing interval is the same impossible beat recorded in Cinderhold's hearth slate.  
**hint:** When the rail beads stop, the lectern can be approached on solid ground.  
**do not reveal:** Do not make musicians metaphysical batteries that disappear after rescue.  
- **`LUE.SkyBelow.01` — Missing-singer lectern:** Examine a scratched rest tally under the score. A seventh resting interval appears between six singers.
- **`LUE.SkyBelow.02` — Counterweight lower bridge:** Compare load changes during marked safe chorus breaks. The bridge carries an absent weight instead of failing.
- **`LUE.SkyBelow.03` — Public choir rest bench:** Read an account only after asking the resting singer. Consent to perform was treated as permanent once spoken.

### Ordinary residents' routines, consent and recoverable failure

#### Rest platform — `LUR.SkyBelow.01`

**window:** dawn  
**trigger:** first arrival  
**action:** Carry a support cushion to a resting singer before asking for a performance  
**reaction:** Singer refuses a performance while willingly sharing a memory about the empty stand  
**line:** A rest is not silence. It is something we are owed.  
**feedback:** A rail-glow gap points toward the hidden seventh rest mark  
**failure:** Demanding a song produces a refusal and no punishment  
**recovery:** Ask about the score with accessible written dialogue  
**persistence:** Consent interaction does not grant magic progress  

#### Hanging-garden bridge — `LUR.SkyBelow.02`

**window:** dusk  
**trigger:** local first story resolved  
**action:** Hold a stabilizer while an NPC traverses a signed route to their home  
**reaction:** Resident comments only on the local choir decision personally known  
**line:** A house is not kept alive if everyone inside must spend their lungs.  
**feedback:** The chosen public bridge state stays visible on reload  
**failure:** Unsupported bridge yields a safe retreat not a lethal trap  
**recovery:** Use lower rail and repeat with no inventory loss  
**persistence:** Optional local visit only  

### Optional local story: The Seventh Rest — `LUQ.SkyBelow.SeventhRest`

**unlock:** Orravane first story resolved; optional  
**objective:** Build one predictable rest interval into the community's load-bearing routine  
**steps:** Inspect the empty music stand and six public rest clocks; Listen to two singers with different opinions about duty; Move a mechanical support brace along the safe bridge; Anchor it to a real rail socket while the choir is in a scheduled break; Choose a longer rest timetable or independent public load instrumentation  
**branches:** {"care":"Community rest board gains protected intervals.","truth":"Transparent load measurement shows exactly when pressure is safe to reduce."}  
**failure:** Brace slips before socket engagement; it returns to safety at lower rail.  
**recovery:** Retake brace; no singer forced into an emergency performance.  
**return dialogue:** The bridge held when we rested. We needed proof of that more than applause.  
**save:** Optional contract only; main choir mechanics remain authoritative  

### Production-only staging requirements

**0:** SM_Choir_SeventhStand  
**1:** SM_Choir_BraceSocket  
**2:** NS_Choir_RailBeads  
**3:** SEQ_Choir_SeventhRest  

### Saved world aftermath and return

**held:** The silent note remains a measured absence beside an enforceable rest charter.  
**many:** Overlaid chorus tracks are readable through subtitles and never required simultaneously.  

### Registered cities and civic power (1)

#### Orravane — `CITY.07`

**recognition:** the upside-down city suspended by an absent seventh note  
**skyline:** Hanging terraces, counterweighted rooms, honey-gold tuning teeth and chalk rest platforms  
**sound:** A chorus with a structurally necessary missing interval  
**social rule:** Tap a load rail before speaking so a voice will not be mistaken for an echo  
**civic power:** Cantors' Support Circle  
**personal stakes:** Yllesh protects occupied buildings; Orru argues that compulsory singing is another kind of erasure  
**anomaly:** At each seventh rest an empty platform rocks as though someone sat down  
**reveal state:** open  
**visual icon:** seven rail beads with one empty socket  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

**Source provenance:** [Living Universe](../../../Authoring/lived_universe_atlas.json) · [World Content](../../../Authoring/world_content_pack.json) · [Literary Cities](../../../Authoring/transmedia_storyworld_v1.json)

---

## The Bones of Yesterdays — `CinderSpine`

**settlements:** Vathless  
**ordinary:** Stonecutters repair homes built from inherited names; children trace rejected family trees under the quarry shelter.  
**architecture:** Ancestral quarry shelves, family-wall dwellings, layered stone ledgers and roots hardened into civic stairs.  
**custom:** Before shaping a family stone, ask whose name is already inside it; refusal to answer is not proof of ownership.  
**taboo:** Breaking a bearing stone to make a moral point is forbidden while people live above it.  
**governance:** Stone stewards prioritize structural safety; remembrance guild advocates for unrecorded families.  
**labor:** Mortar grinding, brace repair, name-rubbing, quarry dust screening and foundation surveying.  
**trade:** Measured safe stone, repair labor and open memorial casts; no sale of another family's deleted ancestry.  
**ecology:** Name lichens distinguish quarry strata by remembered footsteps; stone swallows circle just before shelf fracture.  
**weather:** Dry law-root pressure cracks the quarry with visible fracture lines; marked side shelter stays safe.  
**culture:** The Seam-Walkers  
**public principle:** Stone cut from history still belongs to the people erased with it.  
**ritual:** Quarry workers put a blank space in each wage ledger for missing ancestors.  
**sound direction:** Stone groans, dry percussion and voices trapped in rock strata  
**visual direction:** Ash-white and iron purple; cracks visibly widen before collapse  
**optional activity:** Make a safe brace from a documented material source  
**central dispute:** The mine supports homes while it un-makes the lineages that built them  

### Sensory and accessibility continuity

**day:** Granite powder, cold damp air, chisel echoes too long for the gallery.  
**night:** Faint household names emerge in low angled light along the stairs.  
**hazard cue:** Bright chalk lines widen and gravel rattles before a shelf split.  
**access:** High contrast cracks, dust-resistant camera, low-frequency captions; no invisible collapse.  

### Mystery, physical evidence and competing readings

**question:** How many generations can a house erase and still call itself ancestral?  
**visible:** A bearing stone has a worked child-sized handprint below a false founding date.  
**false lead:** A whole civilization knowingly killed its ancestors; records show disputed incremental decisions over generations.  
**interpretation:** Injustice has become load-bearing architecture; repair demands engineering and witness consent, not demolition.  
**link:** Missing quarry birth marks correlate with Drevlach's speculative debt stamps.  
**hint:** The quieter stair at night reveals the lower inscription.  
**do not reveal:** A name revealed by Glimpse is evidence, not automatic consent to publish it.  
- **`LUE.CinderSpine.01` — Quarry mother-stone:** Brush dust from mismatched chisels without weakening support. An older family name survives beneath a legally new stone seal.
- **`LUE.CinderSpine.02` — Household foundation register:** Compare one inhabited dwelling's weight ledger to its family roll. The building is supported by stone attributed to a person never listed.
- **`LUE.CinderSpine.03` — Memorial stair turn:** Hold a rubbing to angled lantern light. Drevlach's account mark appears where a birth record was removed.

### Ordinary residents' routines, consent and recoverable failure

#### Foundation repair lane — `LUR.CinderSpine.01`

**window:** work  
**trigger:** player enters inhabited district  
**action:** Carry brace pins around a marked unstable stair without breaking ancestral stone  
**reaction:** Stone worker asks whose roof stays standing and whose name was covered  
**line:** I can mend the crack. I cannot decide whose grandfather it was.  
**feedback:** House stabilizes cosmetically without erasing inscription  
**failure:** Hazard warning sends worker to refuge; pins remain at shelter  
**recovery:** Recover pins after pulse and use the signed path  
**persistence:** Physical task gives no infinite stone drops  

#### Remembrance guild wall — `LUR.CinderSpine.02`

**window:** night  
**trigger:** first story resolved  
**action:** Read a family's anonymous remembrance alongside an official house deed  
**reaction:** Relative can decline to identify the erased person  
**line:** We have lived here for every year they say we did not.  
**feedback:** Choice-respecting memorial changes its public plaque  
**failure:** Attempt to publish without consent only produces explicit refusal  
**recovery:** Retain a private rubbing or request authorized public summary  
**persistence:** Optional codex; no forced story overwrite  

### Optional local story: Names Under Load — `LUQ.CinderSpine.LoadingNames`

**unlock:** Vathless first story resolved; optional  
**objective:** Secure a lived-in wall and preserve evidence of its erased builders  
**steps:** Inspect stress seam before entering the inhabited lower room; Listen to household and quarry steward account; Collect support pins from a safely abandoned ledge; Fit pins into two designated load sockets; Choose a consent-protected memorial or a public architectural audit  
**branches:** {"care":"Family-private name archive remains accessible to descendants.","truth":"A structural audit exposes the erased work without exposing living descendants."}  
**failure:** Incorrect socket warns and refuses to bear load; house never auto-collapses.  
**recovery:** Remove and relocate pins; test weight marker before committing.  
**return dialogue:** The wall is still standing. The name under it finally has space to breathe.  
**save:** Optional LUQ contract only; actual collision awaits Unreal  

### Production-only staging requirements

**0:** SM_Quarry_MotherStone  
**1:** SM_Quarry_BracePins  
**2:** NS_Quarry_FractureLine  
**3:** SEQ_Quarry_LoadingNames  

### Saved world aftermath and return

**held:** Ancestry names appear as stable seams in the structures families still occupy.  
**many:** Two construction histories can be inspected without swapping away inhabited homes.  

### Registered cities and civic power (1)

#### Vathless — `CITY.08`

**recognition:** the inhabited quarry of concealed names  
**skyline:** Houses and family stairways carved from load-bearing old quarry walls  
**sound:** A long chisel echo that returns with a different syllable  
**social rule:** No bearing stone may be broken while a household sleeps above it  
**civic power:** The Seam Ward  
**personal stakes:** Ghraet protects structural integrity; Issil asks who has the right to read an erased ancestor aloud  
**anomaly:** The quarry's oldest seam bears a mason's mark from Drevlach's future receipt  
**reveal state:** open  
**visual icon:** handprint beneath a bearing seam  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

**Source provenance:** [Living Universe](../../../Authoring/lived_universe_atlas.json) · [World Content](../../../Authoring/world_content_pack.json) · [Literary Cities](../../../Authoring/transmedia_storyworld_v1.json)

---

## The Hundred Unlived — `HundredUnlived`

**settlements:** Eillun  
**ordinary:** People without authorized birth records teach each other how to build legitimate homes; a visitor's first meal is never conditioned on identity papers.  
**architecture:** Overlapping doorstep grids, unsigned household lamps, reversible street signs and a civic refuge woven from contradictory addresses.  
**custom:** A name is given once as a gift and may be withdrawn; people call each other by agreed gestures when uncertain.  
**taboo:** Never count a person aloud without permission; counting can become conscription or forced recognition.  
**governance:** Shelter pairs and census refusals negotiate practical care; no single municipal count can define personhood.  
**labor:** House-number weaving, lamp repair, garden tending, route witnessing and voluntary address translation.  
**trade:** Portable lamp repairs, shared garden portions and voluntary address stamps; names are never accepted as payment.  
**ecology:** Unlived moths emerge where two paths intersect, and inkless ivy flowers in the shade of unsigned doors.  
**weather:** Census pulses create nonlethal reality strain at marked checkpoints; sheltered routes avoid involuntary counting.  
**culture:** The Unentered Citizenry  
**public principle:** The right to be sheltered cannot depend on surrendering an origin.  
**ritual:** Hosts leave an unlabeled welcome cup and ask a guest how they wish to be addressed.  
**sound direction:** Soft door clicks, unsent letters, footfalls that answer before they occur  
**visual direction:** Fog cobalt and warm shelter amber; no flashing identity scan required  
**optional activity:** Help a guest reach shelter without giving up an identity  
**central dispute:** Recognition may expose the very people it is meant to protect  

### Sensory and accessibility continuity

**day:** Two door knocks one heartbeat apart; herb smoke and lamplight beneath sunny thresholds.  
**night:** Street numbers glow as gestures, never an exact authoritative sequence.  
**hazard cue:** A census arch displays a visible expanding lattice before pressure.  
**access:** Consent dialogue text, signed alternate paths and static-address map; no forced spoken names.  

### Mystery, physical evidence and competing readings

**question:** Can someone have a lived history even when no birth record admits them?  
**visible:** A kitchen stool wears down beneath a wall that officially describes an empty lot.  
**false lead:** The residents are ghost copies; direct care, work and memory establish present lives without origin certainty.  
**interpretation:** Recognition cannot be coerced as the price of existence; unknown origin is not absence of rights.  
**link:** The unclaimed oath syntax is shared with Tharniv's unwritten crown petitions.  
**hint:** Ask at the threshold before touching the public testimony.  
**do not reveal:** Never expose a real character's chosen private name through spell effects without consent.  
- **`LUE.HundredUnlived.01` — Unnamed address loom:** Weave a visible non-identifying directional knot. A guest's address exists as a route even when a registry refuses the person's name.
- **`LUE.HundredUnlived.02` — Long-use kitchen stool:** Compare seat repair stitches with the blank occupancy sheet. Years of communal repair predate the authorized household.
- **`LUE.HundredUnlived.03` — Consent shrine threshold:** Ask permission to read a voluntary public testimony. One person's chosen identity is not reducible to a census count.

### Ordinary residents' routines, consent and recoverable failure

#### Two-door kitchen — `LUR.HundredUnlived.01`

**window:** dawn  
**trigger:** player passes initial census threshold  
**action:** Find a missing water bowl by following voluntary house gestures instead of numbering people  
**reaction:** A neighbor thanks the player for asking for directions, not identity  
**line:** You can find my door. That is enough for breakfast.  
**feedback:** Three location gestures mark safe path  
**failure:** A guessed address brings player to a polite wrong door  
**recovery:** Recheck gestures; no public shaming  
**persistence:** Codex learns gesture only; no mandatory counting  

#### Community address loom — `LUR.HundredUnlived.02`

**window:** night  
**trigger:** Eillun first story resolved  
**action:** Help illuminate a route that can be followed without naming a resident  
**reaction:** Shelter witness explains what public knowledge is and isn't allowed  
**line:** A path belongs to feet, not to a registry.  
**feedback:** New signed footpath is visible without changing main atlas locks  
**failure:** Involuntary census arch refuses entry via safe bypass  
**recovery:** Take alternate loom street; still sees same story  
**persistence:** No involuntary personal flag  

### Optional local story: The Unnumbered Door — `LUQ.HundredUnlived.UnnumberedDoor`

**unlock:** Eillun first story resolved; optional  
**objective:** Make a guest route legible without forcing residents into the census  
**steps:** Find a lost courier using two public directional gestures; Ask two hosts independently how they want travelers guided; Collect lamp glass from an unoccupied repair stand; Set lamps along an alternate marked alley; Choose anonymous shared wayfinding or opt-in address plaques  
**branches:** {"care":"Gestural wayfinding guides guests without personal census.","truth":"Opt-in plaques allow public identification where consent exists."}  
**failure:** An invalid address knocks at the wrong door without escalating hostility.  
**recovery:** Return to the public loom, learn gesture order and retry.  
**return dialogue:** The guest reached our door without knowing our names. We chose to open it.  
**save:** New optional contract; no personal census registry implemented  

### Production-only staging requirements

**0:** SM_Unlived_AddressLoom  
**1:** SM_Unlived_WornStool  
**2:** NS_Unlived_CensusLattice  
**3:** SEQ_Unlived_UnnumberedDoor  

### Saved world aftermath and return

**held:** One visible route honors people whether or not their names become public.  
**many:** Multiple address geometries remain passable with clearly chosen static guide mode.  

### Registered cities and civic power (1)

#### Eillun — `CITY.09`

**recognition:** the city that cannot agree how many people live there  
**skyline:** Woven address signs, twin-door kitchens, unsigned lantern ways and a threshold loom  
**sound:** Paired door knocks whose second answer sometimes comes first  
**social rule:** Ask how to find someone's home, never demand their official number  
**civic power:** Shelter Pair Assemblies  
**personal stakes:** Nhel maintains counts to deliver food; Auvet protects those who won't be counted; Veysha fears what a census can enforce  
**anomaly:** An empty registered plot wears a threshold smooth beneath constant footsteps  
**reveal state:** open  
**visual icon:** two door markers tied by one thread  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

**Source provenance:** [Living Universe](../../../Authoring/lived_universe_atlas.json) · [World Content](../../../Authoring/world_content_pack.json) · [Literary Cities](../../../Authoring/transmedia_storyworld_v1.json)

---

## The Orchard of Unwritten Kings — `OrchardOfKings`

**settlements:** Tharniv  
**ordinary:** Families tend grafted trees whose fruits are subject to laws never enacted; petitions grow on branches rather than sitting on a throne.  
**architecture:** Grafted civic orchards, paper-bark meeting rings, unoccupied crown plinths and root roads fenced by polite petitions.  
**custom:** A petitioner leaves one fruit untouched to represent the person absent from the hearing.  
**taboo:** No outsider may crown another person by praise alone; the empty throne is not a secret boss weakness.  
**governance:** Rotating orchard circles interpret inherited rules; nobody can claim the absent monarch's exclusive mandate.  
**labor:** Root pruning, petition sorting, harvest weighing, branch grafting and shade-road maintenance.  
**trade:** Fruit pledges, tool repair and public harvest exchanges; laws cannot be bought as loot.  
**ecology:** Law roots twist toward unsigned petitions; double-orchard bees pollinate both unchosen grafts.  
**weather:** Enforcement roots visibly lift ground before restraining a lane; marked path skirts pressure safely.  
**culture:** The Untitled Courts  
**public principle:** No law is just because its author wears a name.  
**ritual:** Gardeners recite dissenting votes before deciding where a communal root may grow.  
**sound direction:** Leaves tapping crowns, underground creaks and deliberate empty applause  
**visual direction:** Deep green and garnet with legible law marks on living bark  
**optional activity:** Regraft a public path by gaining consent from both sides  
**central dispute:** Refusing power can conceal power exercised without consent  

### Sensory and accessibility continuity

**day:** Overripe orchard peel, dry petition paper, creaking boughs like seating benches.  
**night:** Empty plinth throws a crown-shaped shadow while no crown exists.  
**hazard cue:** Roots announce their line by raised dirt and fluttering leaves before contact.  
**access:** Stable canopy view and ground markers; clue readings available as plain text.  

### Mystery, physical evidence and competing readings

**question:** Why does an unwritten decree decide who may eat?  
**visible:** A branch knots itself around a petition whose author never signed.  
**false lead:** The crown is cursed by a single necromancer; every petition demonstrates communal complicity instead.  
**interpretation:** A society delegated responsibility to a law whose legitimacy was never examined; removing a tyrant alone cannot change habits.  
**link:** One denied petition's route gestures match Eillun's consent-based addresses.  
**hint:** Leave the untouched fruit on the scale until both records are visible.  
**do not reveal:** Never turn the empty throne into proof of a preselected royal bloodline.  
- **`LUE.OrchardOfKings.01` — Unoccupied crown plinth:** Compare its dust and shadow without taking an imagined crown. The empty shape is a social expectation, not proof of a hidden ruler.
- **`LUE.OrchardOfKings.02` — Harvest ledger ring:** Weigh a rejected apple beside one publicly allocated. Fruit responds to a rule nobody can locate in a signed vote.
- **`LUE.OrchardOfKings.03` — Petition tree bark:** Read two mutually incompatible but still in-force petitions. An Eillun address has been denied harvest access despite no legal judgment.

### Ordinary residents' routines, consent and recoverable failure

#### Civic harvest circle — `LUR.OrchardOfKings.01`

**window:** work  
**trigger:** player enters Tharniv before local trial  
**action:** Use a public scale to sort split-graft fruit without claiming an automatic title  
**reaction:** Orchard worker asks which household has the right to refuse an obligation  
**line:** An apple doesn't ask who wrote the law that weighs it.  
**feedback:** A second graft shadow reveals readable petition under bark  
**failure:** Picking forbidden fruit triggers a verbal challenge, not irreversible faction hostility  
**recovery:** Return fruit to tray; inspect petition and scale  
**persistence:** Low-stakes local reaction only  

#### Empty plinth — `LUR.OrchardOfKings.02`

**window:** night  
**trigger:** Tharniv first story resolved  
**action:** Observe two residents argue a petition under crown-shaped shadow  
**reaction:** No actor pretends the unseen monarch has personally spoken  
**line:** The law survived its author. Perhaps it never had one.  
**feedback:** Later public marker respects committed civic outcome  
**failure:** Night-only clue absent earlier  
**recovery:** Read daytime posted petition to find night meeting schedule  
**persistence:** Optional listening record  

### Optional local story: The Fruit Left Untouched — `LUQ.OrchardOfKings.UntouchedFruit`

**unlock:** Tharniv first chapter resolved; optional  
**objective:** Permit harvest access without crowning anyone or deleting a dissenting petition  
**steps:** Find the unsigned harvest prohibition under petition bark; Hear a grower and a nonresident claimant with conflicting histories; Balance fruit on an existing mechanical public scale; Carry a graft token through the law-root safe path; Choose a common harvest table or an open petition hearing with consent safeguards  
**branches:** {"care":"Common table feeds visitors without invoking an unchosen ruler.","truth":"Public hearing lists actual legal decisions and preserves dissent."}  
**failure:** Scale tips if fruit is moved too soon; no fictional invisible guilt counter.  
**recovery:** Reset physical balance and reweigh with both tokens.  
**return dialogue:** The fruit fed someone. Nobody had to wear the crown.  
**save:** Optional quest, authority remains original orchard progression  

### Production-only staging requirements

**0:** SM_Orchard_EmptyPlinth  
**1:** SM_Orchard_PetitionBark  
**2:** NS_Orchard_RootTelegraph  
**3:** SEQ_Orchard_UntouchedFruit  

### Saved world aftermath and return

**held:** Old petitions remain etched as scars on one stable public harvest table.  
**many:** Two harvest practices can coexist, explicitly marked rather than phasing players without notice.  

### Registered cities and civic power (1)

#### Tharniv — `CITY.10`

**recognition:** the orchard capital with a throne nobody accepted  
**skyline:** An empty crown plinth in a grafted apple grove, petition bark and rooted voting circles  
**sound:** Autumn leaves shuffling like papers being moved across a hearing desk  
**social rule:** Leave one piece of fruit unclaimed at a hearing  
**civic power:** Rotating Harvest Circle  
**personal stakes:** Serriv refuses easy legitimacy; Imro feeds people rather than theories; Eliun records appeals no throne can adjudicate  
**anomaly:** The tree growing over the plinth has older petitions than roots  
**reveal state:** open  
**visual icon:** uncrowned fruit balanced on scales  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

**Source provenance:** [Living Universe](../../../Authoring/lived_universe_atlas.json) · [World Content](../../../Authoring/world_content_pack.json) · [Literary Cities](../../../Authoring/transmedia_storyworld_v1.json)

---

## The Place Before Place — `FirstAbsence`

**settlements:** Auvren  
**ordinary:** Travelers maintain markers to departures they never made; witnesses debate whether a threshold can be a home rather than a test.  
**architecture:** Northern witness plateau, two founding thresholds, limestone rooms containing their own outside, and an open arena built from public testimony.  
**custom:** When someone asks where you come from, it is polite to answer what you choose to carry, not what blood says.  
**taboo:** Never require a traveler to complete a private guardian challenge before they may leave the place.  
**governance:** No crown or registered state; witnesses curate openly contested entry accounts and a stable public exit.  
**labor:** Path chalking, lamp tending, worn step repair, witness listening and maintaining the northern passage.  
**trade:** Repair supplies and harmless collected inscriptions; entry to the final path is not purchasable.  
**ecology:** Threshold moths gather at doors opened from both sides; origin stones cool in reverse sunset order.  
**weather:** Horizon folds over the northern witness land; a legible stable path never disappears with optional effects.  
**culture:** The Before-Witnesses  
**public principle:** More than one remembered beginning may be true without erasing any witness.  
**ritual:** Each newcomer is asked what they need, not which history they belong to.  
**sound direction:** Silent horizon pulses, doubled footfalls, an almost-remembered lullaby  
**visual direction:** Obsidian with contrasting pearl outlines; stable horizon setting mandatory  
**optional activity:** Compare two truthful maps, keeping a safe return path open  
**central dispute:** An imposed single origin would stabilize space by invalidating lives  

### Sensory and accessibility continuity

**day:** Air without echo, chalk dust, daylight seemingly arriving from the destination.  
**night:** Two distant sunrise tones are perceptible as distinct illuminated bands even with audio muted.  
**hazard cue:** A threshold line draws outward before collision rules change, with explicit stable marker.  
**access:** Static-horizon option, reduced distortion, clear route text and no reversed physical controls.  

### Mystery, physical evidence and competing readings

**question:** How can a person arrive before any world grants them a birthplace?  
**visible:** A worn footprint points toward an entry gate that was built after the footprint formed.  
**false lead:** The first witness is the creator of the player and therefore may command them; source only supports a remembered impossible arrival.  
**interpretation:** The protagonist's shared impossible origin grants no unilateral right to overwrite other lives; the confrontation is about inherited responsibility.  
**link:** Both witnesses echo the Bellwold interval without explaining away ordinary civic injustice.  
**hint:** The witnesses can be visited in either order; follow marked northern safe line.  
**do not reveal:** Do not spoil mask-breaking or two mornings in the public first-arrival dialogue.  
- **`LUE.FirstAbsence.01` — Southern arrival footprint:** Compare the player's present footfall with older mark without using personal profile. The earlier mark is possible without choosing a predetermined ancestry.
- **`LUE.FirstAbsence.02` — Twin founding witnesses:** Listen to both testimony markers in either order. The accounts contradict without either requiring immediate erasure.
- **`LUE.FirstAbsence.03` — Northern public threshold:** Inspect a stable exit line before opting into the arena. Auvren can hold open a question without paying with someone's life.

### Ordinary residents' routines, consent and recoverable failure

#### Threshold lamp stand — `LUR.FirstAbsence.01`

**window:** dawn  
**trigger:** player earns first Auvren entry  
**action:** Replace one extinguished lamp glass before questioning a waiting witness  
**reaction:** Witness speaks as an individual who noticed only the player's entry, not all past choices  
**line:** You came here before you came here. I cannot tell you what that makes you.  
**feedback:** Old footprint acquires a separate visible cast shadow  
**failure:** Wrong lamp glass only refracts light; no fake ancestral choice  
**recovery:** Swap with correctly etched glass from adjacent shelf  
**persistence:** Discovery note only  

#### Public north-exit steps — `LUR.FirstAbsence.02`

**window:** night  
**trigger:** Auvren first chapter done and main boss gate true  
**action:** Choose to sit and read two incompatible witness accounts before entering northern arena  
**reaction:** Witness offers information but refuses to assign final moral ending  
**line:** Do not ask me which first day happened. Ask who survived each.  
**feedback:** Optional context without additional gate or reward  
**failure:** Player can ignore dialogue and take same accessible boss route  
**recovery:** Read public placards at any time  
**persistence:** No prerequisite flag; original FinalJourney remains authoritative  

### Optional local story: A Mark for Leaving — `LUQ.FirstAbsence.LeavingMark`

**unlock:** after Auvren first trial; optional before or after ending  
**objective:** Create a reliable public return marker without deciding everyone's first origin  
**steps:** Find the older departure footprint on the southern step; Hear each public witness in any order; Collect a safe chalk lens from the lamp stand; Place it on a public exit rather than a locked private arena door; Choose a single stable marker or visibly paired route markers; preserve safe exit  
**branches:** {"care":"One legible public exit arrow remains regardless of future ending.","truth":"Paired departure accounts are posted together with a stable navigation overlay."}  
**failure:** Choosing an arena-only wall is refused and the chalk lens is returned.  
**recovery:** Use the signed public steps and try either branch again before commitment.  
**return dialogue:** A way out is not a verdict about where we began.  
**save:** Separate optional LUQ contract, cannot mark boss defeated or FinalJourney state  

### Production-only staging requirements

**0:** SM_Absence_OlderFootprint  
**1:** SM_Absence_ChalkLens  
**2:** NS_Absence_ThresholdBand  
**3:** SEQ_Absence_LeavingMark  

### Saved world aftermath and return

**held:** A single public exit keeps old contradiction readable in footnote markers.  
**many:** Paired thresholds remain equally navigable; static single-line guidance is available.  

### Registered cities and civic power (1)

#### Auvren — `CITY.11`

**recognition:** the threshold city whose departure steps were made before arrival  
**skyline:** Limestone rooms that imply an outside within themselves, two founding thresholds and a northern witness plateau  
**sound:** A footstep arrives before its audible contact  
**social rule:** When asked where you are from, describe what you carry, not what blood proves  
**civic power:** Open Witness Steps  
**personal stakes:** Orriv remembers arrivals that never happened; Veilune preserves incompatible testimony; Thae keeps exits reliably passable  
**anomaly:** The second threshold opens on a morning the city never experienced  
**reveal state:** sealed  
**visual icon:** footprint facing its own approach  
**adaption rule:** Keep the city's material signature, audible identity, civic disagreement, and visible ordinary life; never make its paradox a generic fantasy portal.  

**Source provenance:** [Living Universe](../../../Authoring/lived_universe_atlas.json) · [World Content](../../../Authoring/world_content_pack.json) · [Literary Cities](../../../Authoring/transmedia_storyworld_v1.json)

---

