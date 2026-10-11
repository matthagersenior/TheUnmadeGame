"""Regression contract: a cast actually affects traversal, combat, residents and civic state."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def read(p):return (ROOT/p).read_text(encoding="utf-8")

class RealityConsequenceIntegration(unittest.TestCase):
    def test_all_ten_mapped_and_a_real_source_of_casts(self):
        rules=read("Source/TheUnmadeGame/Public/World/UnmadeRealityConsequenceRules.h")
        component=read("Source/TheUnmadeGame/Private/World/UnmadeTenfoldComponent.cpp")
        for rite in ("UnwriteLaw","Witnesscraft","BorrowedLives","LegacyForging",
                     "LivingRoads","TomorrowDebt","UnderstandingBosses",
                     "ParadoxConvergence","Oathbinding","Cartography"):
            self.assertIn("RiteId::"+rite,rules)
        self.assertIn("Hub->RecordRealityConsequence(Id,Power.duration,",component)
        self.assertIn("Player->ReportRealityConsequenceEvent(Id);",component)
        self.assertIn("if(!Persist(true))",component)
        self.assertIn("bool mastered,bool physicallyWitnessed,bool insideFocus",rules)
        self.assertIn("if(mastered && physicallyWitnessed)",rules)
        self.assertIn("mask|=static_cast<std::uint16_t>(1u<<index)",rules)
        self.assertIn("Count(state_.careWitnessMask)>=2",rules)
        self.assertIn("Count(state_.inquiryWitnessMask)>=2",rules)

    def test_physical_transient_and_civic_objects_share_ledger(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for tag in ("Reality.ActiveSafeSpan","Reality.ActiveStrainedSpan",
                    "Reality.ActiveDoubleThreshold","Reality.CivicShared",
                    "Reality.CivicInquiry"):
            self.assertIn(tag,hub)
        self.assertIn("RefreshRealityConsequenceWorld();",hub)
        self.assertIn("RealityLedger.Active(Clock.ElapsedSeconds())",hub)
        self.assertIn("RealityLedger.PublicResponse()",hub)
        self.assertIn("GetRealityConsequenceJournal()",hub)
        self.assertIn("bRealityVisualActive && !RealityLedger.Active",hub)
        self.assertIn("FMath::Square(2600.f)",hub)

    def test_real_enemy_movement_and_individual_memory(self):
        enemy=read("Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp")
        npc=read("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        memory=read("Source/TheUnmadeGame/Private/NPC/UnmadeMemoryComponent.cpp")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        self.assertIn("RealityConsequenceAt(GetActorLocation())",enemy)
        self.assertIn("Consequence.enemySpeedPercent",enemy)
        self.assertIn("Reality.CareWitnessed",memory)
        self.assertIn("Reality.InquiryWitnessed",memory)
        self.assertIn("Reality.CareWitnessed",npc)
        self.assertIn("Reality.InquiryWitnessed",npc)
        self.assertIn("I heard from %s",npc)
        self.assertIn("RealityProfiles[Index].care",player)
        self.assertIn("FMath::Square(720.f)",hub)
        self.assertIn("UnmadeRealityWitness",hub)
        self.assertIn("GetRealityConsequenceJournal()",player)

    def test_old_saves_invalid_values_fail_closed_and_rollback(self):
        hub=read("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for key in ("bHasRealityConsequenceSnapshot=false","RealityCareWitnessMask=0",
                    "RealityInquiryWitnessMask=0","RealityLastRite=-1",
                    "RealityActiveUntil=0"):
            self.assertIn(key,save)
        self.assertIn("RestoredReality.Restore({",hub)
        self.assertIn("bRealityConsequenceRejected=true;",hub)
        self.assertIn("bRealityConsequenceRejected || bOuterReactionsRejected ||",hub)
        self.assertIn("bMaterialSaveRejected)return false;",hub)
        self.assertIn("RealityLedger.Restore(Before,Clock.ElapsedSeconds());",hub)
        self.assertIn("if(!WriteWorldSnapshot())",hub)
        self.assertIn("Tests/world/reality_consequence_test.cpp",
                      read(".github/workflows/static-checks.yml"))
        self.assertIn("test_reality_consequence_wiring.py",
                      read("Scripts/first_pc_build_and_test.ps1"))

    def test_lore_update_policy(self):
        self.assertIn("The Law of Consequence",
                      read("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md"))
        self.assertIn("Reality Consequence Engine",
                      read("docs/lore/living-world-continuity.md"))
        self.assertIn("Reality Consequence Engine",
                      read("docs/lore/CONTINUITY_LOG.md"))

if __name__=="__main__":
    unittest.main()
