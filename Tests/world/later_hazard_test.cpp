#include "World/UnmadeLaterHazardRules.h"
#include "Combat/UnmadeCombatRules.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
using namespace UnmadeCore;
int main(){
    for(int i=0;i<6;++i){
        const auto& spec=LaterHazards[i];
        assert(spec.realm==LaterRealms[i].realm);
        assert(spec.period>=10 && spec.warningAt<spec.impactAt && spec.impactAt<spec.period);
        assert(spec.severity>0 && spec.label && spec.counterplay);
        const auto r=spec.realm;
        const auto& loc=LaterRealms[i];
        const double x=loc.centerX+spec.laneX;
        const double y=loc.centerY+(spec.minY+spec.maxY)*0.5;
        const auto calm=SampleLaterHazard(r,1,x,y,1);
        const auto warn=SampleLaterHazard(r,spec.warningAt+.1,x,y,1);
        const auto impact=SampleLaterHazard(r,spec.impactAt+.1,x,y,1);
        assert(calm.phase==FrontierHazardPhase::Calm && calm.severity==0);
        assert(warn.phase==FrontierHazardPhase::Warning && warn.severity==0);
        assert(impact.phase==FrontierHazardPhase::Impact);
        assert(impact.severity==spec.severity && impact.index==i);
        assert(impact.pulseId!=0 && impact.sourceId==static_cast<std::uint64_t>(0xF1000+i));
        assert(impact.effect==spec.effect);
        assert(SampleLaterHazard(r,spec.impactAt+1,x,y,1).pulseId==impact.pulseId);
        assert(SampleLaterHazard(r,spec.period+spec.impactAt+.1,x,y,1).pulseId>impact.pulseId);
        assert(SampleLaterHazard(r,spec.impactAt+.1,x,y,0).pulseId==0);
        assert(SampleLaterHazard(r,spec.impactAt+.1,x,y,3).pulseId==0);
        assert(SampleLaterHazard(r,spec.impactAt+.1,x,y,4).pulseId==0);
        assert(SampleLaterHazard(r,spec.impactAt+.1,x+spec.halfWidth+30,y,1).pulseId==0);
        assert(SampleLaterHazard(r,spec.impactAt+.1,x,loc.centerY-1170,1).pulseId==0);
        assert(SampleLaterHazard(r,spec.impactAt+.1,x,loc.centerY+2000,1).pulseId==0);
        assert(SampleLaterHazard(r,-1,x,y,1).pulseId==0);
        assert(SampleLaterHazard(r,std::numeric_limits<double>::infinity(),x,y,1).pulseId==0);
        assert(SampleLaterHazard(r,spec.impactAt+.1,std::nan(""),y,1).pulseId==0);
        assert(SampleLaterHazard(Realm::ThreefoldReach,spec.impactAt+.1,x,y,1).pulseId==0);
        if(i==0||i==2||i==4)assert(spec.effect==LaterHazardEffect::Injury);
        else assert(spec.effect==LaterHazardEffect::Strain);
    }
    Combatant guard(100,24,.75);
    assert(guard.SetGuarding(true));
    const double hit=LaterHazards[0].severity;
    assert(guard.ReceiveHit(0xF1000,1,hit,false)==HitOutcome::Applied);
    assert(guard.Health()==100-hit*.25);
    assert(guard.ReceiveHit(0xF1000,1,hit,false)==HitOutcome::Duplicate);
    FractureModel fracture("region.prototype.hub",{"variant.open","variant.sealed"});
    assert(!fracture.ApplyEnvironmentalStrain(-1));
    assert(!fracture.ApplyEnvironmentalStrain(std::nan("")));
    assert(fracture.ApplyEnvironmentalStrain(LaterHazards[1].severity));
    assert(fracture.CurrentStrain()==LaterHazards[1].severity);
    assert(fracture.ApplyEnvironmentalStrain(1000));
    assert(fracture.CurrentStrain()==100);
    assert(!fracture.ApplyEnvironmentalStrain(3));
    assert(fracture.Recover(2));
    assert(fracture.ApplyEnvironmentalStrain(1));
    assert(fracture.CurrentStrain()==95);
    std::cout<<"PASS: six patterned danger zones, safe approaches, distinct injury/strain, deduplicated pulses\n";
}
