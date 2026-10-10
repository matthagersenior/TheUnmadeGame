"""City-first development must not invent residents, quest states, or finished assets."""
from __future__ import annotations
from collections import Counter
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]

def load(name,path):
    spec=importlib.util.spec_from_file_location(name,ROOT/path)
    module=importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module

CITY=load("city_source_factory","Scripts/build_city_assembly.py")
STAGE=load("city_editor_stager","Scripts/unreal_editor/stage_city_assembly.py")
BATCH=load("one_click_batch","Scripts/unreal_editor/first_pc_editor_batch.py")

class CityAssemblyCoverage(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.plan=CITY.build()

    def test_every_city_is_distinct_and_canonical(self):
        canon=json.loads(CITY.CANON.read_text())
        kits=json.loads(CITY.KITS.read_text())
        self.assertEqual([(c["id"],c["realm"],c["name"]) for c in kits["cities"]],
                         [(c["id"],c["realm"],c["name"]) for c in canon["cities"]])
        self.assertEqual(len({c["profile"] for c in kits["cities"]}),11)
        self.assertEqual(len({tuple(c["origin_cm"]) for c in kits["cities"]}),11)
        self.assertEqual(self.plan["cities"],11)

    def test_402_real_canon_source_references_and_proxies(self):
        count=self.plan["counts"]
        self.assertEqual(count,{"SIGNATURE":88,"STREET":44,"SHELL":88,
                                "RESIDENT":82,"QUEST":54,"RITE":10,"CLUE":27,
                                "SIDEQUEST":9})
        self.assertEqual(sum(count.values()),402)
        self.assertEqual(len(self.plan["actors"]),402)
        self.assertEqual(len({a["label"] for a in self.plan["actors"]}),402)
        self.assertTrue(all(a["collision"] is False and
                            a["gameplay_authority"] is False for a in self.plan["actors"]))
        self.assertEqual({a["city"] for a in self.plan["actors"]},
                         {c["name"] for c in json.loads(CITY.KITS.read_text())["cities"]})
        self.assertTrue(all(a["label"].startswith("UM_CITY_CITY_") for a in self.plan["actors"]))

    def test_native_actor_id_and_ability_identity_unchanged(self):
        people=json.loads(CITY.PEOPLE.read_text())["residents"]
        quests=json.loads(CITY.QUESTS.read_text())["quests"]
        world=json.loads(CITY.RITES.read_text())
        residents={p["id"] for p in people}
        quest={f"{q['chapter']}/{s['id']}" for q in quests for s in q["steps"]}
        rites={a["id"] for a in world["abilities"]}
        self.assertEqual({a["source_id"] for a in self.plan["actors"]
                         if a["kind"]=="RESIDENT"},residents)
        self.assertEqual({a["source_id"] for a in self.plan["actors"]
                         if a["kind"]=="QUEST"},quest)
        self.assertEqual({a["source_id"] for a in self.plan["actors"]
                         if a["kind"]=="RITE"},rites)
        self.assertEqual(sum(a["kind"]=="RESIDENT" and a["city"]=="Bellwold"
                             for a in self.plan["actors"]),16)

    def test_corrupt_city_kit_is_rejected_before_editor(self):
        original=CITY.KITS
        try:
            with tempfile.TemporaryDirectory() as tmp:
                source=json.loads(original.read_text())
                source["cities"][0]["name"]="Wrong Canonical City"
                bad=Path(tmp)/"mismatch.json"
                bad.write_text(json.dumps(source))
                CITY.KITS=bad
                with self.assertRaises(ValueError):CITY.build()
        finally:
            CITY.KITS=original

    def test_offline_scene_validation(self):
        self.assertEqual(STAGE.read_plan()["actor_count"],402)
        self.assertEqual(BATCH.validate()[3]["actor_count"],402)
        proc=subprocess.run([sys.executable,
            str(ROOT/"Scripts/unreal_editor/stage_city_assembly.py")],
            capture_output=True,text=True,check=True,timeout=20)
        self.assertIn("DRY RUN ONLY",proc.stdout)
        self.assertIn("402",proc.stdout)

    def test_first_pc_and_ci_require_all_cities(self):
        scripts=(ROOT/"Scripts/unreal_editor/first_pc_editor_batch.py").read_text()
        ps=(ROOT/"Scripts/first_pc_quickstart.ps1").read_text()
        ci=(ROOT/".github/workflows/static-checks.yml").read_text()
        self.assertIn("put_city_assembly",scripts)
        self.assertIn("CITY.apply_in_editor(city)",scripts)
        self.assertIn('"city_assembly":city_stage',scripts)
        self.assertIn("city_assembly.created+$result.city_assembly.skipped -ne 402",ps)
        self.assertIn("build_city_assembly.py --check",ci)
        self.assertIn("stage_city_assembly.py",ci)

if __name__=="__main__":
    unittest.main()
