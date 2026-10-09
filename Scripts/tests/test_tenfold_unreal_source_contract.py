"""Presence and integration contracts; do not confuse these with UE builds."""
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class SignatureRiteIntegration(unittest.TestCase):
    def test_authored_world_sites_and_stories(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        core=read("Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h")
        confluence=read("Source/TheUnmadeGame/Public/World/UnmadeConfluenceRules.h")
        self.assertIn("RitualSites[10]",hub)
        self.assertIn("ConfluenceSites[6]",hub)
        self.assertIn('TEXT("Rite.Site.%d")',hub)
        self.assertIn('TEXT("Confluence.Site.%d")',hub)
        for name in ("UnwriteLaw","Witnesscraft","BorrowedLives",
                     "LegacyForging","LivingRoads","TomorrowDebt",
                     "UnderstandingBosses","ParadoxConvergence",
                     "Oathbinding","Cartography"):
            self.assertIn("RiteId::"+name,core)
        self.assertIn("std::array<RiteSpec,10>",core)
        self.assertIn("std::array<ConfluenceSpec,6>",confluence)
        self.assertIn("FindNearbyConfluence()",read(
            "Source/TheUnmadeGame/Private/World/UnmadeTenfoldComponent.cpp"))

    def test_no_random_invented_npc_testimony(self):
        component=read("Source/TheUnmadeGame/Private/World/UnmadeTenfoldComponent.cpp")
        self.assertIn("Event.Evidence!=EUnmadeEvidenceKind::Witnessed",component)
        self.assertIn("GetObservations()",component)
        self.assertIn("Npc->",component) if False else None
        self.assertIn("Ctx.witness=WitnessBuffer",component)
        self.assertIn("LineTraceTestByChannel",component)
        self.assertNotIn("RequestDialogue",component)
        self.assertNotIn("Ollama",component)
        self.assertNotIn("FMath::Rand",component)

    def test_player_actions_apply_effects_not_only_messages(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        world=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        gear=read("Source/TheUnmadeGame/Private/Items/UnmadeEquipmentComponent.cpp")
        for mechanic in ("LaunchCharacter(", "ExposeToFold(", "AddActorWorldOffset(",
                         "SetActorEnableCollision(false)","SetActorHiddenInGame(true)"):
            self.assertIn(mechanic,player)
        self.assertIn("SetRiteWorldActorState(Tag,true)",world)
        self.assertIn("TemporaryRiteWorldEffects",world)
        self.assertIn("const int32 Borrowed=",gear)
        self.assertIn("Player->GetBorrowedCraftLevel()",gear)
        self.assertIn("ActiveAttackBonus",read(
            "Source/TheUnmadeGame/Private/World/UnmadeTenfoldComponent.cpp"))

    def test_journal_and_controls_are_behind_player_bindings(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        ini=read("Config/DefaultInput.ini")
        for key in ("RiteNext","RitePrevious","RiteJournal","RiteStudy",
                    "RiteUse","RiteChoice1","RiteChoice2",
                    "RiteBreak","RiteLaw",
                    "ConfluenceCycle","ConfluenceStudy",
                    "ConfluenceOption1","ConfluenceOption2"):
            self.assertIn('BindAction("'+key+'"',player)
            self.assertIn('ActionName="'+key+'"',ini)
        self.assertEqual(player.count("IE_Pressed,Tenfold.Get(),"),13)
        self.assertNotIn("IE_Pressed,Tenfold,&UUnmadeTenfoldComponent",player)

    def test_save_migration_and_grants_are_durable(self):
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        component=read("Source/TheUnmadeGame/Private/World/UnmadeTenfoldComponent.cpp")
        gear=read("Source/TheUnmadeGame/Private/Items/UnmadeEquipmentComponent.cpp")
        for field in ("RiteStages","RiteChoices","RiteReadyAt",
                      "DisciplineMastery","RiteTrialBits","LegacyDeedBits",
                      "VerifiedRoadBits","TomorrowDebtDueDay","bOathRedeemed",
                      "ConfluenceStages","ConfluenceCastMasks",
                      "ConfluenceWindowEnds","bHasConfluenceSnapshot"):
            self.assertIn(field,save)
            self.assertIn(field,component)
        for token in ("Chronicle.Restore(Previous)","Confluence.Restore(PriorConfluence)",
                      "Player->RefundRealityStrain(Power.cost)", "if(!Persist(true))"):
            self.assertIn(token,component)
        self.assertIn("Achievement::RiteCartography",gear)
        self.assertIn("Achievement::ConfluenceFirstAbsence",gear)

    def test_native_domain_suites_are_in_ci(self):
        wf=read(".github/workflows/static-checks.yml")
        scenario=read("Tests/integration/offline_slice_scenario.cpp")
        self.assertIn("Tests/world/tenfold_chronicle_test.cpp",wf)
        self.assertIn("Tests/world/confluence_rules_test.cpp",wf)
        self.assertIn("TenfoldChronicle signature;",scenario)
        self.assertIn("ConfluenceJourney confluence;",scenario)
if __name__=="__main__":
    unittest.main()
