"""V5 cannot be mere dialog: assert actual wages, finite home, active walkout,
consented cross-realm underwriting, stable testimony and saved endgame."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def src(p):return (ROOT/p).read_text(encoding="utf-8")
class BesHouseholdIntegration(unittest.TestCase):
    def test_hard_economy_and_named_consent(self):
        rules=src("Source/TheUnmadeGame/Public/World/UnmadeBesHouseholdRules.h")
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        materials=src("Source/TheUnmadeGame/Public/World/UnmadeMaterialConsequencesRules.h")
        economy=src("Source/TheUnmadeGame/Public/World/UnmadeLivingEconomyRules.h")
        for name in ("HearBes","InspectHome","HearRheva","DeliverHome",
                     "BeginWalkout","SettleFromTown","OfferMeraGuarantee",
                     "AcceptMeraGuarantee","EndgameWitness","Underwritten"):
            self.assertIn(name,rules)
        self.assertIn("material.DeliverToNamedHome(Realm::HearthBeneath,day)",rules)
        self.assertIn("economy.UnderwriteBesFromMera(amount)",rules)
        self.assertIn("economy.RepayMeraGuarantee(record_.guaranteeOwed)",rules)
        self.assertIn("PayAllBesArrears",economy)
        self.assertIn("besOnWalkout",economy)
        self.assertIn("besWalkout",materials)
        self.assertIn("Materials.AdvanceAllOneDay(Civic,BesHousehold.OnWalkout())",hub)
        self.assertIn("BesHousehold.AdvanceOneDay(Materials.Town(",hub)
        self.assertIn("LivingTrades.AdvanceOneDay(Materials.Town(",hub)

    def test_actual_three_sites_and_distinct_reactions(self):
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        npc=src("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        final=src("Source/TheUnmadeGame/Private/World/UnmadeFinalWorld.cpp")
        for tag in ("Bes.Home","Bes.WalkoutChoice","Bes.RestitutionChoice",
                    "Bes.MeraGuarantee","Bes.ColdHearth","Bes.WarmHearth",
                    "Bes.WalkoutBanner","Bes.PaidRecord","Bes.GuarantorRecord"):
            self.assertIn(tag,hub)
        for field in ("LineTraceTestByChannel","GetBesHouseholdJournal()",
                      "RecordBesHouseholdConversation(FName StableId)",
                      "Materials.Restore(BeforeMaterials,BeforeCargo,GetGameDay())",
                      "BesHousehold.Restore(Prev,GetGameDay())",
                      "GetBesHouseholdResidentLine(FName StableId)"):
            self.assertIn(field,hub)
        self.assertIn("TryBesHouseholdInteraction(this,ResidentDistSq)",player)
        self.assertIn("RecordBesHouseholdConversation(Target->GetStableId())",player)
        self.assertIn("GetBesHouseholdJournal()",player)
        self.assertIn("IsBesOnWalkout()",npc)
        self.assertIn("GetBesHouseholdResidentLine(NpcId)",npc)
        self.assertIn("BesHousehold.Phase()==UnmadeCore::BesPhase::Underwritten",final)
        self.assertIn("BesHousehold.OnWalkout()",final)
        self.assertIn("FinalStory.BreakMask(",final)

    def test_old_schema_and_bad_record_preservation(self):
        save=src("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for field in ("bHasBesHousehold=false","BesQuestPhase","BesHouseholdDay",
                      "BesHeatBaskets","BesColdDays","BesWalkoutDays",
                      "BesReliefPacks","BesFirstReportDay",
                      "bBesHomeVisited","bBesRhevaInterviewed",
                      "BesMeraGuaranteeOwed","bBesMeraConsented",
                      "bBesMeraGuaranteeUsed"):
            self.assertIn(field,save)
        for term in ("RestoredBes.Restore(","RestoredBes.InitializeAtDay",
                     "bBesHouseholdRejected=true;",
                     "bBesHouseholdRejected)return false;",
                     "Save->bHasBesHousehold=true;"):
            self.assertIn(term,hub)
        self.assertIn("Tests/world/bes_household_test.cpp",
                      src(".github/workflows/static-checks.yml"))
        self.assertIn("test_bes_household_wiring.py",
                      src("Scripts/first_pc_build_and_test.ps1"))

    def test_canon(self):
        for path in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                     "docs/lore/living-world-continuity.md",
                     "docs/lore/CONTINUITY_LOG.md"):
            self.assertIn("Lives Behind the Ledger",src(path))
if __name__=="__main__":unittest.main()
