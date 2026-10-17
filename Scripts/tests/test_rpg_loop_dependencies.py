import unittest,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def source(path):return (ROOT/'Source/TheUnmadeGame'/path).read_text()
class CompleteLoopDependencies(unittest.TestCase):
 def test_every_subsystem_uses_the_same_profile_write_path(self):
  for p in (ROOT/'Source/TheUnmadeGame').rglob('*.cpp'):
   if p.name=='UnmadeSaveSession.cpp':continue
   self.assertNotIn('UGameplayStatics::SaveGameToSlot(',p.read_text(),str(p))
   if p.name!='UnmadePlayableEncounters.cpp':self.assertNotIn('TEXT("UnmadePrototypeNPC")',p.read_text(),str(p))
  survey=source('Private/World/UnmadePlayableEncounters.cpp');self.assertIn('SlotName()==TEXT("UnmadePrototypeNPC")',survey);self.assertIn('FieldSurveyStages=Save->FieldSurveyStages',survey);self.assertIn('CommitFieldSurveyStage(FieldSurveyStages,i',survey)
  self.assertIn('Save->FieldSurveyStages=FieldSurveyStages',source('Private/World/UnmadePrototypeHub.cpp'))
  s=source('Private/Save/UnmadeSaveSession.cpp')
  for v in ['CommitWithPrevious','Handoff.WritesBlocked()','existing->SchemaVersion!=1','HasRejectedWorld','HasRejectedSave','_Backup']:self.assertIn(v,s)
 def test_profile_changes_preserve_existing_records(self):
  s=source('Private/Save/UnmadeSaveSession.cpp');self.assertNotIn('DeleteGameInSlot',s)
  self.assertIn('for(int p=1;p<=3;++p)',s);self.assertIn('DoesSaveGameExist(slot+TEXT("_Backup")',s)
  self.assertIn('Handoff.Begin(EffectiveSelection(),{p,backup})',s)
 def test_resume_waits_for_restored_world_and_prepared_geometry(self):
  s=source('Private/Player/UnmadeCharacter.cpp');part=s.split('void AUnmadeCharacter::RestoreSessionWhenReady()',1)[1].split('void AUnmadeCharacter::CaptureSession',1)[0]
  self.assertLess(part.index('IsWorldReady()'),part.index('SetActorLocation'))
  self.assertLess(part.index('PrepareRealmGeometry'),part.index('SetActorLocation'))
  self.assertIn('Combat->RestoreSession',part)
 def test_all_actions_have_real_handlers_and_controller_buttons(self):
  registry=source('Public/UI/UnmadePlayerActionRules.h');actions=re.findall(r'\{"(\w+)",',registry);self.assertEqual(len(actions),28)
  player=source('Private/Player/UnmadeCharacter.cpp').split('RpgMenuAction(FName Action)',1)[1].split('\n',1)[0]
  ui=source('Private/UI/UnmadeRpgWidget.cpp');header=source('Public/UI/UnmadeRpgWidget.h')
  for n,a in enumerate(actions):self.assertIn('TEXT("'+a+'")',player);self.assertIn('TEXT("Action'+str(n)+'")',ui);self.assertIn('void Action'+str(n)+'();',header)
  self.assertIn('Close();p->RpgMenuAction',ui);self.assertIn('%8',ui)
 def test_crafting_selects_and_validates_the_real_workshop(self):
  equipment=source('Private/Items/UnmadeEquipmentComponent.cpp');self.assertNotIn('for(const auto& Recipe:UnmadeCore::Recipes)',equipment)
  for token in ['static_cast<UnmadeCore::RecipeId>(recipe)','Inventory.Restore(old)','Economy.Restore(money)','CraftField','DescribeRecipe']:self.assertIn(token,equipment)
  world=source('Private/World/UnmadePlayerExperience.cpp');self.assertIn('Near(p,Hall(a,site)',world);self.assertIn('KnowsFieldRecipe(a)',world)
 def test_controls_reach_text_and_physical_signs(self):
  ui=source('Private/UI/UnmadeRpgWidget.cpp');world=source('Private/World/UnmadePrototypeHub.cpp')
  self.assertIn('FormatControls(h->GetTrackedObjective())',ui);self.assertIn('FormatControls(text)',ui);self.assertIn('SignOriginal',world);self.assertIn('prefs->FormatControls',world)
 def test_distance_budget_keeps_economy_clock_independent(self):
  s=source('Private/World/UnmadePrototypeHub.cpp');self.assertIn('DeferredBlocks.Add',s);self.assertIn('PrepareRealmGeometry(player->GetActorLocation())',s);self.assertIn('SimulationInterval',s)
  self.assertNotIn('SetActorTickEnabled(false)',s);self.assertIn('AdvanceWorldClock(Clock,true,DeltaSeconds)',s)
 def test_prototype_performances_are_distinct_and_stagger_has_priority(self):
  s=source('Public/Combat/UnmadeEncounterPerformanceRules.h');self.assertEqual(len(re.findall(r'\{"',s)),9)
  e=source('Private/Combat/UnmadeEnemyCharacter.cpp');self.assertIn('MotionHeldUntil',e);self.assertIn('MotionMayReplace',e);self.assertIn('EncounterPerformances[Arc]',e);self.assertIn('CurrentMotion==FName(TEXT("Death"))',e)
  n=source('Private/NPC/UnmadeNpcCharacter.cpp');self.assertIn('ServiceWorkCue(NpcId,Current)',n)
if __name__=='__main__':unittest.main()
