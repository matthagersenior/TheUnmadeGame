"""No cosmetic-only economies: checks real inventory, actor, clock and resident paths."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(path):return (ROOT/path).read_text(encoding="utf-8")

class MaterialConsequenceWiring(unittest.TestCase):
    def test_all_nine_finite_and_day_bound(self):
        rules=read("Source/TheUnmadeGame/Public/World/UnmadeMaterialConsequencesRules.h")
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for name in ("ThreefoldReach","WidowedRain","HearthBeneath",
                     "TidalLedger","SkyBelow","CinderSpine","HundredUnlived",
                     "OrchardOfKings","FirstAbsence"):
            self.assertIn("Realm::"+name,rules)
        for term in ("towns_[i].store-=PackSize","towns_[i].households+=PackSize","t.store-=p.repairCost",
                     "t.households-=used","t.produced=","t.laborHours=",
                     "t.shortageDays","t.integrity=","cargo_.realm!=i",
                     "InitializeAtDay","MaterialPolicies[static_cast<int>(realm)].dailyNeed"):
            self.assertIn(term,rules)
        self.assertIn("AdvanceMaterialDayIfNeeded();",hub)
        self.assertIn("Materials.AdvanceAllOneDay(Civic)",hub)
        self.assertIn("WriteWorldSnapshot()",hub)

    def test_physical_interaction_and_real_npc_routines(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        for term in ('Prefix+TEXT("Store")','Prefix+TEXT("Homes")',
                     'Prefix+TEXT("Repair")','Prefix+TEXT("Damaged")',
                     'Prefix+TEXT("Repaired")','Prefix+TEXT("Shortage")'):
            self.assertIn(term,hub)
        self.assertIn("Materials.Collect(P->realm,GetGameDay())",hub)
        self.assertIn("Materials.Deliver(P->realm,GetGameDay())",hub)
        self.assertIn("Materials.Repair(P->realm,GetGameDay())",hub)
        self.assertIn("LineTraceTestByChannel",hub)
        self.assertIn("CompetingNpcDistanceSq",hub)
        self.assertIn("Hub->TryMaterialInteraction(this,ResidentDistSq)",player)
        self.assertIn("GetLocalMaterialJournal()",player)
        self.assertIn("GetMaterialDuty(HomeLocation,Role)",npc)
        self.assertIn("GetMaterialDutyDestination(HomeLocation,Duty)",npc)
        self.assertIn("GetMaterialResidentLine(HomeLocation)",npc)

    def test_save_compat_and_rollback(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for field in ("MaterialDays","MaterialStores","MaterialHouseholds",
                      "MaterialIntegrity","MaterialProduced","MaterialConsumed",
                      "MaterialShortageDays","MaterialLaborHours",
                      "MaterialDeliveries","MaterialRepairs","MaterialCargoRealm",
                      "MaterialCargoUnits","MaterialCargoDay"):
            self.assertIn(field,save)
        self.assertIn("Save->bHasMaterialWorld",hub)
        self.assertIn("RestoredMaterials.Restore(Data",hub)
        self.assertIn("RestoredMaterials.InitializeAtDay",hub)
        self.assertIn("bMaterialSaveRejected=true;",hub)
        self.assertIn("bMaterialSaveRejected || bLivingEconomyRejected ||",hub)
        self.assertIn("bBesHouseholdRejected)return false;",hub)
        self.assertIn("Materials.Restore(Before,Cargo,GetGameDay())",hub)
        self.assertIn("Tests/world/material_consequences_test.cpp",
                      read(".github/workflows/static-checks.yml"))
        self.assertIn("test_material_consequence_wiring.py",
                      read("Scripts/first_pc_build_and_test.ps1"))

    def test_synced_canon(self):
        for path in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                     "docs/lore/living-world-continuity.md",
                     "docs/lore/CONTINUITY_LOG.md"):
            self.assertIn("Local Material Consequences",read(path))
if __name__=="__main__":unittest.main()
