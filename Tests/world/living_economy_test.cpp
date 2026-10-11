#include "World/UnmadeLivingEconomyRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main(){
    MaterialWorld materials;LivingEconomy economy;
    assert(NamedTradeWorkers.size()==9 && TradeContracts.size()==2);
    for(int i=0;i<9;++i){
        assert(static_cast<int>(NamedTradeWorkers[i].home)==i);
        assert(economy.WorkerIndex(NamedTradeWorkers[i].stableId)==i);
        assert(std::strlen(NamedTradeWorkers[i].household)>5);
    }
    assert(economy.WorkerIndex("invented.courier")==-1);
    assert(economy.AdvanceOneDay(2));
    assert(!economy.AdvanceOneDay(2));
    assert(economy.Worker(Realm::HearthBeneath)->employerMarks==30);
    assert(economy.Worker(Realm::HearthBeneath)->earnedMarks==2);
    assert(economy.Worker(Realm::HearthBeneath)->shiftsPaid==1);
    assert(economy.Collect(TradeRoute::SaltToHearth,1,materials)==TradeResult::Blocked);
    assert(economy.Collect(TradeRoute::SaltToHearth,2,materials)==TradeResult::Collected);
    assert(materials.Town(Realm::WidowedRain)->store==19);
    assert(economy.Worker(Realm::HearthBeneath)->employerMarks==24); // escrow
    assert(economy.Collect(TradeRoute::HearthToSalt,2,materials)==TradeResult::Blocked);
    assert(economy.Deliver(TradeRoute::HearthToSalt,2,materials)==TradeResult::Blocked);
    assert(economy.Deliver(TradeRoute::SaltToHearth,1,materials)==TradeResult::Blocked);
    auto carried=economy.Save();LivingEconomy inTransit;
    assert(inTransit.Restore(carried,materials,10)); // wait as long as needed
    assert(inTransit.Cargo().route==0);
    assert(economy.Deliver(TradeRoute::SaltToHearth,2,materials)==TradeResult::Delivered);
    assert(economy.Worker(Realm::WidowedRain)->employerMarks==36);
    assert(economy.Worker(Realm::HearthBeneath)->deliveriesWitnessed==1);
    assert(economy.Route(TradeRoute::SaltToHearth)->foreignStock==5);
    assert(materials.Town(Realm::HearthBeneath)->store==18); // foreign rain NOT ember
    assert(economy.Deliver(TradeRoute::SaltToHearth,2,materials)==TradeResult::Blocked);
    assert(economy.Collect(TradeRoute::HearthToSalt,2,materials)==TradeResult::Collected);
    assert(materials.Town(Realm::HearthBeneath)->store==13);
    assert(economy.Worker(Realm::WidowedRain)->employerMarks==30);
    assert(economy.Deliver(TradeRoute::HearthToSalt,2,materials)==TradeResult::Delivered);
    assert(economy.HasEarnedMutualAid());
    assert(economy.Route(TradeRoute::HearthToSalt)->foreignStock==5);
    assert(materials.Town(Realm::WidowedRain)->store==19);
    auto shortage=*materials.Town(Realm::HearthBeneath);
    shortage.households=0;
    assert(economy.ApplyShelterSupport(Realm::HearthBeneath,shortage)==1);
    assert(materials.ReinforceShelter(Realm::HearthBeneath,1));
    assert(economy.Route(TradeRoute::SaltToHearth)->foreignStock==4);
    assert(economy.Route(TradeRoute::SaltToHearth)->consumedSupport==1);
    assert(materials.Town(Realm::HearthBeneath)->integrity==82);
    auto cared=shortage;cared.households=12;
    assert(economy.ApplyShelterSupport(Realm::HearthBeneath,cared)==0);
    assert(economy.Route(TradeRoute::SaltToHearth)->foreignStock==4);
    const auto snapshot=economy.Save();
    LivingEconomy loaded;
    assert(loaded.Restore(snapshot,materials,2));
    assert(loaded.HasEarnedMutualAid());
    auto bad=snapshot;bad.workers[0].employerMarks++;
    assert(!loaded.Restore(bad,materials,2)); // no created currency
    bad=snapshot;bad.routes[0].deliveries++;
    assert(!loaded.Restore(bad,materials,2)); // no forged physical receipt
    bad=snapshot;bad.workers[2].deliveriesWitnessed++;
    assert(!loaded.Restore(bad,materials,2)); // cannot impersonate receiver
    bad=snapshot;bad.cargo.route=77;bad.cargo.collectedDay=2;
    assert(!loaded.Restore(bad,materials,2)); // safe malformed index
    assert(loaded.Save().routes[0].deliveries==snapshot.routes[0].deliveries);
    for(int day=3;day<=24;++day)assert(economy.AdvanceOneDay(day));
    assert(economy.Worker(Realm::FirstAbsence)->owedMarks>0);
    assert(economy.Worker(Realm::FirstAbsence)->employerMarks==0);
    assert(economy.Worker(Realm::FirstAbsence)->earnedMarks==32);
    const auto snapshot2=economy.Save();
    assert(!economy.PayArrears(Realm::FirstAbsence)); // no phantom treasury
    assert(economy.Save().workers[8].owedMarks==snapshot2.workers[8].owedMarks);
    // No discount/no story bypass: mutual aid is actual, bilateral receipts only.
    LivingEconomy noAid;
    assert(!noAid.HasEarnedMutualAid());
    std::cout<<"PASS: named workers, conserved wages/escrow, two physical trade receipts, separate foreign goods, shortage support and corrupt snapshots\n";
}
