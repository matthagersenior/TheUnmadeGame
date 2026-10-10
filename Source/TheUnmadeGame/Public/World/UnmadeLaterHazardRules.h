#pragma once
// The six later lands have different, deterministic danger fields.
// This is C++17 gameplay logic (no Unreal, paid service, or random rolls).
// The Stage==3 intervention disables the local hazard permanently.
#include "World/UnmadeLaterRealmRules.h"
#include "World/UnmadeFrontierHazardRules.h"
#include <array>
#include <cmath>
#include <cstdint>

namespace UnmadeCore {
enum class LaterHazardEffect { Injury, Strain };

struct LaterHazardSpec {
    Realm realm;
    const char* label;
    const char* warning;
    const char* counterplay;
    LaterHazardEffect effect;
    double period,warningAt,impactAt,severity;
    double laneX,halfWidth,minY,maxY; // relative to the isolated realm origin
};

inline constexpr std::array<LaterHazardSpec,6> LaterHazards={{
    {Realm::TidalLedger,"The Debt Tide",
     "The tide starts counting. Shelter to either side or hold guard.",
     "Cross at slack tide; the ledger stones on either flank are safe.",
     LaterHazardEffect::Injury,14,7,11,14,0,570,-710,130},
    {Realm::SkyBelow,"The Falling Note",
     "Harmonics tighten above the lower street. Leave the central chord.",
     "Listen for the silent interval; sidestep the song's narrow band.",
     LaterHazardEffect::Strain,11,5,8,18,250,520,-710,140},
    {Realm::CinderSpine,"The Quarry Split",
     "The stone seam trembles. Reach the sheltered quarry edge or brace.",
     "The eastern survey route stays clear while cracks spread.",
     LaterHazardEffect::Injury,16,8,12,24,-280,530,-570,230},
    {Realm::HundredUnlived,"The Census Sweep",
     "The unasked-for census is reading the road. Step outside the scan.",
     "The sheltered side path preserves anonymity; wait out the scan.",
     LaterHazardEffect::Strain,13,6,9,22,0,410,-740,100},
    {Realm::OrchardOfKings,"The Law Root",
     "The unratified roots are rising. Follow the unbound outer row.",
     "Guard against the roots or use the orchard's unclaimed side route.",
     LaterHazardEffect::Injury,15,8,12,18,230,650,-680,180},
    {Realm::FirstAbsence,"The Unmooring",
     "The horizon doubles. Leave the contradictory axis until it settles.",
     "Hold to the stable memory path; a quiet interval always returns.",
     LaterHazardEffect::Strain,17,9,13,26,-160,570,-700,100}
}};

struct LaterHazardSample {
    FrontierHazardPhase phase=FrontierHazardPhase::Calm;
    LaterHazardEffect effect=LaterHazardEffect::Injury;
    std::uint64_t pulseId=0;
    std::uint64_t sourceId=0;
    int index=-1;
    double severity=0.0;
    const char* warning="";
    const char* counterplay="";
};

inline LaterHazardSample SampleLaterHazard(
    Realm realm,double elapsed,double worldX,double worldY,int firstArcStage) noexcept
{
    const int idx=LaterIndex(realm);
    if(idx<0 || (firstArcStage!=1 && firstArcStage!=2) ||
       !std::isfinite(elapsed) || elapsed<0 || elapsed>1.0e10 ||
       !std::isfinite(worldX) || !std::isfinite(worldY))return {};
    const auto& zone=LaterHazards[idx];
    const auto& region=LaterRealms[idx];
    const double x=worldX-region.centerX, y=worldY-region.centerY;
    // This exclusion is intentional: both investigation clues, the starting
    // platform, safe sidelines and the returned civic settlement remain safe.
    if(std::fabs(x-zone.laneX)>zone.halfWidth ||
       y<zone.minY || y>zone.maxY)return {};
    const double beat=std::fmod(elapsed,zone.period);
    const auto pulse=static_cast<std::uint64_t>(std::floor(elapsed/zone.period))+1;
    const auto source=static_cast<std::uint64_t>(0xF1000+idx);
    if(beat<zone.warningAt)
        return {FrontierHazardPhase::Calm,zone.effect,pulse,source,idx,0,
                zone.warning,zone.counterplay};
    if(beat<zone.impactAt)
        return {FrontierHazardPhase::Warning,zone.effect,pulse,source,idx,0,
                zone.warning,zone.counterplay};
    return {FrontierHazardPhase::Impact,zone.effect,pulse,source,idx,
            zone.severity,zone.warning,zone.counterplay};
}
} // namespace UnmadeCore
