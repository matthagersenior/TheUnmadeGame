from pathlib import Path
import json,unittest,importlib.util
ROOT=Path(__file__).resolve().parents[2]
SPEC=importlib.util.spec_from_file_location("cinematic",ROOT/"Scripts/build_cinematic_handoff.py")
M=importlib.util.module_from_spec(SPEC);SPEC.loader.exec_module(M)
class CinematicHandoff(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.c=json.loads((ROOT/"Authoring/cinematic_explainer_v1.json").read_text())
        cls.r=json.loads((ROOT/"Authoring/cinematic_realm_playbook.json").read_text())
    def test_complete_canon_and_time(self):
        M.verify(self.c,self.r)
        self.assertEqual(len(self.c["shots"]),16)
        self.assertEqual(len(self.r["realms"]),9)
        self.assertEqual(self.c["film_duration_seconds"],149.5)
    def test_full_unreal_csv_generated_no_drift(self):
        for f,content in M.generated(self.r,self.c).items():
            self.assertTrue(f.is_file(),f)
            self.assertEqual(f.read_text(encoding="utf-8"),content,str(f))
    def test_invalid_realms_or_cue_timing_refused(self):
        a=json.loads(json.dumps(self.r))
        a["realms"][2]["id"]=a["realms"][1]["id"]
        with self.assertRaises(ValueError):M.verify(self.c,a)
        b=json.loads(json.dumps(self.c))
        b["shots"][4]["start_seconds"]-=1
        with self.assertRaises(ValueError):M.verify(b,self.r)
        b=json.loads(json.dumps(self.c))
        b["shots"][5]["narration"]+=" Held Morning."
        with self.assertRaises(ValueError):M.verify(b,self.r)
    def test_explicit_source_not_fake_unreal_art(self):
        text=(ROOT/"Authoring/cinematic_explainer_v1.json").read_text()
        self.assertIn("no game screenshots",text.lower())
        self.assertIn("scratch voice",text.lower())
        header=(ROOT/"Source/TheUnmadeGame/Public/Authoring/UnmadeCinematicAuthoringRows.h").read_text()
        self.assertIn("FUnmadeCinematicShotAuthoringRow",header)
        self.assertIn("FUnmadeCinematicRealmAuthoringRow",header)
if __name__=="__main__":unittest.main()
