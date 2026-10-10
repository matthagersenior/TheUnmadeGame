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
inline constexpr std::array<RoadBeat,9> UnansweredRoadBeats={{
    {Realm::ThreefoldReach,"A Bell That Had No Last Note",
     "Why does the Crossings bell ring from a road that no one has walked?",
     "In Bellwold a refugee remembers a bell that has never been cast. In Paperhaven its absence has a signed record. Neither settlement can prove the other wrong.",
     "Listen to a rain-counter on the lost coast or a hearth-reader beneath the mountain.",
     "Campaign.Thread.0"},
    {Realm::WidowedRain,"The Shore Without Its Water",
     "Who can remember a sea that is missing without owning the grief of those left ashore?",
     "Sella's sea chart is not an error. The rain preserved a record of something intentionally withheld, and the debt dates are written in another city's script.",
     "Follow the ferrymen's unpaid dates into Drevlach, or ask the inverted choir who still bears the missing weight.",
     "Campaign.Thread.1"},
    {Realm::HearthBeneath,"The Heat Lent By No Sun",
     "Who keeps a common flame burning when the sun whose gift it was never existed?",
     "Tarin reads a fire with the same missing second found in the bell. The city's warmth survives because someone accepted a cost they did not create.",
     "Seek Orravane's weight-bearing score or Vathless's fossil history to find who pays that cost.",
     "Campaign.Thread.2"},
    {Realm::TidalLedger,"The Account That Borrowed Tomorrow",
     "Can a debt be legitimate when its signatory has not been born?",
     "Veynu's impossible dates show that the world's missing things were pledged as collateral; a contract is written on the underside of reality itself.",
     "A census of people who never arrived may reveal who had the right to authorize those bargains.",
     "Campaign.Thread.3"},
    {Realm::SkyBelow,"The Chorus That Holds The Roof",
     "How many people were removed from history to keep a single hanging street above the ground?",
     "Yllesh's missing voice proves that the same absences are doing work in several realms. The common pattern is a decision, not an accident.",
     "Compare the unentered citizens of Eillun or the untitled court that authorized no crown.",
     "Campaign.Thread.4"},
    {Realm::CinderSpine,"The Stone That Remembered Workers",
     "When a family is mined out of time, who gets to call its descendants fictional?",
     "Ghraet's seams show erased lineages in the mountain's living load. Someone has been converting impossible people into stable places.",
     "Travel toward the court without kings; ask who signed a law that everyone now obeys.",
     "Campaign.Thread.5"},
    {Realm::HundredUnlived,"The Census Of Those Not Born",
     "Can a person be a valid witness without having a beginning recognized by the city?",
     "Eillun's unentered people are not echoes; they live and disagree. A missing origin is not evidence that someone deserves to be erased.",
     "The contradictory threshold in Auvren may be a door rather than the end of the world.",
     "Campaign.Thread.6"},
    {Realm::OrchardOfKings,"The Crown Nobody Accepted",
     "Who has been enforcing the world's oldest rule after every prospective ruler refused it?",
     "Tharniv's root-court remembers a law signed by nobody. Its force survived because an answering consciousness was made responsible for every unresolved question.",
     "Return to the place before geography and ask whether the answer is actually keeping the world safe.",
     "Campaign.Thread.7"},
    {Realm::FirstAbsence,"The Answer With No Question",
     "Why does the first witness remember a traveler who has not yet arrived?",
     "Auvren reveals a seam older than either origin. Nhal-Vey did not create the contradictions; it has been eating the questions people were afraid to ask.",
     "Face Nhal-Vey on the northern terrace, where it insists that only one history may survive.",
     "Campaign.Thread.8"}
}};
struct RoadEvidence {
    bool openingWitnessed=false;  // starter civic dispute, two villages, or two landmarks
    std::uint16_t firstStories=0;  // 9-bit mask of REAL completed first chapters
    int finalAct=0; // from FinalJourney only
};
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
