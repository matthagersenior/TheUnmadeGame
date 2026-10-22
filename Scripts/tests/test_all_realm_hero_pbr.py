"""All nine realm environments and eleven settlements require unique UV/PBR hero landmarks."""
import importlib.util
import io
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
if all(importlib.util.find_spec(x) for x in ("numpy","trimesh","PIL")):
    spec=importlib.util.spec_from_file_location("hero_all_realm_pbr",ROOT/"Scripts/build_hero_models.py")
    builder=importlib.util.module_from_spec(spec);spec.loader.exec_module(builder)
else:builder=None

@unittest.skipUnless(builder is not None,"3D libraries installed in dedicated workflow")
class UniqueRealmPBR(unittest.TestCase):
    def test_all_cities_and_realms_have_specific_structural_pbr_landmarks(self):
        from trimesh.visual.texture import TextureVisuals
        from trimesh.visual.material import PBRMaterial
        seen={}
        for name in builder.CITY_SPECS:
            scene=builder.build_city_environment(name).scene
            heroes=[(key,m) for key,m in scene.geometry.items() if "_hero_pbr_" in key]
            self.assertGreaterEqual(len(heroes),8,name)
            palette=set()
            for key,mesh in heroes:
                self.assertIsInstance(mesh.visual,TextureVisuals,key)
                self.assertIsInstance(mesh.visual.material,PBRMaterial,key)
                self.assertEqual(len(mesh.visual.uv),len(mesh.vertices),key)
                self.assertIsNotNone(mesh.visual.material.baseColorTexture,key)
                self.assertIsNotNone(mesh.visual.material.normalTexture,key)
                self.assertTrue((mesh.visual.uv>=0).all() and (mesh.visual.uv<=1).all(),key)
                palette.add(mesh.visual.material.name)
            self.assertGreaterEqual(len(palette),2,name)
            self.assertTrue(all(name in p for p in palette),name)
            seen[name]=frozenset(palette)
        self.assertEqual(len(set(seen.values())),11)
        for realm,representative in builder.REGION_HABITAT.items():
            scene=builder.build_environment(realm).scene
            heroes=[k for k in scene.geometry if "_hero_pbr_" in k]
            self.assertGreaterEqual(len(heroes),8,realm)
            self.assertTrue(any(representative.lower() in k.lower() for k in heroes),realm)

    def test_landmarks_have_different_real_lore_shapes(self):
        required={
            "Crossings":"survey_fork",
            "Paperhaven":"archive_terrace",
            "Saltwake":"dry_hull",
            "Cinderhold":"thermal_kiln",
            "Drevlach":"tide_balance",
            "Orravane":"choir_span",
            "Vathless":"quarry_cascade",
            "Eillun":"voluntary_threshold",
            "Tharniv":"grafted_court",
            "Auvren":"founding_portal",
            "Bellwold":"empty_yoke",
        }
        for city,label in required.items():
            geometries=builder.build_city_environment(city).scene.geometry
            self.assertTrue(any(label in k for k in geometries if "_hero_pbr_" in k),(city,label))

    def test_50_asset_export_reports_provenance_and_camera_focus(self):
        import tempfile
        from pathlib import Path
        with tempfile.TemporaryDirectory() as tmp:
            ledger=builder.build_all(ROOT,Path(tmp))
            self.assertEqual(ledger["asset_count"],50)
            for a in ledger["assets"]:
                if a["category"] in ("City","Environment"):
                    self.assertGreaterEqual(len(a["pbr_materials"]),2,a["stable_id"])
                    self.assertEqual(len(a["hero_focus"]),3,a["stable_id"])
                    self.assertTrue(a["hero_label"],a["stable_id"])
                else:
                    self.assertEqual(a["hero_focus"],[])
                    self.assertFalse(a["hero_label"])
            self.assertEqual(ledger,builder.build_all(ROOT,Path(tmp),True))

    def test_site_contains_hero_focus_control(self):
        html=(ROOT/"site/real-models.html").read_text()
        self.assertIn("hero_focus",html)
        self.assertIn("hero_label",html)
        self.assertIn("Hero landmark",html)
