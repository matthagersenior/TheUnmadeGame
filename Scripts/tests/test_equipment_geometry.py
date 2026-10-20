"""Test source-linked original item meshes without Unreal or item grants."""
from __future__ import annotations
import importlib.util
import json
import pathlib
import hashlib
import subprocess
import sys
import tempfile
import unittest
ROOT=pathlib.Path(__file__).resolve().parents[2]
SCRIPT=ROOT/"Scripts/build_equipment_geometry.py"
spec=importlib.util.spec_from_file_location("unmade_equipment_geometry",SCRIPT)
E=importlib.util.module_from_spec(spec)
spec.loader.exec_module(E)

class EquipmentModels(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.manifest,cls.assets=E.make_assets(ROOT)

    def test_all_original_items_have_independent_mesh(self):
        m=self.manifest
        self.assertEqual(m["asset_count"],78)
        self.assertEqual(m["main_catalog"],60)
        self.assertEqual(m["regional_gear"],18)
        self.assertEqual(len(self.assets),78)
        self.assertEqual(len({x["id"] for x in m["records"]}),78)
        self.assertEqual(len({x["file"] for x in m["records"]}),78)
        self.assertEqual({x["kind"] for x in m["records"]},
                         {"Weapon","Armor","Charm","Consumable","Material"})
        for x in m["records"]:
            self.assertIn(x["file"],self.assets)
            self.assertGreater(x["triangles"],40)
            self.assertEqual(hashlib.sha256(self.assets[x["file"]]).hexdigest(),x["sha256"])
            self.assertEqual(x["state"],"SOURCE_PREVIEW_ONLY_NOT_UNREAL_IMPORTED_OR_EQUIPPED")
            self.assertTrue(E.G.validate_model(self.assets[x["file"]]))

    def test_source_remains_sole_acquisition_authority(self):
        for record in self.manifest["records"]:
            if record["group"]=="item_catalog":
                self.assertEqual(record["source"],E.LEDGER)
                self.assertIn(record["availability"],("earnable","reserved","planned","definition_only","source_only"))
            else:
                self.assertIn(record["realm"],E.G.REALMS)
                self.assertTrue(record["exclusive_with"])
                self.assertTrue(record["acquisition"])

    def test_deterministic_models_and_manifest(self):
        m,files=E.make_assets(ROOT)
        self.assertEqual(m,self.manifest)
        self.assertEqual(files,self.assets)

    def test_output_can_be_regenerated_and_checked(self):
        with tempfile.TemporaryDirectory() as tmp:
            target=pathlib.Path(tmp)/"models"
            cmd=[sys.executable,str(SCRIPT),"--out-dir",str(target)]
            p=subprocess.run(cmd,capture_output=True,text=True,timeout=30)
            self.assertEqual(p.returncode,0,p.stderr)
            self.assertEqual(len(list(target.glob("*.gltf"))),78)
            p=subprocess.run(cmd+["--verify-output"],capture_output=True,
                             text=True,timeout=30)
            self.assertEqual(p.returncode,0,p.stderr)
            (target/"manifest.json").write_text("{}",encoding="utf-8")
            p=subprocess.run(cmd+["--verify-output"],capture_output=True,
                             text=True,timeout=30)
            self.assertNotEqual(p.returncode,0)

    def test_no_unlock_or_save_side_effects(self):
        source=SCRIPT.read_text()
        self.assertNotIn("SaveGameToSlot",source)
        self.assertNotIn("import unreal",source)
        self.assertNotIn("award_item(",source)

if __name__=="__main__":
    unittest.main()
