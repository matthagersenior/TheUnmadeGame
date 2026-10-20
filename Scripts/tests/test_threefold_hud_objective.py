"""Keep the contextual Threefold hint visible without replacing tracked expedition goals."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]
def get(relative):return (ROOT/relative).read_text(encoding="utf-8")

class ThreefoldHudGuidance(unittest.TestCase):
    def test_hud_reuses_actual_tracked_goal_and_lore_safe_story_guide(self):
        logic=get("Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp")
        self.assertIn('#include "World/UnmadeThreefoldObjectiveGuide.h"',logic)
        start=logic.index("FString AUnmadePrototypeHub::GetTrackedObjective()const{")
        end=logic.index("FString AUnmadePrototypeHub::GetExperienceJournal()const{",start)
        body=logic[start:end]
        self.assertIn("Experience.Tracked()",body)
        self.assertIn("NextRealmObjective(*Expeditions.Arc(a),*Depth.Arc(a),*Experience.Arc(a))",body)
        self.assertIn("if(a==0)",body)
        self.assertIn("NextThreefoldGuide(WitnessEchoes,WitnessBraid,Dispatch,ReturnWitness,GetGameDay())",body)
        self.assertIn("ThreefoldBeat::InvalidState",body)
        self.assertIn("ThreefoldBeat::Completed",body)
        self.assertIn("F9",body)
        self.assertIn("WITNESS ROUTE",body)
        self.assertIn("return Main;",body)
        self.assertNotIn("SaveGameToSlot",body)
        self.assertNotIn("SetActorLocation",body)
        self.assertNotIn("Advance(",body)

    def test_existing_hud_respects_quest_tracking_no_new_menu(self):
        view=get("Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp")
        self.assertIn('Objective->SetText(FText::FromString(pref->FormatControls(h->GetTrackedObjective())))',view)
        self.assertIn('Objective->SetVisibility(!bOpen&&!h->IsBesDirectedScenePlaying()',view)
        self.assertIn("GetWitnessEchoJournal()",view)
        header=get("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        self.assertIn("FString GetTrackedObjective()const;",header)

if __name__=="__main__":unittest.main()
