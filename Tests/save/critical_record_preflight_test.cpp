#include "Save/UnmadeCriticalSavePreflight.h"
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
 // Disabled optional fields must be ignored for legacy/earlier save slots.
 Record legacy;legacy.WitnessBraidOutcome=999;legacy.TradeMarks=-7;
 assert(ValidCriticalSaveShape(legacy));
 std::cout<<"PASS: critical preflight rejects malformed core save shapes "
             "and respects schema-1 optional-field absence\n";
}
