"""The player earns learned powers from physical source events and durable records."""
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
def read(path):return (ROOT/path).read_text(encoding="utf-8")


class AbilityStudyWiring(unittest.TestCase):
    def test_real_creature_attack_telegraph_and_contact_both_required(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        enemy=read("Source/TheUnmadeGame/Public/Combat/UnmadeEnemyCharacter.h")
        for token in ("Enemy->IsActivelyTelegraphing(Now)",
                      "if(RequireWarning&&!Enemy->IsActivelyTelegraphing(Now))continue;",
                      "Before>Target->GetCombat()->GetHealth()",
                      "RecordStudyEvidence(StudyId,2)",
                      "GetWorld()->LineTraceTestByChannel",
                      "FMath::Square(700.0)"):
            self.assertIn(token,player)
        self.assertIn("StudyCreature(UnmadeCore::CreatureKind& Kind)",enemy)
        self.assertIn("WindupEndsAt>Now",enemy)

    def test_study_is_persisted_without_clobbering_other_save_data(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        save=read("Source/TheUnmadeGame/Public/Save/UnmadePrototypeSave.h")
        for token in ("bHasAbilityStudy","AbilityStudyStages","AbilityStudyEvidence",
                      "AbilityStudyTrials","AbilityStudySelection"):
            self.assertIn(token,player)
            self.assertIn(token,save)
        for token in ("if(!PersistStudy()){AbilityStudy.Restore(Before);return false;}",
                      "Malformed Ability Study save",
                      "Save->PlayerStrain=FractureModel.CurrentStrain();",
                      "return UnmadeSave::SaveGameToSlot(Save);",
                      "if(!PersistStudy())AbilityStudy.Restore(Before);"):
            self.assertIn(token,player)

    def test_actual_experiments_cost_strain_and_activate_effects(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for token in ("NearEchoStudySite()", "FractureModel.SpendStrain(8)",
                      "FractureModel.SpendStrain(10)", "FractureModel.SpendStrain(5)",
                      "NextStudyTrialAt=Now+8", "AbilityStudy.Experiment(Id)",
                      "AbilityStudy.Master(Id)", "FractureModel.SpendStrain(9)",
                      "Enemy->ExposeToFold(Now,ExposeSeconds)", "Combat->Dodge(Now)",
                      "Equipment->SetStudyBonuses(0,Ward)",
                      "StudyWardUntil=FMath::Max(StudyWardUntil,Now+WardTime)"):
            self.assertIn(token,player)

    def test_atlas_is_earned_at_the_well_and_works_as_equipment(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        core=read("Source/TheUnmadeGame/Public/Items/UnmadeItemRules.h")
        equip=read("Source/TheUnmadeGame/Public/Items/UnmadeEquipmentComponent.h")
        for token in ("AbilityStudy.CanClaimAtlas(CurrentAtlasRequirements())","NearEchoStudySite()",
                      "Equipment->Claim(UnmadeCore::Achievement::AtlasOfAbsenceMastery)",
                      "Save->VerifiedRoadBits>0",
                      "Save->RiteStages[9]==5",
                      "Save->ConfluenceStages[5]==2",
                      "Save->DiscoveredLoreMask&1"):
            self.assertIn(token,player)
        for token in ("case Achievement::AtlasOfAbsenceMastery: okay=add(ItemId::AtlasOfAbsence,1)",
                      "charm==ItemId::AtlasOfAbsence ? 4",
                      "Equipped(GearSlot::Charm)==ItemId::AtlasOfAbsence ? 2"):
            self.assertIn(token,core)
        self.assertIn("Inventory.Equipped(UnmadeCore::GearSlot::Charm)==UnmadeCore::ItemId::AtlasOfAbsence?2:0",equip)

    def test_requirements_are_visible_not_just_implicit(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        core=read("Source/TheUnmadeGame/Public/World/UnmadeAbilityStudyRules.h")
        self.assertIn("GetStudyJournal()const",player)
        self.assertIn("AbilityStudy.Checklist(Id)",player)
        self.assertIn("AbilityStudy.AtlasChecklist(Atlas)",player)
        self.assertIn('TEXT("  [%s] %s',player)
        self.assertIn("CanClaimAtlas(const AtlasStudyContext& ctx)",core)
        self.assertIn("for(const auto& requirement:AtlasChecklist(ctx))",core)
        self.assertIn("!FindStudyEnemy(SelectedStudy,true)",player)

    def test_journal_surfaces_real_people_creature_and_place_sources(self):
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        for token in ("LIVING SOURCES NEARBY", "FindStudyEnemy(Subject,false)",
                      "NearEchoStudySite()", "UnmadeCore::RiteSpecs",
                      "Resident->GetStableId()!=FName(UTF8_TO_TCHAR(Rite.witness))",
                      "UnmadeStudyMentorVisibility", "Being nearby never grants a skill"):
            self.assertIn(token,player)

    def test_keyboard_and_native_controller_menu_are_connected(self):
        config=read("Config/DefaultInput.ini")
        player=read("Source/TheUnmadeGame/Private/Player/UnmadeCharacter.cpp")
        ui=read("Source/TheUnmadeGame/Private/UI/UnmadeRpgWidget.cpp")
        for action,key in (("StudyObserve","Five"),("StudyProgress","Nine"),
                           ("StudyUse","Zero"),("StudyNext","O")):
            self.assertIn(f'ActionName="{action}",Key={key}',config)
            self.assertIn(f'BindAction("{action}"',player)
            self.assertIn(f'TEXT("{action}")',player)
            self.assertIn(f'TEXT("{action}Action")',ui)
        self.assertIn("p->GetStudyJournal()",ui)
        self.assertIn("p->StudyProgress()",ui)


if __name__=="__main__":unittest.main()
