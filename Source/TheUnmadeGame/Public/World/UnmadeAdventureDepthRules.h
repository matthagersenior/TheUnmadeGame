#pragma once
#include "World/UnmadeExpeditionRules.h"
#include "World/UnmadeIdentityRules.h"
namespace UnmadeCore {
struct DepthArc {int offered=0,witness=0,evidence=0,choice=0,route=0,secret=0,favor=0,training=0,kits=0,purchases=0,tradeDay=0,counterThreat=0;};
class AdventureDepth {
 std::array<DepthArc,9> arcs{};int active=-1;
public:
 const DepthArc* Arc(int a)const{return a>=0&&a<9?&arcs[a]:nullptr;}
 int Active()const{return active;}
 bool Offer(int a,bool visited){if(!Arc(a)||!visited||arcs[a].offered)return false;arcs[a].offered=1;return true;}
 bool Witness(int a){if(!Arc(a)||!arcs[a].offered||arcs[a].witness)return false;arcs[a].witness=1;return true;}
 bool Evidence(int a,bool physical){if(!Arc(a)||!arcs[a].witness||!physical||arcs[a].evidence)return false;arcs[a].evidence=1;return true;}
 bool Choose(int a,int choice,bool bossResolved){if(!Arc(a)||choice<1||choice>2||!bossResolved||!arcs[a].evidence||arcs[a].choice)return false;auto& s=arcs[a];s.choice=choice;s.favor+=choice==1?4:3;active=a;return true;}
 bool Route(int a,bool physical){if(!Arc(a)||!physical||arcs[a].route)return false;arcs[a].route=1;arcs[a].favor+=2;return true;}
 bool Secret(int a,bool physical){if(!Arc(a)||!physical||!arcs[a].route||arcs[a].secret)return false;arcs[a].secret=1;arcs[a].favor+=2;return true;}
 bool CounterThreat(int a){if(!Arc(a)||arcs[a].choice!=2||arcs[a].counterThreat)return false;arcs[a].counterThreat=1;arcs[a].favor+=2;return true;}
 int Price(int a,int service)const{auto* s=Arc(a);if(!s)return 99;return service==0?(s->choice==1?1:2):service==2?0:2;}
 bool Buy(int a,int service,int day,bool benefit){if(!Arc(a)||!arcs[a].choice||service<0||service>2||day<1||day>1000000||!benefit)return false;auto& s=arcs[a];int cost=Price(a,service);if(s.favor<cost)return false;
 if(service==0){if(s.purchases>=4||s.tradeDay>=day)return false;s.purchases++;s.tradeDay=day;}
 if(service==1){if(s.kits>=3)return false;s.kits++;}
 if(service==2){if(s.training>=3||(!s.route&&s.training>=1)||(!s.secret&&s.training>=2))return false;s.training++;active=a;}
 s.favor-=cost;return true;}
 bool Select(int a){if(!Arc(a)||!arcs[a].choice)return false;active=a;return true;}
 bool SpendKit(int a){if(!Arc(a)||arcs[a].kits<1)return false;arcs[a].kits--;return true;}
 std::vector<int> Pack()const{std::vector<int> p{1,active};for(auto& s:arcs)for(int v:{s.offered,s.witness,s.evidence,s.choice,s.route,s.secret,s.favor,s.training,s.kits,s.purchases,s.tradeDay,s.counterThreat})p.push_back(v);return p;}
 bool Restore(const std::vector<int>& p,const Expeditions& e,int day){if(p.size()!=110||p[0]!=1||p[1]<-1||p[1]>8||day<1||day>1000000)return false;std::array<DepthArc,9> n{};int j=2;
 for(int a=0;a<9;++a){auto& s=n[a];s={p[j],p[j+1],p[j+2],p[j+3],p[j+4],p[j+5],p[j+6],p[j+7],p[j+8],p[j+9],p[j+10],p[j+11]};j+=12;auto* v=e.Arc(a);
 if(s.offered<0||s.offered>1||s.witness<0||s.witness>1||s.evidence<0||s.evidence>1||s.choice<0||s.choice>2||s.route<0||s.route>1||s.secret<0||s.secret>1||s.favor<0||s.favor>9||s.training<0||s.training>3||s.kits<0||s.kits>3||s.purchases<0||s.purchases>4||s.tradeDay<0||s.tradeDay>day||s.counterThreat<0||s.counterThreat>1||(s.counterThreat&&s.choice!=2))return false;
 if((s.offered&&!(v->visited&1))||(s.witness&&!s.offered)||(s.evidence&&(!s.witness||!(v->rooms&8)))||(s.choice&&(!s.evidence||!v->boss))||(s.route&&!(v->rooms&4))||(s.secret&&(!s.route||!(v->rooms&8)))||(s.training>1&&!s.route)||(s.training>2&&!s.secret)||((s.training||s.kits||s.purchases)&&!s.choice)||(s.purchases==0)!=(s.tradeDay==0)||s.purchases>s.tradeDay)return false;
 int earned=(s.choice==1?4:s.choice==2?3:0)+s.route*2+s.secret*2+s.counterThreat*2;int spent=s.kits*2+s.purchases*(s.choice==1?1:2);if(s.favor+spent>earned)return false;}
 if(p[1]>=0&&!n[p[1]].choice)return false;
 arcs=n;active=p[1];return true;}
};
// Kits remain charged to their supplying realm; no save identifiers change.
enum class FieldKitResult { Unavailable, Used, SaveFailed };
template<class Writer>
FieldKitResult CommitFieldKit(AdventureDepth& depth,int target,bool benefit,
                             Writer write,int& supplier){
 supplier=-1;
 if(!depth.Arc(target)||!benefit)return FieldKitResult::Unavailable;
 if(depth.Arc(target)->kits>0)supplier=target;
 else for(int a=0;a<9;++a)if(depth.Arc(a)->kits>0){supplier=a;break;}
 if(supplier<0)return FieldKitResult::Unavailable;
 const auto before=depth;
 if(!depth.SpendKit(supplier))return FieldKitResult::Unavailable;
 if(!write()){depth=before;supplier=-1;return FieldKitResult::SaveFailed;}
 return FieldKitResult::Used;
}
// Distinct challenge state is transient until a physical completion is committed.
// Realm-local challenge progress is transient until the existing route receipt saves.
// Every attempt reports *why* progress advanced or stopped. No save schema changes.
enum class ChallengeOutcome { Invalid, Progress, Reset, WaitForWindow, NeedElevation, Observed, Complete };
struct ChallengeReport { ChallengeOutcome outcome=ChallengeOutcome::Invalid; double secondsRemaining=0; };
inline const char* ChallengeInstructions(int realm){
 constexpr const char* hints[]={
  "Ring road bells 1, 3, 2. A wrong note restarts the charter signal.",
  "Keep pump switches 1 and 3 on, 2 off. Complete during tide seconds 4-9 of each 12-second cycle.",
  "Touch all three cooling relays within 18 seconds from the first relay. Expiry starts a new attempt.",
  "Float buoys 1 and 2: their weights are 2 and 3; buoy 3 weighs 4. The required load is 5; toggle a buoy to remove it.",
  "Climb the upper choir walkway before grounding all three anchors. Controls cannot be worked from below.",
  "Close yesterday shutters 1, 2, 3 in order. The wrong shutter resets the sequence.",
  "Read all three census lamps while unseen. Crouch outside the patrol gaze or clear the guards; detection resets the lamps.",
  "Regraft the public root path in order 3, 1, 2. A wrong graft resets the sequence.",
  "Light threshold lamps 2 and 3, leaving lamp 1 dark. Any lamp can be toggled again."
 };
 return realm>=0&&realm<9?hints[realm]:nullptr;
}
// The UI reads contextual outcomes without changing the physical challenge save receipt.
class RealmChallenge {
 int progress=0,mask=0;double until=-1;
public:
 int Progress()const{return progress;}
 int Mask()const{return mask;}
 static double TidePhase(double now){return std::isfinite(now)&&now>=0?std::fmod(now,12.):0;}
 double SecondsRemaining(double now)const{return std::isfinite(now)&&now>=0&&until>=now?until-now:0;}
 ChallengeReport Attempt(int realm,int control,double now,bool elevated,bool quiet){
  if(realm<0||realm>8||control<0||control>2||!std::isfinite(now)||now<0)return {};
  switch(realm){
   case 0:{constexpr int seq[]={0,2,1};bool right=control==seq[progress%3];progress=right?progress+1:control==0?1:0;return {right?(progress>=3?ChallengeOutcome::Complete:ChallengeOutcome::Progress):ChallengeOutcome::Reset,0};}
   case 1:{mask^=1<<control;if(mask!=5)return {ChallengeOutcome::Progress,0};
    double phase=TidePhase(now);return {phase>=4&&phase<9?ChallengeOutcome::Complete:ChallengeOutcome::WaitForWindow,0};}
   case 2:{bool expired=until>=0&&until<now;if(until<now){progress=0;mask=0;until=now+18;}
    mask|=1<<control;return {mask==7?ChallengeOutcome::Complete:expired?ChallengeOutcome::Reset:ChallengeOutcome::Progress,SecondsRemaining(now)};}
   case 3:{mask^=1<<control;int load=(mask&1?2:0)+(mask&2?3:0)+(mask&4?4:0);return {load==5?ChallengeOutcome::Complete:ChallengeOutcome::Progress,0};}
   case 4:{if(!elevated)return {ChallengeOutcome::NeedElevation,0};mask|=1<<control;return {mask==7?ChallengeOutcome::Complete:ChallengeOutcome::Progress,0};}
   case 5:{bool right=control==progress%3;progress=right?progress+1:0;return {right?(progress>=3?ChallengeOutcome::Complete:ChallengeOutcome::Progress):ChallengeOutcome::Reset,0};}
   case 6:{if(!quiet){progress=0;mask=0;return {ChallengeOutcome::Observed,0};}mask|=1<<control;return {mask==7?ChallengeOutcome::Complete:ChallengeOutcome::Progress,0};}
   case 7:{constexpr int seq[]={2,0,1};bool right=control==seq[progress%3];progress=right?progress+1:0;return {right?(progress>=3?ChallengeOutcome::Complete:ChallengeOutcome::Progress):ChallengeOutcome::Reset,0};}
   case 8:{mask^=1<<control;return {mask==6?ChallengeOutcome::Complete:ChallengeOutcome::Progress,0};}
  }
  return {};
 }
 // Kept for existing callers and portable contracts.
 bool Step(int realm,int control,double now,bool elevated,bool quiet){
  return Attempt(realm,control,now,elevated,quiet).outcome==ChallengeOutcome::Complete;
 }
};
}
