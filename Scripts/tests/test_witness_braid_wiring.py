"""Protect physical Witness Braid, evidence provenance, durable non-magical consequences."""
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def read(path): return (ROOT/path).read_text(encoding="utf-8")
class WitnessBraidWiring(unittest.TestCase):
    def test_required_evidence_is_physically_earned(self):
        rules=read("Source/TheUnmadeGame/Public/World/UnmadeWitnessBraidRules.h")
        for key in ("CALL.01","CALL.03","CALL.04","ReturnCount()>=1"):
            self.assertIn(key,rules)
        self.assertIn("saved.outcome!=0 && !Ready(evidence)",rules)
        self.assertIn("AlreadyCommitted",rules)
        self.assertIn("PUBLIC DOCKET",rules)
        self.assertIn("SHELTERED THREAD",rules)

    def test_player_controls_world_and_old_saves_survive(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        assert all(key in hub for key in (
            'Scope=FName("WitnessBraid.Crossings")',
            "bool AUnmadePrototypeHub::ResolveNearbyWitnessBraid(int32 Choice)",
            "WitnessBraid.Commit(Choice,WitnessEchoes)",
            "WitnessBraid.Restore(Before,WitnessEchoes)",
            "RefreshWitnessBraidWorld();",
            '"WitnessBraid.RefugeCord"', '"WitnessBraid.PublicDocket"',
            "bWitnessBraidRejected=true", "bWitnessEchoRejected || bWitnessBraidRejected",
            "Save->WitnessBraidOutcome=WitnessBraid.Snapshot().outcome"))
        self.assertIn("Hub->ResolveNearbyWitnessBraid",player)
        self.assertIn("CommitmentGate.Attempt(",player)
        self.assertIn("bHasWitnessBraidSnapshot=false",save)
        self.assertIn("WitnessBraidOutcome=0",save)
        self.assertIn("ReportLocalEvent(FName(\"World.WitnessBraidResolved\")",player)

    def test_canon_and_tests_follow_source(self):
        for path in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                     "docs/lore/living-world-continuity.md",
                     "docs/lore/CONTINUITY_LOG.md"):
            self.assertIn("Witness Braid",read(path))
        self.assertIn("Tests/world/witness_braid_test.cpp",
                      read(".github/workflows/static-checks.yml"))
        self.assertIn("test_witness_braid_wiring.py",
                      read("Scripts/first_pc_build_and_test.ps1"))
if __name__=="__main__": unittest.main()
