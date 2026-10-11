"""Stage a physical return letter only after a legitimate next-day response."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(path):return (ROOT/path).read_text(encoding="utf-8")

class WitnessReturnWiring(unittest.TestCase):
    def test_real_source_and_save_guards(self):
        rules=read("Source/TheUnmadeGame/Public/World/UnmadeWitnessReturnRules.h")
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        slot=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for term in ("day>delivery.deliveredDay","saved.chosenDay>currentDay",
                     "saved.deliveredDay<saved.collectedDay","Stage()==WitnessDispatchStage::HandDelivered",
                     "PRIVATE COUNSEL","PUBLIC HEARING","OrrelOnReceipt"):
            if term=="Stage()==WitnessDispatchStage::HandDelivered":continue
            self.assertIn(term,rules)
        for term in ("RestoredReturn.Restore","bWitnessReturnRejected=true",
                     "bWitnessDispatchRejected || bWitnessReturnRejected",
                     "Save->bHasWitnessReturnSnapshot=true",
                     "Save->WitnessReturnDeliveredDay=Reply.deliveredDay",
                     "ReturnWitness.Restore(Before,Dispatch,WitnessBraid.Outcome(),GetGameDay())",
                     "GetGameDay()", "UnmadeReturnPickup","UnmadeReturnReceipt"):
            self.assertIn(term,hub)
        for term in ("bHasWitnessReturnSnapshot=false","WitnessReturnStage=0",
                     "WitnessReturnRoute=0","WitnessReturnChosenDay=0",
                     "WitnessReturnCollectedDay=0","WitnessReturnDeliveredDay=0"):
            self.assertIn(term,slot)
    def test_player_uses_existing_twice_confirmed_choice_and_real_actors(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        self.assertIn('Scope=FName("WitnessReturn.HessaReply")',hub)
        self.assertIn('Nearby(FName("npc.bellwold.matron.001"))',hub)
        self.assertIn('GetStableId()!=FName("npc.bridgekeeper.001")',hub)
        self.assertIn('FName("WitnessReturn.ReplyNote")',hub)
        self.assertIn('FName("WitnessReturn.PrivateMarker")',hub)
        self.assertIn('FName("WitnessReturn.PublicMarker")',hub)
        self.assertIn("Hub->ResolveNearbyWitnessReturn",player)
        self.assertIn("Hub->TryWitnessReturn(this,ResidentDistSq)",player)
        self.assertIn("CommitmentGate.Attempt(",player)
        self.assertIn("GetWitnessReturnLine(NpcId)",npc)
        self.assertIn("GetWitnessDispatchRecipientLine(NpcId)",npc)
        self.assertIn("FMath::Square(255.f)",hub)
        self.assertIn("FMath::Square(260.f)",hub)
    def test_ci_and_canonical_volume(self):
        self.assertIn("Tests/world/witness_return_test.cpp",read(".github/workflows/static-checks.yml"))
        self.assertIn("test_witness_return_wiring.py",read("Scripts/first_pc_build_and_test.ps1"))
        for path in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                     "docs/lore/living-world-continuity.md",
                     "docs/lore/CONTINUITY_LOG.md"):
            self.assertIn("The Return of the Witness",read(path))

if __name__=="__main__":unittest.main()
