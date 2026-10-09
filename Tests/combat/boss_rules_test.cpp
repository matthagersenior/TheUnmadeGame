#include "Combat/UnmadeBossRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main(){
    for(int i=0;i<3;++i){
        const auto id=static_cast<BossId>(i);
        const auto* p=FindBoss(id);
        assert(p && p->health>200 && p->rangeCm>=250);
        assert(std::strlen(p->name)>20 && std::strlen(p->weakness)>20);
        for(int j=0;j<i;++j)assert(std::strcmp(p->name,FindBoss(static_cast<BossId>(j))->name)!=0);
        BossEncounter boss(id);
        auto beat=boss.Advance(1,1.0,p->rangeCm-1,false);
        assert(beat.action==BossAction::Telegraph && beat.damage==0);
        assert(boss.Advance(1.2,1.0,p->rangeCm-1,false).action==BossAction::Telegraph);
        beat=boss.Advance(5,1.0,p->rangeCm-1,false);
        assert(beat.action==(id==BossId::HollowBell?BossAction::Shockwave:BossAction::Strike) &&
               beat.damage==p->damage);
        assert(boss.Advance(5.1,.5,p->rangeCm-1,false).action==BossAction::Idle);
        beat=boss.Advance(10,.2,p->rangeCm-1,false);
        assert(beat.action==BossAction::Telegraph && beat.phase==BossPhase::Desperate);
        assert(boss.Advance(10.1,.2,p->rangeCm-1,true).action==BossAction::Stagger);
        assert(boss.Advance(12,.2,p->rangeCm-1,false).action==BossAction::Telegraph);
        assert(boss.Advance(16,0,p->rangeCm-1,false).action==BossAction::Defeated);
        assert(boss.Advance(-1,.5,10,false).action==BossAction::Idle);
    }
    BossEncounter curator(BossId::RedactedCurator);
    assert(curator.Advance(1,1,125,false).action==BossAction::Retreat);
    BossEncounter pilgrim(BossId::UnfinishedPilgrim);
    assert(pilgrim.Advance(1,1,550,false).action==BossAction::Charge);
    BossEncounter bell(BossId::HollowBell);
    auto beat=bell.Advance(0,1,2000,false);
    assert(beat.action==BossAction::Approach);
    assert(bell.Advance(1,1,400,false).action==BossAction::Telegraph);
    assert(bell.Advance(4,1,900,false).action==BossAction::Approach);
    assert(bell.Advance(4.2,1,300,false).action==BossAction::Idle);
    assert(BossEncounter(BossId::None).Advance(1,1,100,false).action==BossAction::Idle);
    std::cout<<"PASS: three named bosses, three phases, telegraphs, fracture counters, dodges and no instant hits\n";
}
