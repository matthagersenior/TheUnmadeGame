"""Wiring contract for consequences. This does NOT compile Unreal Engine."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class CommunityWorldWiring(unittest.TestCase):
    def test_world_manifestation_from_authoritative_save(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertIn("Facts.crossingDecision=ReadStoryChoice()",hub)
        self.assertIn("Facts.local=RegionalTasks.Snapshot()",hub)
        self.assertIn("Facts.factionEndings=Chronicle.Snapshot().endings",hub)
        self.assertIn("Facts.frontierEndings=Frontier.Snapshot().endings",hub)
        self.assertIn("UnmadeCore::EvaluateAllCommunities(Facts)",hub)
        self.assertIn('TEXT("Community.%d.%d")',hub)
        self.assertIn("SetRiteWorldActorState(Tag,false)",hub)
        self.assertIn("SetRiteWorldActorState(Tag,static_cast<int32>(Next[i].state)==State)",hub)
        self.assertIn("CommunityEffects=Next;",hub)
    def test_world_updates_only_after_saved_story_transitions(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertIn("RestoreLivingWorld();\n    RefreshCommunityConsequences();",hub)
        first=hub.index("bool AUnmadePrototypeHub::TryResidentVillageTask(")
        last=hub.index("void AUnmadePrototypeHub::TryFactionConversation(",first)
        self.assertLess(hub.index("if (!WriteWorldSnapshot())",first,last),
                        hub.index("RefreshCommunityConsequences();",first,last))
        for start,end in [
            ("bool AUnmadePrototypeHub::ResolveNearbyFaction(","FString AUnmadePrototypeHub::GetCurrentRealmName("),
            ("bool AUnmadePrototypeHub::ResolveNearbyFrontier(","void AUnmadePrototypeHub::BuildFrontiers()")
        ]:
            block=hub[hub.index(start):hub.index(end)]
            self.assertLess(block.index("if(!WriteWorldSnapshot())"),
                            block.index("RefreshCommunityConsequences();"))
    def test_npc_locality_and_market_link(self):
        npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertIn("GetCommunityOutcome(Home)",npc)
        self.assertIn("GetStableId().ToString()",npc)
        self.assertIn("npc.saltwake.",npc)
        self.assertIn("Outcome.visibleChange",npc)
        self.assertIn("GetCommunityOutcome(Home).tradeTrust",player)
        self.assertIn("Equipment->Buy(Id,Village,Trust)",player)
        self.assertIn("Hub->RefreshCommunityConsequences();",player)
        self.assertIn("Hub->DescribeCommunityAt(GetActorLocation())",player)
    def test_native_regression_is_real_compile(self):
        wf=read(".github/workflows/static-checks.yml")
        self.assertIn("Tests/world/community_consequences_test.cpp",wf)
        self.assertIn("g++ -std=c++17 -Wall -Wextra -Werror",wf)
if __name__=="__main__":
    unittest.main()
