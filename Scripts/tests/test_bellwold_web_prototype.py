"""Source-level guardrails for the publicly labeled Bellwold web concept scene."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]
SITE=ROOT/"site"/"play-bellwold.html"
CANON=ROOT/"docs"/"design"/"2026-10-09-bellwold-afterlight.md"

class BellwoldWebPrototypeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.html=SITE.read_text(encoding="utf-8")
        cls.canon=CANON.read_text(encoding="utf-8")

    def test_realm_asset_reference_and_mobile_controls(self):
        self.assertIn("models/Environment/SM_Unmade_ThreefoldReach.glb",self.html)
        self.assertIn("data-move=",self.html)
        self.assertIn("keydown",self.html)
        self.assertIn("INTERACT",self.html)

    def test_exact_authenticated_evidence_and_witness_pairs(self):
        for word in ("relief cache","altered census","Sorin","Ivera","Hessa","Safe Ward","Open Census"):
            self.assertIn(word.lower(),self.html.lower())
            self.assertIn(word.lower(),self.canon.lower())
        self.assertIn("evidence==='relief'",self.html)
        self.assertIn("evidence==='census'",self.html)
        self.assertIn("witness=true",self.html)

    def test_saved_browser_choice_is_not_claimed_to_be_unreal_save(self):
        self.assertIn("localStorage.setItem",self.html)
        self.assertIn("localStorage.removeItem",self.html)
        self.assertIn("not the Unreal executable",self.html)
        self.assertIn("not in the game's canonical saved data",self.html)
        self.assertIn("not the final Bellwold map",self.html)

    def test_inline_js_syntax_when_node_available(self):
        node=shutil.which("node")
        if not node:self.skipTest("Node unavailable")
        scripts=re.findall(r'<script type="module">([\s\S]+?)</script>',self.html)
        self.assertEqual(len(scripts),1)
        with tempfile.TemporaryDirectory() as td:
            file=Path(td)/"prototype.mjs"
            file.write_text(scripts[0],encoding="utf-8")
            subprocess.run([node,"--check",str(file)],check=True,capture_output=True,text=True)

if __name__=="__main__":
    unittest.main()
