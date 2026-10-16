"""Static adapter checks; portable tests cover the shared transaction behavior."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2] / 'Source/TheUnmadeGame'

class RecoveryCommitWiring(unittest.TestCase):
    def test_consumable_save_captures_healing_and_rolls_back_inventory(self):
        source = (ROOT / 'Private/Items/UnmadeEquipmentComponent.cpp').read_text()
        use = source.split('bool UUnmadeEquipmentComponent::UseConsumable(', 1)[1].split('\nbool ', 1)[0]
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

if __name__ == '__main__':
    unittest.main()
