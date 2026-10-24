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

    // Seeded adversarial day/action schedules cover failed trades, cargo held
    // in transit, arrears, and repeated cold-save restoration without minting.
    std::uint32_t seed=0xC0FFEEu;
    auto next=[&seed](){seed=seed*1664525u+1013904223u;return seed;};
    for(int scenario=0;scenario<1000;++scenario){
        MaterialWorld simMaterials;LivingEconomy simEconomy;
        for(int day=2;day<=46;++day){
            assert(simMaterials.AdvanceAllOneDay({
                RealityPublicResponse::None,RealityPublicResponse::None,
                RealityPublicResponse::None,RealityPublicResponse::None,
                RealityPublicResponse::None,RealityPublicResponse::None,
                RealityPublicResponse::None,RealityPublicResponse::None,
                RealityPublicResponse::None},(next()%7u)==0u));
            assert(simEconomy.AdvanceOneDay(day,(next()%5u)==0u));
            const int action=static_cast<int>(next()%6u);
            const auto route=static_cast<TradeRoute>(next()%2u);
            if(simEconomy.Cargo().route==-1){
                if(action<3)simEconomy.Collect(route,day,simMaterials);
            }else if(action<4){
                simEconomy.Deliver(route,day,simMaterials);
            }else{
                const auto held=simEconomy.Cargo().route;
                const TradeRoute wrong=static_cast<TradeRoute>(1-held);
                const auto before=simEconomy.Save();
                assert(simEconomy.Deliver(wrong,day,simMaterials)==TradeResult::Blocked);
                assert(simEconomy.Save().cargo.route==before.cargo.route);
            }
            simEconomy.PayArrears(static_cast<Realm>(next()%9u));
            if((next()%11u)==0u)simEconomy.PayAllBesArrears();
            const auto saved=simEconomy.Save();
            LivingEconomy restored;
            assert(restored.Restore(saved,simMaterials,day));
            const auto restoredState=restored.Save();
            for(int i=0;i<9;++i){
                const auto& actual=restoredState.workers[i];
                const auto& expected=saved.workers[i];
                assert(actual.employerMarks==expected.employerMarks&&
                       actual.earnedMarks==expected.earnedMarks&&
                       actual.owedMarks==expected.owedMarks&&
                       actual.shiftsPaid==expected.shiftsPaid&&
                       actual.lastPaidDay==expected.lastPaidDay&&
                       actual.deliveriesWitnessed==expected.deliveriesWitnessed);
            }
            for(int i=0;i<2;++i){
                const auto& actual=restoredState.routes[i];
                const auto& expected=saved.routes[i];
                assert(actual.foreignStock==expected.foreignStock&&
                       actual.deliveries==expected.deliveries&&
                       actual.paidMarks==expected.paidMarks&&
                       actual.consumedSupport==expected.consumedSupport);
            }
            assert(restoredState.cargo.route==saved.cargo.route);
            for(int realm=0;realm<9;++realm){
                const auto* town=simMaterials.Town(static_cast<Realm>(realm));
                const auto* worker=restored.Worker(static_cast<Realm>(realm));
                assert(town&&town->store>=0&&town->integrity>=0&&town->integrity<=100);
                assert(worker&&worker->employerMarks>=0&&worker->earnedMarks>=0&&worker->owedMarks>=0);
            }
        }
    }
    std::cout<<"PASS: named workers, conserved wages/escrow, two physical trade receipts, separate foreign goods, shortage support and corrupt snapshots\n";
}
