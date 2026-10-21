"""Actual code path must persist healed player health and halt save-failure fallback."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def src(p):return (ROOT/p).read_text(encoding="utf-8")
class AtomicHealingWiring(unittest.TestCase):
    def test_gear_consumption_stages_combat_before_capture_and_unwinds_inventory(self):
        equip=src("Source/TheUnmadeGame/Private/Items/UnmadeEquipmentComponent.cpp")
        part=equip.split("bool UUnmadeEquipmentComponent::UseConsumable(",1)[1].split("\nbool UUnmadeEquipmentComponent::ForgeWaybreaker",1)[0]
        self.assertIn("CommitHealing(",part)
        self.assertIn("return Persist(FMath::Max(",part)
        self.assertNotIn("if(heal>0) Player->GetCombat()->Heal(heal)",part)
        self.assertIn("Inventory.Restore(Before)",part)
        self.assertIn("if(bPersistenceFailed)*bPersistenceFailed=true;",part)
        self.assertIn("bool* bPersistenceFailed=nullptr",
                      src("Source/TheUnmadeGame/Public/Items/UnmadeEquipmentComponent.h"))
    def test_combat_recovery_stages_before_writer_preserving_receipts(self):
        cpp=src("Source/TheUnmadeGame/Private/Combat/UnmadeCombatComponent.cpp")
        self.assertIn("UnmadeCore::CommitCombatHealing(State",cpp)
        helper=src("Source/TheUnmadeGame/Public/Save/UnmadeRecoveryRules.h")
        self.assertIn("const Combatant before=state",helper)
        self.assertLess(helper.index("state.Heal(amount)"),helper.index("if(writer())return true;"))
        self.assertIn("state=before",helper)
    def test_both_potion_lists_stop_at_first_storage_failure(self):
        player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for handler,nextname in (("UseHealthPotion","UseStrainPotion"),
                                 ("UseStrainPotion","ForgeMythicGear")):
            block=player.split("void AUnmadeCharacter::"+handler+"()",1)[1].split(
                "void AUnmadeCharacter::"+nextname+"()",1)[0]
            self.assertIn("&bPersistenceFailed",block)
            self.assertIn("if(bPersistenceFailed)",block)
            self.assertIn("SAVE FAILED: nothing consumed or restored",block)
            self.assertIn("return;",block.split("if(bPersistenceFailed)",1)[1])
        self.assertIn("Tests/save/atomic_healing_test.cpp",
                      src(".github/workflows/static-checks.yml"))
if __name__=="__main__":unittest.main()
