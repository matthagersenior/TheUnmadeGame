"""End-to-end source contracts for eight non-Cinderhold discovery/social aftermath arcs."""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
def read(name): return (ROOT/name).read_text(encoding="utf-8")

class EightRealmCivicConsequences(unittest.TestCase):
 def test_real_earned_evidence_both_branches_and_reload(self):
  cpp=r'''
#include "World/UnmadeDiscoveryCivicRules.h"
#include <cassert>
#include <cstring>
#include <set>
#include <string>
using namespace UnmadeCore;
int main() {
 Expeditions e;
 RealmAftermathChronicle aftermath;
 std::set<std::string> texts;
 for(int a=0;a<9;++a){
  if(a==2)continue; // Cinderhold's special shared-kiln path is preserved.
  if(a==1){ // Saltwake keeps its existing reachable frontier aftermath.
   assert(!CivicUsesExpeditionAftermath(a));
   assert(CivicSaltwakeWitnessedDiscovery(1,7,1));
   assert(!CivicSaltwakeWitnessedDiscovery(0,7,1));
   assert(!CivicSaltwakeWitnessedDiscovery(1,0,1));
   continue;
  }
  assert(CivicUsesExpeditionAftermath(a));
  const auto* spec=CivicSite(a);assert(spec&&spec->initiator&&spec->careWitness&&spec->truthWitness);
  assert(std::strcmp(spec->careWitness,spec->truthWitness)!=0);
  assert(!CivicEvidenceReady(a,e.Arc(a)));
  assert(e.Brief(a));assert(e.Visit(a,0));
  assert(e.Enter(a,0));assert(!CivicEvidenceReady(a,e.Arc(a)));
  for(int room=1;room<=4;++room)assert(e.Enter(a,room));
  for(int c=0;c<3;++c)assert(e.Inspect(a,c));
  assert(!CivicEvidenceReady(a,e.Arc(a))); // All evidence alone cannot teleport a return.
  assert(e.Choose(a,2));assert(e.Enter(a,5));
  assert(e.Boss(a,false));assert(e.Return(a));
  assert(CivicEvidenceReady(a,e.Arc(a)));
  const Realm realm=static_cast<Realm>(a);
  assert(aftermath.Begin(realm,CivicEvidenceReady(a,e.Arc(a)),true,
                         spec->initiator,spec->initiator)==RealmAftermathResult::Started);
  assert(aftermath.Prepare(realm,RealmAftermathSpecs[a].mechanismSite)==RealmAftermathResult::Prepared);
  int choice=a%2+1;
  const char* marker=choice==1?RealmAftermathSpecs[a].evidenceForCare:RealmAftermathSpecs[a].evidenceForTruth;
  const char* witness=choice==1?spec->careWitness:spec->truthWitness;
  assert(aftermath.Inspect(realm,choice,marker)==RealmAftermathResult::EvidenceFound);
  assert(aftermath.Testify(realm,witness,witness)==RealmAftermathResult::Witnessed);
  assert(aftermath.Decide(realm,3-choice)==RealmAftermathResult::WrongChoice);
  assert(aftermath.Decide(realm,choice)==RealmAftermathResult::Resolved);
  texts.insert(aftermath.WorldConsequence(realm));
  assert(CivicWorkAuthorized(a,0,aftermath.Stage(realm),aftermath.Ending(realm),choice));
  assert(CivicWorkAuthorized(a,choice,aftermath.Stage(realm),aftermath.Ending(realm),choice));
  assert(!CivicWorkAuthorized(a,3-choice,aftermath.Stage(realm),aftermath.Ending(realm),choice));
 }
 assert(texts.size()==7);
 auto saved=aftermath.Snapshot();
 assert(CivicEvidenceValidOnRestore(saved,e,true));
 RealmAftermathChronicle again;assert(again.Restore(saved));
 for(int a:{0,3,4,5,6,7,8})assert(again.Ending(static_cast<Realm>(a))!=0);
 auto bad=saved;bad.stage[0]=4;bad.ending[0]=0;
 assert(!again.Restore(bad));
 assert(!CivicEvidenceValidOnRestore(saved,e,false));
 Expeditions noProof;
 assert(!CivicEvidenceValidOnRestore(saved,noProof,true));
 assert(!CivicUsesExpeditionAftermath(-1));
 assert(CivicSite(1)==nullptr&&CivicSite(2)==nullptr&&CivicSite(9)==nullptr);
}
'''
  with tempfile.TemporaryDirectory() as temp:
   source=Path(temp)/"civic.cpp";executable=Path(temp)/"civic"
   source.write_text(cpp)
   build=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror",
      "-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(source),"-o",str(executable)],
      capture_output=True,text=True)
   self.assertEqual(build.returncode,0,build.stderr)
   run=subprocess.run([str(executable)],capture_output=True,text=True)
   self.assertEqual(run.returncode,0,run.stderr)
 def test_all_seven_missing_realms_have_playable_mechanisms_and_mutually_exclusive_routes(self):
  expedition=read("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  world=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
  hub=read("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
  for token in ("CivicUsesExpeditionAftermath(a)","RealmAftermathSpecs[a].mechanismSite",
                "RealmAftermathSpecs[a].evidenceForCare",
                "RealmAftermathSpecs[a].evidenceForTruth",
                "Expedition.%d.CivicCareGate","Expedition.%d.CivicTruthGate"):
   self.assertIn(token,expedition)
  for token in ("CivicUsesExpeditionAftermath(i)","CivicEvidenceReady",
                "CivicEvidenceValidOnRestore","RealmAftermath.Begin(",
                "RealmAftermath.Testify(", "CivicCareGate", "CivicTruthGate"):
   self.assertIn(token,world)
  self.assertIn("GetDiscoveryCivicRoutineTarget",hub)
 def test_witnesses_have_local_reactions_and_real_daytime_work_routes(self):
  npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
  expedition=read("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp")
  world=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
  for term in ("GetDiscoveryCivicResidentLine", "GetDiscoveryCivicRoutineTarget"):
   self.assertIn(term,npc)
   self.assertIn(term,world)
  self.assertIn("MaterialDuty::Regular",npc)
  self.assertIn("CivicSaltwakeWitnessedDiscovery",expedition)
 def test_saved_provenance_preflight_and_no_invented_resources(self):
  validator=read("Source/TheUnmadeGame/Public/Save/UnmadeCombinedSaveRules.h")
  world=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
  self.assertIn("CivicEvidenceValidOnRestore",validator)
  self.assertIn("CivicEvidenceValidOnRestore",world)
  self.assertIn("bRealmAftermathSaveRejected",world)
  self.assertNotIn("LivingTrades.Restore",read("Source/TheUnmadeGame/Private/World/UnmadeExpeditions.cpp"))
if __name__=="__main__": unittest.main()
