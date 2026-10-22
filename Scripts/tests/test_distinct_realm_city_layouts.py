"""Regression guard: realms and settlements must not share one town-square grid."""
import importlib.util
import unittest
from importlib.util import find_spec
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
SPEC=importlib.util.spec_from_file_location("unmade_hero_topologies",ROOT/"Scripts/build_hero_models.py")
if all(find_spec(name) for name in ("numpy","trimesh","PIL")):
 builder=importlib.util.module_from_spec(SPEC)
 SPEC.loader.exec_module(builder)
else:
 builder=None

def signature(routes):
 return tuple(tuple((round(float(x),2),round(float(z),2)) for x,z in poly) for poly in routes)

@unittest.skipUnless(builder is not None, '3D geometry libraries are exercised in the dedicated asset workflow')
class DistinctRealmCityLayouts(unittest.TestCase):
 def test_all_realms_have_distinct_navigational_shapes(self):
  self.assertEqual(set(builder.REALM_PATHS),set(builder.REALMS))
  keys=[signature(builder.REALM_PATHS[r]) for r in builder.REALMS]
  self.assertEqual(len(set(keys)),9,"Every realm needs a unique spatial footprint, not one straight corridor with recolored walls")
  for key in keys:
   self.assertGreaterEqual(len(key),2,"A single shared central road does not establish a unique environment")
   self.assertTrue(any(len(segment)>2 for segment in key),"Provide branching or shaped routes, not just straight rectangles")
 def test_every_named_city_has_distinct_layout_and_an_accurate_realm(self):
  expected={"Crossings":"ThreefoldReach","Bellwold":"ThreefoldReach","Paperhaven":"ThreefoldReach",
   "Saltwake":"WidowedRain","Cinderhold":"HearthBeneath","Drevlach":"TidalLedger",
   "Orravane":"SkyBelow","Vathless":"CinderSpine","Eillun":"HundredUnlived",
   "Tharniv":"OrchardOfKings","Auvren":"FirstAbsence"}
  self.assertEqual({name:v["realm"] for name,v in builder.CITY_SPECS.items()},expected)
  shapes=[signature(c["routes"]) for c in builder.CITY_SPECS.values()]
  self.assertEqual(len(set(shapes)),len(expected),"Different city names must not just load the same template")
  for name,c in builder.CITY_SPECS.items():
   self.assertNotEqual(signature(c["routes"]),signature(builder.REALM_PATHS[c["realm"]]))
 def test_3d_landmarks_have_realm_and_city_specific_structure(self):
  for realm in builder.REALMS:
   k=builder.build_environment(realm)
   names=set(k.scene.geometry)
   self.assertTrue(any("route_" in n for n in names),realm)
   self.assertTrue(any(realm.lower() in n.lower() for n in names),realm)
   self.assertFalse(any("surveyor_path_stone" in n for n in names),realm)
  for city,spec in builder.CITY_SPECS.items():
   k=builder.build_city_environment(city)
   names=set(k.scene.geometry)
   self.assertTrue(any("city_route_" in n for n in names),city)
   self.assertTrue(any(("identity_"+city.lower()) in n.lower() for n in names),city)
   self.assertGreater(len(names),22,city)
  bell=builder.build_city_environment("Bellwold")
  self.assertTrue(any("uncast_yoke" in n for n in bell.scene.geometry))
  paper=builder.build_city_environment("Paperhaven")
  self.assertTrue(any("archive_vault" in n for n in paper.scene.geometry))
 def test_export_includes_every_city_not_only_realm_overviews(self):
  import tempfile
  with tempfile.TemporaryDirectory() as td:
   data=builder.build_all(ROOT,Path(td))
   self.assertEqual(data["asset_count"],50)
   self.assertEqual(data["categories"],{"NPC":18,"Creature":12,"Environment":9,"City":11})
   self.assertEqual({a["stable_id"] for a in data["assets"] if a["category"]=="City"},
    {"city:"+city for city in builder.CITY_SPECS})
   self.assertEqual(len({a["sha256"] for a in data["assets"] if a["category"] in ("City","Environment")}),20)

if __name__=="__main__":unittest.main()
