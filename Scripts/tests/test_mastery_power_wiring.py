import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


class MasteryPowerWiring(unittest.TestCase):
    def test_power_uses_accepted_owner_and_spatial_gate_before_expense(self):
        source = (ROOT / 'Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp').read_text()
        power = source.split('bool AUnmadePrototypeHub::UseDepthPower(', 1)[1].split('UnmadeCore::FieldKitResult AUnmadePrototypeHub::UseDepthFieldKit', 1)[0]
        self.assertIn('UnmadeSave::IsSessionWritable(p)', power)
        self.assertIn('p->GetWorld()!=GetWorld()', power)
        self.assertIn('MasteryTargetInRange', power)
        self.assertNotIn('DistSquared2D', power)
        self.assertLess(power.index('IsSessionWritable(p)'), power.index('UsePower(now)'))
        self.assertLess(power.index('MasteryTargetInRange'), power.index('UsePower(now)'))
        self.assertNotIn('WriteWorldSnapshot', power)

    def test_component_checks_stagger_before_portable_power_spending(self):
        source = (ROOT / 'Source/TheUnmadeGame/Public/Combat/UnmadeCombatComponent.h').read_text()
        power = source.split('bool UsePower(', 1)[1].split('\n', 1)[0]
        self.assertIn('TryMasteryPower(Tempo,Now,!IsDefeated(),bPlayerTempo,IsStaggered(Now))', power)

    def test_selection_uses_transaction_and_reports_failures(self):
        source = (ROOT / 'Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp').read_text()
        selection = source.split('bool AUnmadePrototypeHub::SelectDepthTechnique(', 1)[1].split('bool AUnmadePrototypeHub::UseDepthPower(', 1)[0]
        self.assertIn('CommitMasterySelection', selection)
        self.assertIn('MasterySelection::SaveFailed', selection)
        self.assertIn('MasterySelection::Unchanged', selection)
        self.assertIn('previous technique', selection)
        self.assertIn('IsSessionWritable(p)', selection)
