#pragma once
// Pure C++17 structural preflight of the most damage-sensitive serialized
// fields. Disabled optional flags may carry legacy unused/default values.
// This catches malformed shapes before any live restoration or disk write;
// it is NOT a substitute for downstream subsystem Restore invariants.
#include "Save/UnmadeSessionRules.h"
#include "Items/UnmadeItemRules.h"
#include "World/UnmadeWitnessEchoRules.h"
#include "Items/UnmadeUnifiedLoadoutRules.h"
#include "World/UnmadeAbilityStudyRules.h"
#include <vector>
#include <cmath>

namespace UnmadeCore {
template<class Record>
bool ValidCriticalSaveShape(const Record& save) {
    if(save.SchemaVersion!=1)return false;
    if(save.bHasPlayerResume && !ValidResume(
        save.ResumePosition.X,save.ResumePosition.Y,save.ResumePosition.Z,
        save.ResumeHealth,save.ResumeStamina))return false;
    if(save.bHasLivingWorldSnapshot && (!std::isfinite(save.LivingWorldSeconds)
        || save.LivingWorldSeconds<0))return false;
    if(save.bHasInventorySnapshot) {
        if(save.ItemQuantities.Num()<28 ||
           save.ItemQuantities.Num()>static_cast<int>(ItemId::Count) ||
           save.EquippedItems.Num()!=3 || save.AwardedMilestoneBits<0)
            return false;
        for(const auto count:save.ItemQuantities)if(count<0)return false;
        for(const auto id:save.EquippedItems)
            if(id<-1 || id>=static_cast<int>(ItemId::Count))return false;
    }
    if(save.bHasEconomySnapshot) {
        if(save.ProfessionSkills.Num()!=3 || save.TradeMarks<0 ||
           save.PaidContractMask<0)return false;
        for(const auto rank:save.ProfessionSkills)if(rank<0)return false;
    }
    // Validate the full nine-realm source domain before accepting equipped
    // rewards. An in-range realm index is not proof a reward was earned.
    Expeditions earned;
    if(save.bHasExpeditions) {
        std::vector<int> record;
        record.reserve(save.ExpeditionState.Num());
        for(const auto value:save.ExpeditionState)record.push_back(value);
        if(!earned.Restore(record))return false;
    }
    if(save.bHasUnifiedLoadout) {
        if(save.UnifiedRealmLoadout.Num()!=3)return false;
        const UnifiedLoadout equipped{save.UnifiedRealmLoadout[0],
                                      save.UnifiedRealmLoadout[1],
                                      save.UnifiedRealmLoadout[2]};
        if(!equipped.Valid(earned))return false;
    }
    // Failed or secondhand-only research cannot skip understanding or trials.
    if(save.bHasAbilityStudy) {
        if(save.AbilityStudyStages.Num()!=StudyCount ||
           save.AbilityStudyEvidence.Num()!=StudyCount ||
           save.AbilityStudyTrials.Num()!=StudyCount ||
           save.AbilityStudySelection<0 ||
           save.AbilityStudySelection>=StudyCount)return false;
        StudySnapshot snapshot;
        for(int i=0;i<StudyCount;++i) {
            snapshot.stages[i]=save.AbilityStudyStages[i];
            snapshot.evidence[i]=save.AbilityStudyEvidence[i];
            snapshot.trials[i]=save.AbilityStudyTrials[i];
        }
        AbilityStudy research;
        if(!research.Restore(snapshot))return false;
    }
    if(save.bHasWitnessEchoSnapshot) {
        const int first=save.WitnessEchoFirstMask;
        const int back=save.WitnessEchoReturnMask;
        if(first<0||back<0||static_cast<unsigned>(first)>WitnessEchoValidMask||
           static_cast<unsigned>(back)>WitnessEchoValidMask||
           (back&~first)!=0)return false;
    }
    if(save.bHasWitnessBraidSnapshot &&
       (save.WitnessBraidOutcome<0||save.WitnessBraidOutcome>2))return false;
    if(save.bHasWitnessDispatchSnapshot) {
        const int stage=save.WitnessDispatchStage;
        const int collected=save.WitnessDispatchCollectedDay;
        const int delivered=save.WitnessDispatchDeliveredDay;
        if(stage<0||stage>2||collected<0||delivered<0||
           collected>10000000||delivered>10000000)return false;
        if(stage==0 && (collected||delivered))return false;
        if(stage==1 && (collected==0||delivered!=0))return false;
        if(stage==2 && (collected==0||delivered<collected))return false;
        if(stage>0 && (!save.bHasWitnessBraidSnapshot||
                        save.WitnessBraidOutcome==0))return false;
    }
    if(save.bHasWitnessReturnSnapshot) {
        const int stage=save.WitnessReturnStage;
        const int route=save.WitnessReturnRoute;
        const int chosen=save.WitnessReturnChosenDay;
        const int carried=save.WitnessReturnCollectedDay;
        const int delivered=save.WitnessReturnDeliveredDay;
        if(stage<0||stage>3||route<0||route>2||chosen<0||carried<0||
           delivered<0||chosen>10000000||carried>10000000||
           delivered>10000000)return false;
        if(stage==0&&(route||chosen||carried||delivered))return false;
        if(stage>0) {
            if(!save.bHasWitnessDispatchSnapshot ||
               save.WitnessDispatchStage!=2 || route==0 ||
               chosen<=save.WitnessDispatchDeliveredDay)return false;
            if(stage==1&&(carried||delivered))return false;
            if(stage>=2&&(carried<chosen||carried==0))return false;
            if(stage==2&&delivered)return false;
            if(stage==3&&(delivered<carried||delivered==0))return false;
        }
    }
    return true;
}
}
