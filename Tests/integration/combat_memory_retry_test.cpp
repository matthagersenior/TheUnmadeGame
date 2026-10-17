#include "Combat/UnmadeCombatRewardRules.h"
#include "NPC/UnmadeMemorySaveRules.h"
#include "UI/UnmadePlayerFacingRules.h"
#include "../save/combined_record_store.h"
#include <cassert>
#include <iostream>
using namespace UnmadeCore;
static void Capture(const InventoryModel& inventory,SaveFixture::Record& r){
 const auto s=inventory.Snapshot();r.bHasInventorySnapshot=true;
 r.ItemQuantities.assign(s.quantities.begin(),s.quantities.end());r.EquippedItems.assign(s.equipped.begin(),s.equipped.end());r.AwardedMilestoneBits=s.milestones;
}
static InventoryModel Load(const SaveFixture::Record& r){
 InventorySnapshot s;for(int i=0;i<r.ItemQuantities.Num();++i)s.quantities[i]=r.ItemQuantities[i];for(int i=0;i<3;++i)s.equipped[i]=r.EquippedItems[i];s.milestones=r.AwardedMilestoneBits;InventoryModel result;assert(result.Restore(s));return result;
}
int main(){
 assert(FirstCombatReward(EnemyStyle::Stalker)==Achievement::FirstStalker);
 assert(FirstCombatReward(EnemyStyle::Watcher)==Achievement::FirstWatcher);
 for(int fault=1;fault<=2;++fault)for(auto style:{EnemyStyle::Stalker,EnemyStyle::Watcher}){
  InventoryModel inv;SaveFixture::Storage store;SaveFixture::Record record;Capture(inv,record);
  assert(store.Commit(record));assert(store.Commit(record));
  PendingCombatRewards queue;assert(!queue.Queue(static_cast<EnemyStyle>(-1)));assert(!queue.Queue(static_cast<EnemyStyle>(99)));
  assert(queue.Queue(style));int claims=0;
  auto has=[&](auto r){return inv.HasClaimed(r);};
  auto claim=[&](auto r){++claims;const auto before=inv.Snapshot();assert(inv.Claim(r)==RewardResult::Awarded);Capture(inv,record);if(store.Commit(record))return true;assert(inv.Restore(before));return false;};
  assert(queue.Attempt(false,0,has,claim)==DefeatSaveResult::Waiting);assert(claims==0);
  store.failAt=store.writes+fault;store.damageOnFailure=true;
  assert(queue.Attempt(true,0,has,claim)==DefeatSaveResult::FirstFailure);assert(queue.Pending());
  assert(CommitSessionCheckpoint(queue.Pending(),[]{assert(false);return true;})==CheckpointSave::PendingDefeat);
  for(int i=1;i<50;++i){queue.Queue(style);assert(queue.Attempt(true,i/10.,has,claim)==DefeatSaveResult::Waiting);}assert(claims==1);
  assert(store.Load());assert(!Load(*store.Load()).HasClaimed(FirstCombatReward(style)));
  assert(queue.Attempt(true,5,has,claim)==DefeatSaveResult::Saved);assert(!queue.Pending());
  const int writes=store.writes;
  for(int i=0;i<30;++i){inv=Load(*store.Load());queue.Queue(style);assert(queue.Attempt(true,6+i,has,claim)==DefeatSaveResult::Saved);assert(store.writes==writes);}
 }
 // Independent rewards retain successful receipts while the remaining one waits.
 PendingCombatRewards both;both.Queue(EnemyStyle::Stalker);both.Queue(EnemyStyle::Watcher);int stalker=0,watcher=0;
 auto none=[](auto){return false;};
 assert(both.Attempt(true,0,none,[&](auto r){if(r==Achievement::FirstStalker){++stalker;return true;}++watcher;return false;})==DefeatSaveResult::FirstFailure);
 assert(both.Pending()&&stalker==1&&watcher==1);
 assert(both.Attempt(true,5,none,[&](auto r){assert(r==Achievement::FirstWatcher);++watcher;return true;})==DefeatSaveResult::Saved);assert(stalker==1&&watcher==2);
 InventoryModel full;auto snapshot=full.Snapshot();for(const auto& item:ItemCatalog)snapshot.quantities[static_cast<int>(item.id)]=item.maxStack;assert(full.Restore(snapshot));
 PendingCombatRewards capacity;capacity.Queue(EnemyStyle::Watcher);assert(capacity.Attempt(true,0,[&](auto r){return full.HasClaimed(r);},[&](auto r){return full.Claim(r)==RewardResult::Awarded;})==DefeatSaveResult::FirstFailure);assert(capacity.Pending()&&full.Snapshot().milestones==snapshot.milestones);
 // Direct witness failure remains dirty during quiet periods, without replaying events.
 bool dirty=true;int writes=0;
 assert(!CommitPendingMemory(dirty,false,[&]{++writes;return true;}));assert(dirty&&writes==0);
 assert(!CommitPendingMemory(dirty,true,[&]{++writes;return false;}));assert(dirty&&writes==1);
 assert(CommitPendingMemory(dirty,true,[&]{++writes;return true;}));assert(!dirty&&writes==2);
 assert(CommitPendingMemory(dirty,true,[&]{assert(false);return false;}));
 // A malformed later subrecord cannot rotate the good backup on a memory retry.
 SaveFixture::Storage store;SaveFixture::Record good;assert(store.Commit(good));assert(store.Commit(good));auto bad=good;SaveFixture::Npc npc;npc.NpcId="npc.witness";npc.Observations.push_back({{1},"Player.Helped","","",2});bad.NpcSnapshots.push_back(npc);const int before=store.writes;dirty=true;
 assert(!CommitPendingMemory(dirty,true,[&]{return store.Commit(bad);}));assert(dirty&&store.writes==before&&store.Load());
 std::cout<<"PASS: combat reward write faults, recovery, checkpoint guards, duplicate kills, reloads, partial rewards, capacity and quiet memory retry\n";
}
