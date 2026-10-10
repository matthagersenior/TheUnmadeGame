#include "World/UnmadeFrontierHazardRules.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
using namespace UnmadeCore;
int main() {
    auto rain=SampleFrontierHazard(Realm::WidowedRain,1.0,0,-49300,true,false);
    assert(rain.phase==FrontierHazardPhase::Calm && rain.damage==0);
    rain=SampleFrontierHazard(Realm::WidowedRain,8.0,0,-49300,true,false);
    assert(rain.phase==FrontierHazardPhase::Warning && rain.damage==0);
    rain=SampleFrontierHazard(Realm::WidowedRain,11.0,0,-49300,true,false);
    assert(rain.phase==FrontierHazardPhase::Impact && rain.damage==16);
    assert(rain.pulseId==1);
    assert(SampleFrontierHazard(Realm::WidowedRain,11.9,0,-49300,true,false).pulseId==1);
    assert(SampleFrontierHazard(Realm::WidowedRain,23.0,0,-49300,true,false).pulseId==2);
    assert(SampleFrontierHazard(Realm::HearthBeneath,11.0,0,50700,true,false).damage==22);
    for(auto off: {2500.,-2500.}) {
        assert(SampleFrontierHazard(Realm::WidowedRain,11.0,off,-49300,true,false).damage==0);
    }
    assert(SampleFrontierHazard(Realm::WidowedRain,11.0,0,-53000,true,false).damage==0);
    assert(SampleFrontierHazard(Realm::WidowedRain,11.0,0,-49300,false,false).damage==0);
    assert(SampleFrontierHazard(Realm::WidowedRain,11.0,0,-49300,true,true).damage==0);
    assert(SampleFrontierHazard(Realm::ThreefoldReach,11.0,0,-49300,true,false).damage==0);
    assert(SampleFrontierHazard(Realm::WidowedRain,
        std::numeric_limits<double>::quiet_NaN(),0,-49300,true,false).damage==0);
    assert(SampleFrontierHazard(Realm::WidowedRain,11.0,
        std::numeric_limits<double>::infinity(),-49300,true,false).damage==0);
    std::cout<<"PASS: both distinct telegraphed frontier hazards, release switch, safe routes and finite input\n";
}
