"""Cinderhold discovery must earn a distinct, saved social route without free victory."""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

def source(path):
    return (ROOT / path).read_text(encoding="utf-8")

class DiscoveryAccordTest(unittest.TestCase):
    def test_saved_alternative_route_requires_evidence_and_local_witnesses(self):
        cpp = r'''
#include "World/UnmadeCinderholdAdventureRules.h"
#include "World/UnmadeRememberedRoadRules.h"
#include <cassert>
using namespace UnmadeCore;
int main() {
 CinderholdAdventure a;
 assert(!a.ChooseSharedAccord(true));
 assert(a.Remember(BesHeard,1));
 assert(a.Remember(VardaBriefed,1));
 assert(a.Inspect(SlagClue::Tracks,1));
 assert(a.Inspect(SlagClue::ServiceLog,1));
 assert(a.Inspect(SlagClue::Cadence,1));
 assert(!a.ChooseSharedAccord(true)); // A living elder must be quieted first.
 assert(a.OpenVent(10));
 assert(a.Quiet(11,1));
 assert(!a.ChooseSharedAccord(false)); // Other-realm proof cannot be invented.
 assert(a.ChooseSharedAccord(true)); // The player entered the real fracture chamber.
 assert(a.Save().route==static_cast<int>(SlagRoute::SharedKiln));
 assert(a.KilnPathOpen(false) && a.KilnPathOpen(true));
 assert(a.ArchivePathOpen());
 assert(!a.Choose(SlagRoute::VentOpen));
 assert(a.Remember(VardaReported,2));
 assert(a.Remember(BesReported,2));
 CinderholdAdventure again;
 assert(again.Restore(a.Save(),3));
 assert(again.KilnPathOpen(false) && again.ArchivePathOpen());
 auto bad=a.Save();bad.resolution=1;
 assert(!again.Restore(bad,3) && again.Save().route==4);
 bad=a.Save();bad.memories&=~VardaBriefed;
 assert(!again.Restore(bad,3) && again.Save().route==4);
 bad=a.Save();bad.route=5;
 assert(!again.Restore(bad,3));
 CinderholdAdventure combat;
 assert(combat.Defeat(1));
 assert(!combat.ChooseSharedAccord(true));
 assert(combat.Choose(SlagRoute::VentSealed));
 assert(!combat.KilnPathOpen(false) && combat.ArchivePathOpen());
 RememberedRoads roads;
 assert(roads.Brief(4,1));
 for(int c=0;c<3;++c)assert(roads.Inspect(4,c,1));
 assert(roads.Cool(4,10));
 assert(roads.Resolve(4,2,true,false,11));
 assert(!roads.Choose(4,4)); // No generic upgrade from cargo board.
 assert(!roads.InheritCinderholdAccord(4,false));
 assert(roads.InheritCinderholdAccord(4,again.Save().route==4));
 assert(roads.LaneOpen(4,false) && roads.RecessOpen(4));
 RememberedRoads restored;
 assert(restored.Restore(roads.Pack(),2));
 assert(restored.LaneOpen(4,false) && restored.RecessOpen(4));
 auto invalid=roads.Pack();invalid[1+3*10+3]=4; // No shared accord in another city.
 RememberedRoads rejected;assert(!rejected.Restore(invalid,2));
}
'''
        with tempfile.TemporaryDirectory() as directory:
            src=Path(directory)/"case.cpp"
            binary=Path(directory)/"case"
            src.write_text(cpp,encoding="utf-8")
            build=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror",
                "-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(src),"-o",str(binary)],
                text=True,capture_output=True)
            self.assertEqual(build.returncode,0,build.stderr)
            execute=subprocess.run([str(binary)],text=True,capture_output=True)
            self.assertEqual(execute.returncode,0,execute.stderr)

    def test_real_interaction_world_gates_and_attributed_social_return(self):
        scene=source("Source/TheUnmadeGame/Private/World/UnmadeCinderholdAdventure.cpp")
        roads=source("Source/TheUnmadeGame/Private/World/UnmadeRememberedRoads.cpp")
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        validator=source("Source/TheUnmadeGame/Public/Save/UnmadeCombinedSaveRules.h")
        for item in ("Cinderhold.Adventure.SharedKilnAccord",
                     "ChooseSharedAccord(", "Expeditions.Arc(2)->rooms&1",
                     "CommitCinderhold(Before)", "Cinderhold.Has(UnmadeCore::VardaReported)",
                     "Cinderhold.Has(UnmadeCore::BesReported)"):
            self.assertIn(item,scene)
        self.assertIn("InheritCinderholdAccord",roads)
        self.assertIn("Cinderhold.Save().route==4",hub)
        self.assertIn("Cinderhold.Save().route==4",validator)
        self.assertIn("bExpeditionSaveRejected",hub)
        self.assertNotIn("LivingTrades.Restore",scene)
        self.assertNotIn("Materials.Restore",scene)

if __name__=="__main__":
    unittest.main()
