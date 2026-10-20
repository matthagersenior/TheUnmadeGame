"""Ensure cosmetic archetypes use existing NPC and creature actors only."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")
class CharacterVisualWiring(unittest.TestCase):
    def test_residents_keep_collision_memory_and_true_imported_rigs(self):
        cpp=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        head=read("Source/TheUnmadeGame/Public/NPC/UnmadeNpcCharacter.h")
        for term in ('UnmadeCharacterGrayboxRules.h','ResidentGraybox(Role)',
                     'NPCProfessionSilhouette','AccessoryVisual->SetupAttachment(GetCapsuleComponent())',
                     'AccessoryVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision)',
                     'AccessoryVisual->SetGenerateOverlapEvents(false)',
                     'AccessoryVisual->SetCanEverAffectNavigation(false)',
                     'if(!bSkeletalRigLoaded','AccessoryVisual->SetVisibility(false)'):
            self.assertIn(term,cpp)
        self.assertIn("TObjectPtr<UStaticMeshComponent> AccessoryVisual;",head)
        self.assertIn("Memory = CreateDefaultSubobject<UUnmadeMemoryComponent>",cpp)
        self.assertIn("Tags.AddUnique(StableId)",cpp)
    def test_creatures_keep_existing_ability_collision_and_attack_profiles(self):
        cpp=read("Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp")
        for term in ('UnmadeCharacterGrayboxRules.h','CreatureGraybox(Kind)',
                     'CreatureStats(Kind,bChampion)',
                     'Combat->Configure(Profile.health,Profile.damage,Profile.cooldown)',
                     'if(!bSkeletalRigLoaded',
                     'EncounterCrest->SetCollisionEnabled(ECollisionEnabled::NoCollision)',
                     'EncounterCrest->SetGenerateOverlapEvents(false)',
                     'EncounterCrest->SetCanEverAffectNavigation(false)'):
            self.assertIn(term,cpp)
        self.assertNotIn("SaveGameToSlot",
                          read("Source/TheUnmadeGame/Public/World/UnmadeCharacterGrayboxRules.h"))
        self.assertIn("Tests/npc/visual_archetypes_test.cpp",read(".github/workflows/static-checks.yml"))
if __name__=="__main__":unittest.main()
