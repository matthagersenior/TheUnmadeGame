"""Ensure a completed Witness Braid cannot teleport a letter or its knowledge."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def src(p):return (ROOT/p).read_text(encoding="utf-8")
class FiniteDispatch(unittest.TestCase):
    def test_save_custody_and_authorized_handoff(self):
        h=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        header=src("Source/TheUnmadeGame/Public/World/UnmadeWitnessDispatchRules.h")
        save=src("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for key in ('"WitnessDispatch.FoldedRecord"',
                    "GetGameDay()", "GetStableId()!=FName(\"npc.bellwold.matron.001\")",
                    "UnmadeDispatchPickup", "UnmadeDispatchReceipt",
                    "Dispatch.Collect(WitnessBraid.Outcome(),GetGameDay())",
                    "Dispatch.HandToHessa(WitnessBraid.Outcome(),GetGameDay())",
                    "Dispatch.Restore(Before,WitnessBraid.Outcome())",
                    "bWitnessDispatchRejected=true", "Save->WitnessDispatchDeliveredDay",
                    "RefreshWitnessDispatchWorld();"):
            self.assertIn(key,h)
        for key in ("bHasWitnessDispatchSnapshot=false","WitnessDispatchStage=0",
                    "WitnessDispatchCollectedDay=0","WitnessDispatchDeliveredDay=0"):
            self.assertIn(key,save)
        self.assertIn("data.deliveredDay<data.collectedDay",header)
        self.assertIn("data.stage!=0&&choice==WitnessBraidOutcome::Unresolved",header)
        self.assertIn("state_.stage==0)return WitnessDispatchEvent::NotReady",header)
    def test_player_needs_physical_actions_and_knowing_hessa(self):
        h=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        p=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        npc=src("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        self.assertIn("Hub->TryWitnessDispatch(this,ResidentDistSq)",p)
        self.assertIn("CompetingNpcDistanceSq",h)
        self.assertIn("FMath::Square(265.f)",h)
        self.assertIn("FMath::Square(260.f)",h)
        self.assertIn("LineTraceTestByChannel",h)
        self.assertIn('if(NpcId!=FName("npc.bellwold.matron.001")',h)
        self.assertIn("GetWitnessDispatchRecipientLine(NpcId)",npc)
        self.assertNotIn("GetWitnessDispatchRecipientLine",src("Source/TheUnmadeGame/Private/Dialogue/UnmadeDialoguePolicy.cpp"))
    def test_ci_and_canon(self):
        self.assertIn("Tests/world/witness_dispatch_test.cpp",src(".github/workflows/static-checks.yml"))
        self.assertIn("test_witness_dispatch_wiring.py",src("Scripts/first_pc_build_and_test.ps1"))
        for p in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                  "docs/lore/living-world-continuity.md",
                  "docs/lore/CONTINUITY_LOG.md"):
            self.assertIn("The Folded Dispatch",src(p))
if __name__=="__main__":unittest.main()
