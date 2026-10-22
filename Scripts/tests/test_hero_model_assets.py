"""Source contract for the actual 50 canon-linked binary glTF hero meshes."""
from __future__ import annotations
import importlib.util,json,tempfile,hashlib,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
FILE=ROOT/"Scripts/build_hero_models.py"
class HeroModels(unittest.TestCase):
 def test_source_roster_is_canon_bound_not_placeholder_labels(self):
  source=FILE.read_text()
  for anchor in ("def build_creature(","def build_resident(","def build_environment(",
    "def build_all(","Authoring/world_content_pack.json","trimesh.Scene()",
    "SPECIES","REALMS","sha256","vertex_colors","'glb'"):
   self.assertIn(anchor,source)
  self.assertNotIn("SaveGameToSlot",source)
  self.assertNotIn("GiveReward",source)
  for detail in ("eye_socket","eye_iris","eye_pupil","eye_glint"):
   self.assertIn("k.orb('"+detail+"'",source)
  pack=json.loads((ROOT/"Authoring/world_content_pack.json").read_text())
  self.assertEqual(len(pack["npcs"]),18)
  self.assertEqual(len(pack["realms"]),9)
  self.assertEqual(len({r["id"] for r in pack["npcs"]}),18)
 def test_actual_glb_geometry_is_generated_and_consistent(self):
  if not all(importlib.util.find_spec(m) for m in ("numpy","trimesh","PIL")):
   self.skipTest("3D asset dependencies tested by dedicated hero-model CI workflow")
  import importlib.util as iu
  spec=iu.spec_from_file_location("hero_builder",FILE);builder=iu.module_from_spec(spec);spec.loader.exec_module(builder)
  import trimesh
  with tempfile.TemporaryDirectory() as td:
   out=Path(td);ledger=builder.build_all(ROOT,out)
   self.assertEqual(ledger["asset_count"],50)
   self.assertEqual(ledger["categories"],{"NPC":18,"Creature":12,"Environment":9,"City":11})
   self.assertEqual(len({r["stable_id"] for r in ledger["assets"]}),50)
   self.assertEqual(len({r["sha256"] for r in ledger["assets"]}),50)
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="NPC"},
     {n["id"] for n in json.loads((ROOT/"Authoring/world_content_pack.json").read_text())["npcs"]})
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="Creature"},
     {"creature:"+k for k in builder.SPECIES})
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="Environment"},
     {"realm:"+k for k in builder.REALMS})
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="City"},
     {"city:"+c for c in builder.CITY_SPECS})
   for record in ledger["assets"]:
    path=out/record["file"]
    self.assertEqual(hashlib.sha256(path.read_bytes()).hexdigest(),record["sha256"])
    self.assertGreater(record["triangles"],150)
    self.assertFalse(record["gameplay_authority"])
    self.assertFalse(record["rigged"])
    loaded=trimesh.load(path,force="scene")
    self.assertEqual(len(loaded.geometry),record["parts"])
    self.assertEqual(sum(len(v.faces) for v in loaded.geometry.values()),record["triangles"])
    self.assertTrue(all(g.is_volume or g.faces.shape[0]>0 for g in loaded.geometry.values()))
    if record["category"]=="Creature" and record["stable_id"]!="creature:KilnWisp":
     names=tuple(loaded.geometry)
     for detail in ("eye_socket","eye_iris","eye_pupil","eye_glint"):
      self.assertTrue(any(detail in name for name in names),(record["stable_id"],detail))
   verified=builder.build_all(ROOT,out,True)
   self.assertEqual(verified,ledger)
 def test_manifest_tamper_is_refused(self):
  if not all(importlib.util.find_spec(m) for m in ("numpy","trimesh","PIL")):
   self.skipTest("3D dependencies tested by dedicated hero-model CI workflow")
  spec=importlib.util.spec_from_file_location("hero_tamper",FILE);b=importlib.util.module_from_spec(spec);spec.loader.exec_module(b)
  with tempfile.TemporaryDirectory() as td:
   folder=Path(td);ledger=b.build_all(ROOT,folder)
   first=folder/ledger["assets"][0]["file"]
   first.write_bytes(first.read_bytes()+b"tampered")
   with self.assertRaisesRegex(ValueError,"deterministic"):b.build_all(ROOT,folder,True)
if __name__=="__main__":unittest.main()
