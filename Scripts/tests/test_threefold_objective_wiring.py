"""Ensure the player can actually read the same canonical next action in both journals."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]
def source(path):return (ROOT/path).read_text(encoding="utf-8")

class ThreefoldObjectiveIntegration(unittest.TestCase):
    def test_native_real_quest_guide_receives_actual_save_backed_objects(self):
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        hdr=source("Source/TheUnmadeGame/Public/World/UnmadeThreefoldObjectiveGuide.h")
        self.assertIn('#include "World/UnmadeThreefoldObjectiveGuide.h"',hub)
        self.assertEqual(hub.count("UnmadeCore::NextThreefoldGuide("),2)
        self.assertIn("WitnessEchoes,WitnessBraid,Dispatch,ReturnWitness,GetGameDay()",hub)
        self.assertIn("FString AUnmadePrototypeHub::GetEvidenceNotebook() const",hub)
        self.assertIn("FString AUnmadePrototypeHub::GetWitnessEchoJournal() const",hub)
        self.assertIn("CURRENT THREEFOLD OBJECTIVE",hub)
        self.assertIn(" | NEXT: ",hub)
        self.assertIn("if(!evidence.Restore(seen.Snapshot())",hdr)
        self.assertIn("!decision.Restore(braid.Snapshot(),evidence)",hdr)
        self.assertIn("!carried.Restore(dispatch.Snapshot(),decision.Outcome())",hdr)
        self.assertIn("!returned.Restore(reply.Snapshot(),carried,decision.Outcome(),currentDay)",hdr)
        self.assertNotIn("SaveGameToSlot",hdr)

    def test_existing_player_controls_show_current_objective(self):
        player=source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertIn("Hub->GetEvidenceNotebook()",player)
        self.assertIn("GetWitnessEchoJournal()",player)
        self.assertIn('ActionName="EvidenceNotebook",Key=F9',
                      source("Config/DefaultInput.ini"))
        self.assertIn("Tests/world/threefold_objective_guide_test.cpp",
                      source(".github/workflows/static-checks.yml"))
        self.assertIn("test_threefold_objective_wiring.py",
                      source("Scripts/first_pc_build_and_test.ps1"))

    def test_guards_stay_local_and_not_narrative_truth(self):
        hdr=source("Source/TheUnmadeGame/Public/World/UnmadeThreefoldObjectiveGuide.h")
        for name in ("InspectBellwold","InspectPaperhaven","InspectCrossings",
                     "ReturnAfterCivicChoice","DecideAtOrrel","DeliverToHessa",
                     "WaitAMorning","DecideHessaReply","ReturnToOrrel","Completed",
                     "InvalidState"):
            self.assertIn(name,hdr)
        self.assertIn("genuine local civic outcome",hdr)
        self.assertIn("press the same choice twice",hdr)
        self.assertIn("remains disputed",hdr)
        for forbidden in ("WitnessBraid.Commit(", "Reply.Choose(", "dispatch.Collect(",
                          "SetActorLocation(", "AddItem("):
            self.assertNotIn(forbidden,hdr)

if __name__=="__main__":unittest.main()
