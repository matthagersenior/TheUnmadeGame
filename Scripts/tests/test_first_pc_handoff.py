"""The first Windows PC procedure must be executable and fail closed."""
import pathlib
import unittest
ROOT=pathlib.Path(__file__).resolve().parents[2]
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
if __name__=="__main__":unittest.main()
