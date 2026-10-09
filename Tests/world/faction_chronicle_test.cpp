#include "World/UnmadeFactionChronicleRules.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main(){
    FactionChronicle story;
    assert(story.Converse("npc.bellwold.matron.001",true)==FactionResult::NoChange);
    assert(story.Converse("npc.bellwold.lamplighter.001",false)==FactionResult::Advanced);
    assert(story.Converse("npc.bellwold.lamplighter.001",false)==FactionResult::NoChange);
    assert(story.Converse("npc.bellwold.matron.001",false)==FactionResult::Advanced);
    assert(story.Converse("npc.bellwold.guard.001",false)==FactionResult::NeedsEvidence);
    assert(story.Stage(Faction::Refuge)==2);
    assert(story.Converse("npc.bellwold.guard.001",true)==FactionResult::ChoiceRequired);
    FactionChronicle pendingSave;
    assert(pendingSave.Restore(story.Snapshot()));
    assert(pendingSave.Stage(Faction::Refuge)==3 &&
           pendingSave.Ending(Faction::Refuge)==FactionEnding::Unresolved);
    assert(story.Decide(Faction::Refuge,FactionEnding::Solidarity)==FactionResult::Resolved);
    assert(story.Decide(Faction::Refuge,FactionEnding::Truth)==FactionResult::NoChange);
    assert(story.Reputation(Faction::Refuge)==40);
    assert(story.Converse("npc.paperhaven.scribe.001",true)==FactionResult::Advanced);
    assert(story.Converse("npc.paperhaven.registrar.001",true)==FactionResult::Advanced);
    assert(story.Converse("npc.paperhaven.archivist.001",true)==FactionResult::ChoiceRequired);
    assert(story.Decide(Faction::Archive,FactionEnding::Truth)==FactionResult::Resolved);
    assert(story.Reputation(Faction::Archive)==25);
    assert(!story.AllResolved());
    assert(story.Converse("npc.guard.001",true)==FactionResult::Advanced);
    assert(story.Converse("npc.roadwarden.001",true)==FactionResult::Advanced);
    assert(story.Converse("npc.welllistener.001",true)==FactionResult::ChoiceRequired);
    assert(story.Decide(Faction::Roadbound,FactionEnding::Solidarity)==FactionResult::Resolved);
    assert(story.AllResolved());
    auto before=story.Snapshot();
    FactionChronicle restored;assert(restored.Restore(before));
    assert(restored.AllResolved());
    auto invalid=before;invalid.stages[1]=10;
    assert(!restored.Restore(invalid) && restored.AllResolved());
    // An unresolved final choice is a valid saved stage-3 state.
    invalid=before;invalid.endings[0]=0;
    assert(restored.Restore(invalid));
    invalid=before;invalid.stages[0]=2;
    assert(!restored.Restore(invalid));
    invalid=before;invalid.endings[1]=5;
    assert(!restored.Restore(invalid));
    assert(restored.Converse(nullptr,true)==FactionResult::NoChange);
    assert(restored.Decide(Faction::Count,FactionEnding::Truth)==FactionResult::NoChange);
    std::cout<<"PASS: three faction chronicle arcs, evidence gating, branches, reputation, snapshots\n";
}
