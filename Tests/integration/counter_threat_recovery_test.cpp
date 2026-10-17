#include "Save/UnmadeCounterThreatRules.h"
#include "Save/UnmadeSessionRules.h"
#include "../save/combined_record_store.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
int main(){int faults=0,reloads=0;
 for(int a=0;a<9;++a){Expeditions e;AdventureDepth depth;
  assert(e.Brief(a));assert(e.Visit(a,0));for(int r=0;r<5;++r){assert(e.Enter(a,r));if(r<4)assert(e.Kill(a,r));if(r>0&&r<4)assert(e.Inspect(a,r-1));}
  for(int k=4;k<8;++k){assert(e.Kill(a,k));}assert(e.Rescue(a,0));assert(e.Rescue(a,1));assert(e.Choose(a,1));assert(e.Enter(a,5));assert(e.Boss(a,true));
  assert(depth.Offer(a,true));assert(depth.Witness(a));assert(depth.Evidence(a,true));assert(depth.Choose(a,2,true));
  auto capture=[&]{SaveFixture::Record r;r.bHasLivingWorldSnapshot=true;r.bHasExpeditions=true;r.ExpeditionState=e.Pack();r.bHasAdventureDepth=true;r.AdventureDepthState=depth.Pack();assert(SaveFixture::Valid(r));return r;};
  const auto before=depth;const auto record=capture();
  assert(!PendingCounterThreat(depth,a,false));assert(PendingCounterThreat(depth,a,true));
  int blockedWrites=0;assert(CommitSessionCheckpoint(PendingCounterThreat(depth,a,true),[&]{++blockedWrites;return true;})==CheckpointSave::PendingDefeat);assert(blockedWrites==0);
  for(int fail=1;fail<=2;++fail){depth=before;SaveFixture::Storage disk;disk.primary=record;disk.backup=record;disk.failAt=fail;disk.damageOnFailure=true;
   assert(CommitCounterThreatReceipt(depth,a,true,[&]{return disk.Commit(capture());})==CounterThreatCommit::SaveFailed);
   assert(depth.Pack()==before.Pack()&&PendingCounterThreat(depth,a,true));++faults;
   for(int i=0;i<12;++i){auto* loaded=disk.Load();assert(loaded);AdventureDepth restored;assert(restored.Restore(loaded->AdventureDepthState,e,1));assert(restored.Pack()==before.Pack());++reloads;}
   disk.failAt=0;assert(CommitCounterThreatReceipt(depth,a,true,[&]{return disk.Commit(capture());})==CounterThreatCommit::Recorded);
   assert(depth.Arc(a)->favor==before.Arc(a)->favor+2&&!PendingCounterThreat(depth,a,true));
   const int writes=disk.writes;assert(CommitCounterThreatReceipt(depth,a,true,[&]{return disk.Commit(capture());})==CounterThreatCommit::AlreadyRecorded);assert(disk.writes==writes);
   for(int i=0;i<12;++i){AdventureDepth restored;assert(restored.Restore(disk.Load()->AdventureDepthState,e,1));assert(restored.Pack()==depth.Pack());++reloads;}
  }
  AdventureDepth protectedAccord;assert(protectedAccord.Offer(a,true));assert(protectedAccord.Witness(a));assert(protectedAccord.Evidence(a,true));assert(protectedAccord.Choose(a,1,true));
  assert(!PendingCounterThreat(protectedAccord,a,true));
  assert(CommitCounterThreatReceipt(protectedAccord,a,true,[]{assert(false);return true;})==CounterThreatCommit::Unavailable);
  int rejectedWrites=0;assert(CommitCounterThreatReceipt(depth,a,false,[&]{++rejectedWrites;return true;})==CounterThreatCommit::Unavailable);
  assert(CommitCounterThreatReceipt(depth,-1,true,[&]{++rejectedWrites;return true;})==CounterThreatCommit::Unavailable);assert(rejectedWrites==0);
 }
 std::cout<<"counter-threat: "<<faults<<" fault paths, "<<reloads<<" reloads, pending checkpoint/duplicate guards\n";
}
