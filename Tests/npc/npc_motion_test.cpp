#include "NPC/UnmadeNpcMotionRules.h"
#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
using namespace UnmadeCore;
int main() {
    const Vec2 start{0,0};
    auto step = SteerNpc(start,{1000,0},NpcMotion::Approach,200,1.0,100);
    assert(step.x == 50.0 && step.y == 0);
    step = SteerNpc({950,0},{1000,0},NpcMotion::Approach,200,0.1,100);
    assert(step.x == 950);
    step = SteerNpc(start,{100,0},NpcMotion::Retreat,200,1.0,300);
    assert(step.x == -50.0 && step.y == 0);
    step = SteerNpc(start,{1000,0},NpcMotion::Retreat,200,1.0,300);
    assert(step.x == 0);
    step = SteerNpc(start,{1000,0},NpcMotion::Stay,200,0.1,0);
    assert(step.x == 0);
    step = SteerNpc(start,{1000,0},NpcMotion::Approach,200,
                    std::numeric_limits<double>::quiet_NaN(),100);
    assert(step.x == 0);
    step = SteerNpc(start,{1000,0},NpcMotion::Approach,-1,0.1,100);
    assert(step.x == 0);
    std::cout << "PASS: deterministic NPC step, retreat, stay, no teleport\n";
}
