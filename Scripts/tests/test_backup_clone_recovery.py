"""A user-owned backup must be cloned into a vacant profile, not overwrite either old slot."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source(p):return (ROOT/p).read_text(encoding="utf-8")
class RecoveryCopy(unittest.TestCase):
 def test_loader_preserves_original_backup_and_primary_when_cloning(self):
  cpp=source("Source/TheUnmadeGame/Private/Save/UnmadeSaveSession.cpp")
  segment=cpp.split("bool RecoverBackupIntoNewProfile(UObject* context,int p)",1)[1].split("bool CreateUnusedProfile",1)[0]
  self.assertIn('Read(SlotName(p)+TEXT("_Backup"))',segment)
  self.assertIn("FirstAvailableRecoveryProfile",segment)
  self.assertIn("UGameplayStatics::SaveGameToSlot(backup,destination,0)",segment)
  self.assertIn("if(!Read(destination))return false;",segment)
  self.assertIn("SelectProfile(context,target,false)",segment)
  self.assertNotIn("DeleteGameInSlot",segment)
  self.assertNotIn("SaveGameToSlot(backup,SlotName(p)",segment)
  self.assertIn("RecoverBackupIntoNewProfile",source("Source/TheUnmadeGame/Public/Save/UnmadeSaveSession.h"))
 def test_distinct_explicit_confirmed_ui_action_exposes_recovery(self):
  ui=source("Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp")
  self.assertIn('TEXT("Copy backup into unused profile")',ui)
  handler=ui.split("void UUnmadeRpgWidget::RecoverToNewProfile()",1)[1].split("\nvoid UUnmadeRpgWidget::",1)[0]
  self.assertIn("SessionIntent::RecoverCopy,SelectedProfile,false",handler)
  self.assertIn("UnmadeSave::RecoverBackupIntoNewProfile(this,SelectedProfile)",handler)
  self.assertLess(handler.index("SessionConfirm.Request"),handler.index("RecoverBackupIntoNewProfile"))
  self.assertIn("UFUNCTION() void RecoverToNewProfile();",source("Source/TheUnmadeGame/Public/UI/UnmadeRpgWidget.h"))
  rules=source("Source/TheUnmadeGame/Public/UI/UnmadePlayerFacingRules.h")
  self.assertIn("RecoverCopy",rules)
  self.assertIn("intent!=SessionIntent::Recover&&intent!=SessionIntent::RecoverCopy",rules)
if __name__=="__main__":unittest.main()
