"""The first Windows PC procedure must be executable and fail closed."""
import importlib.util
import pathlib
import unittest
ROOT=pathlib.Path(__file__).resolve().parents[2]
_spec=importlib.util.spec_from_file_location(
    "unmade_staging_contract",ROOT/"Scripts/unreal_editor/stage_world_manifest.py")
STAGER=importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(STAGER)

class FakeStaticMesh:
    def __init__(self,path): self.path=path
    def get_path_name(self): return self.path

class FakeUnreal:
    StaticMesh=FakeStaticMesh
    def __init__(self,assets): self.assets=assets; self.requested=[]
    def load_asset(self,path):
        self.requested.append(path)
        return self.assets.get(path)
class FirstPcHandoffContract(unittest.TestCase):
    def test_build_and_unreal_automation_are_both_guarded(self):
        p=(ROOT/"Scripts/first_pc_build_and_test.ps1").read_text()
        for needle in ("check_unreal_host.ps1","Engine/Build/BatchFiles/Build.bat",
                       "TheUnmadeGameEditor","Win64","Development",
                       "run_ue_tests.ps1","UNREAL_EDITOR_BUILD_FAILED",
                       "HOST_PREFLIGHT_FAILED","$LASTEXITCODE",
                       "editor-build.log","automation.log","failure.txt"):
            self.assertIn(needle,p)
        self.assertNotIn("Start-Process msiexec",p)
    def test_manual_qa_not_masked(self):
        doc=(ROOT/"docs/qa/windows-unreal-host.md").read_text()
        self.assertIn("first_pc_build_and_test.ps1",doc)
        self.assertIn("does not",doc.lower())
class StagingMeshBindingTests(unittest.TestCase):
    def test_mesh_hints_map_to_stable_unreal_package_paths(self):
        hint="SM_Reach_BellWithoutClapper"
        self.assertEqual(
            "/Game/UnmadeProduction/Assets/SM_Reach_BellWithoutClapper.SM_Reach_BellWithoutClapper",
            STAGER.static_mesh_asset_path(hint))

    def test_imported_meshes_keep_authored_scale_while_proxies_get_marker_scale(self):
        self.assertEqual((1.0,1.0,1.0),STAGER.actor_scale_for_mesh("/Game/Assets/SM_Prop.SM_Prop"))
        self.assertEqual((.6,.6,1.4),STAGER.actor_scale_for_mesh(None))

    def test_non_mesh_and_unsafe_hints_never_become_asset_paths(self):
        for hint in ("SFX_Rain_BellNoBuoy","legacy_art_22.jpg","SM_../outside"):
            with self.subTest(hint=hint):
                self.assertIsNone(STAGER.static_mesh_asset_path(hint))

    def test_imported_static_mesh_is_selected_and_missing_asset_uses_proxy(self):
        hint="SM_Reach_BellWithoutClapper"
        path=STAGER.static_mesh_asset_path(hint)
        imported=FakeStaticMesh(path)
        unreal=FakeUnreal({path:imported})
        chosen,chosen_path=STAGER.resolve_static_mesh(unreal,hint,"cube")
        self.assertIs(chosen,imported)
        self.assertEqual(path,chosen_path)
        missing=FakeUnreal({})
        chosen,chosen_path=STAGER.resolve_static_mesh(missing,hint,"cube")
        self.assertEqual("cube",chosen)
        self.assertIsNone(chosen_path)

    def test_wrong_asset_type_falls_back_without_binding(self):
        hint="SM_Reach_BellWithoutClapper"
        path=STAGER.static_mesh_asset_path(hint)
        chosen,chosen_path=STAGER.resolve_static_mesh(FakeUnreal({path:object()}),hint,"cube")
        self.assertEqual("cube",chosen)
        self.assertIsNone(chosen_path)

    def test_only_engine_cube_placeholders_are_eligible_for_mesh_upgrade(self):
        cube=FakeStaticMesh("/Engine/BasicShapes/Cube.Cube")
        custom=FakeStaticMesh("/Game/Artist/Custom.Custom")
        self.assertTrue(STAGER.is_engine_cube_placeholder(
            FakeStaticMesh("/Engine/BasicShapes/Cube.Cube"),cube))
        self.assertFalse(STAGER.is_engine_cube_placeholder(custom,cube))

    def test_current_manifest_mesh_hints_are_all_importable_identifiers(self):
        manifest=STAGER.read_plan()
        hints={actor["asset_hint"] for actor in manifest["actors"]
               if actor["asset_hint"].startswith("SM_")}
        self.assertTrue(hints)
        self.assertTrue(all(STAGER.static_mesh_asset_path(hint) for hint in hints))


if __name__=="__main__":unittest.main()
