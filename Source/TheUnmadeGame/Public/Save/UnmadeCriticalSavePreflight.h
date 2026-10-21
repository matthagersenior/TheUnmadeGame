#pragma once
// Pure C++17 structural preflight of the most damage-sensitive serialized
// fields. Disabled optional flags may carry legacy unused/default values.
// This catches malformed shapes before any live restoration or disk write;
// it is NOT a substitute for downstream subsystem Restore invariants.
#include "Save/UnmadeSessionRules.h"
#include "Items/UnmadeItemRules.h"
#include "World/UnmadeWitnessEchoRules.h"
#include "Save/UnmadeNpcMemorySaveRules.h"
#include "Items/UnmadeUnifiedLoadoutRules.h"
#include "World/UnmadeAbilityStudyRules.h"
#include "World/UnmadeLivingEconomyRules.h"
#include <vector>
#include <cmath>

namespace UnmadeCore {
template<class Record>
bool ValidCriticalSaveShape(const Record& save) {
    if(save.SchemaVersion!=1)return false;
    if(!ValidNpcSnapshots(save.NpcSnapshots))return false;
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
    // Validated *as one system*: warehouse quantities, workdays, payroll
    // conservation and physically carried goods all refer to the same clock.
    // Earlier saves lacking these optional fields keep their original defaults.
    if(save.bHasMaterialWorld || save.bHasLivingEconomy) {
        if(!save.bHasLivingWorldSnapshot)return false;
        LivingWorldClock clock;
        if(!clock.Restore(save.LivingWorldSeconds))return false;
        const int currentDay=clock.DayIndex()+1;
        MaterialWorld material;
        if(save.bHasMaterialWorld) {
            if(save.MaterialDays.Num()!=9 || save.MaterialStores.Num()!=9 ||
               save.MaterialHouseholds.Num()!=9 || save.MaterialIntegrity.Num()!=9 ||
               save.MaterialProduced.Num()!=9 || save.MaterialConsumed.Num()!=9 ||
               save.MaterialShortageDays.Num()!=9 || save.MaterialLaborHours.Num()!=9 ||
               save.MaterialDeliveries.Num()!=9 || save.MaterialRepairs.Num()!=9)
                return false;
            MaterialWorld::Towns towns{};
            for(int i=0;i<9;++i) {
                towns[i]={save.MaterialDays[i],save.MaterialStores[i],
                          save.MaterialHouseholds[i],save.MaterialIntegrity[i],
                          save.MaterialProduced[i],save.MaterialConsumed[i],
                          save.MaterialShortageDays[i],save.MaterialLaborHours[i],
                          save.MaterialDeliveries[i],save.MaterialRepairs[i]};
            }
            if(!material.Restore(towns,{save.MaterialCargoRealm,
                 save.MaterialCargoUnits,save.MaterialCargoDay},currentDay))
                return false;
        } else if(!material.InitializeAtDay(currentDay))return false;
        if(save.bHasLivingEconomy) {
            if(save.LivingEmployerMarks.Num()!=9 || save.LivingEarnedMarks.Num()!=9 ||
               save.LivingOwedMarks.Num()!=9 || save.LivingPaidShifts.Num()!=9 ||
               save.LivingPaidDays.Num()!=9 || save.LivingWorkerReceipts.Num()!=9 ||
               save.LivingForeignStock.Num()!=2 || save.LivingRouteReceipts.Num()!=2 ||
               save.LivingTradePayments.Num()!=2 || save.LivingConsumedSupport.Num()!=2)
                return false;
            LivingEconomy::Snapshot economy=LivingEconomy{}.Save();
            for(int i=0;i<9;++i) {
                economy.workers[i]={save.LivingEmployerMarks[i],
                    save.LivingEarnedMarks[i],save.LivingOwedMarks[i],
                    save.LivingPaidShifts[i],save.LivingPaidDays[i],
                    save.LivingWorkerReceipts[i]};
            }
            for(int i=0;i<2;++i) {
                economy.routes[i]={save.LivingForeignStock[i],
                    save.LivingRouteReceipts[i],save.LivingTradePayments[i],
                    save.LivingConsumedSupport[i]};
            }
            economy.cargo={save.LivingTradeCargoRoute,save.LivingTradeCargoDay};
            LivingEconomy ledgers;
            if(!ledgers.Restore(economy,material,currentDay))return false;
        }
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
