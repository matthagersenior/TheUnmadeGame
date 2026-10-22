#pragma once
// Optional encounter ecology preview. These six existing encounter IDs and
// creature pairings are source-grounded; lair geometry/behavior is proposed
// prototype presentation, NOT an amendment of sealed or disputed lore.
#include <array>
#include <cmath>
namespace UnmadeCore {
enum class HabitatCue {Quiet,Tracks,Warning,Disturbed,Sheltered};
struct CreatureHabitatSite {
 const char* id;
 const char* kind;
 const char* creature;
 const char* inspection;
 double centerX,centerY,denOffsetY;
 double trackRadius,warningRadius,inspectionRadius;
};
inline constexpr std::array<CreatureHabitatSite,6> HabitatSites={{
 {"Reach.AshRoad","JawDen","Cinderjaw","Bone grooves warn of a jaw den. Guard or retreat.",-2250,1940,710,1750,840,260},
 {"Reach.GlassCut","GlassRoost","Veilwatch","Shards vibrate when the watcher turns. The side ribs break sight.",2350,1920,710,1750,850,260},
 {"Bellwold.Forgotten","HoundUndercroft","HollowHound","Tracks lead beneath a silent stone span. The refuge lies elsewhere.",-20200,1700,710,1750,840,260},
 {"Paperhaven.Torn","ArchiveBurrow","Inkmaw","The torn shelves form a burrow. The remaining shelves shield sight.",20300,1900,710,1750,870,260},
 {"Saltwake.Saltglass","TideCave","SaltglassCrawler","A tide-marked passage draws the crawler. Avoid the waterline.",-2100,-48000,710,1800,900,260},
 {"Cinderhold.Slag","SlagMaw","Ashmaw","The slag vents breathe before the maw stirs. Stone braces offer cover.",-2100,52000,710,1750,890,260}
}};
inline HabitatCue HabitatReaction(const CreatureHabitatSite& site,double distance,
 bool covered,bool night,bool inspected) noexcept {
 if(!std::isfinite(distance)||distance<0||distance>site.trackRadius)return HabitatCue::Quiet;
 if(covered&&distance<=site.warningRadius)return HabitatCue::Sheltered;
 const double warning=site.warningRadius+(night?85.0:0.0);
 if(distance<=site.inspectionRadius+(inspected?20.0:0.0))return HabitatCue::Disturbed;
 if(distance<=warning)return HabitatCue::Warning;
 return HabitatCue::Tracks;
}
inline bool HabitatCanInspect(const CreatureHabitatSite& site,double distance,bool sight) noexcept {
 return sight&&std::isfinite(distance)&&distance>=0&&distance<=site.inspectionRadius;
}
inline constexpr bool HabitatGrantsAbility(HabitatCue) noexcept {return false;}
} // namespace UnmadeCore
