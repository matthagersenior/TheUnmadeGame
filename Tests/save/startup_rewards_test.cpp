#include "Items/UnmadeStarterRules.h"
#include "combined_record_store.h"
#include <cassert>
using namespace UnmadeCore;
int main(){
 for(int recovery=0;recovery<2;++recovery)for(int failure=1;failure<=2;++failure){
  SaveFixture::Storage store;store.primary=SaveFixture::Record{};store.backup=SaveFixture::Record{};store.recovery=recovery;
  store.failAt=recovery?1:failure;store.damageOnFailure=true;
  InventoryModel inventory;int writes=0;
  assert(!CommitStarterKit(inventory,false,[&]{++writes;return true;}));
  assert(writes==0&&!inventory.HasClaimed(Achievement::Starter));
  const auto empty=inventory.Snapshot();
  assert(!CommitStarterKit(inventory,true,[&]{
   ++writes;auto next=*store.Load();next.bHasInventorySnapshot=true;
   const auto state=inventory.Snapshot();next.ItemQuantities.assign(state.quantities.begin(),state.quantities.end());
   next.EquippedItems.assign(state.equipped.begin(),state.equipped.end());next.AwardedMilestoneBits=state.milestones;
   return store.Commit(next);
  }));
  assert(store.Load()&&SaveFixture::Valid(*store.Load()));
  assert(!store.Load()->bHasInventorySnapshot);
  assert(inventory.Snapshot().quantities==empty.quantities);
  assert(inventory.Snapshot().equipped==empty.equipped);
  assert(inventory.Snapshot().milestones==empty.milestones);
  store.failAt=0;
  assert(CommitStarterKit(inventory,true,[&]{++writes;auto next=*store.Load();
   next.bHasInventorySnapshot=true;const auto state=inventory.Snapshot();
   next.ItemQuantities.assign(state.quantities.begin(),state.quantities.end());
   next.EquippedItems.assign(state.equipped.begin(),state.equipped.end());next.AwardedMilestoneBits=state.milestones;
   return store.Commit(next);
  }));
  assert(store.Load()->bHasInventorySnapshot);
  auto saved=inventory.Snapshot();
  saved.quantities[static_cast<int>(ItemId::HearthSalve)]=0; // spent kits must not refill on reload
  for(int reload=0;reload<100;++reload){
   InventoryModel restored;assert(restored.Restore(saved));
   assert(CommitStarterKit(restored,true,[&]{assert(false);return false;}));
   assert(restored.Snapshot().quantities==saved.quantities);
  }
 }
 InventoryModel legacy;auto chosen=legacy.Snapshot();
 chosen.quantities[static_cast<int>(ItemId::WayfarerBlade)]=1;
 chosen.quantities[static_cast<int>(ItemId::BellwoldBulwark)]=1;
 chosen.equipped[0]=static_cast<int>(ItemId::WayfarerBlade);
 chosen.equipped[1]=static_cast<int>(ItemId::BellwoldBulwark);
 assert(legacy.Restore(chosen));
 assert(CommitStarterKit(legacy,true,[]{return true;}));
 assert(legacy.Snapshot().equipped==chosen.equipped);
 InventoryModel full;auto capacity=full.Snapshot();
 // Use the catalog capacity rather than relying on a test-only inventory limit.
 capacity.quantities[static_cast<int>(ItemId::HearthSalve)]=FindItem(ItemId::HearthSalve)->maxStack;
 assert(full.Restore(capacity));int writes=0;
 assert(!CommitStarterKit(full,true,[&]{++writes;return true;}));
 assert(writes==0&&full.Snapshot().quantities==capacity.quantities);
 assert(full.Snapshot().milestones==capacity.milestones);
}
