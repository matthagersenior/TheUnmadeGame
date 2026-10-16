"""Source adapter checks supplement, and do not replace, portable/Unreal runs."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(path):return (ROOT/path).read_text()
class CombinedSliceWiring(unittest.TestCase):
    def test_rewards_read_committed_state_and_keep_transaction_rollback(self):
        source=read('Source/TheUnmadeGame/Private/Items/UnmadeEquipmentComponent.cpp')
        reconcile=source.split('void UUnmadeEquipmentComponent::ReconcileEarnedMilestones()',1)[1].split('\nbool ',1)[0]
        self.assertLess(reconcile.index('UnmadeSave::LoadGameFromSlot()'),reconcile.index('ForEachEarnedMilestone'))
        self.assertLess(reconcile.index('if (bSaveRejected || !Save'),reconcile.index('ForEachEarnedMilestone'))
        self.assertIn('Claim(reward)',reconcile)
        contract=source.split('void UUnmadeEquipmentComponent::ReconcileCivicContracts(',1)[1].split('\nbool ',1)[0]
        self.assertIn('ForEachCivicContract(*Save',contract)
        self.assertIn('Economy.PayContract(town,stage)',contract)
        self.assertIn('if(!Persist())Economy.Restore(Before)',contract)

    def test_sanitized_regressions_are_in_both_repeatable_gates(self):
        workflow=read('.github/workflows/static-checks.yml')
        audit=read('Scripts/audit_gameplay_dependencies.py')
        for path in ('Tests/integration/combined_slice_scenario.cpp','Tests/save/living_economy_bounds_test.cpp'):
            self.assertIn(path,workflow)
            self.assertIn(path,audit)
        for flag in ('-fsanitize=undefined','-fno-sanitize-recover=undefined'):
            self.assertIn(flag,workflow)
            self.assertIn(flag,audit)

    def test_household_failed_writes_restore_all_affected_owners(self):
        world=read('Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp')
        action=world.split('bool AUnmadePrototypeHub::TryBesHouseholdInteraction(',1)[1].split('\nvoid ',1)[0]
        failed=action.split('if(!WriteWorldSnapshot())',1)[1].split('RefreshBesHouseholdWorld()',1)[0]
        self.assertIn('Materials.Restore(BeforeMaterials,BeforeCargo,GetGameDay())',failed)
        self.assertIn('LivingTrades.Restore(BeforeLiving,Materials,GetGameDay())',failed)
        self.assertIn('BesHousehold.Restore(Prev,GetGameDay())',failed)

    def test_owned_reward_selection_failure_never_reports_equipped(self):
        world=read('Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp')
        equipment=read('Source/TheUnmadeGame/Private/Items/UnmadeEquipmentComponent.cpp')
        self.assertIn('CommitRealmSelection(Loadout,Earned,Arc',equipment)
        self.assertIn('!p->GetEquipment()->SelectRealmReward(arc)',world)
        self.assertIn('!p->GetEquipment()->SelectRealmReward(a)',world)
        self.assertEqual(world.count('your previous equipment remains active.'),2)
        for notice in ('Reward ownership is saved','Armor ownership is saved'):
            failure=world.split(notice,1)[1].split('return true;',1)
            self.assertEqual(len(failure),2)
