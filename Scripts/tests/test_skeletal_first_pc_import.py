"""Verify rig handoff and Unreal acceptance boundary without pretending Editor ran."""
from __future__ import annotations
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT=Path(__file__).resolve().parents[2]
def load(path,label):
    spec=importlib.util.spec_from_file_location(label,ROOT/path)
    module=importlib.util.module_from_spec(spec)
    sys.modules[label]=module
    spec.loader.exec_module(module)
    return module

K=load("Scripts/build_skeletal_performance_kit.py","unmade_generated_character_rigs")
S=load("Scripts/unreal_editor/import_skeletal_performance.py","unmade_skeletal_interchange_boundary")

class FakeSkeleton: pass
class FakeSkeletalMesh:
    def __init__(self,skeleton):self.skeleton=skeleton
    def get_editor_property(self,field):return getattr(self,field)
class FakeAnimSequence:
    def __init__(self,skeleton):self.skeleton=skeleton
    def get_editor_property(self,field):return getattr(self,field)
class FakeTask:
    def __init__(self):self.props={}
    def set_editor_property(self,field,value):self.props[field]=value
class FakeLibrary:
    def __init__(self):self.assets={};self.folders=set()
    def load_asset(self,path):return self.assets.get(path)
    def list_assets(self,folder,recursive=False):
        return [p for p in self.assets if p.startswith(folder+"/")]
    def does_directory_exist(self,folder):return folder in self.folders
    def make_directory(self,folder):self.folders.add(folder);return True
class FakeTools:
    def __init__(self,lib):self.lib=lib;self.tasks=[]
    def import_asset_tasks(self,tasks):
        for task in tasks:
            props=dict(task.props);self.tasks.append(props)
            if props["replace_existing"]:raise AssertionError("Attempted replacement")
            doc=json.loads(Path(props["filename"]).read_text())
            folder=props["destination_path"]
            skeleton=FakeSkeleton()
            self.lib.assets[folder+"/"+props["destination_name"]]=FakeSkeletalMesh(skeleton)
            for animation in doc["animations"]:
                self.lib.assets[folder+"/"+animation["name"]]=FakeAnimSequence(skeleton)
class FakeUnreal:
    SkeletalMesh=FakeSkeletalMesh
    AnimSequence=FakeAnimSequence
    AssetImportTask=FakeTask
    def __init__(self):
        self.EditorAssetLibrary=FakeLibrary()
        self.tools=FakeTools(self.EditorAssetLibrary)
        tools=self.tools
        self.AssetToolsHelpers=type("Helpers",(),{"get_asset_tools":lambda self:tools})()

class SkeletalFirstPcImportTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=tempfile.TemporaryDirectory()
        cls.path=Path(cls.temp.name)/"rigs"
        call=subprocess.run([sys.executable,str(ROOT/"Scripts/build_skeletal_performance_kit.py"),
                             "--out",str(cls.path)],capture_output=True,text=True,timeout=30)
        if call.returncode:raise RuntimeError(call.stderr)
    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()

    def test_three_real_skins_and_57_expected_runtime_animation_paths(self):
        plan=S.prepare_plan(self.path)
        self.assertEqual(plan["mesh_count"],3)
        self.assertEqual(plan["animation_count"],57)
        self.assertEqual(len(plan["source_sha256"]),4)
        self.assertFalse(plan["material_atlas_imported"])
        self.assertEqual([r["kind"] for r in plan["rigs"]],list(S.KINDS))
        for rig in plan["rigs"]:
            self.assertEqual(len(rig["animation_assets"]),19)
            self.assertEqual(len(set(rig["animation_assets"])),19)
            self.assertTrue(rig["mesh_asset"].startswith(S.CONTENT_ROOT+"/"))
            self.assertTrue(all(x.startswith(rig["destination"]+"/AN_Unmade")
                                for x in rig["animation_assets"]))
            self.assertEqual(rig["file"].suffix,".gltf")
            self.assertTrue(rig["file"].is_file())

    def test_guarded_real_import_and_idempotence_without_overwrites(self):
        plan=S.prepare_plan(self.path)
        editor=FakeUnreal()
        with patch.dict(os.environ,{"UNMADE_SKIN_IMPORT_APPROVED":"0"}):
            with self.assertRaisesRegex(RuntimeError,"approv"):
                S.import_skeletal_assets(editor,plan)
        self.assertEqual(len(editor.tools.tasks),0)
        with patch.dict(os.environ,{"UNMADE_SKIN_IMPORT_APPROVED":"1"}):
            first=S.import_skeletal_assets(editor,plan)
            self.assertEqual(first["meshes"]["created"],3)
            self.assertEqual(first["animations"]["total"],57)
            self.assertEqual(len(editor.tools.tasks),3)
            self.assertTrue(all(x["automated"] and x["save"] and not x["replace_existing"]
                                for x in editor.tools.tasks))
            second=S.import_skeletal_assets(editor,plan)
            self.assertEqual(second["meshes"]["created"],0)
            self.assertEqual(second["meshes"]["existing"],3)
            self.assertEqual(len(editor.tools.tasks),3)

    def test_partial_and_wrong_type_import_refused_before_mutation(self):
        plan=S.prepare_plan(self.path)
        editor=FakeUnreal()
        rig=plan["rigs"][0]
        editor.EditorAssetLibrary.assets[rig["mesh_asset"]]=FakeSkeletalMesh(FakeSkeleton())
        with patch.dict(os.environ,{"UNMADE_SKIN_IMPORT_APPROVED":"1"}):
            with self.assertRaisesRegex(RuntimeError,"Partial"):
                S.import_skeletal_assets(editor,plan)
        self.assertEqual(editor.tools.tasks,[])
        editor.EditorAssetLibrary.assets.clear()
        for path in [rig["mesh_asset"],*rig["animation_assets"]]:
            editor.EditorAssetLibrary.assets[path]=object()
        with patch.dict(os.environ,{"UNMADE_SKIN_IMPORT_APPROVED":"1"}):
            with self.assertRaisesRegex(RuntimeError,"Wrong type"):
                S.import_skeletal_assets(editor,plan)
        self.assertEqual(editor.tools.tasks,[])

    def test_skeleton_mismatch_rejected(self):
        plan=S.prepare_plan(self.path)
        editor=FakeUnreal()
        with patch.dict(os.environ,{"UNMADE_SKIN_IMPORT_APPROVED":"1"}):
            S.import_skeletal_assets(editor,plan)
            rig=plan["rigs"][1]
            editor.EditorAssetLibrary.assets[rig["animation_assets"][0]].skeleton=FakeSkeleton()
            with self.assertRaisesRegex(RuntimeError,"skeleton mismatch"):
                S.import_skeletal_assets(editor,plan)
        self.assertEqual(len(editor.tools.tasks),3)

    def test_source_tamper_rejected_before_editor_or_receipt(self):
        path=self.path/"SK_UnmadeResident.gltf"
        before=path.read_bytes()
        try:
            path.write_bytes(before+b"tampered")
            with self.assertRaisesRegex(ValueError,"mismatch"):
                S.prepare_plan(self.path)
        finally:path.write_bytes(before)

    def test_real_unreal_launcher_and_source_ci_gate(self):
        launcher=(ROOT/"Scripts/first_pc_quickstart.ps1").read_text()
        workflow=(ROOT/".github/workflows/static-checks.yml").read_text()
        for term in ("import_skeletal_performance.py",
                     "UNMADE_SKIN_IMPORT_APPROVED",
                     "UNMADE_SKIN_IMPORT_RECEIPT",
                     "skeletal-import-receipt.json",
                     "SKELETAL_ASSETS_IMPORT_INCOMPLETE"):
            self.assertIn(term,launcher)
        self.assertIn("import_skeletal_performance.py",workflow)
        self.assertIn("build_skeletal_performance_kit.py",workflow)
        source=(ROOT/"Scripts/unreal_editor/import_skeletal_performance.py").read_text()
        self.assertIn('if os.environ.get("UNMADE_SKIN_IMPORT_APPROVED")!="1"',source)
        self.assertIn('import unreal',source)
        self.assertNotIn("SaveGameToSlot",source)

if __name__=="__main__":
    unittest.main()
