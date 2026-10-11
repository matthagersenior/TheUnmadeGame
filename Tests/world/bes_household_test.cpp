#include "World/UnmadeBesHouseholdRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main(){
    MaterialWorld world;
    LivingEconomy wages;
    BesHousehold bes;
    const std::array<RealityPublicResponse,9> civic{};
    assert(bes.InspectHome(1)==BesOutcome::Blocked);
    assert(bes.HearBes(wages,1)==BesOutcome::Blocked);
    assert(bes.HearRheva(1)==BesOutcome::Blocked);
    // A real unpaid claim arises only after finite town treasury has funded
    // its 16 initial shifts; the complaint cannot be forged at game start.
    for(int day=2;day<=18;++day){
        assert(world.AdvanceAllOneDay(civic));
        assert(wages.AdvanceOneDay(day));
        assert(bes.AdvanceOneDay(day));
    }
    const auto* w=wages.Worker(Realm::HearthBeneath);
    assert(w->employerMarks==0 && w->earnedMarks==32 && w->owedMarks==2);
    assert(bes.HearBes(wages,18)==BesOutcome::Reported);
    assert(bes.HearRheva(18)==BesOutcome::Blocked);
    assert(bes.InspectHome(18)==BesOutcome::HomeInspected);
    assert(bes.InspectHome(18)==BesOutcome::NoChange);
    assert(bes.HearRheva(18)==BesOutcome::LedgerCorroborated);
    assert(bes.HearRheva(18)==BesOutcome::NoChange);
    assert(bes.DeliverHome(world,18)==BesOutcome::Blocked);
    assert(world.Collect(Realm::HearthBeneath,18)==MaterialAction::Collected);
    const int store=world.Town(Realm::HearthBeneath)->store;
    assert(bes.DeliverHome(world,18)==BesOutcome::WarmthDelivered);
    assert(world.Cargo().realm==-1);
    assert(world.Town(Realm::HearthBeneath)->store==store);
    assert(bes.Save().heatBaskets==5);
    assert(bes.DeliverHome(world,18)==BesOutcome::Blocked);
    assert(bes.BeginWalkout(wages,18)==BesOutcome::WalkoutBegun);
    auto savedWorld=world.Snapshot();
    MaterialWorld alternative;
    assert(alternative.Restore(savedWorld,world.Cargo(),18));
    assert(alternative.AdvanceAllOneDay(civic,false));
    assert(world.AdvanceAllOneDay(civic,true));
    assert(alternative.Town(Realm::HearthBeneath)->produced==
           world.Town(Realm::HearthBeneath)->produced+2);
    const auto beforeWages=wages.Worker(Realm::HearthBeneath)->owedMarks;
    const auto shifts=wages.Worker(Realm::HearthBeneath)->shiftsPaid;
    assert(wages.AdvanceOneDay(19,true));
    assert(bes.AdvanceOneDay(19));
    assert(bes.Save().daysWalkedOut==1);
    assert(wages.Worker(Realm::HearthBeneath)->owedMarks==beforeWages);
    assert(wages.Worker(Realm::HearthBeneath)->shiftsPaid==shifts);
    assert(bes.Save().heatBaskets==4);
    assert(bes.SettleFromTown(wages,19)==BesOutcome::Blocked);
    // Mera's explicit and individually sourced consent is required before
    // any person may transfer HER owned wages.
    assert(bes.AcceptMeraGuarantee(wages,19)==BesOutcome::Blocked);
    assert(bes.OfferMeraGuarantee(19)==BesOutcome::ConsentRecorded);
    assert(bes.OfferMeraGuarantee(19)==BesOutcome::Blocked);
    assert(bes.AcceptMeraGuarantee(wages,19)==BesOutcome::GuaranteePaid);
    assert(bes.Save().guaranteeOwed==2);
    assert(bes.Phase()==BesPhase::Underwritten);
    assert(bes.EndgameWitness());
    assert(wages.Worker(Realm::WidowedRain)->earnedMarks==30);
    assert(wages.Worker(Realm::HearthBeneath)->earnedMarks==34);
    assert(wages.Worker(Realm::HearthBeneath)->owedMarks==0);
    assert(bes.AcceptMeraGuarantee(wages,19)==BesOutcome::Blocked);
    assert(bes.SettleFromTown(wages,19)==BesOutcome::Blocked);
    auto receipt=bes.Save();
    BesHousehold loaded;
    assert(loaded.Restore(receipt,19));
    assert(loaded.EndgameWitness());
    auto invalid=receipt;invalid.guaranteeOwed=9;
    assert(!loaded.Restore(invalid,19));
    invalid=receipt;invalid.meraConsented=false;
    assert(!loaded.Restore(invalid,19));
    invalid=receipt;invalid.homeVisited=false;
    assert(!loaded.Restore(invalid,19));
    assert(loaded.Save().guaranteeOwed==2);
    // No invented second payment; a genuinely funded municipal treasury could
    // later pay the guarantor and establish a different saved outcome.
    auto ledger=wages.Save();
    ledger.workers[2].employerMarks=2;
    ledger.workers[1].earnedMarks-=2;
    // Legitimate external transfer would conserve marks; simulate funded
    // employer using an actual source-owned transfer from Mera.
    assert(wages.Restore(ledger,world,19));
    assert(bes.SettleFromTown(wages,19)==BesOutcome::WagesSettled);
    assert(bes.Phase()==BesPhase::Settled);
    assert(bes.Save().guaranteeOwed==0);
    assert(wages.Worker(Realm::HearthBeneath)->employerMarks==0);
    assert(wages.Worker(Realm::WidowedRain)->earnedMarks==30);
    assert(bes.SettleFromTown(wages,19)==BesOutcome::Blocked);
    std::cout<<"PASS: Bes named unpaid shifts, true home packs, real strike production, consented Mera loan, valid preservation and restitution\n";
}
