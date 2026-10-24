#pragma once
namespace UnmadeCore {
// Replace only the same stable person's history. Callers append new people;
// histories belonging to unloaded residents remain in the combined record.
template<class Rows,class Row> bool ReplaceNpcMemory(Rows& saved,const Row& next){
 for(auto& row:saved)if(row.NpcId==next.NpcId){row=next;return true;}
 return false;
}
}
namespace UnmadeCore {
// Retry the existing observations, not the event that originally produced them.
// Readiness and failed writes leave the dirty flag intact for the quiet timer.
template<class Writer> bool CommitPendingMemory(bool& pending,bool ready,Writer write){
 if(!pending)return true;
 if(!ready||!write())return false;
 pending=false;return true;
}
}
