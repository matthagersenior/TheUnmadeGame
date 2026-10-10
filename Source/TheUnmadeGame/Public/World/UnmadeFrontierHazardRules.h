#pragma once
// Deterministic offline fairness model for the storm and ember interventions.
// Pulses are world-clock anchored; guarding works through Combatant::ReceiveHit.
// Only the two actor-backed frontier realms participate (future realms are data).
#include "World/UnmadeWorldAtlas.h"
#include <cmath>
#include <cstdint>
namespace UnmadeCore {
enum class FrontierHazardPhase { Calm, Warning, Impact };
struct FrontierHazardSample {
    FrontierHazardPhase phase=FrontierHazardPhase::Calm;
    std::uint64_t pulseId=0; // stable within one pulse, unique across cycles
    double damage=0.0;
};
inline FrontierHazardSample SampleFrontierHazard(
    Realm realm,double elapsedSeconds,double worldX,double worldY,
    bool storyStarted,bool controlReleased) noexcept {
    if(!storyStarted || controlReleased ||
       !std::isfinite(elapsedSeconds) || elapsedSeconds<0.0 ||
       elapsedSeconds>1.0e10 ||
       !std::isfinite(worldX) || !std::isfinite(worldY))
        return {};
    double centerY=0;
    double damage=0;
    if(realm==Realm::WidowedRain){centerY=-50000.0;damage=16.0;}
    else if(realm==Realm::HearthBeneath){centerY=50000.0;damage=22.0;}
    else return {};
    const double relativeY=worldY-centerY;
    // No unavoidable whole-settlement damage: sheltered side passages,
    // witnesses, shops and the arrival point stay safe.
    if(std::fabs(worldX)>1500.0 || relativeY<200.0 || relativeY>1450.0)
        return {};
    constexpr double Period=12.0;
    const double beat=std::fmod(elapsedSeconds,Period);
    const auto id=static_cast<std::uint64_t>(std::floor(elapsedSeconds/Period))+1;
    if(beat<7.0)return {FrontierHazardPhase::Calm,id,0};
    if(beat<10.0)return {FrontierHazardPhase::Warning,id,0};
    return {FrontierHazardPhase::Impact,id,damage};
}
} // namespace UnmadeCore
