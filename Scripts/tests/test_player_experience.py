import unittest,sys,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'Scripts'))
import build_player_experience as build
import validate_cpp_literals as lexical
class PlayerExperienceContract(unittest.TestCase):
 def test_authored_wings_scenes_and_specialties(self):
  rows=json.loads((ROOT/'Authoring/player_experience_v17.json').read_text())['realms']
  self.assertEqual(len(rows),9);self.assertEqual(sum(len(r['scenes']) for r in rows),36)
  self.assertEqual(build.build(),(ROOT/'Source/TheUnmadeGame/Public/World/UnmadePlayerExperienceRegistry.h').read_text())
 def test_source_lexical_regression(self):
  self.assertTrue(lexical.errors('auto s="bad\nstring";'))
  self.assertTrue(lexical.errors('void f(){}\\nvoid g(){}'))
  self.assertFalse(lexical.errors('auto s="good\\nstring"; // "comment\n'))
  self.assertFalse(lexical.errors('auto s=R"raw(multiline\ntext)raw";'))
  for p in (ROOT/'Source').rglob('*'):
   if p.suffix in ('.h','.cpp'):self.assertFalse(lexical.errors(p.read_text()),str(p))
 def test_contact_and_projectiles_reach_real_physics(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp').read_text()
  for token in ['BeginWeaponStrike','SweepMultiByObjectType','ResolveWeaponContact','ReservedPlayerShot','WeaponTip','TickWeaponContact']:self.assertIn(token,s)
  p=(ROOT/'Source/TheUnmadeGame/Private/Combat/UnmadeTravelingProjectile.cpp').read_text()
  for token in ['SweepSingleByObjectType','ECC_WorldStatic','ECC_Pawn','ReceiveTravelingHit','Expires','Destroy()']:self.assertIn(token,p)
 def test_unified_equipment_and_old_save_migration(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/Items/UnmadeEquipmentComponent.cpp').read_text()
  for token in ['UnifiedLoadout::Migrate','Loadout.Valid(Earned)','Loadout.weapon=-1','Loadout.armor=-1','Loadout=Old','NativeGearBonuses','DescribeComparison']:self.assertIn(token,s)
  style=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp').read_text();self.assertNotIn('Expeditions.Save().gear%2==1',style);self.assertIn('RealmWeapon()',style)
 def test_real_optional_guardian_and_rollbacks(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp').read_text()
  for token in ['e->GetCombat()->IsDefeated()','WingGuardians[a].Get()!=e','Experience=before','Clock=oldClock','WingRelayDeadline=deadlines','Near(p,speaker','WriteWorldSnapshot(p)','ExperienceRoutineTarget','Experience.Reconcile']:self.assertIn(token,s)
 def test_discovery_gate_and_settings(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadePlayerExperience.cpp').read_text()
  for token in ['MapKnowsRoom','MapKnowsChoice','if(!IsRealmKnown(a))','Experience.DiscoverCity']:self.assertIn(token,s)
  ui=(ROOT/'Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp').read_text()
  for token in ['h->IsRealmKnown(i)','GetExperienceMap','GetTrackedObjective','CaptureControl','FInputModeUIOnly','ActionGroups']:self.assertIn(token,ui)
  pref=(ROOT/'Source/TheUnmadeGame/Private/UI/UnmadePlayerPreferences.cpp').read_text()
  for token in ['SaveConfig','ForceRebuildKeymaps','m.ActionName!=names[action-1]','TextScale','CameraSensitivity','AudioVolume']:self.assertIn(token,pref)
 def test_save_dependencies_and_editor_dry_run(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp').read_text()
  for token in ['if(Save->bHasPlayerExperience)','if(bExperienceRejected)return false','Experience.Restore','Experience.Pack','BuildPlayerExperience']:self.assertIn(token,s)
  editor=(ROOT/'Scripts/unreal_editor/install_combat_events.py').read_text();self.assertIn('--execute',editor);self.assertIn('UNREAL_EDITOR_REQUIRED',editor)
if __name__=='__main__':unittest.main()
