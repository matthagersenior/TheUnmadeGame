"""Violence and mercy have bounded, evidence-proven consequences in the NEXT realm.

The response must not teleport through plot knowledge, invent a killing, add
a hidden reward, or punish a player through an untelegraphed damage increase.
"""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]

class NineRealmCombatRepercussions(unittest.TestCase):
 def test_nine_received_battle_reports_change_neighboring_risks_and_review_matters(self):
  cpp=r"""
#include "World/UnmadeCrossRealmCombatRules.h"
#include <cassert>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
 std::set<std::string> labels,signals;
 for(int dst=0;dst<9;++dst){
  const int src=(dst+8)%9;
  assert(CrossRealmCombatSource(dst)==src);
  assert(CrossRealmDestination(src)==dst);
  auto noReport=CrossRealmCombatResponse(dst,1,255,0,true,false,false,true);
  assert(noReport.kind==CrossRealmCombatKind::Unknown && noReport.hazardDelta==0);
  auto unreturned=CrossRealmCombatResponse(dst,1,255,0,false,true,false,true);
  assert(unreturned.kind==CrossRealmCombatKind::Unknown);
  auto unmade=CrossRealmCombatResponse(dst,0,255,0,true,true,false,true);
  assert(unmade.kind==CrossRealmCombatKind::Unknown);
  auto invalid=CrossRealmCombatResponse(dst,1,255,0,true,true,false,false);
  assert(invalid.kind==CrossRealmCombatKind::Unknown);
  auto death=CrossRealmCombatResponse(dst,1,255,0,true,true,false,true);
  auto restraint=CrossRealmCombatResponse(dst,2,0,3,true,true,false,true);
  auto sparse=CrossRealmCombatResponse(dst,1,0,3,true,true,false,true);
  assert(death.kind==CrossRealmCombatKind::BorderAlert);
  assert(death.hazardDelta>=2&&death.hazardDelta<=5);
  assert(sparse.hazardDelta>=1&&sparse.hazardDelta<death.hazardDelta);
  assert(sparse.rescued==2 && death.rescued==0);
  assert(CrossRealmCombatResponse(dst,1,0,2,true,true,false,true).rescued==1);
  assert(restraint.kind==CrossRealmCombatKind::OpenWitnessPass);
  auto costlyMercy=CrossRealmCombatResponse(dst,2,255,0,true,true,false,true);
  auto costlyMercyReviewed=CrossRealmCombatResponse(dst,2,255,0,true,true,true,true);
  assert(costlyMercy.kind==CrossRealmCombatKind::CasualtyVigil);
  assert(costlyMercy.hazardDelta>0);
  assert(costlyMercyReviewed.kind==CrossRealmCombatKind::ReviewedVigil);
  assert(costlyMercyReviewed.hazardDelta<costlyMercy.hazardDelta);
  assert(costlyMercyReviewed.hazardDelta>=0);
  assert(restraint.hazardDelta<=-1&&restraint.hazardDelta>=-4);
  auto moderated=CrossRealmCombatResponse(dst,1,255,0,true,true,true,true);
  auto verifiedPass=CrossRealmCombatResponse(dst,2,0,3,true,true,true,true);
  assert(moderated.kind==CrossRealmCombatKind::ReviewedAlert);
  assert(moderated.hazardDelta<death.hazardDelta);
  assert(moderated.hazardDelta>=0);
  assert(verifiedPass.kind==CrossRealmCombatKind::ReviewedPass);
  assert(verifiedPass.hazardDelta>=restraint.hazardDelta);
  assert(verifiedPass.hazardDelta<=0);
  labels.insert(CrossRealmCombatProfiles[dst].frontier);
  signals.insert(CrossRealmCombatProfiles[dst].warning);
  assert(12+dst+death.hazardDelta<=25);
  assert(12+dst+restraint.hazardDelta>=6);
 }
 assert(labels.size()==9 && signals.size()==9);
 assert(CrossRealmCombatResponse(-1,1,0,0,true,true,false,true).kind==CrossRealmCombatKind::Unknown);
 assert(CrossRealmCombatResponse(9,1,0,0,true,true,false,true).kind==CrossRealmCombatKind::Unknown);
 assert(CrossRealmCombatResponse(0,3,0,0,true,true,false,true).kind==CrossRealmCombatKind::Unknown);
 assert(CrossRealmCombatResponse(0,2,256,0,true,true,false,true).kind==CrossRealmCombatKind::Unknown);
 assert(CrossRealmCombatResponse(0,2,1,4,true,true,false,true).kind==CrossRealmCombatKind::Unknown);
}
"""
  with tempfile.TemporaryDirectory() as d:
   path=Path(d)/"test.cpp"; exe=Path(d)/"test"
   path.write_text(cpp,encoding="utf-8")
   build=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror",
      "-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(path),"-o",str(exe)],capture_output=True,text=True)
   self.assertEqual(build.returncode,0,build.stderr)
   ran=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(ran.returncode,0,ran.stderr)
 def test_battle_facts_are_from_saved_witnessed_origin_and_hand_delivered_record(self):
  native=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp").read_text()
  depth=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp").read_text()
  hub=(ROOT/"Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h").read_text()
  for needle in ("CrossRealmCombatResponse(","CrossRealmCombatSource(a)",
    "CrossRealmCarry.Stage(source)","Expeditions.Arc(source)->boss",
    "Expeditions.Arc(source)->kills","Expeditions.Arc(source)->rescued",
    "Expeditions.Arc(source)->returned","CrossRealmCombatProfiles[a].warning",
    "ReceiveHazardPulse(","response.hazardDelta"):
   self.assertIn(needle,native)
  self.assertIn("CrossRealmCombatSigns",hub)
  self.assertIn("CrossRealmCombatSigns.SetNum(9)",native)
  self.assertIn("CrossRealmCombatSigns[a]->GetTextRender()->SetText(",native)
  self.assertIn("GetCrossRealmCombatRecipientLine(",depth)
  self.assertIn("BATTLE TESTIMONY:",depth)
  self.assertIn("stage==UnmadeCore::CarryStage::Carrying",depth)
  self.assertIn("nothing has changed there yet",depth)

  self.assertIn("CrossRealmCombatResponse(",depth)
 def test_chart_and_comparison_can_deescalate_but_not_spawn_unearned_outcomes(self):
  native=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp").read_text()
  self.assertIn("Identity.Arc(a)->chart!=0",native)
  self.assertIn("CarryStage::Reviewed",native)
  self.assertIn("CrossRealmCombatKind::Unknown",native)
  self.assertNotIn("Expeditions.Boss(source",native)
  self.assertNotIn("Expeditions.Rescue(source",native)
  self.assertNotIn("CrossRealmCarry.Deliver(source",native)
if __name__=="__main__":unittest.main()
