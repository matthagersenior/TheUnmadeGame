"""Source contracts for the nine-realm Unanswered Road main campaign."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def src(path): return (ROOT/path).read_text()
class UnansweredRoadWiring(unittest.TestCase):
    def test_source_story_and_optional_revelations(self):
        road=src("Source/TheUnmadeGame/Private/World/UnmadeCampaignWorld.cpp")
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        rules=src("Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h")
        for term in ('BuildUnansweredRoad();','GetUnansweredRoadEvidence()',
                     'GetUnansweredRoadGuidance()','DescribeUnansweredRoad(',
                     'InspectUnansweredRoad('):
            self.assertIn(term,road+hub)
        for term in ('RoadMask(', 'Frontier.IsResolved(', 'LaterRealm.IsComplete(',
                     'ReadStoryChoice()!=0', 'VillagesVisited.Count()>=2',
                     'Discoveries.Count()>=2', 'RoadRoute(', 'GetAtlasAttunement()',
                     'LineTraceTestByChannel(', 'ActorHasTag(', 'SpawnBlock('):
            self.assertIn(term,road)
        self.assertIn('RoadAct::ConfrontTheAnswer',rules)
        self.assertIn('RoadAct::LivingAfterward',rules)
        self.assertIn('UnansweredRoadBeats',rules)
        self.assertNotIn('SaveGameToSlot(',road)
    def test_journal_navigation_and_boss_gate(self):
        player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        boss=src("Source/TheUnmadeGame/Private/Combat/UnmadeFinalBoss.cpp")
        final=src("Source/TheUnmadeGame/Private/World/UnmadeFinalWorld.cpp")
        self.assertIn('DescribeUnansweredRoad(this)',player)
        self.assertIn('InspectUnansweredRoad(this,ResidentDistSq)',player)
        self.assertIn('GetUnansweredRoadGuidance().bossAccessible',boss)
        self.assertIn('!GetUnansweredRoadGuidance().bossAccessible',final)
    def test_no_fake_unlocks_or_mutated_old_save(self):
        rules=src("Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h")
        self.assertNotIn('EchoQuest.',rules)
        self.assertNotIn('Guardians.',rules)
        self.assertIn('PlanRealmRoute(',rules)
        self.assertIn('if(state.finalAct>=3',rules)
        self.assertIn('if(state.finalAct>0',rules)
        self.assertIn('firstStories',rules)
    def test_main_story_attunement_powers_both_actual_portals_and_guidance(self):
        road=src("Source/TheUnmadeGame/Private/World/UnmadeCampaignWorld.cpp")
        gates=src("Source/TheUnmadeGame/Private/World/UnmadeLaterRealmWorld.cpp")
        rules=src("Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h")
        self.assertIn("const int Attunement=UnmadeCore::RoadTravelAttunement(",road)
        self.assertIn("const int32 Attunement=UnmadeCore::RoadTravelAttunement(",gates)
        self.assertIn("GetUnansweredRoadEvidence()",road)
        self.assertIn("GetUnansweredRoadEvidence()",gates)
        self.assertIn("Attunement<Edge.attunement",gates)
        self.assertIn("if(!state.openingWitnessed) return relic;",rules)
        self.assertIn("return std::max(relic,upper?4:3);",rules)
        self.assertNotIn("Gated by actually earned relics",gates)
    def test_schema_one_journal_rebuilds_from_existing_save_authorities(self):
        rules=src("Source/TheUnmadeGame/Public/World/UnmadeCampaignSpineRules.h")
        adapter=src("Source/TheUnmadeGame/Private/World/UnmadeCampaignWorld.cpp")
        save=src("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        test=src("Tests/story/unanswered_road_test.cpp")
        self.assertIn("RestoreRoadEvidence(OpeningWitnessed,FirstStories",adapter)
        self.assertIn("firstStories&allStories",rules)
        self.assertIn("openingWitnessed||out.firstStories!=0||finalAct!=0",rules)
        self.assertIn("int32 SchemaVersion = 1",save)
        self.assertNotIn("RoadEvidenceSnapshot",save)
        self.assertIn("TestSchemaOneJournalRestoration",test)
        self.assertIn("TestJournalGoldens",test)
        self.assertIn("actual.objective,objective",test)
    def test_tomorrow_debt_mastery_name_matches_inventory_and_codex(self):
        import json
        rite=src("Source/TheUnmadeGame/Public/World/UnmadeTenfoldChronicle.h")
        inventory=src("Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h")
        codex=json.loads(src("Authoring/ability_codex.json"))
        ledger=json.loads(src("docs/canon/dependency-ledger.json"))
        self.assertIn("Tomorrow's Paid Receipt",rite)
        self.assertIn('ItemId::FutureReceipt',inventory)
        self.assertIn('Tomorrow\'s Paid Receipt',inventory)
        self.assertEqual(next(r for r in codex["records"] if r["id"]=="rite.TomorrowDebt")["reward"],"Tomorrow's Paid Receipt")
        self.assertEqual(next(q for q in ledger["quests"] if q["id"]=="quest:rite:TomorrowDebt")["rite_reward_label"],"Tomorrow's Paid Receipt")
        self.assertFalse(any(e.get("type")=="reward_name_conflict" for e in ledger["exceptions"]))
    def test_ci(self):
        self.assertIn("Tests/story/unanswered_road_test.cpp",
                      src(".github/workflows/static-checks.yml"))
if __name__=="__main__":unittest.main()
