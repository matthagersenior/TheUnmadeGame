#include "World/UnmadeRealityConsequenceRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main() {
    RealityConsequenceLedger world;
    assert(RealityProfiles.size()==10);
    for(int i=0;i<10;++i){
        assert(static_cast<int>(RealityProfiles[i].rite)==i);
        assert(RealityProfiles[i].enemySpeedPercent>=60 &&
               RealityProfiles[i].enemySpeedPercent<=145);
        assert(RealityProfiles[i].architecture!=RealityArchitecture::None);
        assert(std::strlen(RealityProfiles[i].description)>15);
    }
    assert(world.PublicResponse()==RealityPublicResponse::None);
    assert(world.Active(2).enemySpeedPercent==100);
    assert(!world.Cast(RiteId::Count,0,8,true,true,true));
    assert(!world.Cast(RiteId::UnwriteLaw,0,-2,true,true,true));
    assert(!world.Cast(RiteId::UnwriteLaw,0,8,true,true,false));
    assert(world.Cast(RiteId::UnwriteLaw,10,8,false,false,true));
    assert(world.Active(11).architecture==RealityArchitecture::StrainedSpan);
    assert(world.Active(11).enemySpeedPercent==140);
    assert(!world.Active(18).active);
    assert(world.PublicResponse()==RealityPublicResponse::None);
    assert(world.Cast(RiteId::Witnesscraft,20,10,true,false,true));
    assert(world.Snapshot().careWitnessMask==0); // no invented witnesses
    assert(world.Cast(RiteId::Witnesscraft,31,10,true,true,true));
    assert(world.Cast(RiteId::Witnesscraft,42,10,true,true,true));
    assert(RealityConsequenceLedger::Count(world.Snapshot().careWitnessMask)==1);
    assert(world.Cast(RiteId::LivingRoads,55,20,true,true,true));
    assert(world.PublicResponse()==RealityPublicResponse::SharedPassage);
    assert(world.Active(60).enemySpeedPercent==80);
    assert(world.Cast(RiteId::ParadoxConvergence,80,8,true,true,true));
    assert(world.Cast(RiteId::TomorrowDebt,91,8,true,true,true));
    assert(world.PublicResponse()==RealityPublicResponse::ContestedAccord);
    assert(std::strstr(RealityConsequenceLedger::PublicLine(world.PublicResponse()),"both")!=nullptr);
    const auto saved=world.Snapshot();
    RealityConsequenceLedger loaded;
    assert(loaded.Restore(saved,93));
    assert(loaded.PublicResponse()==RealityPublicResponse::ContestedAccord);
    assert(!loaded.Active(99).active);
    assert(!loaded.Restore({1024,0,-1,0},100)); // invalid saved masks
    assert(!loaded.Restore({0,1,-1,0},100)); // wrong doctrine bit for rite 0
    assert(!loaded.Restore({1,0,-1,0},100)); // wrong doctrine bit for rite 0
    assert(!loaded.Restore({0,0,3,0},100)); // no expiry
    assert(!loaded.Restore({0,0,-1,7},100)); // no active ID
    assert(!loaded.Restore({0,0,10,123},100)); // out of range
    assert(!loaded.Restore({0,0,1,100},-1)); // invalid world clock
    assert(loaded.Snapshot().inquiryWitnessMask==saved.inquiryWitnessMask);
    RealityConsequenceLedger expired;
    assert(expired.Restore(saved,100));
    assert(expired.Snapshot().activeRite==-1);
    assert(expired.PublicResponse()==RealityPublicResponse::ContestedAccord);
    RealityConsequenceLedger inquiry;
    assert(inquiry.Cast(RiteId::UnwriteLaw,1,8,true,true,true));
    assert(inquiry.Cast(RiteId::BorrowedLives,15,8,true,true,true));
    assert(inquiry.PublicResponse()==RealityPublicResponse::PublicInquiry);
    RealityConsequenceLedger care;
    assert(care.Cast(RiteId::Oathbinding,1,8,true,true,true));
    assert(care.Cast(RiteId::Cartography,15,8,true,true,true));
    assert(care.PublicResponse()==RealityPublicResponse::SharedPassage);
    std::cout<<"PASS: 10 rite profiles, bounded effects, 2 diverse witnesses, 3 civic outcomes, save safety\n";
}
