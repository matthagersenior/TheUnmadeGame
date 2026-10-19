#include "World/UnmadeAbilityStudyRules.h"
#include "Items/UnmadeItemRules.h"
#include <cassert>
#include <cmath>

using namespace UnmadeCore;
static void Master(AbilityStudy& study, StudyId id) {
    assert(study.Observe(id,1)==StudyResult::EvidenceRecorded);
    assert(study.Observe(id,2)==StudyResult::EvidenceRecorded);
    assert(study.Understand(id)==StudyResult::Advanced);
    assert(study.Experiment(id)==StudyResult::Advanced);
    assert(study.Experiment(id)==StudyResult::Advanced);
    assert(study.Master(id)==StudyResult::Advanced);
    assert(study.HasMastered(id));
}

int main(){
    AbilityStudy study;
    assert(!study.AtlasReady());
    assert(study.Observe(StudyId::Cinderjaw,2)==StudyResult::NeedsEvidence);
    assert(study.Understand(StudyId::Cinderjaw)==StudyResult::NeedsEvidence);
    assert(study.Experiment(StudyId::Cinderjaw)==StudyResult::NeedsUnderstanding);
    assert(study.Observe(StudyId::Cinderjaw,1)==StudyResult::EvidenceRecorded);
    assert(study.Observe(StudyId::Cinderjaw,1)==StudyResult::AlreadyRecorded);
    assert(study.Understand(StudyId::Cinderjaw)==StudyResult::NeedsEvidence);
    assert(study.Observe(StudyId::Cinderjaw,2)==StudyResult::EvidenceRecorded);
    assert(study.Understand(StudyId::Cinderjaw)==StudyResult::Advanced);
    assert(study.Experiment(StudyId::Cinderjaw)==StudyResult::Advanced);
    assert(study.Stage(StudyId::Cinderjaw)==StudyStage::Understood);
    auto saved=study.Snapshot();
    AbilityStudy reloaded;
    assert(reloaded.Restore(saved));
    assert(reloaded.Trials(StudyId::Cinderjaw)==1);
    assert(reloaded.Experiment(StudyId::Cinderjaw)==StudyResult::Advanced);
    assert(reloaded.Stage(StudyId::Cinderjaw)==StudyStage::Experimented);
    assert(reloaded.Master(StudyId::Cinderjaw)==StudyResult::Advanced);
    assert(reloaded.Observe(StudyId::Cinderjaw,1)==StudyResult::AlreadyRecorded);
    assert(reloaded.Master(StudyId::Cinderjaw)==StudyResult::AlreadyMastered);

    Master(reloaded,StudyId::PaleStrider);
    assert(reloaded.CanCombine(StudyId::Cinderjaw,StudyId::PaleStrider));
    assert(!reloaded.CanCombine(StudyId::Cinderjaw,StudyId::EchoWell));
    assert(!reloaded.AtlasReady());
    Master(reloaded,StudyId::EchoWell);
    assert(reloaded.AtlasReady());

    AbilityStudy fresh;
    auto bad=reloaded.Snapshot();
    bad.stages[2]=4; // no actual firsthand evidence or experiment
    assert(!fresh.Restore(bad));
    assert(fresh.Stage(StudyId::Cinderjaw)==StudyStage::Unknown);
    bad=reloaded.Snapshot();
    bad.evidence[0]=1; // claimed mastery with uncorroborated evidence
    assert(!fresh.Restore(bad));
    assert(fresh.Restore(reloaded.Snapshot()));
    assert(fresh.AtlasReady());

    InventoryModel inventory;
    assert(inventory.Claim(Achievement::AtlasOfAbsenceMastery)==RewardResult::Awarded);
    assert(inventory.Quantity(ItemId::AtlasOfAbsence)==1);
    assert(inventory.Claim(Achievement::AtlasOfAbsenceMastery)==RewardResult::AlreadyAwarded);
    assert(inventory.Equip(ItemId::AtlasOfAbsence));
    assert(inventory.GlimpseStrainDiscount()==4);
    assert(inventory.FoldDurationBonus()==2);
    auto inventorySnapshot=inventory.Snapshot();
    InventoryModel inventoryReloaded;
    assert(inventoryReloaded.Restore(inventorySnapshot));
    assert(inventoryReloaded.GlimpseStrainDiscount()==4);
    assert(inventoryReloaded.FoldDurationBonus()==2);
    return 0;
}
