"""Make realm terrain patch borders smoothly tapered, UV-mapped sculpted ground."""
import importlib.util
import unittest
from pathlib import Path
try:
 import numpy as np
except ImportError:
 np=None

ROOT=Path(__file__).resolve().parents[2]
spec=importlib.util.spec_from_file_location("terrain_edge_builder",ROOT/"Scripts/build_hero_models.py")
builder=importlib.util.module_from_spec(spec)
spec.loader.exec_module(builder)
surface=builder._district_surface()

class SculptedTerrainEdgeTests(unittest.TestCase):
 def test_each_district_has_separate_embedded_pbr_blend_geometry(self):
  from trimesh.visual.texture import TextureVisuals
  for city in builder.CITY_SPECS:
   model=builder.build_city_environment(city)
   items=[(n,m) for n,m in model.scene.geometry.items() if "_terrain_blend_apron_" in n]
   self.assertEqual(len(items),4,city)
   for name,mesh in items:
    self.assertGreaterEqual(len(mesh.faces),300,name)
    self.assertIsInstance(mesh.visual,TextureVisuals,name)
    self.assertIsNotNone(mesh.visual.material.normalTexture,name)
    self.assertGreater(float(mesh.extents[0]),14.0,name)
    self.assertGreater(float(mesh.extents[2]),14.0,name)
  for realm in builder.REALMS:
   model=builder.build_environment(realm)
   self.assertEqual(sum("_terrain_blend_apron_" in n for n in model.scene.geometry),3,realm)

 def test_edge_continuity_at_existing_ground_patch(self):
  # Inner skirt vertices must match the *actual exported terrain mesh edge*,
  # so no gap opens when the center relief rises/falls.
  for city in builder.CITY_SPECS:
   model=builder.Kit()
   floor=4.12 if city=="Orravane" else -.42
   cx,cz=3.0,-2.0
   surface.finish_district_terrain(model,city,1,cx,cz,floor)
   terrain=next(m for n,m in model.scene.geometry.items() if "_district_terrain_relief_" in n)
   apron=next(m for n,m in model.scene.geometry.items() if "_terrain_blend_apron_" in n)
   for vertex in terrain.vertices:
    x,y,z=(float(n) for n in vertex)
    if abs(abs(x-cx)-6.9)<1e-5 or abs(abs(z-cz)-6.9)<1e-5:
     d=np.linalg.norm(apron.vertices-np.asarray([x,y,z]),axis=1)
     self.assertLess(float(np.min(d)),1e-4,(city,x,z))

 def test_outside_edge_is_lower_with_non_rectangular_city_profile(self):
  shape_signatures=[]
  for city in builder.CITY_SPECS:
   m=builder.Kit()
   surface.finish_district_terrain(m,city,0,0,0,4.12 if city=="Orravane" else -.42)
   part=next(g for n,g in m.scene.geometry.items() if "_terrain_blend_apron_" in n)
   self.assertGreater(len(part.vertices),180)
   self.assertTrue(np.isfinite(part.vertices).all())
   # A visible roughened outer rim, unlike a single square plate.
   outer=part.vertices[np.maximum(abs(part.vertices[:,0]),abs(part.vertices[:,2]))>6.95]
   self.assertGreater(float(np.ptp(np.linalg.norm(outer[:,[0,2]],axis=1))),.4)
   shape_signatures.append((round(float(np.ptp(outer[:,0])),3),
                            round(float(np.ptp(outer[:,2])),3)))
  self.assertGreaterEqual(len(set(shape_signatures)),9)
