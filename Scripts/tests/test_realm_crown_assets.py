"""Nine unmistakably different landmarks, separate from quest/collision authority."""
from __future__ import annotations
import importlib.util
import hashlib
import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

ROOT=Path(__file__).resolve().parents[2]
def load(path, name):
    spec=importlib.util.spec_from_file_location(name,ROOT/path)
    module=importlib.util.module_from_spec(spec)
    sys.modules[name]=module
    spec.loader.exec_module(module)
    return module

G=load("Scripts/build_realm_geometry.py","unmade_crown_base_geometry")
C=load("Scripts/build_realm_crowns.py","unmade_distinctive_realm_crowns")
S=load("Scripts/unreal_editor/import_realm_crowns.py","unmade_distinctive_crown_importer")
F=load("Scripts/tests/test_unreal_prototype_import.py","unmade_test_mock_editor")

class RealmCrownTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.manifest,cls.files=C.build_crowns(ROOT)
    def test_nine_unique_realms_motifs_art_and_canonical_placements(self):
        self.assertEqual(self.manifest["asset_count"],9)
        self.assertEqual(len(self.files),9)
        self.assertEqual([r["realm"] for r in self.manifest["assets"]],list(G.REALMS))
        self.assertEqual(len({r["motif"] for r in self.manifest["assets"]}),9)
        self.assertEqual(len({r["visual_family"] for r in self.manifest["assets"]}),9)
        self.assertEqual(len({r["city_id"] for r in self.manifest["assets"]}),9)
        self.assertEqual(len({r["stable_id"] for r in self.manifest["assets"]}),9)
        self.assertEqual(len({r["ue_position_cm"][0] for r in self.manifest["assets"]})>=5,True)
        self.assertTrue(all(r["status"]=="SOURCE_MESH_ENGINE_UNVERIFIED" for r in self.manifest["assets"]))
        self.assertTrue(all(r["collision"]=="none" and not r["quest_authority"]
                            for r in self.manifest["assets"]))
        self.assertEqual(len({hashlib.sha256(data).hexdigest() for data in self.files.values()}),9)
        self.assertGreater(min(r["triangles"] for r in self.manifest["assets"]),30)
        for row in self.manifest["assets"]:
            self.assertTrue(G.validate_model(self.files[row["file"]]))
            self.assertEqual(hashlib.sha256(self.files[row["file"]]).hexdigest(),row["sha256"])
            document=json.loads(self.files[row["file"]])
            self.assertEqual(len(document["materials"]),3)
            self.assertGreater(document["buffers"][0]["byteLength"],100)
        self.assertNotIn("SaveGameToSlot",(ROOT/"Scripts/build_realm_crowns.py").read_text())

    def test_deterministic_output_cli_roundtrip_and_byte_mismatch(self):
        second,files=C.build_crowns(ROOT)
        self.assertEqual(second,self.manifest)
        self.assertEqual(files,self.files)
        with tempfile.TemporaryDirectory() as td:
            out=Path(td)/"crowns"
            run=subprocess.run([sys.executable,str(ROOT/"Scripts/build_realm_crowns.py"),
                                "--out-dir",str(out)],capture_output=True,text=True,timeout=30)
            self.assertEqual(run.returncode,0,run.stderr)
            self.assertEqual(len(list(out.glob("*.gltf"))),9)
            check=subprocess.run([sys.executable,str(ROOT/"Scripts/build_realm_crowns.py"),
                                  "--out-dir",str(out),"--verify-output"],
                                 capture_output=True,text=True,timeout=30)
            self.assertEqual(check.returncode,0,check.stderr)
            crown=next(out.glob("*.gltf"))
            crown.write_bytes(crown.read_bytes()+b"broken")
            bad=subprocess.run([sys.executable,str(ROOT/"Scripts/build_realm_crowns.py"),
                                "--out-dir",str(out),"--verify-output"],
                               capture_output=True,text=True,timeout=30)
            self.assertNotEqual(bad.returncode,0)

    def test_real_editor_adapter_dry_run_fail_closed_import_and_stage(self):
        with tempfile.TemporaryDirectory() as td:
            folder=Path(td)/"crown";folder.mkdir()
            for name,data in self.files.items():(folder/name).write_bytes(data)
            (folder/"manifest.json").write_text(json.dumps(self.manifest))
            plan=S.prepare_crown_plan(folder)
            self.assertEqual(len(plan["entries"]),9)
            self.assertEqual(len(plan["preview"]),9)
            self.assertTrue(all(x["destination"].startswith(S.IMPORT_ROOT+"/")
                                for x in plan["entries"]))
            fake=F.FakeUnreal()
            with patch.dict(os.environ,{"UNMADE_PROTO_IMPORT_APPROVED":"0"}):
                with self.assertRaisesRegex(RuntimeError,"approv"):
                    S.import_and_stage_crowns(fake,plan)
            self.assertEqual(fake.tools.tasks,[])
            with patch.dict(os.environ,{"UNMADE_PROTO_IMPORT_APPROVED":"1"}):
                first=S.import_and_stage_crowns(fake,plan)
                self.assertEqual(first["imports"]["created"],9)
                self.assertEqual(first["preview"]["created"],9)
                self.assertEqual(len(fake.EditorLevelLibrary.actors),9)
                self.assertTrue(all(a.component.collision=="NO_COLLISION"
                                    for a in fake.EditorLevelLibrary.actors))
                again=S.import_and_stage_crowns(fake,plan)
                self.assertEqual(again["imports"]["created"],0)
                self.assertEqual(again["imports"]["existing"],9)
                self.assertEqual(again["preview"]["created"],0)
                self.assertEqual(again["preview"]["existing"],9)
                self.assertEqual(len(fake.EditorLevelLibrary.actors),9)
                self.assertTrue(all(not t["replace_existing"] for t in fake.tools.tasks))
            first_file=next(folder.glob("*.gltf"))
            first_file.write_bytes(first_file.read_bytes()+b"tamper")
            with self.assertRaisesRegex(ValueError,"hash|SHA|mismatch"):
                S.prepare_crown_plan(folder)

    def test_first_pc_pipeline_and_source_ci_are_connected(self):
        quick=(ROOT/"Scripts/first_pc_quickstart.ps1").read_text()
        ci=(ROOT/".github/workflows/static-checks.yml").read_text()
        for token in ("build_realm_crowns.py","import_realm_crowns.py",
                      "realm-crown-import-receipt.json","CROWN_EDITOR_IMPORT_INCOMPLETE"):
            self.assertIn(token,quick)
        self.assertIn("build_realm_crowns.py --check",ci)
        self.assertIn("UNMADE_PROTO_IMPORT_APPROVED",quick)
        self.assertIn("UNMADE_PROTO_IMPORT_RECEIPT",quick)

if __name__=="__main__":
    unittest.main()
