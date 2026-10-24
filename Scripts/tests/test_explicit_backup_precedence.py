"""Backing up an incompatible primary remains safe and user-directed."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
class BackupOverrideWiring(unittest.TestCase):
 def test_source_policy_honors_explicit_selection_before_schema_refusal(self):
  h=(ROOT/"Source/TheUnmadeGame/Public/Save/UnmadeSessionRules.h").read_text()
  self.assertLess(h.index("if(preferBackup)return backupReadable"),
                  h.index("if(primaryExists&&!compatible)return StoredRecord::Incompatible"))
  s=(ROOT/"Source/TheUnmadeGame/Private/Save/UnmadeSaveSession.cpp").read_text()
  self.assertIn("EffectiveSelection().backup",s)
  self.assertIn("auto* backup=Read(slot+TEXT(\"_Backup\"))",s)
  self.assertIn("if(stored&&(!existing||existing->SchemaVersion!=1))return WriteFailed();",s)
  self.assertIn("existing&&!ValidStored(*existing,slot)",s)
  self.assertIn("bool SelectProfile(UObject* context,int p,bool backup)",s)
  self.assertIn("settings->UseBackup=Selected.backup;",s)
if __name__=="__main__":unittest.main()
