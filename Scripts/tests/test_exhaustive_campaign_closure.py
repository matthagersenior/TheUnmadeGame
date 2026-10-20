"""Keep the no-relic main-story exhaustive test and honest first-PC work queue discoverable."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(path):return (ROOT/path).read_text(encoding="utf-8")
class SourceCampaignClosure(unittest.TestCase):
    def test_native_exhaustive_campaign_registered_in_real_ci(self):
        steps=read(".github/workflows/static-checks.yml")
        name="Tests/story/campaign_exhaustive_walkthrough_test.cpp"
        self.assertIn(name,steps)
        self.assertIn("/tmp/unmade-exhaustive-road",steps)
        test=read(name)
        self.assertIn("mask<(1<<static_cast<int>(Realm::Count))",test)
        self.assertIn("for(int opening=0;opening<=1;++opening)",test)
        self.assertIn("Connected(",test)
        self.assertIn("Morning::Anchor",test)
        self.assertIn("Morning::Many",test)
        self.assertIn("FinalLocalRecord",test)
        self.assertIn("assert(!EvaluateUnansweredRoad(atUpper).bossAccessible)",test)
        self.assertIn("assert(!EvaluateUnansweredRoad(missingAuvren).bossAccessible)",test)
    def test_live_gap_ledger_does_not_confuse_native_tests_with_ue_build(self):
        m=read("docs/qa/pre-pc-closure-matrix.md")
        self.assertIn("1,024 combinations",m)
        self.assertIn("Unreal",m)
        self.assertIn("SOURCE WIRED",m)
        self.assertIn("202 independent",m)
        self.assertIn("57 total",m)
        self.assertIn("CONTENT MISSING",m)
        self.assertIn("actual play",m)
        self.assertIn("docs", "docs")
        status=read("docs/development-status.md")
        self.assertIn("pre-pc-closure-matrix.md",status)
        self.assertIn("pre-pc-closure-matrix.md",read("docs/qa/pre-pc-readiness.md"))
if __name__=="__main__":unittest.main()
