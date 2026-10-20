"""One-click import contract, tested without claiming Unreal can be run here."""
from __future__ import annotations
import importlib.util
import json
import os
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

ROOT=Path(__file__).resolve().parents[2]
def load(relative, label):
    s=importlib.util.spec_from_file_location(label,ROOT/relative)
    mod=importlib.util.module_from_spec(s)
    s.loader.exec_module(mod)
    return mod

G=load("Scripts/build_realm_geometry.py","unmade_proto_scene_assets")
E=load("Scripts/build_equipment_geometry.py","unmade_proto_item_assets")
P=load("Scripts/unreal_editor/import_prototype_geometry.py","unmade_proto_importer")

class StaticMesh: pass
class FakeTask:
    def __init__(self):self.props={}
    def set_editor_property(self,key,value):self.props[key]=value
    def get_editor_property(self,key):return self.props.get(key,[])
class FakeLibrary:
    def __init__(self):
        self.assets={};self.directories=set()
    def does_asset_exist(self,path):return path in self.assets
    def does_directory_exist(self,path):return path in self.directories
    def make_directory(self,path):self.directories.add(path);return True
    def list_assets(self,path,recursive=False):
        return [p for p in self.assets if p.startswith(path+"/") and
                (recursive or "/" not in p[len(path)+1:])]
    def load_asset(self,path):return self.assets.get(path)
class FakeTools:
    def __init__(self,library):self.library=library;self.tasks=[]
    def import_asset_tasks(self,tasks):
        for task in tasks:
            props=task.props
            self.tasks.append(dict(props))
            path=props["destination_path"]+"/"+props["destination_name"]
            if props["replace_existing"] or path in self.library.assets:
                raise AssertionError("importer tried to replace existing content")
            self.library.assets[path]=StaticMesh()
            task.props["imported_object_paths"]=[path]
class FakeWorld:
    def __init__(self,name):self.name=name
    def get_name(self):return self.name
class FakeComp:
    def __init__(self):self.mesh=None;self.collision=None
    def set_static_mesh(self,mesh):self.mesh=mesh
    def set_collision_enabled(self,value):self.collision=value
class FakeActor:
    def __init__(self,position):self.position=position;self.label="";self.component=FakeComp()
    def set_actor_label(self,label):self.label=label
    def get_actor_label(self):return self.label
    def get_component_by_class(self,_):return self.component
class FakeLevels:
    def __init__(self):self.world=None;self.actors=[];self.saved=0
    def get_editor_world(self):return self.world
    def get_all_level_actors(self):return self.actors
    def spawn_actor_from_class(self,_,position):
        a=FakeActor(position);self.actors.append(a);return a
    def save_current_level(self):self.saved+=1;return True
class FakeLevelSubsystem:
    def __init__(self,levels):self.levels=levels;self.created=set()
    def load_level(self,path):
        if path not in self.created:return False
        self.levels.world=FakeWorld(path.rsplit("/",1)[-1]);return True
    def new_level(self,path):
        self.created.add(path);return self.load_level(path)
class FakeUnreal:
    StaticMesh=StaticMesh
    StaticMeshActor=type("StaticMeshActor",(),{})
    StaticMeshComponent=type("StaticMeshComponent",(),{})
    AssetImportTask=FakeTask
    class CollisionEnabled:
        NO_COLLISION="NO_COLLISION"
    def __init__(self):
        self.lib=FakeLibrary();self.tools=FakeTools(self.lib)
        self.EditorAssetLibrary=self.lib
        self.AssetToolsHelpers=type("Tools",(),{"get_asset_tools":lambda _, :self.tools})()
        self.EditorLevelLibrary=FakeLevels()
        self.subsystem=FakeLevelSubsystem(self.EditorLevelLibrary)
        self.LevelEditorSubsystem=type("LevelEditorSubsystem",(),{})
    def get_editor_subsystem(self,_):return self.subsystem
    def Vector(self,*xyz):return tuple(xyz)

class PrototypeGeometryImportTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=tempfile.TemporaryDirectory()
        root=Path(cls.temp.name)
        cls.world=root/"world";cls.equipment=root/"equipment"
        cls.world.mkdir();cls.equipment.mkdir()
        m,files=G.build(ROOT)
        for name,blob in files.items():(cls.world/name).write_bytes(blob)
        (cls.world/"manifest.json").write_text(json.dumps(m),encoding="utf-8")
        m,files=E.make_assets(ROOT)
        for name,blob in files.items():(cls.equipment/name).write_bytes(blob)
        (cls.equipment/"manifest.json").write_text(json.dumps(m),encoding="utf-8")
    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()

    def test_all_193_files_verified_and_preview_is_88_source_mapped_city_objects(self):
        plan=P.prepare_plan(self.world,self.equipment)
        self.assertEqual(len(plan["entries"]),193)
        self.assertEqual(len(plan["preview"]),88)
        self.assertEqual(len({x["id"] for x in plan["entries"]}),193)
        self.assertEqual(len({x["destination"] for x in plan["entries"]}),193)
        self.assertEqual({x["kind"] for x in plan["entries"]},{"world","equipment"})
        self.assertTrue(all(x["source_file"].is_file() for x in plan["entries"]))
        self.assertTrue(all(x["destination"].startswith(P.IMPORT_ROOT+"/")
                            for x in plan["entries"]))
        self.assertTrue(all(x["asset_key"] in {y["asset_key"] for y in plan["entries"]}
                            for x in plan["preview"]))
        self.assertEqual({x["realm"] for x in plan["preview"]},set(G.REALMS))
        self.assertTrue(all(x["label"].startswith("UM_PROTO_") for x in plan["preview"]))

    def test_failed_hash_blocks_entire_plan_before_import(self):
        item=next(self.world.glob("*.gltf"))
        old=item.read_bytes()
        try:
            item.write_bytes(old+b"tamper")
            with self.assertRaisesRegex(ValueError,"SHA|hash|mismatch"):
                P.prepare_plan(self.world,self.equipment)
        finally:item.write_bytes(old)

    def test_no_unapproved_import_and_no_mutations(self):
        plan=P.prepare_plan(self.world,self.equipment)
        unreal=FakeUnreal()
        with patch.dict(os.environ,{"UNMADE_PROTO_IMPORT_APPROVED":"0"}):
            with self.assertRaisesRegex(RuntimeError,"approv"):
                P.import_assets(unreal,plan)
        self.assertEqual(len(unreal.tools.tasks),0)
        self.assertEqual(unreal.lib.assets,{})

    def test_import_isolated_single_mesh_idempotent_and_non_overwriting(self):
        plan=P.prepare_plan(self.world,self.equipment)
        mini={"entries":plan["entries"][:2],"preview":[]}
        unreal=FakeUnreal()
        with patch.dict(os.environ,{"UNMADE_PROTO_IMPORT_APPROVED":"1"}):
            first=P.import_assets(unreal,mini)
            self.assertEqual(first["created"],2)
            self.assertEqual(first["existing"],0)
            self.assertEqual(len(first["assets"]),2)
            self.assertTrue(all(task["automated"] and task["save"] and
                                not task["replace_existing"] for task in unreal.tools.tasks))
            second=P.import_assets(unreal,mini)
            self.assertEqual(second["created"],0)
            self.assertEqual(second["existing"],2)
            self.assertEqual(len(unreal.tools.tasks),2)
        self.assertTrue(all(path.startswith(P.IMPORT_ROOT)
                            for path in first["assets"].values()))

    def test_existing_wrong_type_fails_closed_without_overwrite(self):
        plan=P.prepare_plan(self.world,self.equipment)
        mini={"entries":[plan["entries"][0]],"preview":[]}
        target=mini["entries"][0]
        unreal=FakeUnreal()
        unreal.lib.assets[target["destination"]+"/"+target["asset_name"]]=object()
        with patch.dict(os.environ,{"UNMADE_PROTO_IMPORT_APPROVED":"1"}):
            with self.assertRaisesRegex(RuntimeError,"StaticMesh|unexpected|wrong"):
                P.import_assets(unreal,mini)
        self.assertEqual(len(unreal.tools.tasks),0)

    def test_preview_is_isolated_noncolliding_and_safe_to_repeat(self):
        plan=P.prepare_plan(self.world,self.equipment)
        unreal=FakeUnreal()
        with patch.dict(os.environ,{"UNMADE_PROTO_IMPORT_APPROVED":"1"}):
            result=P.import_assets(unreal,plan)
            preview=P.stage_preview(unreal,plan,result["assets"])
            self.assertEqual(preview["created"],88)
            self.assertEqual(preview["existing"],0)
            self.assertEqual(len(unreal.EditorLevelLibrary.actors),88)
            self.assertTrue(all(a.component.collision=="NO_COLLISION"
                                for a in unreal.EditorLevelLibrary.actors))
            again=P.stage_preview(unreal,plan,result["assets"])
            self.assertEqual(again["created"],0)
            self.assertEqual(again["existing"],88)
            self.assertEqual(len(unreal.EditorLevelLibrary.actors),88)
            self.assertEqual(unreal.EditorLevelLibrary.saved,1)

    def test_never_stage_into_gameplay_or_rewrite_artist_actors(self):
        plan=P.prepare_plan(self.world,self.equipment)
        unreal=FakeUnreal()
        mock_assets={x["asset_key"]:StaticMesh() for x in plan["entries"]}
        with patch.dict(os.environ,{"UNMADE_PROTO_IMPORT_APPROVED":"1"}):
            unreal.subsystem.created.add(P.PREVIEW_MAP)
            unreal.subsystem.load_level(P.PREVIEW_MAP)
            existing=FakeActor((1,2,3));existing.label=plan["preview"][0]["label"]
            existing.component.mesh="ARTIST_MODEL"
            unreal.EditorLevelLibrary.actors.append(existing)
            report=P.stage_preview(unreal,plan,mock_assets)
            self.assertEqual(report["created"],87)
            self.assertEqual(report["existing"],1)
            self.assertEqual(existing.component.mesh,"ARTIST_MODEL")
            self.assertEqual(existing.position,(1,2,3))

if __name__=="__main__":unittest.main()
