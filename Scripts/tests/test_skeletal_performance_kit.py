"""Check low-poly rig and animation outputs before any Unreal import."""
from __future__ import annotations
import importlib.util
import pathlib
import json
import base64
import struct
import subprocess
import sys
import tempfile
import unittest

ROOT=pathlib.Path(__file__).resolve().parents[2]
SCRIPT=ROOT/"Scripts/build_skeletal_performance_kit.py"
spec=importlib.util.spec_from_file_location("unmade_skeletal_kit",SCRIPT)
K=importlib.util.module_from_spec(spec)
spec.loader.exec_module(K)

class SkeletalAssetKitTests(unittest.TestCase):
    def test_three_skinned_actor_types_with_nineteen_named_animation_clips(self):
        self.assertEqual(set(K.KINDS),{"Resident","Beast","Wraith"})
        for kind in K.KINDS:
            with self.subTest(kind=kind):
                doc=K.make_actor(kind)
                self.assertEqual(doc["asset"]["version"],"2.0")
                self.assertEqual(len(doc["skins"]),1)
                self.assertGreaterEqual(len(doc["skins"][0]["joints"]),4)
                self.assertEqual(len(doc["animations"]),19)
                self.assertEqual(len({a["name"] for a in doc["animations"]}),19)
                attrs=doc["meshes"][0]["primitives"][0]["attributes"]
                for attr in ("POSITION","NORMAL","JOINTS_0","WEIGHTS_0"):
                    self.assertIn(attr,attrs)
                for animation in doc["animations"]:
                    self.assertGreater(len(animation["channels"]),0)
                    self.assertEqual(len(animation["channels"]),len(animation["samplers"]))
                    self.assertTrue(all(c["target"]["path"]=="rotation"
                                        for c in animation["channels"]))
                raw=base64.b64decode(doc["buffers"][0]["uri"].split(",",1)[1])
                self.assertEqual(len(raw),doc["buffers"][0]["byteLength"])
                self.assertTrue(all(v.get("byteOffset",0)+v["byteLength"]<=len(raw)
                                    for v in doc["bufferViews"]))

    def test_animation_targets_refer_to_real_bones(self):
        for kind in K.KINDS:
            doc=K.make_actor(kind)
            nodes=doc["nodes"]
            joint_ids=set(doc["skins"][0]["joints"])
            for animation in doc["animations"]:
                for channel in animation["channels"]:
                    self.assertIn(channel["target"]["node"],joint_ids)
                    self.assertLess(channel["sampler"],len(animation["samplers"]))
            scene_nodes=doc["scenes"][0]["nodes"]
            self.assertTrue(all(0<=x<len(nodes) for x in scene_nodes))

    def test_material_atlas_contains_six_original_pbr_definitions(self):
        doc=K.make_material_atlas()
        self.assertEqual(len(doc["materials"]),6)
        self.assertEqual(len(doc["meshes"][0]["primitives"]),6)
        self.assertEqual({x["name"] for x in doc["materials"]},set(K.MATERIALS))
        raw=base64.b64decode(doc["buffers"][0]["uri"].split(",",1)[1])
        self.assertEqual(len(raw),doc["buffers"][0]["byteLength"])

    def test_cli_exports_four_distinct_files_usable_by_import_pipeline(self):
        with tempfile.TemporaryDirectory() as tmp:
            output=pathlib.Path(tmp)/"rigs"
            proc=subprocess.run([sys.executable,str(SCRIPT),"--out",str(output)],
                                capture_output=True,text=True,timeout=30)
            self.assertEqual(proc.returncode,0,proc.stderr)
            self.assertEqual(len(list(output.glob("*.gltf"))),4)
            for item in output.glob("*.gltf"):
                result=K.validate(item)
                self.assertGreater(result["bytes"],1000)
            self.assertIn("AN_UnmadeResident_Idle",
                [x["name"] for x in json.loads((output/"SK_UnmadeResident.gltf").read_text())["animations"]])

    def test_no_unsupported_unreal_completion_claim(self):
        source=SCRIPT.read_text(encoding="utf-8")
        self.assertNotIn("import unreal",source)
        self.assertNotIn("SaveGameToSlot",source)

if __name__=="__main__":
    unittest.main()
