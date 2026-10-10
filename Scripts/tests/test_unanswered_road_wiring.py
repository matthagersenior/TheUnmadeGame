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
    def test_ci(self):
        self.assertIn("Tests/story/unanswered_road_test.cpp",
                      src(".github/workflows/static-checks.yml"))
if __name__=="__main__":unittest.main()
