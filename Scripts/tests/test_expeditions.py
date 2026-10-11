import json,unittest,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'Scripts'))
from build_frontier_adventures import build
class ExpeditionContracts(unittest.TestCase):
 def test_generated_registry_matches(self):self.assertEqual(build(),(ROOT/'Source/TheUnmadeGame/Public/World/UnmadeExpeditionRegistry.h').read_text())
 def test_nine_unique_adventures_and_eighteen_sites(self):
  a=json.loads((ROOT/'Authoring/frontier_adventures_v14.json').read_text())['arcs'];self.assertEqual(len(a),9)
  for key in ['realm','frontier','refuge','dungeon','boss','weapon','armor','technique']:self.assertEqual(len({x[key] for x in a}),9)
  self.assertEqual(len({p for x in a for p in x['people']}),45);self.assertTrue(all(len(x['clues'])==3 for x in a))
 def test_real_combat_and_atomic_save_hooks(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp').read_text();self.assertIn('Combat->TryStrikeTarget(P->GetCombat()',s);self.assertIn('ExpeditionPattern.Step',s);self.assertIn('SetAttackDamage',s)
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp').read_text();self.assertIn('!e->GetCombat()->IsDefeated()',s);self.assertIn('Expeditions=before',s);self.assertIn('ExpeditionReturnGates',s);self.assertIn('SetExpeditionBonuses',s)
 def test_optional_snapshot_and_input_reachability(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp').read_text();self.assertIn('if(Save->bHasExpeditions)',s);self.assertIn('if(bExpeditionSaveRejected)return false',s)
  s=(ROOT/'Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp').read_text();self.assertIn('TryExpeditionInteraction',s);self.assertIn('TryExpeditionAbility',s);self.assertIn('GetExpeditionJournal',s)
