from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def txt(p):return (ROOT/p).read_text()
class ResidentWiring(unittest.TestCase):
    def test_personal_save_and_rollback(self):
        h=txt("Source/TheUnmadeGame/Public/NPC/UnmadeResidentContinuityRules.h")
        hub=txt("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        world=txt("Source/TheUnmadeGame/Private/World/UnmadeResidentContinuityWorld.cpp")
        save=txt("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        self.assertIn("ResidentContinuityCount",h)
        for field in ("bHasResidentContinuitySnapshot","ResidentEncounterVisits",
                      "ResidentEncounterLastDays","ResidentEncounterAidFlags"):
            self.assertIn(field,hub)
            self.assertIn(field,save)
        self.assertIn("bResidentContinuityRejected",hub)
        self.assertIn("ResidentRelationships.Restore(Old)",world)
        self.assertIn("ResidentRelationships.Talk(",world)
        self.assertIn("ResidentRelationships.Aid(",world)
        self.assertIn("GetUnansweredRoadEvidence()",world)
        self.assertIn("WriteWorldSnapshot()",world)
    def test_real_player_interaction(self):
        player=txt("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        npc=txt("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        self.assertIn("Hub->RecordResidentConversation(Target->GetStableId())",player)
        self.assertIn("Hub->RecordResidentAid(Target->GetStableId())",player)
        self.assertIn("GetResidentReturnLine(NpcId,CurrentHome)",npc)
    def test_ci(self):
        self.assertIn("Tests/npc/resident_continuity_test.cpp",
            txt(".github/workflows/static-checks.yml"))
if __name__=="__main__":unittest.main()
