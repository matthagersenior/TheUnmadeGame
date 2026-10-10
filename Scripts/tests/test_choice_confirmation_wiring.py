"""No one-press irreversible resolution across faction, frontier or aftermath."""
import unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def load(p):return (ROOT/p).read_text(encoding="utf-8")
class ChoiceConfirmationSource(unittest.TestCase):
    def test_target_bound_confirmation_before_world_mutation(self):
        hub=load("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        header=load("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        player=load("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertIn("GetNearbyCommitPreview(",header)
        self.assertIn("GetNearbyCommitPreview(",hub)
        self.assertIn("CommitmentGate.Attempt(",player)
        self.assertIn("CommitmentGate.Cancel()",player)
        self.assertIn("CommitmentAttempt::Confirmed",player)
        self.assertIn("ResolveNearbyFaction(",player)
        self.assertIn("ResolveNearbyRealmAftermath(",player)
    def test_confirmation_in_github_native_suite(self):
        workflow=load(".github/workflows/static-checks.yml")
        self.assertIn("Tests/story/commitment_rules_test.cpp",workflow)
if __name__=="__main__":unittest.main()
