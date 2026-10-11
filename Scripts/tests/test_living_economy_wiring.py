"""V4 requires actual source gameplay: paid named workers, E cargo and saved foreign relief."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class LivingEconomyWiring(unittest.TestCase):
    def test_source_links_payroll_and_material_workdays(self):
        domain=read("Source/TheUnmadeGame/Public/World/UnmadeLivingEconomyRules.h")
        material=read("Source/TheUnmadeGame/Public/World/UnmadeMaterialConsequencesRules.h")
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for key in ("npc.marketporter.001","npc.saltwake.courier.001",
                    "npc.cinderhold.courier.001","npc.tidal.ferryman.001",
                    "npc.sky.structuralist.001","npc.bones.mason.001",
                    "npc.unlived.caretaker.001","npc.orchard.gardener.001",
                    "npc.absence.caretaker.001","owedMarks","earnedMarks",
                    "employerMarks","PayArrears","accountedMarks!=9*32",
                    "buyer.employerMarks-=c.agreedMarks",
                    "seller.employerMarks+=c.agreedMarks",
                    "if(in.cargo.route==-1)"):
            self.assertIn(key,domain)
        self.assertIn("ExportForContract",material)
        self.assertIn("ReinforceShelter",material)
        self.assertIn("Materials.AdvanceAllOneDay(Civic)",hub)
        self.assertIn("LivingTrades.AdvanceOneDay(Materials.Town(",hub)
        self.assertIn("LivingTrades.ApplyShelterSupport(Realm,*Town)",hub)
        self.assertIn("Materials.ReinforceShelter(Realm,Support)",hub)
    def test_actual_actor_handoff_returning_people_and_boss(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        final=read("Source/TheUnmadeGame/Private/World/UnmadeFinalWorld.cpp")
        for needle in ('Prefix+TEXT("Start")','Prefix+TEXT("Finish")',
                       "UnmadeLivingTrade","LineTraceTestByChannel",
                       "LivingTrades.Collect(Contract.route,GetGameDay(),Materials)",
                       "LivingTrades.Deliver(Contract.route,GetGameDay(),Materials)",
                       "LivingTrades.Restore(BeforeLiving,Materials,GetGameDay())",
                       "GetLivingEconomyResidentLine(FName StableId)",
                       "GetLivingEconomyJournal()"):
            self.assertIn(needle,hub)
        self.assertIn("Hub->TryLivingTradeInteraction(this,ResidentDistSq)",player)
        self.assertIn("Hub->GetLivingEconomyJournal()",player)
        self.assertIn("GetLivingEconomyResidentLine(NpcId)",npc)
        self.assertIn("LivingTrades.HasEarnedMutualAid()",final)
        self.assertIn("FinalStory.BreakMask(",final)
    def test_fail_closed_backwards_compatible_and_ci(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for field in ("bHasLivingEconomy=false","LivingEmployerMarks",
                      "LivingEarnedMarks","LivingOwedMarks","LivingPaidShifts",
                      "LivingPaidDays","LivingWorkerReceipts","LivingForeignStock",
                      "LivingRouteReceipts","LivingTradePayments",
                      "LivingConsumedSupport","LivingTradeCargoRoute=-1",
                      "LivingTradeCargoDay=0"):
            self.assertIn(field,save)
        for field in ("RestoredLiving.InitializeAtDay","RestoredLiving.Restore(",
                      "bLivingEconomyRejected=true","Save->bHasLivingEconomy=true",
                      "bMaterialSaveRejected || bLivingEconomyRejected ||",
                      "bBesHouseholdRejected)return false;"):
            self.assertIn(field,hub)
        self.assertIn("Tests/world/living_economy_test.cpp",
                      read(".github/workflows/static-checks.yml"))
        self.assertIn("test_living_economy_wiring.py",
                      read("Scripts/first_pc_build_and_test.ps1"))
    def test_lore_sync(self):
        for path in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                     "docs/lore/living-world-continuity.md",
                     "docs/lore/CONTINUITY_LOG.md"):
            self.assertIn("The Living Economy",read(path))
if __name__=="__main__":unittest.main()
