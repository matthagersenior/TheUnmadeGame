"""The road must physically change again after earned restoration and claimant resolution."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]

class SecondReturnDuties(unittest.TestCase):
    def test_each_realm_has_earned_second_visit_duties_not_automatic_disclosures(self):
        code=r'''
#include "World/UnmadePublicAccordDutyRules.h"
#include <cassert>
#include <set>
#include <utility>
using namespace UnmadeCore;
int main(){
 std::set<std::pair<int,int>> refugeSites,watchSites;
 for(int a=0;a<9;++a){
  auto reserve=PublicAccordDuty(a,0,1,true,3,false,true);
  auto restored=PublicAccordDuty(a,0,1,true,3,false,true,false,true);
  auto hearing=PublicAccordDuty(a,0,2,true,3,false,true);
  auto claimant=PublicAccordDuty(a,0,2,true,3,false,true,true,false);
  assert(reserve.active&&hearing.active&&restored.active&&claimant.active);
  assert(restored.duty==AccordDuty::Sanctuary);
  assert(claimant.duty==AccordDuty::RoadWatch);
  assert(restored.dx!=reserve.dx||restored.dy!=reserve.dy);
  assert(claimant.dx!=hearing.dx||claimant.dy!=hearing.dy);
  refugeSites.insert({restored.dx,restored.dy});
  watchSites.insert({claimant.dx,claimant.dy});
  assert(PublicAccordDuty(a,0,1,true,3,false,true,true,false).duty==AccordDuty::Reserve);
  assert(PublicAccordDuty(a,0,2,true,3,false,true,false,true).duty==AccordDuty::Hearing);
  for(int n=0;n<5;++n){
   for(int branch=1;branch<=2;++branch){
    auto duty=PublicAccordDuty(a,n,branch,true,3,false,true,true,true);
    assert(duty.active);
    const int x=-1800+duty.dx, y=300+duty.dy;
    assert(x*x+y*y < 1850*1850);
   }
  }
  assert(!PublicAccordDuty(a,0,2,true,3,true,true,true,true).active);
  assert(!PublicAccordDuty(a,0,2,true,3,false,false,true,true).active);
  assert(!PublicAccordDuty(a,1,1,true,0,false,true,true,true).active);
  assert(!PublicAccordDuty(a,2,2,true,1,false,true,true,true).active);
 }
 assert(refugeSites.size()==9&&watchSites.size()==9);
}
'''
        with tempfile.TemporaryDirectory() as temp:
            path=Path(temp)/"test.cpp";binary=Path(temp)/"test"
            path.write_text(code,encoding="utf-8")
            build=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",
                 str(ROOT/"Source/TheUnmadeGame/Public"),str(path),"-o",str(binary)],
                 capture_output=True,text=True)
            self.assertEqual(build.returncode,0,build.stderr)
            run=subprocess.run([str(binary)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)
    def test_live_return_path_consults_earned_receipts_and_changes_motions(self):
        native=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp").read_text()
        section=native.split("bool AUnmadePrototypeHub::GetPublicAccordRoutineTarget(",1)[1].split("FName AUnmadePrototypeHub::GetPublicAccordWorkCue(",1)[0]
        self.assertIn("Depth.Arc(a)->counterThreat",section)
        self.assertIn("Identity.Arc(a)->repair==7",section)
        self.assertIn("PublicAccordDuty(",section)
        cue=native.split("FName AUnmadePrototypeHub::GetPublicAccordWorkCue(",1)[1].split("bool AUnmadePrototypeHub::TryDepthInteraction",1)[0]
        self.assertIn("FName(TEXT(\"Gesture\"))",cue)
        self.assertIn("FName(TEXT(\"Inspect\"))",cue)
        self.assertIn("FName(TEXT(\"Repair\"))",cue)
        self.assertIn("Depth.Arc(a)->counterThreat",cue)
        self.assertIn("SANCTUARY DUTY",native)
        self.assertIn("ROAD WATCH",native)

if __name__=="__main__":unittest.main()
