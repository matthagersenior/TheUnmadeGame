"""Pre-Unreal contract checks for player-facing controls and responsive prompts."""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


class PrePcControlParity(unittest.TestCase):
    def test_analog_triggers_are_supported_but_sticks_are_not(self):
        # Exercise the same portable predicate used by the Unreal input adapter.
        program = r"""
#include "UI/UnmadePlayerFacingRules.h"
#include <cassert>
int main() {
 using UnmadeCore::CanBindPlayerControl;
 assert(CanBindPlayerControl(true, false, false));
 assert(CanBindPlayerControl(true, true, true));    // gameplay left/right triggers
 assert(!CanBindPlayerControl(true, true, false));  // analog movement sticks
 assert(!CanBindPlayerControl(false, false, true)); // invalid key
 assert(UnmadeCore::ShouldCaptureTriggerAnalog(true, true, 0.75f));
 assert(!UnmadeCore::ShouldCaptureTriggerAnalog(true, true, 0.12f));
 assert(!UnmadeCore::ShouldCaptureTriggerAnalog(false, true, 1.0f));
 assert(!UnmadeCore::ShouldCaptureTriggerAnalog(true, false, 1.0f));
}
"""
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "controls.cpp"
            out = Path(tmp) / "controls"
            src.write_text(program)
            compile_result = subprocess.run(
                ["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                 "-I", str(ROOT / "Source/TheUnmadeGame/Public"),
                 str(src), "-o", str(out)],
                capture_output=True, text=True,
            )
            self.assertEqual(compile_result.returncode, 0, compile_result.stderr)
            run_result = subprocess.run([str(out)], capture_output=True, text=True)
            self.assertEqual(run_result.returncode, 0, run_result.stderr)

    def test_unreal_adapter_uses_portable_binding_policy(self):
        src = (ROOT / "Source/TheUnmadeGame/Private/UI/UnmadePlayerPreferences.cpp").read_text()
        self.assertIn("UnmadeCore::CanBindPlayerControl", src)
        self.assertIn("key==EKeys::Gamepad_LeftTrigger", src)
        self.assertIn("key==EKeys::Gamepad_RightTrigger", src)

    def test_analog_trigger_capture_is_connected_to_ui_input(self):
        header = (ROOT / "Source/TheUnmadeGame/Public/UI/UnmadeRpgWidget.h").read_text()
        widget = (ROOT / "Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp").read_text()
        self.assertIn("NativeOnAnalogValueChanged", header)
        self.assertIn("FReply UUnmadeRpgWidget::NativeOnAnalogValueChanged", widget)
        self.assertIn("UnmadeCore::ShouldCaptureTriggerAnalog", widget)
        self.assertIn("CaptureControl(e.GetKey())", widget)

    def test_contextual_prompt_is_the_one_displayed(self):
        src = (ROOT / "Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp").read_text()
        self.assertIn("pref->FormatControls(contextual)", src)
        self.assertNotIn("pref->FormatControls(h->GetDepthPrompt(p))", src)


if __name__ == "__main__":
    unittest.main()
