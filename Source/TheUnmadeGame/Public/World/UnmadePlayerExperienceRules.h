#pragma once
#include "World/UnmadeAdventureDepthRules.h"
#include "World/UnmadeAdventureLayout.h"
#include "World/UnmadePlayerExperienceRegistry.h"
namespace UnmadeCore {
inline int WingAnchor(int a){if(a<0||a>8)return -1;int n=0;for(int r=1;r<6;++r)if(DungeonRoom(a,r).x>DungeonRoom(a,n).x)n=r;return n;}
inline DungeonPoint WingRoom(int a,int r){if(a<0||a>8||r<0||r>2)return {0,0};auto& p=ExperienceSpecs[a].wing_grid[r];return {p[0]*2600,p[1]*2600};}

inline bool MapKnowsCity(const ExpeditionState& s){return s.brief||s.visited;}
inline bool MapKnowsRoom(const ExpeditionState& s,int room){return room>=0&&room<6&&(s.rooms&(1<<room));}
inline bool MapKnowsChoice(const DepthArc& s){return s.offered&&s.witness;}
inline bool ServiceOpen(int realm,int hour){return realm>=0&&realm<9&&hour>=0&&hour<24&&(realm==2||(realm==6?(hour>=18||hour<6):hour>=6&&hour<22));}
struct ServicePoint {double x,y;};
inline ServicePoint ServiceApproach(double x,double y,double gx,double gy,double hx,double hy){bool inside=std::abs(x-hx)<680&&y>hy-550&&y<hy+1650;if(inside)return {gx,gy};double front=hy-800;if(y<=front+80){if(std::abs(x-hx)>80)return {hx,front};return {hx,hy-400};}if(y<=hy-550&&std::abs(x-hx)<200)return {hx,hy-400};int side=x>=hx?1:-1;if(std::abs(x-hx)<900)return {hx+side*1000,y};return {hx+side*1000,front};}

struct BuildEffect {int attack=0,armor=0;double cost=1,speed=1,reach=1;};
inline BuildEffect BuildEffects(int n){return n==1?BuildEffect{5,-2,1.2,1,1}:n==2?BuildEffect{-2,5,1,.9,1}:n==3?BuildEffect{0,0,1.1,1.08,1.1}:BuildEffect{};}
struct ExperienceArc {
 int serviceDay=0,serviceUses=0;
 int offered=0,steps=0,guardian=0,complete=0,build=0,tool=0,recipe=0,privilege=0,insight=0,spent=0,repair=0,reconciled=0,scenes=0;
 std::array<int,5> visits{},days{};
};
class PlayerExperience {
 std::array<ExperienceArc,9> arcs{};int tracked=-1,tutorial=0,cityMask=0,activeBuild=-1,observerDefeated=0;
public:
 const ExperienceArc* Arc(int a)const{return a>=0&&a<9?&arcs[a]:nullptr;}
 bool CityKnown(int a)const{return a>=0&&a<9&&(cityMask&(1<<a));}
 bool DiscoverCity(int a,bool physical){if(a<0||a>=9||!physical||CityKnown(a))return false;cityMask|=1<<a;return true;}
 bool ObserverCleared()const{return observerDefeated!=0;}
 bool ClearObserver(int a){if(a!=6||!arcs[6].offered||observerDefeated)return false;observerDefeated=1;return true;}
 int ActiveBuild()const{return activeBuild;}
 bool SelectBuild(int a){if(!Arc(a)||(!arcs[a].build&&!arcs[a].recipe)||activeBuild==a)return false;activeBuild=a;return true;}
 int Tracked()const{return tracked;}int Tutorial()const{return tutorial;}
 bool Track(int a,const Expeditions& e){if(!Arc(a)||!(CityKnown(a)||MapKnowsCity(*e.Arc(a))))return false;tracked=a;return true;}
 bool Learn(int bit){if(bit<0||bit>5||(tutorial&(1<<bit)))return false;tutorial|=1<<bit;return true;}
 bool OfferWing(int a,const Expeditions& e){if(!Arc(a)||!e.Arc(a)->rooms||arcs[a].offered)return false;arcs[a].offered=1;return true;}
 bool WingStep(int a,int step,bool physical){if(!Arc(a)||!physical||step<0||step>2||!arcs[a].offered)return false;auto& s=arcs[a];if((s.steps&(1<<step))||(step&&!(s.steps&(1<<(step-1)))))return false;s.steps|=1<<step;return true;}
 bool RestartCooling(int a){if(a!=2||!arcs[a].offered||arcs[a].guardian||arcs[a].steps==7||!arcs[a].steps)return false;arcs[a].steps=0;return true;}
 bool Guardian(int a,bool yielded=false){if((yielded&&a!=3)||!Arc(a)||arcs[a].steps!=7||arcs[a].guardian)return false;arcs[a].guardian=yielded?2:1;arcs[a].insight+=2;return true;}
 bool CompleteWing(int a){if(!Arc(a)||!arcs[a].guardian||arcs[a].complete)return false;arcs[a].complete=1;arcs[a].insight+=3;return true;}
 bool Visit(int a,int person,int day){if(!Arc(a)||person<0||person>4||day<1||day>1000000||arcs[a].days[person]>=day)return false;auto& s=arcs[a];s.days[person]=day;s.visits[person]=std::min(5,s.visits[person]+1);return true;}
 bool Scene(int a,int scene,const Expeditions& e,const AdventureDepth& d){if(!Arc(a)||scene<0||scene>3)return false;auto& s=arcs[a];auto* v=e.Arc(a);auto* p=d.Arc(a);if(s.scenes&(1<<scene))return false;bool allowed=scene==0?s.offered&&s.visits[4]>=1:scene==1?s.guardian&&s.visits[3]>=2:scene==2?s.complete&&p->choice&&s.visits[0]>=2:v->returned&&v->rescued==3&&s.visits[1]>=1&&s.visits[2]>=1;if(!allowed)return false;s.scenes|=1<<scene;s.insight+=1;return true;}
 bool Upgrade(int a,int option){if(!Arc(a)||option<1||option>6||!arcs[a].complete)return false;auto& s=arcs[a];int cost=option<=3?(s.build?1:2):1;if(s.insight<cost)return false;if(option<=3){if(s.build==option)return false;s.build=option;activeBuild=a;}else if(option==4){if(s.tool)return false;s.tool=1;}else if(option==5){if(s.recipe)return false;s.recipe=1;activeBuild=a;}else {if(s.privilege)return false;s.privilege=1;}s.insight-=cost;s.spent+=cost;return true;}
 bool Repair(int a,const AdventureDepth& d){if(!Arc(a)||!d.Arc(a)->choice||!arcs[a].complete||arcs[a].repair||arcs[a].insight<1)return false;auto& s=arcs[a];s.repair=1;s.insight--;s.spent++;return true;}
 bool Reconcile(int a,const AdventureDepth& d){if(!Arc(a)||d.Arc(a)->choice!=2||!arcs[a].repair||arcs[a].reconciled||arcs[a].visits[3]<2)return false;arcs[a].reconciled=1;return true;}
 bool Specialty(int a,int day){if(!Arc(a)||!arcs[a].privilege||day<1||day>1000000||arcs[a].serviceDay>=day||arcs[a].serviceUses>=2)return false;arcs[a].serviceDay=day;arcs[a].serviceUses++;return true;}
 std::vector<int> Pack()const{std::vector<int> p{1,tracked,tutorial,cityMask,activeBuild,observerDefeated};for(const auto& s:arcs){for(int v:{s.offered,s.steps,s.guardian,s.complete,s.build,s.tool,s.recipe,s.privilege,s.insight,s.spent,s.repair,s.reconciled,s.scenes,s.serviceDay,s.serviceUses})p.push_back(v);for(int v:s.visits)p.push_back(v);for(int v:s.days)p.push_back(v);}return p;}
 bool Restore(const std::vector<int>& p,const Expeditions& e,const AdventureDepth& d,int day){if(p.size()!=231||p[0]!=1||p[1]<-1||p[1]>8||p[2]<0||p[2]>63||p[3]<0||p[3]>511||p[4]<-1||p[4]>8||p[5]<0||p[5]>1||day<1||day>1000000)return false;PlayerExperience n;n.tracked=p[1];n.tutorial=p[2];n.cityMask=p[3];n.activeBuild=p[4];n.observerDefeated=p[5];int j=6;
  for(int a=0;a<9;++a){auto& s=n.arcs[a];int* fields[]={&s.offered,&s.steps,&s.guardian,&s.complete,&s.build,&s.tool,&s.recipe,&s.privilege,&s.insight,&s.spent,&s.repair,&s.reconciled,&s.scenes,&s.serviceDay,&s.serviceUses};for(auto f:fields)*f=p[j++];for(auto& v:s.visits)v=p[j++];for(auto& v:s.days)v=p[j++];
   if(s.offered<0||s.offered>1||(s.steps!=0&&s.steps!=1&&s.steps!=3&&s.steps!=7)||s.guardian<0||s.guardian>2||s.complete<0||s.complete>1||s.build<0||s.build>3||s.tool<0||s.tool>1||s.recipe<0||s.recipe>1||s.privilege<0||s.privilege>1||s.insight<0||s.insight>9||s.spent<0||s.spent>1000||s.repair<0||s.repair>1||s.reconciled<0||s.reconciled>1||s.scenes<0||s.scenes>15)return false;
   if(s.serviceDay<0||s.serviceDay>day||s.serviceUses<0||s.serviceUses>2||((s.serviceUses==0)!=(s.serviceDay==0))||(s.serviceUses&&!s.privilege))return false;
   for(int k=0;k<5;++k)if(s.visits[k]<0||s.visits[k]>5||s.days[k]<0||s.days[k]>day||((s.visits[k]==0)!=(s.days[k]==0)))return false;
   if((s.offered&&!e.Arc(a)->rooms)||(s.steps&&!s.offered)||(s.guardian&&s.steps!=7)||(s.guardian==2&&a!=3)||(s.complete&&!s.guardian)||((s.build||s.tool||s.recipe||s.privilege||s.repair)&&!s.complete)||(s.repair&&!d.Arc(a)->choice)||(s.reconciled&&(!s.repair||d.Arc(a)->choice!=2||s.visits[3]<2)))return false;
   int count=0;for(int k=0;k<4;++k)if(s.scenes&(1<<k))count++;int earned=(s.guardian?2:0)+s.complete*3+count;
   int minimum=(s.build?2:0)+s.tool+s.recipe+s.privilege+s.repair;if(s.spent<minimum||s.insight+s.spent!=earned)return false;
   if((s.scenes&1)&&(!s.offered||s.visits[4]<1))return false;
   if((s.scenes&2)&&(!s.guardian||s.visits[3]<2))return false;
   if((s.scenes&4)&&(!s.complete||!d.Arc(a)->choice||s.visits[0]<2))return false;
   if((s.scenes&8)&&(!e.Arc(a)->returned||e.Arc(a)->rescued!=3||s.visits[1]<1||s.visits[2]<1))return false;
  }if(n.observerDefeated&&!n.arcs[6].offered)return false;
  if(n.activeBuild>=0&&!n.arcs[n.activeBuild].build&&!n.arcs[n.activeBuild].recipe)return false;
  if(n.tracked>=0&&!n.CityKnown(n.tracked)&&!MapKnowsCity(*e.Arc(n.tracked)))return false;
  *this=n;return true;}
};
}
