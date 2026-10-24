"""Nine distinct, reproducible prototype Foley signatures for received evidence reviews."""
import hashlib, tempfile, unittest
from pathlib import Path
from Scripts import build_identity_audio as identity
ROOT=Path(__file__).resolve().parents[2]
REALMS=("crossings","saltwake","cinderhold","drevlach","orravane","vathless","eillun","tharniv","auvren")
class NineRealmReviewAudio(unittest.TestCase):
 def test_original_identity_set_is_preserved_and_nine_new_reviews_are_deterministic(self):
  with tempfile.TemporaryDirectory() as d1,tempfile.TemporaryDirectory() as d2:
   one=identity.build(Path(d1));two=identity.build(Path(d2))
   self.assertEqual(identity.validate(Path(d1)),one)
   self.assertEqual(identity.validate(Path(d2)),two)
   self.assertEqual(len(one["assets"]),54)
   expected=["report_review_"+realm for realm in REALMS]
   self.assertEqual([row["cue"] for row in one["assets"][36:45]],expected)
   self.assertEqual([row["cue"] for row in one["assets"][:36]],
      identity.EVENTS+["ambience_"+r for r in REALMS])
   hashes=[row["sha256"] for row in one["assets"][36:45]]
   self.assertEqual(len(set(hashes)),9)
   self.assertEqual(hashes,[row["sha256"] for row in two["assets"][36:45]])
   for row in one["assets"][-9:]:
    self.assertTrue((Path(d1)/row["file"]).exists())
    self.assertEqual(hashlib.sha256((Path(d1)/row["file"]).read_bytes()).hexdigest(),row["sha256"])
    self.assertGreater(row["rms"],0)
    self.assertLess(row["peak"],0.8)
    self.assertEqual(row["unreal_asset"],"/Game/UnmadeProduction/Audio/Identity/SFX_ID_"+row["cue"])
 def test_runtime_review_uses_nine_exact_asset_names_and_preserves_audio_preferences(self):
  rules=(ROOT/"Source/TheUnmadeGame/Public/World/UnmadeCrossRealmReviewRules.h").read_text()
  native=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp").read_text()
  presentation=(ROOT/"Source/TheUnmadeGame/Public/Combat/UnmadePresentation.h").read_text()
  for realm in REALMS:self.assertIn('"report_review_'+realm+'"',rules)
  self.assertIn("CrossRealmReviewCue(arc)",native)
  self.assertIn("UnmadePresentation::Sound(p,UTF8_TO_TCHAR(cue))",native)
  self.assertIn("GetDefault<UUnmadePlayerPreferences>()->AudioVolume",presentation)
  self.assertIn("/Game/UnmadeProduction/Audio/Identity/",presentation)
 def test_unreal_importer_consumes_manifest_and_protects_edited_soundwaves(self):
  script=(ROOT/"Scripts/unreal_editor/import_identity_audio.py").read_text()
  self.assertIn("identity_audio_manifest.json",script)
  self.assertIn("replace_existing=False",script)
  self.assertIn("row['unreal_asset']",script)
if __name__=="__main__":unittest.main()
