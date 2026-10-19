"""Save-backed first-chamber anomalies should sound and read differently in nine realms."""
import hashlib,tempfile,unittest
from pathlib import Path
from Scripts import build_identity_audio as audio
ROOT=Path(__file__).resolve().parents[2]
REALMS=("crossings","saltwake","cinderhold","drevlach","orravane","vathless","eillun","tharniv","auvren")
class FirstFractureSenses(unittest.TestCase):
 def test_nine_original_first_fracture_stings_preserve_existing_forty_five(self):
  with tempfile.TemporaryDirectory() as first,tempfile.TemporaryDirectory() as second:
   a=audio.build(Path(first));b=audio.build(Path(second))
   self.assertEqual(audio.validate(Path(first)),a)
   self.assertEqual(audio.validate(Path(second)),b)
   self.assertEqual(len(a["assets"]),54)
   self.assertEqual([x["cue"] for x in a["assets"][45:]],
                    ["first_fracture_"+r for r in REALMS])
   self.assertEqual([x["cue"] for x in a["assets"][36:45]],
                    ["report_review_"+r for r in REALMS])
   self.assertEqual(len({x["sha256"] for x in a["assets"][45:]}),9)
   self.assertEqual([x["sha256"] for x in a["assets"]],
                    [x["sha256"] for x in b["assets"]])
   for row in a["assets"][45:]:
    self.assertGreater(row["rms"],0)
    self.assertLess(row["peak"],.75)
    self.assertGreater(row["duration"],1.0)
    self.assertEqual(hashlib.sha256((Path(first)/row["file"]).read_bytes()).hexdigest(),row["sha256"])
    self.assertEqual(row["unreal_asset"],
                     "/Game/UnmadeProduction/Audio/Identity/SFX_ID_"+row["cue"])
 def test_nine_facts_have_unique_sensory_provenance_without_story_spoilers(self):
  rules=(ROOT/"Source/TheUnmadeGame/Public/World/UnmadeFirstFractureRules.h").read_text()
  for realm in REALMS:self.assertIn('"first_fracture_'+realm+'"',rules)
  self.assertIn("FirstFractureCue(",rules)
  self.assertIn("FirstFractureLine(",rules)
  self.assertNotIn("Nhal-Vey",rules)
  self.assertNotIn("Held Morning",rules)
 def test_sound_only_on_first_physically_saved_chamber_arrival(self):
  cpp=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp").read_text()
  start=cpp.index("if(playerReady&&!bExpeditionSaveRejected)")
  segment=cpp[start:cpp.index("for(int a=0;a<9;++a){auto t=Expeditions.Arc(a);",start)]
  self.assertIn("CommitExpeditions(before,p)",segment)
  self.assertIn("!(before.Arc(a)->rooms&1)",segment)
  self.assertIn("(Expeditions.Arc(a)->rooms&1)",segment)
  self.assertIn("FirstFractureCue(a)",segment)
  self.assertIn("FirstFractureLine(a)",segment)
  self.assertIn("UnmadePresentation::Sound(p,UTF8_TO_TCHAR(cue))",segment)
  self.assertIn("NotifyRpg(",segment)
  self.assertLess(segment.index("CommitExpeditions(before,p)"),segment.index("FirstFractureCue(a)"))
  self.assertNotIn("PlayIdentityMotion(",segment)
  self.assertNotIn("Expeditions.Boss(",segment)
  self.assertNotIn("CrossRealmCarry.",segment)
  self.assertIn("GetDefault<UUnmadePlayerPreferences>()->AudioVolume",
    (ROOT/"Source/TheUnmadeGame/Public/Combat/UnmadePresentation.h").read_text())
 def test_import_manifest_non_destructive(self):
  script=(ROOT/"Scripts/unreal_editor/import_identity_audio.py").read_text()
  self.assertIn("identity_audio_manifest.json",script)
  self.assertIn("replace_existing=False",script)
if __name__=="__main__":unittest.main()
