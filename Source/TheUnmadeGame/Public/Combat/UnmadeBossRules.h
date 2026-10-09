#pragma once
// THE UNMADE — deterministic named boss behaviors. Standalone C++17, no AI.
#include <algorithm>
#include <cmath>
#include <cstdint>
namespace UnmadeCore {
enum class BossId { None=-1, HollowBell=0, RedactedCurator=1, UnfinishedPilgrim=2, Count=3 };
enum class BossPhase { Resolute, Fractured, Desperate };
enum class BossAction { Idle, Approach, Retreat, Charge, Telegraph, Strike, Shockwave, Stagger, Defeated };
struct BossProfile {
    BossId id;
    const char* name;
    const char* location;
    const char* weakness;
    double health;
    double damage;
    double rangeCm;
    double windupSeconds;
    double recoverySeconds;
};
inline const BossProfile* FindBoss(BossId id) noexcept {
    static constexpr BossProfile Profiles[]={
        {BossId::HollowBell,"The Bell That Buried Its Keeper","Bellwold ossuary",
         "Fold interrupts the toll",300,28,450,2.0,2.5},
        {BossId::RedactedCurator,"The Curator of Missing Names","Paperhaven black archive",
         "Glimpse reveals the erased signature",260,24,720,1.5,2.25},
        {BossId::UnfinishedPilgrim,"The Unfinished Pilgrim","Crossings broken causeway",
         "Fold exposes the path between charges",340,32,300,1.8,2.75}
    };
    const int i=static_cast<int>(id);
    return i>=0 && i<3 ? &Profiles[i] : nullptr;
}
struct BossBeat {
    BossAction action=BossAction::Idle;
    BossPhase phase=BossPhase::Resolute;
    double damage=0;
};
class BossEncounter final {
public:
    explicit BossEncounter(BossId id=BossId::None) noexcept : kind_(id) {}
    BossId Kind() const noexcept { return kind_; }
    BossBeat Advance(double now, double healthFraction, double distanceCm,
                     bool fractureExposure) noexcept {
        const auto* spec=FindBoss(kind_);
        if(!spec || !std::isfinite(now) || now<0 || !std::isfinite(healthFraction) ||
           healthFraction<0 || healthFraction>1 || !std::isfinite(distanceCm) || distanceCm<0)
            return {};
        const BossPhase phase = healthFraction<=0.3 ? BossPhase::Desperate
            : healthFraction<=0.65 ? BossPhase::Fractured : BossPhase::Resolute;
        if(healthFraction==0) return {BossAction::Defeated,phase,0};
        if(fractureExposure) {
            charging_=false;
            nextReady_=std::max(nextReady_,now+1.5);
            return {BossAction::Stagger,phase,0};
        }
        if(charging_) {
            if(now<telegraphUntil_) return {BossAction::Telegraph,phase,0};
            charging_=false;
            nextReady_=now+spec->recoverySeconds;
            // Leaving the radius or blocking line of sight stops the telegraphed hit.
            if(distanceCm>spec->rangeCm) return {BossAction::Approach,phase,0};
            const double multiplier=phase==BossPhase::Desperate?1.45:
                                    phase==BossPhase::Fractured?1.2:1.0;
            return {kind_==BossId::HollowBell ? BossAction::Shockwave : BossAction::Strike,
                    phase,spec->damage*multiplier};
        }
        if(kind_==BossId::RedactedCurator && distanceCm<230.0)
            return {BossAction::Retreat,phase,0};
        if(distanceCm>spec->rangeCm)
            return {kind_==BossId::UnfinishedPilgrim && distanceCm<1600.0
                ? BossAction::Charge : BossAction::Approach,phase,0};
        if(now<nextReady_) return {BossAction::Idle,phase,0};
        // Every strike requires an earlier, observable warning.
        const double windup=spec->windupSeconds*(phase==BossPhase::Desperate?0.7:1.0);
        charging_=true;
        telegraphUntil_=now+windup;
        return {BossAction::Telegraph,phase,0};
    }
    void Reset() noexcept { charging_=false;telegraphUntil_=0;nextReady_=0; }
private:
    BossId kind_=BossId::None;
    bool charging_=false;
    double telegraphUntil_=0;
    double nextReady_=0;
};
}
