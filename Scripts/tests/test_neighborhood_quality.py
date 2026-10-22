"""Regression: unique, inhabited settlements need inspectable, physically grounded district life."""
import importlib.util
import math
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
MOD=ROOT/"Scripts/build_hero_models.py"
spec=importlib.util.spec_from_file_location("neighborhood_hero",MOD)
if all(importlib.util.find_spec(m) for m in ("numpy","trimesh","PIL")):
 builder=importlib.util.module_from_spec(spec)
 spec.loader.exec_module(builder)
else:builder=None

@unittest.skipUnless(builder is not None,"3D deps installed by the hero asset workflow")
class NeighborhoodQuality(unittest.TestCase):
 def test_city_and_realm_have_habited_distinct_ground_and_public_life(self):
  for city in builder.CITY_SPECS:
   m=builder.build_city_environment(city)
   names=list(m.scene.geometry)
   self.assertEqual(sum("district_ground_" in n for n in names),4,city)
   self.assertGreaterEqual(sum("streetlife_"+city.lower()+"_" in n for n in names),8,city)
   self.assertEqual(sum("_habitation_shell_" in n for n in names),20,city)
  for realm in builder.REALMS:
   m=builder.build_environment(realm)
   city=builder.REGION_HABITAT[realm]
   names=list(m.scene.geometry)
   self.assertEqual(sum("district_ground_" in n for n in names),3,realm)
   self.assertGreaterEqual(sum("streetlife_"+city.lower()+"_" in n for n in names),6,realm)
   self.assertEqual(sum("_habitation_shell_" in n for n in names),12,realm)

 def test_city_footprints_have_independent_non_overlapping_building_centers(self):
  for city in builder.CITY_SPECS:
   centers=builder.district_house_positions(builder.CITY_DISTRICTS[city])
   self.assertEqual(len(centers),20,city)
   self.assertEqual(len(set((round(x,3),round(z,3)) for x,z in centers)),20,city)
   nearest=min(math.hypot(a[0]-b[0],a[1]-b[1])
               for i,a in enumerate(centers) for b in centers[i+1:])
   self.assertGreaterEqual(nearest,3.05,city+" has intersecting 2.25-2.73m houses")

 def test_manifest_has_real_distinct_district_viewpoints(self):
  import tempfile
  with tempfile.TemporaryDirectory() as tmp:
   output=builder.build_all(ROOT,Path(tmp))
   self.assertEqual(output["asset_count"],50)
   for item in output["assets"]:
    views=item["district_views"]
    if item["category"] in ("City","Environment"):
     expected=4 if item["category"]=="City" else 3
     self.assertEqual(len(views),expected,item["stable_id"])
     self.assertTrue(all(len(v)==3 for v in views))
     self.assertEqual(len({tuple(v) for v in views}),expected)
    else:self.assertEqual(views,[])
