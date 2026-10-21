#pragma once
// Preserve stable identity and evidence provenance when not all NPC actors are
// streamed/spawned at save time. Pure C++17; Unreal TArray/FName/FGuid and
// native fixture arrays expose the same minimal operations.
namespace UnmadeCore {
template<class Snapshots>
bool ValidNpcSnapshots(const Snapshots& snapshots) {
    if(snapshots.Num()>512)return false;
    for(int i=0;i<snapshots.Num();++i) {
        const auto& npc=snapshots[i];
        if(npc.NpcId.IsNone() || npc.Observations.Num()>64)return false;
        for(int j=0;j<i;++j)if(snapshots[j].NpcId==npc.NpcId)return false;
        for(int j=0;j<npc.Observations.Num();++j) {
            const auto& event=npc.Observations[j];
            if(!event.EventId.IsValid() || event.EventKind.IsNone())return false;
            const int kind=static_cast<int>(event.Evidence);
            if(kind!=0 && kind!=1)return false;
            if(kind==1 && event.SpeakerId.IsNone())return false;
            for(int k=0;k<j;++k)
                if(npc.Observations[k].EventId==event.EventId)return false;
        }
    }
    return true;
}
// Copy/validate before assignment. Existing off-screen records survive;
// freshly spawned residents replace only records for their *same* stable ID.
// Malformed updates cannot partially mutate an already saved array.
template<class Snapshots,class Snapshot>
bool MergeNpcSnapshot(Snapshots& snapshots,const Snapshot& incoming) {
    if(incoming.NpcId.IsNone()||!ValidNpcSnapshots(snapshots))return false;
    Snapshots next=snapshots;
    bool found=false;
    for(int i=0;i<next.Num();++i)
        if(next[i].NpcId==incoming.NpcId) {
            next[i]=incoming;found=true;break;
        }
    if(!found)next.Add(incoming);
    if(!ValidNpcSnapshots(next))return false;
    snapshots=next;
    return true;
}
} // namespace UnmadeCore
