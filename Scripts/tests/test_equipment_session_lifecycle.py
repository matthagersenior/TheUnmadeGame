"""Inventory and profile-switch UI contracts for the pre-PC first playable."""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def source(path):
    return (ROOT / path).read_text(encoding="utf-8")


class EquipmentSessionLifecycle(unittest.TestCase):
    def test_successful_equipment_change_does_not_toggle_open_inventory(self):
        player = source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for action in ("EquipNextWeapon", "EquipNextArmor", "EquipNextCharm"):
            with self.subTest(action=action):
                handler = player.split(f"void AUnmadeCharacter::{action}()", 1)[1].split("\n}", 1)[0]
                self.assertIn("Equipment->EquipNext(", handler)
                self.assertIn("if(!IsRpgMenuOpen())ShowInventory();", handler)

    def test_second_explicit_switch_press_does_not_retry_a_failed_save(self):
        widget = source("Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp")
        for method, intent in (("LoadProfile", "Load"), ("NewProfile", "New")):
            with self.subTest(method=method):
                handler = widget.split(f"void UUnmadeRpgWidget::{method}(){{", 1)[1].split("\n", 1)[0]
                self.assertIn(f"!SessionConfirm.Awaiting(UnmadeCore::SessionIntent::{intent},SelectedProfile)&&p->SaveSessionNow()", handler)
                self.assertIn(f"SessionConfirm.Request(UnmadeCore::SessionIntent::{intent},SelectedProfile,", handler)
        backup = widget.split("void UUnmadeRpgWidget::LoadBackup(){", 1)[1].split("\n", 1)[0]
        self.assertNotIn("SaveSessionNow", backup)

    def test_confirmation_remains_bound_to_intent_and_profile(self):
        code = r"""
#include "UI/UnmadePlayerFacingRules.h"
#include <cassert>
int main() {
 using namespace UnmadeCore;
 SessionConfirmation c;
 assert(!c.Awaiting(SessionIntent::Load, 2));
 assert(!c.Request(SessionIntent::Load, 2, false));
 assert(c.Awaiting(SessionIntent::Load, 2));
 assert(!c.Awaiting(SessionIntent::New, 2));
 assert(!c.Awaiting(SessionIntent::Load, 1));
 assert(c.Request(SessionIntent::Load, 2, false));
 assert(!c.Awaiting(SessionIntent::Load, 2));
 assert(!c.Request(SessionIntent::New, 3, false));
 assert(c.Awaiting(SessionIntent::New, 3));
 c.Clear();
 assert(!c.Awaiting(SessionIntent::New, 3));
}
"""
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "confirmation.cpp"
            exe = Path(tmp) / "confirmation"
            src.write_text(code, encoding="utf-8")
            compiled = subprocess.run(
                ["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                 "-I", str(ROOT / "Source/TheUnmadeGame/Public"), str(src), "-o", str(exe)],
                capture_output=True, text=True)
            self.assertEqual(compiled.returncode, 0, compiled.stderr)
            ran = subprocess.run([str(exe)], capture_output=True, text=True)
            self.assertEqual(ran.returncode, 0, ran.stderr)


if __name__ == "__main__":
    unittest.main()
