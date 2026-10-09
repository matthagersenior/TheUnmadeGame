#include "World/UnmadeCommunityConsequences.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main() {
    ConsequenceFacts input;
    const auto first=EvaluateAllCommunities(input);
    for(int i=0;i<5;++i) {
        assert(first[i].state==CommunityState::Uncertain);
        assert(first[i].tradeTrust==0 && !first[i].sharedAccess &&
               !first[i].publicEvidence);
        assert(std::strlen(first[i].visibleChange)>20);
    }
    input.local.bellwold=1;
    input.local.paperhaven=2;
    input.crossingDecision=2;
    const auto healing=EvaluateAllCommunities(input);
    for(int i=0;i<3;++i)
        assert(healing[i].state==CommunityState::Rebuilding);
    assert(healing[3].state==CommunityState::Uncertain);
    input.factionEndings={2,1,1};
    input.frontierEndings={1,2};
    const auto after=EvaluateAllCommunities(input);
    assert(after[0].state==CommunityState::Shared);
    assert(after[1].state==CommunityState::Revealed);
    assert(after[2].state==CommunityState::Shared);
    assert(after[3].state==CommunityState::Shared);
    assert(after[4].state==CommunityState::Revealed);
    assert(after[1].publicEvidence && !after[1].sharedAccess);
    assert(after[0].sharedAccess && !after[0].publicEvidence);
    assert(after[0].tradeTrust>healing[0].tradeTrust);
    assert(after[1].tradeTrust>healing[1].tradeTrust);
    assert(after[0].tradeTrust!=after[1].tradeTrust);
    assert(std::strcmp(after[0].visibleChange,after[1].visibleChange)!=0);
    // Two distinct moral choices both create value; neither erases existing help.
    auto changed=input;
    changed.factionEndings[0]=1;
    assert(EvaluateCommunity(Community::Bellwold,changed).state==CommunityState::Shared);
    assert(EvaluateCommunity(Community::Paperhaven,changed).state==
           EvaluateCommunity(Community::Paperhaven,input).state);
    // No new point system to farm: unchanged facts imply unchanged world.
    const auto repeat=EvaluateAllCommunities(input);
    for(int i=0;i<5;++i) {
        assert(repeat[i].state==after[i].state);
        assert(repeat[i].tradeTrust==after[i].tradeTrust);
        assert(std::strcmp(repeat[i].visibleChange,after[i].visibleChange)==0);
    }
    changed.factionEndings[2]=8;
    assert(!ValidFacts(changed));
    assert(EvaluateCommunity(Community::Crossings,changed).state==CommunityState::Uncertain);
    assert(EvaluateCommunity(Community::Count,input).state==CommunityState::Uncertain);
    std::cout<<"PASS: five communities react to decisions, local progress, real trade trust, branches and invalid state\n";
}
