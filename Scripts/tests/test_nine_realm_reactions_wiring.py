"""v2 is real eight-realm runtime integration, not a nine-name lore table."""
from pathlib import Path
import unittest
ROOT=Path(__file__).resolve().parents[2]
def src(p):return (ROOT/p).read_text(encoding="utf-8")

class NineRealmWiring(unittest.TestCase):
    def test_all_nine_policy_sources_are_distinct_and_cast_is_spatial(self):
        rules=src("Source/TheUnmadeGame/Public/World/UnmadeRealmReactionRules.h")
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for name in ("ThreefoldReach","WidowedRain","HearthBeneath","TidalLedger",
                     "SkyBelow","CinderSpine","HundredUnlived","OrchardOfKings",
                     "FirstAbsence"):
            self.assertIn("Realm::"+name,rules)
        self.assertIn("RealmReactionAt(CastPosition.X,CastPosition.Y)",hub)
        self.assertIn("OuterReactions.Cast(Policy->realm,Id,Clock.ElapsedSeconds()",hub)
        self.assertIn("RealityLedger.Cast(Id,Clock.ElapsedSeconds()",hub)
        self.assertIn("OuterReactions.Read(Policy->realm)",hub)
        self.assertIn("RealmEffect(Policy->realm,*Ledger",hub)
        self.assertIn("GetRealmHazardPercent",hub)
        self.assertIn("GetRealmTradeTrustBonus",hub)
        self.assertIn("bWitness=true;break;",hub)

    def test_tangible_eight_realm_variants_and_combat(self):
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        enemy=src("Source/TheUnmadeGame/Private/Combat/UnmadeEnemyCharacter.cpp")
        player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for name in ("SafeSpan","StrainedSpan","DoubleThreshold",
                     "CareLedger","InquiryLedger"):
            self.assertIn('Prefix+TEXT("'+name+'")',hub)
        self.assertIn("RefreshOuterReactionWorld();",hub)
        self.assertIn("ActiveNow!=OuterActiveMask",hub)
        self.assertIn("Consequence.enemySpeedPercent",enemy)
        self.assertIn("Hub->GetRealmHazardPercent(GetActorLocation())",player)
        self.assertIn("FractureModel.ApplyEnvironmentalStrain(AdjustedSeverity)",player)
        self.assertIn("Hub->GetRealmTradeTrustBonus(P)",player)

    def test_provenance_and_no_global_civic_telepathy(self):
        player=src("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        npc=src("Source/TheUnmadeGame/Private/NPC/UnmadeNpcCharacter.cpp")
        self.assertIn('TEXT("Reality.Realm.%d")',player)
        self.assertIn("Event.SubjectId==FName(*FString::Printf",npc)
        self.assertIn("LocalPolicy->careLine",npc)
        self.assertIn("LocalPolicy->inquiryLine",npc)
        self.assertIn("Event.SpeakerId.ToString()",npc)
        self.assertIn("EUnmadeEvidenceKind::Witnessed",npc)

    def test_save_is_additive_atomic_fail_closed(self):
        save=src("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        hub=src("Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp")
        for key in ("bHasOuterRealmReactions=false","OuterCareMasks","OuterInquiryMasks",
                    "OuterActiveRites","OuterActiveUntil"):
            self.assertIn(key,save)
        self.assertIn("Save->OuterCareMasks.Num()==8",hub)
        self.assertIn("RestoredOuter.Restore(Snapshot,RestoredClock.ElapsedSeconds())",hub)
        self.assertIn("bOuterReactionsRejected=true",hub)
        self.assertIn("bRealityConsequenceRejected || bOuterReactionsRejected ||",hub)
        self.assertIn("bMaterialSaveRejected)return false;",hub)
        self.assertIn("OuterReactions.Restore(BeforeOuter,Clock.ElapsedSeconds())",hub)
        self.assertIn("Save->bHasOuterRealmReactions=true;",hub)
        self.assertIn("Tests/world/nine_realm_reactions_test.cpp",
                      src(".github/workflows/static-checks.yml"))
        self.assertIn("test_nine_realm_reactions_wiring.py",
                      src("Scripts/first_pc_build_and_test.ps1"))

    def test_canon_sync(self):
        for path in ("docs/lore/VOLUME_X_THE_UNANSWERED_ATLAS.md",
                     "docs/lore/CONTINUITY_LOG.md",
                     "docs/lore/living-world-continuity.md"):
            self.assertIn("Nine-Realm Reaction System",src(path))
if __name__=="__main__":unittest.main()
