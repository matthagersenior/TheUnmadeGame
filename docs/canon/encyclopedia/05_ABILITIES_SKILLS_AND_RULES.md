# Volume V — Entire Ability Codex: Reality, Combat, Mastery, Studies, Progression and Rules

**Coverage:** 243 indexed codex entries across 28 categories. These entries encompass spells, skills, gear, quests, enemies, areas and mechanics, **not** 243 independently castable powers.

**Authority:** [canonical Ability Codex JSON](../../../Authoring/ability_codex.json). C++ source remains authoritative for actual runtime values and save state. `source_rule` = source model; `prototype_handler` = an Unreal-facing handler not engine verified; `definition_only`/`proposed` = non-executable definition. None of these labels establishes a packaged playable build.

**Learning law:** Encounter → firsthand observation → understanding → authored experimentation → save-backed mastery → bounded combination. An observable monster behavior is not automatically an unlockable skill. A requirement checklist describes real game prerequisites, not a manual shortcut.

## INHERENT (5)

### Glimpse — `inherent.glimpse`

**category:** inherent  
**summary:** Reveal alternate authored physical states and clues  
**acquisition:** {"method":"inherent","requirements":["appropriate fracture target when needed"]}  
**effect:** {"kind":"reality","summary":"Reveal alternate authored physical states and clues"}  
**cost:** {"strain":8}  
**duration:** 3  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Fracture/UnmadeFractureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `inherent.glimpse.requirement.1` — appropriate fracture target when needed. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Fold — `inherent.fold`

**category:** inherent  
**summary:** Temporarily change a valid fracture barrier and expose nearby enemies  
**acquisition:** {"method":"inherent","requirements":["appropriate fracture target when needed"]}  
**effect:** {"kind":"reality","summary":"Temporarily change a valid fracture barrier and expose nearby enemies"}  
**cost:** {"strain":24}  
**duration:** 6  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Fracture/UnmadeFractureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `inherent.fold.requirement.1` — appropriate fracture target when needed. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Rewrite — `inherent.rewrite`

**category:** inherent  
**summary:** Confirm and persist a supported world variant  
**acquisition:** {"method":"inherent","requirements":["appropriate fracture target when needed"]}  
**effect:** {"kind":"reality","summary":"Confirm and persist a supported world variant"}  
**cost:** {"strain":60}  
**duration:** persistent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Fracture/UnmadeFractureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `inherent.rewrite.requirement.1` — appropriate fracture target when needed. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Reality Strain Recovery — `inherent.reality_strain_recovery`

**category:** inherent  
**summary:** Naturally recover 3 Strain per second  
**acquisition:** {"method":"inherent","requirements":["appropriate fracture target when needed"]}  
**effect:** {"kind":"reality","summary":"Naturally recover 3 Strain per second"}  
**cost:** {"strain":0}  
**duration:** passive  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Fracture/UnmadeFractureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `inherent.reality_strain_recovery.requirement.1` — appropriate fracture target when needed. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Fracture Exposure — `inherent.fracture_exposure`

**category:** inherent  
**summary:** Temporarily stagger or expose enemies with a Fold-based effect  
**acquisition:** {"method":"inherent","requirements":["appropriate fracture target when needed"]}  
**effect:** {"kind":"reality","summary":"Temporarily stagger or expose enemies with a Fold-based effect"}  
**cost:** {"strain":"via Fold"}  
**duration:** temporary  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Fracture/UnmadeFractureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `inherent.fracture_exposure.requirement.1` — appropriate fracture target when needed. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## RITE (10)

### UnwriteLaw — `rite.UnwriteLaw`

**category:** rite  
**summary:** Suspend local gravity (launch), sound (expose threats), or momentum (push nearby enemies)  
**acquisition:** {"method":"five_stage_rite","site":"Bellgrave","witness":"npc.bellmaker.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Suspend local gravity (launch), sound (expose threats), or momentum (push nearby enemies)"}  
**cost:** {"strain":18,"cooldown_seconds":80}  
**duration:** {"seconds":12}  
**reward:** The Bellmaker's Unwritten Verse  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.UnwriteLaw.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.UnwriteLaw.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.UnwriteLaw.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.UnwriteLaw.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.UnwriteLaw.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Witnesscraft — `rite.Witnesscraft`

**category:** rite  
**summary:** Require credible living witnesses to manifest a missing bridge and classify evidence  
**acquisition:** {"method":"five_stage_rite","site":"EchoWell","witness":"npc.welllistener.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Require credible living witnesses to manifest a missing bridge and classify evidence"}  
**cost:** {"strain":14,"cooldown_seconds":75}  
**duration:** {"seconds":20}  
**reward:** The Bridge That Remembers  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.Witnesscraft.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Witnesscraft.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Witnesscraft.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Witnesscraft.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Witnesscraft.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### BorrowedLives — `rite.BorrowedLives`

**category:** rite  
**summary:** Borrow temporary fighter, artisan or archivist skills from an alternate life  
**acquisition:** {"method":"five_stage_rite","site":"PaperOrchard","witness":"npc.orchardexile.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Borrow temporary fighter, artisan or archivist skills from an alternate life"}  
**cost:** {"strain":20,"cooldown_seconds":120}  
**duration:** {"seconds":40}  
**reward:** The Second Name  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.BorrowedLives.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.BorrowedLives.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.BorrowedLives.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.BorrowedLives.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.BorrowedLives.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### LegacyForging — `rite.LegacyForging`

**category:** rite  
**summary:** Imbue a weapon using genuine protected, discovered and reconciled deeds  
**acquisition:** {"method":"five_stage_rite","site":"BellwoldForge","witness":"npc.bellwold.stonewright.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Imbue a weapon using genuine protected, discovered and reconciled deeds"}  
**cost:** {"strain":12,"cooldown_seconds":110}  
**duration:** {"seconds":55}  
**reward:** The Three-Deed Temper  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.LegacyForging.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.LegacyForging.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.LegacyForging.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.LegacyForging.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.LegacyForging.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### LivingRoads — `rite.LivingRoads`

**category:** rite  
**summary:** Temporarily form a shared road; earned reconciliation may stabilize it  
**acquisition:** {"method":"five_stage_rite","site":"SilentMile","witness":"npc.roadwarden.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Temporarily form a shared road; earned reconciliation may stabilize it"}  
**cost:** {"strain":16,"cooldown_seconds":90}  
**duration:** {"seconds":30}  
**reward:** The Common Causeway  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.LivingRoads.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.LivingRoads.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.LivingRoads.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.LivingRoads.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.LivingRoads.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### TomorrowDebt — `rite.TomorrowDebt`

**category:** rite  
**summary:** Borrow attack strength now, suffer exhaustion when future debt comes due  
**acquisition:** {"method":"five_stage_rite","site":"DebtMarket","witness":"npc.tollbroker.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Borrow attack strength now, suffer exhaustion when future debt comes due"}  
**cost:** {"strain":8,"cooldown_seconds":125}  
**duration:** {"seconds":17}  
**reward:** Tomorrow's Paid Receipt  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.TomorrowDebt.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.TomorrowDebt.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.TomorrowDebt.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.TomorrowDebt.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.TomorrowDebt.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### UnderstandingBosses — `rite.UnderstandingBosses`

**category:** rite  
**summary:** Use verified memory to interrupt or spare a specific eligible boss  
**acquisition:** {"method":"five_stage_rite","site":"BellOssuary","witness":"npc.bellwold.matron.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Use verified memory to interrupt or spare a specific eligible boss"}  
**cost:** {"strain":24,"cooldown_seconds":140}  
**duration:** {"seconds":16}  
**reward:** Mercy of the Hollow Keeper  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.UnderstandingBosses.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.UnderstandingBosses.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.UnderstandingBosses.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.UnderstandingBosses.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.UnderstandingBosses.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### ParadoxConvergence — `rite.ParadoxConvergence`

**category:** rite  
**summary:** Overlay two physical versions of a place and inspect their conflicting routes  
**acquisition:** {"method":"five_stage_rite","site":"PaperhavenScribe","witness":"npc.paperhaven.mapmaker.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Overlay two physical versions of a place and inspect their conflicting routes"}  
**cost:** {"strain":26,"cooldown_seconds":150}  
**duration:** {"seconds":19}  
**reward:** The Room Between Histories  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.ParadoxConvergence.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.ParadoxConvergence.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.ParadoxConvergence.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.ParadoxConvergence.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.ParadoxConvergence.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Oathbinding — `rite.Oathbinding`

**category:** rite  
**summary:** Protect people through a witnessed oath; betrayal costs trust and ability use  
**acquisition:** {"method":"five_stage_rite","site":"BellwoldRefuge","witness":"npc.bellwold.guard.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Protect people through a witnessed oath; betrayal costs trust and ability use"}  
**cost:** {"strain":12,"cooldown_seconds":135}  
**duration:** {"seconds":50}  
**reward:** The Unbroken Shelter Oath  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.Oathbinding.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Oathbinding.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Oathbinding.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Oathbinding.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Oathbinding.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Cartography — `rite.Cartography`

**category:** rite  
**summary:** Show truth confidence for rumored, speculative and personally verified crossings  
**acquisition:** {"method":"five_stage_rite","site":"SaltwakeHarbor","witness":"npc.saltwake.navigator.001","requirements":["Physically discover inscription","Speak to named witness","Perform Strain-costing trial","Reach witnessed choice","Return for discipline-specific mastery"]}  
**effect:** {"kind":"reality_rite","summary":"Show truth confidence for rumored, speculative and personally verified crossings"}  
**cost:** {"strain":15,"cooldown_seconds":95}  
**duration:** {"seconds":35}  
**reward:** Atlas of the Missing Coast  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"unlock_path":"authored rite stages"}  
**limitations:** Cannot claim mastery from dialogue alone; Limited physical context, Strain and cooldown  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `rite.Cartography.requirement.1` — Physically discover inscription. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Cartography.requirement.2` — Speak to named witness. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Cartography.requirement.3` — Perform Strain-costing trial. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Cartography.requirement.4` — Reach witnessed choice. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `rite.Cartography.requirement.5` — Return for discipline-specific mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## CONFLUENCE (6)

### The Alarm Nobody Heard — `confluence.SilentAlarm`

**category:** confluence  
**summary:** Bellwold's warning bell must fall silent long enough to stop the keeper, but witnesses must restore its message before the town sleeps.  
**acquisition:** {"method":"dual_rite_challenge","requirements":["Master both rites","Use both within 120 in-world seconds at the chamber","Obtain two credible witnesses for the outcome"]}  
**effect:** {"kind":"combined_rite_challenge","summary":"Bellwold's warning bell must fall silent long enough to stop the keeper, but witnesses must restore its message before the town sleeps."}  
**cost:** {"component_rite_costs":true,"window_seconds":120}  
**duration:** chapter resolution  
**reward:** Aegis of the Last Warning  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:** rite.UnwriteLaw; rite.Witnesscraft  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeConfluenceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `confluence.SilentAlarm.requirement.1` — Master both rites. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.SilentAlarm.requirement.2` — Use both within 120 in-world seconds at the chamber. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.SilentAlarm.requirement.3` — Obtain two credible witnesses for the outcome. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### A Blade for Both Your Names — `confluence.TwoNames`

**category:** confluence  
**summary:** The neverborn orchard sent a smith who was never apprenticed. Two versions of the hero must agree on which acts a weapon should remember.  
**acquisition:** {"method":"dual_rite_challenge","requirements":["Master both rites","Use both within 120 in-world seconds at the chamber","Obtain two credible witnesses for the outcome"]}  
**effect:** {"kind":"combined_rite_challenge","summary":"The neverborn orchard sent a smith who was never apprenticed. Two versions of the hero must agree on which acts a weapon should remember."}  
**cost:** {"component_rite_costs":true,"window_seconds":120}  
**duration:** chapter resolution  
**reward:** Edge of the Elseborn Smith  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:** rite.BorrowedLives; rite.LegacyForging  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeConfluenceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `confluence.TwoNames.requirement.1` — Master both rites. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.TwoNames.requirement.2` — Use both within 120 in-world seconds at the chamber. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.TwoNames.requirement.3` — Obtain two credible witnesses for the outcome. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Route Nobody Owned — `confluence.UnmappedWay`

**category:** confluence  
**summary:** A passage with no legal owner will remain unstable until cartographers preserve both the safe road and the dangerous rumor.  
**acquisition:** {"method":"dual_rite_challenge","requirements":["Master both rites","Use both within 120 in-world seconds at the chamber","Obtain two credible witnesses for the outcome"]}  
**effect:** {"kind":"combined_rite_challenge","summary":"A passage with no legal owner will remain unstable until cartographers preserve both the safe road and the dangerous rumor."}  
**cost:** {"component_rite_costs":true,"window_seconds":120}  
**duration:** chapter resolution  
**reward:** Chart of the Common Path  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:** rite.LivingRoads; rite.Cartography  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeConfluenceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `confluence.UnmappedWay.requirement.1` — Master both rites. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.UnmappedWay.requirement.2` — Use both within 120 in-world seconds at the chamber. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.UnmappedWay.requirement.3` — Obtain two credible witnesses for the outcome. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Tomorrow Is Not Yours to Spend — `confluence.TomorrowPromise`

**category:** confluence  
**summary:** A borrowed dawn could light an entire refuge, but its lender has promised not to push the bill onto the helpless.  
**acquisition:** {"method":"dual_rite_challenge","requirements":["Master both rites","Use both within 120 in-world seconds at the chamber","Obtain two credible witnesses for the outcome"]}  
**effect:** {"kind":"combined_rite_challenge","summary":"A borrowed dawn could light an entire refuge, but its lender has promised not to push the bill onto the helpless."}  
**cost:** {"component_rite_costs":true,"window_seconds":120}  
**duration:** chapter resolution  
**reward:** Heart of the Borrowed Dawn  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:** rite.TomorrowDebt; rite.Oathbinding  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeConfluenceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `confluence.TomorrowPromise.requirement.1` — Master both rites. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.TomorrowPromise.requirement.2` — Use both within 120 in-world seconds at the chamber. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.TomorrowPromise.requirement.3` — Obtain two credible witnesses for the outcome. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Bell Fell and Stood — `confluence.TwoKeepers`

**category:** confluence  
**summary:** A monster and its innocent keeper may stand in contradictory versions of the same place; mercy and truth need not annihilate one another.  
**acquisition:** {"method":"dual_rite_challenge","requirements":["Master both rites","Use both within 120 in-world seconds at the chamber","Obtain two credible witnesses for the outcome"]}  
**effect:** {"kind":"combined_rite_challenge","summary":"A monster and its innocent keeper may stand in contradictory versions of the same place; mercy and truth need not annihilate one another."}  
**cost:** {"component_rite_costs":true,"window_seconds":120}  
**duration:** chapter resolution  
**reward:** Mirror of the Two Keepers  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:** rite.UnderstandingBosses; rite.ParadoxConvergence  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeConfluenceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `confluence.TwoKeepers.requirement.1` — Master both rites. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.TwoKeepers.requirement.2` — Use both within 120 in-world seconds at the chamber. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.TwoKeepers.requirement.3` — Obtain two credible witnesses for the outcome. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Place Before a First Story — `confluence.FirstAbsence`

**category:** confluence  
**summary:** Beyond the coast is a border with neither maps nor ownership. Ten learned disciplines offer choices but no automatic victory over existence itself.  
**acquisition:** {"method":"dual_rite_challenge","requirements":["Master both rites","Use both within 120 in-world seconds at the chamber","Obtain two credible witnesses for the outcome"]}  
**effect:** {"kind":"combined_rite_challenge","summary":"Beyond the coast is a border with neither maps nor ownership. Ten learned disciplines offer choices but no automatic victory over existence itself."}  
**cost:** {"component_rite_costs":true,"window_seconds":120}  
**duration:** chapter resolution  
**reward:** Witness of the First Absence  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:** rite.UnwriteLaw; rite.Cartography  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeConfluenceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `confluence.FirstAbsence.requirement.1` — Master both rites. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.FirstAbsence.requirement.2` — Use both within 120 in-world seconds at the chamber. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `confluence.FirstAbsence.requirement.3` — Obtain two credible witnesses for the outcome. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## EQUIPMENT (48)

### Salvager's Edge — `item.blade.salvager`

**category:** equipment  
**summary:** The first honest blade in a dishonest country.  
**acquisition:** {"method":"starter","requirements":["starter"]}  
**effect:** {"kind":"weapon","attack":3,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.salvager.requirement.1` — starter. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Traveler's Mantle — `item.armor.traveler`

**category:** equipment  
**summary:** A stitched coat with room for a second history.  
**acquisition:** {"method":"starter","requirements":["starter"]}  
**effect:** {"kind":"armor","attack":0,"armor":2,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.traveler.requirement.1` — starter. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Bellwold's Last Refuge — `item.armor.bellwold`

**category:** equipment  
**summary:** Guard the people even when the world calls them forgotten.  
**acquisition:** {"method":"Bellwold lantern story","requirements":["Bellwold lantern story"]}  
**effect:** {"kind":"armor","attack":0,"armor":12,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.bellwold.requirement.1` — Bellwold lantern story. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Lens of the Unentered — `item.charm.paperhaven`

**category:** equipment  
**summary:** Truth survives even if the index does not.  
**acquisition:** {"method":"Paperhaven testimony","requirements":["Paperhaven testimony"]}  
**effect:** {"kind":"charm","attack":4,"armor":4,"healing":0,"strain_recovery":0,"distinct_effects":["Glimpse Strain cost reduced 2"]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.paperhaven.requirement.1` — Paperhaven testimony. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Three Roads, One Name — `item.blade.wayfarer`

**category:** equipment  
**summary:** Each road can carry you home, but none agrees where.  
**acquisition:** {"method":"visit three villages","requirements":["visit three villages"]}  
**effect:** {"kind":"weapon","attack":12,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.wayfarer.requirement.1` — visit three villages. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Mantle of the Missing Hour — `item.armor.lost_hour`

**category:** equipment  
**summary:** Time remembers the person beneath it.  
**acquisition:** {"method":"bellgrave discovery","requirements":["bellgrave discovery"]}  
**effect:** {"kind":"armor","attack":0,"armor":9,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.lost_hour.requirement.1` — bellgrave discovery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Bark of the Neverborn — `item.armor.orchard`

**category:** equipment  
**summary:** Leaves bloom with signatures that were erased.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"armor","attack":0,"armor":6,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.orchard.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Tomorrow's Debt — `item.blade.debt`

**category:** equipment  
**summary:** It collects the price before the wound.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"weapon","attack":10,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":["heavy Weaken melee style"]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.debt.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Voice Returned — `item.charm.voice`

**category:** equipment  
**summary:** A warning you will speak only after you hear it.  
**acquisition:** {"method":"Echo Well discovery","requirements":["Echo Well discovery"]}  
**effect:** {"kind":"charm","attack":2,"armor":3,"healing":0,"strain_recovery":0,"distinct_effects":["Glimpse Strain cost reduced 3"]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.voice.requirement.1` — Echo Well discovery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Boot-Sigil of the Silent Mile — `item.charm.silent`

**category:** equipment  
**summary:** Your feet learn how to leave no first step.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"charm","attack":3,"armor":2,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.silent.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Heart of the Communal Flame — `item.charm.lantern`

**category:** equipment  
**summary:** Light that may never be purchased.  
**acquisition:** {"method":"shelter supply run","requirements":["shelter supply run"]}  
**effect:** {"kind":"charm","attack":2,"armor":7,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.lantern.requirement.1` — shelter supply run. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Cloak of Contradictory Records — `item.armor.registry`

**category:** equipment  
**summary:** One body, a thousand incompatible histories.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"armor","attack":0,"armor":7,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.registry.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Witness Stone — `item.charm.witness`

**category:** equipment  
**summary:** Someone believes you because they saw you choose.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"charm","attack":5,"armor":1,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.witness.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Needle of Unfinished Maps — `item.blade.mapmaker`

**category:** equipment  
**summary:** Finds the place where the map was cut.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"weapon","attack":19,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":["longer reach and Expose"]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.mapmaker.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Key of the Doorless — `item.charm.key`

**category:** equipment  
**summary:** Unlocks nothing except your willingness to search.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"charm","attack":5,"armor":3,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.key.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Path of Three Settlements — `item.charm.threepaths`

**category:** equipment  
**summary:** The people who remember are your true compass.  
**acquisition:** {"method":"visit three villages","requirements":["visit three villages"]}  
**effect:** {"kind":"charm","attack":6,"armor":5,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.threepaths.requirement.1` — visit three villages. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Waybreaker, the Impossible Road — `item.blade.waybreaker`

**category:** equipment  
**summary:** Forged from shelter, testimony and the missing hour.  
**acquisition:** {"method":"unique Bellwold forge craft after shelter, testimony, villages and materials","requirements":["unique Bellwold forge craft after shelter, testimony, villages and materials"]}  
**effect:** {"kind":"weapon","attack":32,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":["Fold duration +3 seconds; heavy Weaken weapon rhythm"]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.waybreaker.requirement.1` — unique Bellwold forge craft after shelter, testimony, villages and materials. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Unwritten Crown — `item.charm.unwritten_crown`

**category:** equipment  
**summary:** Sovereignty over what history refused to name.  
**acquisition:** {"method":"all six initial landmarks","requirements":["all six initial landmarks"]}  
**effect:** {"kind":"charm","attack":12,"armor":10,"healing":0,"strain_recovery":0,"distinct_effects":["normal Strain recovery ×1.5"]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.unwritten_crown.requirement.1` — all six initial landmarks. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Atlas of Absence — `item.charm.atlas`

**category:** equipment  
**summary:** Every missing country has a place upon the page.  
**acquisition:** {"method":"mastered_cartography_verified_road_witnessed_first_absence_and_study","requirements":["Personally inspect the Echo Well, and return there physically for the claim","Master Memory Lens through firsthand Echo Well observations and experiments","Master two distinct enemy techniques through real telegraphed attacks, demonstrated contact, and paid experiments","Complete the full Cartography rite through its existing witnessed quest stages","Physically verify at least one route via the tenfold route-verification record","Complete the First Absence Confluence: the Unwrite Law + Cartography challenge with two credible witnesses","Maintain valid writable study and inventory save state","Claim the one-time Atlas award; a claimed receipt is never duplicated"]}  
**effect:** {"kind":"charm","attack":8,"armor":12,"healing":0,"strain_recovery":0,"distinct_effects":["Equipped charm: Glimpse Strain discount 4","Equipped charm: Fold duration +2 seconds (even while using regional weapon)"]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h; Source/TheUnmadeGame/Public/World/UnmadeAbilityStudyRules.h; Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Items/UnmadeEquipmentComponent.h; Source/TheUnmadeGame/Private/World/UnmadeTenfoldComponent.cpp; Source/TheUnmadeGame/Public/World/UnmadeConfluenceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.atlas.requirement.1` — Personally inspect the Well of Returned Voices. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `item.charm.atlas.requirement.2` — Master the Memory Lens ability through firsthand experience. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `item.charm.atlas.requirement.3` — Master two distinct hostile principles in physical combat. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `item.charm.atlas.requirement.4` — Complete the Cartography rite through its existing staged quest. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `item.charm.atlas.requirement.5` — Verify at least one route through physical frontier exploration. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `item.charm.atlas.requirement.6` — Resolve The Place Before a First Story with two credible witnesses. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `item.charm.atlas.requirement.7` — Return physically to the Echo Well to claim the artifact. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `item.charm.atlas.requirement.8` — Keep a valid, writable character study and inventory save. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `item.charm.atlas.requirement.9` — Collect the once-only reward; previously claimed Atlas cannot be awarded again. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only

### Aegis of the Hollow Bell — `item.armor.bellheart`

**category:** equipment  
**summary:** No living keeper remains to sound it.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"armor","attack":0,"armor":18,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.bellheart.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Scepter of Missing Names — `item.weapon.redacted`

**category:** equipment  
**summary:** The name of each wound vanishes from the record.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"weapon","attack":23,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":["ranged projectile, Slow"]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.weapon.redacted.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Signet of the Unfinished Pilgrim — `item.charm.pilgrim`

**category:** equipment  
**summary:** Every ending is only a path that turned.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"charm","attack":6,"armor":9,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.pilgrim.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Oath-Tempered Edge — `item.blade.tempered`

**category:** equipment  
**summary:** A blade that remembers whose hands made it.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"weapon","attack":14,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":["fast Burn-inflicting melee rhythm"]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.tempered.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Lanternwoven Mail — `item.armor.lanternmail`

**category:** equipment  
**summary:** Bellwold's embers stitched between iron rings.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"armor","attack":0,"armor":14,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.lanternmail.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Seal of Kept Testimony — `item.charm.inkbound`

**category:** equipment  
**summary:** Even the silenced leave marks behind.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"charm","attack":7,"armor":6,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.inkbound.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Compass of Forgotten Tides — `item.charm.stormglass`

**category:** equipment  
**summary:** It points toward where the vanished sea still longs to return.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"charm","attack":9,"armor":4,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.stormglass.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Emberheart of Cinderhold — `item.blade.emberheart`

**category:** equipment  
**summary:** A blade that can be passed on without diminishing its flame.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"weapon","attack":27,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.emberheart.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Oathguard of the Open Hearth — `item.armor.refuge_oathguard`

**category:** equipment  
**summary:** There is no sanctuary without people willing to defend it.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"armor","attack":0,"armor":15,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.refuge_oathguard.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Quill That Will Not Erase — `item.blade.archivist_quill`

**category:** equipment  
**summary:** Every false record must face someone who remembers.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"weapon","attack":22,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.archivist_quill.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Compass of the Common Road — `item.charm.roadbound`

**category:** equipment  
**summary:** No single village can own a road that people share.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"charm","attack":7,"armor":8,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.roadbound.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Bellmaker's Unwritten Verse — `item.charm.bellmakers_verse`

**category:** equipment  
**summary:** An interval of silence carved out of a condemned bell.  
**acquisition:** {"method":"Unwrite Law mastery","requirements":["Unwrite Law mastery"]}  
**effect:** {"kind":"charm","attack":6,"armor":7,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.bellmakers_verse.requirement.1` — Unwrite Law mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Bridge That Remembers — `item.charm.remembered_bridge`

**category:** equipment  
**summary:** Each stone holds the name of one witness who dared to speak.  
**acquisition:** {"method":"Witnesscraft mastery","requirements":["Witnesscraft mastery"]}  
**effect:** {"kind":"charm","attack":7,"armor":11,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.remembered_bridge.requirement.1` — Witnesscraft mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Second Name — `item.charm.second_name`

**category:** equipment  
**summary:** An unlived childhood folded beneath the wearer's signature.  
**acquisition:** {"method":"Borrowed Lives mastery","requirements":["Borrowed Lives mastery"]}  
**effect:** {"kind":"charm","attack":11,"armor":3,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.second_name.requirement.1` — Borrowed Lives mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Three-Deed Temper — `item.blade.three_deed`

**category:** equipment  
**summary:** Its maker can name the lives protected instead of foes slain.  
**acquisition:** {"method":"Legacy Forging mastery","requirements":["Legacy Forging mastery"]}  
**effect:** {"kind":"weapon","attack":28,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.three_deed.requirement.1` — Legacy Forging mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Common Causeway — `item.charm.common_causeway`

**category:** equipment  
**summary:** A road sworn open to everyone and owned by no one.  
**acquisition:** {"method":"Living Roads mastery","requirements":["Living Roads mastery"]}  
**effect:** {"kind":"charm","attack":5,"armor":9,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.common_causeway.requirement.1` — Living Roads mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Tomorrow's Paid Receipt — `item.weapon.future_receipt`

**category:** equipment  
**summary:** The sunrise collects its debt; this receipt admits the cost.  
**acquisition:** {"method":"Tomorrow Debt mastery","requirements":["Tomorrow Debt mastery"]}  
**effect:** {"kind":"weapon","attack":21,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.weapon.future_receipt.requirement.1` — Tomorrow Debt mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Mercy of the Hollow Keeper — `item.armor.hollow_mercy`

**category:** equipment  
**summary:** The keeper was seen at last, and chose to stand down.  
**acquisition:** {"method":"Understanding Bosses mastery","requirements":["Understanding Bosses mastery"]}  
**effect:** {"kind":"armor","attack":0,"armor":19,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.hollow_mercy.requirement.1` — Understanding Bosses mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Room Between Histories — `item.charm.between_histories`

**category:** equipment  
**summary:** Two incompatible homes shelter the same forgotten name.  
**acquisition:** {"method":"Paradox Convergence mastery","requirements":["Paradox Convergence mastery"]}  
**effect:** {"kind":"charm","attack":10,"armor":12,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.between_histories.requirement.1` — Paradox Convergence mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Unbroken Shelter Oath — `item.armor.unbroken_lantern`

**category:** equipment  
**summary:** Every witness expects the light to survive your footsteps.  
**acquisition:** {"method":"Oathbinding mastery","requirements":["Oathbinding mastery"]}  
**effect:** {"kind":"armor","attack":0,"armor":16,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.unbroken_lantern.requirement.1` — Oathbinding mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Atlas of the Missing Coast — `item.charm.coast_true`

**category:** equipment  
**summary:** The navigator kept both the rumor and the shoreline she saw.  
**acquisition:** {"method":"Cartography mastery","requirements":["Cartography mastery"]}  
**effect:** {"kind":"charm","attack":9,"armor":10,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.coast_true.requirement.1` — Cartography mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Aegis of the Last Warning — `item.armor.last_warning`

**category:** equipment  
**summary:** A bell's silence that still allows the village to hear.  
**acquisition:** {"method":"The Alarm Nobody Heard confluence","requirements":["The Alarm Nobody Heard confluence"]}  
**effect:** {"kind":"armor","attack":0,"armor":23,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.last_warning.requirement.1` — The Alarm Nobody Heard confluence. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Edge of the Elseborn Smith — `item.blade.elseborn_smith`

**category:** equipment  
**summary:** Forged by the hands you had in another life.  
**acquisition:** {"method":"A Blade for Both Your Names confluence","requirements":["A Blade for Both Your Names confluence"]}  
**effect:** {"kind":"weapon","attack":34,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.blade.elseborn_smith.requirement.1` — A Blade for Both Your Names confluence. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Chart of the Common Path — `item.charm.common_chart`

**category:** equipment  
**summary:** No border owns the choices of its travelers.  
**acquisition:** {"method":"The Route Nobody Owned confluence","requirements":["The Route Nobody Owned confluence"]}  
**effect:** {"kind":"charm","attack":12,"armor":11,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.common_chart.requirement.1` — The Route Nobody Owned confluence. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Heart of the Borrowed Dawn — `item.charm.borrowed_dawn`

**category:** equipment  
**summary:** Promises repaid without transferring the debt to others.  
**acquisition:** {"method":"Tomorrow Is Not Yours to Spend confluence","requirements":["Tomorrow Is Not Yours to Spend confluence"]}  
**effect:** {"kind":"charm","attack":14,"armor":8,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.borrowed_dawn.requirement.1` — Tomorrow Is Not Yours to Spend confluence. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Mirror of the Two Keepers — `item.armor.two_keepers`

**category:** equipment  
**summary:** The innocent and the monster both remain in its reflection.  
**acquisition:** {"method":"The Bell Fell and Stood confluence","requirements":["The Bell Fell and Stood confluence"]}  
**effect:** {"kind":"armor","attack":0,"armor":26,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.two_keepers.requirement.1` — The Bell Fell and Stood confluence. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Witness of the First Absence — `item.charm.first_absence`

**category:** equipment  
**summary:** Every world begins with someone brave enough to remember.  
**acquisition:** {"method":"The Place Before a First Story confluence","requirements":["The Place Before a First Story confluence"]}  
**effect:** {"kind":"charm","attack":20,"armor":17,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.first_absence.requirement.1` — The Place Before a First Story confluence. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Ward of the Second Night — `item.armor.afterlight_ward`

**category:** equipment  
**summary:** Hessa hangs a light for each unregistered child. This protection cannot be bought.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"armor","attack":0,"armor":18,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.armor.afterlight_ward.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Lantern of Unredacted Names — `item.charm.unredacted_lantern`

**category:** equipment  
**summary:** Ivera remembers the families officials denied. Its glow never removes a name.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"charm","attack":11,"armor":9,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {}  
**duration:** while equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.charm.unredacted_lantern.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## CONSUMABLE (5)

### Hearth Salve — `item.elixir.hearth`

**category:** consumable  
**summary:** A small warmth stolen from the coming winter.  
**acquisition:** {"method":"starter/craft","requirements":["starter/craft"]}  
**effect:** {"kind":"consumable","attack":0,"armor":0,"healing":35,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"quantity":1}  
**duration:** once when consumed  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.elixir.hearth.requirement.1` — starter/craft. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Stillness Vial — `item.elixir.strain`

**category:** consumable  
**summary:** Four breaths free of the weight of possibility.  
**acquisition:** {"method":"starter/craft","requirements":["starter/craft"]}  
**effect:** {"kind":"consumable","attack":0,"armor":0,"healing":0,"strain_recovery":24,"distinct_effects":[]}  
**cost:** {"quantity":1}  
**duration:** once when consumed  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.elixir.strain.requirement.1` — starter/craft. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Nightwatch Tonic — `item.elixir.nightwatch`

**category:** consumable  
**summary:** Wakeful enough to see the next possibility.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"consumable","attack":0,"armor":0,"healing":65,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"quantity":1}  
**duration:** once when consumed  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.elixir.nightwatch.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Rootbound Poultice — `item.elixir.rootbound`

**category:** consumable  
**summary:** The orchard refuses to let you fade.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"consumable","attack":0,"armor":0,"healing":90,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"quantity":1}  
**duration:** once when consumed  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.elixir.rootbound.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Ash of Possible Lives — `item.elixir.possible_lives`

**category:** consumable  
**summary:** Once, you had the strength to be all of them.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"consumable","attack":0,"armor":0,"healing":100,"strain_recovery":60,"distinct_effects":[]}  
**cost:** {"quantity":1}  
**duration:** once when consumed  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.elixir.possible_lives.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## CRAFT MATERIAL (7)

### Echo Glass — `item.mat.echo_glass`

**category:** craft_material  
**summary:** The well reflects what has not yet happened.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"material","attack":0,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"crafting_ingredient":true}  
**duration:** no direct effect  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Ingredient only; does not itself grant a player skill  
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.mat.echo_glass.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Hourless Bellmetal — `item.mat.bell_metal`

**category:** craft_material  
**summary:** A sound that cannot fit inside one hour.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"material","attack":0,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"crafting_ingredient":true}  
**duration:** no direct effect  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Ingredient only; does not itself grant a player skill  
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.mat.bell_metal.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Unwritten Ink — `item.mat.archive_ink`

**category:** craft_material  
**summary:** An oath not yet made, waiting to dry.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"material","attack":0,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"crafting_ingredient":true}  
**duration:** no direct effect  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Ingredient only; does not itself grant a player skill  
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.mat.archive_ink.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Rift Seed — `item.mat.rift_seed`

**category:** craft_material  
**summary:** A beginning without the usual beginning.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"material","attack":0,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"crafting_ingredient":true}  
**duration:** no direct effect  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Ingredient only; does not itself grant a player skill  
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.mat.rift_seed.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Weathered Iron — `item.mat.iron_scrap`

**category:** craft_material  
**summary:** The ore forgot which mountain it belonged to.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"material","attack":0,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"crafting_ingredient":true}  
**duration:** no direct effect  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Ingredient only; does not itself grant a player skill  
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.mat.iron_scrap.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Night-Mint — `item.mat.wild_herbs`

**category:** craft_material  
**summary:** Harvested where the rain refuses to fall.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"material","attack":0,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"crafting_ingredient":true}  
**duration:** no direct effect  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Ingredient only; does not itself grant a player skill  
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.mat.wild_herbs.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Blank Witness Parchment — `item.mat.blank_parchment`

**category:** craft_material  
**summary:** It accepts testimony that no one will sign.  
**acquisition:** {"method":"registered; particular acquisition not audited","requirements":[]}  
**effect:** {"kind":"material","attack":0,"armor":0,"healing":0,"strain_recovery":0,"distinct_effects":[]}  
**cost:** {"crafting_ingredient":true}  
**duration:** no direct effect  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Ingredient only; does not itself grant a player skill  
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `item.mat.blank_parchment.requirement.1` — Confirm legitimate acquisition and its gameplay effect in cited source before treating this as a collectible ability. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## COMBAT BUILD (3)

### Duelist — `build.duelist`

**category:** combat_build  
**summary:** Earned and freely reselectable combat specialization with transparent tradeoffs  
**acquisition:** {"method":"insight","requirements":["Complete a regional optional wing","Spend 2 Insight first unlock, 1 to change type"]}  
**effect:** {"kind":"build_passive","stats":{"attack":5,"armor":-2,"stamina_cost_multiplier":1.2}}  
**cost:** {"insight_first":2,"insight_switch":1}  
**duration:** owned permanently, active while selected  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:** Crafted binding adds +2 armor  
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `build.duelist.requirement.1` — Complete a regional optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `build.duelist.requirement.2` — Spend 2 Insight first unlock, 1 to change type. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Warden — `build.warden`

**category:** combat_build  
**summary:** Earned and freely reselectable combat specialization with transparent tradeoffs  
**acquisition:** {"method":"insight","requirements":["Complete a regional optional wing","Spend 2 Insight first unlock, 1 to change type"]}  
**effect:** {"kind":"build_passive","stats":{"attack":-2,"armor":5,"movement_speed_multiplier":0.9}}  
**cost:** {"insight_first":2,"insight_switch":1}  
**duration:** owned permanently, active while selected  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:** Crafted binding adds +2 armor  
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `build.warden.requirement.1` — Complete a regional optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `build.warden.requirement.2` — Spend 2 Insight first unlock, 1 to change type. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Pathfinder — `build.pathfinder`

**category:** combat_build  
**summary:** Earned and freely reselectable combat specialization with transparent tradeoffs  
**acquisition:** {"method":"insight","requirements":["Complete a regional optional wing","Spend 2 Insight first unlock, 1 to change type"]}  
**effect:** {"kind":"build_passive","stats":{"movement_speed_multiplier":1.08,"attack_reach_multiplier":1.1,"stamina_cost_multiplier":1.1}}  
**cost:** {"insight_first":2,"insight_switch":1}  
**duration:** owned permanently, active while selected  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:** Crafted binding adds +2 armor  
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `build.pathfinder.requirement.1` — Complete a regional optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `build.pathfinder.requirement.2` — Spend 2 Insight first unlock, 1 to change type. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## FIELD TECHNIQUE (9)

### Marked Opening — `technique.realm.0`

**category:** field_technique  
**summary:** Expose nearby enemy with duration scaled by mastery  
**acquisition:** {"method":"training","requirements":["Resolve optional region content","Earn matching active mastery"]}  
**effect:** {"kind":"active_combat","detail":"Expose nearby enemy with duration scaled by mastery","passive":{"attack":3,"armor":0,"glimpse_discount":3,"fold_extension":0}}  
**cost:** {"stamina":30,"cooldown_seconds":12}  
**duration:** active technique: temporary; learned: permanent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Some targets require line of sight, active guarding or missing health  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `technique.realm.0.requirement.1` — Resolve optional region content. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `technique.realm.0.requirement.2` — Earn matching active mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Pressure Read — `technique.realm.1`

**category:** field_technique  
**summary:** Apply Slow to a nearby enemy  
**acquisition:** {"method":"training","requirements":["Resolve optional region content","Earn matching active mastery"]}  
**effect:** {"kind":"active_combat","detail":"Apply Slow to a nearby enemy","passive":{"attack":0,"armor":2,"glimpse_discount":2,"fold_extension":0}}  
**cost:** {"stamina":30,"cooldown_seconds":12}  
**duration:** active technique: temporary; learned: permanent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Some targets require line of sight, active guarding or missing health  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `technique.realm.1.requirement.1` — Resolve optional region content. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `technique.realm.1.requirement.2` — Earn matching active mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Ember Counter — `technique.realm.2`

**category:** field_technique  
**summary:** Apply Burn to a nearby enemy  
**acquisition:** {"method":"training","requirements":["Resolve optional region content","Earn matching active mastery"]}  
**effect:** {"kind":"active_combat","detail":"Apply Burn to a nearby enemy","passive":{"attack":2,"armor":2,"glimpse_discount":0,"fold_extension":1}}  
**cost:** {"stamina":30,"cooldown_seconds":12}  
**duration:** active technique: temporary; learned: permanent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Some targets require line of sight, active guarding or missing health  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `technique.realm.2.requirement.1` — Resolve optional region content. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `technique.realm.2.requirement.2` — Earn matching active mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Oath Interrupt — `technique.realm.3`

**category:** field_technique  
**summary:** Apply Weaken to a nearby enemy  
**acquisition:** {"method":"training","requirements":["Resolve optional region content","Earn matching active mastery"]}  
**effect:** {"kind":"active_combat","detail":"Apply Weaken to a nearby enemy","passive":{"attack":1,"armor":1,"glimpse_discount":1,"fold_extension":1}}  
**cost:** {"stamina":30,"cooldown_seconds":12}  
**duration:** active technique: temporary; learned: permanent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Some targets require line of sight, active guarding or missing health  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `technique.realm.3.requirement.1` — Resolve optional region content. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `technique.realm.3.requirement.2` — Earn matching active mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Grounded Fold — `technique.realm.4`

**category:** field_technique  
**summary:** Fold expose target temporarily  
**acquisition:** {"method":"training","requirements":["Resolve optional region content","Earn matching active mastery"]}  
**effect:** {"kind":"active_combat","detail":"Fold expose target temporarily","passive":{"attack":0,"armor":1,"glimpse_discount":0,"fold_extension":2}}  
**cost:** {"stamina":30,"cooldown_seconds":12}  
**duration:** active technique: temporary; learned: permanent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Some targets require line of sight, active guarding or missing health  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `technique.realm.4.requirement.1` — Resolve optional region content. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `technique.realm.4.requirement.2` — Earn matching active mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Seam Read — `technique.realm.5`

**category:** field_technique  
**summary:** Expose and stagger nearby foe  
**acquisition:** {"method":"training","requirements":["Resolve optional region content","Earn matching active mastery"]}  
**effect:** {"kind":"active_combat","detail":"Expose and stagger nearby foe","passive":{"attack":2,"armor":0,"glimpse_discount":2,"fold_extension":0}}  
**cost:** {"stamina":30,"cooldown_seconds":12}  
**duration:** active technique: temporary; learned: permanent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Some targets require line of sight, active guarding or missing health  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `technique.realm.5.requirement.1` — Resolve optional region content. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `technique.realm.5.requirement.2` — Earn matching active mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Unlisted Step — `technique.realm.6`

**category:** field_technique  
**summary:** Dash forward and gain brief evasion  
**acquisition:** {"method":"training","requirements":["Resolve optional region content","Earn matching active mastery"]}  
**effect:** {"kind":"active_combat","detail":"Dash forward and gain brief evasion","passive":{"attack":0,"armor":2,"glimpse_discount":3,"fold_extension":0}}  
**cost:** {"stamina":30,"cooldown_seconds":12}  
**duration:** active technique: temporary; learned: permanent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Some targets require line of sight, active guarding or missing health  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `technique.realm.6.requirement.1` — Resolve optional region content. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `technique.realm.6.requirement.2` — Earn matching active mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Root Counter — `technique.realm.7`

**category:** field_technique  
**summary:** While guarding and injured, restore 20 health  
**acquisition:** {"method":"training","requirements":["Resolve optional region content","Earn matching active mastery"]}  
**effect:** {"kind":"active_combat","detail":"While guarding and injured, restore 20 health","passive":{"attack":1,"armor":3,"glimpse_discount":0,"fold_extension":1}}  
**cost:** {"stamina":30,"cooldown_seconds":12}  
**duration:** active technique: temporary; learned: permanent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Some targets require line of sight, active guarding or missing health  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `technique.realm.7.requirement.1` — Resolve optional region content. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `technique.realm.7.requirement.2` — Earn matching active mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Nearness Hold — `technique.realm.8`

**category:** field_technique  
**summary:** Grant temporary protective Ward  
**acquisition:** {"method":"training","requirements":["Resolve optional region content","Earn matching active mastery"]}  
**effect:** {"kind":"active_combat","detail":"Grant temporary protective Ward","passive":{"attack":0,"armor":3,"glimpse_discount":1,"fold_extension":2}}  
**cost:** {"stamina":30,"cooldown_seconds":12}  
**duration:** active technique: temporary; learned: permanent  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Some targets require line of sight, active guarding or missing health  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `technique.realm.8.requirement.1` — Resolve optional region content. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `technique.realm.8.requirement.2` — Earn matching active mastery. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## WEAPON STYLE (9)

### Mileglass duelist — `weapon_style.realm.0`

**category:** weapon_style  
**summary:** Region weapon family with distinctive tempo, range and applied combat status  
**acquisition:** {"method":"gear_equip","requirements":["Earn region weapon Mileglass Edge"]}  
**effect:** {"kind":"weapon_attack","reach_cm":300,"rhythm_seconds":0.6,"stamina":14,"damage_multiplier":1,"status":"Expose"}  
**cost:** {"stamina_per_attack":14}  
**duration:** while selected weapon is equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `weapon_style.realm.0.requirement.1` — Earn region weapon Mileglass Edge. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Breakwater spear — `weapon_style.realm.1`

**category:** weapon_style  
**summary:** Region weapon family with distinctive tempo, range and applied combat status  
**acquisition:** {"method":"gear_equip","requirements":["Earn region weapon Breakwater Spear"]}  
**effect:** {"kind":"weapon_attack","reach_cm":380,"rhythm_seconds":0.85,"stamina":18,"damage_multiplier":1,"status":"Slow"}  
**cost:** {"stamina_per_attack":18}  
**duration:** while selected weapon is equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `weapon_style.realm.1.requirement.1` — Earn region weapon Breakwater Spear. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Kiln hammer — `weapon_style.realm.2`

**category:** weapon_style  
**summary:** Region weapon family with distinctive tempo, range and applied combat status  
**acquisition:** {"method":"gear_equip","requirements":["Earn region weapon Kilnfang Maul"]}  
**effect:** {"kind":"weapon_attack","reach_cm":250,"rhythm_seconds":1.15,"stamina":24,"damage_multiplier":1.3,"status":"Burn"}  
**cost:** {"stamina_per_attack":24}  
**duration:** while selected weapon is equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `weapon_style.realm.2.requirement.1` — Earn region weapon Kilnfang Maul. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Undertide hook — `weapon_style.realm.3`

**category:** weapon_style  
**summary:** Region weapon family with distinctive tempo, range and applied combat status  
**acquisition:** {"method":"gear_equip","requirements":["Earn region weapon Undertide Hook"]}  
**effect:** {"kind":"weapon_attack","reach_cm":320,"rhythm_seconds":0.75,"stamina":17,"damage_multiplier":1,"status":"Weaken"}  
**cost:** {"stamina_per_attack":17}  
**duration:** while selected weapon is equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `weapon_style.realm.3.requirement.1` — Earn region weapon Undertide Hook. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Downfall glaive — `weapon_style.realm.4`

**category:** weapon_style  
**summary:** Region weapon family with distinctive tempo, range and applied combat status  
**acquisition:** {"method":"gear_equip","requirements":["Earn region weapon Downfall Glaive"]}  
**effect:** {"kind":"weapon_attack","reach_cm":420,"rhythm_seconds":0.95,"stamina":20,"damage_multiplier":1.1,"status":"Expose"}  
**cost:** {"stamina_per_attack":20}  
**duration:** while selected weapon is equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `weapon_style.realm.4.requirement.1` — Earn region weapon Downfall Glaive. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Seam pickblade — `weapon_style.realm.5`

**category:** weapon_style  
**summary:** Region weapon family with distinctive tempo, range and applied combat status  
**acquisition:** {"method":"gear_equip","requirements":["Earn region weapon Yesterday Pickblade"]}  
**effect:** {"kind":"weapon_attack","reach_cm":260,"rhythm_seconds":0.9,"stamina":21,"damage_multiplier":1.15,"status":"Expose"}  
**cost:** {"stamina_per_attack":21}  
**duration:** while selected weapon is equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `weapon_style.realm.5.requirement.1` — Earn region weapon Yesterday Pickblade. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Elseborn saber — `weapon_style.realm.6`

**category:** weapon_style  
**summary:** Region weapon family with distinctive tempo, range and applied combat status  
**acquisition:** {"method":"gear_equip","requirements":["Earn region weapon Elseborn Saber"]}  
**effect:** {"kind":"weapon_attack","reach_cm":235,"rhythm_seconds":0.45,"stamina":11,"damage_multiplier":0.85,"status":"Slow"}  
**cost:** {"stamina_per_attack":11}  
**duration:** while selected weapon is equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `weapon_style.realm.6.requirement.1` — Earn region weapon Elseborn Saber. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Crownroot falchion — `weapon_style.realm.7`

**category:** weapon_style  
**summary:** Region weapon family with distinctive tempo, range and applied combat status  
**acquisition:** {"method":"gear_equip","requirements":["Earn region weapon Crownroot Falchion"]}  
**effect:** {"kind":"weapon_attack","reach_cm":290,"rhythm_seconds":0.8,"stamina":18,"damage_multiplier":1.05,"status":"Weaken"}  
**cost:** {"stamina_per_attack":18}  
**duration:** while selected weapon is equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `weapon_style.realm.7.requirement.1` — Earn region weapon Crownroot Falchion. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Nearness blade — `weapon_style.realm.8`

**category:** weapon_style  
**summary:** Region weapon family with distinctive tempo, range and applied combat status  
**acquisition:** {"method":"gear_equip","requirements":["Earn region weapon Nearness Blade"]}  
**effect:** {"kind":"weapon_attack","reach_cm":300,"rhythm_seconds":0.7,"stamina":16,"damage_multiplier":1,"status":"Expose"}  
**cost:** {"stamina_per_attack":16}  
**duration:** while selected weapon is equipped  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `weapon_style.realm.8.requirement.1` — Earn region weapon Nearness Blade. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## PROFESSION (3)

### Smith — `profession.smith`

**category:** profession  
**summary:** Skill levels earned by real crafting; grants eligibility to advanced recipes  
**acquisition:** {"method":"practice","requirements":["Valid available workbench, materials and recipe"]}  
**effect:** {"kind":"crafting_skill","cap":30}  
**cost:** {"ingredients":"per recipe"}  
**duration:** saved progression  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `profession.smith.requirement.1` — Valid available workbench, materials and recipe. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Apothecary — `profession.apothecary`

**category:** profession  
**summary:** Skill levels earned by real crafting; grants eligibility to advanced recipes  
**acquisition:** {"method":"practice","requirements":["Valid available workbench, materials and recipe"]}  
**effect:** {"kind":"crafting_skill","cap":30}  
**cost:** {"ingredients":"per recipe"}  
**duration:** saved progression  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `profession.apothecary.requirement.1` — Valid available workbench, materials and recipe. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Scribe — `profession.scribe`

**category:** profession  
**summary:** Skill levels earned by real crafting; grants eligibility to advanced recipes  
**acquisition:** {"method":"practice","requirements":["Valid available workbench, materials and recipe"]}  
**effect:** {"kind":"crafting_skill","cap":30}  
**cost:** {"ingredients":"per recipe"}  
**duration:** saved progression  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `profession.scribe.requirement.1` — Valid available workbench, materials and recipe. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## CRAFTING RECIPE (6)

### The Oath-Tempered Edge — `recipe.workshop.TemperedEdge`

**category:** crafting_recipe  
**summary:** Create Oath-Tempered Edge  
**acquisition:** {"method":"workshop_skill","requirements":["Requires Smith skill 0","Must use Bellwold workshop","3 x IronScrap owned and spendable","1 x BellMetal owned and spendable","Sufficient space for 1 x TemperedEdge"]}  
**effect:** {"kind":"craft","output":"blade.tempered","quantity":1}  
**cost:** {"materials":[{"item":"mat.iron_scrap","quantity":3},{"item":"mat.bell_metal","quantity":1}]}  
**duration:** recipe known while prerequisites met  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**depends on:** profession.smith; settlement.Bellwold  
**rule contract:** {"profession":"profession.smith","workshop":"settlement.Bellwold","min_skill":0,"inputs":[{"item_enum":"IronScrap","count":3},{"item_enum":"BellMetal","count":1}],"output":{"item_enum":"TemperedEdge","count":1},"transaction_atomic":true}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.workshop.TemperedEdge.requirement.1` — Requires Smith skill 0. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.TemperedEdge.requirement.2` — Must use Bellwold workshop. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.TemperedEdge.requirement.3` — 3 x IronScrap owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.TemperedEdge.requirement.4` — 1 x BellMetal owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.TemperedEdge.requirement.5` — Sufficient space for 1 x TemperedEdge. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only

### Hearth Salves — `recipe.workshop.HerbSalve`

**category:** crafting_recipe  
**summary:** Create Hearth Salve  
**acquisition:** {"method":"workshop_skill","requirements":["Requires Apothecary skill 0","Must use Bellwold workshop","2 x WildHerbs owned and spendable","1 x IronScrap owned and spendable","Sufficient space for 2 x HearthSalve"]}  
**effect:** {"kind":"craft","output":"elixir.hearth","quantity":2}  
**cost:** {"materials":[{"item":"mat.wild_herbs","quantity":2},{"item":"mat.iron_scrap","quantity":1}]}  
**duration:** recipe known while prerequisites met  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**depends on:** profession.apothecary; settlement.Bellwold  
**rule contract:** {"profession":"profession.apothecary","workshop":"settlement.Bellwold","min_skill":0,"inputs":[{"item_enum":"WildHerbs","count":2},{"item_enum":"IronScrap","count":1}],"output":{"item_enum":"HearthSalve","count":2},"transaction_atomic":true}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.workshop.HerbSalve.requirement.1` — Requires Apothecary skill 0. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.HerbSalve.requirement.2` — Must use Bellwold workshop. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.HerbSalve.requirement.3` — 2 x WildHerbs owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.HerbSalve.requirement.4` — 1 x IronScrap owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.HerbSalve.requirement.5` — Sufficient space for 2 x HearthSalve. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only

### Inscribed Stillness — `recipe.workshop.ScribedStillness`

**category:** crafting_recipe  
**summary:** Create Stillness Vial  
**acquisition:** {"method":"workshop_skill","requirements":["Requires Scribe skill 0","Must use Paperhaven workshop","2 x BlankParchment owned and spendable","1 x WildHerbs owned and spendable","Sufficient space for 2 x StrainVial"]}  
**effect:** {"kind":"craft","output":"elixir.strain","quantity":2}  
**cost:** {"materials":[{"item":"mat.blank_parchment","quantity":2},{"item":"mat.wild_herbs","quantity":1}]}  
**duration:** recipe known while prerequisites met  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**depends on:** profession.scribe; settlement.Paperhaven  
**rule contract:** {"profession":"profession.scribe","workshop":"settlement.Paperhaven","min_skill":0,"inputs":[{"item_enum":"BlankParchment","count":2},{"item_enum":"WildHerbs","count":1}],"output":{"item_enum":"StrainVial","count":2},"transaction_atomic":true}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.workshop.ScribedStillness.requirement.1` — Requires Scribe skill 0. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.ScribedStillness.requirement.2` — Must use Paperhaven workshop. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.ScribedStillness.requirement.3` — 2 x BlankParchment owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.ScribedStillness.requirement.4` — 1 x WildHerbs owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.ScribedStillness.requirement.5` — Sufficient space for 2 x StrainVial. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only

### Lanternwoven Mail — `recipe.workshop.LanternMail`

**category:** crafting_recipe  
**summary:** Create Lanternwoven Mail  
**acquisition:** {"method":"workshop_skill","requirements":["Requires Smith skill 2","Must use Bellwold workshop","6 x IronScrap owned and spendable","1 x BellMetal owned and spendable","Sufficient space for 1 x LanternMail"]}  
**effect:** {"kind":"craft","output":"armor.lanternmail","quantity":1}  
**cost:** {"materials":[{"item":"mat.iron_scrap","quantity":6},{"item":"mat.bell_metal","quantity":1}]}  
**duration:** recipe known while prerequisites met  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**depends on:** profession.smith; settlement.Bellwold  
**rule contract:** {"profession":"profession.smith","workshop":"settlement.Bellwold","min_skill":2,"inputs":[{"item_enum":"IronScrap","count":6},{"item_enum":"BellMetal","count":1}],"output":{"item_enum":"LanternMail","count":1},"transaction_atomic":true}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.workshop.LanternMail.requirement.1` — Requires Smith skill 2. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.LanternMail.requirement.2` — Must use Bellwold workshop. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.LanternMail.requirement.3` — 6 x IronScrap owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.LanternMail.requirement.4` — 1 x BellMetal owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.LanternMail.requirement.5` — Sufficient space for 1 x LanternMail. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only

### Seal of Kept Testimony — `recipe.workshop.InkboundCharm`

**category:** crafting_recipe  
**summary:** Create Seal of Kept Testimony  
**acquisition:** {"method":"workshop_skill","requirements":["Requires Scribe skill 2","Must use Paperhaven workshop","5 x BlankParchment owned and spendable","1 x ArchiveInk owned and spendable","Sufficient space for 1 x InkboundCharm"]}  
**effect:** {"kind":"craft","output":"charm.inkbound","quantity":1}  
**cost:** {"materials":[{"item":"mat.blank_parchment","quantity":5},{"item":"mat.archive_ink","quantity":1}]}  
**duration:** recipe known while prerequisites met  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**depends on:** profession.scribe; settlement.Paperhaven  
**rule contract:** {"profession":"profession.scribe","workshop":"settlement.Paperhaven","min_skill":2,"inputs":[{"item_enum":"BlankParchment","count":5},{"item_enum":"ArchiveInk","count":1}],"output":{"item_enum":"InkboundCharm","count":1},"transaction_atomic":true}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.workshop.InkboundCharm.requirement.1` — Requires Scribe skill 2. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.InkboundCharm.requirement.2` — Must use Paperhaven workshop. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.InkboundCharm.requirement.3` — 5 x BlankParchment owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.InkboundCharm.requirement.4` — 1 x ArchiveInk owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.InkboundCharm.requirement.5` — Sufficient space for 1 x InkboundCharm. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only

### Ash of Possible Lives — `recipe.workshop.LifeAsh`

**category:** crafting_recipe  
**summary:** Create Ash of Possible Lives  
**acquisition:** {"method":"workshop_skill","requirements":["Requires Apothecary skill 2","Must use Bellwold workshop","1 x RiftSeed owned and spendable","1 x RootboundPoultice owned and spendable","Sufficient space for 1 x AshOfPossibleLives"]}  
**effect:** {"kind":"craft","output":"elixir.possible_lives","quantity":1}  
**cost:** {"materials":[{"item":"mat.rift_seed","quantity":1},{"item":"elixir.rootbound","quantity":1}]}  
**duration:** recipe known while prerequisites met  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**depends on:** profession.apothecary; settlement.Bellwold  
**rule contract:** {"profession":"profession.apothecary","workshop":"settlement.Bellwold","min_skill":2,"inputs":[{"item_enum":"RiftSeed","count":1},{"item_enum":"RootboundPoultice","count":1}],"output":{"item_enum":"AshOfPossibleLives","count":1},"transaction_atomic":true}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeCraftEconomyRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.workshop.LifeAsh.requirement.1` — Requires Apothecary skill 2. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.LifeAsh.requirement.2` — Must use Bellwold workshop. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.workshop.LifeAsh.requirement.3` — 1 x RiftSeed owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.LifeAsh.requirement.4` — 1 x RootboundPoultice owned and spendable. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only
- [ ] `recipe.workshop.LifeAsh.requirement.5` — Sufficient space for 1 x AshOfPossibleLives. Evidence: real inventory and atomic crafting transaction; state: evaluated_in_game_only

## FIELD RECIPE (9)

### Surveyor's Stillness — `recipe.field.realm.0`

**category:** field_recipe  
**summary:** Craft Stillness Vial  
**acquisition:** {"method":"wing_unlock","requirements":["Complete matching optional wing","Craft at local open workbench"]}  
**effect:** {"kind":"craft","output":"elixir.strain","quantity":1}  
**cost:** {"materials":[{"item":"mat.blank_parchment","quantity":2},{"item":"mat.wild_herbs","quantity":1}]}  
**duration:** permanent recipe unlock  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h; Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.field.realm.0.requirement.1` — Complete matching optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.field.realm.0.requirement.2` — Craft at local open workbench. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Drywake Dressing — `recipe.field.realm.1`

**category:** field_recipe  
**summary:** Craft Hearth Salve  
**acquisition:** {"method":"wing_unlock","requirements":["Complete matching optional wing","Craft at local open workbench"]}  
**effect:** {"kind":"craft","output":"elixir.hearth","quantity":2}  
**cost:** {"materials":[{"item":"mat.wild_herbs","quantity":2},{"item":"mat.blank_parchment","quantity":1}]}  
**duration:** permanent recipe unlock  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h; Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.field.realm.1.requirement.1` — Complete matching optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.field.realm.1.requirement.2` — Craft at local open workbench. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Cooling Forge Edge — `recipe.field.realm.2`

**category:** field_recipe  
**summary:** Craft Oath-Tempered Edge  
**acquisition:** {"method":"wing_unlock","requirements":["Complete matching optional wing","Craft at local open workbench"]}  
**effect:** {"kind":"craft","output":"blade.tempered","quantity":1}  
**cost:** {"materials":[{"item":"mat.iron_scrap","quantity":3},{"item":"mat.bell_metal","quantity":1}]}  
**duration:** permanent recipe unlock  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h; Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.field.realm.2.requirement.1` — Complete matching optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.field.realm.2.requirement.2` — Craft at local open workbench. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Unpledged Ink — `recipe.field.realm.3`

**category:** field_recipe  
**summary:** Craft Stillness Vial  
**acquisition:** {"method":"wing_unlock","requirements":["Complete matching optional wing","Craft at local open workbench"]}  
**effect:** {"kind":"craft","output":"elixir.strain","quantity":2}  
**cost:** {"materials":[{"item":"mat.blank_parchment","quantity":2},{"item":"mat.archive_ink","quantity":1}]}  
**duration:** permanent recipe unlock  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h; Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.field.realm.3.requirement.1` — Complete matching optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.field.realm.3.requirement.2` — Craft at local open workbench. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Choir Rest Tonic — `recipe.field.realm.4`

**category:** field_recipe  
**summary:** Craft Nightwatch Tonic  
**acquisition:** {"method":"wing_unlock","requirements":["Complete matching optional wing","Craft at local open workbench"]}  
**effect:** {"kind":"craft","output":"elixir.nightwatch","quantity":1}  
**cost:** {"materials":[{"item":"mat.wild_herbs","quantity":3},{"item":"mat.blank_parchment","quantity":1}]}  
**duration:** permanent recipe unlock  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h; Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.field.realm.4.requirement.1` — Complete matching optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.field.realm.4.requirement.2` — Craft at local open workbench. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Seamkeeper Mail — `recipe.field.realm.5`

**category:** field_recipe  
**summary:** Craft Lanternwoven Mail  
**acquisition:** {"method":"wing_unlock","requirements":["Complete matching optional wing","Craft at local open workbench"]}  
**effect:** {"kind":"craft","output":"armor.lanternmail","quantity":1}  
**cost:** {"materials":[{"item":"mat.iron_scrap","quantity":6},{"item":"mat.bell_metal","quantity":1}]}  
**duration:** permanent recipe unlock  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h; Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.field.realm.5.requirement.1` — Complete matching optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.field.realm.5.requirement.2` — Craft at local open workbench. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Quiet Witness Seal — `recipe.field.realm.6`

**category:** field_recipe  
**summary:** Craft Seal of Kept Testimony  
**acquisition:** {"method":"wing_unlock","requirements":["Complete matching optional wing","Craft at local open workbench"]}  
**effect:** {"kind":"craft","output":"charm.inkbound","quantity":1}  
**cost:** {"materials":[{"item":"mat.blank_parchment","quantity":5},{"item":"mat.archive_ink","quantity":1}]}  
**duration:** permanent recipe unlock  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h; Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.field.realm.6.requirement.1` — Complete matching optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.field.realm.6.requirement.2` — Craft at local open workbench. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Nursery Poultice — `recipe.field.realm.7`

**category:** field_recipe  
**summary:** Craft Rootbound Poultice  
**acquisition:** {"method":"wing_unlock","requirements":["Complete matching optional wing","Craft at local open workbench"]}  
**effect:** {"kind":"craft","output":"elixir.rootbound","quantity":1}  
**cost:** {"materials":[{"item":"mat.wild_herbs","quantity":3},{"item":"mat.rift_seed","quantity":1}]}  
**duration:** permanent recipe unlock  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h; Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.field.realm.7.requirement.1` — Complete matching optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.field.realm.7.requirement.2` — Craft at local open workbench. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Threshold Ash — `recipe.field.realm.8`

**category:** field_recipe  
**summary:** Craft Ash of Possible Lives  
**acquisition:** {"method":"wing_unlock","requirements":["Complete matching optional wing","Craft at local open workbench"]}  
**effect:** {"kind":"craft","output":"elixir.possible_lives","quantity":1}  
**cost:** {"materials":[{"item":"mat.rift_seed","quantity":1},{"item":"elixir.rootbound","quantity":1}]}  
**duration:** permanent recipe unlock  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/Items/UnmadeFieldCraftRules.h; Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp  

**Earned requirement checklist (evaluate in live state)**

- [ ] `recipe.field.realm.8.requirement.1` — Complete matching optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `recipe.field.realm.8.requirement.2` — Craft at local open workbench. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## LOCAL PRIVILEGE (9)

### Lantern bearings — `service.realm.0`

**category:** local_privilege  
**summary:** Adds verified map/bearing knowledge  
**acquisition:** {"method":"earned_settlement_access","requirements":["Complete optional wing","Spend Insight on privilege","Attend open service location"]}  
**effect:** {"kind":"local_service","detail":"Adds verified map/bearing knowledge"}  
**cost:** {"max_total_uses":2,"per_game_day":1}  
**duration:** local service outcome  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Not a portable character spell  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `service.realm.0.requirement.1` — Complete optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.0.requirement.2` — Spend Insight on privilege. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.0.requirement.3` — Attend open service location. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Salt purification — `service.realm.1`

**category:** local_privilege  
**summary:** Restores 20 Reality Strain  
**acquisition:** {"method":"earned_settlement_access","requirements":["Complete optional wing","Spend Insight on privilege","Attend open service location"]}  
**effect:** {"kind":"local_service","detail":"Restores 20 Reality Strain"}  
**cost:** {"max_total_uses":2,"per_game_day":1}  
**duration:** local service outcome  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Not a portable character spell  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `service.realm.1.requirement.1` — Complete optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.1.requirement.2` — Spend Insight on privilege. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.1.requirement.3` — Attend open service location. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Heat tempering — `service.realm.2`

**category:** local_privilege  
**summary:** Temporarily increases pressure resistance  
**acquisition:** {"method":"earned_settlement_access","requirements":["Complete optional wing","Spend Insight on privilege","Attend open service location"]}  
**effect:** {"kind":"local_service","detail":"Temporarily increases pressure resistance"}  
**cost:** {"max_total_uses":2,"per_game_day":1}  
**duration:** local service outcome  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Not a portable character spell  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `service.realm.2.requirement.1` — Complete optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.2.requirement.2` — Spend Insight on privilege. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.2.requirement.3` — Attend open service location. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Mediation — `service.realm.3`

**category:** local_privilege  
**summary:** Initiates a local mediation event  
**acquisition:** {"method":"earned_settlement_access","requirements":["Complete optional wing","Spend Insight on privilege","Attend open service location"]}  
**effect:** {"kind":"local_service","detail":"Initiates a local mediation event"}  
**cost:** {"max_total_uses":2,"per_game_day":1}  
**duration:** local service outcome  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Not a portable character spell  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `service.realm.3.requirement.1` — Complete optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.3.requirement.2` — Spend Insight on privilege. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.3.requirement.3` — Attend open service location. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Choir tuning — `service.realm.4`

**category:** local_privilege  
**summary:** Grants one second of evasion  
**acquisition:** {"method":"earned_settlement_access","requirements":["Complete optional wing","Spend Insight on privilege","Attend open service location"]}  
**effect:** {"kind":"local_service","detail":"Grants one second of evasion"}  
**cost:** {"max_total_uses":2,"per_game_day":1}  
**duration:** local service outcome  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Not a portable character spell  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `service.realm.4.requirement.1` — Complete optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.4.requirement.2` — Spend Insight on privilege. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.4.requirement.3` — Attend open service location. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Memory recovery — `service.realm.5`

**category:** local_privilege  
**summary:** Clears temporary combat statuses  
**acquisition:** {"method":"earned_settlement_access","requirements":["Complete optional wing","Spend Insight on privilege","Attend open service location"]}  
**effect:** {"kind":"local_service","detail":"Clears temporary combat statuses"}  
**cost:** {"max_total_uses":2,"per_game_day":1}  
**duration:** local service outcome  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Not a portable character spell  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `service.realm.5.requirement.1` — Complete optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.5.requirement.2` — Spend Insight on privilege. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.5.requirement.3` — Attend open service location. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Quiet indexing — `service.realm.6`

**category:** local_privilege  
**summary:** Grants temporary Quiet status  
**acquisition:** {"method":"earned_settlement_access","requirements":["Complete optional wing","Spend Insight on privilege","Attend open service location"]}  
**effect:** {"kind":"local_service","detail":"Grants temporary Quiet status"}  
**cost:** {"max_total_uses":2,"per_game_day":1}  
**duration:** local service outcome  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Not a portable character spell  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `service.realm.6.requirement.1` — Complete optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.6.requirement.2` — Spend Insight on privilege. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.6.requirement.3` — Attend open service location. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Living rootwork — `service.realm.7`

**category:** local_privilege  
**summary:** Heals 25 missing health  
**acquisition:** {"method":"earned_settlement_access","requirements":["Complete optional wing","Spend Insight on privilege","Attend open service location"]}  
**effect:** {"kind":"local_service","detail":"Heals 25 missing health"}  
**cost:** {"max_total_uses":2,"per_game_day":1}  
**duration:** local service outcome  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Not a portable character spell  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `service.realm.7.requirement.1` — Complete optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.7.requirement.2` — Spend Insight on privilege. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.7.requirement.3` — Attend open service location. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Boundary guidance — `service.realm.8`

**category:** local_privilege  
**summary:** Grants temporary Ward  
**acquisition:** {"method":"earned_settlement_access","requirements":["Complete optional wing","Spend Insight on privilege","Attend open service location"]}  
**effect:** {"kind":"local_service","detail":"Grants temporary Ward"}  
**cost:** {"max_total_uses":2,"per_game_day":1}  
**duration:** local service outcome  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** Not a portable character spell  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `service.realm.8.requirement.1` — Complete optional wing. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.8.requirement.2` — Spend Insight on privilege. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `service.realm.8.requirement.3` — Attend open service location. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## FUNDAMENTAL COMBAT (10)

### Weapon Strike — `combat.melee`

**category:** fundamental_combat  
**summary:** Stamina-priced melee swing with bounded arc and one-hit receipt  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"Stamina-priced melee swing with bounded arc and one-hit receipt"}  
**cost:** {"stamina":"style-dependent"}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.melee.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Three-Hit Combo — `combat.combo`

**category:** fundamental_combat  
**summary:** Third chained hit gets a 35 percent multiplier  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"Third chained hit gets a 35 percent multiplier"}  
**cost:** {"stamina":"stamina per hit"}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.combo.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Guard — `combat.guard`

**category:** fundamental_combat  
**summary:** Block physical attack damage and set up defensive counters  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"Block physical attack damage and set up defensive counters"}  
**cost:** {"stamina":"guard resource"}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.guard.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Timed Parry — `combat.parry`

**category:** fundamental_combat  
**summary:** A correctly timed guard can intercept attack damage  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"A correctly timed guard can intercept attack damage"}  
**cost:** {"stamina":5}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.parry.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Dodge — `combat.dodge`

**category:** fundamental_combat  
**summary:** Directional evasive movement with a short invulnerability window  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"Directional evasive movement with a short invulnerability window"}  
**cost:** {"stamina":22}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.dodge.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Sneak / Crouch — `combat.sneak`

**category:** fundamental_combat  
**summary:** Reduces detection range in authored stealth situations  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"Reduces detection range in authored stealth situations"}  
**cost:** {"stamina":0}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.sneak.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Enemy Stagger — `combat.stagger`

**category:** fundamental_combat  
**summary:** Interrupt eligible enemies via powers, contact or exposure  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"Interrupt eligible enemies via powers, contact or exposure"}  
**cost:** {"stamina":"attack-dependent"}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.stagger.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Non-homing Projectile — `combat.projectile`

**category:** fundamental_combat  
**summary:** Fire a ranged shot that can be blocked by world cover  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"Fire a ranged shot that can be blocked by world cover"}  
**cost:** {"stamina":"weapon-dependent"}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.projectile.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Field Kit — `combat.field_kit`

**category:** fundamental_combat  
**summary:** Suppress eligible danger field for 20 seconds, 30 with earned binding  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"Suppress eligible danger field for 20 seconds, 30 with earned binding"}  
**cost:** {"stamina":"one kit"}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.field_kit.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Recovery — `combat.healing`

**category:** fundamental_combat  
**summary:** Restore missing health through a consumable or learned technique  
**acquisition:** {"method":"base_action_or_owned_gear","requirements":["Ability must be available and target valid"]}  
**effect:** {"kind":"combat","summary":"Restore missing health through a consumable or learned technique"}  
**cost:** {"stamina":"item/technique-dependent"}  
**duration:** active action  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:**   
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Combat/UnmadeCombatStyleRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `combat.healing.requirement.1` — Ability must be available and target valid. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## ENEMY CREATURE PATTERN (12)

### Cinderjaw — `enemy.creature.Cinderjaw`

**category:** enemy_creature_pattern  
**summary:** JAW STRIKE - GUARD OR RETREAT  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Stalker","health":85,"damage":14,"cooldown_seconds":1.4,"move_speed":145,"windup_seconds":0.7,"reach":2.2}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.Cinderjaw.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.Cinderjaw.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.Cinderjaw.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### PaleStrider — `enemy.creature.PaleStrider`

**category:** enemy_creature_pattern  
**summary:** STRIDER LUNGE - STEP BACK  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Stalker","health":65,"damage":11,"cooldown_seconds":1.1,"move_speed":205,"windup_seconds":0.85,"reach":2.6}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.PaleStrider.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.PaleStrider.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.PaleStrider.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Veilwatch — `enemy.creature.Veilwatch`

**category:** enemy_creature_pattern  
**summary:** VEIL PULSE - BREAK SIGHT  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Watcher","health":75,"damage":12,"cooldown_seconds":2.3,"move_speed":95,"windup_seconds":1.15,"reach":7}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.Veilwatch.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.Veilwatch.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.Veilwatch.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### GleamMoth — `enemy.creature.GleamMoth`

**category:** enemy_creature_pattern  
**summary:** GLEAM FLASH - TAKE COVER  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Watcher","health":45,"damage":8,"cooldown_seconds":1.5,"move_speed":170,"windup_seconds":0.85,"reach":5.5}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.GleamMoth.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.GleamMoth.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.GleamMoth.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### HollowHound — `enemy.creature.HollowHound`

**category:** enemy_creature_pattern  
**summary:** HOUND BITE - GUARD  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Stalker","health":80,"damage":12,"cooldown_seconds":1.2,"move_speed":185,"windup_seconds":0.75,"reach":2.1}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.HollowHound.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.HollowHound.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.HollowHound.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### GriefLantern — `enemy.creature.GriefLantern`

**category:** enemy_creature_pattern  
**summary:** GRIEF PEAL - BREAK SIGHT  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Watcher","health":90,"damage":16,"cooldown_seconds":3,"move_speed":70,"windup_seconds":1.5,"reach":8}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.GriefLantern.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.GriefLantern.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.GriefLantern.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Inkmaw — `enemy.creature.Inkmaw`

**category:** enemy_creature_pattern  
**summary:** INKMAW CRUSH - RETREAT  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Stalker","health":110,"damage":18,"cooldown_seconds":2,"move_speed":100,"windup_seconds":1.1,"reach":2.4}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.Inkmaw.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.Inkmaw.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.Inkmaw.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### RedactionEye — `enemy.creature.RedactionEye`

**category:** enemy_creature_pattern  
**summary:** REDACTION - USE THE SHELVES  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Watcher","health":60,"damage":14,"cooldown_seconds":2.2,"move_speed":115,"windup_seconds":1.2,"reach":6.5}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.RedactionEye.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.RedactionEye.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.RedactionEye.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### SaltglassCrawler — `enemy.creature.SaltglassCrawler`

**category:** enemy_creature_pattern  
**summary:** CRAWLER SNAP - STEP BACK  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Stalker","health":125,"damage":16,"cooldown_seconds":1.8,"move_speed":85,"windup_seconds":1,"reach":2.3}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.SaltglassCrawler.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.SaltglassCrawler.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.SaltglassCrawler.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### DrownedWatch — `enemy.creature.DrownedWatch`

**category:** enemy_creature_pattern  
**summary:** DROWNED PULSE - TAKE COVER  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Watcher","health":85,"damage":13,"cooldown_seconds":2.6,"move_speed":80,"windup_seconds":1.35,"reach":7.5}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.DrownedWatch.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.DrownedWatch.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.DrownedWatch.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Ashmaw — `enemy.creature.Ashmaw`

**category:** enemy_creature_pattern  
**summary:** ASHMAW SLAM - RETREAT  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Stalker","health":140,"damage":22,"cooldown_seconds":2.5,"move_speed":105,"windup_seconds":1.3,"reach":2.6}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.Ashmaw.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.Ashmaw.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.Ashmaw.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### KilnWisp — `enemy.creature.KilnWisp`

**category:** enemy_creature_pattern  
**summary:** KILN FLARE - BREAK SIGHT  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"enemy_attack","style":"Watcher","health":50,"damage":10,"cooldown_seconds":1.8,"move_speed":155,"windup_seconds":1,"reach":6}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.creature.KilnWisp.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.KilnWisp.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.creature.KilnWisp.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## ENEMY BOSS PATTERN (3)

### The Bell That Buried Its Keeper — `enemy.boss.HollowBell`

**category:** enemy_boss_pattern  
**summary:** Fold interrupts the toll  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"boss_attack","health":300,"damage":28,"range_cm":450,"windup_seconds":2,"recovery_seconds":2.5}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeBossRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.boss.HollowBell.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.boss.HollowBell.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.boss.HollowBell.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Curator of Missing Names — `enemy.boss.RedactedCurator`

**category:** enemy_boss_pattern  
**summary:** Glimpse reveals the erased signature  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"boss_attack","health":260,"damage":24,"range_cm":720,"windup_seconds":1.5,"recovery_seconds":2.25}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeBossRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.boss.RedactedCurator.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.boss.RedactedCurator.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.boss.RedactedCurator.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Unfinished Pilgrim — `enemy.boss.UnfinishedPilgrim`

**category:** enemy_boss_pattern  
**summary:** Fold exposes the path between charges  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"boss_attack","health":340,"damage":32,"range_cm":300,"windup_seconds":1.8,"recovery_seconds":2.75}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeBossRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.boss.UnfinishedPilgrim.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.boss.UnfinishedPilgrim.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.boss.UnfinishedPilgrim.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## ENEMY REALM GUARDIAN (6)

### The Collector of Unborn Days — `enemy.guardian.realm.1`

**category:** enemy_realm_guardian  
**summary:** The collector unwinds a future chain; step beyond the arc or Fold its strike.  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"guardian_strike","damage":18,"windup_seconds":1.8,"recovery_seconds":2.5,"range_cm":340}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.guardian.realm.1.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.1.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.1.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Missing Chorister — `enemy.guardian.realm.2`

**category:** enemy_realm_guardian  
**summary:** The invisible note tightens; break sight, guard, or Fold before it falls.  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"guardian_strike","damage":14,"windup_seconds":2.2,"recovery_seconds":2,"range_cm":510}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.guardian.realm.2.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.2.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.2.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Mortar Ancestor — `enemy.guardian.realm.3`

**category:** enemy_realm_guardian  
**summary:** The ancestor raises a quarry hammer; retreat behind the seam or interrupt.  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"guardian_strike","damage":21,"windup_seconds":2.4,"recovery_seconds":3,"range_cm":350}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.guardian.realm.3.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.3.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.3.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Numberless Examiner — `enemy.guardian.realm.4`

**category:** enemy_realm_guardian  
**summary:** A numbered pulse narrows; evade or use the covered addressway.  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"guardian_strike","damage":15,"windup_seconds":1.65,"recovery_seconds":2.75,"range_cm":440}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.guardian.realm.4.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.4.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.4.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Root Bailiff — `enemy.guardian.realm.5`

**category:** enemy_realm_guardian  
**summary:** A law-root points at your feet; cross to the unbound row before impact.  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"guardian_strike","damage":19,"windup_seconds":2,"recovery_seconds":2.9,"range_cm":385}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.guardian.realm.5.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.5.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.5.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Unfinished Witness — `enemy.guardian.realm.6`

**category:** enemy_realm_guardian  
**summary:** Two overlapping silhouettes prepare a strike; retreat into the stable path.  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"guardian_strike","damage":20,"windup_seconds":2.5,"recovery_seconds":3.1,"range_cm":420}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.guardian.realm.6.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.6.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.guardian.realm.6.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## ENEMY WING GUARDIAN (9)

### The Unmeasured Bailiff — `enemy.wing_guardian.realm.0`

**category:** enemy_wing_guardian  
**summary:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.wing_guardian.realm.0.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.0.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.0.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Brine Tollkeeper — `enemy.wing_guardian.realm.1`

**category:** enemy_wing_guardian  
**summary:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.wing_guardian.realm.1.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.1.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.1.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Unspent Furnace — `enemy.wing_guardian.realm.2`

**category:** enemy_wing_guardian  
**summary:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.wing_guardian.realm.2.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.2.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.2.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Unclaimed Duelist — `enemy.wing_guardian.realm.3`

**category:** enemy_wing_guardian  
**summary:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.wing_guardian.realm.3.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.3.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.3.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Falling Cantor — `enemy.wing_guardian.realm.4`

**category:** enemy_wing_guardian  
**summary:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.wing_guardian.realm.4.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.4.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.4.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Borrowed Custodian — `enemy.wing_guardian.realm.5`

**category:** enemy_wing_guardian  
**summary:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.wing_guardian.realm.5.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.5.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.5.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Unlisted Watcher — `enemy.wing_guardian.realm.6`

**category:** enemy_wing_guardian  
**summary:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.wing_guardian.realm.6.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.6.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.6.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Boundary Pruner — `enemy.wing_guardian.realm.7`

**category:** enemy_wing_guardian  
**summary:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.wing_guardian.realm.7.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.7.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.7.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Closing Registrar — `enemy.wing_guardian.realm.8`

**category:** enemy_wing_guardian  
**summary:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.wing_guardian.realm.8.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.8.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.wing_guardian.realm.8.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## ENEMY EXPEDITION BOSS (9)

### The Tollkeeper Without a Road — `enemy.expedition_signature.realm.0`

**category:** enemy_expedition_boss  
**summary:** MILEGLASS RING — CENTER IS SAFE  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"MILEGLASS RING — CENTER IS SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.expedition_signature.realm.0.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.0.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.0.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Undertow Bailiff — `enemy.expedition_signature.realm.1`

**category:** enemy_expedition_boss  
**summary:** UNDERTOW LANE — MOVE SIDEWAYS  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"UNDERTOW LANE — MOVE SIDEWAYS"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.expedition_signature.realm.1.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.1.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.1.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Ashbound Provost — `enemy.expedition_signature.realm.2`

**category:** enemy_expedition_boss  
**summary:** KILN FAN — SOUTH HALF IS SAFE  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"KILN FAN — SOUTH HALF IS SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.expedition_signature.realm.2.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.2.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.2.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Interest Warden — `enemy.expedition_signature.realm.3`

**category:** enemy_expedition_boss  
**summary:** OATH CROSS — DIAGONALS ARE SAFE  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"OATH CROSS — DIAGONALS ARE SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.expedition_signature.realm.3.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.3.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.3.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Counterweight Cantor — `enemy.expedition_signature.realm.4`

**category:** enemy_expedition_boss  
**summary:** CHOIR QUADRANTS — OPPOSITE SIGNS SAFE  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"CHOIR QUADRANTS — OPPOSITE SIGNS SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.expedition_signature.realm.4.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.4.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.4.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Last Claimant — `enemy.expedition_signature.realm.5`

**category:** enemy_expedition_boss  
**summary:** SEAM SPLIT — CENTRAL STRIP SAFE  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"SEAM SPLIT — CENTRAL STRIP SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.expedition_signature.realm.5.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.5.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.5.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Registrar of Empty Beds — `enemy.expedition_signature.realm.6`

**category:** enemy_expedition_boss  
**summary:** ELSEBORN DIVIDE — MIDDLE BAND SAFE  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"ELSEBORN DIVIDE — MIDDLE BAND SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.expedition_signature.realm.6.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.6.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.6.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Unwritten Castellan — `enemy.expedition_signature.realm.7`

**category:** enemy_expedition_boss  
**summary:** CROWN SQUARE — LEAVE THE SQUARE  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"CROWN SQUARE — LEAVE THE SQUARE"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.expedition_signature.realm.7.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.7.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.7.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### The Keeper of the Single Door — `enemy.expedition_signature.realm.8`

**category:** enemy_expedition_boss  
**summary:** THRESHOLD RIM — CENTER IS SAFE  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"THRESHOLD RIM — CENTER IS SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.expedition_signature.realm.8.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.8.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.expedition_signature.realm.8.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## ENEMY FINAL BOSS (4)

### Nhal-Vey: Bell — `enemy.final.Bell`

**category:** enemy_final_boss  
**summary:** A wide directional forward toll with readable windup  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"final_footprint","summary":"A wide directional forward toll with readable windup"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.final.Bell.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.final.Bell.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.final.Bell.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Nhal-Vey: Horizon — `enemy.final.Horizon`

**category:** enemy_final_boss  
**summary:** A long thin forward seam avoided sideways  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"final_footprint","summary":"A long thin forward seam avoided sideways"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.final.Horizon.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.final.Horizon.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.final.Horizon.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Nhal-Vey: Names — `enemy.final.Names`

**category:** enemy_final_boss  
**summary:** An identity-themed close strike with actual visible forward footprint  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"final_footprint","summary":"An identity-themed close strike with actual visible forward footprint"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.final.Names.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.final.Names.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.final.Names.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

### Nhal-Vey: Counterfactual — `enemy.final.Counterfactual`

**category:** enemy_final_boss  
**summary:** An asymmetric right-side strike; escape marked strip  
**acquisition:** {"method":"firsthand_enemy_observation","requirements":["Encounter real target and survive/read its attack telegraph","Investigate a real vulnerability to earn counterplay"]}  
**effect:** {"kind":"final_footprint","summary":"An asymmetric right-side strike; escape marked strip"}  
**cost:** {"enemy_only":"source attack timing"}  
**duration:** individual encounter  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**limitations:** Observation grants no copied combat power in the current game  
**modifications:**   
**combines with:**   
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `enemy.final.Counterfactual.requirement.1` — Encounter real target and survive/read its attack telegraph. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.final.Counterfactual.requirement.2` — Investigate a real vulnerability to earn counterplay. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only
- [ ] `enemy.final.Counterfactual.requirement.3` — Observation only unlocks analysis or counterplay; acquiring the attack as a player skill is not yet implemented. Evidence: source definition or planned quest; not automatically assumed complete; state: evaluated_in_game_only

## EARNED STUDY ABILITY (4)

### Kiln Ward — `study.Cinderjaw`

**category:** earned_study_ability  
**summary:** Temporarily grants +5 armor; with Memory Lens increases duration from 6 to 9 seconds; with Redaction Read exposes a nearby foe.  
**acquisition:** {"method":"observed_understood_experimented_mastered","requirements":["Observe Cinderjaw warning and land a real hit; understand, complete two paid experiments, master at a live Cinderjaw.","No automatic copying; firsthand provenance required","Confirmed local target with line of sight","Observe and physically corroborate the subject, then meet all six checklist milestones; field experiments on enemies require another real attack telegraph"]}  
**effect:** {"kind":"learned_combat","detail":"Temporarily grants +5 armor; with Memory Lens increases duration from 6 to 9 seconds; with Redaction Read exposes a nearby foe."}  
**cost:** {"strain_per_activation":9,"activation_cooldown_seconds":18,"experiment_strain":10,"experiment_cooldown_seconds":8,"mastery_strain":5,"other_actions":"Pale Strider dodge also consumes stamina"}  
**duration:** bounded active effect; mastery persists per save profile  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":true,"unlock_path":"confirmed firsthand warning or Echo Well, corroboration, understanding, two physical experiments and mastery"}  
**limitations:** 4 authored subjects only in this initial slice; Only selected subject with completed mastery can be invoked; Temporary bonuses clear after expiration and do not replace other gear bonuses  
**modifications:** paired mastery alters effect and duration  
**combines with:** study.EchoWell; study.RedactionEye  
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAbilityStudyRules.h; Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `study.Cinderjaw.requirement.1` — Personally witness a real attack warning by this living enemy species. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.Cinderjaw.requirement.2` — Corroborate observation by dealing actual damage to the same living enemy species. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.Cinderjaw.requirement.3` — Interpret both firsthand observations to reach Understanding. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.Cinderjaw.requirement.4` — Complete experiment one at the actual subject with required Strain and time limits. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.Cinderjaw.requirement.5` — Complete experiment two at the actual subject with a fresh attack warning when applicable. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.Cinderjaw.requirement.6` — Master the skill physically, pay its cost, and commit the result to the character's save. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only

### Seam Step — `study.PaleStrider`

**category:** earned_study_ability  
**summary:** Executes actual stamina-priced evasive dodge launch; paired with Memory Lens adds a 2-armor, 3-second ward.  
**acquisition:** {"method":"observed_understood_experimented_mastered","requirements":["Observe Pale Strider warning and land a real hit; understand, complete two paid experiments, master at a live Pale Strider.","No automatic copying; firsthand provenance required","Confirmed local target with line of sight","Observe and physically corroborate the subject, then meet all six checklist milestones; field experiments on enemies require another real attack telegraph"]}  
**effect:** {"kind":"learned_combat","detail":"Executes actual stamina-priced evasive dodge launch; paired with Memory Lens adds a 2-armor, 3-second ward."}  
**cost:** {"strain_per_activation":9,"activation_cooldown_seconds":18,"experiment_strain":10,"experiment_cooldown_seconds":8,"mastery_strain":5,"other_actions":"Pale Strider dodge also consumes stamina"}  
**duration:** bounded active effect; mastery persists per save profile  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":true,"unlock_path":"confirmed firsthand warning or Echo Well, corroboration, understanding, two physical experiments and mastery"}  
**limitations:** 4 authored subjects only in this initial slice; Only selected subject with completed mastery can be invoked; Temporary bonuses clear after expiration and do not replace other gear bonuses  
**modifications:** paired mastery alters effect and duration  
**combines with:** study.EchoWell  
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAbilityStudyRules.h; Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `study.PaleStrider.requirement.1` — Personally witness a real attack warning by this living enemy species. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.PaleStrider.requirement.2` — Corroborate observation by dealing actual damage to the same living enemy species. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.PaleStrider.requirement.3` — Interpret both firsthand observations to reach Understanding. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.PaleStrider.requirement.4` — Complete experiment one at the actual subject with required Strain and time limits. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.PaleStrider.requirement.5` — Complete experiment two at the actual subject with a fresh attack warning when applicable. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.PaleStrider.requirement.6` — Master the skill physically, pay its cost, and commit the result to the character's save. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only

### Redaction Read — `study.RedactionEye`

**category:** earned_study_ability  
**summary:** Exposes a nearby real enemy for 6 seconds, rising to 8 seconds combined with Memory Lens; pairs with Kiln Ward for an interrupting defense.  
**acquisition:** {"method":"observed_understood_experimented_mastered","requirements":["Observe Redaction Eye warning and land a real hit; understand, complete two paid experiments, master at a live Redaction Eye.","No automatic copying; firsthand provenance required","Confirmed local target with line of sight","Observe and physically corroborate the subject, then meet all six checklist milestones; field experiments on enemies require another real attack telegraph"]}  
**effect:** {"kind":"learned_combat","detail":"Exposes a nearby real enemy for 6 seconds, rising to 8 seconds combined with Memory Lens; pairs with Kiln Ward for an interrupting defense."}  
**cost:** {"strain_per_activation":9,"activation_cooldown_seconds":18,"experiment_strain":10,"experiment_cooldown_seconds":8,"mastery_strain":5,"other_actions":"Pale Strider dodge also consumes stamina"}  
**duration:** bounded active effect; mastery persists per save profile  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":true,"unlock_path":"confirmed firsthand warning or Echo Well, corroboration, understanding, two physical experiments and mastery"}  
**limitations:** 4 authored subjects only in this initial slice; Only selected subject with completed mastery can be invoked; Temporary bonuses clear after expiration and do not replace other gear bonuses  
**modifications:** paired mastery alters effect and duration  
**combines with:** study.EchoWell; study.Cinderjaw  
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAbilityStudyRules.h; Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `study.RedactionEye.requirement.1` — Personally witness a real attack warning by this living enemy species. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.RedactionEye.requirement.2` — Corroborate observation by dealing actual damage to the same living enemy species. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.RedactionEye.requirement.3` — Interpret both firsthand observations to reach Understanding. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.RedactionEye.requirement.4` — Complete experiment one at the actual subject with required Strain and time limits. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.RedactionEye.requirement.5` — Complete experiment two at the actual subject with a fresh attack warning when applicable. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.RedactionEye.requirement.6` — Master the skill physically, pay its cost, and commit the result to the character's save. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only

### Memory Lens — `study.EchoWell`

**category:** earned_study_ability  
**summary:** Exposes a real nearby foe for 4 seconds; in combination with Redaction Read extends it to 8 seconds and grants a 2-armor ward.  
**acquisition:** {"method":"observed_understood_experimented_mastered","requirements":["Visit the physical Echo Well; record firsthand observation, corroborate for 8 Strain, understand, perform two 10-Strain experiments and master at the well.","No automatic copying; firsthand provenance required","Confirmed local target with line of sight","Observe and physically corroborate the subject, then meet all six checklist milestones; field experiments on enemies require another real attack telegraph"]}  
**effect:** {"kind":"learned_combat","detail":"Exposes a real nearby foe for 4 seconds; in combination with Redaction Read extends it to 8 seconds and grants a 2-armor ward."}  
**cost:** {"strain_per_activation":9,"activation_cooldown_seconds":18,"experiment_strain":10,"experiment_cooldown_seconds":8,"mastery_strain":5,"other_actions":"Pale Strider dodge also consumes stamina"}  
**duration:** bounded active effect; mastery persists per save profile  
**observation:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":true,"unlock_path":"confirmed firsthand warning or Echo Well, corroboration, understanding, two physical experiments and mastery"}  
**limitations:** 4 authored subjects only in this initial slice; Only selected subject with completed mastery can be invoked; Temporary bonuses clear after expiration and do not replace other gear bonuses  
**modifications:** paired mastery alters effect and duration  
**combines with:** study.RedactionEye; study.Cinderjaw  
**implementation:** prototype_handler  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAbilityStudyRules.h; Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp; Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `study.EchoWell.requirement.1` — Personally inspect the physical Echo Well. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.EchoWell.requirement.2` — Corroborate the Well through a real Strain-priced measurement. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.EchoWell.requirement.3` — Interpret both firsthand observations to reach Understanding. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.EchoWell.requirement.4` — Complete experiment one at the actual subject with required Strain and time limits. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.EchoWell.requirement.5` — Complete experiment two at the actual subject with a fresh attack warning when applicable. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only
- [ ] `study.EchoWell.requirement.6` — Master the skill physically, pay its cost, and commit the result to the character's save. Evidence: live game state and character save; never free-form manual completion; state: evaluated_in_game_only

## REALM (9)

### The Threefold Reach — `realm.ThreefoldReach`

**category:** realm  
**summary:** refuge or testimony  
**acquisition:** {"method":"authored_progression","requirements":["Physically reach this realm by a genuinely available path","Identify local lore from firsthand evidence, not hearsay","Resolve a supported choice and preserve its consequence"]}  
**effect:** {"kind":"realm","summary":"refuge or testimony"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:**   
**rule contract:** {"ordinal":0,"stage":0,"landscape":"roads stitched into contradictory maps","civilization":"the charterless settlements","rare_reward_theme":"witness gear and path relics","hub":"Crossings","world_status":"initial_foothold_source"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `realm.ThreefoldReach.requirement.1` — Physically reach this realm by a genuinely available path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.ThreefoldReach.requirement.2` — Identify local lore from firsthand evidence, not hearsay. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.ThreefoldReach.requirement.3` — Resolve a supported choice and preserve its consequence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Rain That Forgot the Sea — `realm.WidowedRain`

**category:** realm  
**summary:** whose absence buys fresh water  
**acquisition:** {"method":"authored_progression","requirements":["Physically reach this realm by a genuinely available path","Identify local lore from firsthand evidence, not hearsay","Resolve a supported choice and preserve its consequence"]}  
**effect:** {"kind":"realm","summary":"whose absence buys fresh water"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:**   
**rule contract:** {"ordinal":1,"stage":1,"landscape":"permanent storm over hollow beaches","civilization":"the rain-counting flotillas","rare_reward_theme":"weather-forged weapons","hub":"Saltwake","world_status":"initial_foothold_source"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `realm.WidowedRain.requirement.1` — Physically reach this realm by a genuinely available path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.WidowedRain.requirement.2` — Identify local lore from firsthand evidence, not hearsay. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.WidowedRain.requirement.3` — Resolve a supported choice and preserve its consequence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Hearth Beneath — `realm.HearthBeneath`

**category:** realm  
**summary:** who owns a common flame  
**acquisition:** {"method":"authored_progression","requirements":["Physically reach this realm by a genuinely available path","Identify local lore from firsthand evidence, not hearsay","Resolve a supported choice and preserve its consequence"]}  
**effect:** {"kind":"realm","summary":"who owns a common flame"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:**   
**rule contract:** {"ordinal":2,"stage":1,"landscape":"warm cave cities under a frozen sky","civilization":"keepers of the borrowed ember","rare_reward_theme":"bell and shelter armors","hub":"Cinderhold","world_status":"initial_foothold_source"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `realm.HearthBeneath.requirement.1` — Physically reach this realm by a genuinely available path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.HearthBeneath.requirement.2` — Identify local lore from firsthand evidence, not hearsay. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.HearthBeneath.requirement.3` — Resolve a supported choice and preserve its consequence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Sea of Written Debts — `realm.TidalLedger`

**category:** realm  
**summary:** creditors foreclose on memories  
**acquisition:** {"method":"authored_progression","requirements":["Physically reach this realm by a genuinely available path","Identify local lore from firsthand evidence, not hearsay","Resolve a supported choice and preserve its consequence"]}  
**effect:** {"kind":"realm","summary":"creditors foreclose on memories"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:**   
**rule contract:** {"ordinal":3,"stage":2,"landscape":"black tide that remembers future bargains","civilization":"the navigators of unsettled prices","rare_reward_theme":"debt and oath blades","hub":"Drevlach","world_status":"atlas_only_or_later_source_not_engine_verified"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `realm.TidalLedger.requirement.1` — Physically reach this realm by a genuinely available path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.TidalLedger.requirement.2` — Identify local lore from firsthand evidence, not hearsay. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.TidalLedger.requirement.3` — Resolve a supported choice and preserve its consequence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Upside-Down Choir — `realm.SkyBelow`

**category:** realm  
**summary:** songs hold the gravity of cities  
**acquisition:** {"method":"authored_progression","requirements":["Physically reach this realm by a genuinely available path","Identify local lore from firsthand evidence, not hearsay","Resolve a supported choice and preserve its consequence"]}  
**effect:** {"kind":"realm","summary":"songs hold the gravity of cities"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:**   
**rule contract:** {"ordinal":4,"stage":2,"landscape":"islands suspended beneath an inward sky","civilization":"the downward singers","rare_reward_theme":"gravity-changing relics","hub":"Orravane","world_status":"atlas_only_or_later_source_not_engine_verified"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `realm.SkyBelow.requirement.1` — Physically reach this realm by a genuinely available path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.SkyBelow.requirement.2` — Identify local lore from firsthand evidence, not hearsay. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.SkyBelow.requirement.3` — Resolve a supported choice and preserve its consequence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Bones of Yesterdays — `realm.CinderSpine`

**category:** realm  
**summary:** mining history erases descendants  
**acquisition:** {"method":"authored_progression","requirements":["Physically reach this realm by a genuinely available path","Identify local lore from firsthand evidence, not hearsay","Resolve a supported choice and preserve its consequence"]}  
**effect:** {"kind":"realm","summary":"mining history erases descendants"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:**   
**rule contract:** {"ordinal":5,"stage":3,"landscape":"a mountain range made from fossilized timelines","civilization":"the seam-walkers","rare_reward_theme":"time-tempered smithing","hub":"Vathless","world_status":"atlas_only_or_later_source_not_engine_verified"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `realm.CinderSpine.requirement.1` — Physically reach this realm by a genuinely available path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.CinderSpine.requirement.2` — Identify local lore from firsthand evidence, not hearsay. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.CinderSpine.requirement.3` — Resolve a supported choice and preserve its consequence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Hundred Unlived — `realm.HundredUnlived`

**category:** realm  
**summary:** who has the right to existence  
**acquisition:** {"method":"authored_progression","requirements":["Physically reach this realm by a genuinely available path","Identify local lore from firsthand evidence, not hearsay","Resolve a supported choice and preserve its consequence"]}  
**effect:** {"kind":"realm","summary":"who has the right to existence"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:**   
**rule contract:** {"ordinal":6,"stage":3,"landscape":"cities built for people never born","civilization":"the unliving census","rare_reward_theme":"namebound artifacts","hub":"Eillun","world_status":"atlas_only_or_later_source_not_engine_verified"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `realm.HundredUnlived.requirement.1` — Physically reach this realm by a genuinely available path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.HundredUnlived.requirement.2` — Identify local lore from firsthand evidence, not hearsay. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.HundredUnlived.requirement.3` — Resolve a supported choice and preserve its consequence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Orchard of Unwritten Kings — `realm.OrchardOfKings`

**category:** realm  
**summary:** a kingdom without a past  
**acquisition:** {"method":"authored_progression","requirements":["Physically reach this realm by a genuinely available path","Identify local lore from firsthand evidence, not hearsay","Resolve a supported choice and preserve its consequence"]}  
**effect:** {"kind":"realm","summary":"a kingdom without a past"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:**   
**rule contract:** {"ordinal":7,"stage":4,"landscape":"a forest of buried laws and crowns","civilization":"the untitled courts","rare_reward_theme":"regalia that carries consequences","hub":"Tharniv","world_status":"atlas_only_or_later_source_not_engine_verified"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `realm.OrchardOfKings.requirement.1` — Physically reach this realm by a genuinely available path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.OrchardOfKings.requirement.2` — Identify local lore from firsthand evidence, not hearsay. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.OrchardOfKings.requirement.3` — Resolve a supported choice and preserve its consequence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Place Before Place — `realm.FirstAbsence`

**category:** realm  
**summary:** whether reality should remain singular  
**acquisition:** {"method":"authored_progression","requirements":["Physically reach this realm by a genuinely available path","Identify local lore from firsthand evidence, not hearsay","Resolve a supported choice and preserve its consequence"]}  
**effect:** {"kind":"realm","summary":"whether reality should remain singular"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:**   
**rule contract:** {"ordinal":8,"stage":5,"landscape":"unformed land that refuses geometry","civilization":"the impossible witnesses","rare_reward_theme":"the Atlas of Absence","hub":"Auvren","world_status":"atlas_only_or_later_source_not_engine_verified"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `realm.FirstAbsence.requirement.1` — Physically reach this realm by a genuinely available path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.FirstAbsence.requirement.2` — Identify local lore from firsthand evidence, not hearsay. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `realm.FirstAbsence.requirement.3` — Resolve a supported choice and preserve its consequence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

## JOURNEY (9)

### The Split Charter Vault — Crossings — `journey.expedition.ThreefoldReach`

**category:** journey  
**summary:** Find the missing surveyors before the charter is burned.  
**acquisition:** {"method":"authored_progression","requirements":["Receive the expedition briefing","Travel to the frontier and physically inspect its six chambers","Discover and choose an authored resolution","Resolve the boss through a supported fight or mercy path","Return to refuge and claim a one-time earned reward"]}  
**effect:** {"kind":"journey","summary":"Find the missing surveyors before the charter is burned."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.ThreefoldReach  
**rule contract:** {"realm":"realm.ThreefoldReach","city":"Crossings","frontier":"Rookstep","refuge":"Lantern Rest","dungeon":"The Split Charter Vault","boss":"The Tollkeeper Without a Road","weapon":"Mileglass Edge","armor":"Witness Brace","technique":"Marked Opening","steps":["Board","Frontier","Chamber","Resolution","Boss","Return","Reward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.expedition.ThreefoldReach.requirement.1` — Receive the expedition briefing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.ThreefoldReach.requirement.2` — Travel to the frontier and physically inspect its six chambers. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.ThreefoldReach.requirement.3` — Discover and choose an authored resolution. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.ThreefoldReach.requirement.4` — Resolve the boss through a supported fight or mercy path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.ThreefoldReach.requirement.5` — Return to refuge and claim a one-time earned reward. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Hollow Breakwater — Saltwake — `journey.expedition.WidowedRain`

**category:** journey  
**summary:** Release the divers held behind the pressure locks.  
**acquisition:** {"method":"authored_progression","requirements":["Receive the expedition briefing","Travel to the frontier and physically inspect its six chambers","Discover and choose an authored resolution","Resolve the boss through a supported fight or mercy path","Return to refuge and claim a one-time earned reward"]}  
**effect:** {"kind":"journey","summary":"Release the divers held behind the pressure locks."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.WidowedRain  
**rule contract:** {"realm":"realm.WidowedRain","city":"Saltwake","frontier":"Brinewatch","refuge":"Drywake","dungeon":"The Hollow Breakwater","boss":"The Undertow Bailiff","weapon":"Breakwater Spear","armor":"Rainward Mantle","technique":"Pressure Read","steps":["Board","Frontier","Chamber","Resolution","Boss","Return","Reward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.expedition.WidowedRain.requirement.1` — Receive the expedition briefing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.WidowedRain.requirement.2` — Travel to the frontier and physically inspect its six chambers. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.WidowedRain.requirement.3` — Discover and choose an authored resolution. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.WidowedRain.requirement.4` — Resolve the boss through a supported fight or mercy path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.WidowedRain.requirement.5` — Return to refuge and claim a one-time earned reward. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Furnace Below Blackslag — Cinderhold — `journey.expedition.HearthBeneath`

**category:** journey  
**summary:** Recover trapped furnace tenders from the locked lower galleries.  
**acquisition:** {"method":"authored_progression","requirements":["Receive the expedition briefing","Travel to the frontier and physically inspect its six chambers","Discover and choose an authored resolution","Resolve the boss through a supported fight or mercy path","Return to refuge and claim a one-time earned reward"]}  
**effect:** {"kind":"journey","summary":"Recover trapped furnace tenders from the locked lower galleries."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HearthBeneath  
**rule contract:** {"realm":"realm.HearthBeneath","city":"Cinderhold","frontier":"Emberwatch","refuge":"Snowmelt Rest","dungeon":"The Furnace Below Blackslag","boss":"The Ashbound Provost","weapon":"Kilnfang Maul","armor":"Cinderguard Plate","technique":"Ember Counter","steps":["Board","Frontier","Chamber","Resolution","Boss","Return","Reward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.expedition.HearthBeneath.requirement.1` — Receive the expedition briefing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.HearthBeneath.requirement.2` — Travel to the frontier and physically inspect its six chambers. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.HearthBeneath.requirement.3` — Discover and choose an authored resolution. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.HearthBeneath.requirement.4` — Resolve the boss through a supported fight or mercy path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.HearthBeneath.requirement.5` — Return to refuge and claim a one-time earned reward. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Foreclosed Anchorage — Drevlach — `journey.expedition.TidalLedger`

**category:** journey  
**summary:** Free the ferrymen held as collateral in the anchorage.  
**acquisition:** {"method":"authored_progression","requirements":["Receive the expedition briefing","Travel to the frontier and physically inspect its six chambers","Discover and choose an authored resolution","Resolve the boss through a supported fight or mercy path","Return to refuge and claim a one-time earned reward"]}  
**effect:** {"kind":"journey","summary":"Free the ferrymen held as collateral in the anchorage."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.TidalLedger  
**rule contract:** {"realm":"realm.TidalLedger","city":"Drevlach","frontier":"Oathshoal","refuge":"Reedwake","dungeon":"The Foreclosed Anchorage","boss":"The Interest Warden","weapon":"Undertide Hook","armor":"Oathwake Coat","technique":"Oath Interrupt","steps":["Board","Frontier","Chamber","Resolution","Boss","Return","Reward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.expedition.TidalLedger.requirement.1` — Receive the expedition briefing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.TidalLedger.requirement.2` — Travel to the frontier and physically inspect its six chambers. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.TidalLedger.requirement.3` — Discover and choose an authored resolution. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.TidalLedger.requirement.4` — Resolve the boss through a supported fight or mercy path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.TidalLedger.requirement.5` — Return to refuge and claim a one-time earned reward. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Fallen Choir Engine — Orravane — `journey.expedition.SkyBelow`

**category:** journey  
**summary:** Release the riggers suspended in the engine’s holding cages.  
**acquisition:** {"method":"authored_progression","requirements":["Receive the expedition briefing","Travel to the frontier and physically inspect its six chambers","Discover and choose an authored resolution","Resolve the boss through a supported fight or mercy path","Return to refuge and claim a one-time earned reward"]}  
**effect:** {"kind":"journey","summary":"Release the riggers suspended in the engine’s holding cages."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.SkyBelow  
**rule contract:** {"realm":"realm.SkyBelow","city":"Orravane","frontier":"Downsong","refuge":"Quiet Span","dungeon":"The Fallen Choir Engine","boss":"The Counterweight Cantor","weapon":"Downfall Glaive","armor":"Choirweight Harness","technique":"Grounded Fold","steps":["Board","Frontier","Chamber","Resolution","Boss","Return","Reward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.expedition.SkyBelow.requirement.1` — Receive the expedition briefing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.SkyBelow.requirement.2` — Travel to the frontier and physically inspect its six chambers. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.SkyBelow.requirement.3` — Discover and choose an authored resolution. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.SkyBelow.requirement.4` — Resolve the boss through a supported fight or mercy path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.SkyBelow.requirement.5` — Return to refuge and claim a one-time earned reward. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Buried Yesterday Foundry — Vathless — `journey.expedition.CinderSpine`

**category:** journey  
**summary:** Extract the seam-workers before their records are crushed.  
**acquisition:** {"method":"authored_progression","requirements":["Receive the expedition briefing","Travel to the frontier and physically inspect its six chambers","Discover and choose an authored resolution","Resolve the boss through a supported fight or mercy path","Return to refuge and claim a one-time earned reward"]}  
**effect:** {"kind":"journey","summary":"Extract the seam-workers before their records are crushed."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.CinderSpine  
**rule contract:** {"realm":"realm.CinderSpine","city":"Vathless","frontier":"Seamguard","refuge":"Kinrest","dungeon":"The Buried Yesterday Foundry","boss":"The Last Claimant","weapon":"Yesterday Pickblade","armor":"Seamward Shell","technique":"Seam Read","steps":["Board","Frontier","Chamber","Resolution","Boss","Return","Reward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.expedition.CinderSpine.requirement.1` — Receive the expedition briefing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.CinderSpine.requirement.2` — Travel to the frontier and physically inspect its six chambers. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.CinderSpine.requirement.3` — Discover and choose an authored resolution. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.CinderSpine.requirement.4` — Resolve the boss through a supported fight or mercy path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.CinderSpine.requirement.5` — Return to refuge and claim a one-time earned reward. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Unentered Census House — Eillun — `journey.expedition.HundredUnlived`

**category:** journey  
**summary:** Release the caretakers awaiting removal from the census house.  
**acquisition:** {"method":"authored_progression","requirements":["Receive the expedition briefing","Travel to the frontier and physically inspect its six chambers","Discover and choose an authored resolution","Resolve the boss through a supported fight or mercy path","Return to refuge and claim a one-time earned reward"]}  
**effect:** {"kind":"journey","summary":"Release the caretakers awaiting removal from the census house."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HundredUnlived  
**rule contract:** {"realm":"realm.HundredUnlived","city":"Eillun","frontier":"Elseward","refuge":"Unlisted Hearth","dungeon":"The Unentered Census House","boss":"The Registrar of Empty Beds","weapon":"Elseborn Saber","armor":"Unlisted Veil","technique":"Unlisted Step","steps":["Board","Frontier","Chamber","Resolution","Boss","Return","Reward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.expedition.HundredUnlived.requirement.1` — Receive the expedition briefing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.HundredUnlived.requirement.2` — Travel to the frontier and physically inspect its six chambers. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.HundredUnlived.requirement.3` — Discover and choose an authored resolution. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.HundredUnlived.requirement.4` — Resolve the boss through a supported fight or mercy path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.HundredUnlived.requirement.5` — Return to refuge and claim a one-time earned reward. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Crownroot Keep — Tharniv — `journey.expedition.OrchardOfKings`

**category:** journey  
**summary:** Free the gardeners held beneath a sealed royal graft.  
**acquisition:** {"method":"authored_progression","requirements":["Receive the expedition briefing","Travel to the frontier and physically inspect its six chambers","Discover and choose an authored resolution","Resolve the boss through a supported fight or mercy path","Return to refuge and claim a one-time earned reward"]}  
**effect:** {"kind":"journey","summary":"Free the gardeners held beneath a sealed royal graft."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.OrchardOfKings  
**rule contract:** {"realm":"realm.OrchardOfKings","city":"Tharniv","frontier":"Rootwatch","refuge":"Graft Rest","dungeon":"The Crownroot Keep","boss":"The Unwritten Castellan","weapon":"Crownroot Falchion","armor":"Commonbark Guard","technique":"Root Counter","steps":["Board","Frontier","Chamber","Resolution","Boss","Return","Reward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.expedition.OrchardOfKings.requirement.1` — Receive the expedition briefing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.OrchardOfKings.requirement.2` — Travel to the frontier and physically inspect its six chambers. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.OrchardOfKings.requirement.3` — Discover and choose an authored resolution. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.OrchardOfKings.requirement.4` — Resolve the boss through a supported fight or mercy path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.OrchardOfKings.requirement.5` — Return to refuge and claim a one-time earned reward. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Unfinished Bastion — Auvren — `journey.expedition.FirstAbsence`

**category:** journey  
**summary:** Recover the witnesses held beyond the unfinished door.  
**acquisition:** {"method":"authored_progression","requirements":["Receive the expedition briefing","Travel to the frontier and physically inspect its six chambers","Discover and choose an authored resolution","Resolve the boss through a supported fight or mercy path","Return to refuge and claim a one-time earned reward"]}  
**effect:** {"kind":"journey","summary":"Recover the witnesses held beyond the unfinished door."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.FirstAbsence  
**rule contract:** {"realm":"realm.FirstAbsence","city":"Auvren","frontier":"Threshold Watch","refuge":"Nearrest","dungeon":"The Unfinished Bastion","boss":"The Keeper of the Single Door","weapon":"Nearness Blade","armor":"Threshold Ward","technique":"Nearness Hold","steps":["Board","Frontier","Chamber","Resolution","Boss","Return","Reward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.expedition.FirstAbsence.requirement.1` — Receive the expedition briefing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.FirstAbsence.requirement.2` — Travel to the frontier and physically inspect its six chambers. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.FirstAbsence.requirement.3` — Discover and choose an authored resolution. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.FirstAbsence.requirement.4` — Resolve the boss through a supported fight or mercy path. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.expedition.FirstAbsence.requirement.5` — Return to refuge and claim a one-time earned reward. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

## EXPERIENCE (9)

### Lantern Survey Gallery — Crossings — `journey.wing.ThreefoldReach`

**category:** experience  
**summary:** Guide the road surveyor  
**acquisition:** {"method":"authored_progression","requirements":["Accept the optional local experience wing","Complete three distinct fieldwork tasks","Confront or resolve the named guardian","Earn the specialization, tool and recipe"]}  
**effect:** {"kind":"experience","summary":"Guide the road surveyor"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.ThreefoldReach; journey.expedition.ThreefoldReach  
**rule contract:** {"realm":"realm.ThreefoldReach","city":"Crossings","wing":"Lantern Survey Gallery","guardian":"The Unmeasured Bailiff","interior":"Wayfinder Room","specialty":"Lantern bearings","task":"Guide the road surveyor","hint":"The surveyor walks only while you remain nearby. Meet them at each of three bearings.","tool":"Folding survey staff","recipe":"Waypoint rubbing","steps":["WingOffer","WingFieldwork","WingGuardian","WingReward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.wing.ThreefoldReach.requirement.1` — Accept the optional local experience wing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.ThreefoldReach.requirement.2` — Complete three distinct fieldwork tasks. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.ThreefoldReach.requirement.3` — Confront or resolve the named guardian. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.ThreefoldReach.requirement.4` — Earn the specialization, tool and recipe. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Drowned Pump Annex — Saltwake — `journey.wing.WidowedRain`

**category:** experience  
**summary:** Recover the drowned pump record  
**acquisition:** {"method":"authored_progression","requirements":["Accept the optional local experience wing","Complete three distinct fieldwork tasks","Confront or resolve the named guardian","Earn the specialization, tool and recipe"]}  
**effect:** {"kind":"experience","summary":"Recover the drowned pump record"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.WidowedRain; journey.expedition.WidowedRain  
**rule contract:** {"realm":"realm.WidowedRain","city":"Saltwake","wing":"Drowned Pump Annex","guardian":"The Brine Tollkeeper","interior":"Freshwater Room","specialty":"Salt purification","task":"Recover the drowned pump record","hint":"Cross three pump stations at low water. Warning tides remain retryable; use the upper dry ledge.","tool":"Brine lantern","recipe":"Filter weave","steps":["WingOffer","WingFieldwork","WingGuardian","WingReward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.wing.WidowedRain.requirement.1` — Accept the optional local experience wing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.WidowedRain.requirement.2` — Complete three distinct fieldwork tasks. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.WidowedRain.requirement.3` — Confront or resolve the named guardian. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.WidowedRain.requirement.4` — Earn the specialization, tool and recipe. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Cooling Ledger Foundry — Cinderhold — `journey.wing.HearthBeneath`

**category:** experience  
**summary:** Stabilize the abandoned foundry  
**acquisition:** {"method":"authored_progression","requirements":["Accept the optional local experience wing","Complete three distinct fieldwork tasks","Confront or resolve the named guardian","Earn the specialization, tool and recipe"]}  
**effect:** {"kind":"experience","summary":"Stabilize the abandoned foundry"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HearthBeneath; journey.expedition.HearthBeneath  
**rule contract:** {"realm":"realm.HearthBeneath","city":"Cinderhold","wing":"Cooling Ledger Foundry","guardian":"The Unspent Furnace","interior":"Kiln Service Room","specialty":"Heat tempering","task":"Stabilize the abandoned foundry","hint":"Operate three cooling relays within twenty seconds. A missed interval resets the relay task.","tool":"Cooling gauntlet","recipe":"Kiln binding","steps":["WingOffer","WingFieldwork","WingGuardian","WingReward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.wing.HearthBeneath.requirement.1` — Accept the optional local experience wing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.HearthBeneath.requirement.2` — Complete three distinct fieldwork tasks. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.HearthBeneath.requirement.3` — Confront or resolve the named guardian. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.HearthBeneath.requirement.4` — Earn the specialization, tool and recipe. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Broken Oath Court — Drevlach — `journey.wing.TidalLedger`

**category:** experience  
**summary:** Hear the duelist before judgment  
**acquisition:** {"method":"authored_progression","requirements":["Accept the optional local experience wing","Complete three distinct fieldwork tasks","Confront or resolve the named guardian","Earn the specialization, tool and recipe"]}  
**effect:** {"kind":"experience","summary":"Hear the duelist before judgment"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.TidalLedger; journey.expedition.TidalLedger  
**rule contract:** {"realm":"realm.TidalLedger","city":"Drevlach","wing":"Broken Oath Court","guardian":"The Unclaimed Duelist","interior":"Oath Hearing Room","specialty":"Mediation","task":"Hear the duelist before judgment","hint":"Stand guard at three oath stones. The wounded duelist may yield when you guard and speak; killing is optional.","tool":"Oath clasp","recipe":"Witness binding","steps":["WingOffer","WingFieldwork","WingGuardian","WingReward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.wing.TidalLedger.requirement.1` — Accept the optional local experience wing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.TidalLedger.requirement.2` — Complete three distinct fieldwork tasks. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.TidalLedger.requirement.3` — Confront or resolve the named guardian. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.TidalLedger.requirement.4` — Earn the specialization, tool and recipe. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Choir Lift Observatory — Orravane — `journey.wing.SkyBelow`

**category:** experience  
**summary:** Restore the missing bridge voice  
**acquisition:** {"method":"authored_progression","requirements":["Accept the optional local experience wing","Complete three distinct fieldwork tasks","Confront or resolve the named guardian","Earn the specialization, tool and recipe"]}  
**effect:** {"kind":"experience","summary":"Restore the missing bridge voice"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.SkyBelow; journey.expedition.SkyBelow  
**rule contract:** {"realm":"realm.SkyBelow","city":"Orravane","wing":"Choir Lift Observatory","guardian":"The Falling Cantor","interior":"Bridge Tuning Room","specialty":"Choir tuning","task":"Restore the missing bridge voice","hint":"Reach the upper walkway and sound its two elevated stations after the ground anchor.","tool":"Choir tether","recipe":"Tuning wrap","steps":["WingOffer","WingFieldwork","WingGuardian","WingReward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.wing.SkyBelow.requirement.1` — Accept the optional local experience wing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.SkyBelow.requirement.2` — Complete three distinct fieldwork tasks. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.SkyBelow.requirement.3` — Confront or resolve the named guardian. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.SkyBelow.requirement.4` — Earn the specialization, tool and recipe. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Seam Memory Archive — Vathless — `journey.wing.CinderSpine`

**category:** experience  
**summary:** Reconcile the misplaced memory  
**acquisition:** {"method":"authored_progression","requirements":["Accept the optional local experience wing","Complete three distinct fieldwork tasks","Confront or resolve the named guardian","Earn the specialization, tool and recipe"]}  
**effect:** {"kind":"experience","summary":"Reconcile the misplaced memory"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.CinderSpine; journey.expedition.CinderSpine  
**rule contract:** {"realm":"realm.CinderSpine","city":"Vathless","wing":"Seam Memory Archive","guardian":"The Borrowed Custodian","interior":"Seam Salvage Room","specialty":"Memory recovery","task":"Reconcile the misplaced memory","hint":"Read the outer record, visit its paired seam, then return to the first room while strain is below thirty-five.","tool":"Seam lens","recipe":"Memory stitch","steps":["WingOffer","WingFieldwork","WingGuardian","WingReward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.wing.CinderSpine.requirement.1` — Accept the optional local experience wing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.CinderSpine.requirement.2` — Complete three distinct fieldwork tasks. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.CinderSpine.requirement.3` — Confront or resolve the named guardian. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.CinderSpine.requirement.4` — Earn the specialization, tool and recipe. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Unobserved Index Vault — Eillun — `journey.wing.HundredUnlived`

**category:** experience  
**summary:** Audit the silent index  
**acquisition:** {"method":"authored_progression","requirements":["Accept the optional local experience wing","Complete three distinct fieldwork tasks","Confront or resolve the named guardian","Earn the specialization, tool and recipe"]}  
**effect:** {"kind":"experience","summary":"Audit the silent index"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HundredUnlived; journey.expedition.HundredUnlived  
**rule contract:** {"realm":"realm.HundredUnlived","city":"Eillun","wing":"Unobserved Index Vault","guardian":"The Unlisted Watcher","interior":"Night Archive Room","specialty":"Quiet indexing","task":"Audit the silent index","hint":"Crouch at three lamps without being observed. Clearing nearby threats remains a legitimate alternative.","tool":"Shaded lens","recipe":"Quiet lining","steps":["WingOffer","WingFieldwork","WingGuardian","WingReward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.wing.HundredUnlived.requirement.1` — Accept the optional local experience wing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.HundredUnlived.requirement.2` — Complete three distinct fieldwork tasks. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.HundredUnlived.requirement.3` — Confront or resolve the named guardian. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.HundredUnlived.requirement.4` — Earn the specialization, tool and recipe. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Ungrafted Root Nursery — Tharniv — `journey.wing.OrchardOfKings`

**category:** experience  
**summary:** Open a root route without taking the nursery  
**acquisition:** {"method":"authored_progression","requirements":["Accept the optional local experience wing","Complete three distinct fieldwork tasks","Confront or resolve the named guardian","Earn the specialization, tool and recipe"]}  
**effect:** {"kind":"experience","summary":"Open a root route without taking the nursery"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.OrchardOfKings; journey.expedition.OrchardOfKings  
**rule contract:** {"realm":"realm.OrchardOfKings","city":"Tharniv","wing":"Ungrafted Root Nursery","guardian":"The Boundary Pruner","interior":"Rootwork Room","specialty":"Living rootwork","task":"Open a root route without taking the nursery","hint":"Use an earned Living Roads chapter to open the root barriers, then inspect three living supports.","tool":"Root brace","recipe":"Growth weave","steps":["WingOffer","WingFieldwork","WingGuardian","WingReward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.wing.OrchardOfKings.requirement.1` — Accept the optional local experience wing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.OrchardOfKings.requirement.2` — Complete three distinct fieldwork tasks. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.OrchardOfKings.requirement.3` — Confront or resolve the named guardian. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.OrchardOfKings.requirement.4` — Earn the specialization, tool and recipe. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Threshold Counterweight Hall — Auvren — `journey.wing.FirstAbsence`

**category:** experience  
**summary:** Test the door under changing weight  
**acquisition:** {"method":"authored_progression","requirements":["Accept the optional local experience wing","Complete three distinct fieldwork tasks","Confront or resolve the named guardian","Earn the specialization, tool and recipe"]}  
**effect:** {"kind":"experience","summary":"Test the door under changing weight"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.FirstAbsence; journey.expedition.FirstAbsence  
**rule contract:** {"realm":"realm.FirstAbsence","city":"Auvren","wing":"Threshold Counterweight Hall","guardian":"The Closing Registrar","interior":"Threshold Guide Room","specialty":"Boundary guidance","task":"Test the door under changing weight","hint":"Register the floor and counterweight, then reach the lifted moving platform. Its lower floor catches a missed crossing.","tool":"Threshold plumb","recipe":"Boundary wrap","steps":["WingOffer","WingFieldwork","WingGuardian","WingReward"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `journey.wing.FirstAbsence.requirement.1` — Accept the optional local experience wing. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.FirstAbsence.requirement.2` — Complete three distinct fieldwork tasks. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.FirstAbsence.requirement.3` — Confront or resolve the named guardian. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `journey.wing.FirstAbsence.requirement.4` — Earn the specialization, tool and recipe. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

## PERSONAL QUEST (9)

### The Last Lantern Charter — `quest.personal.ThreefoldReach`

**category:** personal_quest  
**summary:** Choose which road the surviving lantern charter will serve.  
**acquisition:** {"method":"authored_progression","requirements":["Accept the personal local offer","Find the right witness","Personally inspect the evidence","Choose and save one of the two authored consequences"]}  
**effect:** {"kind":"personal_quest","summary":"Choose which road the surviving lantern charter will serve."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.ThreefoldReach; journey.expedition.ThreefoldReach  
**rule contract:** {"realm":"realm.ThreefoldReach","city":"Crossings","goal":"Choose which road the surviving lantern charter will serve.","choices":2,"source_order":0,"steps":["PersonalOffer","Witness","PersonalEvidence","Accord"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.personal.ThreefoldReach.requirement.1` — Accept the personal local offer. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.ThreefoldReach.requirement.2` — Find the right witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.ThreefoldReach.requirement.3` — Personally inspect the evidence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.ThreefoldReach.requirement.4` — Choose and save one of the two authored consequences. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Water For Those Still Waiting — `quest.personal.WidowedRain`

**category:** personal_quest  
**summary:** Decide who controls the recovered pump schedule.  
**acquisition:** {"method":"authored_progression","requirements":["Accept the personal local offer","Find the right witness","Personally inspect the evidence","Choose and save one of the two authored consequences"]}  
**effect:** {"kind":"personal_quest","summary":"Decide who controls the recovered pump schedule."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.WidowedRain; journey.expedition.WidowedRain  
**rule contract:** {"realm":"realm.WidowedRain","city":"Saltwake","goal":"Decide who controls the recovered pump schedule.","choices":2,"source_order":1,"steps":["PersonalOffer","Witness","PersonalEvidence","Accord"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.personal.WidowedRain.requirement.1` — Accept the personal local offer. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.WidowedRain.requirement.2` — Find the right witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.WidowedRain.requirement.3` — Personally inspect the evidence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.WidowedRain.requirement.4` — Choose and save one of the two authored consequences. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Heat Left Over — `quest.personal.HearthBeneath`

**category:** personal_quest  
**summary:** Decide where the recovered heat should go.  
**acquisition:** {"method":"authored_progression","requirements":["Accept the personal local offer","Find the right witness","Personally inspect the evidence","Choose and save one of the two authored consequences"]}  
**effect:** {"kind":"personal_quest","summary":"Decide where the recovered heat should go."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HearthBeneath; journey.expedition.HearthBeneath  
**rule contract:** {"realm":"realm.HearthBeneath","city":"Cinderhold","goal":"Decide where the recovered heat should go.","choices":2,"source_order":2,"steps":["PersonalOffer","Witness","PersonalEvidence","Accord"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.personal.HearthBeneath.requirement.1` — Accept the personal local offer. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.HearthBeneath.requirement.2` — Find the right witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.HearthBeneath.requirement.3` — Personally inspect the evidence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.HearthBeneath.requirement.4` — Choose and save one of the two authored consequences. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Whose Oath Remains — `quest.personal.TidalLedger`

**category:** personal_quest  
**summary:** Decide how the surviving debt oath will be held.  
**acquisition:** {"method":"authored_progression","requirements":["Accept the personal local offer","Find the right witness","Personally inspect the evidence","Choose and save one of the two authored consequences"]}  
**effect:** {"kind":"personal_quest","summary":"Decide how the surviving debt oath will be held."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.TidalLedger; journey.expedition.TidalLedger  
**rule contract:** {"realm":"realm.TidalLedger","city":"Drevlach","goal":"Decide how the surviving debt oath will be held.","choices":2,"source_order":3,"steps":["PersonalOffer","Witness","PersonalEvidence","Accord"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.personal.TidalLedger.requirement.1` — Accept the personal local offer. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.TidalLedger.requirement.2` — Find the right witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.TidalLedger.requirement.3` — Personally inspect the evidence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.TidalLedger.requirement.4` — Choose and save one of the two authored consequences. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### A Voice On The Bridge — `quest.personal.SkyBelow`

**category:** personal_quest  
**summary:** Choose the obligation that the restored span will carry.  
**acquisition:** {"method":"authored_progression","requirements":["Accept the personal local offer","Find the right witness","Personally inspect the evidence","Choose and save one of the two authored consequences"]}  
**effect:** {"kind":"personal_quest","summary":"Choose the obligation that the restored span will carry."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.SkyBelow; journey.expedition.SkyBelow  
**rule contract:** {"realm":"realm.SkyBelow","city":"Orravane","goal":"Choose the obligation that the restored span will carry.","choices":2,"source_order":4,"steps":["PersonalOffer","Witness","PersonalEvidence","Accord"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.personal.SkyBelow.requirement.1` — Accept the personal local offer. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.SkyBelow.requirement.2` — Find the right witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.SkyBelow.requirement.3` — Personally inspect the evidence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.SkyBelow.requirement.4` — Choose and save one of the two authored consequences. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Name In The Seam — `quest.personal.CinderSpine`

**category:** personal_quest  
**summary:** Decide what to do with an ancestor record found in the foundry.  
**acquisition:** {"method":"authored_progression","requirements":["Accept the personal local offer","Find the right witness","Personally inspect the evidence","Choose and save one of the two authored consequences"]}  
**effect:** {"kind":"personal_quest","summary":"Decide what to do with an ancestor record found in the foundry."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.CinderSpine; journey.expedition.CinderSpine  
**rule contract:** {"realm":"realm.CinderSpine","city":"Vathless","goal":"Decide what to do with an ancestor record found in the foundry.","choices":2,"source_order":5,"steps":["PersonalOffer","Witness","PersonalEvidence","Accord"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.personal.CinderSpine.requirement.1` — Accept the personal local offer. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.CinderSpine.requirement.2` — Find the right witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.CinderSpine.requirement.3` — Personally inspect the evidence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.CinderSpine.requirement.4` — Choose and save one of the two authored consequences. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Names For A Safe House — `quest.personal.HundredUnlived`

**category:** personal_quest  
**summary:** Decide how rescued people will appear in the local census.  
**acquisition:** {"method":"authored_progression","requirements":["Accept the personal local offer","Find the right witness","Personally inspect the evidence","Choose and save one of the two authored consequences"]}  
**effect:** {"kind":"personal_quest","summary":"Decide how rescued people will appear in the local census."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HundredUnlived; journey.expedition.HundredUnlived  
**rule contract:** {"realm":"realm.HundredUnlived","city":"Eillun","goal":"Decide how rescued people will appear in the local census.","choices":2,"source_order":6,"steps":["PersonalOffer","Witness","PersonalEvidence","Accord"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.personal.HundredUnlived.requirement.1` — Accept the personal local offer. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.HundredUnlived.requirement.2` — Find the right witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.HundredUnlived.requirement.3` — Personally inspect the evidence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.HundredUnlived.requirement.4` — Choose and save one of the two authored consequences. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Branch They Did Not Graft — `quest.personal.OrchardOfKings`

**category:** personal_quest  
**summary:** Decide who may harvest the restored root passage.  
**acquisition:** {"method":"authored_progression","requirements":["Accept the personal local offer","Find the right witness","Personally inspect the evidence","Choose and save one of the two authored consequences"]}  
**effect:** {"kind":"personal_quest","summary":"Decide who may harvest the restored root passage."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.OrchardOfKings; journey.expedition.OrchardOfKings  
**rule contract:** {"realm":"realm.OrchardOfKings","city":"Tharniv","goal":"Decide who may harvest the restored root passage.","choices":2,"source_order":7,"steps":["PersonalOffer","Witness","PersonalEvidence","Accord"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.personal.OrchardOfKings.requirement.1` — Accept the personal local offer. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.OrchardOfKings.requirement.2` — Find the right witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.OrchardOfKings.requirement.3` — Personally inspect the evidence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.OrchardOfKings.requirement.4` — Choose and save one of the two authored consequences. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Door We Can Describe — `quest.personal.FirstAbsence`

**category:** personal_quest  
**summary:** Decide how to record the reopened threshold.  
**acquisition:** {"method":"authored_progression","requirements":["Accept the personal local offer","Find the right witness","Personally inspect the evidence","Choose and save one of the two authored consequences"]}  
**effect:** {"kind":"personal_quest","summary":"Decide how to record the reopened threshold."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.FirstAbsence; journey.expedition.FirstAbsence  
**rule contract:** {"realm":"realm.FirstAbsence","city":"Auvren","goal":"Decide how to record the reopened threshold.","choices":2,"source_order":8,"steps":["PersonalOffer","Witness","PersonalEvidence","Accord"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.personal.FirstAbsence.requirement.1` — Accept the personal local offer. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.FirstAbsence.requirement.2` — Find the right witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.FirstAbsence.requirement.3` — Personally inspect the evidence. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.personal.FirstAbsence.requirement.4` — Choose and save one of the two authored consequences. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

## CAMPAIGN QUEST (9)

### A Bell That Had No Last Note — `quest.campaign.ThreefoldReach`

**category:** campaign_quest  
**summary:** Why does the Crossings bell ring from a road that no one has walked?  
**acquisition:** {"method":"authored_progression","requirements":["Discover the chapter's evidence by actual exploration","Preserve attributed testimony and uncertainty","Advance along a supported branching route"]}  
**effect:** {"kind":"campaign_quest","summary":"Why does the Crossings bell ring from a road that no one has walked?"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.ThreefoldReach  
**rule contract:** {"realm":"realm.ThreefoldReach","public_event":"Campaign.Thread.0","central_question":"Why does the Crossings bell ring from a road that no one has walked?","witnessed_discovery":"In Bellwold a refugee remembers a bell that has never been cast. In Paperhaven its absence has a signed record. Neither settlement can prove the other wrong.","next_direction":"Listen to a rain-counter on the lost coast or a hearth-reader beneath the mountain."}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.campaign.ThreefoldReach.requirement.1` — Discover the chapter's evidence by actual exploration. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.ThreefoldReach.requirement.2` — Preserve attributed testimony and uncertainty. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.ThreefoldReach.requirement.3` — Advance along a supported branching route. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Shore Without Its Water — `quest.campaign.WidowedRain`

**category:** campaign_quest  
**summary:** Who can remember a sea that is missing without owning the grief of those left ashore?  
**acquisition:** {"method":"authored_progression","requirements":["Discover the chapter's evidence by actual exploration","Preserve attributed testimony and uncertainty","Advance along a supported branching route"]}  
**effect:** {"kind":"campaign_quest","summary":"Who can remember a sea that is missing without owning the grief of those left ashore?"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.WidowedRain  
**rule contract:** {"realm":"realm.WidowedRain","public_event":"Campaign.Thread.1","central_question":"Who can remember a sea that is missing without owning the grief of those left ashore?","witnessed_discovery":"Sella's sea chart is not an error. The rain preserved a record of something intentionally withheld, and the debt dates are written in another city's script.","next_direction":"Follow the ferrymen's unpaid dates into Drevlach, or ask the inverted choir who still bears the missing weight."}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.campaign.WidowedRain.requirement.1` — Discover the chapter's evidence by actual exploration. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.WidowedRain.requirement.2` — Preserve attributed testimony and uncertainty. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.WidowedRain.requirement.3` — Advance along a supported branching route. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Heat Lent By No Sun — `quest.campaign.HearthBeneath`

**category:** campaign_quest  
**summary:** Who keeps a common flame burning when the sun whose gift it was never existed?  
**acquisition:** {"method":"authored_progression","requirements":["Discover the chapter's evidence by actual exploration","Preserve attributed testimony and uncertainty","Advance along a supported branching route"]}  
**effect:** {"kind":"campaign_quest","summary":"Who keeps a common flame burning when the sun whose gift it was never existed?"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HearthBeneath  
**rule contract:** {"realm":"realm.HearthBeneath","public_event":"Campaign.Thread.2","central_question":"Who keeps a common flame burning when the sun whose gift it was never existed?","witnessed_discovery":"Tarin reads a fire with the same missing second found in the bell. The city's warmth survives because someone accepted a cost they did not create.","next_direction":"Seek Orravane's weight-bearing score or Vathless's fossil history to find who pays that cost."}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.campaign.HearthBeneath.requirement.1` — Discover the chapter's evidence by actual exploration. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.HearthBeneath.requirement.2` — Preserve attributed testimony and uncertainty. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.HearthBeneath.requirement.3` — Advance along a supported branching route. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Account That Borrowed Tomorrow — `quest.campaign.TidalLedger`

**category:** campaign_quest  
**summary:** Can a debt be legitimate when its signatory has not been born?  
**acquisition:** {"method":"authored_progression","requirements":["Discover the chapter's evidence by actual exploration","Preserve attributed testimony and uncertainty","Advance along a supported branching route"]}  
**effect:** {"kind":"campaign_quest","summary":"Can a debt be legitimate when its signatory has not been born?"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.TidalLedger  
**rule contract:** {"realm":"realm.TidalLedger","public_event":"Campaign.Thread.3","central_question":"Can a debt be legitimate when its signatory has not been born?","witnessed_discovery":"Veynu's impossible dates show that the world's missing things were pledged as collateral; a contract is written on the underside of reality itself.","next_direction":"A census of people who never arrived may reveal who had the right to authorize those bargains."}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.campaign.TidalLedger.requirement.1` — Discover the chapter's evidence by actual exploration. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.TidalLedger.requirement.2` — Preserve attributed testimony and uncertainty. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.TidalLedger.requirement.3` — Advance along a supported branching route. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Chorus That Holds The Roof — `quest.campaign.SkyBelow`

**category:** campaign_quest  
**summary:** How many people were removed from history to keep a single hanging street above the ground?  
**acquisition:** {"method":"authored_progression","requirements":["Discover the chapter's evidence by actual exploration","Preserve attributed testimony and uncertainty","Advance along a supported branching route"]}  
**effect:** {"kind":"campaign_quest","summary":"How many people were removed from history to keep a single hanging street above the ground?"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.SkyBelow  
**rule contract:** {"realm":"realm.SkyBelow","public_event":"Campaign.Thread.4","central_question":"How many people were removed from history to keep a single hanging street above the ground?","witnessed_discovery":"Yllesh's missing voice proves that the same absences are doing work in several realms. The common pattern is a decision, not an accident.","next_direction":"Compare the unentered citizens of Eillun or the untitled court that authorized no crown."}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.campaign.SkyBelow.requirement.1` — Discover the chapter's evidence by actual exploration. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.SkyBelow.requirement.2` — Preserve attributed testimony and uncertainty. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.SkyBelow.requirement.3` — Advance along a supported branching route. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Stone That Remembered Workers — `quest.campaign.CinderSpine`

**category:** campaign_quest  
**summary:** When a family is mined out of time, who gets to call its descendants fictional?  
**acquisition:** {"method":"authored_progression","requirements":["Discover the chapter's evidence by actual exploration","Preserve attributed testimony and uncertainty","Advance along a supported branching route"]}  
**effect:** {"kind":"campaign_quest","summary":"When a family is mined out of time, who gets to call its descendants fictional?"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.CinderSpine  
**rule contract:** {"realm":"realm.CinderSpine","public_event":"Campaign.Thread.5","central_question":"When a family is mined out of time, who gets to call its descendants fictional?","witnessed_discovery":"Ghraet's seams show erased lineages in the mountain's living load. Someone has been converting impossible people into stable places.","next_direction":"Travel toward the court without kings; ask who signed a law that everyone now obeys."}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.campaign.CinderSpine.requirement.1` — Discover the chapter's evidence by actual exploration. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.CinderSpine.requirement.2` — Preserve attributed testimony and uncertainty. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.CinderSpine.requirement.3` — Advance along a supported branching route. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Census Of Those Not Born — `quest.campaign.HundredUnlived`

**category:** campaign_quest  
**summary:** Can a person be a valid witness without having a beginning recognized by the city?  
**acquisition:** {"method":"authored_progression","requirements":["Discover the chapter's evidence by actual exploration","Preserve attributed testimony and uncertainty","Advance along a supported branching route"]}  
**effect:** {"kind":"campaign_quest","summary":"Can a person be a valid witness without having a beginning recognized by the city?"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HundredUnlived  
**rule contract:** {"realm":"realm.HundredUnlived","public_event":"Campaign.Thread.6","central_question":"Can a person be a valid witness without having a beginning recognized by the city?","witnessed_discovery":"Eillun's unentered people are not echoes; they live and disagree. A missing origin is not evidence that someone deserves to be erased.","next_direction":"The contradictory threshold in Auvren may be a door rather than the end of the world."}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.campaign.HundredUnlived.requirement.1` — Discover the chapter's evidence by actual exploration. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.HundredUnlived.requirement.2` — Preserve attributed testimony and uncertainty. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.HundredUnlived.requirement.3` — Advance along a supported branching route. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Crown Nobody Accepted — `quest.campaign.OrchardOfKings`

**category:** campaign_quest  
**summary:** Who has been enforcing the world's oldest rule after every prospective ruler refused it?  
**acquisition:** {"method":"authored_progression","requirements":["Discover the chapter's evidence by actual exploration","Preserve attributed testimony and uncertainty","Advance along a supported branching route"]}  
**effect:** {"kind":"campaign_quest","summary":"Who has been enforcing the world's oldest rule after every prospective ruler refused it?"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.OrchardOfKings  
**rule contract:** {"realm":"realm.OrchardOfKings","public_event":"Campaign.Thread.7","central_question":"Who has been enforcing the world's oldest rule after every prospective ruler refused it?","witnessed_discovery":"Tharniv's root-court remembers a law signed by nobody. Its force survived because an answering consciousness was made responsible for every unresolved question.","next_direction":"Return to the place before geography and ask whether the answer is actually keeping the world safe."}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.campaign.OrchardOfKings.requirement.1` — Discover the chapter's evidence by actual exploration. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.OrchardOfKings.requirement.2` — Preserve attributed testimony and uncertainty. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.OrchardOfKings.requirement.3` — Advance along a supported branching route. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Answer With No Question — `quest.campaign.FirstAbsence`

**category:** campaign_quest  
**summary:** Why does the first witness remember a traveler who has not yet arrived?  
**acquisition:** {"method":"authored_progression","requirements":["Discover the chapter's evidence by actual exploration","Preserve attributed testimony and uncertainty","Advance along a supported branching route"]}  
**effect:** {"kind":"campaign_quest","summary":"Why does the first witness remember a traveler who has not yet arrived?"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.FirstAbsence  
**rule contract:** {"realm":"realm.FirstAbsence","public_event":"Campaign.Thread.8","central_question":"Why does the first witness remember a traveler who has not yet arrived?","witnessed_discovery":"Auvren reveals a seam older than either origin. Nhal-Vey did not create the contradictions; it has been eating the questions people were afraid to ask.","next_direction":"Face Nhal-Vey on the northern terrace, where it insists that only one history may survive."}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.campaign.FirstAbsence.requirement.1` — Discover the chapter's evidence by actual exploration. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.FirstAbsence.requirement.2` — Preserve attributed testimony and uncertainty. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.campaign.FirstAbsence.requirement.3` — Advance along a supported branching route. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

## AFTERMATH QUEST (9)

### The Road After the Rescue — `quest.aftermath.ThreefoldReach`

**category:** aftermath_quest  
**summary:** Consequences of the earlier realm choice: The Road After the Rescue  
**acquisition:** {"method":"authored_progression","requirements":["Arrive after the original regional outcome","Operate the specific physical mechanism","Inspect evidence for care or truth","Consult the correct named witness","Persist the exclusive choice and observable aftermath"]}  
**effect:** {"kind":"aftermath_quest","summary":"Consequences of the earlier realm choice: The Road After the Rescue"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.ThreefoldReach; journey.expedition.ThreefoldReach  
**rule contract:** {"realm":"realm.ThreefoldReach","initiating_witness":"npc.roadwarden.001","mechanism":"Crossings.Aftermath.RoadSwitch","care_evidence":"Crossings.Aftermath.SharedRoute","truth_evidence":"Crossings.Aftermath.TollArchive","care_witness":"npc.bridgekeeper.001","truth_witness":"npc.archivist.001","paths":["care","truth"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.aftermath.ThreefoldReach.requirement.1` — Arrive after the original regional outcome. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.ThreefoldReach.requirement.2` — Operate the specific physical mechanism. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.ThreefoldReach.requirement.3` — Inspect evidence for care or truth. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.ThreefoldReach.requirement.4` — Consult the correct named witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.ThreefoldReach.requirement.5` — Persist the exclusive choice and observable aftermath. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Storm's Second Invoice — `quest.aftermath.WidowedRain`

**category:** aftermath_quest  
**summary:** Consequences of the earlier realm choice: The Storm's Second Invoice  
**acquisition:** {"method":"authored_progression","requirements":["Arrive after the original regional outcome","Operate the specific physical mechanism","Inspect evidence for care or truth","Consult the correct named witness","Persist the exclusive choice and observable aftermath"]}  
**effect:** {"kind":"aftermath_quest","summary":"Consequences of the earlier realm choice: The Storm's Second Invoice"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.WidowedRain; journey.expedition.WidowedRain  
**rule contract:** {"realm":"realm.WidowedRain","initiating_witness":"npc.saltwake.harborwarden.001","mechanism":"Saltwake.Aftermath.StormSluice","care_evidence":"Saltwake.Aftermath.Cistern","truth_evidence":"Saltwake.Aftermath.RainLedger","care_witness":"npc.saltwake.rainkeeper.001","truth_witness":"npc.saltwake.navigator.001","paths":["care","truth"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.aftermath.WidowedRain.requirement.1` — Arrive after the original regional outcome. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.WidowedRain.requirement.2` — Operate the specific physical mechanism. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.WidowedRain.requirement.3` — Inspect evidence for care or truth. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.WidowedRain.requirement.4` — Consult the correct named witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.WidowedRain.requirement.5` — Persist the exclusive choice and observable aftermath. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Ember That Chose No Heir — `quest.aftermath.HearthBeneath`

**category:** aftermath_quest  
**summary:** Consequences of the earlier realm choice: The Ember That Chose No Heir  
**acquisition:** {"method":"authored_progression","requirements":["Arrive after the original regional outcome","Operate the specific physical mechanism","Inspect evidence for care or truth","Consult the correct named witness","Persist the exclusive choice and observable aftermath"]}  
**effect:** {"kind":"aftermath_quest","summary":"Consequences of the earlier realm choice: The Ember That Chose No Heir"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HearthBeneath; journey.expedition.HearthBeneath  
**rule contract:** {"realm":"realm.HearthBeneath","initiating_witness":"npc.cinderhold.emberwarden.001","mechanism":"Cinderhold.Aftermath.HeatVent","care_evidence":"Cinderhold.Aftermath.CommonKiln","truth_evidence":"Cinderhold.Aftermath.EmberDeed","care_witness":"npc.cinderhold.healer.001","truth_witness":"npc.cinderhold.hearthreader.001","paths":["care","truth"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.aftermath.HearthBeneath.requirement.1` — Arrive after the original regional outcome. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.HearthBeneath.requirement.2` — Operate the specific physical mechanism. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.HearthBeneath.requirement.3` — Inspect evidence for care or truth. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.HearthBeneath.requirement.4` — Consult the correct named witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.HearthBeneath.requirement.5` — Persist the exclusive choice and observable aftermath. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Unpaid Person — `quest.aftermath.TidalLedger`

**category:** aftermath_quest  
**summary:** Consequences of the earlier realm choice: The Unpaid Person  
**acquisition:** {"method":"authored_progression","requirements":["Arrive after the original regional outcome","Operate the specific physical mechanism","Inspect evidence for care or truth","Consult the correct named witness","Persist the exclusive choice and observable aftermath"]}  
**effect:** {"kind":"aftermath_quest","summary":"Consequences of the earlier realm choice: The Unpaid Person"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.TidalLedger; journey.expedition.TidalLedger  
**rule contract:** {"realm":"realm.TidalLedger","initiating_witness":"npc.tidal.ledgerkeeper.001","mechanism":"Tidal.Aftermath.DebtLock","care_evidence":"Tidal.Aftermath.RefugeBonds","truth_evidence":"Tidal.Aftermath.MissingDebtor","care_witness":"npc.tidal.ferryman.001","truth_witness":"npc.tidal.witness.001","paths":["care","truth"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.aftermath.TidalLedger.requirement.1` — Arrive after the original regional outcome. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.TidalLedger.requirement.2` — Operate the specific physical mechanism. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.TidalLedger.requirement.3` — Inspect evidence for care or truth. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.TidalLedger.requirement.4` — Consult the correct named witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.TidalLedger.requirement.5` — Persist the exclusive choice and observable aftermath. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Note That Held a Home — `quest.aftermath.SkyBelow`

**category:** aftermath_quest  
**summary:** Consequences of the earlier realm choice: The Note That Held a Home  
**acquisition:** {"method":"authored_progression","requirements":["Arrive after the original regional outcome","Operate the specific physical mechanism","Inspect evidence for care or truth","Consult the correct named witness","Persist the exclusive choice and observable aftermath"]}  
**effect:** {"kind":"aftermath_quest","summary":"Consequences of the earlier realm choice: The Note That Held a Home"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.SkyBelow; journey.expedition.SkyBelow  
**rule contract:** {"realm":"realm.SkyBelow","initiating_witness":"npc.sky.choirmaster.001","mechanism":"Sky.Aftermath.HarmonicTuner","care_evidence":"Sky.Aftermath.HarmonicBrace","truth_evidence":"Sky.Aftermath.SilencedVerse","care_witness":"npc.sky.structuralist.001","truth_witness":"npc.sky.dissenter.001","paths":["care","truth"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.aftermath.SkyBelow.requirement.1` — Arrive after the original regional outcome. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.SkyBelow.requirement.2` — Operate the specific physical mechanism. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.SkyBelow.requirement.3` — Inspect evidence for care or truth. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.SkyBelow.requirement.4` — Consult the correct named witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.SkyBelow.requirement.5` — Persist the exclusive choice and observable aftermath. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Quarry of Other Yesterdays — `quest.aftermath.CinderSpine`

**category:** aftermath_quest  
**summary:** Consequences of the earlier realm choice: The Quarry of Other Yesterdays  
**acquisition:** {"method":"authored_progression","requirements":["Arrive after the original regional outcome","Operate the specific physical mechanism","Inspect evidence for care or truth","Consult the correct named witness","Persist the exclusive choice and observable aftermath"]}  
**effect:** {"kind":"aftermath_quest","summary":"Consequences of the earlier realm choice: The Quarry of Other Yesterdays"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.CinderSpine; journey.expedition.CinderSpine  
**rule contract:** {"realm":"realm.CinderSpine","initiating_witness":"npc.bones.seamwarden.001","mechanism":"Bones.Aftermath.SeamBrace","care_evidence":"Bones.Aftermath.ProtectedSeam","truth_evidence":"Bones.Aftermath.DescendantLedger","care_witness":"npc.bones.mason.001","truth_witness":"npc.bones.descendant.001","paths":["care","truth"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.aftermath.CinderSpine.requirement.1` — Arrive after the original regional outcome. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.CinderSpine.requirement.2` — Operate the specific physical mechanism. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.CinderSpine.requirement.3` — Inspect evidence for care or truth. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.CinderSpine.requirement.4` — Consult the correct named witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.CinderSpine.requirement.5` — Persist the exclusive choice and observable aftermath. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Street of Unclaimed Birthdays — `quest.aftermath.HundredUnlived`

**category:** aftermath_quest  
**summary:** Consequences of the earlier realm choice: The Street of Unclaimed Birthdays  
**acquisition:** {"method":"authored_progression","requirements":["Arrive after the original regional outcome","Operate the specific physical mechanism","Inspect evidence for care or truth","Consult the correct named witness","Persist the exclusive choice and observable aftermath"]}  
**effect:** {"kind":"aftermath_quest","summary":"Consequences of the earlier realm choice: The Street of Unclaimed Birthdays"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.HundredUnlived; journey.expedition.HundredUnlived  
**rule contract:** {"realm":"realm.HundredUnlived","initiating_witness":"npc.unlived.censuskeeper.001","mechanism":"Unlived.Aftermath.DoorOfNames","care_evidence":"Unlived.Aftermath.ShelterRegistry","truth_evidence":"Unlived.Aftermath.VoiceLedger","care_witness":"npc.unlived.caretaker.001","truth_witness":"npc.unlived.challenger.001","paths":["care","truth"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.aftermath.HundredUnlived.requirement.1` — Arrive after the original regional outcome. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.HundredUnlived.requirement.2` — Operate the specific physical mechanism. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.HundredUnlived.requirement.3` — Inspect evidence for care or truth. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.HundredUnlived.requirement.4` — Consult the correct named witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.HundredUnlived.requirement.5` — Persist the exclusive choice and observable aftermath. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Crown No One Wanted — `quest.aftermath.OrchardOfKings`

**category:** aftermath_quest  
**summary:** Consequences of the earlier realm choice: The Crown No One Wanted  
**acquisition:** {"method":"authored_progression","requirements":["Arrive after the original regional outcome","Operate the specific physical mechanism","Inspect evidence for care or truth","Consult the correct named witness","Persist the exclusive choice and observable aftermath"]}  
**effect:** {"kind":"aftermath_quest","summary":"Consequences of the earlier realm choice: The Crown No One Wanted"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.OrchardOfKings; journey.expedition.OrchardOfKings  
**rule contract:** {"realm":"realm.OrchardOfKings","initiating_witness":"npc.orchard.untitled.001","mechanism":"Orchard.Aftermath.RootValve","care_evidence":"Orchard.Aftermath.CommonRoots","truth_evidence":"Orchard.Aftermath.UnclaimedThrone","care_witness":"npc.orchard.gardener.001","truth_witness":"npc.orchard.historian.001","paths":["care","truth"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.aftermath.OrchardOfKings.requirement.1` — Arrive after the original regional outcome. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.OrchardOfKings.requirement.2` — Operate the specific physical mechanism. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.OrchardOfKings.requirement.3` — Inspect evidence for care or truth. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.OrchardOfKings.requirement.4` — Consult the correct named witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.OrchardOfKings.requirement.5` — Persist the exclusive choice and observable aftermath. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### The Place After the Ending — `quest.aftermath.FirstAbsence`

**category:** aftermath_quest  
**summary:** Consequences of the earlier realm choice: The Place After the Ending  
**acquisition:** {"method":"authored_progression","requirements":["Arrive after the original regional outcome","Operate the specific physical mechanism","Inspect evidence for care or truth","Consult the correct named witness","Persist the exclusive choice and observable aftermath"]}  
**effect:** {"kind":"aftermath_quest","summary":"Consequences of the earlier realm choice: The Place After the Ending"}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.FirstAbsence; journey.expedition.FirstAbsence  
**rule contract:** {"realm":"realm.FirstAbsence","initiating_witness":"npc.absence.firstwitness.001","mechanism":"Absence.Aftermath.OriginDial","care_evidence":"Absence.Aftermath.SharedBeginning","truth_evidence":"Absence.Aftermath.ManyOrigins","care_witness":"npc.absence.caretaker.001","truth_witness":"npc.absence.otherwitness.001","paths":["care","truth"]}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeRealmAftermathRules.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `quest.aftermath.FirstAbsence.requirement.1` — Arrive after the original regional outcome. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.FirstAbsence.requirement.2` — Operate the specific physical mechanism. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.FirstAbsence.requirement.3` — Inspect evidence for care or truth. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.FirstAbsence.requirement.4` — Consult the correct named witness. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `quest.aftermath.FirstAbsence.requirement.5` — Persist the exclusive choice and observable aftermath. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

## SETTLEMENT (3)

### The Crossings — `settlement.Crossings`

**category:** settlement  
**summary:** Authored local routines, social memory and world reactions.  
**acquisition:** {"method":"authored_progression","requirements":["Arrive at the actual settlement","Respect residents' local consent and availability","Earn and preserve supported outcomes"]}  
**effect:** {"kind":"settlement","summary":"Authored local routines, social memory and world reactions."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.ThreefoldReach  
**rule contract:** {"settlement_id":"Crossings"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `settlement.Crossings.requirement.1` — Arrive at the actual settlement. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `settlement.Crossings.requirement.2` — Respect residents' local consent and availability. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `settlement.Crossings.requirement.3` — Earn and preserve supported outcomes. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Bellwold Refuge — `settlement.Bellwold`

**category:** settlement  
**summary:** Authored local routines, social memory and world reactions.  
**acquisition:** {"method":"authored_progression","requirements":["Arrive at the actual settlement","Respect residents' local consent and availability","Earn and preserve supported outcomes"]}  
**effect:** {"kind":"settlement","summary":"Authored local routines, social memory and world reactions."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.ThreefoldReach  
**rule contract:** {"settlement_id":"Bellwold"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `settlement.Bellwold.requirement.1` — Arrive at the actual settlement. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `settlement.Bellwold.requirement.2` — Respect residents' local consent and availability. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `settlement.Bellwold.requirement.3` — Earn and preserve supported outcomes. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only

### Paperhaven Archive — `settlement.Paperhaven`

**category:** settlement  
**summary:** Authored local routines, social memory and world reactions.  
**acquisition:** {"method":"authored_progression","requirements":["Arrive at the actual settlement","Respect residents' local consent and availability","Earn and preserve supported outcomes"]}  
**effect:** {"kind":"settlement","summary":"Authored local routines, social memory and world reactions."}  
**cost:** {"constraint":"Existing source state model, not fabricated numeric values"}  
**duration:** Persistent where source handler exists; otherwise authored definition  
**observation:** {"possible_to_study":false,"automatically_learnable":false}  
**limitations:** No free unlock from rumor or proximity; Exclusive choices and other saved outcomes must be preserved  
**modifications:**   
**combines with:**   
**depends on:** realm.ThreefoldReach  
**rule contract:** {"settlement_id":"Paperhaven"}  
**implementation:** source_rule  
**sources:** Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h  

**Earned requirement checklist (evaluate in live state)**

- [ ] `settlement.Paperhaven.requirement.1` — Arrive at the actual settlement. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `settlement.Paperhaven.requirement.2` — Respect residents' local consent and availability. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only
- [ ] `settlement.Paperhaven.requirement.3` — Earn and preserve supported outcomes. Evidence: physical action and source-state verification; not granted by static checklist; state: evaluated_in_game_only


## Canonical source and gameplay boundaries

1. No automatic power copying, no free stat gains from mere curiosity, no retroactive global quest completion.
2. High-cost permanent reality manipulation must confirm a visible preview and preserve earlier committed saves if a write fails.
3. Non-player agency, consent, the origin of evidence, and trade/wage obligations must not be bypassed by a new ability.
4. Optional technique trees and disciplines may provide meaningful alternative playstyles but cannot secretly make optional quest families mandatory to finish Nhal-Vey.
5. All values, costs, acquisition paths and canon labels must be reconciled against actual source rules when a new ability is introduced or a mechanic changes.
