"""V6: no fake finished cinematics; require wired, paced captions, truthful
choice confirmations, reachable interior blockout and no outcome mutation by art."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(path):return (ROOT/path).read_text(encoding="utf-8")
class UnpaidShiftDirectedWiring(unittest.TestCase):
    def test_playable_timed_scene_events(self):
        rules=read("Source/TheUnmadeGame/Public/World/UnmadeUnpaidShiftSceneRules.h")
        header=read("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        present=read("Source/TheUnmadeGame/Private/World/UnmadeUnpaidShiftPresentation.cpp")
        for term in ("UnpaidScene::FirstWage","UnpaidScene::HomeCold",
                     "UnpaidScene::HearthWarm","UnpaidScene::Ledger",
                     "UnpaidScene::Refusal","UnpaidScene::Restitution",
                     "UnpaidScene::MeraOffer","UnpaidScene::MeraSacrifice",
                     "UnpaidScene::BesReturns","UnpaidScene::MeraReturns",
                     "UnpaidScene::MaskPaid","UnpaidScene::MaskUnderwritten",
                     "UnpaidScene::MaskUnpaid","camera","blocking","foley",
                     "voice","duration"):
            self.assertIn(term,rules)
        for term in ("FUnmadeBesDirectedBeat","BlueprintAssignable",
                     "PlayBesDirectedScene","SkipBesDirectedScene",
                     "GetBesDecisionOptions","GetBesDecisionPreviewForStation"):
            self.assertIn(term,header)
        for term in ("SetTimer(BesSubtitleTimer","OnBesDirectedBeat.Broadcast",
                     "ClearTimer(BesSubtitleTimer)","UnpaidBeatAt",
                     "Beat->duration","BesDecisionPreview(FName Tag)"):
            self.assertIn(term,present)
        self.assertIn("UGameplayStatics::PlaySound2D(",present)
        self.assertIn("StaticLoadObject(",present)
        self.assertIn("BesSubtitleOverlay->DisplayBeat(",present)
        self.assertIn("BesSubtitleOverlay->ClearBeat()",present)
        self.assertIn("if(USoundBase* Cue=",present)  # sound remains optional
    def test_player_control_actual_source_state(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        present=read("Source/TheUnmadeGame/Private/World/UnmadeUnpaidShiftPresentation.cpp")
        final=read("Source/TheUnmadeGame/Private/World/UnmadeFinalWorld.cpp")
        for token in ("BesPendingDecision!=Decision",
                      "BesDecisionExpiresAt=Now+12.0",
                      "BesDecisionPreview(Decision)",
                      "Result==UnmadeCore::BesOutcome::WagesSettled",
                      "WriteWorldSnapshot()","LivingTrades.Restore(",
                      "Materials.Restore(","PlayBesDirectedScene(",
                      "GetBesHouseholdResidentLine(",
                      "FName(TEXT(\"Bes.Home\"))"):
            self.assertIn(token,hub)
        self.assertIn("MaybePlayBesReturnScene(Target->GetStableId())",player)
        self.assertIn("RecordBesHouseholdConversation(Target->GetStableId())",player)
        self.assertIn("GetTimeSeconds()",present)
        self.assertIn("return BesDecisionPreview(StationTag);",present)
        self.assertIn("LastBesReturningSceneAt+60.0",present)
        self.assertIn("AnimateBesWorkingCart();",hub)
        self.assertIn("Bes.WorkingCart",hub)
        self.assertIn("Bes.WorkingCoal",hub)
        self.assertIn("SetActorEnableCollision(false)",hub)
        self.assertIn("BesHousehold.OnWalkout()",present)
        self.assertIn("BesMaskScene(BesHousehold.Phase())",final)
        for token in ("Bes.ColdHearth","Bes.WarmHearth","Bes.WalkoutBanner",
                      "Bes.GuarantorRecord"):
            self.assertIn(token,hub)
    def test_enterable_home_not_solid_faux_house(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for tag in ("Bes.InteriorFloor","Bes.WestWall","Bes.EastWall",
                    "Bes.NorthWall","Bes.SouthWallWest","Bes.SouthWallEast",
                    "Bes.Home","Bes.Bed","Bes.WarmthStorage","Bes.PayDesk",
                    "Bes.MissedWageBook","Bes.ChildHeightStool","Bes.DoorLintel"):
            self.assertIn(tag,hub)
        self.assertNotIn('FVector(2.2,1.8,2.1),\n        FName(TEXT("Bes.Home"))',hub)
    def test_ci_and_canon(self):
        self.assertIn("Tests/world/unpaid_shift_scene_test.cpp",
                      read(".github/workflows/static-checks.yml"))
        self.assertIn("test_unpaid_shift_directed_wiring.py",
                      read("Scripts/first_pc_build_and_test.ps1"))
        for path in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                     "docs/lore/CONTINUITY_LOG.md",
                     "docs/lore/living-world-continuity.md"):
            self.assertIn("The Unpaid Shift — Directed Quest",read(path))
if __name__=="__main__":unittest.main()
