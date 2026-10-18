"""Playable return-road accord consequences: timed NPC duties and physical inspection."""
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]

class AccordReturnDuties(unittest.TestCase):
    def test_all_nine_have_distinct_branch_worksites_and_fail_closed(self):
        code=r'''
#include "World/UnmadePublicAccordDutyRules.h"
#include <cassert>
#include <set>
#include <utility>
using namespace UnmadeCore;
int main(){
 std::set<std::pair<int,int>> sheltered,publicDocket;
 for(int a=0;a<9;++a){
  auto care=PublicAccordDuty(a,0,1,true,3,false,true);
  auto hearing=PublicAccordDuty(a,0,2,true,3,false,true);
  assert(care.active&&hearing.active);
  assert(care.duty==AccordDuty::Reserve && hearing.duty==AccordDuty::Hearing);
  assert(care.dx!=hearing.dx||care.dy!=hearing.dy);
  sheltered.insert({care.dx,care.dy});
  publicDocket.insert({hearing.dx,hearing.dy});
  for(int n=0;n<5;++n){
   auto tasked=PublicAccordDuty(a,n,1,true,3,false,true);
   assert(tasked.active);
   assert(!PublicAccordDuty(a,n,1,false,3,false,true).active);
   assert(!PublicAccordDuty(a,n,0,true,3,false,true).active);
   assert(!PublicAccordDuty(a,n,1,true,3,true,true).active);
   assert(!PublicAccordDuty(a,n,1,true,3,false,false).active);
  }
  assert(!PublicAccordDuty(a,1,1,true,0,false,true).active);
  assert(!PublicAccordDuty(a,2,2,true,1,false,true).active);
  assert(PublicAccordDuty(a,1,1,true,1,false,true).active);
  assert(PublicAccordDuty(a,2,2,true,2,false,true).active);
 }
 assert(sheltered.size()==9 && publicDocket.size()==9);
 assert(!PublicAccordDuty(-1,0,1,true,3,false,true).active);
 assert(!PublicAccordDuty(9,0,1,true,3,false,true).active);
 assert(!PublicAccordDuty(0,5,1,true,3,false,true).active);
 assert(!PublicAccordDuty(0,0,3,true,3,false,true).active);
}
'''
        with tempfile.TemporaryDirectory() as temp:
            source=Path(temp)/"test.cpp";binary=Path(temp)/"test"
            source.write_text(code,encoding="utf-8")
            compile_result=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror",
                "-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(source),"-o",str(binary)],
                capture_output=True,text=True)
            self.assertEqual(compile_result.returncode,0,compile_result.stderr)
            run=subprocess.run([str(binary)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)

    def test_existing_npc_patrols_follow_actual_accord_worksite_only_during_routine(self):
        npc=(ROOT/"Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp").read_text()
        hub=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp").read_text()
        header=(ROOT/"Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h").read_text()
        self.assertIn("GetPublicAccordRoutineTarget(NpcId",npc)
        self.assertLess(npc.index("if (Motion == UnmadeCore::NpcMotion::Stay)"),
                        npc.index("GetPublicAccordRoutineTarget(NpcId"))
        self.assertLess(npc.index("GetPublicAccordRoutineTarget(NpcId"),
                        npc.index("SteerNpc("))
        self.assertIn("GetPublicAccordRoutineTarget(FName",header)
        self.assertIn("PublicAccordDuty(a,n,",hub)
        self.assertIn("Expeditions.Arc(a)->returned",hub)
        self.assertIn("!bDepthSaveRejected",hub)
        self.assertIn("!bExpeditionSaveRejected",hub)
        self.assertIn("Shop(a,0,0)",hub)
        self.assertNotIn("SetActorLocation(",hub.split("bool AUnmadePrototypeHub::GetPublicAccordRoutineTarget",1)[1].split("bool AUnmadePrototypeHub::TryDepthInteraction",1)[0])

    def test_saved_branch_notice_is_physically_inspectable_without_rewards(self):
        hub=(ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp").read_text()
        self.assertIn("DepthAccordSigns[a].IsValid()",hub)
        self.assertIn("Add(a,6,0,",hub)
        self.assertIn("if(kind==6)",hub)
        self.assertIn("Depth.Arc(arc)->counterThreat",hub)
        self.assertIn("Expeditions.Arc(a)->returned",hub)
        self.assertIn("E  Inspect public accord",hub)
        interaction=hub.split("if(kind==6)",1)[1].split("if(kind==5",1)[0]
        self.assertNotIn("WriteWorldSnapshot(",interaction)
        self.assertNotIn("Depth.Choose(",interaction)

if __name__=="__main__":unittest.main()
