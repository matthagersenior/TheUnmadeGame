#pragma once
// Deterministic cross-system effects. No fabricated omniscient observer,
// remote settlement propagation, hidden moral score, or random rolls.
#include "World/UnmadeTenfoldChronicle.h"
#include <array>
#include <cmath>
#include <cstdint>

namespace UnmadeCore {
enum class RealityPublicResponse : int {
    None=0, SharedPassage=1, PublicInquiry=2, ContestedAccord=3
};
enum class RealityArchitecture : int { None=0, SafeSpan=1, StrainedSpan=2, DoubleThreshold=3 };
struct RealityProfile {
    RiteId rite;
    const char* description;
    RealityArchitecture architecture;
    int enemySpeedPercent; // actual local enemy movement, not free attack damage
    bool care;             // a civic claim, never an absolute moral verdict
};
inline constexpr std::array<RealityProfile,10> RealityProfiles = {{
    {RiteId::UnwriteLaw,"The suspended law makes footings uncertain.",RealityArchitecture::StrainedSpan,140,false},
    {RiteId::Witnesscraft,"Independent testimony briefly supports a crossing.",RealityArchitecture::SafeSpan,75,true},
    {RiteId::BorrowedLives,"A second life overlays the threshold.",RealityArchitecture::DoubleThreshold,120,false},
    {RiteId::LegacyForging,"Remembered labor braces the public way.",RealityArchitecture::SafeSpan,85,true},
    {RiteId::LivingRoads,"An open path briefly belongs to all travelers.",RealityArchitecture::SafeSpan,80,true},
    {RiteId::TomorrowDebt,"Borrowed time strains the neighboring structure.",RealityArchitecture::StrainedSpan,130,false},
    {RiteId::UnderstandingBosses,"A remembered enemy finds room to yield.",RealityArchitecture::SafeSpan,70,true},
    {RiteId::ParadoxConvergence,"The structure holds two incompatible histories.",RealityArchitecture::DoubleThreshold,125,false},
    {RiteId::Oathbinding,"A promise supports the refuge approach.",RealityArchitecture::SafeSpan,75,true},
    {RiteId::Cartography,"Verified paths make the approach legible.",RealityArchitecture::SafeSpan,90,true}
}};
struct RealityConsequenceSnapshot {
    std::uint16_t careWitnessMask=0;
    std::uint16_t inquiryWitnessMask=0;
    int activeRite=-1; // temporary event, or -1 if no active event
    double activeUntil=0;
};
struct RealityConsequenceView {
    RealityArchitecture architecture=RealityArchitecture::None;
    int enemySpeedPercent=100;
    bool care=false;
    bool active=false;
};
class RealityConsequenceLedger final {
public:
    const RealityConsequenceSnapshot& Snapshot()const noexcept{return state_;}
    static constexpr int MaxDistinctRites=10;
    static bool ValidId(int rite) noexcept {return rite>=0 && rite<MaxDistinctRites;}
    static int Count(std::uint16_t mask)noexcept {
        int n=0;
        while(mask){n+=mask&1;mask=static_cast<std::uint16_t>(mask>>1);}
        return n;
    }
    static bool ValidMask(std::uint16_t care,std::uint16_t inquiry)noexcept {
        if((care|inquiry)&~std::uint16_t{1023})return false;
        if(care&inquiry)return false;
        for(int i=0;i<MaxDistinctRites;++i){
            const auto bit=std::uint16_t{1}<<i;
            if(RealityProfiles[i].care && (inquiry&bit))return false;
            if(!RealityProfiles[i].care && (care&bit))return false;
        }
        return true;
    }
    bool Restore(RealityConsequenceSnapshot saved,double worldTime)noexcept {
        if(!std::isfinite(worldTime)||worldTime<0 ||
           !ValidMask(saved.careWitnessMask,saved.inquiryWitnessMask)||
           !std::isfinite(saved.activeUntil)||saved.activeUntil<0 ||
           (saved.activeRite!=-1 && !ValidId(saved.activeRite))||
           (saved.activeRite==-1 && saved.activeUntil!=0)||
           (saved.activeRite!=-1 && saved.activeUntil<=0))return false;
        // Elapsed casts leave only the distinct, publicly witnessed civic history.
        if(saved.activeRite!=-1 && saved.activeUntil<=worldTime){
            saved.activeRite=-1;saved.activeUntil=0;
        }
        state_=saved;return true;
    }
    bool Cast(RiteId rite,double worldTime,double duration,
              bool mastered,bool physicallyWitnessed,bool insideFocus)noexcept {
        const int index=static_cast<int>(rite);
        if(!insideFocus||!ValidId(index)||!std::isfinite(worldTime)||
           worldTime<0||!std::isfinite(duration)||duration<=0||
           duration>3600)return false;
        state_.activeRite=index;
        state_.activeUntil=worldTime+duration;
        // Only two distinct, mastered, genuinely witnessed rites can
        // support a permanent civic response; spam cannot buy a verdict.
        if(mastered && physicallyWitnessed){
            auto& mask=RealityProfiles[index].care
                ?state_.careWitnessMask:state_.inquiryWitnessMask;
            mask|=static_cast<std::uint16_t>(1u<<index);
        }
        return true;
    }
    RealityConsequenceView Active(double now)const noexcept {
        if(!std::isfinite(now)||now<0||state_.activeRite<0 ||
           now>=state_.activeUntil)return {};
        const auto& p=RealityProfiles[state_.activeRite];
        return {p.architecture,p.enemySpeedPercent,p.care,true};
    }
    RealityPublicResponse PublicResponse()const noexcept {
        const bool care=Count(state_.careWitnessMask)>=2;
        const bool inquiry=Count(state_.inquiryWitnessMask)>=2;
        if(care && inquiry)return RealityPublicResponse::ContestedAccord;
        if(care)return RealityPublicResponse::SharedPassage;
        if(inquiry)return RealityPublicResponse::PublicInquiry;
        return RealityPublicResponse::None;
    }
    static const char* PublicLine(RealityPublicResponse response)noexcept {
        switch(response){
        case RealityPublicResponse::SharedPassage:
            return "Two distinct witnessed rites support a public passage; it protects some travelers while the city's original debts remain.";
        case RealityPublicResponse::PublicInquiry:
            return "Two witnessed disruptions lead to a public engineering inquiry; questioning a wall does not establish guilt.";
        case RealityPublicResponse::ContestedAccord:
            return "Witnesses have records of both care and disruption. The city keeps a protected way and a separate public inquiry.";
        default:
            return "The Crossings has not heard two distinct, mastered and witnessed rites making the same civic claim.";
        }
    }
private:
    RealityConsequenceSnapshot state_{};
};
} // namespace UnmadeCore
