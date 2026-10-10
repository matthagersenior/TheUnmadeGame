"""Director edition must cover the nine realms and retain audible-export policy."""
from pathlib import Path
import copy
import importlib.util
import json
import unittest

ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "Scripts/validate_directors_cut.py"
SPEC = importlib.util.spec_from_file_location("validate_directors_cut", SCRIPT)
M = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(M)


class DirectorsCutContract(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.data = json.loads((ROOT / "Authoring/cinematic_directors_cut_v2_index.json").read_text(encoding="utf-8"))

    def test_full_cinematic_shots_are_contiguous_and_complete(self):
        M.verify_index(self.data)
        self.assertEqual(len(self.data["chapters"]), 7)

    def test_lost_audio_and_missing_narration_rejected(self):
        data = copy.deepcopy(self.data)
        data["audio_acceptance"]["channels"] = 0
        with self.assertRaises(ValueError):
            M.verify_index(data)
        data = copy.deepcopy(self.data)
        data["audio_acceptance"]["voice_present"] = False
        with self.assertRaises(ValueError):
            M.verify_index(data)

    def test_missing_world_or_branch_rejected(self):
        data = copy.deepcopy(self.data)
        data["chapters"][1]["scenes"][2]["title"] = "UNKNOWN MISSING WORLD"
        with self.assertRaises(ValueError):
            M.verify_index(data)
        data = copy.deepcopy(self.data)
        data["chapters"][6]["scenes"][0]["title"] = "THE ONLY MORNING"
        with self.assertRaises(ValueError):
            M.verify_index(data)

    def test_audio_video_time_drift_rejected(self):
        data = copy.deepcopy(self.data)
        data["chapters"][0]["scenes"][2]["start_seconds"] += 5
        with self.assertRaises(ValueError):
            M.verify_index(data)
        data = copy.deepcopy(self.data)
        data["chapters"][6]["scenes"][-1]["end_seconds"] += 9
        with self.assertRaises(ValueError):
            M.verify_index(data)

    def test_not_represented_as_real_gameplay(self):
        data = copy.deepcopy(self.data)
        data["editorial_status"] = "fully built UE5 gameplay"
        with self.assertRaises(ValueError):
            M.verify_index(data)


if __name__ == "__main__":
    unittest.main()
