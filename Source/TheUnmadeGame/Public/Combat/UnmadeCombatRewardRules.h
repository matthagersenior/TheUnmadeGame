#pragma once
#include "Combat/UnmadeCombatRules.h"
#include "Items/UnmadeItemRules.h"
#include "Save/UnmadeDefeatSaveRetryRules.h"
namespace UnmadeCore {
inline Achievement FirstCombatReward(EnemyStyle style){
 switch(style){case EnemyStyle::Stalker:return Achievement::FirstStalker;
 case EnemyStyle::Watcher:return Achievement::FirstWatcher;default:return Achievement::Count;}
}
// Session-only queue: defeated actors may stop, while the player owns retries.
// Existing milestone bits remain the durable receipts; no per-kill loot is added.
class PendingCombatRewards final {
public:
 bool Pending()const{return pending_!=0;}
 bool Queue(EnemyStyle style){
  const auto reward=FirstCombatReward(style);if(reward==Achievement::Count)return false;
  if(!pending_)retry_.Reset();
  pending_|=1u<<static_cast<int>(style);return true;
 }
 template<class Has,class Claim> DefeatSaveResult Attempt(bool ready,double now,Has has,Claim claim){
  if(!ready||!Pending())return DefeatSaveResult::Waiting;
  return retry_.Attempt(0,now,[&]{
   for(int i=0;i<2;++i)if(pending_&(1u<<i)){
    const auto reward=FirstCombatReward(static_cast<EnemyStyle>(i));
    if(has(reward)||claim(reward))pending_&=~(1u<<i);
   }
   return !Pending();
  });
 }
private:
 unsigned pending_=0;DefeatSaveRetry retry_;
};
}
