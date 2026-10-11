#pragma once
#include "Combat/UnmadeCombatRules.h"
#include <array>

namespace UnmadeCore {
enum class CreatureKind { Cinderjaw, PaleStrider, Veilwatch, GleamMoth, HollowHound,
    GriefLantern, Inkmaw, RedactionEye, SaltglassCrawler, DrownedWatch, Ashmaw, KilnWisp };
struct CreatureProfile {
    EnemyStyle style; double health, damage, cooldown, speed, windup, reach, retreat;
    const char* warning;
};
inline CreatureProfile CreatureStats(CreatureKind kind, bool champion=false) {
    static const std::array<CreatureProfile,12> profiles={{
        {EnemyStyle::Stalker,85,14,1.4,145,.70,2.2,0,"JAW STRIKE - GUARD OR RETREAT"},
        {EnemyStyle::Stalker,65,11,1.1,205,.85,2.6,0,"STRIDER LUNGE - STEP BACK"},
        {EnemyStyle::Watcher,75,12,2.3,95,1.15,7,3.5,"VEIL PULSE - BREAK SIGHT"},
        {EnemyStyle::Watcher,45,8,1.5,170,.85,5.5,2.5,"GLEAM FLASH - TAKE COVER"},
        {EnemyStyle::Stalker,80,12,1.2,185,.75,2.1,0,"HOUND BITE - GUARD"},
        {EnemyStyle::Watcher,90,16,3,70,1.5,8,4,"GRIEF PEAL - BREAK SIGHT"},
        {EnemyStyle::Stalker,110,18,2,100,1.1,2.4,0,"INKMAW CRUSH - RETREAT"},
        {EnemyStyle::Watcher,60,14,2.2,115,1.2,6.5,3,"REDACTION - USE THE SHELVES"},
        {EnemyStyle::Stalker,125,16,1.8,85,1,2.3,0,"CRAWLER SNAP - STEP BACK"},
        {EnemyStyle::Watcher,85,13,2.6,80,1.35,7.5,4,"DROWNED PULSE - TAKE COVER"},
        {EnemyStyle::Stalker,140,22,2.5,105,1.3,2.6,0,"ASHMAW SLAM - RETREAT"},
        {EnemyStyle::Watcher,50,10,1.8,155,1,6,2.8,"KILN FLARE - BREAK SIGHT"}
    }};
    const auto index=static_cast<unsigned>(kind);
    auto p=profiles[index<profiles.size()?index:0];
    if(champion) { p.health*=1.8; p.damage*=1.2; p.windup+=.25; p.cooldown+=.4; }
    return p;
}
inline EnemyIntent CreatureIntent(const CreatureProfile& p,double distance,bool sight,
                                 bool recovering) {
    if(!std::isfinite(distance)||distance<0||recovering)return EnemyIntent::Hold;
    if(p.style==EnemyStyle::Watcher&&distance<p.retreat)return EnemyIntent::Retreat;
    return sight&&distance<=p.reach?EnemyIntent::Attack:EnemyIntent::Approach;
}
}
