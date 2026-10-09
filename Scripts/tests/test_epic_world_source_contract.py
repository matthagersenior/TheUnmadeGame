"""Boundaries between domain-tested systems and uncompiled Unreal source."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class EpicWorldContract(unittest.TestCase):
    def test_bosses_spawn_and_have_separate_rules(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        boss=read("Source/TheUnmadeGame/Private/Combat/UnmadeBossCharacter.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        assert "SpawnActor<AUnmadeBossCharacter>" in hub
        assert "BossEncounter" in read("Source/TheUnmadeGame/Public/Combat/UnmadeBossCharacter.h")
        for tag in ("HollowBell","RedactedCurator","UnfinishedPilgrim"):
            self.assertIn("BossId::"+tag,hub)
            self.assertIn("BossId::"+tag,player)
        self.assertIn("Encounter.Advance(",boss)
        self.assertIn("GetCombat()->TryStrikeTarget",boss)
        self.assertIn("GetCombat()->SetGearBonuses(",boss)
        self.assertIn("bPreviouslyDefeated",hub)
        self.assertIn("ExposeToFold",read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp"))

    def test_crafting_and_economy_have_real_save_and_actions(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        equip=read("Source/TheUnmadeGame/Private/Items/UnmadeEquipmentComponent.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for action in ("MarketBuy","MarketSell","ProfessionCraft"):
            self.assertIn('BindAction("'+action+'"',player)
        for name in ("Economy.Buy(", "Economy.Sell(", "Economy.Craft(",
                     "Economy.Restore(BeforeMoney)", "ReconcileCivicContracts"):
            self.assertIn(name,equip)
        for field in ("TradeMarks","PaidContractMask","ProfessionSkills","bHasEconomySnapshot"):
            self.assertIn(field,save)
        self.assertIn("bSaveRejected",equip)

    def test_faction_arcs_are_saved_and_player_decides(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        self.assertIn("TryFactionConversation",player)
        self.assertIn("ResolveNearbyFaction",player)
        self.assertIn("ResolveNearbyFrontier",player)
        self.assertIn("Chronicle.Decide(",hub)
        self.assertIn("Chronicle.Restore(Before)",hub)
        self.assertIn("FactionEndings",save)

    def test_new_realm_actors_have_unique_id_and_save_hooks(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for marker in ("Gateway.ToRain","Gateway.ToHearth","Gateway.ReturnRain",
                       "Gateway.ReturnHearth","Frontier.Clue.Rain","Frontier.Clue.Hearth"):
            self.assertIn(marker,hub)
        for action in ("TryTravelFrontier","InspectFrontierClue","TryFrontierConversation",
                       "ResolveNearbyFrontier"):
            self.assertIn(action,hub)
        self.assertIn("ConfigureFrontier(Resident)",hub)
        self.assertIn("bFrontierResident",npc)
        self.assertIn("CrossFrontierGateway",player)
        for field in ("VisitedFrontierRealms","DiscoveredFrontierClues",
                      "FrontierStages","FrontierEndings"):
            self.assertIn(field,save)

    def test_combined_scenario_is_compiled_in_ci(self):
        workflow=read(".github/workflows/static-checks.yml")
        scenario=read("Tests/integration/offline_slice_scenario.cpp")
        for name in ("boss_rules_test.cpp","craft_economy_test.cpp","faction_chronicle_test.cpp",
                     "frontier_realms_test.cpp","offline_slice_scenario.cpp"):
            self.assertIn(name,workflow)
        for model in ("BossEncounter", "RegionalEconomy", "FactionChronicle",
                      "FrontierJourney", "InventoryModel"):
            self.assertIn(model,scenario)
if __name__=="__main__":
    unittest.main()
