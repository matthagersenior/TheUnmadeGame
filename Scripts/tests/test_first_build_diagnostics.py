"""Static contract tests for the Windows first-build handoff.

These guard the scripts and diagnostics while an Unreal host is unavailable.
They do not claim to execute PowerShell or compile the Unreal project.
"""
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


class FirstBuildDiagnosticsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build = (ROOT / "Scripts/first_pc_build_and_test.ps1").read_text(encoding="utf-8")
        cls.preflight = (ROOT / "Scripts/check_unreal_host.ps1").read_text(encoding="utf-8")
        cls.quickstart = (ROOT / "Scripts/first_pc_quickstart.ps1").read_text(encoding="utf-8")
        cls.launcher = (ROOT / "START_THE_UNMADE.cmd").read_text(encoding="utf-8")

    def test_every_authored_registry_gate_precedes_editor_compilation(self):
        compile_at = self.build.index('Write-Host "Compiling C++ Editor target')
        gates = (
            "build_remembered_roads.py",
            "build_frontier_adventures.py",
            "build_world_identity.py",
            "build_adventure_depth.py",
        )
        for gate in gates:
            with self.subTest(gate=gate):
                self.assertLess(self.build.index(gate), compile_at)
        catch_at = self.build.index("} catch {")
        self.assertLess(compile_at, catch_at)
        self.assertNotIn("python ", self.build[catch_at:])

    def test_host_preflight_reports_exact_engine_and_build_toolchain(self):
        for evidence in (
            "Engine/Build/Build.version",
            "MajorVersion",
            "MinorVersion",
            "installationVersion",
            "cl.exe",
            "Windows Kits/10/Include",
            "Win32_OperatingSystem",
            "Win32_VideoController",
            "Build.bat",
        ):
            with self.subTest(evidence=evidence):
                self.assertIn(evidence, self.preflight)

    def test_quickstart_compile_only_mode_cannot_claim_automation_or_stage_success(self):
        self.assertIn("[switch]$BuildOnly", self.quickstart)
        self.assertIn("-BuildOnly:$BuildOnly", self.quickstart)
        self.assertIn('if(!$SkipStage -and !$BuildOnly)', self.quickstart)
        self.assertIn('if(!$DontOpenEditor -and !$BuildOnly)', self.quickstart)
        self.assertIn("BUILT_NOT_TESTED_NOT_STAGED", self.quickstart)

    def test_double_click_failure_message_points_to_real_report_folder(self):
        self.assertIn("TestReports/quickstart-*", self.launcher)
        self.assertNotIn("TestReportsquickstart-*", self.launcher)

    def test_first_build_diagnostics_document_matches_compile_only_entrypoint(self):
        doc = (ROOT / "docs/qa/first-build-diagnostics.md").read_text(encoding="utf-8")
        self.assertIn(".\\START_THE_UNMADE.cmd -BuildOnly", doc)
        self.assertIn("TheUnmadeGameEditor", doc)
        self.assertIn("first actual UBT/UHT/MSVC error", doc)


if __name__ == "__main__":
    unittest.main()
