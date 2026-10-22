"""Art regression: every inhabited building visibly joins its realm-specific ground."""
import importlib.util
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
if all(importlib.util.find_spec(m) for m in ("numpy","trimesh","PIL")):
 spec=importlib.util.spec_from_file_location("grounded_hero",ROOT/"Scripts/build_hero_models.py")
 builder=importlib.util.module_from_spec(spec)
 spec.loader.exec_module(builder)
 surface=builder._district_surface()
else:builder=surface=None

@unittest.skipUnless(builder is not None,"Model libraries installed in GitHub 3D workflow")
class TerrainGroundedNeighborhoods(unittest.TestCase):
 def test_sculpted_ground_height_is_reused_in_foundation_placement(self):
  import numpy as np
  for city in builder.CITY_SPECS:
   floor=4.12 if city=="Orravane" else -.42
   shell=builder.Kit()
   surface.finish_district_terrain(shell,city,0,0,0,floor)
   name,mesh=next((n,m) for n,m in shell.scene.geometry.items() if "_district_terrain_relief_" in n)
   # Existing 15x15 relief contains real Y coordinates. Sampling matches the
   # source exactly at every grid vertex, not a different arbitrary road height.
   for vertex in mesh.vertices[::19]:
    expected=surface.terrain_surface_height(city,0,float(vertex[0]),float(vertex[2]),0,0,floor)
    self.assertAlmostEqual(float(vertex[1]),expected,places=5,msg=(city,name))

 def test_each_dwelling_has_terrain_anchored_supports_and_graded_entry_walkway(self):
  from trimesh.visual.texture import TextureVisuals
  for city in builder.CITY_SPECS:
   scene=builder.build_city_environment(city).scene
   for tag in ("_terrain_joinery_support_","_terrain_joinery_walkway_"):
    meshes=[(n,g) for n,g in scene.geometry.items() if tag in n]
    self.assertEqual(len(meshes),20,(city,tag))
    for name,geometry in meshes:
     self.assertIsInstance(geometry.visual,TextureVisuals,name)
     self.assertGreaterEqual(len(geometry.faces),24,name)
     self.assertGreaterEqual(float(geometry.extents[1]),.13,name)
   self.assertEqual(sum("_habitation_shell_" in n for n in scene.geometry),20)
  for realm in builder.REALMS:
   scene=builder.build_environment(realm).scene
   for tag in ("_terrain_joinery_support_","_terrain_joinery_walkway_"):
    self.assertEqual(sum(tag in n for n in scene.geometry),12,(realm,tag))
   self.assertEqual(sum("_habitation_shell_" in n for n in scene.geometry),12,realm)

 def test_city_specific_joinery_materials_are_not_shared_generic_props(self):
  cities=("Saltwake","Orravane","Vathless","Tharniv","Bellwold")
  signatures=[]
  for city in cities:
   scene=builder.build_city_environment(city).scene
   path=next(g for n,g in scene.geometry.items() if "_terrain_joinery_walkway_" in n)
   signatures.append((round(float(path.extents[0]),2),round(float(path.extents[1]),2),round(float(path.extents[2]),2)))
   mats={g.visual.material.name for n,g in scene.geometry.items() if "_terrain_joinery_support_" in n}
   self.assertTrue(all(name.startswith(city+" ") for name in mats),city)
  self.assertGreaterEqual(len(set(signatures)),4)
