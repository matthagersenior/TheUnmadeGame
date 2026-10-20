import json,math,sys,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'Scripts'))
import build_remembered_roads
class RememberedRoadCoverage(unittest.TestCase):
 def setUp(self):self.d=json.loads((ROOT/'Authoring/remembered_roads_v13.json').read_text())
 def test_every_canonical_person_and_scout_once(self):
  profiles=json.loads((ROOT/'Authoring/resident_return_encounters.json').read_text())['residents'];new=self.d['residents'];ids={p['id'] for p in new}
  self.assertEqual(len(new),88);self.assertEqual(len(ids),88);self.assertEqual(len(profiles),88)
  self.assertEqual([p['id'] for p in profiles[:82]],[p['id'] for p in new[:82]])
  self.assertEqual({p['id'] for p in profiles[82:]},{p['id'] for p in new if '.scout.' in p['id']})
 def test_all_realms_cities_and_distinct_adventures(self):
  cities=self.d['cities'];realms=json.loads((ROOT/'Authoring/world_content_pack.json').read_text())['realms'];self.assertEqual({c['realm'] for c in cities},{r['id'] for r in realms});self.assertEqual(len(cities),11)
  for key in ['title','cargo','elder']:self.assertEqual(len({c[key] for c in cities}),11)
  for c in cities:
   self.assertNotEqual(c['destination'],c['index']);self.assertIn(c['destination'],range(11));self.assertEqual(len(c['clues']),3)
   self.assertTrue(any(p['id']==c['sender'] and p['city']==c['index'] for p in self.d['residents']))
 def test_current_registry_and_personal_thresholds(self):
  self.assertEqual(build_remembered_roads.build(),(ROOT/'Source/TheUnmadeGame/Public/World/UnmadeRememberedRoadRegistry.h').read_text())
  coords=set()
  for p in self.d['residents']:
   self.assertTrue(math.isfinite(p['home_x']) and math.isfinite(p['home_y']));self.assertNotIn((p['home_x'],p['home_y']),coords);coords.add((p['home_x'],p['home_y']))
   self.assertIn(p['name'],p['agrees']);self.assertIn(p['name'],p['disagrees']);self.assertIn(p['preference'],(1,2,3))
 def test_runtime_uses_actual_custody_and_actual_people(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadeRememberedRoads.cpp').read_text()
  for f in ['RememberedRoads.Pickup','RememberedRoads.Checkpoint','RememberedRoads.Deliver','RememberedRoads.Tell','RememberedRoads.VisitHome','ActualKeeper','Resident->GetActorLocation()','SpendRealityStrain(60)','RefundRealityStrain(60)','RememberedRoads=Before','RememberedRoads.Choose(City,Cinderhold.Save().route)','The First Cart Through keeps your saved Blackslag route','RoadCargoVisual','RoadLaneGates','RoadRecessGates']:self.assertIn(f,s)
  for f in ['AddCurrency','AddFuel','SetActorLocation(Player','PlanRealmRoute(','LoadGameFromSlot']:self.assertNotIn(f,s)
 def test_additive_save_and_actual_runtime_entry_points(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp').read_text();self.assertIn('if(bRoadSaveRejected)return false;',s);self.assertIn('RememberedRoads.Restore(Data,RestoredClock.DayIndex()+1)',s)
  p=(ROOT/'Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp').read_text()
  for f in ['TryRoadInteraction(this,ResidentDistSq)','TryRoadAbility(this,0)','TryRoadAbility(this,1)','TryRoadAbility(this,2','RecordRoadConversation(Target->GetStableId())','GetRoadJournal()']:self.assertIn(f,p)
