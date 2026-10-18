"""Narrative acceptance is gameplay acceptance: the mystery, action, cost and witness must agree.

These checks intentionally reject claims of automatic consent and unrelated courier grind.
"""
import json
import re
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
def data(path):return json.loads((ROOT/path).read_text(encoding="utf-8"))
def source(path):return (ROOT/path).read_text(encoding="utf-8")

REALMS=("ThreefoldReach","WidowedRain","HearthBeneath","TidalLedger",
        "SkyBelow","CinderSpine","HundredUnlived","OrchardOfKings","FirstAbsence")
CITIES=("Crossings","Saltwake","Cinderhold","Drevlach","Orravane",
        "Vathless","Eillun","Tharniv","Auvren")
ROOT_QUESTION=("removed from reality","who benefits","right to restore")

class NarrativePurposeIntegrity(unittest.TestCase):
 def test_nine_authored_stories_drive_one_question_with_physical_actions(self):
  depth=data("Authoring/adventure_depth_v16.json")["realms"]
  quest=data("Authoring/main_quest_interaction_contracts.json")
  mech=data("Authoring/world_identity_v15.json")["realms"]
  combat=data("Authoring/nine_realm_tactical_encounters_v1.json")["encounters"]
  self.assertEqual([r["realm_index"] for r in depth],list(range(9)))
  self.assertEqual(tuple(r["city"] for r in depth),CITIES)
  self.assertEqual(tuple(q["realm"] for q in quest["quests"]),REALMS)
  self.assertEqual(tuple(x["city"] for x in mech),CITIES)
  self.assertEqual(tuple(x["realm"] for x in combat),CITIES)
  for a,(row,main,identity,fight) in enumerate(zip(depth,quest["quests"],mech,combat)):
   for field in ("mystery_question","physical_lead","thread_after_return","signature_mechanics"):
    self.assertGreater(len(row[field]),55 if field!="signature_mechanics" else 10,(a,field))
   self.assertIn("?",row["mystery_question"])
   self.assertNotIn("Nhal-Vey",row["mystery_question"] if a<8 else "")
   self.assertTrue(any(x in row["physical_lead"].lower() for x in
      ("evade","brace","fight","tune","turn","cross","read","inspect","reverse","climb","trace","disarm","ring","align","affirm","disrupt","hold","open","release")),
      (a,"no physical player action"))
   self.assertTrue(any(x in row["physical_lead"].lower() for x in
      ("guard","boss","tollkeeper","dungeon","threat","danger","arena","sentinel","provost","counter","enemy","fracture","hazard","combat","seal","duelist","choir")),
      (a,"no physical danger/counterplay"))
   self.assertNotEqual(row["option_a"],row["option_b"])
   self.assertNotEqual(row["return_a"],row["return_b"])
   self.assertNotEqual(row["public_return_a"],row["public_return_b"])
   self.assertTrue(main["player_verbs"] and fight["objective"] and identity["side_quests"])
  self.assertEqual(len({r["mystery_question"] for r in depth}),9)
  self.assertEqual(len({r["thread_after_return"] for r in depth}),9)
  self.assertEqual([a["act"] for a in quest["act_gates"]],list(range(7)))
  self.assertEqual(quest["final_encounter"]["id"],"QUEST_FINAL_NHAL_VEY")
  self.assertIn("QUEST_FINAL_NHAL_VEY",source("Authoring/main_quest_interaction_contracts.json"))

 def test_consented_truth_never_fabricates_publicly_named_victims(self):
  depth=data("Authoring/adventure_depth_v16.json")["realms"]
  quest=data("Authoring/main_quest_interaction_contracts.json")["quests"]
  for realm in (0,5,6):
   row=depth[realm]
   public=" ".join(row[k].lower() for k in
     ("option_b","return_b","witness_return_b","public_return_b"))
   self.assertRegex(public,r"anonym|unnamed|voluntar|private|withheld|without.*names")
   for prohibited in ("names are posted","neighboring names are","register names openly",
       "publish the excluded names","publish the neighboring names",
       "names have a witness beside them"):
    self.assertNotIn(prohibited,public,(row["city"],prohibited))
   manual=quest[realm]["world_after_save"]["truth"].lower()
   self.assertTrue(any(x in manual for x in ("consent","opt-in","without forc","protected")))
  # Critically, document text cannot imply a fictional consent event was saved.
  self.assertIn("not a recorded consent",source("docs/lore/living-world-continuity.md").lower())

 def test_playable_journal_explains_why_a_quest_matters_without_spoiling_the_answer(self):
  registry=source("Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h")
  builder=source("Scripts/build_adventure_depth.py")
  journal=source("Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp")
  for field in ("mystery_question","physical_lead","thread_after_return","signature_mechanics"):
   self.assertIn("const char* "+field+";",registry)
   self.assertIn("'"+field+"'",builder)
   self.assertIn("spec."+field,journal)
  self.assertIn("if(Expeditions.Arc(a)->returned&&s->choice)",journal)
  self.assertIn("UNANSWERED QUESTION",journal)
  self.assertIn("WHAT YOU CAN DO",journal)
  self.assertIn("NEXT THREAD",journal)
  self.assertIn("Reality can contain",source("docs/lore/living-world-continuity.md"))
  # Optional courier content must not become the condition for core finale.
  final=source("Authoring/main_quest_interaction_contracts.json")
  final_gates=data("Authoring/main_quest_interaction_contracts.json")["act_gates"]
  mandatory=" ".join(x["requires"].lower() for x in final_gates if not x["optional"])
  for forbidden in ("cross-realm custody","report delivery","nine packets","petition relay"):
   self.assertNotIn(forbidden,mandatory)
  self.assertIn("Nhal-Vey",final)

if __name__=="__main__":unittest.main()
