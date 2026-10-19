#pragma once
// Finite physical cargo, dated local testimony and personal home returns.
// No currency/fuel/quest rewards, remote transport or main campaign state.
#include "World/UnmadeRememberedRoadRegistry.h"
#include <vector>
#include <cmath>
namespace UnmadeCore {
struct RoadCityState { int briefDay=0,clues=0,resolution=0,route=0,departDay=0,arrivalDay=0,reportDay=0,returnDay=0,stock=4,returnRoute=0; };
struct RoadPersonState { int knowledge=0,toldDay=0,homeDay=0,homeKnowledge=0,furnishings=0; };
struct RoadSnapshot { std::array<RoadCityState,RoadCityCount> cities{}; std::array<RoadPersonState,RoadPersonCount> people{}; int cargoCity=-1,cargoStep=0,cargoDay=0; };
class RememberedRoads final {
public:
 static constexpr int MaxDay=833334;
 RoadSnapshot Save()const noexcept{return state_;}
 const RoadCityState* City(int i)const noexcept{return ValidCity(i)?&state_.cities[i]:nullptr;}
 const RoadPersonState* Person(int i)const noexcept{return i>=0&&i<RoadPersonCount?&state_.people[i]:nullptr;}
 bool Brief(int i,int day)noexcept{if(!ValidCity(i)||!Day(day)||state_.cities[i].briefDay)return false;state_.cities[i].briefDay=day;return true;}
 bool Inspect(int i,int clue,int day)noexcept{if(!ValidCity(i)||clue<0||clue>2||!Day(day))return false;
  auto& s=state_.cities[i];if(s.clues&(1<<clue))return false;s.clues|=1<<clue;return true;}
 bool Resolve(int i,int result,bool actualActor,bool defeated,double now)noexcept{
  if(!ValidCity(i)||!actualActor||state_.cities[i].resolution||result<1||result>2)return false;
  if(result==1&&!defeated)return false;
  if(result==2&&(defeated||state_.cities[i].clues!=7||!Cooling(i,now)))return false;
  state_.cities[i].resolution=result;return true;
 }
 bool Choose(int i,int route)noexcept{if(!Ready(i)||route<1||route>3||state_.cities[i].route)return false;state_.cities[i].route=route;return true;}
 // Only Cinderhold's saved and physically earned accord may be inherited.
 // Ordinary cargo and Rewrite controls cannot issue this fourth route.
 bool InheritCinderholdAccord(int i,bool documented)noexcept{
  if(i!=4||!documented||!Ready(i)||state_.cities[i].route||state_.cities[i].resolution!=2)return false;
  state_.cities[i].route=4;return true;
 }
 bool Maintain(int i,double now)noexcept{
  if(!Ready(i)||state_.cities[i].route||!Time(now)||now<nextTurn_[i])return false;
  nextTurn_[i]=now+2;return ++turns_[i]>=3?Choose(i,1):false;
 }
 int Turns(int i)const noexcept{return ValidCity(i)?turns_[i]:0;}
 bool Pickup(int i,int day)noexcept{
  if(!ValidCity(i)||!Day(day)||state_.cargoCity!=-1)return false;
  auto& s=state_.cities[i];
  if(!s.route||!s.briefDay||day<s.briefDay||s.departDay||s.stock<1)return false;
  --s.stock;s.departDay=day;state_.cargoCity=i;state_.cargoStep=1;state_.cargoDay=day;return true;
 }
 bool Checkpoint(int i,int day)noexcept{
  if(!ValidCity(i)||!Day(day)||state_.cargoCity!=i||state_.cargoStep!=1||day<state_.cargoDay)return false;
  state_.cargoStep=2;return true;
 }
 bool Deliver(int destination,int day)noexcept{
  if(!ValidCity(destination)||!Day(day)||state_.cargoCity<0||state_.cargoStep!=2||day<state_.cargoDay)return false;
  const int origin=state_.cargoCity;if(RoadCities[origin].destination!=destination)return false;
  ++state_.cities[destination].stock;state_.cities[origin].arrivalDay=day;state_.cargoCity=-1;state_.cargoStep=0;state_.cargoDay=0;return true;
 }
 bool Report(int city,int day)noexcept{if(!ValidCity(city)||!Day(day))return false;
  auto& s=state_.cities[city];if(!s.arrivalDay||day<s.arrivalDay||s.reportDay)return false;s.reportDay=day;return true;}
 bool Return(int city,int day)noexcept{if(!ValidCity(city)||!Day(day))return false;
  auto& s=state_.cities[city];if(!s.reportDay||day<s.reportDay||s.returnDay)return false;s.returnDay=day;s.returnRoute=s.route;return true;}
 bool Tell(int person,int city,int day)noexcept{
  if(person<0||person>=RoadPersonCount||!ValidCity(city)||!Day(day))return false;
  const auto& s=state_.cities[city];
  auto& p=state_.people[person];
  if(!s.arrivalDay||day<s.arrivalDay||day<p.toldDay||p.knowledge&(1<<city))return false;
  // Only origin or physically receiving city may acquire this delivery report.
  if(RoadPeople[person].city!=city&&RoadPeople[person].city!=RoadCities[city].destination)return false;
  p.knowledge|=1<<city;p.toldDay=day;return true;
 }
 bool VisitHome(int person,int day)noexcept{
  if(person<0||person>=RoadPersonCount||!Day(day))return false;
  auto& p=state_.people[person];
  if(!p.knowledge||p.homeDay||day<p.toldDay)return false;
  p.homeDay=day;p.homeKnowledge=p.knowledge;return true;
 }
 bool InspectFurniture(int person,int item)noexcept{
  if(person<0||person>=RoadPersonCount||item<0||item>3)return false;
  auto& p=state_.people[person];if(p.furnishings&(1<<item))return false;p.furnishings|=1<<item;return true;
 }
 bool Cool(int i,double now)noexcept{if(!ValidCity(i)||state_.cities[i].clues!=7||!Time(now))return false;cool_[i]=now+(FoldActive(i,now)?18:12);return true;}
 bool Cooling(int i,double now)const noexcept{return ValidCity(i)&&Time(now)&&now<cool_[i];}
 bool Glimpse(int i,double now)noexcept{if(!ValidCity(i)||!Time(now)||now<glimpse_[i])return false;glimpse_[i]=now+3;return true;}
 bool Fold(int i,double now)noexcept{if(!ValidCity(i)||!Time(now)||FoldActive(i,now))return false;fold_[i]=now+6;if(Cooling(i,now))cool_[i]=now+18;return true;}
 bool FoldActive(int i,double now)const noexcept{return ValidCity(i)&&Time(now)&&now<fold_[i];}
 bool LaneOpen(int i,bool daylight)const noexcept{return ValidCity(i)&&(state_.cities[i].route==2||state_.cities[i].route==4||(state_.cities[i].route==1&&daylight));}
 bool RecessOpen(int i)const noexcept{return ValidCity(i)&&state_.cities[i].route!=2;}
 int TotalUnits()const noexcept{int n=state_.cargoCity>=0?1:0;for(const auto& c:state_.cities)n+=c.stock;return n;}
 std::vector<int> Pack()const{
  std::vector<int> v{1};for(const auto& c:state_.cities)for(int x:{c.briefDay,c.clues,c.resolution,c.route,c.departDay,c.arrivalDay,c.reportDay,c.returnDay,c.stock,c.returnRoute})v.push_back(x);
  for(const auto& p:state_.people)for(int x:{p.knowledge,p.toldDay,p.homeDay,p.homeKnowledge,p.furnishings})v.push_back(x);
  for(int x:{state_.cargoCity,state_.cargoStep,state_.cargoDay})v.push_back(x);
  return v;
 }
 bool Restore(const std::vector<int>& v,int day)noexcept{
  if(!Day(day)||v.size()!=1+RoadCityCount*10+RoadPersonCount*5+3||v[0]!=1)return false;
  RoadSnapshot s;int k=1;for(auto& c:s.cities){c={v[k],v[k+1],v[k+2],v[k+3],v[k+4],v[k+5],v[k+6],v[k+7],v[k+8],v[k+9]};k+=10;}
  for(auto& p:s.people){p={v[k],v[k+1],v[k+2],v[k+3],v[k+4]};k+=5;}s.cargoCity=v[k];s.cargoStep=v[k+1];s.cargoDay=v[k+2];
  if(s.cargoCity < -1||s.cargoCity>=RoadCityCount||s.cargoStep<0||s.cargoStep>2||s.cargoDay<0||s.cargoDay>day)return false;
  if((s.cargoCity==-1)!=(s.cargoStep==0)||((s.cargoCity==-1)!=(s.cargoDay==0)))return false;
  int pending=0;
  for(int i=0;i<RoadCityCount;++i){const auto& c=s.cities[i];
   for(int d:{c.briefDay,c.departDay,c.arrivalDay,c.reportDay,c.returnDay})if(d<0||d>day)return false;
   if(c.clues<0||c.clues>7||c.resolution<0||c.resolution>2||c.route<0||c.route>(i==4?4:3)||c.returnRoute<0||c.returnRoute>(i==4?4:3))return false;
   if(c.resolution==2&&c.clues!=7)return false;
   if((c.route==4||c.returnRoute==4)&&(i!=4||c.resolution!=2))return false;
   if(c.route&&(!c.briefDay||c.clues!=7||!c.resolution))return false;
   if(c.departDay&&(!c.route||c.departDay<c.briefDay))return false;
   if(c.arrivalDay&&(!c.departDay||c.arrivalDay<c.departDay))return false;
   if(c.reportDay&&(!c.arrivalDay||c.reportDay<c.arrivalDay))return false;
   if(c.returnDay&&(!c.reportDay||c.returnDay<c.reportDay||c.returnRoute!=c.route))return false;
   if(!c.returnDay&&c.returnRoute)return false;
   if(c.departDay&&!c.arrivalDay){++pending;if(s.cargoCity!=i||s.cargoDay!=c.departDay)return false;}
   int expected=4-(c.departDay?1:0);for(int j=0;j<RoadCityCount;++j)if(RoadCities[j].destination==i&&s.cities[j].arrivalDay)++expected;
   if(c.stock!=expected)return false;
  }
  if(pending!=(s.cargoCity>=0?1:0))return false;
  for(int i=0;i<RoadPersonCount;++i){const auto& p=s.people[i];
   if(p.furnishings<0||p.furnishings>15||p.knowledge<0||p.knowledge>2047||p.toldDay<0||p.toldDay>day||p.homeDay<0||p.homeDay>day)return false;
   if((p.knowledge==0)!=(p.toldDay==0)||p.homeKnowledge<0||(p.homeKnowledge&~p.knowledge)||(p.homeDay==0)!=(p.homeKnowledge==0))return false;
   for(int j=0;j<RoadCityCount;++j)if(p.knowledge&(1<<j)){
    if((p.homeKnowledge&(1<<j))&&p.homeDay<s.cities[j].arrivalDay)return false;
    if(!s.cities[j].arrivalDay||p.toldDay<s.cities[j].arrivalDay||(RoadPeople[i].city!=j&&RoadPeople[i].city!=RoadCities[j].destination))return false;
   }
  }
  state_=s;cool_.fill(0);fold_.fill(0);glimpse_.fill(0);nextTurn_.fill(0);turns_.fill(0);return true;
 }
private:
 static bool ValidCity(int i)noexcept{return i>=0&&i<RoadCityCount;}
 static bool Day(int i)noexcept{return i>=1&&i<=MaxDay;}
 static bool Time(double t)noexcept{return std::isfinite(t)&&t>=0;}
 bool Ready(int i)const noexcept{return ValidCity(i)&&state_.cities[i].briefDay&&state_.cities[i].clues==7&&state_.cities[i].resolution;}
 RoadSnapshot state_{};
 std::array<double,RoadCityCount> cool_{},fold_{},glimpse_{},nextTurn_{};
 std::array<int,RoadCityCount> turns_{};
};
}
