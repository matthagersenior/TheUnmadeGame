#include "World/UnmadeMaterialConsequencesRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main(){
    MaterialWorld w;
    assert(MaterialPolicies.size()==9);
    for(int i=0;i<9;++i){
        auto realm=static_cast<Realm>(i);
        assert(static_cast<int>(MaterialPolicies[i].realm)==i);
        const auto* t=w.Town(realm);
        assert(t && t->store==MaterialPolicies[i].initialStore);
        assert(t->households==MaterialPolicies[i].initialHouseholds);
        assert(t->day==1 && t->integrity==80);
        assert(MaterialPolicies[i].dailyNeed>0);
    }
    assert(w.Town(Realm::Count)==nullptr);
    std::array<RealityPublicResponse,9> neutral{};
    auto old=w.Snapshot();
    assert(w.AdvanceAllOneDay(neutral));
    assert(w.Town(Realm::HearthBeneath)->day==2);
    assert(w.Town(Realm::WidowedRain)->store!=old[1].store ||
           w.Town(Realm::WidowedRain)->households!=old[1].households);
    assert(w.Town(Realm::HearthBeneath)->laborHours>0);
    assert(w.Town(Realm::HearthBeneath)->consumed>0);
    const auto before=w.Snapshot();
    assert(w.Collect(Realm::HearthBeneath,2)==MaterialAction::Collected);
    assert(w.Cargo().realm==2);
    assert(w.Town(Realm::HearthBeneath)->store==before[2].store-5);
    assert(w.Collect(Realm::HearthBeneath,2)==MaterialAction::Blocked);
    assert(w.Deliver(Realm::WidowedRain,2)==MaterialAction::Blocked);
    assert(w.Deliver(Realm::HearthBeneath,1)==MaterialAction::Blocked);
    assert(w.Deliver(Realm::HearthBeneath,2)==MaterialAction::Delivered);
    assert(w.Cargo().realm==-1);
    assert(w.Town(Realm::HearthBeneath)->households==before[2].households+5);
    assert(w.Deliver(Realm::HearthBeneath,2)==MaterialAction::Blocked);
    // Condition 80 is genuinely worn: one paid repair is valid, repeated
    // repairs at condition 98 are not a free repeatable action.
    assert(w.Repair(Realm::HearthBeneath,2)==MaterialAction::Repaired);
    assert(w.Repair(Realm::HearthBeneath,2)==MaterialAction::NoWork);
    for(int day=2;day<18;++day)assert(w.AdvanceAllOneDay(neutral));
    assert(w.Town(Realm::HearthBeneath)->day==18);
    assert(w.Town(Realm::HearthBeneath)->shortageDays>0);
    assert(w.Town(Realm::HearthBeneath)->integrity<80);
    assert(MaterialWorld::Duty(Realm::HearthBeneath,NpcRole::Courier,*w.Town(Realm::HearthBeneath),DayPhase::Day)==MaterialDuty::Haul);
    assert(MaterialWorld::Duty(Realm::HearthBeneath,NpcRole::Courier,*w.Town(Realm::HearthBeneath),DayPhase::Night)==MaterialDuty::Regular);
    const auto p=w.Snapshot();
    const auto canRepair=w.Repair(Realm::HearthBeneath,18);
    assert(canRepair==MaterialAction::Repaired||canRepair==MaterialAction::Blocked);
    if(canRepair==MaterialAction::Repaired){
        assert(w.Town(Realm::HearthBeneath)->integrity>p[2].integrity);
        assert(w.Town(Realm::HearthBeneath)->store==p[2].store-MaterialPolicies[2].repairCost);
    }
    auto saved=w.Snapshot();auto cargo=w.Cargo();
    MaterialWorld load;
    assert(load.Restore(saved,cargo,18));
    assert(load.Town(Realm::HearthBeneath)->shortageDays==w.Town(Realm::HearthBeneath)->shortageDays);
    auto invalid=saved;invalid[4].store=-1;
    assert(!load.Restore(invalid,cargo,18));
    assert(load.Town(Realm::HearthBeneath)->store==w.Town(Realm::HearthBeneath)->store);
    invalid=saved;invalid[1].day=19;
    assert(!load.Restore(invalid,cargo,18));
    invalid=saved;invalid[6].households=101;
    assert(!load.Restore(invalid,cargo,18));
    assert(!load.Restore(saved,{3,5,19},18));
    assert(!load.Restore(saved,{3,5,0},18));
    assert(!load.Restore(saved,{-1,5,0},18));
    assert(load.Restore(saved,cargo,18));
    MaterialWorld cared,uncared;
    auto care=neutral;care[1]=RealityPublicResponse::SharedPassage;
    for(int i=0;i<3;++i){assert(cared.AdvanceAllOneDay(care));assert(uncared.AdvanceAllOneDay(neutral));}
    assert(cared.Town(Realm::WidowedRain)->laborHours>
           uncared.Town(Realm::WidowedRain)->laborHours);
    assert(cared.Town(Realm::HearthBeneath)->laborHours==
           uncared.Town(Realm::HearthBeneath)->laborHours);
    std::cout<<"PASS: nine supplies, daily labor, shortages, physical no-duplication cargo, repairs, civic impact and atomic validation\n";
}
