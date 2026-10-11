#include "World/UnmadeRealmReactionRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main(){
    assert(RealmReactionPolicies.size()==9);
    for(int i=0;i<9;++i){
        const auto& p=RealmReactionPolicies[i];
        assert(static_cast<int>(p.realm)==i);
        assert(PolicyFor(p.realm)==&p);
        assert(RealmReactionAt(p.originX,p.originY)==&p);
        assert(p.careHazard>=55&&p.inquiryHazard<=150);
        assert(std::strlen(p.material)>5 && std::strlen(p.institution)>5);
        assert(std::strlen(p.dependence)>5);
        assert(RealmReactionAt(p.originX+9000,p.originY)==nullptr ||
               RealmReactionAt(p.originX+9000,p.originY)!=&p);
    }
    assert(RealmReactionAt(0,0)->realm==Realm::ThreefoldReach);
    assert(RealmReactionAt(0,-50000)->realm==Realm::WidowedRain);
    assert(RealmReactionAt(0,50000)->realm==Realm::HearthBeneath);
    assert(RealmReactionAt(100000,-50000)->realm==Realm::TidalLedger);
    assert(RealmReactionAt(-100000,-50000)->realm==Realm::SkyBelow);
    assert(RealmReactionAt(100000,50000)->realm==Realm::CinderSpine);
    assert(RealmReactionAt(-100000,50000)->realm==Realm::HundredUnlived);
    assert(RealmReactionAt(200000,0)->realm==Realm::OrchardOfKings);
    assert(RealmReactionAt(-200000,0)->realm==Realm::FirstAbsence);
    assert(RealmReactionAt(12345,34567)==nullptr);
    RealityConsequenceLedger first,storm,choir,heat;
    assert(first.Cast(RiteId::LivingRoads,10,20,true,true,true));
    assert(storm.Cast(RiteId::LivingRoads,10,20,true,true,true));
    assert(choir.Cast(RiteId::LivingRoads,10,20,true,true,true));
    assert(heat.Cast(RiteId::TomorrowDebt,10,20,true,true,true));
    auto rain=RealmEffect(Realm::WidowedRain,storm,11);
    auto air=RealmEffect(Realm::SkyBelow,choir,11);
    auto hearth=RealmEffect(Realm::HearthBeneath,heat,11);
    assert(rain.active && air.active && hearth.active);
    assert(rain.hazardPercent==65 && air.hazardPercent==75);
    assert(hearth.hazardPercent==125);
    assert(rain.enemySpeedPercent!=air.enemySpeedPercent);
    assert(!RealmEffect(Realm::WidowedRain,storm,31).active);
    assert(RealmEffect(Realm::ThreefoldReach,first,11).tradeTrustBonus==20);
    OuterRealmReactions all;
    assert(!all.Read(Realm::ThreefoldReach));
    assert(!all.Cast(Realm::ThreefoldReach,RiteId::LivingRoads,1,6,true,true));
    assert(all.Cast(Realm::WidowedRain,RiteId::LivingRoads,1,6,true,true));
    assert(all.Cast(Realm::WidowedRain,RiteId::Oathbinding,9,6,true,true));
    assert(all.Read(Realm::WidowedRain)->PublicResponse()==RealityPublicResponse::SharedPassage);
    assert(all.Read(Realm::HearthBeneath)->PublicResponse()==RealityPublicResponse::None);
    assert(all.Cast(Realm::SkyBelow,RiteId::TomorrowDebt,10,9,true,true));
    assert(all.Cast(Realm::SkyBelow,RiteId::ParadoxConvergence,20,9,true,true));
    assert(all.Read(Realm::SkyBelow)->PublicResponse()==RealityPublicResponse::PublicInquiry);
    auto saved=all.Save();
    OuterRealmReactions restored;
    assert(restored.Restore(saved,30));
    assert(restored.Read(Realm::SkyBelow)->PublicResponse()==RealityPublicResponse::PublicInquiry);
    assert(restored.Read(Realm::WidowedRain)->PublicResponse()==RealityPublicResponse::SharedPassage);
    assert(!restored.Read(Realm::SkyBelow)->Active(30).active);
    const auto before=restored.Save();
    auto corrupt=saved;
    corrupt[1].careWitnessMask=0x400;
    assert(!restored.Restore(corrupt,30));
    assert(restored.Save()[1].careWitnessMask==before[1].careWitnessMask);
    corrupt=saved;
    corrupt[2].activeRite=100;
    assert(!restored.Restore(corrupt,25));
    assert(restored.Save()[1].careWitnessMask==before[1].careWitnessMask);
    std::cout<<"PASS: nine realm identities, nonhomogeneous physics, isolated civic proofs, atomic restoration\n";
}
