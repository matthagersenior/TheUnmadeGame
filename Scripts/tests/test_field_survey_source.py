import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
class FieldSurveySource(unittest.TestCase):
    def test_physical_custody_and_atomic_publication(self):
        source=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadePlayableEncounters.cpp').read_text()
        self.assertIn('Distance>FMath::Square(260.f)',source)
        self.assertIn('LineTraceTestByChannel',source)
        self.assertLess(source.index('SaveGameToSlot'),source.index('FieldSurveyStages=Save->FieldSurveyStages'))
        self.assertIn('legacy->Version==1&&legacy->Stages.Num()==6',source)
        self.assertIn('stage<0||stage>2',source)
        self.assertIn('Previous<2',source)
        self.assertIn('TryFieldSurveyInteraction(this,ResidentDistSq)',
            (ROOT/'Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp').read_text())
