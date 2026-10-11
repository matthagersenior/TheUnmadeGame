#pragma once
#include "World/UnmadeExpeditionRules.h"
#include <algorithm>
namespace UnmadeCore {
struct IdentityArc{int puzzle=0,repair=0,hunt=0,chart=0,restDay=0;};
// Stable optional field. Rewards are services, a one-time recovery, and a local map.
class WorldIdentity {
 std::array<IdentityArc,9> s{};
public:
 const IdentityArc* Arc(int a)const{return a>=0&&a<9?&s[a]:nullptr;}
 static int Target(int a){constexpr int t[]={7,5,3,6,1,4,2,7,5};return a>=0&&a<9?t[a]:0;}
 static int Coupling(int a,int lever){  constexpr int coupling[9][3]={{1,2,4},{3,6,4},{1,3,6},{5,2,6},{7,3,1},{4,6,3},{2,5,7},{3,5,4},{6,7,2}};
return a>=0&&a<9&&lever>=0&&lever<3?coupling[a][lever]:0;}
 bool Operate(int a,int lever){if(!Arc(a)||lever<0||lever>2)return false;auto& t=s[a];if(t.puzzle==Target(a))return false;
  // Three coupled valves: readable target pattern; incorrect choices remain reversible.
  t.puzzle^=Coupling(a,lever);return true;}
 bool Repair(int a,int post,bool returned){if(!Arc(a)||post<0||post>2||!returned||(s[a].repair&(1<<post)))return false;s[a].repair|=1<<post;return true;}
 bool Hunt(int a,int kills){if(!Arc(a)||s[a].hunt||kills<4)return false;s[a].hunt=1;return true;}
 bool Chart(int a,int clues){if(!Arc(a)||s[a].chart||clues!=7||s[a].puzzle!=Target(a))return false;s[a].chart=1;return true;}
 bool Rest(int a,int day,bool returned){if(!Arc(a)||day<1||day>1000000||!returned||s[a].repair!=7||day<=s[a].restDay)return false;s[a].restDay=day;return true;}
 std::vector<int> Pack()const{std::vector<int> p{1};for(auto& a:s)for(int v:{a.puzzle,a.repair,a.hunt,a.chart,a.restDay})p.push_back(v);return p;}
 bool Restore(const std::vector<int>& p,const Expeditions& e,int day){if(p.size()!=46||p[0]!=1||day<1||day>1000000)return false;std::array<IdentityArc,9> n{};int j=1;
 for(int a=0;a<9;++a){auto& t=n[a];t={p[j],p[j+1],p[j+2],p[j+3],p[j+4]};j+=5;auto* v=e.Arc(a);int kills=0;for(int k=0;k<8;++k)kills+=(v->kills>>k)&1;
 if(t.puzzle<0||t.puzzle>7||t.repair<0||t.repair>7||t.hunt<0||t.hunt>1||t.chart<0||t.chart>1||t.restDay<0||t.restDay>day||(t.puzzle&&!(v->rooms&4))||(t.repair&&!v->returned)||(t.hunt&&kills<4)||(t.chart&&(v->clues!=7||t.puzzle!=Target(a)))||(t.restDay&&(t.repair!=7||!v->returned)))return false;}
 s=n;return true;}
};
// Signature footprints use locked world-space aim; leave the displayed safe shape or cover.
inline bool SignatureHit(int shape,double x,double y,double radius){if(!std::isfinite(x)||!std::isfinite(y)||radius<=0)return false;double d=std::hypot(x,y);
 switch(shape){case 1:return d<=radius&&d>=radius*.55;case 2:return std::abs(x)<=radius*.28&&std::abs(y)<=radius;case 3:return d<=radius&&y>=0;case 4:return d<=radius&&((std::abs(x)<=radius*.2&&std::abs(y)<=radius)||(std::abs(y)<=radius*.2&&std::abs(x)<=radius));case 5:return d<=radius&&x*y>=0;case 6:return d<=radius&&(x<=-radius*.25||x>=radius*.25);case 7:return d<=radius&&(y<=-radius*.25||y>=radius*.25);case 8:return std::abs(x)<=radius&&std::abs(y)<=radius;case 9:return d<=radius&&d>=radius*.75;default:return d<=radius;}
}
}
