"""The NPC conversation UI must expose player-led, non-mutating dialogue."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]


class InteractiveDialogueUiTests(unittest.TestCase):
    def read(self, path):
        return (ROOT / path).read_text(encoding="utf-8")

    def test_player_interaction_opens_conversation_instead_of_sending_hello(self):
        player = self.read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        interaction = player.split("void AUnmadeCharacter::Interact()", 1)[1].split(
            "void AUnmadeCharacter::OfferAid()", 1
        )[0]
        self.assertIn("RpgWidget->BeginNpcConversation(Target)", interaction)

    def test_conversation_has_three_topics_and_custom_question(self):
        header = self.read("Source/TheUnmadeGame/Public/UI/UnmadeRpgWidget.h")
        widget = self.read("Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp")
        for method in ("AskNpcMemory", "AskNpcConcern", "AskNpcWitnessed", "SubmitNpcQuestion"):
            self.assertIn(f"void {method}()", header)
            self.assertIn(f"UUnmadeRpgWidget::{method}()", widget)
        for prompt in (
            "What do you remember about me?",
            "What worries you today?",
            "What have you personally witnessed?",
        ):
            self.assertIn(prompt, widget)
        self.assertIn("ConversationInput", header)
        self.assertIn("ConversationTranscript", header)

    def test_model_reply_is_scoped_to_active_npc_and_never_changes_world_state(self):
        widget = self.read("Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp")
        handler = widget.split("void UUnmadeRpgWidget::NpcLine(", 1)[1].split("\n", 1)[0]
        self.assertIn("Id!=CurrentDialogueNpcId", handler)
        self.assertIn("AppendConversation(", handler)
        dialogue = self.read("Source/TheUnmadeGame/Private/Dialogue/UnmadeLocalDialogueSubsystem.cpp")
        self.assertIn("FallbackLine", dialogue)
        self.assertNotIn("ApplyWorldChange(", dialogue)
        self.assertNotIn("GrantReward(", dialogue)

    def test_engine_automation_keeps_multiple_npc_accounts_independent(self):
        engine = self.read("Source/TheUnmadeGame/Private/Tests/LocalDialogueAutomationTest.cpp")
        self.assertIn('"Unmade.LocalDialogue.MultiNPCProvenance"', engine)
        test = engine.split("FUnmadeLocalDialogueMultiNpcTest::RunTest", 1)[1]
        for invariant in (
            "Personally witnessed",
            "Heard rumor",
            "NOT witnessed",
            "correct",
            "cannot be downgraded",
        ):
            self.assertIn(invariant, test)

    def test_accessible_topics_render_in_modal_and_close_returns_to_game(self):
        header = self.read("Source/TheUnmadeGame/Public/UI/UnmadeRpgWidget.h")
        widget = self.read("Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp")
        self.assertIn("BeginNpcConversation(AUnmadeNpcCharacter* Npc)", header)
        self.assertIn("ConversationPanel", header)
        self.assertIn("SetKeyboardFocus", widget)
        self.assertIn("ConversationPanel->SetVisibility", widget)
        self.assertIn("CloseNpcConversation", header)

    def test_conversation_is_bounded_and_keeps_journal_history(self):
        widget = self.read("Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp")
        self.assertIn("SetMaxTextLength(400)", widget)
        self.assertIn("Question.Left(400)", widget)
        handler = widget.split("void UUnmadeRpgWidget::NpcLine(", 1)[1].split("\n", 1)[0]
        self.assertIn("Hub(GetWorld())", handler)
        self.assertIn("NotifyRpg", handler)
        self.assertIn("Page!=8", widget)

    def test_offline_topics_return_grounded_distinct_dialogue(self):
        policy = self.read("Source/TheUnmadeGame/Private/Dialogue/UnmadeDialoguePolicy.cpp")
        subsystem = self.read("Source/TheUnmadeGame/Private/Dialogue/UnmadeLocalDialogueSubsystem.cpp")
        self.assertIn("BuildOfflineReply", policy)
        self.assertIn("BuildOfflineReply", subsystem)
        self.assertIn("I saw it myself", policy)
        self.assertIn("I did not see it myself", policy)
        self.assertIn("Player.Helped", policy)
        self.assertIn("Unknown event IDs stay out", policy)

    def test_threefold_mechanism_line_has_no_duplicate_article_in_master_and_handoff(self):
        import json
        master = json.loads(self.read("Authoring/main_quest_interaction_contracts.json"))
        line = master["quests"][0]["steps"][2]["line"]
        handoff = self.read("Authoring/generated/main_quest_steps_unreal.csv")
        encyclopedia = self.read("docs/canon/encyclopedia/06_QUESTS_STORY_AND_ENDINGS.md")
        self.assertNotIn("The the unanswered interval", line)
        self.assertIn(line, handoff)
        self.assertIn(line, encyclopedia)


if __name__ == "__main__":
    unittest.main()
