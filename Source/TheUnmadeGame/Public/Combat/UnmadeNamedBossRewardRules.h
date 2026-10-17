#pragma once
#include "Combat/UnmadeBossRules.h"
#include "Items/UnmadeItemRules.h"
namespace UnmadeCore {
inline Achievement NamedBossReward(BossId boss){
 switch(boss){
 case BossId::HollowBell:return Achievement::HollowBell;
 case BossId::RedactedCurator:return Achievement::RedactedCurator;
 case BossId::UnfinishedPilgrim:return Achievement::UnfinishedPilgrim;
 default:return Achievement::Count;
 }
}
inline bool NamedBossPending(BossId boss,bool defeated,bool mercy,bool claimed){
 return NamedBossReward(boss)!=Achievement::Count&&(defeated||mercy)&&!claimed;
}
template<class Has,class Claim> bool CommitNamedBossReward(BossId boss,Has has,Claim claim){
 const auto reward=NamedBossReward(boss);
 return reward!=Achievement::Count&&(has(reward)||claim(reward));
}
}
