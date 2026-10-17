import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


class EncounterHazardWiring(unittest.TestCase):
    def test_chamber_refresh_gates_receipts_and_damage_on_restored_owner(self):
        source = (ROOT / 'Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp').read_text()
        refresh = source.split('void AUnmadePrototypeHub::RefreshExpeditions()', 1)[1].split('bool AUnmadePrototypeHub::ExpeditionEnemyInactive', 1)[0]
        self.assertIn('p->GetWorld()==GetWorld()&&UnmadeSave::IsSessionWritable(p)', refresh)
        self.assertIn('if(playerReady&&!bExpeditionSaveRejected)', refresh)
        self.assertIn('EncounterHazardInCylinder(playerReady,Alive(p),hazard', refresh)
        self.assertLess(refresh.index('IsSessionWritable(p)'), refresh.index('Expeditions.Enter('))
        self.assertLess(refresh.index('EncounterHazardInCylinder'), refresh.index('ReceiveHazardPulse('))
        self.assertIn('280,150', refresh)

    def test_wing_refresh_retains_real_pulse_geometry_and_sight(self):
        source = (ROOT / 'Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp').read_text()
        refresh = source.split('void AUnmadePrototypeHub::RefreshPlayerExperience()', 1)[1].split('bool AUnmadePrototypeHub::WingGuardianInactive', 1)[0]
        self.assertIn('p->GetWorld()==GetWorld()&&UnmadeSave::IsSessionWritable(p)', refresh)
        self.assertIn('SampleWingFlood(', refresh)
        self.assertIn('EncounterHazardInSphere(playerReady,', refresh)
        self.assertIn('Near(p,Wing(a,1),750)', refresh)
        self.assertIn('flood.pulse,10', refresh)
        self.assertIn('if(playerReady&&!bExperienceRejected', refresh)
        self.assertLess(refresh.index('IsSessionWritable(p)'), refresh.index('ReceiveHazardPulse('))
        # Required construction presentation still runs before the player resumes.
        self.assertIn('WingExitGates[a]->SetActorHiddenInGame', refresh)
        self.assertIn('SettlementRoomGates[town]->SetActorHiddenInGame', refresh)
