"""Guard first-hand codex, source-specific NPC memories and the actual player controls."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def source(path):return (ROOT/path).read_text(encoding="utf-8")
class ProvenanceWiring(unittest.TestCase):
    def test_evidence_revealed_only_after_personal_visit(self):
        rules=source("Source/TheUnmadeGame/Public/World/UnmadeEvidenceProvenanceRules.h")
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        hub_header=source("Source/TheUnmadeGame/Public/World/UnmadePrototypeHub.h")
        self.assertIn("if(!Seen)continue;",hub)
        self.assertIn("KnownNotebookNode(WitnessEchoes,Node.id)",hub)
        self.assertIn("NotebookComparisonUnlocked(WitnessEchoes)",hub)
        self.assertIn("UFUNCTION(BlueprintPure,Category=\"Unmade|Investigation\")",hub_header)
        self.assertIn("FString GetEvidenceNotebook() const;",hub_header)
        for id in ("CALL.01","CALL.03","CALL.04"):
            self.assertIn(id,rules)
        self.assertIn("if (!ledger.HasFirst(id))",rules)
        self.assertIn("ledger.HasReturn(id)",rules)
    def test_input_is_read_only_and_uses_existing_save(self):
        player=source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        config=source("Config/DefaultInput.ini")
        self.assertIn('ActionName="EvidenceNotebook",Key=F9',config)
        self.assertIn('BindAction("EvidenceNotebook"',player)
        self.assertIn("Hub->GetEvidenceNotebook();",player)
        self.assertIn('World.WitnessBraidShelter',player)
        self.assertIn('World.WitnessBraidDocket',player)
        self.assertNotIn('World.WitnessBraidResolved',player)
    def test_knowledge_cannot_teleport(self):
        npc=source("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        mem=source("Source/TheUnmadeGame/Private/NPC/UnmadeMemoryComponent.cpp")
        hub=source("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertIn("for(const FUnmadeNpcObservation& Event:Memory->GetObservations())",npc)
        self.assertIn("BraidAccountFromEvent",npc)
        self.assertIn("WitnessProvenance::PersonallyWitnessed",npc)
        self.assertIn("WitnessProvenance::HeardFromPerson",npc)
        self.assertIn("My source was %s.",npc)
        self.assertIn("FMath::Square(400.f)",hub)
        self.assertIn("Event.Evidence == EUnmadeEvidenceKind::Witnessed",hub)
        self.assertIn("World.WitnessBraidShelter",mem)
        self.assertIn("World.WitnessBraidDocket",mem)
        self.assertNotIn("WitnessBraid.Outcome()",npc)
        self.assertIn("const FGuid EventId = FGuid::NewGuid();",source("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp"))
    def test_ci_first_pc_and_bible(self):
        self.assertIn("Tests/world/evidence_provenance_test.cpp",source(".github/workflows/static-checks.yml"))
        self.assertIn("test_evidence_provenance_wiring.py",source("Scripts/first_pc_build_and_test.ps1"))
        for name in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                     "docs/lore/living-world-continuity.md",
                     "docs/lore/CONTINUITY_LOG.md"):
            self.assertIn("The Living Evidence",source(name))
if __name__=="__main__":unittest.main()
