"""Offline geometry production: real glTF data, canon links, deterministic output.

These tests never claim Unreal import success, a walkable level or collision.
"""
from __future__ import annotations

import copy
import hashlib
import importlib.util
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
PATH=ROOT/"Scripts/build_realm_geometry.py"
spec=importlib.util.spec_from_file_location("unmade_realm_geometry",PATH)
GEOM=importlib.util.module_from_spec(spec)
spec.loader.exec_module(GEOM)


class RealmGeometryProductionTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.manifest,cls.files=GEOM.build(ROOT)

    def test_complete_unique_city_module_and_physical_clue_coverage(self):
        m=self.manifest
        self.assertEqual(m["asset_count"],115)
        self.assertEqual(m["city_module_count"],88)
        self.assertEqual(m["clue_count"],27)
        self.assertEqual(len(self.files),115)
        self.assertEqual(len({e["stable_id"] for e in m["assets"]}),115)
        self.assertEqual(set(x["realm"] for x in m["assets"]),set(GEOM.REALMS))
        for entry in m["assets"]:
            self.assertIn(entry["file"],self.files)
            self.assertEqual(hashlib.sha256(self.files[entry["file"]]).hexdigest(),
                             entry["sha256"])
            self.assertGreater(entry["triangles"],30)
            self.assertEqual(entry["status"],"SOURCE_MESH_ENGINE_UNVERIFIED")
            if entry["kind"]=="physical_clue":
                self.assertIsNone(entry["ue_position_cm"])
                self.assertTrue(entry["interaction"])
                self.assertTrue(entry["discovery"])
            else:
                self.assertEqual(len(entry["ue_position_cm"]),3)

    def test_each_generated_gltf_has_valid_triangle_buffers(self):
        for filename,data in self.files.items():
            with self.subTest(file=filename):
                self.assertTrue(GEOM.validate_model(data))
                doc=json.loads(data)
                self.assertEqual(doc["asset"]["version"],"2.0")
                self.assertEqual(len(doc["scenes"]),1)
                self.assertTrue(doc["meshes"][0]["primitives"])
                self.assertEqual(len(doc["materials"]),3)

    def test_distinct_regions_have_distinct_pbr_materials_and_forms(self):
        by_realm={}
        for row in self.manifest["assets"]:
            doc=json.loads(self.files[row["file"]])
            signature=(tuple(doc["materials"][0]["pbrMetallicRoughness"]
                             ["baseColorFactor"]),row["triangles"])
            by_realm.setdefault(row["realm"],set()).add(signature)
        self.assertEqual(len(by_realm),9)
        self.assertEqual(len({next(iter(s))[0] for s in by_realm.values()}),9)
        self.assertTrue(all(len(v)>2 for v in by_realm.values()))

    def test_repeated_generation_byte_exact(self):
        a,files=GEOM.build(ROOT)
        self.assertEqual(a,self.manifest)
        self.assertEqual(files,self.files)

    def test_output_roundtrip_and_tamper_detection(self):
        with tempfile.TemporaryDirectory() as temp:
            target=Path(temp)/"geometry"
            command=[sys.executable,str(PATH),"--out-dir",str(target)]
            proc=subprocess.run(command,capture_output=True,text=True,timeout=30)
            self.assertEqual(proc.returncode,0,proc.stderr)
            self.assertTrue((target/"manifest.json").is_file())
            self.assertEqual(len(list(target.glob("*.gltf"))),115)
            proc=subprocess.run(command+["--verify-output"],capture_output=True,
                                text=True,timeout=30)
            self.assertEqual(proc.returncode,0,proc.stderr)
            first=next(target.glob("*.gltf"))
            first.write_bytes(first.read_bytes()+b"tamper")
            proc=subprocess.run(command+["--verify-output"],capture_output=True,
                                text=True,timeout=30)
            self.assertNotEqual(proc.returncode,0)

    def test_reject_invalid_module_and_source_order(self):
        with self.assertRaisesRegex(ValueError,"Unsupported city module"):
            GEOM.module_geometry("weaponized_claim", "braided_roads", [1,1,2])
        with self.assertRaisesRegex(ValueError,"Degenerate"):
            mesh=GEOM.Mesh()
            mesh.face([(0,0,0),(1,0,0),(2,0,0)])

    def test_legacy_game_save_and_quests_not_touched(self):
        # No Source/ or saved state dependencies: these are isolated model assets.
        source=(ROOT/"Scripts/build_realm_geometry.py").read_text()
        self.assertNotIn("SaveGameToSlot",source)
        self.assertNotIn("import unreal",source)
        self.assertNotIn("save_current_level",source)
        self.assertIn("SOURCE_MESH_ENGINE_UNVERIFIED",source)


if __name__=="__main__":
    unittest.main()
