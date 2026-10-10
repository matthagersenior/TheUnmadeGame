"""The master pack must be the single editing source; C++ and Unreal CSV drift fail."""
import importlib.util
import json
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
SPEC=importlib.util.spec_from_file_location("unmade_world_builder",
                                           ROOT/"Scripts/build_world_content.py")
MODULE=importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)

class WorldPack(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.data=json.loads((ROOT/"Authoring/world_content_pack.json").read_text())
    def test_schema_and_canon(self):
        MODULE.verify(self.data)
        self.assertEqual(len(self.data["realms"]),9)
        self.assertEqual(len(self.data["npcs"]),18)
        self.assertEqual(len(self.data["abilities"]),10)
    def test_all_outputs_checked_in_and_reproducible(self):
        for filename,content in MODULE.render(self.data).items():
            self.assertTrue(filename.is_file(),filename)
            self.assertEqual(filename.read_text(encoding="utf-8"),content,str(filename))
    def test_reject_duplicate_and_unregistered_people(self):
        invalid=json.loads(json.dumps(self.data))
        invalid["npcs"][1]["id"]=invalid["npcs"][0]["id"]
        with self.assertRaises(ValueError):MODULE.verify(invalid)
        invalid=json.loads(json.dumps(self.data))
        invalid["npcs"][3]["id"]="npc.mystery.unregistered.999"
        with self.assertRaises(ValueError):MODULE.verify(invalid)
    def test_actor_binds_real_names_and_lines(self):
        world=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeLaterRealmWorld.cpp").read_text()
        npc=(ROOT/"Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp").read_text()
        bootstrap=(ROOT/"Scripts/first_pc_build_and_test.ps1").read_text()
        self.assertIn("FindOuterPerson(Witness.Id)",world)
        self.assertIn("Authored->displayName",world)
        self.assertIn("Authored?Authored->dayLine:Witness.Line",world)
        self.assertIn("FindOuterPerson(Identifier.Get())",npc)
        self.assertIn("LocalProfile->nightLine:LocalProfile->dayLine",npc)
        self.assertIn("build_world_content.py",bootstrap)
if __name__=="__main__":unittest.main()
