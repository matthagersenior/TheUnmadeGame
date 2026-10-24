"""Adapter/fixture parity checks; C++ exercises the production validation logic."""
from pathlib import Path
import re
import unittest
ROOT = Path(__file__).resolve().parents[2]
PUBLIC = ROOT / 'Source/TheUnmadeGame/Public'
class CombinedSaveWiring(unittest.TestCase):
    def test_fixture_has_every_serialized_field_and_presence_guard(self):
        save = (PUBLIC / 'Save/UnmadePrototypeSave.h').read_text()
        fixture = (ROOT / 'Tests/save/combined_record_fixture.h').read_text()
        validator = (PUBLIC / 'Save/UnmadeCombinedSaveRules.h').read_text()
        critical = (PUBLIC / 'Save/UnmadeCriticalSavePreflight.h').read_text()
        for declaration in re.findall(r'UPROPERTY\(SaveGame\)\s*([^;]+);', save):
            names = declaration.split(' ', 1)[1].split('=', 1)[0]
            for name in names.split(','):
                name = name.split('=')[0].strip()
                if not re.fullmatch(r'[A-Za-z_]\w*', name):
                    continue
                self.assertRegex(fixture, r'\b' + name + r'\b')
                self.assertTrue('Save->' + name in validator or 'save.' + name in critical, name)

    def test_profile_read_and_write_use_preflight(self):
        source = (ROOT / 'Source/TheUnmadeGame/Private/Save/UnmadeSaveSession.cpp').read_text()
        self.assertIn('ValidCombinedRecord(record', source)
        self.assertIn('save&&ValidStored(*save,slot)', source)
        self.assertIn('typed&&ValidStored(*typed,slot)', source)
        self.assertIn('CommitCombinedRecord(*s,existing,backup', source)
        self.assertLess(source.index('CaptureSession(s)'), source.index('CommitCombinedRecord(*s'))
        self.assertIn('if(!Read(slot+(backup?', source)
        self.assertIn('if(!settings->UseBackup){settings->UseBackup=true;', source)
        self.assertIn('LOAD FAILED:', source)
        self.assertIn('SAVE FAILED:', source)
        self.assertNotIn('StoredRecord::Incompatible?raw', source)

    def test_potions_stop_after_persistence_failure(self):
        source = (ROOT / 'Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp').read_text()
        for name in ('UseHealthPotion', 'UseStrainPotion'):
            handler = source.split('void AUnmadeCharacter::' + name + '()', 1)[1].split('\nvoid ', 1)[0]
            self.assertIn('UseFirstConsumable', handler)
            self.assertIn('ConsumableCommit::SaveFailed', handler)
            self.assertIn('Nothing consumed or restored', handler)

    def test_legacy_survey_import_cannot_bypass_record_rejection(self):
        session = (ROOT / 'Source/TheUnmadeGame/Private/Save/UnmadeSaveSession.cpp').read_text()
        encounters = (ROOT / 'Source/TheUnmadeGame/Private/World/UnmadePlayableEncounters.cpp').read_text()
        self.assertIn('ValidSurveyRecord(legacy->Version,legacy->Stages)', session)
        self.assertIn('!Save&&UnmadeSave::HasRecord()', encounters)
        self.assertIn('else if(bFieldSurveySaveUsable&&', encounters)
        self.assertIn('if(existing&&!ValidStored(*existing,slot))existing=nullptr', session)
