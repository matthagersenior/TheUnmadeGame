#pragma once
#include "Combat/UnmadeBossRules.h"
#include "Combat/UnmadeFinalEncounterRules.h"
#include "Combat/UnmadeRealmGuardianRules.h"
#include "World/UnmadeCinderholdAdventureRules.h"
#include "World/UnmadeExpeditionRules.h"

namespace UnmadeCore {
// An absent, defeated or unrestored player cannot observe an armed warning.
// Cancel only transient attack state; durable outcomes and retry owners stay live.
template<class Cancel>
bool RequireEncounterPlayer(bool ready,Cancel cancel){
 if(ready)return true;
 cancel();return false;
}
inline void InterruptEncounter(BossEncounter& pattern){pattern.Reset();}
inline void InterruptEncounter(GuardianBeat& pattern){pattern.Reset();}
inline void InterruptEncounter(FinalBattleRhythm& pattern){pattern.Reset();}
inline void InterruptEncounter(ExpeditionPattern& pattern){pattern=ExpeditionPattern{};}
inline void InterruptEncounter(SlagElderPattern& pattern){pattern=SlagElderPattern{};}
inline void InterruptAttackWindup(double& windup,double& nextAllowed,double& recovery){
 windup=-1;nextAllowed=0;recovery=0;
}
// The rhythm already includes the act's base damage and any Names modifier.
// Keep health, swing receipts and ordinary guard/armor mitigation authoritative.
template<class Combat>
bool ConfigureAuthoredFinalStrike(Combat& combat,const FinalBeat& beat){
 if(beat.move!=FinalMove::Strike||!std::isfinite(beat.damage)||beat.damage<=0)return false;
 if(!combat.SetAttackDamage(beat.damage))return false;
 combat.SetGearBonuses(0,0);return true;
}
}
