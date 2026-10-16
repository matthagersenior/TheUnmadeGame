#pragma once
namespace UnmadeCore {
// Replace only the same stable person's history. Callers append new people;
// histories belonging to unloaded residents remain in the combined record.
template<class Rows,class Row> bool ReplaceNpcMemory(Rows& saved,const Row& next){
 for(auto& row:saved)if(row.NpcId==next.NpcId){row=next;return true;}
 return false;
}
}
