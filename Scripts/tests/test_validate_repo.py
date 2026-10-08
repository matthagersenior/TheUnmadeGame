"""Tests for source-only validation (not substitutes for Unreal automation)."""
import json
import sys
import tempfile
import unittest
from pathlib import Path

SCRIPTS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SCRIPTS))
from validate_repo import validate  # noqa: E402


class ValidateRepositoryTests(unittest.TestCase):
    def test_real_repository_structure(self):
        self.assertEqual(validate(SCRIPTS.parent), [])

    def test_empty_directory_has_explicit_missing_file_errors(self):
        with tempfile.TemporaryDirectory() as temporary:
            problems = validate(Path(temporary))
        self.assertTrue(any("TheUnmadeGame.uproject" in issue for issue in problems))

    def test_malformed_project_descriptor_is_reported(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "TheUnmadeGame.uproject").write_text("{ invalid", encoding="utf-8")
            problems = validate(root)
        self.assertTrue(any("invalid uproject descriptor" in issue for issue in problems))

    def test_missing_unreal_plugin_is_reported(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "TheUnmadeGame.uproject").write_text(
                json.dumps({"Modules": [{"Name": "TheUnmadeGame", "Type": "Runtime"}], "Plugins": []}),
                encoding="utf-8",
            )
            problems = validate(root)
        self.assertIn("uproject must enable EnhancedInput", problems)


if __name__ == "__main__":
    unittest.main()
