import unittest,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]/'Source/TheUnmadeGame'
def s(path):return (ROOT/path).read_text()
class PlayerFacingAdapters(unittest.TestCase):
 def test_formatter_uses_tested_single_pass_policy(self):
  text=s('Private/UI/UnmadePlayerPreferences.cpp');self.assertIn('FormatControlLabels',text);self.assertNotIn('result.Find(key',text)
 def test_recovery_is_scoped_and_never_saves_over_backup(self):
  text=s('Private/UI/UnmadeRpgWidget.cpp');handler=text.split('void UUnmadeRpgWidget::LoadBackup(){')[1].split('\n')[0]
  self.assertLess(handler.index('SessionConfirm.Request'),handler.index('SelectProfile'));self.assertIn('SessionIntent::Recover,SelectedProfile,false',handler);self.assertNotIn('SaveSessionNow',handler);self.assertIn('ActionFeedback=TEXT',handler);self.assertIn('Page==6||Page==7',text)
  self.assertIn('SelectedProfile=(SelectedProfile+1)%4;SessionConfirm.Clear()',text);self.assertIn('if(!bOpen)return;SessionConfirm.Clear()',text)
 def test_both_trade_directions_require_actual_visibility(self):
  text=s('Private/Player/UnmadeCharacter.cpp')
  for name in ['BuyMarketSupplies','SellMarketSupplies']:
   handler=text.split('void AUnmadeCharacter::'+name+'()')[1].split('\nvoid ',1)[0];self.assertIn('UnmadeTradeSight',handler);self.assertIn('LineTraceTestByChannel',handler);self.assertIn('PhysicalTradeAllowed',handler);self.assertIn('It->CanTradeWithPlayer()',handler)
 def test_creator_uses_existing_saved_setters_and_real_choices(self):
  text=s('Private/UI/UnmadeRpgWidget.cpp')
  for token in ['SetCharacterFeature','SetCharacterChosenName','CharacterIdentity::ChoiceCounts','NextAspect','PreviousFeature','NextFeature','NameInput->GetText','%8','Page==7?text:pref->FormatControls(text)']:self.assertIn(token,text)
  header=s('Public/UI/UnmadeRpgWidget.h')
  for callback in re.findall(r'UFUNCTION\(\)\s+void (\w+)\(',header):self.assertIn('UUnmadeRpgWidget::'+callback+'(',text)
  for n in range(28):self.assertIn(f'void UUnmadeRpgWidget::Action{n}(){{PerformAction({n});}}',text)
  player=s('Private/Player/UnmadeCharacter.cpp')
  for name in ['SetCharacterFeature','SetCharacterChosenName']:
   handler=player.split('bool AUnmadeCharacter::'+name)[1].split('\nvoid ',1)[0];self.assertIn('Identity.Restore(Before)',handler);self.assertIn('SaveCharacterIdentity()',handler)
  self.assertIn('TEXT("Sneak")',player.split('RpgMenuAction(FName Action)',1)[1])
 def test_enemy_motion_uses_tested_priority(self):
  text=s('Private/Combat/UnmadeEnemyCharacter.cpp');self.assertIn('MotionMayReplace',text);self.assertIn('CurrentMotion==FName(TEXT("Stagger"))&&now<MotionHeldUntil',text);self.assertIn('CurrentMotion=NAME_None; // A new injury restarts',text)
if __name__=='__main__':unittest.main()
