"""Offline multi-village wiring checks: these are not Unreal gameplay tests."""
import unittest
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
def read(path):
    return (ROOT/path).read_text(encoding="utf-8")
class MultiVillageSourceTests(unittest.TestCase):
    def test_registry_and_three_connected_places(self):
        registry=read("Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h")
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertIn("std::array<ResidentSpec, 48>",registry)
        self.assertIn("std::array<SettlementSpec, 3>",registry)
        for village in ("Crossings","Bellwold","Paperhaven"):
            self.assertIn("SettlementId::"+village,registry)
        for road in ("Route.WestCauseway","Route.EastCauseway"):
            self.assertIn(road,hub)
        self.assertIn("SpawnCitizen(Resident)",hub)
        self.assertIn("FName(\"Bellwold.Belltower\")",hub)
        self.assertIn("FName(\"Paperhaven.ArchiveTower\")",hub)

    def test_all_npcs_use_local_minds_and_memory(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertIn("ConfigureIdentity(",hub)
        self.assertIn("AuthoredLine = InAuthoredLine",npc)
        self.assertIn("SettlementTrustDelta(HomeSettlement",npc)
        self.assertIn("LocalRoutineTarget(",npc)
        self.assertIn("LocalRumorTarget(HomeSettlement)",npc)
        self.assertIn("LocalInvestigationTarget(HomeSettlement)",npc)
        self.assertIn("FindNearbyCitizen(",player)
        self.assertIn("Save->NpcSnapshots.Reset()",hub)
        self.assertNotIn("UUnmadeLocalDialogueSubsystem",npc)

    def test_local_village_tasks_and_save_are_authoritative(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for field in ("VisitedSettlementsMask","BellwoldTaskStage",
                      "PaperhavenTaskStage","NpcSnapshots","WorldVariantId"):
            self.assertIn(field,save)
        self.assertIn("RestoredTasks.Restore(",hub)
        self.assertIn("RegionalTasks.Restore(Previous)",hub)
        self.assertIn("RegionalTasks.Converse(",hub)
        self.assertIn("TryResidentVillageTask(Target->GetStableId()",player)
        self.assertIn('ReportLocalEvent(FName("Player.HelpedVillage")',player)
        self.assertIn("Hub->GetVisitedVillageCount()",player)

    def test_native_rules_are_tested_in_ci(self):
        workflow=read(".github/workflows/static-checks.yml")
        for path in ("Tests/world/settlement_registry_test.cpp",
                     "Tests/world/regional_tasks_test.cpp",
                     "Tests/integration/offline_slice_scenario.cpp"):
            self.assertIn(path,workflow)
if __name__=="__main__":
    unittest.main()
