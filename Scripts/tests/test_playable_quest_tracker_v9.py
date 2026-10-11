import pathlib
import unittest

ROOT=pathlib.Path(__file__).resolve().parents[2]
def source(path):
    return (ROOT/path).read_text(encoding="utf-8")

class BesQuestObjectiveTest(unittest.TestCase):
    def test_native_nonmodal_status_widget(self):
        s=source("Source/TheUnmadeGame/Private/UI/UnmadeBesQuestTrackerWidget.cpp")
        for marker in ("ConstructWidget<UCanvasPanel>","ConstructWidget<UTextBlock>",
                       "HitTestInvisible","HideObjective","ShowObjective"):
            self.assertIn(marker,s)
    def test_uses_saved_truth_not_static_checklist(self):
        s=source("Source/TheUnmadeGame/Private/World/UnmadeBesQuestTracker.cpp")
        for marker in ("BesHousehold.Save()","BesHousehold.Phase()","LivingTrades.Worker(",
                       "Record.homeVisited","Record.wardenInterviewed","Record.meraConsented",
                       "Record.heatBaskets","Record.daysCold","Worker->owedMarks"):
            self.assertIn(marker,s)
        self.assertIn("BesActiveScene>=0",s)
        self.assertIn("FMath::Square(3400.f)",s)
    def test_hub_refresh_without_journal_input(self):
        s=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertIn("UpdateBesQuestTracker();",s)
        self.assertIn("TObjectPtr<class UUnmadeBesQuestTrackerWidget> BesQuestTracker",
                      source("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h"))
if __name__=="__main__":unittest.main()
