#pragma once
#include "World/UnmadeAdventureDepthRules.h"
namespace UnmadeCore {
inline bool PendingCounterThreat(const AdventureDepth& depth,int realm,bool defeated){
 const auto* arc=depth.Arc(realm);return defeated&&arc&&arc->choice==2&&!arc->counterThreat;
}
enum class CounterThreatCommit { Unavailable, AlreadyRecorded, Recorded, SaveFailed };
template<class Writer>
CounterThreatCommit CommitCounterThreatReceipt(AdventureDepth& depth,int realm,bool defeated,Writer write){
 const auto* arc=depth.Arc(realm);
 if(!defeated||!arc||arc->choice!=2)return CounterThreatCommit::Unavailable;
 if(arc->counterThreat)return CounterThreatCommit::AlreadyRecorded;
 const auto before=depth;
 if(!depth.CounterThreat(realm))return CounterThreatCommit::Unavailable;
 if(!write()){depth=before;return CounterThreatCommit::SaveFailed;}
 return CounterThreatCommit::Recorded;
}
}
