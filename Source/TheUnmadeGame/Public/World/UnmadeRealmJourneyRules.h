#pragma once
#include "World/UnmadePlayerExperienceRules.h"
#include "World/UnmadeExpeditionRegistry.h"
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
// Derived wayfinding always points to something the player has reason to know.
// For chambers not yet entered, mark the last documented room instead of
// leaking a future chamber's hidden layout, NPC location or solution.
enum class JourneyFocus { Main,Personal,Optional,FreeRoam };
struct JourneyWaypoint {
 bool available=false,precise=false,optional=false;
 JourneyFocus focus=JourneyFocus::FreeRoam;
 JourneyStep step=JourneyStep::Home;
 int index=-1;
 double x=0,y=0;
 const char* cue="No mandatory task remains; choose the next main-story road.";
};
inline JourneyWaypoint NextRealmWaypoint(int a,const ExpeditionState& e,
 const DepthArc& d,const ExperienceArc& x){
 if(a<0||a>=9)return {};
 const auto goal=NextRealmObjective(e,d,x);
 const auto& city=ExpeditionSpecs[a];
 JourneyWaypoint w;w.step=goal.step;w.index=goal.index;
 w.focus=goal.step>=JourneyStep::WingOffer?
   (goal.step==JourneyStep::Home?JourneyFocus::FreeRoam:JourneyFocus::Optional):
   goal.step>=JourneyStep::PersonalOffer?JourneyFocus::Personal:JourneyFocus::Main;
 w.optional=w.focus==JourneyFocus::Personal||w.focus==JourneyFocus::Optional;
 if(goal.step==JourneyStep::Home)return w;
 w.available=true;
 auto point=[&](double dx,double dy,bool precise,const char* message){
  w.x=city.x+dx;w.y=city.y+dy;w.precise=precise;w.cue=message;
 };
 auto dungeon=[&](int room,double dx,double dy,bool precise,const char* message){
  const auto pos=DungeonRoom(a,room);
  point(pos.x+dx,15000+pos.y+dy,precise,message);
 };
 switch(goal.step){
 case JourneyStep::Board:
  point(700,-700,true,"Read the real city adventure board; the unknown route is not mapped yet.");break;
 case JourneyStep::Frontier:
  point(0,8500,true,"Follow the documented northbound road to the frontier witness marker.");break;
 case JourneyStep::Chamber:
  if(goal.index==0)
   dungeon(0,0,-650,true,"The first dungeon entrance is known from the frontier. Enter it in person.");
  else
   dungeon(goal.index-1,0,0,false,"Return to the last entered chamber and search onward. The NEXT chamber remains undiscovered.");
  break;
 case JourneyStep::Resolution:
  dungeon(4,650,0,true,"Inspect the holding-chamber council, then make the real rescue or evidence choice.");break;
 case JourneyStep::Boss:
  dungeon(5,0,-650,true,"Enter the known arena. Fight, use earned counters, or present full evidence to the living boss.");break;
 case JourneyStep::Return:
  point(650,8350,true,"Follow the opened outer return walk and let the frontier leader witness your return.");break;
 case JourneyStep::Reward:
  point(650,8350,true,"Claim the already-earned weapon or armor at the documented frontier equipment bench.");break;
 case JourneyStep::PersonalOffer:
  point(0,8000,false,"Ask for the leader's optional personal account near the documented frontier; their live position may change.");break;
 case JourneyStep::Witness:
  point(-3500,11500,false,"Seek the named local witness in the refuge district. No moving person is tracked remotely.");break;
 case JourneyStep::PersonalEvidence:
  dungeon(3,750,-550,true,"Return to the visited fourth chamber and inspect the specific physical evidence.");break;
 case JourneyStep::Accord:
  point(0,8000,false,"Find the leader in person to compare and deliberately confirm both optional public accord outcomes.");break;
 case JourneyStep::WingOffer:
  dungeon(WingAnchor(a),0,0,false,"The wing is optional. Search from its last documented anchor chamber for the eastward branch.");break;
 case JourneyStep::WingFieldwork:{
  const int previous=goal.index>0?goal.index-1:0;
  if(goal.index==0)
   dungeon(WingAnchor(a),0,0,false,"Enter the optional wing from its documented anchor chamber. Its first station is not yet mapped.");
  else{
   const auto last=WingRoom(a,previous);
   point(last.x+450,15000+last.y-450,false,
    "Return to the last completed optional fieldwork station, then explore the next passage.");
  }
  break;
 }
 case JourneyStep::WingGuardian:{
  const auto last=WingRoom(a,2);
  point(last.x+450,15000+last.y-450,false,
    "From the last finished optional wing station, search for its guardian; its arena is not revealed.");
  break;
 }
 case JourneyStep::WingReward:{
  const auto last=WingRoom(a,2);
  point(last.x-600,15000+last.y+600,true,
    "The optional guardian is resolved. Claim the earned reward at the now-known final wing site.");
  break;
 }
 case JourneyStep::Home:break;
 }
 return w;
}
inline int ExpeditionEnemyRoom(int slot){
 if(slot==11||slot==12)return 5; // transient boss support, never a durable kill slot
 return slot>=0&&slot<9?(slot<4?slot:slot<8?4:5):-1;
}
inline bool ExpeditionEncounterActive(const ExpeditionState& e,int slot){
 const int room=ExpeditionEnemyRoom(slot);
 return room>=0&&e.brief&&(e.rooms&(1<<room))&&!e.boss&&
        (slot==8||slot==11||slot==12
         ?e.branch!=0:!(e.kills&(1<<slot)));
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
