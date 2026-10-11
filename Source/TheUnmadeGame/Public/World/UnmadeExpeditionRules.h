#pragma once
#include <array>
#include <vector>
#include <cmath>
namespace UnmadeCore {
struct ExpeditionState {int brief=0,visited=0,rooms=0,clues=0,rescued=0,kills=0,branch=0,boss=0,returned=0,reward=0;};
struct ExpeditionSave {std::array<ExpeditionState,9> arcs{};int gear=0,technique=0;};
struct ExpeditionBenefit {int attack=0,armor=0,glimpseDiscount=0,foldSeconds=0;};
class Expeditions {
 ExpeditionSave s;
 bool Valid(int a)const{return a>=0&&a<9;}
public:
 const ExpeditionSave& Save()const{return s;}
 const ExpeditionState* Arc(int a)const{return Valid(a)?&s.arcs[a]:nullptr;}
 bool Brief(int a){if(!Valid(a)||s.arcs[a].brief)return false;s.arcs[a].brief=1;return true;}
 bool Visit(int a,int site){if(!Valid(a)||site<0||site>1||!s.arcs[a].brief||(s.arcs[a].visited&(1<<site)))return false;s.arcs[a].visited|=1<<site;return true;}
 bool Enter(int a,int room){if(!Valid(a)||room<0||room>5)return false;auto& t=s.arcs[a];if(!(t.visited&1)||(room>0&&!(t.rooms&(1<<(room-1))))||(room==5&&!t.branch)||(t.rooms&(1<<room)))return false;t.rooms|=1<<room;return true;}
 bool Inspect(int a,int clue){if(!Valid(a)||clue<0||clue>2)return false;auto& t=s.arcs[a];if(!(t.rooms&(1<<(clue+1)))||(t.clues&(1<<clue)))return false;t.clues|=1<<clue;return true;}
 bool Kill(int a,int enemy){if(!Valid(a)||enemy<0||enemy>7)return false;auto& t=s.arcs[a];const int room=enemy<4?enemy:4;if(!(t.rooms&(1<<room))||(t.kills&(1<<enemy)))return false;t.kills|=1<<enemy;return true;}
 bool Rescue(int a,int who){if(!Valid(a)||who<0||who>1)return false;auto& t=s.arcs[a];if(!(t.rooms&16)||(!(t.kills&(1<<(4+who)))&&!t.boss)||(t.rescued&(1<<who)))return false;t.rescued|=1<<who;return true;}
 bool Choose(int a,int branch){if(!Valid(a)||branch<1||branch>2)return false;auto& t=s.arcs[a];if(t.branch||!(t.rooms&16)||(branch==1?t.rescued!=3:t.clues!=7))return false;t.branch=branch;return true;}
 bool Boss(int a,bool defeated){if(!Valid(a))return false;auto& t=s.arcs[a];if(t.boss||!t.branch||!(t.rooms&32)||(!defeated&&(t.branch!=2||t.clues!=7)))return false;t.boss=defeated?1:2;return true;}
 bool Return(int a){if(!Valid(a))return false;auto& t=s.arcs[a];if(!t.boss||t.returned)return false;t.returned=1;return true;}
 bool Claim(int a,int type){if(!Valid(a)||type<1||type>2)return false;auto& t=s.arcs[a];if(!t.returned||t.reward)return false;t.reward=type;s.gear=a*2+type;s.technique=a+1;return true;}
 bool Equip(int a){if(!Valid(a)||!s.arcs[a].reward)return false;s.gear=a*2+s.arcs[a].reward;s.technique=a+1;return true;}
 ExpeditionBenefit Benefit()const{ExpeditionBenefit b;if(s.gear){int a=(s.gear-1)/2;bool blade=s.gear%2==1;b.attack=blade?7+a*2:0;b.armor=blade?0:5+a;}if(s.technique){constexpr int A[]={3,0,2,1,0,2,0,1,0},D[]={0,2,2,1,1,0,2,3,3},G[]={3,2,0,1,0,2,3,0,1},F[]={0,0,1,1,2,0,0,1,2};int i=s.technique-1;b.attack+=A[i];b.armor+=D[i];b.glimpseDiscount=G[i];b.foldSeconds=F[i];}return b;}
 std::vector<int> Pack()const{std::vector<int> p{1};for(auto& t:s.arcs)for(int v:{t.brief,t.visited,t.rooms,t.clues,t.rescued,t.kills,t.branch,t.boss,t.returned,t.reward})p.push_back(v);p.push_back(s.gear);p.push_back(s.technique);return p;}
 bool Restore(const std::vector<int>& p){if(p.size()!=93||p[0]!=1)return false;ExpeditionSave n;int j=1;for(auto& t:n.arcs){int* fs[]={&t.brief,&t.visited,&t.rooms,&t.clues,&t.rescued,&t.kills,&t.branch,&t.boss,&t.returned,&t.reward};for(auto f:fs)*f=p[j++];if(t.brief<0||t.brief>1||t.visited<0||t.visited>3||t.rooms<0||t.rooms>63||t.clues<0||t.clues>7||t.rescued<0||t.rescued>3||t.kills<0||t.kills>255||t.branch<0||t.branch>2||t.boss<0||t.boss>2||t.returned<0||t.returned>1||t.reward<0||t.reward>2)return false;
 if(!t.brief&&(t.visited||t.rooms||t.clues||t.rescued||t.kills||t.branch||t.boss||t.returned||t.reward))return false;
 if(t.rooms&&!(t.visited&1))return false;
 for(int r=1;r<6;++r)if((t.rooms&(1<<r))&&!(t.rooms&(1<<(r-1))))return false;
 for(int c=0;c<3;++c)if((t.clues&(1<<c))&&!(t.rooms&(1<<(c+1))))return false;
 for(int k=0;k<8;++k)if((t.kills&(1<<k))&&!(t.rooms&(1<<(k<4?k:4))))return false;
 for(int q=0;q<2;++q)if((t.rescued&(1<<q))&&(!(t.kills&(1<<(4+q)))&&!t.boss))return false;
 if((t.branch==1&&t.rescued!=3)||(t.branch==2&&t.clues!=7)||(t.branch&&!(t.rooms&16))||(t.rooms&32&&!t.branch)||(t.boss&&!(t.rooms&32))||(t.boss==2&&t.branch!=2)||(t.returned&&!t.boss)||(t.reward&&!t.returned))return false;}
 n.gear=p[j++];n.technique=p[j++];if(n.gear<0||n.gear>18||n.technique<0||n.technique>9)return false;
 if(n.gear&&n.arcs[(n.gear-1)/2].reward!=(n.gear%2?1:2))return false;
 if(n.technique&&!n.arcs[n.technique-1].reward)return false;
 if((n.gear==0)!=(n.technique==0)|| (n.gear&&((n.gear-1)/2!=n.technique-1)))return false;
 bool any=false;for(auto& t:n.arcs)any|=t.reward!=0;if(any!=(n.gear!=0))return false;s=n;return true;}
};
struct DungeonHazard {bool warning=false,danger=false;unsigned long long pulse=0;};
inline DungeonHazard SampleDungeonHazard(double now,bool disabled){if(disabled||!std::isfinite(now)||now<0||now>1e12)return {};double phase=std::fmod(now,9.0);return {phase<2,phase>=2&&phase<3,static_cast<unsigned long long>(now/9)+1};}
enum class ExpeditionRole {Sentinel,Skirmisher,Marksman,Disruptor,Boss};
struct ExpeditionAttack {double reach,windup,recovery,radius,damage;const char* warning;};
inline ExpeditionAttack ExpeditionMove(ExpeditionRole role,int realm,int turn,bool phase){
 if(role==ExpeditionRole::Sentinel)return {280,1.25,1.7,160,18,"SHIELD BREAK - STEP BEHIND"};
 if(role==ExpeditionRole::Skirmisher)return {230,.8,1.2,110,13,"LUNGE - SIDESTEP"};
 if(role==ExpeditionRole::Marksman)return {950,1.6,2.4,130,16,"AIMED SHOT - COVER OR MOVE"};
 if(role==ExpeditionRole::Disruptor)return {650,2.0,2.8,240,11,"FRACTURE PULSE - LEAVE THE MARK"};
 int move=(turn+realm)%3;if(move==0)return {420,1.5,2.5,190,26,"BOSS CRUSH - CLEAR THE MARK"};if(move==1)return {500,phase?.85:1.4,2,270,22,"BOSS SWEEP - WIDE MARK"};return {1100,1.8,2.8,140,20,"BOSS SEAL - BREAK LINE OF SIGHT"};
}
struct ExpeditionTick {bool started=false,strike=false,recovering=false;ExpeditionAttack move{};};
class ExpeditionPattern {double end=-1,ready=0,last=-1;int turn=0;ExpeditionAttack locked{};
public:ExpeditionTick Step(double now,ExpeditionRole role,int realm,bool phase,bool sight,double distance,bool exposed){ExpeditionTick r;if(!std::isfinite(now)||now<0||now<last||!std::isfinite(distance)||distance<0)return r;last=now;r.move=end>=0?locked:ExpeditionMove(role,realm,turn,phase);if(!sight||exposed||distance>r.move.reach){end=-1;ready=std::max(ready,now+.5);r.recovering=exposed;return r;}if(end>=0){if(now>=end){r.strike=true;end=-1;ready=now+r.move.recovery;++turn;}return r;}if(now<ready){r.recovering=true;return r;}locked=r.move;end=now+r.move.windup;r.started=true;return r;}
};
}
