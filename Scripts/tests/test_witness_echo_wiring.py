"""Guard physical Volume X clues, durable journal and source-to-canon keys."""
from pathlib import Path
import json
import re
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source(path):return (ROOT/path).read_text(encoding="utf-8")
class WitnessEchoWiring(unittest.TestCase):
    def test_authoritative_volume_x_callback_ids_match_cpp(self):
        json_lore=json.loads(source("Authoring/transmedia_storyworld_v1.json"))
        ids={c["id"] for c in json_lore["callback_register"]}
        c=source("Source/TheUnmadeGame/Public/World/UnmadeWitnessEchoRules.h")
        source_ids=set(re.findall(r'\{"(CALL\.\d+)","site\.callback\.',c))
        self.assertEqual(source_ids,{"CALL.01","CALL.03","CALL.04"})
        self.assertTrue(source_ids.issubset(ids))
        self.assertIn("WitnessEchoRegistrySize=13",c)
        self.assertIn("if((s.first&~WitnessEchoValidMask)",c)
        self.assertIn("(s.returnRead&~s.first)",c)
        self.assertIn("FirstReading",c)
        self.assertIn("LaterMeaning",c)

    def test_relics_are_physical_and_not_quest_shortcuts(self):
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for relic in ("site.callback.uncounted_cup","site.callback.two_faced_press",
                      "site.callback.inkless_nail"):
            self.assertIn('FName("'+relic+'")',hub)
            self.assertIn('"'+relic+'"',source("Source/TheUnmadeGame/Public/World/UnmadeWitnessEchoRules.h"))
        self.assertIn("AUnmadePrototypeHub::InspectSite(AUnmadeLoreSite* Site)",hub)
        self.assertIn("WitnessEchoes.Read(PhysicalUtf8.Get(),LocalOutcome)",hub)
        self.assertIn("WitnessEchoes.Restore(OldEcho)",hub)
        self.assertIn("(bNew || bEchoChanged) && !WriteWorldSnapshot()",hub)
        self.assertIn("WitnessEchoSummary=Hub->GetWitnessEchoJournal();",player)
        self.assertIn("WitnessEchoSummary",player)
        # New clues don't increase original ordinary landmark count or claim a
        # main quest result from a concept story.
        self.assertNotIn("UnmadeCore::RoadAdvance(",source("Source/TheUnmadeGame/Public/World/UnmadeWitnessEchoRules.h"))

    def test_save_fields_are_optional_and_malformed_not_overwritten(self):
        save=source("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertIn("bool bHasWitnessEchoSnapshot=false",save)
        self.assertIn("int32 WitnessEchoFirstMask=0",save)
        self.assertIn("int32 WitnessEchoReturnMask=0",save)
        self.assertIn("if(Save->bHasWitnessEchoSnapshot)",hub)
        self.assertIn("bWitnessEchoRejected=true",hub)
        self.assertIn("bWitnessEchoRejected)return false",hub)
        self.assertIn("Save->WitnessEchoFirstMask=",hub)
        self.assertIn("Save->WitnessEchoReturnMask=",hub)

    def test_offline_cpp_covered_in_ci_and_first_pc_handoff(self):
        workflow=source(".github/workflows/static-checks.yml")
        first_pc=source("Scripts/first_pc_build_and_test.ps1")
        self.assertIn("Tests/world/witness_echo_test.cpp",workflow)
        self.assertIn("test_witness_echo_wiring.py",first_pc)
if __name__=="__main__":unittest.main()
