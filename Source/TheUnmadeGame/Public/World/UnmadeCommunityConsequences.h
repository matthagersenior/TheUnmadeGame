#pragma once
// Authored, replay-safe consequences derived from EXISTING saved player choices.
// No new hidden counters, random events, AI, timed grinds, or divergent save slots.
#include "World/UnmadeFactionChronicleRules.h"
#include "World/UnmadeFrontierRealmRules.h"
#include <array>
namespace UnmadeCore {
enum class Community : int { Crossings, Bellwold, Paperhaven, Saltwake, Cinderhold, Count };
enum class CommunityState : int { Uncertain, Rebuilding, Shared, Revealed };
struct ConsequenceFacts {
    int crossingDecision=0; // 0 unresolved, 1 shelter, 2 archive
    RegionalTaskSnapshot local{};
    std::array<int,3> factionEndings{};
    std::array<int,2> frontierEndings{};
};
struct CommunityConsequence {
    Community community=Community::Crossings;
    CommunityState state=CommunityState::Uncertain;
    int tradeTrust=0; // applies existing finite, clamped merchant discount
    bool sharedAccess=false;
    bool publicEvidence=false;
    const char* visibleChange="The settlement still waits for a choice.";
};
inline constexpr std::array<const char*,5> CommunityNames = {{
    "The Crossings","Bellwold Refuge","Paperhaven Archive",
    "Saltwake, the Rainward Port","Cinderhold, the Borrowed Hearth"
}};
inline constexpr std::array<std::array<const char*,4>,5> CommunityDescriptions = {{
    {{"Three roads disagree over who may travel.",
      "The market chooses whether to shelter travelers or study the fracture.",
      "Travelers maintain a common path that no authority may close.",
      "Witnesses maintain public boundary records and a safe alternative path."}},
    {{"Lanterns illuminate homes that the census forgot.",
      "Refuge workers redistribute light while the matron records the missing.",
      "Bellwold's open hearth welcomes anyone who needs shelter.",
      "Bellwold publishes the keeper's history without closing its refuge."}},
    {{"The register has blank spaces where people still live.",
      "Copyists restore testimony while the vault remains guarded.",
      "The restored archive admits witnesses without demanding an official name.",
      "Redacted testimonies are visible to everyone, including the authorities."}},
    {{"The rain never stops, and freshwater is priced by the cup.",
      "Navigator and rainkeeper compare water ledgers for missing entries.",
      "Saltwake has opened a communal cistern to stranded travelers.",
      "Saltwake's water accounts are public; profiteers can be challenged."}},
    {{"Every household borrows warmth from an unowned ember.",
      "The keepers count embers to learn who has been denied heat.",
      "Cinderhold maintains a common flame for every household.",
      "Records of who withheld warmth are preserved in the open."}}
}};
inline bool ValidFacts(const ConsequenceFacts& facts) noexcept {
    if(facts.crossingDecision<0 || facts.crossingDecision>2 ||
       facts.local.bellwold<0 || facts.local.bellwold>2 ||
       facts.local.paperhaven<0 || facts.local.paperhaven>2)return false;
    for(const int e:facts.factionEndings)if(e<0 || e>2)return false;
    for(const int e:facts.frontierEndings)if(e<0 || e>2)return false;
    return true;
}
inline CommunityConsequence EvaluateCommunity(Community which,
                                                const ConsequenceFacts& facts) noexcept {
    CommunityConsequence out{};
    const int i=static_cast<int>(which);
    if(i<0 || i>=5 || !ValidFacts(facts))return out;
    out.community=which;
    int localProgress=0;
    int finalDecision=0;
    if(which==Community::Crossings) {
        localProgress=facts.crossingDecision>0?1:0;
        finalDecision=facts.factionEndings[2];
    } else if(which==Community::Bellwold) {
        localProgress=facts.local.bellwold;
        finalDecision=facts.factionEndings[0];
    } else if(which==Community::Paperhaven) {
        localProgress=facts.local.paperhaven;
        finalDecision=facts.factionEndings[1];
    } else if(which==Community::Saltwake) {
        finalDecision=facts.frontierEndings[0];
    } else {
        finalDecision=facts.frontierEndings[1];
    }
    out.state=finalDecision==1?CommunityState::Shared
        :finalDecision==2?CommunityState::Revealed
        :localProgress>0?CommunityState::Rebuilding:CommunityState::Uncertain;
    out.tradeTrust=out.state==CommunityState::Shared?40
        :out.state==CommunityState::Revealed?25
        :out.state==CommunityState::Rebuilding?10:0;
    out.sharedAccess=out.state==CommunityState::Shared;
    out.publicEvidence=out.state==CommunityState::Revealed;
    out.visibleChange=CommunityDescriptions[i][static_cast<int>(out.state)];
    return out;
}
inline std::array<CommunityConsequence,5> EvaluateAllCommunities(
    const ConsequenceFacts& facts) noexcept {
    std::array<CommunityConsequence,5> all{};
    for(int i=0;i<5;++i)
        all[i]=EvaluateCommunity(static_cast<Community>(i),facts);
    return all;
}
} // namespace UnmadeCore
