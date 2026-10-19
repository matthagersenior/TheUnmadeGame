# Volume III — Bestiary, Creature Behaviors, Guardians and Named Bosses

**Status:** Portable behavior rules and canonical encounter indexes are source-level contracts. Collision, telegraph readability, enemy AI movement, audiovisual performance and fairness still require a real Unreal playtest. Enemy attacks may be studied only through specifically earned authored skills; merely observing a monster does not copy its powers.

## Creature behavior and study opportunities (12)

### Cinderjaw — `enemy.creature.Cinderjaw`

**behavior:** JAW STRIKE - GUARD OR RETREAT  
**effect or tell:** {"kind":"enemy_attack","style":"Stalker","health":85,"damage":14,"cooldown_seconds":1.4,"move_speed":145,"windup_seconds":0.7,"reach":2.2}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### PaleStrider — `enemy.creature.PaleStrider`

**behavior:** STRIDER LUNGE - STEP BACK  
**effect or tell:** {"kind":"enemy_attack","style":"Stalker","health":65,"damage":11,"cooldown_seconds":1.1,"move_speed":205,"windup_seconds":0.85,"reach":2.6}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### Veilwatch — `enemy.creature.Veilwatch`

**behavior:** VEIL PULSE - BREAK SIGHT  
**effect or tell:** {"kind":"enemy_attack","style":"Watcher","health":75,"damage":12,"cooldown_seconds":2.3,"move_speed":95,"windup_seconds":1.15,"reach":7}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### GleamMoth — `enemy.creature.GleamMoth`

**behavior:** GLEAM FLASH - TAKE COVER  
**effect or tell:** {"kind":"enemy_attack","style":"Watcher","health":45,"damage":8,"cooldown_seconds":1.5,"move_speed":170,"windup_seconds":0.85,"reach":5.5}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### HollowHound — `enemy.creature.HollowHound`

**behavior:** HOUND BITE - GUARD  
**effect or tell:** {"kind":"enemy_attack","style":"Stalker","health":80,"damage":12,"cooldown_seconds":1.2,"move_speed":185,"windup_seconds":0.75,"reach":2.1}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### GriefLantern — `enemy.creature.GriefLantern`

**behavior:** GRIEF PEAL - BREAK SIGHT  
**effect or tell:** {"kind":"enemy_attack","style":"Watcher","health":90,"damage":16,"cooldown_seconds":3,"move_speed":70,"windup_seconds":1.5,"reach":8}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### Inkmaw — `enemy.creature.Inkmaw`

**behavior:** INKMAW CRUSH - RETREAT  
**effect or tell:** {"kind":"enemy_attack","style":"Stalker","health":110,"damage":18,"cooldown_seconds":2,"move_speed":100,"windup_seconds":1.1,"reach":2.4}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### RedactionEye — `enemy.creature.RedactionEye`

**behavior:** REDACTION - USE THE SHELVES  
**effect or tell:** {"kind":"enemy_attack","style":"Watcher","health":60,"damage":14,"cooldown_seconds":2.2,"move_speed":115,"windup_seconds":1.2,"reach":6.5}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### SaltglassCrawler — `enemy.creature.SaltglassCrawler`

**behavior:** CRAWLER SNAP - STEP BACK  
**effect or tell:** {"kind":"enemy_attack","style":"Stalker","health":125,"damage":16,"cooldown_seconds":1.8,"move_speed":85,"windup_seconds":1,"reach":2.3}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### DrownedWatch — `enemy.creature.DrownedWatch`

**behavior:** DROWNED PULSE - TAKE COVER  
**effect or tell:** {"kind":"enemy_attack","style":"Watcher","health":85,"damage":13,"cooldown_seconds":2.6,"move_speed":80,"windup_seconds":1.35,"reach":7.5}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### Ashmaw — `enemy.creature.Ashmaw`

**behavior:** ASHMAW SLAM - RETREAT  
**effect or tell:** {"kind":"enemy_attack","style":"Stalker","health":140,"damage":22,"cooldown_seconds":2.5,"move_speed":105,"windup_seconds":1.3,"reach":2.6}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  

### KilnWisp — `enemy.creature.KilnWisp`

**behavior:** KILN FLARE - BREAK SIGHT  
**effect or tell:** {"kind":"enemy_attack","style":"Watcher","health":50,"damage":10,"cooldown_seconds":1.8,"move_speed":155,"windup_seconds":1,"reach":6}  
**observation policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**counterplay or limitation:** Observation grants no copied combat power in the current game  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeCreatureRules.h  


## EXPEDITION ENCOUNTERS (9)

### The Tollkeeper Without a Road — `boss:expedition:ThreefoldReach`

**realm:** ThreefoldReach  
**location:** The Split Charter Vault  
**access:** Reach sixth chamber after rescue or evidence choice.  
**hp:** 240  
**attacks:** Crush (26 damage); Sweep (22 damage); Seal (20 damage); Mileglass Ring (24 damage)  
**resolutions:** Defeat; Three-clue evidence surrender  
**aftermath:** Saved defeated/yielded state; Return passage opens; released captives return  
**rewards:** gear:ThreefoldReach:weapon; gear:ThreefoldReach:armor  
**quest:** quest:expedition:ThreefoldReach  
**source:** World/UnmadeExpeditionRegistry.h  

### The Undertow Bailiff — `boss:expedition:WidowedRain`

**realm:** WidowedRain  
**location:** The Hollow Breakwater  
**access:** Reach sixth chamber after rescue or evidence choice.  
**hp:** 265  
**attacks:** Crush (26 damage); Sweep (22 damage); Seal (20 damage); Undertow Lane (24 damage)  
**resolutions:** Defeat; Three-clue evidence surrender  
**aftermath:** Saved defeated/yielded state; Return passage opens; released captives return  
**rewards:** gear:WidowedRain:weapon; gear:WidowedRain:armor  
**quest:** quest:expedition:WidowedRain  
**source:** World/UnmadeExpeditionRegistry.h  

### The Ashbound Provost — `boss:expedition:HearthBeneath`

**realm:** HearthBeneath  
**location:** The Furnace Below Blackslag  
**access:** Reach sixth chamber after rescue or evidence choice.  
**hp:** 290  
**attacks:** Crush (26 damage); Sweep (22 damage); Seal (20 damage); Kiln Fan (24 damage)  
**resolutions:** Defeat; Three-clue evidence surrender  
**aftermath:** Saved defeated/yielded state; Return passage opens; released captives return  
**rewards:** gear:HearthBeneath:weapon; gear:HearthBeneath:armor  
**quest:** quest:expedition:HearthBeneath  
**source:** World/UnmadeExpeditionRegistry.h  

### The Interest Warden — `boss:expedition:TidalLedger`

**realm:** TidalLedger  
**location:** The Foreclosed Anchorage  
**access:** Reach sixth chamber after rescue or evidence choice.  
**hp:** 315  
**attacks:** Crush (26 damage); Sweep (22 damage); Seal (20 damage); Oath Cross (24 damage)  
**resolutions:** Defeat; Three-clue evidence surrender  
**aftermath:** Saved defeated/yielded state; Return passage opens; released captives return  
**rewards:** gear:TidalLedger:weapon; gear:TidalLedger:armor  
**quest:** quest:expedition:TidalLedger  
**source:** World/UnmadeExpeditionRegistry.h  

### The Counterweight Cantor — `boss:expedition:SkyBelow`

**realm:** SkyBelow  
**location:** The Fallen Choir Engine  
**access:** Reach sixth chamber after rescue or evidence choice.  
**hp:** 340  
**attacks:** Crush (26 damage); Sweep (22 damage); Seal (20 damage); Choir Quadrants (24 damage)  
**resolutions:** Defeat; Three-clue evidence surrender  
**aftermath:** Saved defeated/yielded state; Return passage opens; released captives return  
**rewards:** gear:SkyBelow:weapon; gear:SkyBelow:armor  
**quest:** quest:expedition:SkyBelow  
**source:** World/UnmadeExpeditionRegistry.h  

### The Last Claimant — `boss:expedition:CinderSpine`

**realm:** CinderSpine  
**location:** The Buried Yesterday Foundry  
**access:** Reach sixth chamber after rescue or evidence choice.  
**hp:** 365  
**attacks:** Crush (26 damage); Sweep (22 damage); Seal (20 damage); Seam Split (24 damage)  
**resolutions:** Defeat; Three-clue evidence surrender  
**aftermath:** Saved defeated/yielded state; Return passage opens; released captives return  
**rewards:** gear:CinderSpine:weapon; gear:CinderSpine:armor  
**quest:** quest:expedition:CinderSpine  
**source:** World/UnmadeExpeditionRegistry.h  

### The Registrar of Empty Beds — `boss:expedition:HundredUnlived`

**realm:** HundredUnlived  
**location:** The Unentered Census House  
**access:** Reach sixth chamber after rescue or evidence choice.  
**hp:** 390  
**attacks:** Crush (26 damage); Sweep (22 damage); Seal (20 damage); Elseborn Divide (24 damage)  
**resolutions:** Defeat; Three-clue evidence surrender  
**aftermath:** Saved defeated/yielded state; Return passage opens; released captives return  
**rewards:** gear:HundredUnlived:weapon; gear:HundredUnlived:armor  
**quest:** quest:expedition:HundredUnlived  
**source:** World/UnmadeExpeditionRegistry.h  

### The Unwritten Castellan — `boss:expedition:OrchardOfKings`

**realm:** OrchardOfKings  
**location:** The Crownroot Keep  
**access:** Reach sixth chamber after rescue or evidence choice.  
**hp:** 415  
**attacks:** Crush (26 damage); Sweep (22 damage); Seal (20 damage); Crown Square (24 damage)  
**resolutions:** Defeat; Three-clue evidence surrender  
**aftermath:** Saved defeated/yielded state; Return passage opens; released captives return  
**rewards:** gear:OrchardOfKings:weapon; gear:OrchardOfKings:armor  
**quest:** quest:expedition:OrchardOfKings  
**source:** World/UnmadeExpeditionRegistry.h  

### The Keeper of the Single Door — `boss:expedition:FirstAbsence`

**realm:** FirstAbsence  
**location:** The Unfinished Bastion  
**access:** Reach sixth chamber after rescue or evidence choice.  
**hp:** 440  
**attacks:** Crush (26 damage); Sweep (22 damage); Seal (20 damage); Threshold Rim (24 damage)  
**resolutions:** Defeat; Three-clue evidence surrender  
**aftermath:** Saved defeated/yielded state; Return passage opens; released captives return  
**rewards:** gear:FirstAbsence:weapon; gear:FirstAbsence:armor  
**quest:** quest:expedition:FirstAbsence  
**source:** World/UnmadeExpeditionRegistry.h  


## ORIGINAL ENCOUNTERS (3)

### The Bell That Buried Its Keeper — `boss:original:HollowBell`

**realm:** ThreefoldReach  
**location:** Bellwold ossuary  
**access:** Travel physically to Bellwold ossuary arena; attacks always have a warning.  
**hp:** 300  
**attacks:** Fold interrupts the toll  
**resolutions:** Defeat; Pacify using learned Understanding Bosses and firsthand evidence  
**aftermath:** One-time boss achievement and reward persist through save/load.  
**rewards:** item:BellheartAegis  
**source:** Combat/UnmadeBossRules.h  

### The Curator of Missing Names — `boss:original:RedactedCurator`

**realm:** ThreefoldReach  
**location:** Paperhaven black archive  
**access:** Travel physically to Paperhaven black archive arena; attacks always have a warning.  
**hp:** 260  
**attacks:** Glimpse reveals the erased signature  
**resolutions:** Defeat  
**aftermath:** One-time boss achievement and reward persist through save/load.  
**rewards:** item:RedactedScepter  
**source:** Combat/UnmadeBossRules.h  

### The Unfinished Pilgrim — `boss:original:UnfinishedPilgrim`

**realm:** ThreefoldReach  
**location:** Crossings broken causeway  
**access:** Travel physically to Crossings broken causeway arena; attacks always have a warning.  
**hp:** 340  
**attacks:** Fold exposes the path between charges  
**resolutions:** Defeat  
**aftermath:** One-time boss achievement and reward persist through save/load.  
**rewards:** item:PilgrimsSignet  
**source:** Combat/UnmadeBossRules.h  


## GUARDIAN ENCOUNTERS (6)

### The Collector of Unborn Days — `boss:guardian:TidalLedger`

**realm:** TidalLedger  
**location:** Northern terrace  
**access:** Complete quest:echo:TidalLedger and approach the guardian; nonlethal choice requires nearby line of sight.  
**hp:** 140  
**attacks:** The collector unwinds a future chain; step beyond the arc or Fold its strike.  
**resolutions:** Defeat; Double-confirm mercy pact after witnessed Echo  
**aftermath:** The ferry keeps the collector's chains as proof no child's future may be pledged.; The published record names those who forged debts in another person's time.  
**rewards:** world:guardian:TidalLedger  
**quest:** quest:echo:TidalLedger  
**source:** Combat/UnmadeRealmGuardianRules.h  

### The Missing Chorister — `boss:guardian:SkyBelow`

**realm:** SkyBelow  
**location:** Northern terrace  
**access:** Complete quest:echo:SkyBelow and approach the guardian; nonlethal choice requires nearby line of sight.  
**hp:** 110  
**attacks:** The invisible note tightens; break sight, guard, or Fold before it falls.  
**resolutions:** Defeat; Double-confirm mercy pact after witnessed Echo  
**aftermath:** The new meter counts the erased singer among willing supporters.; The open stage gives a name to the voice the council denied.  
**rewards:** world:guardian:SkyBelow  
**quest:** quest:echo:SkyBelow  
**source:** Combat/UnmadeRealmGuardianRules.h  

### The Mortar Ancestor — `boss:guardian:CinderSpine`

**realm:** CinderSpine  
**location:** Northern terrace  
**access:** Complete quest:echo:CinderSpine and approach the guardian; nonlethal choice requires nearby line of sight.  
**hp:** 175  
**attacks:** The ancestor raises a quarry hammer; retreat behind the seam or interrupt.  
**resolutions:** Defeat; Double-confirm mercy pact after witnessed Echo  
**aftermath:** The protected homes use stone no longer borrowed from erased families.; The ancestor wall carries the worker's name in daylight.  
**rewards:** world:guardian:CinderSpine  
**quest:** quest:echo:CinderSpine  
**source:** Combat/UnmadeRealmGuardianRules.h  

### The Numberless Examiner — `boss:guardian:HundredUnlived`

**realm:** HundredUnlived  
**location:** Northern terrace  
**access:** Complete quest:echo:HundredUnlived and approach the guardian; nonlethal choice requires nearby line of sight.  
**hp:** 115  
**attacks:** A numbered pulse narrows; evade or use the covered addressway.  
**resolutions:** Defeat; Double-confirm mercy pact after witnessed Echo  
**aftermath:** The safe homes no longer ask for a registered beginning.; The public threshold requires permission before recognizing a name.  
**rewards:** world:guardian:HundredUnlived  
**quest:** quest:echo:HundredUnlived  
**source:** Combat/UnmadeRealmGuardianRules.h  

### The Root Bailiff — `boss:guardian:OrchardOfKings`

**realm:** OrchardOfKings  
**location:** Northern terrace  
**access:** Complete quest:echo:OrchardOfKings and approach the guardian; nonlethal choice requires nearby line of sight.  
**hp:** 160  
**attacks:** A law-root points at your feet; cross to the unbound row before impact.  
**resolutions:** Defeat; Double-confirm mercy pact after witnessed Echo  
**aftermath:** The shared harvest never demands an oath to a crown.; The empty court records every challenge to a stolen title.  
**rewards:** world:guardian:OrchardOfKings  
**quest:** quest:echo:OrchardOfKings  
**source:** Combat/UnmadeRealmGuardianRules.h  

### The Unfinished Witness — `boss:guardian:FirstAbsence`

**realm:** FirstAbsence  
**location:** Northern terrace  
**access:** Complete quest:echo:FirstAbsence and approach the guardian; nonlethal choice requires nearby line of sight.  
**hp:** 185  
**attacks:** Two overlapping silhouettes prepare a strike; retreat into the stable path.  
**resolutions:** Defeat; Double-confirm mercy pact after witnessed Echo  
**aftermath:** The harbor holds a gentle doorway open to people who need refuge.; The hall records different beginnings without calling a witness a liar.  
**rewards:** world:guardian:FirstAbsence  
**quest:** quest:echo:FirstAbsence  
**source:** Combat/UnmadeRealmGuardianRules.h  


## WING ENCOUNTERS (9)

### The Unmeasured Bailiff — `boss:wing:ThreefoldReach`

**realm:** ThreefoldReach  
**location:** Lantern Survey Gallery  
**access:** Complete three wing mechanisms in order.  
**hp:** 150  
**attacks:** Crush; Sweep; Seal; Regional signature  
**resolutions:** Defeat  
**aftermath:** Guardian state and 5 total insight are saved, not an inventory drop.  
**rewards:** resource:insight:5  
**quest:** quest:wing:ThreefoldReach  
**source:** World/UnmadePlayerExperienceRegistry.h  

### The Brine Tollkeeper — `boss:wing:WidowedRain`

**realm:** WidowedRain  
**location:** Drowned Pump Annex  
**access:** Complete three wing mechanisms in order.  
**hp:** 162  
**attacks:** Crush; Sweep; Seal; Regional signature  
**resolutions:** Defeat  
**aftermath:** Guardian state and 5 total insight are saved, not an inventory drop.  
**rewards:** resource:insight:5  
**quest:** quest:wing:WidowedRain  
**source:** World/UnmadePlayerExperienceRegistry.h  

### The Unspent Furnace — `boss:wing:HearthBeneath`

**realm:** HearthBeneath  
**location:** Cooling Ledger Foundry  
**access:** Complete three wing mechanisms in order.  
**hp:** 174  
**attacks:** Crush; Sweep; Seal; Regional signature  
**resolutions:** Defeat  
**aftermath:** Guardian state and 5 total insight are saved, not an inventory drop.  
**rewards:** resource:insight:5  
**quest:** quest:wing:HearthBeneath  
**source:** World/UnmadePlayerExperienceRegistry.h  

### The Unclaimed Duelist — `boss:wing:TidalLedger`

**realm:** TidalLedger  
**location:** Broken Oath Court  
**access:** Complete three wing mechanisms in order.  
**hp:** 186  
**attacks:** Crush; Sweep; Seal; Regional signature  
**resolutions:** Defeat; Yield to oath/guard challenge  
**aftermath:** Guardian state and 5 total insight are saved, not an inventory drop.  
**rewards:** resource:insight:5  
**quest:** quest:wing:TidalLedger  
**source:** World/UnmadePlayerExperienceRegistry.h  

### The Falling Cantor — `boss:wing:SkyBelow`

**realm:** SkyBelow  
**location:** Choir Lift Observatory  
**access:** Complete three wing mechanisms in order.  
**hp:** 198  
**attacks:** Crush; Sweep; Seal; Regional signature  
**resolutions:** Defeat  
**aftermath:** Guardian state and 5 total insight are saved, not an inventory drop.  
**rewards:** resource:insight:5  
**quest:** quest:wing:SkyBelow  
**source:** World/UnmadePlayerExperienceRegistry.h  

### The Borrowed Custodian — `boss:wing:CinderSpine`

**realm:** CinderSpine  
**location:** Seam Memory Archive  
**access:** Complete three wing mechanisms in order.  
**hp:** 210  
**attacks:** Crush; Sweep; Seal; Regional signature  
**resolutions:** Defeat  
**aftermath:** Guardian state and 5 total insight are saved, not an inventory drop.  
**rewards:** resource:insight:5  
**quest:** quest:wing:CinderSpine  
**source:** World/UnmadePlayerExperienceRegistry.h  

### The Unlisted Watcher — `boss:wing:HundredUnlived`

**realm:** HundredUnlived  
**location:** Unobserved Index Vault  
**access:** Complete three wing mechanisms in order.  
**hp:** 222  
**attacks:** Crush; Sweep; Seal; Regional signature  
**resolutions:** Defeat  
**aftermath:** Guardian state and 5 total insight are saved, not an inventory drop.  
**rewards:** resource:insight:5  
**quest:** quest:wing:HundredUnlived  
**source:** World/UnmadePlayerExperienceRegistry.h  

### The Boundary Pruner — `boss:wing:OrchardOfKings`

**realm:** OrchardOfKings  
**location:** Ungrafted Root Nursery  
**access:** Complete three wing mechanisms in order.  
**hp:** 234  
**attacks:** Crush; Sweep; Seal; Regional signature  
**resolutions:** Defeat  
**aftermath:** Guardian state and 5 total insight are saved, not an inventory drop.  
**rewards:** resource:insight:5  
**quest:** quest:wing:OrchardOfKings  
**source:** World/UnmadePlayerExperienceRegistry.h  

### The Closing Registrar — `boss:wing:FirstAbsence`

**realm:** FirstAbsence  
**location:** Threshold Counterweight Hall  
**access:** Complete three wing mechanisms in order.  
**hp:** 246  
**attacks:** Crush; Sweep; Seal; Regional signature  
**resolutions:** Defeat  
**aftermath:** Guardian state and 5 total insight are saved, not an inventory drop.  
**rewards:** resource:insight:5  
**quest:** quest:wing:FirstAbsence  
**source:** World/UnmadePlayerExperienceRegistry.h  


## ROAD ENCOUNTERS (11)

### Milewatch Elder — `boss:road:Crossings`

**realm:** ThreefoldReach  
**location:** Crossings  
**access:** Reach the chalk witness post after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Crossings  
**quest:** quest:road:Crossings  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Uncast Bell Elder — `boss:road:Bellwold`

**realm:** ThreefoldReach  
**location:** Bellwold  
**access:** Reach the bell screen after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Bellwold  
**quest:** quest:road:Bellwold  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Marginbound Elder — `boss:road:Paperhaven`

**realm:** ThreefoldReach  
**location:** Paperhaven  
**access:** Reach the copy press after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Paperhaven  
**quest:** quest:road:Paperhaven  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Hollow Hull Elder — `boss:road:Saltwake`

**realm:** WidowedRain  
**location:** Saltwake  
**access:** Reach the rain shutter after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Saltwake  
**quest:** quest:road:Saltwake  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Bafflewatch Elder — `boss:road:Cinderhold`

**realm:** HearthBeneath  
**location:** Cinderhold  
**access:** Reach the kiln baffle after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Cinderhold  
**quest:** quest:road:Cinderhold  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Unpledged Tide Elder — `boss:road:Drevlach`

**realm:** TidalLedger  
**location:** Drevlach  
**access:** Reach the tide clamp after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Drevlach  
**quest:** quest:road:Drevlach  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Restweight Elder — `boss:road:Orravane`

**realm:** SkyBelow  
**location:** Orravane  
**access:** Reach the counterweight brake after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Orravane  
**quest:** quest:road:Orravane  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Last Brace Elder — `boss:road:Vathless`

**realm:** CinderSpine  
**location:** Vathless  
**access:** Reach the seam brace after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Vathless  
**quest:** quest:road:Vathless  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Unclaimed Lintel Elder — `boss:road:Eillun`

**realm:** HundredUnlived  
**location:** Eillun  
**access:** Reach the privacy screen after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Eillun  
**quest:** quest:road:Eillun  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Crownless Root Elder — `boss:road:Tharniv`

**realm:** OrchardOfKings  
**location:** Tharniv  
**access:** Reach the root sluice after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Tharniv  
**quest:** quest:road:Tharniv  
**source:** World/UnmadeRememberedRoadRegistry.h  

### Near Door Elder — `boss:road:Auvren`

**realm:** FirstAbsence  
**location:** Auvren  
**access:** Reach the threshold brace after investigating all three clues.  
**attacks:** Ground Slam; Cinder Sweep  
**resolutions:** Defeat; Quiet after evidence and mechanical cooling  
**aftermath:** Saved territorial state, public road access and dated delivery testimony.  
**rewards:** world:road:Auvren  
**quest:** quest:road:Auvren  
**source:** World/UnmadeRememberedRoadRegistry.h  


## FINAL ENCOUNTERS (1)

### Nhal-Vey, The Answer That Ate Its Question — `boss:final:NhalVey`

**realm:** FirstAbsence  
**location:** Auvren northern terrace  
**access:** Finish Auvren first chapter and earn a legitimate Unanswered Road route.  
**hp:** 330  
**attacks:** Bell: frontal wide arc; Horizon: long thin seam; Names: frontal thrust; Counterfactual: offset strip  
**resolutions:** Defeat 330 HP Veil; Defeat 290 HP Unmasked form; Choose Held Morning or Many Mornings; Optional 240 HP Echo rematch  
**aftermath:** Existing quests, people, equipment and saved consequences persist; Held Morning preserves seams, Many Mornings preserves contradictory histories.; Rematch patterns vary by chosen morning.  
**rewards:** world:held_morning; world:many_mornings  
**quest:** quest:campaign:FirstAbsence  
**source:** Combat/UnmadeFinalEncounterRules.h  


## Source-defined named encounter patterns (31)

These profiles cross-reference some of the named bosses above; they are not 31 additional unique boss encounters.

### The Bell That Buried Its Keeper — `enemy.boss.HollowBell`

**pattern family:** enemy_boss_pattern  
**telegraph:** Fold interrupts the toll  
**effect:** {"kind":"boss_attack","health":300,"damage":28,"range_cm":450,"windup_seconds":2,"recovery_seconds":2.5}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeBossRules.h  

### The Curator of Missing Names — `enemy.boss.RedactedCurator`

**pattern family:** enemy_boss_pattern  
**telegraph:** Glimpse reveals the erased signature  
**effect:** {"kind":"boss_attack","health":260,"damage":24,"range_cm":720,"windup_seconds":1.5,"recovery_seconds":2.25}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeBossRules.h  

### The Unfinished Pilgrim — `enemy.boss.UnfinishedPilgrim`

**pattern family:** enemy_boss_pattern  
**telegraph:** Fold exposes the path between charges  
**effect:** {"kind":"boss_attack","health":340,"damage":32,"range_cm":300,"windup_seconds":1.8,"recovery_seconds":2.75}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeBossRules.h  

### The Collector of Unborn Days — `enemy.guardian.realm.1`

**pattern family:** enemy_realm_guardian  
**telegraph:** The collector unwinds a future chain; step beyond the arc or Fold its strike.  
**effect:** {"kind":"guardian_strike","damage":18,"windup_seconds":1.8,"recovery_seconds":2.5,"range_cm":340}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

### The Missing Chorister — `enemy.guardian.realm.2`

**pattern family:** enemy_realm_guardian  
**telegraph:** The invisible note tightens; break sight, guard, or Fold before it falls.  
**effect:** {"kind":"guardian_strike","damage":14,"windup_seconds":2.2,"recovery_seconds":2,"range_cm":510}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

### The Mortar Ancestor — `enemy.guardian.realm.3`

**pattern family:** enemy_realm_guardian  
**telegraph:** The ancestor raises a quarry hammer; retreat behind the seam or interrupt.  
**effect:** {"kind":"guardian_strike","damage":21,"windup_seconds":2.4,"recovery_seconds":3,"range_cm":350}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

### The Numberless Examiner — `enemy.guardian.realm.4`

**pattern family:** enemy_realm_guardian  
**telegraph:** A numbered pulse narrows; evade or use the covered addressway.  
**effect:** {"kind":"guardian_strike","damage":15,"windup_seconds":1.65,"recovery_seconds":2.75,"range_cm":440}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

### The Root Bailiff — `enemy.guardian.realm.5`

**pattern family:** enemy_realm_guardian  
**telegraph:** A law-root points at your feet; cross to the unbound row before impact.  
**effect:** {"kind":"guardian_strike","damage":19,"windup_seconds":2,"recovery_seconds":2.9,"range_cm":385}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

### The Unfinished Witness — `enemy.guardian.realm.6`

**pattern family:** enemy_realm_guardian  
**telegraph:** Two overlapping silhouettes prepare a strike; retreat into the stable path.  
**effect:** {"kind":"guardian_strike","damage":20,"windup_seconds":2.5,"recovery_seconds":3.1,"range_cm":420}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeRealmGuardianRules.h  

### The Unmeasured Bailiff — `enemy.wing_guardian.realm.0`

**pattern family:** enemy_wing_guardian  
**telegraph:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

### The Brine Tollkeeper — `enemy.wing_guardian.realm.1`

**pattern family:** enemy_wing_guardian  
**telegraph:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

### The Unspent Furnace — `enemy.wing_guardian.realm.2`

**pattern family:** enemy_wing_guardian  
**telegraph:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

### The Unclaimed Duelist — `enemy.wing_guardian.realm.3`

**pattern family:** enemy_wing_guardian  
**telegraph:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

### The Falling Cantor — `enemy.wing_guardian.realm.4`

**pattern family:** enemy_wing_guardian  
**telegraph:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

### The Borrowed Custodian — `enemy.wing_guardian.realm.5`

**pattern family:** enemy_wing_guardian  
**telegraph:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

### The Unlisted Watcher — `enemy.wing_guardian.realm.6`

**pattern family:** enemy_wing_guardian  
**telegraph:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

### The Boundary Pruner — `enemy.wing_guardian.realm.7`

**pattern family:** enemy_wing_guardian  
**telegraph:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

### The Closing Registrar — `enemy.wing_guardian.realm.8`

**pattern family:** enemy_wing_guardian  
**telegraph:** Optional fieldwork guardian with physical encounter resolution and save-backed defeat/yield  
**effect:** {"kind":"prototype_guardian","summary":"Shared base combat patterns; no unique transferable spell validated"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h  

### The Tollkeeper Without a Road — `enemy.expedition_signature.realm.0`

**pattern family:** enemy_expedition_boss  
**telegraph:** MILEGLASS RING — CENTER IS SAFE  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"MILEGLASS RING — CENTER IS SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

### The Undertow Bailiff — `enemy.expedition_signature.realm.1`

**pattern family:** enemy_expedition_boss  
**telegraph:** UNDERTOW LANE — MOVE SIDEWAYS  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"UNDERTOW LANE — MOVE SIDEWAYS"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

### The Ashbound Provost — `enemy.expedition_signature.realm.2`

**pattern family:** enemy_expedition_boss  
**telegraph:** KILN FAN — SOUTH HALF IS SAFE  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"KILN FAN — SOUTH HALF IS SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

### The Interest Warden — `enemy.expedition_signature.realm.3`

**pattern family:** enemy_expedition_boss  
**telegraph:** OATH CROSS — DIAGONALS ARE SAFE  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"OATH CROSS — DIAGONALS ARE SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

### The Counterweight Cantor — `enemy.expedition_signature.realm.4`

**pattern family:** enemy_expedition_boss  
**telegraph:** CHOIR QUADRANTS — OPPOSITE SIGNS SAFE  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"CHOIR QUADRANTS — OPPOSITE SIGNS SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

### The Last Claimant — `enemy.expedition_signature.realm.5`

**pattern family:** enemy_expedition_boss  
**telegraph:** SEAM SPLIT — CENTRAL STRIP SAFE  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"SEAM SPLIT — CENTRAL STRIP SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

### The Registrar of Empty Beds — `enemy.expedition_signature.realm.6`

**pattern family:** enemy_expedition_boss  
**telegraph:** ELSEBORN DIVIDE — MIDDLE BAND SAFE  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"ELSEBORN DIVIDE — MIDDLE BAND SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

### The Unwritten Castellan — `enemy.expedition_signature.realm.7`

**pattern family:** enemy_expedition_boss  
**telegraph:** CROWN SQUARE — LEAVE THE SQUARE  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"CROWN SQUARE — LEAVE THE SQUARE"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

### The Keeper of the Single Door — `enemy.expedition_signature.realm.8`

**pattern family:** enemy_expedition_boss  
**telegraph:** THRESHOLD RIM — CENTER IS SAFE  
**effect:** {"kind":"telegraphed_regional_footprint","safe_hint":"THRESHOLD RIM — CENTER IS SAFE"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h; Source/TheUnmadeGame/Public/World/UnmadeExpeditionRules.h  

### Nhal-Vey: Bell — `enemy.final.Bell`

**pattern family:** enemy_final_boss  
**telegraph:** A wide directional forward toll with readable windup  
**effect:** {"kind":"final_footprint","summary":"A wide directional forward toll with readable windup"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h  

### Nhal-Vey: Horizon — `enemy.final.Horizon`

**pattern family:** enemy_final_boss  
**telegraph:** A long thin forward seam avoided sideways  
**effect:** {"kind":"final_footprint","summary":"A long thin forward seam avoided sideways"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h  

### Nhal-Vey: Names — `enemy.final.Names`

**pattern family:** enemy_final_boss  
**telegraph:** An identity-themed close strike with actual visible forward footprint  
**effect:** {"kind":"final_footprint","summary":"An identity-themed close strike with actual visible forward footprint"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h  

### Nhal-Vey: Counterfactual — `enemy.final.Counterfactual`

**pattern family:** enemy_final_boss  
**telegraph:** An asymmetric right-side strike; escape marked strip  
**effect:** {"kind":"final_footprint","summary":"An asymmetric right-side strike; escape marked strip"}  
**cost:** {"enemy_only":"source attack timing"}  
**constraints:** Observation grants no copied combat power in the current game  
**study policy:** {"possible_to_study":true,"automatically_learnable":false,"currently_usable_by_player":false,"study_grants":"readable counterplay","copying":"design candidate only; would require separate balanced quest, evidence and save-backed mastery"}  
**implementation:** prototype_handler  
**source:** Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h  


## Combat lore boundaries

- Boss phases must change tactics rather than merely hit points. Telegraph attacks with visible and caption-readable cues; no hidden instantaneous damage.
- An enemy's surrender, pacification or understanding is not identical to killing it; preserve its authored aftermath, witnesses and resource effects.
- Nhal-Vey's first defeat breaks a veil. Its second form and the optional post-ending Echo are distinct; the Echo never becomes a mandatory gate.
- Guardian, Echo and observatory requirements belong to optional journeys. No grinding those encounters to unlock the main Auvren trial.

**Authority:** [encounter ledger](../dependency-ledger.json), [Ability Codex](../../../Authoring/ability_codex.json), [combat rules](../../../Source/TheUnmadeGame/Public/Combat/UnmadeCombatRules.h), and [Nhal-Vey final encounter](../../../Source/TheUnmadeGame/Public/Combat/UnmadeFinalEncounterRules.h).
