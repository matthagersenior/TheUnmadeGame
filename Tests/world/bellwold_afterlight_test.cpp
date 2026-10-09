#include "World/UnmadeAfterlightRules.h"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace UnmadeCore;
int main() {
    BellwoldAfterlight q;
    assert(q.Stage()==0);
    assert(q.Begin(1,1,"npc.bellwold.matron.001")==AfterlightResult::Locked);
    assert(q.Begin(2,0,"npc.bellwold.matron.001")==AfterlightResult::Locked);
    assert(q.Begin(2,1,"npc.bellwold.guard.001")==AfterlightResult::NoChange);
    assert(q.Begin(2,1,"npc.bellwold.matron.001")==AfterlightResult::Started);
    assert(q.Stage()==1);
    assert(q.Begin(2,1,"npc.bellwold.matron.001")==AfterlightResult::NoChange);
    assert(q.Inspect(AfterlightChoice::Relief,"Bellwold.Afterlight.Census")==AfterlightResult::WrongEvidence);
    assert(q.Inspect(AfterlightChoice::Relief,"Bellwold.Afterlight.Relief")==AfterlightResult::EvidenceFound);
    assert(q.Stage()==2);
    assert(q.Converse("npc.bellwold.childtutor.001")==AfterlightResult::WrongWitness);
    assert(q.Converse("npc.bellwold.healer.001")==AfterlightResult::Witnessed);
    assert(q.Stage()==3);
    assert(q.Resolve(AfterlightChoice::Revelation)==AfterlightResult::WrongEvidence);
    assert(q.Resolve(AfterlightChoice::Relief)==AfterlightResult::Resolved);
    assert(q.Stage()==4 && q.Outcome()==AfterlightChoice::Relief);
    assert(q.Resolve(AfterlightChoice::Relief)==AfterlightResult::NoChange);
    assert(q.Converse("npc.bellwold.healer.001")==AfterlightResult::NoChange);
    assert(std::strcmp(q.Effect().worldTag,"Bellwold.Afterlight.SafeWard")==0);
    assert(q.Effect().shelterCapacity==2 && q.Effect().publishedWitnesses==0);
    BellwoldAfterlight loaded;
    assert(loaded.Restore(q.Snapshot()));
    assert(loaded.Outcome()==AfterlightChoice::Relief);
    auto corrupt=q.Snapshot();
    corrupt.stage=2; // outcome without resolution would duplicate the earned reward
    assert(!loaded.Restore(corrupt));
    assert(loaded.Outcome()==AfterlightChoice::Relief);
    BellwoldAfterlight other;
    assert(other.Begin(2,2,"npc.bellwold.matron.001")==AfterlightResult::Started);
    assert(other.Inspect(AfterlightChoice::Relief,"Bellwold.Afterlight.Relief")==AfterlightResult::EvidenceFound);
    // The player can change investigative approach before involving a witness.
    assert(other.Inspect(AfterlightChoice::Revelation,"Bellwold.Afterlight.Census")==AfterlightResult::EvidenceFound);
    assert(other.Converse("npc.bellwold.healer.001")==AfterlightResult::WrongWitness);
    assert(other.Converse("npc.bellwold.childtutor.001")==AfterlightResult::Witnessed);
    assert(other.Resolve(AfterlightChoice::Revelation)==AfterlightResult::Resolved);
    assert(std::strcmp(other.Effect().worldTag,"Bellwold.Afterlight.OpenCensus")==0);
    assert(other.Effect().publishedWitnesses==2 && other.Effect().shelterCapacity==0);
    assert(other.Restore(other.Snapshot()));
    corrupt=other.Snapshot();corrupt.approach=7;
    assert(!other.Restore(corrupt));
    BellwoldAfterlight empty;
    corrupt={2,0,0};
    assert(!empty.Restore(corrupt));
    assert(empty.Stage()==0);
    std::cout<<"PASS: Bellwold Afterlight requires a finished faction, real evidence and matching witness, branches and atomic saves\n";
}
