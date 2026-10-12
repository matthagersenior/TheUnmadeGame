#pragma once
#include <algorithm>
#include <cmath>
namespace UnmadeCore {
enum class CombatStatus {None,Slow,Burn,Weaken,Expose};
struct WeaponStyle {double reach=240,cooldown=.75,cost=15,bonus=1;CombatStatus status=CombatStatus::None;const char* name="Balanced blade";};
inline WeaponStyle RealmWeapon(int a){constexpr WeaponStyle s[]={
 {300,.6,14,1,CombatStatus::Expose,"Mileglass duelist"},{380,.85,18,1,CombatStatus::Slow,"Breakwater spear"},
 {250,1.15,24,1.3,CombatStatus::Burn,"Kiln hammer"},{320,.75,17,1,CombatStatus::Weaken,"Undertide hook"},
 {420,.95,20,1.1,CombatStatus::Expose,"Downfall glaive"},{260,.9,21,1.15,CombatStatus::Expose,"Seam pickblade"},
 {235,.45,11,.85,CombatStatus::Slow,"Elseborn saber"},{290,.8,18,1.05,CombatStatus::Weaken,"Crownroot falchion"},
 {300,.7,16,1,CombatStatus::Expose,"Nearness blade"}};return a>=0&&a<9?s[a]:WeaponStyle{};}
struct StyleSwing {bool started=false;int combo=0;double multiplier=1;};
class CombatTempo {
 double energy=100,last=-1,ready=0,comboUntil=0,dodgeUntil=0,parryUntil=0,powerReady=0,guardReady=0;int combo=0;
public:
 bool RestoreEnergy(double value){if(!std::isfinite(value)||value<0||value>100)return false;Reset();energy=value;return true;}
 double Stamina()const{return energy;}
 void Reset(){*this=CombatTempo{};}
 void Tick(double now,bool guarding,int training){if(!std::isfinite(now)||now<0||(last>=0&&now<last))return;double dt=last<0?0:std::min(2.,now-last);last=now;energy=std::clamp(energy+dt*(guarding?-9.:14.+std::clamp(training,0,3)*2),0.,100.);}
 bool Spend(double n){if(!std::isfinite(n)||n<=0||energy<n)return false;energy-=n;return true;}
 StyleSwing Attack(double now,const WeaponStyle& style,bool guarding){if(!std::isfinite(now)||now<0||guarding||now<ready||!std::isfinite(style.cost)||!std::isfinite(style.cooldown)||style.cooldown<=0||!Spend(style.cost))return {};combo=now<=comboUntil?(combo%3)+1:1;comboUntil=now+1.6;ready=now+style.cooldown;return {true,combo,style.bonus*(combo==3?1.35:1.)};}
 bool Dodge(double now,int training=0){if(!std::isfinite(now)||now<0||now<ready||!Spend(22))return false;ready=now+.6;dodgeUntil=now+.32+std::clamp(training,0,3)*.035;combo=0;return true;}
 bool Guard(double now,int training=0){if(!std::isfinite(now)||now<0||now<guardReady||energy<5)return false;guardReady=now+.45;parryUntil=now+.2+std::clamp(training,0,3)*.025;return true;}
 int Defense(double now,bool guard){if(!std::isfinite(now)||now<0)return 0;if(now<dodgeUntil)return 1;if(guard&&now<parryUntil&&Spend(5)){parryUntil=0;return 2;}return 0;}
 void GrantEvasion(double now,double seconds){if(std::isfinite(now)&&std::isfinite(seconds)&&now>=0&&seconds>0)dodgeUntil=std::max(dodgeUntil,now+std::min(seconds,2.));}
 bool Power(double now){if(!std::isfinite(now)||now<0||now<powerReady||!Spend(30))return false;powerReady=now+12;return true;}
};
}
