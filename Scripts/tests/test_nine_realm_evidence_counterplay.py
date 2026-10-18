"""Physically earned evidence enables distinct tactical counterplay; it never solves a boss remotely."""
from pathlib import Path
import subprocess,tempfile,unittest
ROOT=Path(__file__).resolve().parents[2]
def read(path):return (ROOT/path).read_text(encoding="utf-8")
class EvidenceInterventions(unittest.TestCase):
 def test_all_nine_interventions_need_local_discovery_living_boss_and_cooldown(self):
  cpp=r'''
#include "World/UnmadeEvidenceCounterplayRules.h"
#include <cassert>
#include <set>
#include <string>
using namespace UnmadeCore;
int main(){
 std::set<std::string> names,consequences;
 for(int a=0;a<9;++a){
  auto s=EvidenceCounterplaySpecs[a];
  names.insert(s.name); consequences.insert(std::to_string(s.exposureSeconds)+"/"+std::to_string(s.coverSeconds));
  assert(s.exposureSeconds>=3 && s.exposureSeconds<=8);
  assert(s.coverSeconds>=0 && s.coverSeconds<=8);
  assert(s.cooldownSeconds>=20 && s.cooldownSeconds<=50);
  assert(s.offsetX!=0 && s.offsetY!=0);
  EvidenceCounterplay counter;
  assert(counter.Use(a,10,false,false,false,true)==EvidenceCounterplayResult::MissingProof);
  assert(counter.Use(a,10,true,false,false,true)==EvidenceCounterplayResult::Active);
  assert(EvidenceCounterplay{}.Use(a,10,false,true,false,true)==EvidenceCounterplayResult::Active);
  assert(counter.Use(a,11,false,true,false,true)==EvidenceCounterplayResult::Recovering);
  assert(counter.Use(a,25,false,true,false,true)==EvidenceCounterplayResult::Recovering);
  assert(counter.Use(a,100,false,true,false,true)==EvidenceCounterplayResult::Active);
  assert(counter.Use(a,101,false,true,true,true)==EvidenceCounterplayResult::NoLiveBoss);
  assert(counter.Use(a,101,false,true,false,false)==EvidenceCounterplayResult::NoLiveBoss);
  assert(counter.Use(a,102,true,true,false,false)==EvidenceCounterplayResult::NoLiveBoss);
  assert(counter.Use(a,-1,true,true,false,true)==EvidenceCounterplayResult::Invalid);
  assert(counter.Use(a,101,true,true,false,true)==EvidenceCounterplayResult::Invalid);
 }
 assert(names.size()==9);assert(consequences.size()>=5);
 EvidenceCounterplay x;
 assert(x.Use(-1,1,true,false,false,true)==EvidenceCounterplayResult::Invalid);
 assert(x.Use(9,1,true,false,false,true)==EvidenceCounterplayResult::Invalid);
 assert(x.Use(0,1,true,false,false,false)==EvidenceCounterplayResult::NoLiveBoss);
}
'''
  with tempfile.TemporaryDirectory() as directory:
   p=Path(directory)/"test.cpp";exe=Path(directory)/"test";p.write_text(cpp,encoding="utf8")
   c=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror","-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(p),"-o",str(exe)],capture_output=True,text=True)
   self.assertEqual(c.returncode,0,c.stderr);r=subprocess.run([str(exe)],capture_output=True,text=True)
   self.assertEqual(r.returncode,0,r.stderr)
 def test_each_real_arena_offers_a_physical_evidence_station_with_real_consequences(self):
  hub=read("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  hdr=read("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  for token in ("EvidenceCounterStation(a)","Candidate(a,13,0,","case 13:","EvidenceCounterplayResult::Active","EvidenceCounters[arc].Use(","Depth.Arc(arc)->evidence","t->clues==7","ExposeToFold(","ArenaShelterUntil[arc]","SetRiteWorldActorState(","ExpeditionEnemyInactive(arc,8)","counter.exposureSeconds","counter.coverSeconds"):
   self.assertIn(token,hub)
  self.assertIn("std::array<UnmadeCore::EvidenceCounterplay,9> EvidenceCounters",hdr)
  self.assertIn("ARENA DISCOVERY",hub)
  self.assertIn("Counterplay earned",hub)
  self.assertIn("No boss outcome or rescue was automatically saved",hub)
  start=hub.index("case 13:")
  end=hub.index("case 10:",start)
  segment=hub[start:end]
  for negative in ("Expeditions.Boss(","Expeditions.Rescue(","Expeditions.Choose(","WriteWorldSnapshot("):
   self.assertNotIn(negative,segment)
 def test_narrative_theme_and_optional_path_preserved(self):
  source=read("Source/TheUnmadeGame/Public/World/UnmadeEvidenceCounterplayRules.h")
  journal=read("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  for word in ("charter","pressure","kiln","counterfoil","choir","seam","census","graft","threshold"):
   self.assertIn(word,source.lower())
  self.assertIn("Existing free grounding lever",journal)
  self.assertIn("t->branch!=0",journal)
if __name__=="__main__":unittest.main()
