"""Source integration contracts; Unreal runtime assertions live in C++ automation tests."""
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

def source(path: str) -> str:
    return (ROOT / path).read_text(encoding="utf-8")

class LocalDialogueSourceTests(unittest.TestCase):
    def test_local_endpoint_and_modules_only(self):
        code = source("Source/TheUnmadeGame/Private/Dialogue/UnmadeLocalDialogueSubsystem.cpp")
        build = source("Source/TheUnmadeGame/TheUnmadeGame.Build.cs")
        self.assertIn("127.0.0.1:11434/api/chat", code)
        self.assertIn('"HTTP"', build)
        self.assertIn('"Json"', build)
        self.assertIn('"JsonUtilities"', build)
        self.assertIn("SetTimeout(", code)
        self.assertIn('SetBoolField(TEXT("stream"), false)', code)
        self.assertNotIn("api.openai.com", code)

    def test_fail_closed_without_ai_and_no_game_mutation(self):
        code = source("Source/TheUnmadeGame/Private/Dialogue/UnmadeLocalDialogueSubsystem.cpp")
        self.assertIn("bEnableLocalModel", code)
        self.assertIn("FallbackLine", code)
        self.assertIn("bRequestInFlight", code)
        self.assertIn("ProcessRequest()", code)
        self.assertIn("OnDialogueReady.Broadcast", code)
        self.assertNotIn("GrantReward(", code)
        self.assertNotIn("ApplyWorldChange(", code)

    def test_memory_context_has_provenance_and_bounded_size(self):
        code = source("Source/TheUnmadeGame/Private/Dialogue/UnmadeDialoguePolicy.cpp")
        self.assertIn("EUnmadeEvidenceKind::Rumor", code)
        self.assertIn("SpeakerId", code)
        self.assertIn("GetObservations()", code)
        self.assertIn("MaxContextEvents", code)
        self.assertIn("TryExtractLine", code)

    def test_model_uses_authored_characterization_with_a_hard_knowledge_limit(self):
        code = source("Source/TheUnmadeGame/Private/Dialogue/UnmadeLocalDialogueSubsystem.cpp")
        self.assertIn("GetDialogueCharacterContext()", code)
        self.assertIn("Character desire and fear guide voice only", code)
        self.assertIn("hard limit on factual claims", code)

    def test_followup_context_is_bounded_and_not_authoritative(self):
        code = source("Source/TheUnmadeGame/Private/Dialogue/UnmadeLocalDialogueSubsystem.cpp")
        self.assertIn("MaxConversationHistoryCharacters = 1200", code)
        self.assertIn("PreviousDialogue.Right(MaxConversationHistoryCharacters)", code)
        self.assertIn("untrusted dialogue, not evidence", code)

    def test_player_uses_subsystem_with_fallback(self):
        player = source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        self.assertIn("GetSubsystem<UUnmadeLocalDialogueSubsystem>()", player)
        self.assertIn("RequestDialogue(Target,", player)
        self.assertIn("GetReactionText()", player)

    def test_engine_tests_cover_valid_and_broken_payloads(self):
        tests = source("Source/TheUnmadeGame/Private/Tests/LocalDialogueAutomationTest.cpp")
        self.assertIn("Unmade.LocalDialogue.Parse", tests)
        self.assertIn("Unmade.LocalDialogue.Provenance", tests)
        for term in ("valid response", "malformed", "overlong", "rumor", "witnessed"):
            self.assertIn(term, tests)

if __name__ == "__main__":
    unittest.main()
