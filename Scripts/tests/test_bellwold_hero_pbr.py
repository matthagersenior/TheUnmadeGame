"""UV + embedded PBR assets for Bellwold's unique stone refuge and empty bell yoke."""
import importlib.util
import io
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
spec=importlib.util.spec_from_file_location("unmade_pbr_builder",ROOT/"Scripts/build_hero_models.py")
if all(importlib.util.find_spec(m) for m in ("numpy","trimesh","PIL")):
 builder=importlib.util.module_from_spec(spec)
 spec.loader.exec_module(builder)
else:builder=None

@unittest.skipUnless(builder is not None,"UV/PBR mesh dependencies not installed")
class BellwoldHeroPBRTests(unittest.TestCase):
 def test_city_has_own_embedded_usable_pbr_textures_and_real_uvs(self):
  from trimesh.visual.texture import TextureVisuals
  from trimesh.visual.material import PBRMaterial
  city=builder.build_city_environment("Bellwold")
  candidates={name:geo for name,geo in city.scene.geometry.items() if "_hero_pbr_" in name}
  self.assertGreaterEqual(len(candidates),18,"Detailed, locally built hero structure missing")
  materials={}
  for name,mesh in candidates.items():
   self.assertIsInstance(mesh.visual,TextureVisuals,name)
   self.assertEqual(len(mesh.visual.uv),len(mesh.vertices),name)
   self.assertTrue((mesh.visual.uv>=0).all() and (mesh.visual.uv<=1).all(),name)
   material=mesh.visual.material
   self.assertIsInstance(material,PBRMaterial,name)
   self.assertIsNotNone(material.baseColorTexture,name)
   self.assertIsNotNone(material.normalTexture,name)
   self.assertGreaterEqual(material.baseColorTexture.width,128)
   self.assertGreaterEqual(material.baseColorTexture.height,128)
   materials[material.name]=material
  self.assertTrue({"Bellwold Weathered Stone","Bellwold Salvaged Timber","Bellwold Verdigris Copper"}.issubset(materials.keys()))
  self.assertGreaterEqual(materials["Bellwold Verdigris Copper"].metallicFactor,.65)
  self.assertLessEqual(materials["Bellwold Weathered Stone"].metallicFactor,.1)

 def test_true_bell_absence_and_survival_of_glb_export(self):
  import trimesh
  from trimesh.visual.texture import TextureVisuals
  city=builder.build_city_environment("Bellwold")
  names=list(city.scene.geometry)
  for token in ("hero_pbr_refuge_stone","hero_pbr_timber","hero_pbr_copper","hero_pbr_arch","hero_pbr_empty_yoke"):
   self.assertTrue(any(token in name for name in names),token)
  self.assertFalse(any("cast_bell_body" in name for name in names))
  payload=city.scene.export(file_type="glb")
  self.assertGreater(len(payload),10000)
  read=trimesh.load(io.BytesIO(payload),file_type="glb",force="scene")
  textured=[g for g in read.geometry.values() if isinstance(g.visual,TextureVisuals) and getattr(g.visual.material,"baseColorTexture",None) is not None]
  self.assertGreaterEqual(len(textured),18)
  self.assertTrue(any(g.visual.material.normalTexture is not None for g in textured))

 def test_gallery_exposes_lighting_presets_and_bell_yoke_focus(self):
  text=(ROOT/"site/real-models.html").read_text(encoding="utf-8")
  self.assertIn('id="lighting"',text)
  self.assertIn("applyLighting(",text)
  self.assertIn("Empty bell yoke",text)
  self.assertIn('id="pbr"',text)

 def test_bellwold_keeps_its_own_material_and_hero_identities(self):
  city=builder.build_city_environment("Bellwold")
  names=list(city.scene.geometry)
  self.assertTrue(any("empty_yoke_" in n for n in names))
  self.assertTrue(any("hero_pbr_refuge_stone_" in n for n in names))
  self.assertFalse(any("hero_pbr_saltwake_" in n for n in names))
