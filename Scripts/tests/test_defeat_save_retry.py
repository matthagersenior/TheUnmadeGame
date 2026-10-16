from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
class DefeatRetryWiring(unittest.TestCase):
    def test_both_defeated_actors_use_the_portable_retry_boundary(self):
        for actor in ('UnmadeRealmGuardian','UnmadeFinalBoss'):
            source=(ROOT/f'Source/TheUnmadeGame/Private/Combat/{actor}.cpp').read_text()
            defeated=source.split('if(GetCombat()->IsDefeated())',1)[1].split('AUnmadeCharacter* Player',1)[0]
            self.assertIn('DefeatSave.Attempt(',defeated)
            self.assertIn('DefeatSaveResult::FirstFailure',defeated)
            self.assertIn('Retrying every five seconds',defeated)
            self.assertIn('reloading returns to the previous save',defeated)
            self.assertIn('DefeatSave.Reset()',source)
    def test_retry_state_is_transient_and_generated_includes_stay_last(self):
        for actor in ('UnmadeRealmGuardian','UnmadeFinalBoss'):
            header=(ROOT/f'Source/TheUnmadeGame/Public/Combat/{actor}.h').read_text()
            self.assertIn('UnmadeCore::DefeatSaveRetry DefeatSave',header)
            includes=[line for line in header.splitlines() if line.startswith('#include')]
            self.assertIn(f'{actor}.generated.h',includes[-1])
        save=(ROOT/'Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h').read_text()
        self.assertNotIn('DefeatSaveRetry',save)
