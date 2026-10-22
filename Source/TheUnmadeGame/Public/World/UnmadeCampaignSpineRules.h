#pragma once
// THE UNMADE | The Unanswered Road
// One coherent main plot through existing source-real first-chapter adventures.
// No new save format: derives progress from actual visited/resolved quests.
// Two branches per act prevent a nine-realm completionist checklist.
#include "World/UnmadeWorldAtlas.h"
#include <array>
#include <cstdint>
#include <cstring>
namespace UnmadeCore {
enum class RoadAct : int {
    Opening=0,MissingWorld=1,CostOfAbsence=2,RightToExist=3,
    BeforePlace=4,ConfrontTheAnswer=5,LivingAfterward=6
};
struct RoadBeat {
    Realm realm;
    const char* chapter;
    const char* localQuestion;
    const char* witnessedDiscovery;
    const char* nextDirection;
    const char* publicTag;
};
#include "World/UnmadeCodexCampaignGenerated.inc"
struct RoadEvidence {
    bool openingWitnessed=false;  // starter civic dispute, two villages, or two landmarks
    std::uint16_t firstStories=0;  // 9-bit mask of REAL completed first chapters
    int finalAct=0; // from FinalJourney only
};
// Reconstruct the journal from fields that already existed in schema-1
// saves. Older records have no journal snapshot, so completion receipts and
// final-journey progress must remain authoritative after loading them.
inline RoadEvidence RestoreRoadEvidence(bool openingWitnessed,
                                       std::uint16_t firstStories,
                                       int finalAct) noexcept {
    constexpr std::uint16_t allStories=static_cast<std::uint16_t>(
        (1u<<static_cast<int>(Realm::Count))-1u);
    RoadEvidence out;
    out.firstStories=static_cast<std::uint16_t>(firstStories&allStories);
    out.openingWitnessed=openingWitnessed||out.firstStories!=0||finalAct!=0;
    out.finalAct=finalAct;
    return out;
}
struct RoadGuidance {
    RoadAct act=RoadAct::Opening;
    const char* title="";
    const char* question="";
    const char* objective="";
    std::array<Realm,3> alternatives{{Realm::Count,Realm::Count,Realm::Count}};
    int available=0;
    bool bossAccessible=false;
};
inline constexpr std::uint16_t RoadMask(Realm r) noexcept {
    const int i=static_cast<int>(r);
    return (i>=0&&i<9)?static_cast<std::uint16_t>(1u<<i):0;
}
inline constexpr bool RoadHas(const RoadEvidence& state,Realm r)noexcept {
    return (state.firstStories&RoadMask(r))!=0;
}
// Relics offer early shortcuts; completed FIRST chapters independently
// make the Unanswered Road traversable, without the optional forge or landmarks.
inline int RoadTravelAttunement(const RoadEvidence& state,int relicAttunement) noexcept {
    const int relic=std::clamp(relicAttunement,0,4);
    // Preserve passage access for legitimately progressed final-stage saves.
    if(state.finalAct>=1 && state.finalAct<=5) return 4;
    if(!state.openingWitnessed) return relic;
    const bool frontier=RoadHas(state,Realm::WidowedRain)||
                        RoadHas(state,Realm::HearthBeneath);
    if(!frontier) return std::max(relic,1);
    const bool middle=RoadHas(state,Realm::TidalLedger)||
                      RoadHas(state,Realm::SkyBelow)||
                      RoadHas(state,Realm::CinderSpine);
    if(!middle) return std::max(relic,2);
    const bool upper=RoadHas(state,Realm::HundredUnlived)||
                     RoadHas(state,Realm::OrchardOfKings);
    return std::max(relic,upper?4:3);
}
inline RoadGuidance EvaluateUnansweredRoad(const RoadEvidence& state)noexcept {
    RoadGuidance out;
    if(state.finalAct>=3 && state.finalAct<=5) {
        out.act=RoadAct::LivingAfterward;
        out.title="The World After Its Answer";
        out.question="A victory is not a disappearance; how will the surviving people live now?";
        out.objective="Return to familiar people and public records. The Auvren echo is optional.";
        out.bossAccessible=true;
        return out;
    }
    if(state.finalAct>0 && state.finalAct<=2) {
        out.act=RoadAct::ConfrontTheAnswer;
        out.title="Nhal-Vey, The Answer That Ate Its Question";
        out.question="Why did its first death leave a second enemy behind?";
        out.objective="Survive the unveiled form, then choose a new morning deliberately.";
        out.bossAccessible=true;
        return out;
    }
    if(!state.openingWitnessed){
        out.title="A Bell That Had No Last Note";
        out.question=UnansweredRoadBeats[0].localQuestion;
        out.objective="Listen to Bellwold or Paperhaven, investigate two landmarks, or take a stand in the Crossings.";
        out.alternatives[0]=Realm::ThreefoldReach;out.available=1;
        return out;
    }
    const bool frontier=RoadHas(state,Realm::WidowedRain)||RoadHas(state,Realm::HearthBeneath);
    if(!frontier){
        out.act=RoadAct::MissingWorld;out.title="What the World Withheld";
        out.question="Why does the bell's missing note answer from both rain and flame?";
        out.objective="Resolve one FIRST local chapter: Saltwake's lost sea OR Cinderhold's borrowed ember.";
        out.alternatives={{Realm::WidowedRain,Realm::HearthBeneath,Realm::Count}};
        out.available=2;return out;
    }
    const bool middle=RoadHas(state,Realm::TidalLedger)||RoadHas(state,Realm::SkyBelow)||
                      RoadHas(state,Realm::CinderSpine);
    if(!middle){
        out.act=RoadAct::CostOfAbsence;out.title="What the Absences Cost";
        out.question="Who pays whenever the world forgets something that was real?";
        out.objective="Resolve the FIRST chapter in Drevlach, Orravane or Vathless; their evidence speaks to the same missing burden.";
        // At least one is adjacent to every accessible completed frontier.
        if(RoadHas(state,Realm::WidowedRain)&&!RoadHas(state,Realm::HearthBeneath))
            out.alternatives={{Realm::TidalLedger,Realm::SkyBelow,Realm::Count}};
        else if(RoadHas(state,Realm::HearthBeneath)&&!RoadHas(state,Realm::WidowedRain))
            out.alternatives={{Realm::SkyBelow,Realm::CinderSpine,Realm::Count}};
        else out.alternatives={{Realm::TidalLedger,Realm::SkyBelow,Realm::CinderSpine}};
        out.available=(out.alternatives[2]==Realm::Count)?2:3;
        return out;
    }
    const bool upper=RoadHas(state,Realm::HundredUnlived)||RoadHas(state,Realm::OrchardOfKings);
    if(!upper){
        out.act=RoadAct::RightToExist;out.title="Who Deserves To Be Real";
        out.question="Are the erased inhabitants the cost of keeping every city standing?";
        out.objective="Resolve Eillun's or Tharniv's FIRST chapter; learn what a world may ask of its people.";
        if(RoadHas(state,Realm::CinderSpine)&&!RoadHas(state,Realm::SkyBelow)&&!RoadHas(state,Realm::TidalLedger))
            out.alternatives={{Realm::OrchardOfKings,Realm::Count,Realm::Count}};
        else if(RoadHas(state,Realm::TidalLedger)&&!RoadHas(state,Realm::SkyBelow)&&!RoadHas(state,Realm::CinderSpine))
            out.alternatives={{Realm::HundredUnlived,Realm::Count,Realm::Count}};
        else out.alternatives={{Realm::HundredUnlived,Realm::OrchardOfKings,Realm::Count}};
        out.available=out.alternatives[1]==Realm::Count?1:2;
        return out;
    }
    if(!RoadHas(state,Realm::FirstAbsence)){
        out.act=RoadAct::BeforePlace;out.title="The Witness Before Place";
        out.question="Who created an answer to protect a world from contradictory truths?";
        out.objective="Find Auvren via the linked upper realms and resolve its FIRST trial. Echo and guardian quests remain optional.";
        out.alternatives[0]=Realm::FirstAbsence;out.available=1;
        return out;
    }
    out.act=RoadAct::ConfrontTheAnswer;
    out.title="The Answer That Ate Its Question";
    out.question="Did Nhal-Vey save the world, or silence everybody who disagreed?";
    out.objective="Enter the northern Auvren arena and face the first form. Its defeat may not be the ending.";
    out.alternatives[0]=Realm::FirstAbsence;out.available=1;
    out.bossAccessible=true;return out;
}
// A genuine playable route, not a fabricated travel time or unlocked gate.
// Returns empty if required attunement has not been earned.
inline std::vector<Realm> RoadRoute(Realm current,Realm destination,
                                   int attunement) {
    return PlanRealmRoute(current,destination,attunement);
}
} // namespace UnmadeCore
