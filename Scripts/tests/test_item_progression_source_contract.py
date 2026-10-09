"""Gear and rewards must be real, durable, NPC/quest-earned and AI-independent."""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]
def read(p): return (ROOT/p).read_text(encoding="utf-8")
class GearSourceContract(unittest.TestCase):
    def test_item_catalog_and_deterministic_rewards_have_native_coverage(self):
        core=read("Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h")
        workflow=read(".github/workflows/static-checks.yml")
        self.assertIn("std::array<ItemDef, static_cast<int>(ItemId::Count)>",core)
        for x in ("Waybreaker", "UnwrittenCrown", "BellwoldBulwark",
                  "PaperhavenLens", "HearthSalve", "StrainVial", "ForgeWaybreaker"):
            self.assertIn(x,core)
        self.assertIn("Tests/items/item_rules_test.cpp",workflow)
        self.assertIn("Tests/world/world_atlas_test.cpp",workflow)
        self.assertIn("Tests/integration/offline_slice_scenario.cpp",workflow)

    def test_ue_actor_has_equipment_and_player_controls(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        config=read("Config/DefaultInput.ini")
        for action in ("ItemInventory","ItemWeapon","ItemArmor","ItemCharm",
                       "ItemHeal","ItemStrain","ItemForge"):
            self.assertIn('BindAction("'+action+'"',player)
            self.assertIn('ActionName="'+action+'"',config)
        for token in ("CreateDefaultSubobject<UUnmadeEquipmentComponent>",
                      "ForgeWaybreaker()", "ReconcileEarnedRewards()",
                      "Target->GetCombat()->IsDefeated()", "Achievement::FirstWatcher"):
            self.assertIn(token,player)
        combat=read("Source/TheUnmadeGame/Private/Combat/UnmadeCombatComponent.cpp")
        self.assertIn("Swing.baseDamage + GearAttack",combat)
        self.assertIn("Target->GearArmor",combat)

    def test_earned_rewards_only_from_canonical_saved_state(self):
        cpp=read("Source/TheUnmadeGame/Private/Items/UnmadeEquipmentComponent.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for token in ("ItemQuantities","EquippedItems","AwardedMilestoneBits",
                      "bHasInventorySnapshot"):
            self.assertIn(token,save)
            self.assertIn(token,cpp)
        for token in ("Save->BellwoldTaskStage == 2", "Save->PaperhavenTaskStage == 2",
                      "Save->VisitedSettlementsMask", "Save->DiscoveredLoreMask",
                      "Save->SupplyActivityStage == 2", "Inventory.Restore(Before)",
                      "if (bSaveRejected) return false"):
            self.assertIn(token,cpp.replace("==2"," == 2"))
        self.assertIn("Save->PlayerStrain = UpdatedStrain",cpp)
        self.assertIn("Player->GetCombat()->Heal(heal)",cpp)
        self.assertNotIn("RequestDialogue(",cpp)
        self.assertNotIn("Ollama",cpp)

    def test_unique_gear_changes_reality_mechanics_without_ai(self):
        rules=read("Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h")
        effects=read("Source/TheUnmadeGame/Public/Fracture/UnmadeFractureRules.h")
        equipment=read("Source/TheUnmadeGame/Public/Items/UnmadeEquipmentComponent.h")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for name in ("GlimpseStrainDiscount", "FoldDurationBonus", "RecoveryMultiplier"):
            self.assertIn(name, rules)
        self.assertIn("Result::InvalidModifier", effects)
        self.assertIn("FoldBonusSeconds()", equipment)
        self.assertIn("GlimpseDiscount()", equipment)
        self.assertIn("StrainRecoveryMultiplier()", equipment)
        self.assertIn("FractureModel.Glimpse(IsValid(Anchor), GetWorld()->GetTimeSeconds(), Discount)", player)
        self.assertIn("FractureModel.Fold(IsValid(Anchor), GetWorld()->GetTimeSeconds(), Extension)", player)

    def test_atlas_is_explicitly_future_data_not_silent_unreal_claims(self):
        atlas=read("Source/TheUnmadeGame/Public/World/UnmadeWorldAtlas.h")
        self.assertIn("std::array<RealmSpec,",atlas)
        self.assertIn("WorldAtlas",atlas)
        self.assertIn("PlanRealmRoute(",atlas)
        self.assertIn("AttunementFromRewards(",atlas)
        self.assertIn("DESIGN DATA",atlas)
        self.assertNotIn("HTTP",atlas)

if __name__=="__main__":
    unittest.main()
