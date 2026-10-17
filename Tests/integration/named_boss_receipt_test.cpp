#include "Combat/UnmadeNamedBossRewardRules.h"
#include "Save/UnmadeDefeatSaveRetryRules.h"
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
int main(){int cases=0,reloads=0;
 assert(NamedBossReward(BossId::HollowBell)==Achievement::HollowBell);
 assert(NamedBossReward(BossId::RedactedCurator)==Achievement::RedactedCurator);
 assert(NamedBossReward(BossId::UnfinishedPilgrim)==Achievement::UnfinishedPilgrim);
 for(int boss=0;boss<3;++boss)for(int mercy=0;mercy<=(boss==0?1:0);++mercy)
 for(int recovery=0;recovery<2;++recovery)for(int fail=1;fail<=(recovery?1:2);++fail){
  ++cases;const auto id=static_cast<BossId>(boss);const auto reward=NamedBossReward(id);
  InventoryModel inventory;assert(inventory.Claim(Achievement::Starter)==RewardResult::Awarded);
  SaveFixture::Storage store;SaveFixture::Record record;Capture(inventory,record);store.primary=record;store.backup=record;store.recovery=recovery;store.failAt=fail;store.damageOnFailure=true;
  const auto before=inventory.Snapshot();DefeatSaveRetry retry;int claims=0;
  auto claim=[&]{return CommitNamedBossReward(id,[&](auto r){return inventory.HasClaimed(r);},[&](auto r){
   ++claims;const auto old=inventory.Snapshot();if(inventory.Claim(r)!=RewardResult::Awarded)return false;
   auto next=*store.Load();Capture(inventory,next);if(store.Commit(next))return true;assert(inventory.Restore(old));return false;
  });};
  auto pending=[&]{return NamedBossPending(id,!mercy,mercy,inventory.HasClaimed(reward));};
  assert(pending());int checkpoints=0;
  assert(CommitSessionCheckpoint(pending(),[&]{++checkpoints;return true;})==CheckpointSave::PendingDefeat);assert(checkpoints==0);
  assert(retry.Attempt(boss,0,claim)==DefeatSaveResult::FirstFailure);assert(pending());assert(inventory.Snapshot().quantities==before.quantities&&inventory.Snapshot().milestones==before.milestones);
  assert(store.Load()&&!Load(*store.Load()).HasClaimed(reward));
  for(int tick=1;tick<50;++tick)assert(retry.Attempt(boss,tick/10.,claim)==DefeatSaveResult::Waiting);
  assert(claims==1);store.failAt=0;assert(retry.Attempt(boss,5,claim)==DefeatSaveResult::Saved);assert(!pending());
  const auto saved=inventory.Snapshot();const int writes=store.writes;
  assert(CommitSessionCheckpoint(pending(),[&]{++checkpoints;return true;})==CheckpointSave::Saved);assert(checkpoints==1);
  for(int repeat=0;repeat<25;++repeat){
   ++reloads;auto back=Load(*store.Load());assert(back.HasClaimed(reward));assert(back.Snapshot().quantities==saved.quantities);
   assert(CommitNamedBossReward(id,[&](auto r){return back.HasClaimed(r);},[](auto){assert(false);return false;}));
   assert(retry.Attempt(boss,10+repeat,claim)==DefeatSaveResult::Waiting);assert(store.writes==writes);
  }
 }
 for(int boss=0;boss<3;++boss){
  InventoryModel full;auto s=full.Snapshot();for(const auto& item:ItemCatalog)s.quantities[static_cast<int>(item.id)]=item.maxStack;assert(full.Restore(s));
  const auto id=static_cast<BossId>(boss);const auto before=full.Snapshot();
  assert(!CommitNamedBossReward(id,[&](auto r){return full.HasClaimed(r);},[&](auto r){return full.Claim(r)==RewardResult::Awarded;}));
  assert(full.Snapshot().quantities==before.quantities&&full.Snapshot().milestones==before.milestones);
  assert(NamedBossPending(id,true,false,false));assert(!NamedBossPending(id,false,false,false));assert(!NamedBossPending(id,true,false,true));
 }
 assert(NamedBossReward(BossId::None)==Achievement::Count&&!NamedBossPending(BossId::None,true,true,false));
 assert(!CommitNamedBossReward(BossId::Count,[](auto){assert(false);return false;},[](auto){assert(false);return false;}));
 std::cout<<"PASS: "<<cases<<" named victory/mercy receipt faults; "<<reloads<<" reloads; checkpoint blocks, five-second retries, inventory capacity and settled suppression\n";
}
