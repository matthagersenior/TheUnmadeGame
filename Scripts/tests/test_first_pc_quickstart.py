"""First-PC prep must be autonomous to inspect and fail closed before UE writes."""
from __future__ import annotations
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import unittest

ROOT=Path(__file__).resolve().parents[2]
def load(name,path):
    spec=importlib.util.spec_from_file_location(name,ROOT/path)
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module
P=load("first_pc_prod_contract","Scripts/prepare_unreal_authoring.py")
E=load("first_pc_editor_batch","Scripts/unreal_editor/first_pc_editor_batch.py")

class PcProductionQuickstart(unittest.TestCase):
    def test_198_rows_seven_real_source_tables(self):
        plan=P.authoring_plan()
        self.assertEqual(plan["total_rows"],198)
        self.assertEqual(len(plan["tables"]),7)
        self.assertEqual([x["rows"] for x in plan["tables"]],[18,9,10,16,9,54,82])
        self.assertEqual(plan["staging_map"],"/Game/UnmadeProduction/Maps/Unmade_AuthoringStaging")
        self.assertEqual(len({x["asset"] for x in plan["tables"]}),7)
        self.assertTrue(all(len(x["sha256"])==64 for x in plan["tables"]))
        self.assertTrue(all(x["asset"].startswith("/Game/UnmadeProduction/Data/") for x in plan["tables"]))
        self.assertEqual(len(E.validate()[1]["actors"]),52)
        self.assertEqual(len(E.validate()[2]["work_orders"]),90)

    def test_no_unreal_needed_for_inspection(self):
        for script in ["Scripts/prepare_unreal_authoring.py","Scripts/unreal_editor/first_pc_editor_batch.py"]:
            proc=subprocess.run([sys.executable,str(ROOT/script)],
                stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True,
                timeout=15,check=True)
            self.assertIn("52",proc.stdout)
            self.assertNotIn("Traceback",proc.stderr)

    def test_manifest_drift_fails_closed(self):
        original=P.TABLES
        try:
            P.TABLES=(("npcs_unreal.csv","DT_UnmadeNPCs","UnmadeNpcAuthoringRow",19),)
            with self.assertRaises(ValueError):P.authoring_plan()
            P.TABLES=(("ghost.csv","DT_Ghost","UnmadeNpcAuthoringRow",1),)
            with self.assertRaises(ValueError):P.authoring_plan()
        finally:
            P.TABLES=original

    def test_windows_double_click_and_reporting_contract(self):
        cmd=(ROOT/"START_THE_UNMADE.cmd").read_text()
        ps=(ROOT/"Scripts/first_pc_quickstart.ps1").read_text()
        for name in ("powershell.exe","first_pc_quickstart.ps1","%ERRORLEVEL%","pause"):
            self.assertIn(name,cmd)
        for name in ("EngineAssociation","UE_","prepare_unreal_authoring.py",
                     "first_pc_build_and_test.ps1","UNMADE_EDITOR_APPROVED",
                     "UnrealEditor-Cmd.exe","editor-import-receipt.json",
                     "EDITOR_PYTHON_IMPORT_FAILED","Start-Process",
                     "InspectOnly","SkipStage","failure.txt"):
            self.assertIn(name,ps)
        self.assertNotIn("Invoke-WebRequest",ps)
        self.assertNotIn("winget install",ps)
        project=json.loads((ROOT/"TheUnmadeGame.uproject").read_text())
        self.assertTrue(any(p["Name"]=="PythonScriptPlugin" and p["Enabled"]
                            for p in project["Plugins"]))

    def test_import_only_in_namespaced_folder(self):
        body=(ROOT/"Scripts/unreal_editor/first_pc_editor_batch.py").read_text()
        self.assertIn('os.environ.get("UNMADE_EDITOR_APPROVED")!="1"',body)
        self.assertIn('"/Game/UnmadeProduction/Maps/Unmade_AuthoringStaging"',body)
        self.assertIn("if not isinstance(asset,unreal.DataTable)",body)
        self.assertIn('get_editor_property("row_struct")',body)
        self.assertIn("save_loaded_asset(asset)",body)
        self.assertIn("STAGER.apply_in_editor(stage)",body)
        self.assertIn("LIVED.apply_in_editor(lived)",body)
        self.assertIn("lived_staging",body)
        self.assertIn("UNMADE_EDITOR_RECEIPT",body)

if __name__=="__main__":
    unittest.main()
