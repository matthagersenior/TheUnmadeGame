"""Prove authoritative save loader and writer use the same portable preflight."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def get(p):return (ROOT/p).read_text(encoding="utf-8")
class SavePreflightSource(unittest.TestCase):
 def test_live_loader_and_writer_preflight_existing_and_captured_state(self):
    src=get("Source/TheUnmadeGame/Private/Save/UnmadeSaveSession.cpp")
    self.assertIn('#include "Save/UnmadeCriticalSavePreflight.h"',src)
    self.assertIn("UnmadeCore::ValidCriticalSaveShape(s)",src)
    self.assertIn("return s&&ValidStored(*s)?s:nullptr;",src)
    self.assertIn("if(!existing||!ValidStored(*existing))return false;",src)
    self.assertIn("if(!s||!ValidStored(*s))return false;",src)
    self.assertIn("SessionPlayer->CaptureSession(s);if(!ValidStored(*s))return false;",src)
    self.assertLess(src.index("CaptureSession(s);if(!ValidStored(*s))"),
                    src.index("CommitWithPrevious(*s"))
 def test_legacy_optional_zeroes_and_native_fixture_are_covered(self):
    header=get("Source/TheUnmadeGame/Public/Save/UnmadeCriticalSavePreflight.h")
    self.assertIn("if(save.bHasInventorySnapshot)",header)
    self.assertIn("if(save.bHasWitnessEchoSnapshot)",header)
    self.assertIn("if(save.bHasWitnessReturnSnapshot)",header)
    self.assertNotIn("SaveGameToSlot",header)
    self.assertIn("Tests/save/critical_record_preflight_test.cpp",
                  get(".github/workflows/static-checks.yml"))
if __name__=="__main__":unittest.main()
