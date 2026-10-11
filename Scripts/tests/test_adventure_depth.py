import json,unittest,sys,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'Scripts'))
import build_adventure_depth as builder
class DepthContracts(unittest.TestCase):
 def test_authored_quests_distinct(self):
  rows=json.loads((ROOT/'Authoring/adventure_depth_v16.json').read_text())['realms']
  for f in builder.FIELDS:self.assertEqual(len({r[f] for r in rows}),9,f)
  self.assertEqual(builder.build(),(ROOT/'Source/TheUnmadeGame/Public/World/UnmadeAdventureDepthRegistry.h').read_text())
 def test_duplicate_combat_declaration_regression(self):
  s=(ROOT/'Source/TheUnmadeGame/Public/Combat/UnmadeCombatComponent.h').read_text();self.assertEqual(s.count('bool IsGuarding('),1)
 def test_choice_and_resource_receipts(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp').read_text();self.assertIn('Depth=before;DepthChallenges=beforeChallenge',s);self.assertLess(s.index('WriteWorldSnapshot(p)'),s.index('Heal(35)'));self.assertIn('Person(GetWorld(),a,0)',s);self.assertIn('DepthPendingChoice!=action',s);self.assertIn('DepthConfirmUntil',s)
  h=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp').read_text();self.assertIn('if(bDepthSaveRejected)return false;',h);self.assertIn('if(Save->bHasAdventureDepth)',h)
 def test_physical_evidence_and_cache_presence(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp').read_text();self.assertIn('if(DepthEvidenceActors[a].IsValid())Add',s);self.assertIn('if(DepthCacheActors[a].IsValid())Add',s)
 def test_gameplay_notices_reach_interface(self):
  for p in (ROOT/'Source/TheUnmadeGame/Private').rglob('*.cpp'):
   if p.name!='UnmadeRpgFeedback.cpp':self.assertNotIn('GEngine->AddOnScreenDebugMessage',p.read_text(),str(p))
 def test_native_menu_and_control_recovery(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp').read_text()
  literals=re.findall(r'"(?:\\.|[^"\\])*"',s)
  self.assertTrue(literals)
  self.assertTrue(all('\n' not in literal for literal in literals),'ordinary C++ strings cannot contain literal line breaks')
  for x in ['OnDialogueReady.AddUniqueDynamic','OnDialogueReady.RemoveDynamic','SetIgnoreMoveInput(false)','SetWidgetToFocus','UScrollBox','DescribeUnansweredRoad','SelectDepthTechnique']:self.assertIn(x,s)
  p=(ROOT/'Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp').read_text();self.assertIn('if(!RpgWidget)',p);self.assertIn('Combat->Dodge',p);self.assertIn('FMath::Square(Style.reach)',p)
