from pathlib import Path
import importlib.util,json,subprocess,sys,unittest
ROOT=Path(__file__).resolve().parents[2]
def load(name,path):
    spec=importlib.util.spec_from_file_location(name,ROOT/path)
    m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
S=load("unmade_scene_builder","Scripts/build_story_scene_handoff.py")
T=load("unmade_scene_stager","Scripts/unreal_editor/stage_world_manifest.py")
class QuestAndSceneHandoff(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.q,cls.p,cls.w,cls.c=S.load()
    def test_quest_contracts_and_all_saved_branches(self):
        self.assertTrue(S.verify(self.q,self.p,self.w,self.c))
        self.assertEqual(len(S.quest_rows(self.q)),54)
        self.assertEqual([x["act"] for x in self.q["act_gates"]],list(range(7)))
        for quest in self.q["quests"]:
            self.assertEqual(len(quest["steps"]),6)
            self.assertEqual(set(x["outcome"] for x in quest["steps"]
                   if x.get("outcome")),{"care","truth"})
            self.assertIn("recovery",quest["steps"][2])
        self.assertEqual(len(self.q["final_encounter"]["stages"]),4)
    def test_82_persona_ids_canonically_ordered(self):
        self.assertEqual(len(self.p["residents"]),82)
        self.assertEqual(len(set(p["id"] for p in self.p["residents"])),82)
        native=(ROOT/"Source/TheUnmadeGame/Public/Authoring/UnmadeResidentReturnData.h").read_text()
        for entry in self.p["residents"]:
            self.assertIn(entry["id"],native)
        self.assertIn("ReturnDialogue",native)
    def test_generated_scene_manifest_and_import_contract(self):
        m=S.scene_manifest(self.w,self.c)
        self.assertEqual(m["expected_actor_count"],52)
        self.assertEqual(sum(x["kind"]=="RealmProductionReference" for x in m["actors"]),36)
        self.assertEqual(sum(x["kind"]=="CinematicShotReference" for x in m["actors"]),16)
        self.assertTrue(all(x["collision"] is False for x in m["actors"]))
        self.assertEqual(T.read_plan(),m)
        code=(ROOT/"Scripts/unreal_editor/stage_world_manifest.py").read_text()
        self.assertIn('if not a.apply:',code)
        self.assertIn("Unmade_AuthoringStaging",code)
        self.assertIn("if label in existing:",code)
        self.assertIn("save_current_level()",code)
        self.assertIn("NO_COLLISION",code)
    def test_stale_generated_rejected_and_exact_csv(self):
        expected=S.outputs(self.q,self.p,self.w,self.c)
        for path,data in expected.items():
            self.assertTrue(path.is_file(),str(path))
            actual=path.read_text(encoding="utf-8")
            if path.suffix==".json":
                self.assertEqual(json.loads(actual),json.loads(data))
            else:self.assertEqual(actual,data)
    def test_bad_tags_or_ids_rejected(self):
        q=json.loads(json.dumps(self.q))
        q["quests"][3]["real_source_tags"]["mechanism"]="Tidal.MadeUp.Mechanism"
        with self.assertRaises(ValueError):S.verify(q,self.p,self.w,self.c)
        p=json.loads(json.dumps(self.p))
        p["residents"][2]["id"]=p["residents"][0]["id"]
        with self.assertRaises(ValueError):S.verify(self.q,p,self.w,self.c)
    def test_editor_dry_run_without_unreal(self):
        out=subprocess.run([sys.executable,
            str(ROOT/"Scripts/unreal_editor/stage_world_manifest.py")],
            capture_output=True,text=True,check=True)
        self.assertIn("DRY RUN: 52",out.stdout)
        self.assertIn("36",out.stdout)
        self.assertIn("16",out.stdout)
    def test_runtime_wiring(self):
        hub=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp").read_text()
        char=(ROOT/"Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp").read_text()
        ret=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeResidentContinuityWorld.cpp").read_text()
        save=(ROOT/"Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h").read_text()
        self.assertIn("bHasResidentContinuitySnapshot",save)
        self.assertIn("ResidentRelationships=RestoredResidentContinuity",hub)
        self.assertIn("bResidentContinuityRejected",hub)
        self.assertIn("RecordResidentConversation(Target->GetStableId())",char)
        self.assertIn("RecordResidentAid(Target->GetStableId())",char)
        self.assertIn("FindReturnDialogue(ID.Get())",ret)
    def test_ci_and_first_pc(self):
        ci=(ROOT/".github/workflows/static-checks.yml").read_text()
        pc=(ROOT/"Scripts/first_pc_build_and_test.ps1").read_text()
        for src in (ci,pc):self.assertIn("build_story_scene_handoff.py",src)
        self.assertIn("resident_continuity_test.cpp",ci)
if __name__=="__main__":unittest.main()
