from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source():return (ROOT/'Source/TheUnmadeGame/Private/Save/UnmadeSaveSession.cpp').read_text()
class ProfileHandoffWiring(unittest.TestCase):
    def test_failed_travel_is_scoped_and_restores_original_selection(self):
        s=source()
        self.assertIn('OnTravelFailure().AddStatic',s)
        failure=s.split('void ProfileTravelFailed(',1)[1].split('void EnsureTravelFailureHandler()',1)[0]
        self.assertIn('OriginWorld.Get()',failure)
        self.assertIn('Handoff.Fail(',failure)
        self.assertIn('ClearTravelFailureHandler()',failure)
        self.assertIn('OriginInstance->GetWorld()==OriginWorld.Get()',failure)
        self.assertIn('Original.profile',failure)
        self.assertIn('Original.backup',failure)
        self.assertIn('PROFILE LOAD FAILED',failure)
        self.assertNotIn('SaveGameToSlot',failure)
    def test_selection_is_staged_and_duplicate_requests_cannot_create_files(self):
        s=source()
        selection=s.split('bool SelectProfile(',1)[1].split('bool CreateUnusedProfile(',1)[0]
        self.assertIn('Handoff.Pending()',selection)
        self.assertIn('Handoff.Begin(',selection)
        self.assertNotIn('settings->ActiveProfile=p',selection)
        self.assertNotIn('SaveConfig()',selection)
        self.assertLess(selection.index('Handoff.Begin('),selection.index('OpenLevel('))
        new=s.split('bool CreateUnusedProfile(',1)[1].split('FString DescribeProfiles()',1)[0]
        self.assertLess(new.index('Handoff.Pending()'),new.index('CreateSaveGameObject'))
    def test_registration_requires_the_new_world_before_accepting_target(self):
        s=source();register=s.split('void RegisterPlayer(',1)[1].split('FString SlotName(',1)[0]
        self.assertIn('Handoff.RegisterOwner(',register)
        self.assertIn('OriginWorld.Get()==p->GetWorld()',register)
        self.assertIn('ProfileArrival::Waiting',register)
        self.assertIn('FailedWorld.Get()!=p->GetWorld()',register)
        self.assertIn('ProfileArrival::TargetReady',register)
        self.assertIn('settings->ActiveProfile=Selected.profile',register)
        self.assertIn('SaveConfig()',register)
        self.assertIn('ClearTravelFailureHandler()',register)
        self.assertIn('if(Handoff.WritesBlocked())return WriteFailed()',s)
        self.assertIn('Handoff.RecoverTarget()',s)
        self.assertIn('if(Handoff.Pending())return; // Destination feedback',s)
