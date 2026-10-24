"""Public return-road consequences are observed locally, not claimed as private eyewitness memories."""
import json
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]


class NineRealmReturnConsequences(unittest.TestCase):
    def test_both_visible_aftereffects_are_distinct_and_present_in_generated_registry(self):
        rows=json.loads((ROOT/"Authoring/adventure_depth_v16.json").read_text(encoding="utf-8"))["realms"]
        self.assertEqual([row["realm_index"] for row in rows],list(range(9)))
        for key in ("public_return_a","public_return_b"):
            self.assertEqual(len({row[key] for row in rows}),9)
            self.assertTrue(all(len(row[key])>=70 for row in rows))
        rules=(ROOT/"Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h").read_text()
        self.assertIn("const char* public_return_a;",rules)
        self.assertIn("const char* public_return_b;",rules)
        builder=(ROOT/"Scripts/build_adventure_depth.py").read_text()
        self.assertIn("'public_return_a'",builder)
        self.assertIn("'public_return_b'",builder)

    def test_public_aftereffects_require_witnessed_return_and_actual_choice(self):
        s=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp").read_text()
        reaction=s.split("FString AUnmadePrototypeHub::GetExpeditionReaction(FName person)const",1)[1]
        self.assertIn("if(t->returned)",reaction)
        self.assertIn("Depth.Arc(a)->choice",reaction)
        self.assertIn("public_return_a",reaction)
        self.assertIn("public_return_b",reaction)
        self.assertIn("n==1||n==2||n==4",reaction)
        self.assertLess(reaction.index("if(t->returned)"),reaction.index("public_return_a"))
        self.assertIn("if((n==1||n==2)&&!(t->rescued",reaction)
        self.assertNotIn("Depth.Choose(",reaction)

    def test_native_data_are_nine_nonempty_pairs(self):
        code=r"""
#include "World/UnmadeAdventureDepthRegistry.h"
#include <cassert>
#include <cstring>
#include <set>
#include <string>
int main() {
 std::set<std::string> care,truth;
 for(const auto& row:UnmadeCore::DepthSpecs) {
  assert(std::strlen(row.public_return_a)>=70);
  assert(std::strlen(row.public_return_b)>=70);
  assert(std::strcmp(row.public_return_a,row.public_return_b)!=0);
  care.insert(row.public_return_a);truth.insert(row.public_return_b);
 }
 assert(care.size()==9 && truth.size()==9);
}
"""
        with tempfile.TemporaryDirectory() as tmp:
            p=Path(tmp)/"return.cpp";exe=Path(tmp)/"return"
            p.write_text(code)
            res=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",
                str(ROOT/"Source/TheUnmadeGame/Public"),str(p),"-o",str(exe)],
                capture_output=True,text=True)
            self.assertEqual(res.returncode,0,res.stderr)
            run=subprocess.run([str(exe)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)


if __name__=="__main__":
    unittest.main()
