"""Visible district-level 3D detail contract, not just isolated hero landmarks."""
import importlib.util
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
if all(importlib.util.find_spec(m) for m in ("numpy","trimesh","PIL")):
 spec=importlib.util.spec_from_file_location("all_district_surfaces",ROOT/"Scripts/build_hero_models.py")
 builder=importlib.util.module_from_spec(spec)
 spec.loader.exec_module(builder)
else:builder=None

@unittest.skipUnless(builder is not None,"Model deps installed in art workflow")
class DistrictSurfaceTests(unittest.TestCase):
 def test_all_city_dwellings_have_embedded_uv_pbr_facades_and_roofs(self):
  import numpy as np
  from trimesh.visual.texture import TextureVisuals
  for city in builder.CITY_SPECS:
   scene=builder.build_city_environment(city).scene
   found={name:geo for name,geo in scene.geometry.items() if "_district_pbr_" in name}
   roofs=[(name,g) for name,g in found.items() if "_pitched_roof_" in name]
   faces=[(name,g) for name,g in found.items() if "_apertured_facade_" in name]
   self.assertEqual(len(roofs),20,city)
   self.assertEqual(len(faces),20,city)
   self.assertGreaterEqual(len(found),100,city)
   for name,geo in found.items():
    self.assertIsInstance(geo.visual,TextureVisuals,name)
    self.assertEqual(len(geo.visual.uv),len(geo.vertices),name)
    self.assertIsNotNone(geo.visual.material.baseColorTexture,name)
    self.assertIsNotNone(geo.visual.material.normalTexture,name)
   self.assertTrue(all(np.ptp(g.vertices[:,1])>.25 for _,g in roofs),city)
 def test_all_realm_residential_roofs_are_textured(self):
  for realm,city in builder.REGION_HABITAT.items():
   names=list(builder.build_environment(realm).scene.geometry)
   self.assertEqual(sum("_district_pbr_pitched_roof_" in n for n in names),12,realm)
   self.assertEqual(sum("_district_pbr_apertured_facade_" in n for n in names),12,realm)
 def test_individual_district_terrain_is_sculpted_with_surface_uvs(self):
  import numpy as np
  from trimesh.visual.texture import TextureVisuals
  for city in builder.CITY_SPECS:
   parts=[g for n,g in builder.build_city_environment(city).scene.geometry.items() if "_district_terrain_relief_" in n]
   self.assertEqual(len(parts),4,city)
   for g in parts:
    self.assertGreaterEqual(len(g.faces),125)
    self.assertGreater(np.ptp(g.vertices[:,1]),.10)
    self.assertIsInstance(g.visual,TextureVisuals)
    self.assertIsNotNone(g.visual.material.normalTexture)
  for realm in builder.REALMS:
   names=list(builder.build_environment(realm).scene.geometry)
   self.assertEqual(sum("_district_terrain_relief_" in n for n in names),3,realm)
 def test_manifest_reports_neighborhood_finishes_without_changing_roster(self):
  import tempfile
  with tempfile.TemporaryDirectory() as td:
   manifest=builder.build_all(ROOT,Path(td))
   self.assertEqual(manifest["asset_count"],50)
   for a in manifest["assets"]:
    if a["category"]=="City":
     self.assertEqual(a["textured_roofs"],20)
     self.assertEqual(a["sculpted_districts"],4)
    elif a["category"]=="Environment":
     self.assertEqual(a["textured_roofs"],12)
     self.assertEqual(a["sculpted_districts"],3)
    else:
     self.assertEqual(a["textured_roofs"],0)
     self.assertEqual(a["sculpted_districts"],0)
   self.assertEqual(manifest,builder.build_all(ROOT,Path(td),True))
