#pragma once
#include "Combat/UnmadeCombatRules.h"

namespace UnmadeCore {
struct StatusDamageOutcome {
 HitOutcome hit=HitOutcome::InvalidDamage;
 double injury=0;
 bool defeated=false;
};
// A status retains its original source at the native boundary. Only an actual
// lethal pulse from a known player source earns the existing defeat receipt;
// periodic ticks and duplicate/invalid pulses never invent more kill rewards.
template<class Defeat>
StatusDamageOutcome ApplyStatusDamagePulse(Combatant& target,std::uint64_t source,
 std::uint64_t pulse,double damage,bool playerOwned,Defeat defeat){
 const double before=target.Health();
 const auto hit=target.ReceiveHit(source,pulse,damage,false);
 const bool applied=hit==HitOutcome::Applied;
 const bool defeated=applied&&before>0&&!target.IsAlive();
 if(defeated&&playerOwned)defeat();
 return {hit,applied?before-target.Health():0.,defeated};
}
}
