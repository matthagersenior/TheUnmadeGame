#include "Save/UnmadeCriticalSavePreflight.h"
#include "World/UnmadeExpeditionRules.h"
#include "World/UnmadeAbilityStudyRules.h"
#include "World/UnmadeLivingEconomyRules.h"
#include "Items/UnmadeItemRules.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
using namespace UnmadeCore;
template<class T> struct Array:std::vector<T>{
    using std::vector<T>::vector;
    int Num()const{return static_cast<int>(this->size());}
};
struct Pos { double X=0,Y=0,Z=135; };
struct Record{
 int SchemaVersion=1;
 bool bHasPlayerResume=false;Pos ResumePosition{};
 double ResumeHealth=100,ResumeStamina=100;
 bool bHasLivingWorldSnapshot=false;double LivingWorldSeconds=0;
 bool bHasInventorySnapshot=false;
 Array<int> ItemQuantities,EquippedItems;
 long long AwardedMilestoneBits=0;
 bool bHasEconomySnapshot=false;Array<int> ProfessionSkills;int TradeMarks=45,PaidContractMask=0;
 bool bHasUnifiedLoadout=false;Array<int> UnifiedRealmLoadout;
 bool bHasExpeditions=false;Array<int> ExpeditionState;
 bool bHasAbilityStudy=false;Array<int> AbilityStudyStages,AbilityStudyEvidence,AbilityStudyTrials;
 int AbilityStudySelection=0;
 bool bHasMaterialWorld=false;
 Array<int> MaterialDays,MaterialStores,MaterialHouseholds,MaterialIntegrity,MaterialProduced,
    MaterialConsumed,MaterialShortageDays,MaterialLaborHours,MaterialDeliveries,MaterialRepairs;
 int MaterialCargoRealm=-1,MaterialCargoUnits=0,MaterialCargoDay=0;
 bool bHasLivingEconomy=false;
 Array<int> LivingEmployerMarks,LivingEarnedMarks,LivingOwedMarks,LivingPaidShifts,
     LivingPaidDays,LivingWorkerReceipts,LivingForeignStock,LivingRouteReceipts,
     LivingTradePayments,LivingConsumedSupport;
 int LivingTradeCargoRoute=-1,LivingTradeCargoDay=0;
 bool bHasWitnessEchoSnapshot=false;int WitnessEchoFirstMask=0,WitnessEchoReturnMask=0;
 bool bHasWitnessBraidSnapshot=false;int WitnessBraidOutcome=0;
 bool bHasWitnessDispatchSnapshot=false;
 int WitnessDispatchStage=0,WitnessDispatchCollectedDay=0,WitnessDispatchDeliveredDay=0;
 bool bHasWitnessReturnSnapshot=false;
 int WitnessReturnStage=0,WitnessReturnRoute=0,
     WitnessReturnChosenDay=0,WitnessReturnCollectedDay=0,WitnessReturnDeliveredDay=0;
};
int main(){
 Record s;
 assert(ValidCriticalSaveShape(s)); // old empty schema-1 saves remain valid
 s.SchemaVersion=2;assert(!ValidCriticalSaveShape(s));s.SchemaVersion=1;
 s.bHasPlayerResume=true;s.ResumeHealth=35;
 assert(ValidCriticalSaveShape(s));
 s.ResumePosition.X=999999;assert(!ValidCriticalSaveShape(s));s.ResumePosition.X=0;
 s.ResumeStamina=std::numeric_limits<double>::quiet_NaN();
 assert(!ValidCriticalSaveShape(s));s.ResumeStamina=100;
 s.bHasLivingWorldSnapshot=true;
 s.LivingWorldSeconds=std::numeric_limits<double>::infinity();
 assert(!ValidCriticalSaveShape(s));s.LivingWorldSeconds=480;
 s.bHasInventorySnapshot=true;s.ItemQuantities=Array<int>(28,0);
 s.EquippedItems=Array<int>(3,-1);
 assert(ValidCriticalSaveShape(s));
 s.ItemQuantities[0]=-1;assert(!ValidCriticalSaveShape(s));s.ItemQuantities[0]=0;
 s.EquippedItems[2]=10000;assert(!ValidCriticalSaveShape(s));s.EquippedItems[2]=-1;
 s.ItemQuantities.resize(27);assert(!ValidCriticalSaveShape(s));
 s.ItemQuantities.resize(28);
 s.AwardedMilestoneBits=-1;assert(!ValidCriticalSaveShape(s));s.AwardedMilestoneBits=0;
 s.bHasEconomySnapshot=true;s.ProfessionSkills=Array<int>(3,0);
 assert(ValidCriticalSaveShape(s));
 s.TradeMarks=-1;assert(!ValidCriticalSaveShape(s));s.TradeMarks=45;
 s.ProfessionSkills.resize(2);assert(!ValidCriticalSaveShape(s));s.ProfessionSkills.resize(3);
 s.bHasUnifiedLoadout=true;s.UnifiedRealmLoadout=Array<int>(3,-1);
 assert(ValidCriticalSaveShape(s));
 s.UnifiedRealmLoadout[0]=9;assert(!ValidCriticalSaveShape(s));s.UnifiedRealmLoadout[0]=-1;
 s.bHasWitnessEchoSnapshot=true;
 s.WitnessEchoFirstMask=7;s.WitnessEchoReturnMask=3;
 assert(ValidCriticalSaveShape(s));
 s.WitnessEchoReturnMask=8;assert(!ValidCriticalSaveShape(s));s.WitnessEchoReturnMask=3;
 s.WitnessEchoFirstMask=8192;assert(!ValidCriticalSaveShape(s));s.WitnessEchoFirstMask=7;
 s.bHasWitnessBraidSnapshot=true;s.WitnessBraidOutcome=1;
 assert(ValidCriticalSaveShape(s));
 s.WitnessBraidOutcome=3;assert(!ValidCriticalSaveShape(s));s.WitnessBraidOutcome=1;
 s.bHasWitnessDispatchSnapshot=true;s.WitnessDispatchStage=1;
 s.WitnessDispatchCollectedDay=2;assert(ValidCriticalSaveShape(s));
 s.WitnessDispatchStage=2;s.WitnessDispatchDeliveredDay=1;
 assert(!ValidCriticalSaveShape(s));s.WitnessDispatchDeliveredDay=2;
 assert(ValidCriticalSaveShape(s));
 s.bHasWitnessReturnSnapshot=true;
 s.WitnessReturnStage=1;s.WitnessReturnRoute=2;s.WitnessReturnChosenDay=3;
 assert(ValidCriticalSaveShape(s));
 s.WitnessReturnChosenDay=2;assert(!ValidCriticalSaveShape(s));s.WitnessReturnChosenDay=3;
 s.WitnessReturnStage=2;s.WitnessReturnCollectedDay=3;assert(ValidCriticalSaveShape(s));
 s.WitnessReturnStage=3;s.WitnessReturnDeliveredDay=4;assert(ValidCriticalSaveShape(s));
 s.WitnessReturnDeliveredDay=2;assert(!ValidCriticalSaveShape(s));
 s.WitnessReturnDeliveredDay=4;assert(ValidCriticalSaveShape(s));
 // A saved expedition must satisfy the *real* domain Restore, not merely
 // consist of ints; the first pack int is the append-only format revision.
 s.bHasExpeditions=true;
 s.ExpeditionState=Array<int>(93,0);
 assert(!ValidCriticalSaveShape(s)); // unrecognized pack revision
 const auto basePack=Expeditions{}.Pack();
 s.ExpeditionState.assign(basePack.begin(),basePack.end());
 assert(ValidCriticalSaveShape(s));
 s.ExpeditionState[2]=1; // visited without briefing is impossible
 assert(!ValidCriticalSaveShape(s));
 s.ExpeditionState.assign(basePack.begin(),basePack.end());
 assert(ValidCriticalSaveShape(s));
 s.UnifiedRealmLoadout[0]=0; // valid index, but unearned reward
 assert(!ValidCriticalSaveShape(s));
 s.UnifiedRealmLoadout[0]=-1;
 assert(ValidCriticalSaveShape(s));
 // Firsthand ability research also has required stage/evidence/trial
 // dependencies; a forged mastery cannot bypass Observation -> Mastery.
 s.bHasAbilityStudy=true;
 s.AbilityStudyStages=Array<int>(StudyCount,0);
 s.AbilityStudyEvidence=Array<int>(StudyCount,0);
 s.AbilityStudyTrials=Array<int>(StudyCount,0);
 s.AbilityStudySelection=0;
 assert(ValidCriticalSaveShape(s));
 s.AbilityStudyStages[0]=4; // bogus mastered study, no firsthand evidence
 assert(!ValidCriticalSaveShape(s));
 s.AbilityStudyStages[0]=1;s.AbilityStudyEvidence[0]=1;
 assert(ValidCriticalSaveShape(s));
 s.AbilityStudySelection=StudyCount;
 assert(!ValidCriticalSaveShape(s));
 s.AbilityStudySelection=0;
 s.AbilityStudyEvidence[0]=2; // secondhand signal without first encounter
 assert(!ValidCriticalSaveShape(s));
 s.AbilityStudyEvidence[0]=1;
 assert(ValidCriticalSaveShape(s));

 // Warehouse units and marks are finite: the source-of-truth domain classes
 // validate a save as a whole, including cargo custody and treasury conservation.
 s.bHasMaterialWorld=true;
 assert(!ValidCriticalSaveShape(s)); // partial columns cannot be loaded
 MaterialWorld original;
 const auto towns=original.Snapshot();
 for(const auto& t:towns) {
    s.MaterialDays.push_back(t.day);s.MaterialStores.push_back(t.store);
    s.MaterialHouseholds.push_back(t.households);s.MaterialIntegrity.push_back(t.integrity);
    s.MaterialProduced.push_back(t.produced);s.MaterialConsumed.push_back(t.consumed);
    s.MaterialShortageDays.push_back(t.shortageDays);s.MaterialLaborHours.push_back(t.laborHours);
    s.MaterialDeliveries.push_back(t.deliveries);s.MaterialRepairs.push_back(t.repairs);
 }
 assert(ValidCriticalSaveShape(s));
 s.MaterialStores[0]=-1;assert(!ValidCriticalSaveShape(s));s.MaterialStores[0]=towns[0].store;
 assert(ValidCriticalSaveShape(s));
 s.bHasLivingEconomy=true;
 assert(!ValidCriticalSaveShape(s)); // no fabricated worker or trade columns
 const auto economy=LivingEconomy{}.Save();
 for(const auto& w:economy.workers){
    s.LivingEmployerMarks.push_back(w.employerMarks);s.LivingEarnedMarks.push_back(w.earnedMarks);
    s.LivingOwedMarks.push_back(w.owedMarks);s.LivingPaidShifts.push_back(w.shiftsPaid);
    s.LivingPaidDays.push_back(w.lastPaidDay);s.LivingWorkerReceipts.push_back(w.deliveriesWitnessed);
 }
 for(const auto& r:economy.routes){
    s.LivingForeignStock.push_back(r.foreignStock);s.LivingRouteReceipts.push_back(r.deliveries);
    s.LivingTradePayments.push_back(r.paidMarks);s.LivingConsumedSupport.push_back(r.consumedSupport);
 }
 assert(ValidCriticalSaveShape(s));
 s.LivingEmployerMarks[0]+=1;assert(!ValidCriticalSaveShape(s)); // invented mark
 s.LivingEmployerMarks[0]-=1;assert(ValidCriticalSaveShape(s));
 s.MaterialCargoRealm=0;s.MaterialCargoUnits=5;s.MaterialCargoDay=1;
 assert(ValidCriticalSaveShape(s));
 s.LivingTradeCargoRoute=0;s.LivingTradeCargoDay=1;
 assert(!ValidCriticalSaveShape(s)); // two simultaneous player-held crates
 s.LivingTradeCargoRoute=-1;s.LivingTradeCargoDay=0;
 s.MaterialCargoRealm=-1;s.MaterialCargoUnits=0;s.MaterialCargoDay=0;
 assert(ValidCriticalSaveShape(s));
 // Disabled optional fields must be ignored for legacy/earlier save slots.
 Record legacy;legacy.WitnessBraidOutcome=999;legacy.TradeMarks=-7;
 assert(ValidCriticalSaveShape(legacy));
 std::cout<<"PASS: critical preflight rejects malformed core save shapes "
             "and respects schema-1 optional-field absence\n";
}
