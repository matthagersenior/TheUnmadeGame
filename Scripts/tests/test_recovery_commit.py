"""Static adapter checks; portable tests cover the shared transaction behavior."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2] / 'Source/TheUnmadeGame'

class RecoveryCommitWiring(unittest.TestCase):
    def test_consumable_save_captures_healing_and_rolls_back_inventory(self):
        source = (ROOT / 'Private/Items/UnmadeEquipmentComponent.cpp').read_text()
        use = source.split('UnmadeCore::ConsumableCommit UUnmadeEquipmentComponent::UseConsumable(', 1)[1].split('\nbool ', 1)[0]
        self.assertIn('CommitHealing', use)
        self.assertIn('Inventory.Restore(Before)', use)
        self.assertIn('Persist(', use)

    def test_finite_healing_service_commits_benefit_with_use(self):
        source = (ROOT / 'Private/World/UnmadePlayerExperience.cpp').read_text()
        action = source.split('bool AUnmadePrototypeHub::ExperienceAction(', 1)[1].split('\nFString ', 1)[0]
        self.assertIn('CommitHealing', action)
        self.assertNotIn('case 7:p->GetCombat()->Heal(25)', action)
        self.assertIn('CommitIntegrationRecovery(action==9&&a==1?20:0', action)
        self.assertNotIn('case 1:p->RecoverIntegrationStrain(20)', action)

    def test_hunt_and_daily_sanctuary_commit_health_before_capture(self):
        source=(ROOT / 'Private/World/UnmadeWorldIdentity.cpp').read_text()
        action=source.split('bool AUnmadePrototypeHub::TryIdentityInteraction(',1)[1]
        self.assertIn('CommitHealing(*p->GetCombat(),heal?35:0',action)
        self.assertNotIn('if(heal)p->GetCombat()->Heal(35)',action)
        self.assertIn('Identity=before',action)
        self.assertIn('no reward consumed at full health',action)

    def test_local_favor_provision_commits_healing_with_purchase(self):
        source=(ROOT / 'Private/World/UnmadeAdventureDepth.cpp').read_text()
        action=source.split('bool AUnmadePrototypeHub::TryDepthInteraction(',1)[1].split('FString AUnmadePrototypeHub::GetDepthReaction',1)[0]
        self.assertIn('CommitHealing(*p->GetCombat(),heal?35:0',action)
        self.assertNotIn('if(heal)p->GetCombat()->Heal(35)',action)
        self.assertIn('Depth=before;DepthChallenges=beforeChallenge',action)

    def test_route_cache_prompt_has_no_phantom_kit_requirement(self):
        source=(ROOT / 'Private/World/UnmadeAdventureDepth.cpp').read_text()
        self.assertIn('E SEARCH AFTER OPENING ROUTE',source)
        self.assertNotIn('E SEARCH WITH ONE FIELD KIT',source)
        cache=source.split('if(kind==4)',1)[1].split('if(kind==5)',1)[0]
        self.assertNotIn('SpendKit',cache)

if __name__ == '__main__':
    unittest.main()
