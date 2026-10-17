#include "Combat/UnmadeStatusDamageRules.h"
#include "Combat/UnmadeCombatRewardRules.h"
#include "../save/combined_record_store.h"
#include <cassert>
#include <iostream>
#include <limits>
using namespace UnmadeCore;

static void Capture(const InventoryModel& inventory,SaveFixture::Record& record){
 const auto state=inventory.Snapshot();record.bHasInventorySnapshot=true;
 record.ItemQuantities.assign(state.quantities.begin(),state.quantities.end());
 record.EquippedItems.assign(state.equipped.begin(),state.equipped.end());
 record.AwardedMilestoneBits=state.milestones;
}
static InventoryModel Load(const SaveFixture::Record& record){
 InventorySnapshot state;
 for(int i=0;i<record.ItemQuantities.Num();++i)state.quantities[i]=record.ItemQuantities[i];
 for(int i=0;i<3;++i)state.equipped[i]=record.EquippedItems[i];
 state.milestones=record.AwardedMilestoneBits;InventoryModel inventory;
 assert(inventory.Restore(state));return inventory;
}
int main(){int faults=0,reloads=0;
 // The initial player strike survives; only the later source-owned Burn kills.
 // The existing first-style receipt must outlive the defeated actor's Tick.
 for(auto style:{EnemyStyle::Stalker,EnemyStyle::Watcher})for(int recovery=0;recovery<2;++recovery)
 for(int fail=1;fail<=(recovery?1:2);++fail){
  ++faults;Combatant enemy(6);PendingCombatRewards pending;int credits=0;
  assert(enemy.ReceiveHit(7,1,4,false)==HitOutcome::Applied&&enemy.IsAlive());
  auto outcome=ApplyStatusDamagePulse(enemy,100,1,4,true,[&]{++credits;pending.Queue(style);});
  assert(outcome.hit==HitOutcome::Applied&&outcome.injury==2&&outcome.defeated);
  assert(credits==1&&pending.Pending());
  for(int repeat=0;repeat<20;++repeat){
   auto duplicate=ApplyStatusDamagePulse(enemy,100,1,4,true,[&]{++credits;});
   assert(duplicate.hit==HitOutcome::AlreadyDefeated&&!duplicate.defeated&&duplicate.injury==0);
  }
  assert(credits==1);
  InventoryModel inventory;SaveFixture::Record record;Capture(inventory,record);
  SaveFixture::Storage storage;storage.primary=record;storage.backup=record;
  storage.recovery=recovery;storage.failAt=fail;storage.damageOnFailure=true;
  int claims=0;
  auto claim=[&](Achievement reward){++claims;const auto before=inventory.Snapshot();
   if(inventory.Claim(reward)!=RewardResult::Awarded)return false;
   auto next=*storage.Load();Capture(inventory,next);
   if(storage.Commit(next))return true;
   assert(inventory.Restore(before));return false;
  };
  auto has=[&](Achievement reward){return inventory.HasClaimed(reward);};
  assert(pending.Attempt(false,0,has,claim)==DefeatSaveResult::Waiting&&claims==0);
  assert(pending.Attempt(true,0,has,claim)==DefeatSaveResult::FirstFailure);
  assert(pending.Pending()&&claims==1&&!has(FirstCombatReward(style)));
  assert(storage.Load()&&!Load(*storage.Load()).HasClaimed(FirstCombatReward(style)));
  assert(CommitSessionCheckpoint(pending.Pending(),[]{assert(false);return true;})==CheckpointSave::PendingDefeat);
  for(int tick=1;tick<50;++tick)assert(pending.Attempt(true,tick/10.,has,claim)==DefeatSaveResult::Waiting);
  assert(claims==1);storage.failAt=0;
  assert(pending.Attempt(true,5,has,claim)==DefeatSaveResult::Saved&&!pending.Pending());
  const auto quantities=inventory.Snapshot().quantities;const int writes=storage.writes;
  for(int repeat=0;repeat<128;++repeat){++reloads;inventory=Load(*storage.Load());
   assert(has(FirstCombatReward(style))&&inventory.Snapshot().quantities==quantities);
   pending.Queue(style);assert(pending.Attempt(true,6+repeat,has,claim)==DefeatSaveResult::Saved);
   assert(storage.writes==writes);
  }
 }
 // Missing/non-player attribution still damages, without assigning player loot.
 Combatant unknown(4);int credits=0;
 auto unowned=ApplyStatusDamagePulse(unknown,11,1,4,false,[&]{++credits;});
 assert(unowned.defeated&&unowned.injury==4&&credits==0);
 Combatant guarded(5);guarded.SetGuarding(true);
 auto first=ApplyStatusDamagePulse(guarded,12,1,4,true,[&]{++credits;});
 assert(first.hit==HitOutcome::Applied&&first.injury==1&&!first.defeated&&credits==0);
 const double health=guarded.Health();
 assert(ApplyStatusDamagePulse(guarded,12,1,4,true,[&]{++credits;}).hit==HitOutcome::Duplicate);
 assert(guarded.Health()==health&&credits==0);
 for(double damage:{0.,-1.,std::numeric_limits<double>::infinity(),std::numeric_limits<double>::quiet_NaN()})
  assert(ApplyStatusDamagePulse(guarded,12,2,damage,true,[&]{++credits;}).hit==HitOutcome::InvalidDamage);
 assert(ApplyStatusDamagePulse(guarded,0,2,4,true,[&]{++credits;}).hit==HitOutcome::InvalidSource);
 assert(guarded.Health()==health&&credits==0);
 for(int pulse=2;pulse<=5;++pulse){
  auto outcome=ApplyStatusDamagePulse(guarded,12,pulse,4,true,[&]{++credits;});
  assert(outcome.hit==HitOutcome::Applied&&outcome.injury==1);
  assert(outcome.defeated==(pulse==5)&&credits==(pulse==5?1:0));
 }
 // Inventory capacity retains the receipt rather than inventing partial loot.
 InventoryModel full;auto fullState=full.Snapshot();
 for(const auto& item:ItemCatalog)fullState.quantities[static_cast<int>(item.id)]=item.maxStack;
 assert(full.Restore(fullState));PendingCombatRewards capacity;Combatant victim(4);
 assert(ApplyStatusDamagePulse(victim,14,1,4,true,[&]{capacity.Queue(EnemyStyle::Watcher);}).defeated);
 assert(capacity.Attempt(true,0,[&](auto r){return full.HasClaimed(r);},
  [&](auto r){return full.Claim(r)==RewardResult::Awarded;})==DefeatSaveResult::FirstFailure);
 assert(capacity.Pending()&&full.Snapshot().quantities==fullState.quantities&&full.Snapshot().milestones==fullState.milestones);
 // Whole-record rejection cannot silently settle a delayed reward or rotate backups.
 InventoryModel inventory;PendingCombatRewards pending;pending.Queue(EnemyStyle::Stalker);
 SaveFixture::Record record;Capture(inventory,record);SaveFixture::Storage storage;
 storage.primary=record;storage.backup=record;const auto before=inventory.Snapshot();
 auto malformed=[&](Achievement reward){assert(inventory.Claim(reward)==RewardResult::Awarded);
  auto invalid=record;Capture(inventory,invalid);invalid.bHasFieldSurveys=true;invalid.FieldSurveyStages.clear();
  assert(!storage.Commit(invalid));assert(inventory.Restore(before));return false;
 };
 assert(pending.Attempt(true,0,[&](auto r){return inventory.HasClaimed(r);},malformed)==DefeatSaveResult::FirstFailure);
 assert(pending.Pending()&&storage.writes==0&&storage.Load()&&!inventory.HasClaimed(Achievement::FirstStalker));
 std::cout<<"PASS: source-attributed status defeats, "<<faults<<" write faults, "<<reloads<<" reloads, duplicate/invalid pulses and rejected combined rewards\n";
}
