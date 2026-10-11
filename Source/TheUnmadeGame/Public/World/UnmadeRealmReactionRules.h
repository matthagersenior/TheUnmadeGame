#pragma once
// The nine realms share a causal grammar but NOT one global civic or physical policy.
// Realm 0 retains its pre-v2 RealityConsequenceLedger and v1 save fields.
#include "World/UnmadeRealityConsequenceRules.h"
#include "World/UnmadeFrontierRealmRules.h"
#include "World/UnmadeLaterRealmRules.h"
#include <array>
#include <algorithm>
#include <cmath>

namespace UnmadeCore {
struct RealmReactionPolicy {
    Realm realm;
    const char* city;
    const char* material;
    const char* institution;
    const char* dependence;
    double originX,originY,radius;
    int enemyBias;       // distinct hostility and cover behavior, percent points
    int careHazard;      // percentage of local existing hazard severity
    int inquiryHazard;   // exposes local instability instead of a cost-free buff
    int publicTradeTrust; // only used where an actual local goods market exists
    const char* careLine;
    const char* inquiryLine;
};
inline constexpr std::array<RealmReactionPolicy,9> RealmReactionPolicies = {{
    {Realm::ThreefoldReach,"The Crossings","contracted stone","Bridge Ledger Court",
     "shared passage and owed labor",0,0,2600,0,80,115,20,
     "A witnessed safe passage carries a debt to its builders.",
     "The survey court demands a record without naming protected guests."},
    {Realm::WidowedRain,"Saltwake","counted rainwater","rainward tally",
     "fresh water and dry-harbor tolls",0,-50000,2900,-5,65,130,0,
     "Witnesses divert stormwater toward a shared cistern.",
     "The port audits the rain merchants' claims before releasing water."},
    {Realm::HearthBeneath,"Cinderhold","borrowed embers","hearth assembly",
     "winter warmth and shared kilns",0,50000,2900,-10,70,125,0,
     "The common kiln receives a safer channel of warmth.",
     "The assembly investigates who redirected another family's heat."},
    {Realm::TidalLedger,"Drevlach","future-dated tide","debt adjudicators",
     "collateral and ferry dues",100000,-50000,3200,10,90,135,0,
     "The ferry carries passengers without binding their next day's wages.",
     "The lenders must show the future-dated receipts in public."},
    {Realm::SkyBelow,"Orravane","load-bearing song","inverted choir",
     "resonant supports and evacuation", -100000,-50000,3200,15,75,140,0,
     "Singers hold the walkway through a shared counterweight verse.",
     "The choir tests whether missing notes have weakened inhabited roofs."},
    {Realm::CinderSpine,"Vathless","ancestral stone","quarry seam council",
     "wages and occupied quarry edges",100000,50000,3200,10,80,130,0,
     "Workers reinforce the safe seam without erasing its lineage.",
     "The seam council records the quarry's displaced builders."},
    {Realm::HundredUnlived,"Eillun","unclaimed doorways","consent census",
     "shelter and protected identity",-100000,50000,3200,-5,60,120,0,
     "An unseen guest receives passage without compulsory registration.",
     "The census must ask permission before publishing an impossible name."},
    {Realm::OrchardOfKings,"Tharniv","unratified roots","untitled court",
     "communal grafts and inheritance",200000,0,3200,0,75,125,0,
     "A common rootway survives without awarding ownership to a crown.",
     "The court examines the buried law that raised the roots."},
    {Realm::FirstAbsence,"Auvren","unfinished geometry","first witnesses",
     "stable memory and competing origins",-200000,0,3200,20,85,145,0,
     "A shared memory anchors a usable threshold without erasing others.",
     "Two incompatible origin charts remain visible for comparison."}
}};
inline const RealmReactionPolicy* RealmReactionAt(double x,double y)noexcept {
    if(!std::isfinite(x)||!std::isfinite(y))return nullptr;
    for(const auto& p:RealmReactionPolicies) {
        const double dx=x-p.originX,dy=y-p.originY;
        if(dx*dx+dy*dy<=p.radius*p.radius)return &p;
    }
    return nullptr;
}
inline const RealmReactionPolicy* PolicyFor(Realm realm)noexcept {
    const int i=static_cast<int>(realm);
    return i>=0&&i<9?&RealmReactionPolicies[i]:nullptr;
}
struct RealmReactionView {
    RealityArchitecture architecture=RealityArchitecture::None;
    int enemySpeedPercent=100;
    int hazardPercent=100;
    int tradeTrustBonus=0;
    bool active=false;
};
inline RealmReactionView RealmEffect(Realm realm,
                                    const RealityConsequenceLedger& ledger,
                                    double now)noexcept {
    const auto* policy=PolicyFor(realm);
    if(!policy)return {};
    const auto pulse=ledger.Active(now);
    if(!pulse.active)return {};
    const int enemy=std::clamp(pulse.enemySpeedPercent+policy->enemyBias,55,160);
    const int hazard=pulse.care?policy->careHazard:policy->inquiryHazard;
    const int trust=pulse.care?policy->publicTradeTrust:0;
    return {pulse.architecture,enemy,hazard,trust,true};
}
// Preserve v1 Reach independently. Only eight outer realm records live here.
class OuterRealmReactions final {
public:
    static constexpr int Count=8;
    using Snapshot=std::array<RealityConsequenceSnapshot,Count>;
    const RealityConsequenceLedger* Read(Realm realm)const noexcept {
        const int idx=static_cast<int>(realm)-1;
        return idx>=0&&idx<Count?&ledger_[idx]:nullptr;
    }
    Snapshot Save()const noexcept {
        Snapshot s{};
        for(int i=0;i<Count;++i)s[i]=ledger_[i].Snapshot();
        return s;
    }
    bool Restore(const Snapshot& s,double worldTime)noexcept {
        auto next=ledger_;
        for(int i=0;i<Count;++i)
            if(!next[i].Restore(s[i],worldTime))return false;
        ledger_=next;return true;
    }
    bool Cast(Realm realm,RiteId rite,double now,double duration,
              bool mastered,bool witnessed)noexcept {
        const int idx=static_cast<int>(realm)-1;
        if(idx<0||idx>=Count)return false;
        return ledger_[idx].Cast(rite,now,duration,mastered,witnessed,true);
    }
private:
    std::array<RealityConsequenceLedger,Count> ledger_{};
};
} // namespace UnmadeCore
