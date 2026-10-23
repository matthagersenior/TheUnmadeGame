"""Source contract for the actual 68 canon-linked binary glTF visual references."""
from __future__ import annotations
import importlib.util,json,tempfile,hashlib,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
FILE=ROOT/"Scripts/build_hero_models.py"
class HeroModels(unittest.TestCase):
 def test_source_roster_is_canon_bound_not_placeholder_labels(self):
  source=FILE.read_text()
  for anchor in ("def build_creature(","def build_expedition_boss(","def build_lair_entrance(",
    "def build_resident(","def build_environment(","def build_all(",
    "read_expedition_roster(root)","Authoring/world_content_pack.json","trimesh.Scene()",
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
  arcs=json.loads((ROOT/"Authoring/frontier_adventures_v14.json").read_text())["arcs"]
  self.assertEqual(len(arcs),9)
  self.assertEqual(len({row["boss"] for row in arcs}),9)
  self.assertEqual(len({row["dungeon"] for row in arcs}),9)
 def test_people_bosses_and_creatures_have_anatomical_variation(self):
  if not all(importlib.util.find_spec(m) for m in ("numpy","trimesh","PIL")):
   self.skipTest("3D asset dependencies tested by dedicated hero-model CI workflow")
  spec=importlib.util.spec_from_file_location("hero_anatomy",FILE);b=importlib.util.module_from_spec(spec);spec.loader.exec_module(b)
  pack=json.loads((ROOT/"Authoring/world_content_pack.json").read_text())
  people=[]
  for i,row in enumerate(pack["npcs"]):
   model=b.build_resident((row["id"],row["display_name"],row["realm"],row["visual_direction"]),i)
   names=tuple(model.scene.geometry)
   self.assertGreaterEqual(model.count,90,row["id"])
   for part in ("resident_jaw","resident_ear_","resident_iris_","resident_finger_","coat_lapel_"):
    self.assertTrue(any(part in name for name in names),(row["id"],part))
   people.append((round(float(model.scene.bounds[1][1]),2),round(float(model.scene.bounds[0][0]),2),model.count))
  self.assertGreaterEqual(len(set(people)),12,"NPC proportions and hair should vary across the cast")
  arcs=json.loads((ROOT/"Authoring/frontier_adventures_v14.json").read_text())["arcs"]
  silhouettes=[]
  for i,arc in enumerate(arcs):
   model=b.build_expedition_boss(arc,i);names=tuple(model.scene.geometry)
   self.assertGreaterEqual(model.count,50,arc["boss"])
   for part in ("boss_mask_cheek_","boss_mask_brow_","boss_abdominal_lamella_","boss_hand_claw_","boss_signature_"):
    self.assertTrue(any(part in name for name in names),(arc["boss"],part))
   bounds=model.scene.bounds
   silhouettes.append(tuple(round(float(v),2) for v in (bounds[1][1]-bounds[0][1],bounds[1][0]-bounds[0][0],bounds[1][2]-bounds[0][2])))
  self.assertGreaterEqual(len(set(silhouettes)),8,"boss outlines should remain individually recognizable")
  for kind in b.SPECIES:
   model=b.build_creature(kind);names=tuple(model.scene.geometry)
   self.assertGreaterEqual(model.count,20,kind)
   anatomy={"Cinderjaw":"dermal_shoulder_scute_","PaleStrider":"stride_spine_tine_",
    "Veilwatch":"shroud_pleat_","GleamMoth":"wing_membrane_rib_",
    "HollowHound":"rib_socket_rim_","GriefLantern":"cage_crossbrace_",
    "Inkmaw":"binding_rivet_","RedactionEye":"corneal_iris_lamination_",
    "SaltglassCrawler":"shell_growth_ridge_","DrownedWatch":"drowned_scale_",
    "Ashmaw":"slag_armor_seam_","KilnWisp":"coal_crust_node_"}[kind]
   self.assertTrue(any(anatomy in name for name in names),(kind,anatomy))
 def test_public_model_gallery_explains_each_creature_signature(self):
  viewer=(ROOT/"site/real-models.html").read_text()
  for species in ("Cinderjaw","PaleStrider","Veilwatch","GleamMoth",
    "HollowHound","GriefLantern","Inkmaw","RedactionEye",
    "SaltglassCrawler","DrownedWatch","Ashmaw","KilnWisp"):
   self.assertIn(species+":'",viewer)
  self.assertIn("function descriptionFor(a)",viewer)
  self.assertIn("static geometry accents, not animated tells or new abilities",viewer)
  for category in ("Boss","Lair"):
   self.assertIn('data-group="'+category+'"',viewer)
 def test_actual_glb_geometry_is_generated_and_consistent(self):
  if not all(importlib.util.find_spec(m) for m in ("numpy","trimesh","PIL")):
   self.skipTest("3D asset dependencies tested by dedicated hero-model CI workflow")
  import importlib.util as iu
  spec=iu.spec_from_file_location("hero_builder",FILE);builder=iu.module_from_spec(spec);spec.loader.exec_module(builder)
  import trimesh
  with tempfile.TemporaryDirectory() as td:
   out=Path(td);ledger=builder.build_all(ROOT,out)
   self.assertEqual(ledger["asset_count"],68)
   self.assertEqual(ledger["categories"],{"NPC":18,"Creature":12,"Environment":9,"City":11,"Boss":9,"Lair":9})
   self.assertEqual(len({r["stable_id"] for r in ledger["assets"]}),68)
   self.assertEqual(len({r["sha256"] for r in ledger["assets"]}),68)
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="NPC"},
     {n["id"] for n in json.loads((ROOT/"Authoring/world_content_pack.json").read_text())["npcs"]})
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="Creature"},
     {"creature:"+k for k in builder.SPECIES})
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="Environment"},
     {"realm:"+k for k in builder.REALMS})
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="City"},
     {"city:"+c for c in builder.CITY_SPECS})
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="Boss"},
     {"boss.expedition.signature."+str(i) for i in range(9)})
   self.assertEqual({r["stable_id"] for r in ledger["assets"] if r["category"]=="Lair"},
     {"lair.expedition."+str(i) for i in range(9)})
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
    if record["category"]=="Creature" and record["stable_id"] not in {
      "creature:GriefLantern","creature:KilnWisp"}:
     names=tuple(loaded.geometry)
     for detail in ("eye_socket","eye_iris","eye_pupil","eye_glint"):
      self.assertTrue(any(detail in name for name in names),(record["stable_id"],detail))
    if record["category"]=="Creature":
     signature_detail={
      "Cinderjaw":"jaw_hinge_iron",
      "PaleStrider":"lunge_tendon",
      "Veilwatch":"pulse_lens_vane",
      "GleamMoth":"wing_ocellus",
      "HollowHound":"exposed_rib",
      "GriefLantern":"bell_clapper",
      "Inkmaw":"layered_jaw_leaf",
      "RedactionEye":"redaction_brow",
      "SaltglassCrawler":"scute_center_ridge",
      "DrownedWatch":"helmet_tide_seam",
      "Ashmaw":"chimney_vent_louver",
      "KilnWisp":"orbiting_cinder",
     }[record["stable_id"].split(":",1)[1]]
     self.assertTrue(any(signature_detail in name for name in loaded.geometry),
      (record["stable_id"],signature_detail))
     anatomy_detail={
      "Cinderjaw":"dermal_shoulder_scute_","PaleStrider":"stride_spine_tine_",
      "Veilwatch":"shroud_pleat_","GleamMoth":"wing_membrane_rib_",
      "HollowHound":"rib_socket_rim_","GriefLantern":"cage_crossbrace_",
      "Inkmaw":"binding_rivet_","RedactionEye":"corneal_iris_lamination_",
      "SaltglassCrawler":"shell_growth_ridge_","DrownedWatch":"drowned_scale_",
      "Ashmaw":"slag_armor_seam_","KilnWisp":"coal_crust_node_",
     }[record["stable_id"].split(":",1)[1]]
     self.assertTrue(any(anatomy_detail in name for name in loaded.geometry),
      (record["stable_id"],anatomy_detail))
    if record["category"]=="NPC":
     for detail in ("resident_jaw","resident_iris_","resident_finger_","coat_lapel_"):
      self.assertTrue(any(detail in name for name in loaded.geometry),(record["stable_id"],detail))
     self.assertGreaterEqual(record["parts"],90,record["stable_id"])
    if record["category"]=="Boss":
     signature="boss_signature_"
     self.assertTrue(any(signature in name for name in loaded.geometry),record["stable_id"])
     for detail in ("boss_mask_cheek_","boss_mask_brow_","boss_abdominal_lamella_","boss_hand_claw_"):
      self.assertTrue(any(detail in name for name in loaded.geometry),(record["stable_id"],detail))
     self.assertGreaterEqual(record["parts"],50,record["stable_id"])
    if record["category"]=="Lair":
     signature="lair_signature_"
     self.assertTrue(any(signature in name for name in loaded.geometry),record["stable_id"])
     self.assertGreaterEqual(record["parts"],18,record["stable_id"])
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
