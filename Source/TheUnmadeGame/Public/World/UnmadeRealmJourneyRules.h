#pragma once
#include "World/UnmadePlayerExperienceRules.h"
namespace UnmadeCore {
enum class JourneyStep { Board, Frontier, Chamber, Resolution, Boss, Return, Reward, PersonalOffer, Witness, PersonalEvidence, Accord, WingOffer, WingFieldwork, WingGuardian, WingReward, Home };
struct JourneyObjective {JourneyStep step;int index=-1;};
inline JourneyObjective NextRealmObjective(const ExpeditionState& e,const DepthArc& d,const ExperienceArc& x){
 if(!e.brief)return {JourneyStep::Board};
 if(!(e.visited&1))return {JourneyStep::Frontier};
 for(int r=0;r<5;++r)if(!(e.rooms&(1<<r)))return {JourneyStep::Chamber,r};
 if(!e.branch)return {JourneyStep::Resolution};
 if(!(e.rooms&32))return {JourneyStep::Chamber,5};
 if(!e.boss)return {JourneyStep::Boss};
 if(!e.returned)return {JourneyStep::Return};
 if(!e.reward)return {JourneyStep::Reward};
 if(!d.offered)return {JourneyStep::PersonalOffer};
 if(!d.witness)return {JourneyStep::Witness};
 if(!d.evidence)return {JourneyStep::PersonalEvidence};
 if(!d.choice)return {JourneyStep::Accord};
 if(!x.offered)return {JourneyStep::WingOffer};
 for(int r=0;r<3;++r)if(!(x.steps&(1<<r)))return {JourneyStep::WingFieldwork,r};
 if(!x.guardian)return {JourneyStep::WingGuardian};
 if(!x.complete)return {JourneyStep::WingReward};
 return {JourneyStep::Home};
}
inline int ExpeditionEnemyRoom(int slot){return slot>=0&&slot<9?(slot<4?slot:slot<8?4:5):-1;}
inline bool ExpeditionEncounterActive(const ExpeditionState& e,int slot){
 const int room=ExpeditionEnemyRoom(slot);
 return room>=0&&e.brief&&(e.rooms&(1<<room))&&!e.boss&&(slot==8?e.branch!=0:!(e.kills&(1<<slot)));
}
inline bool InsideRegisteredChamber(int a,int room,double x,double y,double z){
 if(a<0||a>=9||room<0||room>=6||!std::isfinite(x)||!std::isfinite(y)||!std::isfinite(z))return false;
 const auto c=DungeonRoom(a,room);
 return std::abs(x-c.x)<1150&&std::abs(y-c.y)<1150&&std::abs(z)<180;
}
// Transient confirmation never becomes an invented saved choice.
class ExpeditionConfirmation {
 int arc=-1,kind=-1,value=-1;double until=-1,last=-1;
public:
 bool Confirm(int a,int k,int v,double now){
  if(a<0||a>=9||k<1||k>2||v<1||v>2||!std::isfinite(now)||now<0){arc=-1;return false;}
  bool same=a==arc&&k==kind&&v==value&&now>=last&&now<=until;
  last=now;if(same){arc=-1;until=-1;return true;}
  arc=a;kind=k;value=v;until=now+20;return false;
 }
};
}
