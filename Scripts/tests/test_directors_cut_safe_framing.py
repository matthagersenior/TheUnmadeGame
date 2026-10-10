"""Regression protection for THE UNMADE director animatic framing/audio policy."""
import copy
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

SCRIPT = Path(__file__).resolve().parents[1] / "reframe_directors_cut.py"
spec = importlib.util.spec_from_file_location("reframe_directors_cut", SCRIPT)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


class DirectorsCutSafeAreaTests(unittest.TestCase):
    def setUp(self):
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        self.root = Path(tmp.name)
        atlas = self.root / "rebuild" / "art_atlas"
        atlas.mkdir(parents=True)
        (atlas / "test.jpg").write_bytes(b"preflight never opens this mock")
        scenes = []
        for i in range(56):
            scenes.append(dict(id=f"C{i+1:02d}", chapter="THE IMPOSSIBLE PERSON",
                               title="An impossible memory", subtitle="Nothing is cropped",
                               art="test.jpg", start=i*12.5, end=(i+1)*12.5,
                               duration=12.5))
        self.manifest = dict(scenes=scenes, total_seconds=700.0)
        (self.root / "THE_UNMADE_Full_Universe_Directors_Cut_manifest.json").write_text(json.dumps(self.manifest))

    def test_exact_safe_area_and_source(self):
        self.assertEqual(len(module.validate_scene_source(self.manifest, self.root)), 56)
        self.assertEqual(module.ART_BOX, (80, 92, 1200, 542))
        self.assertLess(module.ART_BOX[3], module.TITLE_Y)
        self.assertLess(module.TITLE_Y, module.SUBTITLE_Y)
        self.assertLess(module.SUBTITLE_Y, module.FOOTER_Y)

    def test_bad_scene_and_timing_rejected(self):
        bad = copy.deepcopy(self.manifest)
        bad["scenes"][3]["id"] = "C90"
        with self.assertRaises(ValueError):
            module.validate_scene_source(bad, self.root)
        bad = copy.deepcopy(self.manifest)
        bad["scenes"][3]["start"] += 3
        with self.assertRaises(ValueError):
            module.validate_scene_source(bad, self.root)

    def test_missing_art_and_traversal_rejected(self):
        bad = copy.deepcopy(self.manifest)
        bad["scenes"][0]["art"] = "not_there.jpg"
        with self.assertRaises(FileNotFoundError):
            module.validate_scene_source(bad, self.root)
        bad["scenes"][0]["art"] = "../secret.jpg"
        with self.assertRaises(ValueError):
            module.validate_scene_source(bad, self.root)

    def test_voice_not_enabled_implicitly(self):
        with self.assertRaisesRegex(ValueError, "approved narration"):
            module.build(self.root, self.root / "out.mp4", None)

    def test_check_needs_no_media_toolchain(self):
        module.build(self.root, self.root / "out.mp4", None, check=True)


if __name__ == "__main__":
    unittest.main()
