"""Behavioral regression of the nine physically operated route puzzles."""
import subprocess
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


class NineRealmRoutePlayability(unittest.TestCase):
    def test_every_realm_has_recoverable_contextual_puzzle(self):
        code = r"""
#include "World/UnmadeAdventureDepthRules.h"
#include <cassert>
#include <string>
#include <set>
using namespace UnmadeCore;
int main() {
 std::set<std::string> hints;
 for(int a=0;a<9;++a) {
   const char* hint=ChallengeInstructions(a);
   assert(hint&&std::string(hint).size()>25);
   hints.insert(hint);
 }
 assert(hints.size()==9);
 assert(!ChallengeInstructions(-1));
 RealmChallenge crossings;
 assert(crossings.Attempt(0,0,1,false,true).outcome==ChallengeOutcome::Progress);
 assert(crossings.Attempt(0,1,2,false,true).outcome==ChallengeOutcome::Reset);
 assert(crossings.Attempt(0,0,3,false,true).outcome==ChallengeOutcome::Progress);
 assert(crossings.Attempt(0,2,4,false,true).outcome==ChallengeOutcome::Progress);
 assert(crossings.Attempt(0,1,5,false,true).outcome==ChallengeOutcome::Complete);
 RealmChallenge tide;
 tide.Attempt(1,0,0,false,true);
 assert(tide.Attempt(1,2,2,false,true).outcome==ChallengeOutcome::WaitForWindow);
 assert(tide.TidePhase(5)==5);
 tide.Attempt(1,2,3,false,true);
 assert(tide.Attempt(1,2,5,false,true).outcome==ChallengeOutcome::Complete);
 RealmChallenge kiln;
 kiln.Attempt(2,0,0,false,true);
 kiln.Attempt(2,1,1,false,true);
 auto expired=kiln.Attempt(2,2,20,false,true);
 assert(expired.outcome==ChallengeOutcome::Reset);
 assert(expired.secondsRemaining>=17);
 assert(kiln.Attempt(2,0,21,false,true).outcome==ChallengeOutcome::Progress);
 assert(kiln.Attempt(2,1,22,false,true).outcome==ChallengeOutcome::Complete);
 RealmChallenge buoy;
 buoy.Attempt(3,0,2,false,true);
 assert(buoy.Attempt(3,1,3,false,true).outcome==ChallengeOutcome::Complete);
 RealmChallenge choir;
 assert(choir.Attempt(4,0,3,false,true).outcome==ChallengeOutcome::NeedElevation);
 assert(choir.Mask()==0);
 choir.Attempt(4,0,4,true,true);choir.Attempt(4,1,5,true,true);
 assert(choir.Attempt(4,2,6,true,true).outcome==ChallengeOutcome::Complete);
 RealmChallenge seam;
 assert(seam.Attempt(5,1,2,false,true).outcome==ChallengeOutcome::Reset);
 seam.Attempt(5,0,3,false,true);seam.Attempt(5,1,4,false,true);
 assert(seam.Attempt(5,2,5,false,true).outcome==ChallengeOutcome::Complete);
 RealmChallenge census;
 census.Attempt(6,0,2,false,true);
 assert(census.Attempt(6,1,3,false,false).outcome==ChallengeOutcome::Observed);
 assert(census.Mask()==0);
 census.Attempt(6,0,4,false,true);census.Attempt(6,1,5,false,true);
 assert(census.Attempt(6,2,6,false,true).outcome==ChallengeOutcome::Complete);
 RealmChallenge root;
 root.Attempt(7,2,2,false,true);root.Attempt(7,0,3,false,true);
 assert(root.Attempt(7,1,4,false,true).outcome==ChallengeOutcome::Complete);
 RealmChallenge threshold;
 threshold.Attempt(8,0,2,false,true);threshold.Attempt(8,1,3,false,true);
 threshold.Attempt(8,0,4,false,true);
 assert(threshold.Attempt(8,2,5,false,true).outcome==ChallengeOutcome::Complete);
 assert(threshold.Attempt(8,3,6,false,true).outcome==ChallengeOutcome::Invalid);
}
"""
        with tempfile.TemporaryDirectory() as tmp:
            src=Path(tmp)/"challenge.cpp"
            exe=Path(tmp)/"challenge"
            src.write_text(code,encoding="utf-8")
            compiler=subprocess.run(["g++","-std=c++17","-Wall","-Wextra","-Werror",
                "-I",str(ROOT/"Source/TheUnmadeGame/Public"),str(src),"-o",str(exe)],
                capture_output=True,text=True)
            self.assertEqual(compiler.returncode,0,compiler.stderr)
            run=subprocess.run([str(exe)],capture_output=True,text=True)
            self.assertEqual(run.returncode,0,run.stderr)

    def test_native_controls_report_what_happened_and_show_current_clock(self):
        path=ROOT/"Source/TheUnmadeGame/Private/World/UnmadeAdventureDepth.cpp"
        src=path.read_text()
        self.assertIn("DepthChallenges[arc].Attempt(",src)
        self.assertIn("ChallengeInstructions(arc)",src)
        self.assertIn("ChallengeOutcome::NeedElevation",src)
        self.assertIn("ChallengeOutcome::Observed",src)
        self.assertIn("ChallengeOutcome::WaitForWindow",src)
        self.assertIn("TidePhase(",src)
        self.assertIn("SecondsRemaining(",src)
        self.assertIn("DepthChallengeSigns[a]",src)


if __name__ == "__main__":
    unittest.main()
