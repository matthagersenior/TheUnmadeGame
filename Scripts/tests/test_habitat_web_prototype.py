"""The public habitat experience must react to player movement, not be an inert gallery."""
from pathlib import Path
import re,shutil,subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
PAGE=ROOT/"site/play-habitats.html"
CANON=ROOT/"Source/TheUnmadeGame/Private/World/UnmadePlayableEncounters.cpp"
class HabitatWebPrototype(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.text=PAGE.read_text(encoding="utf-8")
  cls.enc=CANON.read_text(encoding="utf-8")
 def test_all_six_previously_authored_sites_and_creatures(self):
  ids=("Reach.AshRoad","Reach.GlassCut","Bellwold.Forgotten","Paperhaven.Torn","Saltwake.Saltglass","Cinderhold.Slag")
  for ident in ids:
   self.assertIn(ident,self.text)
   self.assertIn(ident,self.enc)
  for creature in ("Cinderjaw","Veilwatch","Hollow Hound","Inkmaw","Saltglass Crawler","Ashmaw"):
   self.assertIn(creature,self.text)
   self.assertIn(creature,self.enc)
 def test_physical_reactive_loop_not_only_interact_marker(self):
  for feature in ("function visibleFromDen()","function interact()","function animate()",
                  "function intersectsRect(","shutter.visible=false",
                  "creature defends its territory","composure-16",
                  "localStorage.setItem","cue=dist","Line of sight blocked",
                  "data-move=","WASD"):
   self.assertIn(feature,self.text)
  self.assertIn("not Unreal gameplay or admitted canon habitat placement",self.text)
  self.assertIn("No permanent loss",self.text)
 def test_publish_and_js_syntax(self):
  script=re.findall(r'<script type="module">([\s\S]+?)</script>',self.text)
  self.assertEqual(len(script),1)
  if shutil.which("node"):
   with tempfile.TemporaryDirectory() as td:
    path=Path(td)/"habitats.mjs";path.write_text(script[0],encoding="utf-8")
    subprocess.run(["node","--check",str(path)],check=True,capture_output=True,text=True)
  build=(ROOT/"Scripts/publishing/build_public_site.py").read_text()
  self.assertIn("play-habitats.html",build)
  self.assertIn("play-habitats.html",(ROOT/"site/index.html").read_text())
