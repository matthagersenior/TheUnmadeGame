"""Static source integration checks; real gameplay requires Unreal Editor compilation."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
def read(path):
    return (ROOT / path).read_text(encoding="utf-8")

class LivingWorldSourceTests(unittest.TestCase):
    def test_six_authored_sites_and_expanded_accessible_ground(self):
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertEqual(hub.count("SpawnLoreSite(UnmadeCore::District::"), 6)
        self.assertIn("FVector(52, 52, 1)", hub)
        for district in ("EchoWell","PaperOrchard","SilentMile","BellGrave",
                         "MarketLedger","ShelterThreshold"):
            self.assertIn("UnmadeCore::District::" + district, hub)
        self.assertIn("FVector(-1720, 850, 90)", hub)
        self.assertIn("FVector(1260, 1490, 90)", hub)
        registry = read("Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h")
        self.assertIn("std::array<ResidentSpec, 48>", registry)
        self.assertIn("SpawnCitizen(Resident)", hub)
        for resident in ("npc.welllistener.001", "npc.orchardexile.001",
                         "npc.tollbroker.001", "npc.roadwarden.001",
                         "npc.bellmaker.001", "npc.nightcourier.001"):
            self.assertIn(resident, registry)
        for road in ("Route.WestCauseway", "Route.EastCauseway"):
            self.assertIn(road, hub)
        self.assertIn("BuildVillages();", hub)

    def test_world_clock_and_visits_persist_without_erasing_other_saves(self):
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        save = read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for field in ("bHasLivingWorldSnapshot", "LivingWorldSeconds",
                      "DiscoveredLoreMask"):
            self.assertIn(field, save)
        for existing in ("NpcSnapshots","LexiconEvidence","WorldVariantId",
                         "LocalConflictChoice"):
            self.assertIn(existing, save)
        for token in ("RestoredClock.Restore(", "RestoredDiscoveries.Restore(",
                      "LoadOrCreate()", "SaveGameToSlot(", "Discoveries.Restore(OldMask)"):
            self.assertIn(token, hub)

    def test_environment_and_inspection_are_reactive_but_not_model_generated(self):
        lore = read("Source/TheUnmadeGame/Private/World/UnmadeLoreSite.cpp")
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player = read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertIn("SelectAmbientCue(District, Phase, StoryChoice)", lore)
        self.assertIn("Glow->SetIntensity(", lore)
        self.assertIn("Glow->SetLightColor(", lore)
        self.assertIn("Site->GetInspectionText(", hub)
        self.assertIn("Hub->InspectSite(NearestSite)", player)
        self.assertIn("const bool bLoreIsCloser", player)
        self.assertIn("BestSiteDistSq <", player)
        self.assertIn("FVector::DistSquared(GetActorLocation(), Target->GetActorLocation())", player)
        self.assertIn("Hub->GetDiscoveredCount()", player)
        self.assertNotIn("UUnmadeLocalDialogueSubsystem", lore)

    def test_npc_routines_use_same_clock_and_do_not_override_emergencies(self):
        npc = read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        self.assertIn("CachedHub->GetCurrentPhase()", npc)
        self.assertIn("if (Motion == UnmadeCore::NpcMotion::Stay)", npc)
        self.assertIn("UnmadeCore::LocalRoutineTarget(", npc)
        self.assertIn("UnmadeCore::LocalInvestigationTarget(HomeSettlement)", npc)
        self.assertIn("UnmadeCore::LocalRumorTarget(HomeSettlement)", npc)
        self.assertIn("HomeLocation = GetActorLocation()", npc)
        self.assertIn("NpcAction::InvestigateAnomaly", npc)
        self.assertIn("NpcAction::AvoidPlayer", npc)

    def test_character_dialogue_and_local_overheard_rumors(self):
        npc = read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        registry = read("Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h")
        for token in ("npc.welllistener.001", "npc.bellmaker.001"):
            self.assertIn(token, registry)
        for token in ("AuthoredLine", "bDirect", "World.ConflictShelter",
                      "SettlementTrustDelta"):
            self.assertIn(token, npc)
        for token in ("bOverheardOne", "OVERHEARD |", "LineTraceTestByChannel",
                      "Event.Evidence == EUnmadeEvidenceKind::Witnessed"):
            self.assertIn(token, hub)

    def test_time_of_day_affects_directional_light_and_cues(self):
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertIn("SpawnActor<ADirectionalLight>", hub)
        self.assertIn("Sunlight->GetComponent()->SetIntensity", hub)
        self.assertIn("|| bPhaseChanged", hub)

    def test_three_discoverable_villages_and_save_migration(self):
        hub = read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        registry = read("Source/TheUnmadeGame/Public/World/UnmadeSettlementRegistry.h")
        save = read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for village in ("Crossings", "Bellwold", "Paperhaven"):
            self.assertIn("SettlementId::" + village, registry)
        self.assertIn("VisitedSettlementsMask", save)
        self.assertIn("RestoredVillages.Restore", hub)
        self.assertIn("VillagesVisited.Visit", hub)
        self.assertIn("VillagesVisited.Restore", hub)
        self.assertIn("Save->NpcSnapshots.Reset()", hub)

    def test_offline_test_and_combined_scenario(self):
        workflow = read(".github/workflows/static-checks.yml")
        suite = read("Tests/integration/offline_slice_scenario.cpp")
        self.assertIn("Tests/world/living_world_test.cpp", workflow)
        self.assertIn("LivingWorldClock restoredWorld;", suite)
        self.assertIn("DiscoveryLedger restoredPlaces;", suite)

if __name__ == "__main__":
    unittest.main()
