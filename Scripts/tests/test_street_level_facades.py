"""Regressions for visible door/window recesses instead of opaque flat wall overlays."""
import importlib.util
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
if all(importlib.util.find_spec(m) for m in ("numpy","trimesh","PIL")):
 spec=importlib.util.spec_from_file_location("street_level_builder",ROOT/"Scripts/build_hero_models.py")
 builder=importlib.util.module_from_spec(spec)
 spec.loader.exec_module(builder)
else:builder=None

@unittest.skipUnless(builder is not None,"3D generator dependencies not installed")
class StreetLevelFacades(unittest.TestCase):
 def test_façade_cutouts_clear_actual_door_and_window_centers(self):
  import numpy as np
  surface=ROOT/"Scripts/district_surface_detail.py"
  spec=importlib.util.spec_from_file_location("street_surface_checks",surface)
  module=importlib.util.module_from_spec(spec)
  spec.loader.exec_module(module)
  for w,h in ((2.25,2.3),(2.73,3.38)):
   for side in (-1,1):
    mesh=module.apertured_facade_mesh(x=0,y=0,z=1.20,w=w,h=h,door_side=side)
    self.assertGreaterEqual(len(mesh.faces),18)
    center=mesh.triangles_center
    self.assertTrue(np.allclose(center[:,2],1.2))
    door_x=side*.4
    self.assertFalse(((abs(center[:,0]-door_x)<.27)&(center[:,1]>.10)&(center[:,1]<1.47)).any(),
                     "Face covers the existing door aperture")
    for x in (-w*.33,w*.33):
     self.assertFalse(((abs(center[:,0]-x)<.13)&(abs(center[:,1]-h*.58)<.16)).any(),
                      "Face covers a glowing window aperture")
    self.assertLessEqual(float(mesh.bounds[1][0]),w*.55)
    self.assertGreaterEqual(float(mesh.bounds[0][0]),-w*.55)

 def test_every_dwelling_has_physically_recessed_portal_and_window_glazing(self):
  from trimesh.visual.texture import TextureVisuals
  for city in builder.CITY_SPECS:
   scene=builder.build_city_environment(city).scene
   names=list(scene.geometry)
   counts={
     "apertured_facade":sum("_district_pbr_apertured_facade_" in n for n in names),
     "recessed_glazing":sum("_district_pbr_recessed_glazing_" in n for n in names),
     "entrance_casing":sum("_district_pbr_entrance_casing_" in n for n in names),
     "public_threshold":sum("_district_pbr_public_threshold_" in n for n in names)
   }
   self.assertEqual(counts,{key:20 for key in counts},city)
   for key,g in scene.geometry.items():
    if "_district_pbr_apertured_facade_" in key:
     self.assertIsInstance(g.visual,TextureVisuals,key)
     self.assertGreaterEqual(len(g.faces),18)
   self.assertEqual(sum("_habitation_shell_" in n for n in names),20,city)

 def test_regional_overviews_keep_12_façades_and_unique_street_layouts(self):
  for realm in builder.REALMS:
   scene=builder.build_environment(realm).scene
   names=list(scene.geometry)
   for tag in ("_district_pbr_apertured_facade_","_district_pbr_recessed_glazing_",
               "_district_pbr_entrance_casing_","_district_pbr_public_threshold_"):
    self.assertEqual(sum(tag in n for n in names),12,(realm,tag))
   self.assertEqual(sum("_habitation_shell_" in n for n in names),12,realm)
   self.assertTrue(any("neighborhood_route_" in n for n in names),realm)
